#pragma once

#include "aliases.hpp"

namespace Dragon::Graphics
{
    struct Rotation {
        Rotation() {this->q = {1.0, 0.0, 0.0, 0.0}; }

        Dragon::vec4 q;
        
        Dragon::vec3 toEulerAngles();
        Dragon::vec4 toQuaternion() {return this->q;}
        Dragon::mat3x3 toRotationMatrix();
        Dragon::vec4 toAxisAngle();
    };
} // namespace Dragon::Graphics
