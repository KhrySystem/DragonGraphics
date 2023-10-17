#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>

namespace Dragon::Graphics {
    class Window {
        private:
            VkSurfaceKHR surface;
            GLFWwindow* window;
        public:
            Window(VkInstance instance, int width, int height, std::string title);
            VkSurfaceKHR getSurface() {return this->surface;}
            GLFWwindow* getRawWindow() {return this->window;}

            void setWindowTitle(std::string title) {glfwSetWindowTitle(this->window, title.c_str());}
            void setWindowSize(int width, int height) {glfwSetWindowSize(this->window, width, height);}
            int getWindowHeight();
            int getWindowWidth();

            bool shouldClose() {return glfwWindowShouldClose(this->window);}


    };
} // namespace Dragon::Graphics