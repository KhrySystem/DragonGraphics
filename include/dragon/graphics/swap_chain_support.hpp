#pragma once

#include <dragon/core.hpp>

struct DgSwapChainSupportDetails_T {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

typedef struct DgSwapChainSupportDetails_T DgSwapChainSupportDetails;