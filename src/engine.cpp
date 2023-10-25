#include <cassert>
#include <dragon/graphics.hpp>

void Dragon::Graphics::Engine::onAddition(Dragon::Engine* parent) {
    
    if(!glfwInit()) {
        throw std::string("GLFW Initialization failed.");
    }

    if(!glfwVulkanSupported()) {
        throw std::string("Current GLFW instance does not support Vulkan");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    this->parent = parent;
}

Dragon::InstanceBuilder Dragon::Graphics::Engine::adjustInstanceParams(Dragon::Engine* parent, Dragon::InstanceBuilder &previous) {
    uint32_t count;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
    for (std::uint32_t i{}; i < count; ++i)
    {
        previous = previous.enable_extension(glfwExtensions[i]);
    }
    return previous;
}

Dragon::Graphics::Window* Dragon::Graphics::Engine::createWindow(int width, int height, std::string title) {
    Dragon::Graphics::Window* window = new Window(this->parent->getInstance(), width, height, title);
    this->windows.push_back(window);
    return window;
}

void Dragon::Graphics::Engine::beforePhysicalDeviceSelection(Dragon::Engine* parent) {
    if(this->windows.size() == 0)
        this->windows.push_back(new Dragon::Graphics::Window(parent->getInstance(), 1, 1, ""));
}

Dragon::PhysicalDeviceSelector Dragon::Graphics::Engine::adjustPhysicalDeviceParams(Dragon::Engine* parent, Dragon::PhysicalDeviceSelector &previous) {
    return previous.set_surface(this->windows.at(0)->getSurface()).add_required_extension("VK_KHR_swapchain");
}

Dragon::DeviceBuilder Dragon::Graphics::Engine::adjustDeviceParams(Dragon::Engine* parent, Dragon::DeviceBuilder &previous) {
    return previous;
}

void Dragon::Graphics::Engine::afterDeviceCreation(Dragon::Engine* parent) {
    for(Dragon::Graphics::Window* window : this->windows) {
        window->finalize(this);
    }
}

void Dragon::Graphics::Engine::update(Dragon::Engine* parent) {
    glfwPollEvents();
    for(int i = 0; i < this->windows.size(); i++) {
        Dragon::Graphics::Window* window = this->windows.at(i);
        if(!window->shouldClose()) {
            window->update(this);
        } else {
            window->close(parent->getInstance(), parent->getDevice());
            this->windows.erase(this->windows.begin() + i);
        }
    }
}