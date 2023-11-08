#pragma once

#include <dragon/core.hpp>

namespace Dragon {
    class SwapchainBuilder;

    class Swapchain {
        private:
            VkFormat imageFormat;
            VkSwapchainKHR swapchain;
            VkExtent2D extent;
            std::vector<VkImageView> imageViews;
            std::vector<VkImage> images;
            Device device;
        public:
            inline VkFormat getImageFormat() {return this->imageFormat;}
            inline VkExtent2D getExtent() {return this->extent;}
            inline std::vector<VkImageView> getImageViews() {return this->imageViews;}
            inline std::vector<VkImage> getImages() {return this->images;}

            inline operator VkSwapchainKHR() const {return this->swapchain;}

            void close();

            friend class SwapchainBuilder;
            friend class Swapchain;
    };

    class SwapchainBuilder {
        private:
            Device device;
            Swapchain oldSwapchain;
            VkSurfaceKHR surface;
            uint32_t targetWidth;
            uint32_t targetHeight;
            bool isClipped = false;
            VkSharingMode sharingMode;
            QueueFamily graphicsQueueFamily;
            QueueFamily presentQueueFamily;
        public:
            inline SwapchainBuilder(Device device) {this->device = device;}

            Result<Swapchain> build();

            inline void makeClipped() {this->isClipped = true;}
            inline void setOldSwapchain(Swapchain swapchain) {this->oldSwapchain = swapchain;}
            inline void setFramebufferSize(uint32_t width, uint32_t height) {
                this->targetWidth = width;
                this->targetHeight = height;
            }
            inline void setQueueFamilies(QueueFamily graphics, QueueFamily present) {
                this->graphicsQueueFamily = graphics;
                this->presentQueueFamily = present;
            }
            inline void setSurface(VkSurfaceKHR &surface) {this->surface = surface;}
    };
}
