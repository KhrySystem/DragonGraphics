#pragma once

#include "aliases.hpp"
#include "rotation.hpp"

namespace Dragon::Graphics {
    /**
     * @brief Represents a vertex. Models contain lists of these. 
     * 
     * @details Not recommended to build by hand except for extremely simple models.
     * It is highly recommended to put your data into a simple image or obj file before
     * building Verticies by hand. 
     * 
     * @see Dragon::Graphics::Model
    */
    struct DGGRAPHICSAPI Vertex
    {
        /**Current position variable*/
        #ifndef DRAGON_2D_SHADER
        Dragon::vec3 position; 
        #else
        DRAGON::vec2 position;
        #endif
    };
    
}