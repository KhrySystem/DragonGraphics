#pragma once

#include "aliases.hpp"

public namespace Dragon::Graphics {
    /**
     * @brief Represents an output to a window
     * 
     * @see Dragon::Graphics::Window
     * @see Dragon::Graphics::Window::getCamera()
     * @see Dragon::Graphics::window::createNewCamera()
    */
    class DGGRAPHICSAPI Camera {
        private:
            Dragon::mat4x4 model
            Dragon::mat4x4 view;
            Dragon::mat4x4 projection; /**<Moves the world space into this camera's viewpoint instead of global view.*/
            Dragon::mat4x4 windowMat; /**<Constrains the camera to displaying inside of a certain area of the scree. Should be left to the identity matrix when no multi-camera options are required.*/
    };
}