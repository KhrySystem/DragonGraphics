#pragma once

namespace Dragon {
    class Swapchain {
        private:
            VkFormat imageFormat;
        public:
            VkFormat getImageFormat() {return this->imageFormat;}
    };
}
