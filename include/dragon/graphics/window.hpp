#pragma once

#include <vulkan/vulkan.hpp>
#include "engine.hpp"

#ifdef Dragon_2D_RENDER
    #include "shaders/2d.frag.hpp"
    #include "shaders/2d.vert.hpp"
#else
    #include "shaders/3d.frag.hpp"
    #include "shaders/3d.vert.hpp"
#endif

namespace Dragon::Graphics {
    /**
     * @brief Window class. Represents an active game world, including at least one camera
    */
    class DGGRAPHICSAPI Window {
        private:
            VkSurfaceKHR surface; /**<Vulkan Surface. Serves as the interconnect between the native window and Vulkan.*/
            GLFWwindow* window; /**<GLFW window. container around a native window in a cross platform way.*/
            Dragon::Swapchain swapchain; /**<VkBootstrap swapchain object. Represents actually sending data to the window surface.*/
            VkPipelineLayout pipelineLayout; /**<Pipeline layout for this window's graphics pipeline*/
            VkRenderPass renderPass; /**<Render pass for this window's graphics pipeline*/
            VkPipeline graphicsPipeline; /**<Graphics Pipeline for this object. Do not touch.*/
            std::vector<VkFramebuffer> framebuffers; /**Framebuffers for rendering multiple frames to the window*/
            VkCommandPool commandPool; /**<The thing responsible for the graphical GPU commands here*/
            std::array<VkCommandBuffer, DRAGON_FRAME_RENDER_COUNT> commandBuffers;
            std::array<VkSemaphore, DRAGON_FRAME_RENDER_COUNT> imageAvailableSemaphores;
            std::array<VkSemaphore, DRAGON_FRAME_RENDER_COUNT> renderFinishedSemaphores;
            std::array<VkFence, DRAGON_FRAME_RENDER_COUNT> inFlightFences;

            size_t frameID = 0;

            void createRenderPass(VkFormat format, Dragon::Device device);

            void cleanupSwapchain(VkDevice device);
            void createSwapchain(Dragon::Device device);
            void createFramebuffers(Dragon::Device device);
            void recordCommandBuffer(size_t imageIndex);

        public:
            bool framebufferResized = false;
            /**
             * @brief Constructor.
             * 
             * @throws std::string
            */
            Window(Engine* parent, int width, int height, std::string title);
            void afterInstanceCreation(Engine* parent);
            /**
             * @brief Returns the raw Vulkan surface object attached to this window.
             * 
             * @returns VkSurfaceKHR from this window
            */
            inline VkSurfaceKHR getSurface() {return this->surface;}
            /**
             * @brief Returns the raw GLFW window object representing this window to the OS.
             * 
             * @returns GLFWwindow* from this window
            */
            inline GLFWwindow* getRawWindow() {return this->window;}
            /**
             * @brief Returns the raw Vulkan swapchain object attached to this window
             * 
             * @returns Dragon::Swapchain from this window
            */
            inline Dragon::Swapchain getRawSwapchain() {return this->swapchain;}
            /**
             * @brief Finishes creating the window's Vulkan objects
             * 
             * @param parent the Dragon::Graphics::Engine that created this object
             * 
             * @throws std::string
            */
            void finalize(Engine* parent);
            /**
             * @brief Sets the window title for this window
             * 
             * Note that there is no good way to get the current window title, 
             * so if your program requires the window title to represent a state you should store it yourself.
             * 
             * @param title the new title. 
            */
            inline void setWindowTitle(std::string title) {glfwSetWindowTitle(this->window, title.c_str());}
            /**
             * @brief Sets the window size. 
             * 
             * @see Dragon::Graphics::Window::getWindowHeight()
             * @see Dragon::Graphics::Window::getWindowWidth()
            */
            inline void setWindowSize(int width, int height) {glfwSetWindowSize(this->window, width, height);}
            /**
             * @brief Gets this window's height
             * 
             * @returns This window's height in pixels
            */
            int getWindowHeight();
            /**
             * @brief Gets this window's width
             * 
             * @returns This window's width in pixels
            */
            int getWindowWidth();

            /**
             * @brief Renders this window based on the models attached and the current active camera
             * 
             * @throws std::string
            */
            void update(Engine* parent);

            /**
             * @brief Checks this window's close flag and returns the state
             * 
             * @returns true if this window should NOT close
             * 
             * @code
             * if(!window->shouldClose()) {
             *      //stuff
             * }
             * @endcode
            */
            inline bool shouldClose() {return glfwWindowShouldClose(this->window);}
            /**
             * @brief closes this window and frees its objects. 
             * 
             * This does NOT free models. This function always succeeds.
            */
            void close(VkInstance instance, VkDevice device);
    };
} // namespace Dragon::Graphics