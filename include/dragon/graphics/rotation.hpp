#pragma once

#include "aliases.hpp"

namespace Dragon::Graphics
{
    /**
     * @brief Represents a 3D rotation of an object and gives multiple representations for it
     * 
     * The actual interface is a quaternion, however this may be changed into Euler Angles, a Rotation Matrix, or Euler Axis-Angle
    */
    class Rotation {
    private:
        Dragon::vec4 q; /**<This rotation as a unit rotation quaternion. Highly recommended to not touch this unless you know what you're doing. 2 cups of coffee and a 15 minute YouTube video is not enough.*/

    public:
        Rotation() {this->q = {1.0, 0.0, 0.0, 0.0}; }

        
        /**
         * @returns This rotation in Euler Angles <x, y, z> form.
        */
        Dragon::vec3 toEulerAngles();
        /**
         * @returns This rotation as a quaternion in <r, i, j, k> form.
        */
        Dragon::vec4 toQuaternion() {return this->q;}
        /**
         * @returns This rotation as a rotation matrix. 
         * No I'm not going to write the representation.
        */
        Dragon::mat3x3 toRotationMatrix();
        /**
         * @returns This rotation in axis angle <theta, x, y, z> form.
        */
        Dragon::vec4 toAxisAngle();
    };
} // namespace Dragon::Graphics
