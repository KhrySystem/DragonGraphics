#include <dragon/graphics.hpp>
#include <format>

Dragon::Graphics::Window::Window(VkInstance instance, int width, int height, std::string title) {
    GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    if(!window) {
        throw "First GLFW Window was NULL, Graphics initialization cannot proceed.\n";
    }

    VkResult error = glfwCreateWindowSurface(instance, window, nullptr, &this->surface);
    if(error != VK_SUCCESS) {
        throw std::format("Window Surface failed with {}", std::to_string(error));
    }
}