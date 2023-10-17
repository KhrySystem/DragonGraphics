#include <dragon/core/Submodule.hpp>
#include <GLFW/glfw3.h>

#include "window.hpp"

namespace Dragon::Graphics
{
    class Engine : public Dragon::Submodule
    {
        private:
            std::vector<Window*> windows;
        public:
            Window* getWindow(size_t index);

            void beforeInstanceCreation(Dragon::Engine* parent) override;
            Dragon::InstanceBuilder adjustInstanceParams(Dragon::Engine* parent, Dragon::InstanceBuilder &previous) override;
            void beforePhysicalDeviceSelection(Dragon::Engine* parent) override;
            Dragon::PhysicalDeviceSelector adjustPhysicalDeviceParams(Dragon::Engine* parent, Dragon::PhysicalDeviceSelector &previous) override;

            void update(Dragon::Engine* parent) override;

            void afterClose(Dragon::Engine* parent) override;
            void close(Dragon::Engine* parent) override;
    };
    
} // namespace Dragon::Graphics

