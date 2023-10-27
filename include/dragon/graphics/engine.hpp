/**
 * @brief Container header file for the Graphics Engine
*/

#pragma once

#include <dragon/core.hpp>

namespace Dragon::Graphics {
    class Engine;
}

#include "window.hpp"

/**
 * @brief Namespace holding everything pertaining to graphics. Does not include GLM aliases.
 * 
 * @ingroup Graphics
 * 
 * @see Dragon::Graphics::Engine
*/
namespace Dragon::Graphics
{
    /**
     * @brief Graphics submodule. Must be added to a Dragon::Engine instance in order to be useful.
     * 
     * @ingroup Graphics
     * 
     * @see Dragon::Engine::addSubmodule(Dragon::Submodule* submodule)
    */
    class DGGRAPHICSAPI Engine : public Dragon::Submodule {
        private:
            /**
             * Parent Core Engine. Populated by the internal onAddition call.
             * 
             * @see Dragon::Engine
            */
            Dragon::Engine* parent; 
            std::vector<Window*> windows; /**<Storage of all active windows.*/
            VkQueue graphicsQueue;
            VkQueue presentQueue;
        public:
            virtual void onAddition(Dragon::Engine* parent) override;

            /**
             * @brief Returns this engine's parent core engine object pointer
            */
            inline Dragon::Engine* getParent() {return this->parent;}

            /**
             * @param index The window's index, in order of creation.
             * 
             * @returns the pointer to the Window
             * 
             * @throws std::string
            */
            inline Window* getWindow(size_t index) {return this->windows.at(index);};

            /**
             * Checks to see if there are still open windows from this engine
             * 
             * @returns if it is safe to close and shutdown this engine
            */
            inline bool areWindowsOpen() {return this->windows.size() != 0;}
            inline VkQueue getGraphicsQueue() {return this->graphicsQueue;}
            inline VkQueue getPresentQueue() {return this->presentQueue;}

            /**
             * @param width The initial width of the window (in pixels)
             * @param height The initial height of the window (in pixels)
             * @param title The initial title of the window
             * 
             * @returns the pointer to the Window that was just created.
             * 
             * @throws std::string
            */
            Window* createWindow(int width, int height, std::string title);

            /**
             * Reimplemented from Dragon::Submodule
            */
            virtual Dragon::InstanceBuilder adjustInstanceParams(Dragon::Engine* parent, Dragon::InstanceBuilder &previous) override;
            virtual void beforePhysicalDeviceSelection(Dragon::Engine* parent) override;
            /**
             * Reimplemented from Dragon::Submodule
            */
            virtual Dragon::PhysicalDeviceSelector adjustPhysicalDeviceParams(Dragon::Engine* parent, Dragon::PhysicalDeviceSelector &previous) override;
            /**
             * Reimplemented from Dragon::Submodule
            */
            virtual Dragon::DeviceBuilder adjustDeviceParams(Dragon::Engine* parent, Dragon::DeviceBuilder &previous) override;
            virtual void afterDeviceCreation(Dragon::Engine* parent) override;

            /**
             * @brief Performs a single frame update of all windows, polls inputs, closes windows 
             * that have been requested to be closed, and tries to pretend like everything is right with the world.
            */
            virtual void update(Dragon::Engine* parent) override;

            inline virtual void close(Dragon::Engine* parent) override {assert(this->windows.size() == 0);}
            inline virtual void afterClose(Dragon::Engine* parent) override {glfwTerminate();}
    };
    
} // namespace Dragon::Graphics

