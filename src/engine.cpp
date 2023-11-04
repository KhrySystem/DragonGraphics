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

    void Graphics::Engine::adjustInstanceParams(Dragon::Engine* parent, InstanceBuilder &previous) {
        uint32_t count;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
        for(uint32_t i{}; i < count; ++i)
        {
            previous.enableExtension(glfwExtensions[i]);
        }
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

    void Graphics::Engine::adjustPhysicalDeviceParams(Dragon::Engine* parent, PhysicalDeviceBuilder &previous) {
        for(Window* window : this->windows) {
            window->afterInstanceCreation(this);
        }
        VkSurfaceKHR surf = this->windows[0]->getSurface();
        previous.setSurface(surf);
    }

    void Graphics::Engine::adjustDeviceParams(Dragon::Engine* parent, DeviceBuilder &previous) {
        previous.enableExtension("VK_KHR_swapchain");
    }

    void Graphics::Engine::afterDeviceCreation(Dragon::Engine* parent) {
        Result<Queue> graphicsQueueResult = parent->getDevice().getQueue(QueueFamilyType::GRAPHICS);

        if(!graphicsQueueResult) {
            throw fmt::format("Failed to create Graphics queue with {}", graphicsQueueResult.getError().message);
        }

        this->graphicsQueue = graphicsQueueResult.getValue();

        Result<Queue> presentQueueResult = parent->getDevice().getQueue(QueueFamilyType::PRESENT);
        if(!presentQueueResult) {
            throw fmt::format("Failed to create Present queue with {}", presentQueueResult.getError().message);
        }
        this->presentQueue = presentQueueResult.getValue();

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