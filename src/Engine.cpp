#include <cassert>
#include <dragon/graphics.hpp>

void Dragon::Graphics::Engine::beforeInstanceCreation(Dragon::Engine* parent) {
    if(!glfwInit()) {
        throw "GLFW Initialization failed.";
    }

    if(!glfwVulkanSupported()) {
        throw "Current GLFW instance does not support Vulkan";
    }
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

void Dragon::Graphics::Engine::editWindow(size_t index, uint32_t width, uint32_t height, std::string title) {
    assert(0 <= index && index < this->windows.size());
    GLFWwindow* window = this->windows.at(0);
    glfwSetWindowSize(window, width, height);
    glfwSetWindowTitle(window, title.c_str());
}

void Dragon::Graphics::Engine::beforePhysicalDeviceSelection(Dragon::Engine* parent) {
    GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    if(!window) {
        throw "First GLFW Window was NULL, Graphics initialization cannot proceed.\n";
    }

    VkResult error = glfwCreateWindowSurface(parent->instance.instance, window, nullptr, &this->surface);
}

Dragon::PhysicalDeviceSelector Dragon::Graphics::Engine::adjustPhysicalDeviceParams(Dragon::Engine* parent, Dragon::PhysicalDeviceSelector &previous) {
    return previous.set_surface(this->surface);
}

void closeWindow(VkInstance instance, VkSurfaceKHR surface, GLFWwindow* window, ) {
    vkDestroySurfaceKHR(instance, surface, nullptr);
    glfwDestroyWindow(window);
}

void Dragon::Graphics::Engine::update(Dragon::Engine* parent) {
    for(GLFWwindow* window : this->windows) {
        if(glfwWindowShouldClose(window)) {
            
        }
    }
}


void Dragon::Graphics::Engine::close(Dragon::Engine* parent) {
    for(GLFWwindow* window)
}

void Dragon::Graphics::Engine::afterClose(Dragon::Engine* parent) {
    glfwTerminate();
}