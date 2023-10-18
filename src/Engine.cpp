#include <cassert>
#include <dragon/graphics.hpp>

void Dragon::Graphics::Engine::onAddition(Dragon::Engine* parent) {
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

Dragon::Graphics::Window* Dragon::Graphics::Engine::getWindow(size_t index) {
    assert(0 <= index && index < this->windows.size());
    return this->windows.at(0);
}

void Dragon::Graphics::Engine::beforePhysicalDeviceSelection(Dragon::Engine* parent) {

    this->windows.push_back(new Dragon::Graphics::Window(parent->instance.instance, 1, 1, ""));
}

Dragon::PhysicalDeviceSelector Dragon::Graphics::Engine::adjustPhysicalDeviceParams(Dragon::Engine* parent, Dragon::PhysicalDeviceSelector &previous) {
    return previous.set_surface(this->windows.at(0)->getSurface());
}

void Dragon::Graphics::Engine::update(Dragon::Engine* parent) {
    for(int i = 0; i < this->windows.size(); i++) {
        Dragon::Graphics::Window* window = this->windows.at(i);
        if(!window->shouldClose()) {

        } else {
            this->windows.erase(this->windows.begin() + i);
        }
    }
}


void Dragon::Graphics::Engine::close(Dragon::Engine* parent) {
    //for(GLFWwindow* window)
}

void Dragon::Graphics::Engine::afterClose(Dragon::Engine* parent) {
    glfwTerminate();
}