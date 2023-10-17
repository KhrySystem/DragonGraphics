#include <dragon/core/Submodule.hpp>
#include <glfw/glfw3.h>

namespace Dragon::Graphics
{
    class Engine : public Dragon::Submodule
    {
        private:
            std::vector<GLFWwindow*> windows;
        public:
            void editWindow(size_t index, uint32_t width, uint32_t height, std::string title);

            void beforeInstanceCreation(Dragon::Engine* parent) override;
            Dragon::InstanceBuilder adjustInstanceParams(Dragon::Engine* parent, Dragon::InstanceBuilder &previous) override;
            void beforePhysicalDeviceSelection(Dragon::Engine* parent) override;
            Dragon::PhysicalDeviceSelector adjustPhysicalDeviceParams(Dragon::Engine* parent, Dragon::PhysicalDeviceSelector &previous) override;

            void update(Dragon::Engine* parent) override;

            void afterClose(Dragon::Engine* parent) override;
            void close(Dragon::Engine* parent) override;
    };
    
} // namespace Dragon::Graphics

