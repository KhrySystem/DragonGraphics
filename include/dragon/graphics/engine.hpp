#include <dragon/core.hpp>


#include "window.hpp"

namespace Dragon::Graphics
{
    class Engine : public Dragon::Submodule
    {
        private:
            Dragon::Engine* parent;
            std::vector<Window*> windows;
        public:
            virtual void onAddition(Dragon::Engine* parent) override;

            Window* getWindow(size_t index);

            bool areWindowsOpen() {return this->windows.size() != 0;}

            Window* createWindow(int width, int height, std::string title);

            virtual Dragon::InstanceBuilder adjustInstanceParams(Dragon::Engine* parent, Dragon::InstanceBuilder &previous) override;
            virtual void beforePhysicalDeviceSelection(Dragon::Engine* parent) override;
            virtual Dragon::PhysicalDeviceSelector adjustPhysicalDeviceParams(Dragon::Engine* parent, Dragon::PhysicalDeviceSelector &previous) override;

            virtual void update(Dragon::Engine* parent) override;

            virtual void afterClose(Dragon::Engine* parent) override;
            virtual void close(Dragon::Engine* parent) override;
    };
    
} // namespace Dragon::Graphics

