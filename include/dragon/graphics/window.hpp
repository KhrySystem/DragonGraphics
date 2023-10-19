#pragma once

#include <vulkan/vulkan.hpp>

namespace Dragon::Graphics {
    class Window {
        private:
            VkSurfaceKHR surface;
            GLFWwindow* window;
        public:
            Window(VkInstance instance, int width, int height, std::string title);
            inline VkSurfaceKHR getSurface() {return this->surface;}
            inline GLFWwindow* getRawWindow() {return this->window;}

            inline void setWindowTitle(std::string title) {glfwSetWindowTitle(this->window, title.c_str());}
            inline void setWindowSize(int width, int height) {glfwSetWindowSize(this->window, width, height);}
            int getWindowHeight();
            int getWindowWidth();

            void update();

            inline bool shouldClose() {return glfwWindowShouldClose(this->window);}
            void close(VkInstance instance);
    };
} // namespace Dragon::Graphics