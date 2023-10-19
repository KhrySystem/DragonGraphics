#pragma once

#include <glm/glm.hpp>

namespace Dragon::Graphics
{
    struct Rotation {
        Rotation() {this->q = {1.0, 0.0, 0.0, 0.0}; }

        glm::vec4 q;
        
        glm::vec3 toEulerAngles();
        glm::vec4 toQuaternion() {return this->q;}
        glm::mat3x3 toRotationMatrix();
        glm::vec4 toAxisAngle();
    };
} // namespace Dragon::Graphics
