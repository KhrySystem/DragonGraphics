#pragma once

#include <glm/glm.hpp>
#include "rotation.hpp"

namespace Dragon::Graphics {
    struct Vertex
    {
        glm::vec3 position;
        Rotation rotation;
    };
    
}