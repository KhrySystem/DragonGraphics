#include <cstdint>
#include <limits>
#include <algorithm>
#include <dragon/graphics.hpp>

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    for(auto format : availableFormats) {
        if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &presentModes) {
    for(auto mode : presentModes) {
        if(mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

namespace Dragon {
    Result<Swapchain> SwapchainBuilder::build() {
        VkSurfaceCapabilitiesKHR surfCaps;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(this->device.getPhysicalDevice()->getDevice(), this->surface, &surfCaps);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(this->device.getPhysicalDevice()->getDevice(), this->surface, &formatCount, nullptr);
        if(formatCount == 0) {
            ResultError error;
            error.message = fmt::format("No surface formats were found.");
            error.resultCode = VK_ERROR_FORMAT_NOT_SUPPORTED;
            return Result<Swapchain>(error);
        }

        std::vector<VkSurfaceFormatKHR> formats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(this->device.getPhysicalDevice()->getDevice(), this->surface, &formatCount, formats.data());

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(this->device.getPhysicalDevice()->getDevice(), this->surface, &presentModeCount, nullptr);
        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(this->device.getPhysicalDevice()->getDevice(), this->surface, &presentModeCount, presentModes.data());

        VkSurfaceFormatKHR format = chooseSwapSurfaceFormat(formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(presentModes);

        VkExtent2D extent;

        if(surfCaps.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            extent = surfCaps.currentExtent;
        } else {
            VkExtent2D extent = {
                static_cast<uint32_t>(this->targetWidth),
                static_cast<uint32_t>(this->targetHeight)
            };

            extent.width = std::clamp(extent.width, surfCaps.minImageExtent.width, surfCaps.maxImageExtent.width);
            extent.height = std::clamp(extent.height, surfCaps.minImageExtent.height, surfCaps.maxImageExtent.height);
        }

        uint32_t imageCount = surfCaps.minImageCount + 1;

        if (surfCaps.maxImageCount > 0 && imageCount > surfCaps.maxImageCount) {
            imageCount = surfCaps.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.clipped = this->isClipped;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.flags = 0;
        createInfo.imageArrayLayers = 1;
        createInfo.imageColorSpace = format.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageFormat = format.format;
        if(this->presentQueueFamily.getID() == this->graphicsQueueFamily.getID()) {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        } else {
            uint32_t queueFamilyIndices[] = {this->presentQueueFamily.getID(), this->graphicsQueueFamily.getID()};
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }

        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.minImageCount = imageCount;
        createInfo.oldSwapchain = this->oldSwapchain;
        createInfo.presentMode = presentMode;
        createInfo.preTransform = surfCaps.currentTransform;
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = this->surface;

        Swapchain swapchain;
        swapchain.device = this->device;
        swapchain.extent = extent;
        swapchain.imageFormat = format.format;
        VkResult result = vkCreateSwapchainKHR(this->device, &createInfo, nullptr, &swapchain.swapchain);
        if(result != VK_SUCCESS) {
            ResultError error;
            error.message = fmt::format("Could not create swapchain with {}", string_VkResult(result));
            error.resultCode = result;
            return Result<Swapchain>(error);
        }

        vkGetSwapchainImagesKHR(this->device, swapchain.swapchain, &imageCount, nullptr);
        swapchain.images.resize(imageCount);
        vkGetSwapchainImagesKHR(this->device, swapchain.swapchain, &imageCount, swapchain.images.data());
        swapchain.imageViews.resize(imageCount);

        size_t i = 0;
        for(VkImage image : swapchain.images) {
            VkImageViewCreateInfo icInfo{};
            icInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            icInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            icInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            icInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            icInfo.flags = 0;
            icInfo.format = format.format;
            icInfo.image = image;
            icInfo.pNext = nullptr;
            icInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            icInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            icInfo.subresourceRange.baseArrayLayer = 0;
            icInfo.subresourceRange.baseMipLevel = 0;
            icInfo.subresourceRange.layerCount = 1;
            icInfo.subresourceRange.levelCount = 1;
            icInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

            result = vkCreateImageView(this->device, &icInfo, nullptr, &swapchain.imageViews[i]);
            if(result != VK_SUCCESS) {
                ResultError error;
                error.message = fmt::format("Could not create ImageView with {}", string_VkResult(result));
                error.resultCode = result;
                return Result<Swapchain>(error);
            }
        }

        return Result<Swapchain>(swapchain);
    }

    void Swapchain::close() {
        for(auto imageView : this->imageViews) {
            vkDestroyImageView(this->device, imageView, nullptr);
        }
        vkDestroySwapchainKHR(this->device, this->swapchain, nullptr);
    }
}