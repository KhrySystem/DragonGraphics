namespace Dragon::Graphics {
    class Window {
        private:
            VkSurfaceKHR surface;
            GLFWwindow* window;
        public:
            Window(int width, int height, std::string title);

    }
} // namespace Dragon::Graphics