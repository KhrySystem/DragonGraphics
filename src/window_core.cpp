#include <dragon/graphics.hpp>

#ifndef NDEBUG
#include <iostream>
#endif

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

Dragon::Swapchain Dragon::Graphics::Window::recreateSwapchain(Dragon::Device device) {
    #ifndef NDEBUG
    std::cout << "Recreating swapchain\n";
    #endif
    Dragon::SwapchainBuilder builder{device};

    builder.set_old_swapchain(this->swapchain);

    Dragon::Result<Dragon::Swapchain> result = builder.build();

    if(!result) {
        throw fmt::format("Window swapchain creation failed with {}", result.error().message());
    }

    return result.value();
}

void Dragon::Graphics::Window::update(Dragon::Graphics::Engine* parent) {
    if(this->shouldClose()) return;
}

void Dragon::Graphics::Window::close(VkInstance instance, VkDevice device) {
    vkDestroyCommandPool(device, this->commandPool, nullptr);
    for (auto framebuffer : this->framebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);
    vkb::destroy_swapchain(this->swapchain);
    vkDestroySurfaceKHR(instance, this->surface, nullptr);
}