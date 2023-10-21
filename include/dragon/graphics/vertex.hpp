#pragma once

#include "aliases.hpp"
#include "rotation.hpp"

namespace Dragon::Graphics {
    struct Vertex
    {
        Dragon::vec3 position;
        Rotation rotation;
    };
    
}