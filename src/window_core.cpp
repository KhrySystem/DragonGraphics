#include <dragon/graphics.hpp>

#ifndef NDEBUG
#include <iostream>
#endif

void framebufferResized(GLFWwindow* window, int width, int height) {
    auto w = reinterpret_cast<Dragon::Graphics::Window*>(glfwGetWindowUserPointer(window));
    w->framebufferResized=true;
}

Dragon::Graphics::Window::Window(Dragon::Graphics::Engine* parent, int width, int height, std::string title) {
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!window) {
        throw std::string("First GLFW Window was NULL, Graphics initialization cannot proceed.\n");
    }
    this->window = window;
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, ::framebufferResized);

    if(parent->getParent()->getInstance().instance != VK_NULL_HANDLE) {
        this->afterInstanceCreation(parent);
        this->finalize(parent);
    }
}

void Dragon::Graphics::Window::afterInstanceCreation(Dragon::Graphics::Engine* parent) {
    VkResult result = glfwCreateWindowSurface(parent->getParent()->getInstance(), window, nullptr, &this->surface);
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

void Dragon::Graphics::Window::createFramebuffers(Dragon::Device device) {
    auto imageViews = this->swapchain.get_image_views(nullptr);
    if(!imageViews) {
        throw fmt::format("Swapchain image views failed with {}", imageViews.error().message());
    }

    this->framebuffers.resize(this->swapchain.image_count);

    for (size_t i = 0; i < this->swapchain.image_count; i++) {
        VkImageView attachments[] = {
            imageViews.value()[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = this->renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = this->swapchain.extent.width;
        framebufferInfo.height = this->swapchain.extent.height;
        framebufferInfo.layers = 1;

        VkResult result = vkCreateFramebuffer(device, &framebufferInfo, nullptr, &this->framebuffers[i]);
        if (result != VK_SUCCESS) {
            throw fmt::format("failed to create framebuffer {} with {}", i, string_VkResult(result));
        }
    }
}

void Dragon::Graphics::Window::createSwapchain(Dragon::Device device) {
    Dragon::SwapchainBuilder builder{device};

    builder.set_old_swapchain(this->swapchain);

    Dragon::Result<Dragon::Swapchain> result = builder.build();

    if(!result) {
        throw fmt::format("Window swapchain creation failed with {}", result.error().message());
    }

    this->swapchain = result.value();

}

void Dragon::Graphics::Window::cleanupSwapchain(VkDevice device) {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(this->window, &width, &height);
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(device);
    for (auto framebuffer : this->framebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
}

void Dragon::Graphics::Window::close(VkInstance instance, VkDevice device) {
    this->cleanupSwapchain(device);
    vkb::destroy_swapchain(this->swapchain);
    vkDestroyCommandPool(device, this->commandPool, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);
    vkDestroySurfaceKHR(instance, this->surface, nullptr);
}