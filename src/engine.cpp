#include <cassert>
#include <dragon/graphics.hpp>

namespace Dragon {
    void Graphics::Engine::onAddition(Dragon::Engine* parent) {
        
        if(!glfwInit()) {
            throw std::string("GLFW Initialization failed.");
        }

        if(!glfwVulkanSupported()) {
            throw std::string("Current GLFW instance does not support Vulkan");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        this->parent = parent;
    }

    InstanceBuilder Graphics::Engine::adjustInstanceParams(Dragon::Engine* parent, InstanceBuilder &previous) {
        uint32_t count;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
        for (std::uint32_t i{}; i < count; ++i)
        {
            previous = previous.enable_extension(glfwExtensions[i]);
        }
        return previous;
    }

    Graphics::Window* Graphics::Engine::createWindow(int width, int height, std::string title) {
        Graphics::Window* window = new Window(this, width, height, title);
        this->windows.push_back(window);
        return window;
    }

    void Graphics::Engine::beforePhysicalDeviceSelection(Dragon::Engine* parent) {
        if(this->windows.size() == 0)
            this->windows.push_back(new Graphics::Window(this, 1, 1, ""));
    }

    PhysicalDeviceSelector Graphics::Engine::adjustPhysicalDeviceParams(Dragon::Engine* parent, PhysicalDeviceSelector &previous) {
        for(Window* window : this->windows) {
            window->afterInstanceCreation(this);
        }
        return previous.set_surface(this->windows.at(0)->getSurface()).add_required_extension("VK_KHR_swapchain");
    }

    DeviceBuilder Graphics::Engine::adjustDeviceParams(Dragon::Engine* parent, DeviceBuilder &previous) {
        return previous;
    }

    void Graphics::Engine::afterDeviceCreation(Dragon::Engine* parent) {
        Result<VkQueue> graphicsQueueResult = parent->getDevice().get_queue(vkb::QueueType::graphics);

        if(!graphicsQueueResult) {
            throw fmt::format("Failed to create Graphics queue with {}", graphicsQueueResult.error().message());
        }

        this->graphicsQueue = graphicsQueueResult.value();

        Result<VkQueue> presentQueueResult = parent->getDevice().get_queue(vkb::QueueType::present);
        if(!presentQueueResult) {
            throw fmt::format("Failed to create Present queue with {}", presentQueueResult.error().message());
        }
        this->presentQueue = presentQueueResult.value();

        for(Graphics::Window* window : this->windows) {
            window->finalize(this);
        }
    }

    void Graphics::Engine::update(Dragon::Engine* parent) {
        glfwPollEvents();
        for(int i = 0; i < this->windows.size(); i++) {
            Graphics::Window* window = this->windows.at(i);
            if(!window->shouldClose()) {
                window->update(this);
            } else {
                window->close(parent->getInstance(), parent->getDevice());
                this->windows.erase(this->windows.begin() + i);
            }
        }
    }
}