/**
 * @brief Container header file for the Graphics Engine
*/

#pragma once

#include <dragon/core.hpp>

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
    class Engine : public Dragon::Submodule
    {
        private:
            /**
             * Parent Core Engine. Populated by the internal onAddition call.
             * 
             * @see Dragon::Engine
            */
            Dragon::Engine* parent; 
            std::vector<Window*> windows; /**<Storage of all active windows.*/
        public:
            virtual void onAddition(Dragon::Engine* parent) override;

            /**
             * @param index The window's index, in order of creation.
             * 
             * @returns the pointer to the Window
             * 
             * @throws std::string
            */
            Window* getWindow(size_t index);

            /**
             * Checks to see if there are still open windows from this engine
             * 
             * @returns if it is safe to close and shutdown this engine
            */
            bool areWindowsOpen() {return this->windows.size() != 0;}

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

            virtual Dragon::InstanceBuilder adjustInstanceParams(Dragon::Engine* parent, Dragon::InstanceBuilder &previous) override;
            virtual void beforePhysicalDeviceSelection(Dragon::Engine* parent) override;
            virtual Dragon::PhysicalDeviceSelector adjustPhysicalDeviceParams(Dragon::Engine* parent, Dragon::PhysicalDeviceSelector &previous) override;

            /**
             * @brief Performs a single frame update of all windows, polls inputs, closes windows 
             * that have been requested to be closed, and tries to pretend like everything is right with the world.
            */
            virtual void update(Dragon::Engine* parent) override;

            virtual void afterClose(Dragon::Engine* parent) override;
            virtual void close(Dragon::Engine* parent) override;
    };
    
} // namespace Dragon::Graphics

