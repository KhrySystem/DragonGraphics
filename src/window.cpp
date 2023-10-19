#include <dragon/graphics.hpp>

Dragon::Graphics::Window::Window(VkInstance instance, int width, int height, std::string title) {
    GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    if(!window) {
        throw std::string("First GLFW Window was NULL, Graphics initialization cannot proceed.\n");
    }
    this->window = window;
    
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &this->surface);
    if(result != VK_SUCCESS) {
        throw fmt::format("Window Surface failed with {}", string_VkResult(result));
    }
}

int Dragon::Graphics::Window::getWindowWidth() {
    int width, height;
    glfwGetWindowSize(this->window, &width, &height);
    return width;
}

int Dragon::Graphics::Window::getWindowHeight() {
    int width, height;
    glfwGetWindowSize(this->window, &width, &height);
    return height;
}

void Dragon::Graphics::Window::update() {
    if(this->shouldClose()) return;
}