#pragma once

#include <dragon/core.hpp>
#include "swapchain.hpp"

namespace Dragon {
    class SwapchainBuilder {
        public:
            Result<Swapchain> build();
    };
}