#include <dragon/graphics.hpp>

#ifndef NDEBUG
#include <iostream>
#endif

void framebufferResized(GLFWwindow* window, int width, int height) {
    auto w = reinterpret_cast<Dragon::Graphics::Window*>(glfwGetWindowUserPointer(window));
    w->framebufferResized=true;
}

namespace Dragon {
    Graphics::Window::Window(Graphics::Engine* parent, int width, int height, std::string title) {
        GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if(!window) {
            throw std::string("First GLFW Window was NULL, Graphics initialization cannot proceed.\n");
        }
        this->window = window;
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, ::framebufferResized);

        if(parent->getParent()->getInstance() != VK_NULL_HANDLE) {
            this->afterInstanceCreation(parent);
            this->finalize(parent);
        }
    }

    void Graphics::Window::afterInstanceCreation(Graphics::Engine* parent) {
        VkResult result = glfwCreateWindowSurface(parent->getParent()->getInstance(), window, nullptr, &this->surface);
        if(result != VK_SUCCESS) {
            throw fmt::format("Window Surface failed with {}", string_VkResult(result));
        }
    }

    int Graphics::Window::getWindowWidth() {
        int width, height;
        glfwGetWindowSize(this->window, &width, &height);
        return width;
    }

    int Graphics::Window::getWindowHeight() {
        int width, height;
        glfwGetWindowSize(this->window, &width, &height);
        return height;
    }

    void Graphics::Window::createFramebuffers(Device device) {
        auto imageViews = swapchain.getImageViews();

        this->framebuffers.resize(imageViews.size());

        for (size_t i = 0; i < imageViews.size(); i++) {
            VkImageView attachments[] = {
                imageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = this->renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = this->swapchain.getExtent().width;
            framebufferInfo.height = this->swapchain.getExtent().height;
            framebufferInfo.layers = 1;

            VkResult result = vkCreateFramebuffer(device, &framebufferInfo, nullptr, &this->framebuffers[i]);
            if (result != VK_SUCCESS) {
                throw fmt::format("failed to create framebuffer {} with {}", i, string_VkResult(result));
            }
        }
    }

    void Graphics::Window::createSwapchain(Device device) {
        SwapchainBuilder builder{device};

        builder.setOldSwapchain(this->swapchain);
        builder.setSurface(this->surface);

        Result<Swapchain> result = builder.build();

        if(!result) {
            throw fmt::format("Window swapchain creation failed with {}", result.getError().message);
        }

        this->swapchain = result.getValue();

    }

    void Graphics::Window::cleanupSwapchain(VkDevice device) {
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

    void Graphics::Window::close(VkInstance instance, VkDevice device) {
        this->cleanupSwapchain(device);
        this->swapchain.~Swapchain();
        vkDestroyCommandPool(device, this->commandPool, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyRenderPass(device, renderPass, nullptr);
        vkDestroySurfaceKHR(instance, this->surface, nullptr);
    }
}