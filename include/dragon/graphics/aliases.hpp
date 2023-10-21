#pragma once

#include <glm/glm.hpp>

namespace Dragon
{
    #ifdef DRAGON_DOUBLE_PRECISION_SHADER
    using scalar = double;
    using vec2   = glm::dvec2;
    using vec3   = glm::dvec3;
    using vec4   = glm::dvec4;

    using mat2x2 = glm::dmat2x2;
    using mat2x3 = glm::dmat2x3;
    using mat2x4 = glm::dmat2x4;

    using mat3x2 = glm::dmat3x2;
    using mat3x3 = glm::dmat3x3;
    using mat3x4 = glm::dmat3x4;
    
    using mat4x2 = glm::dmat4x2;
    using mat4x3 = glm::dmat4x3;
    using mat4x4 = glm::dmat4x4;
    #else
    using scalar = float;
    using vec2   = glm::vec2;
    using vec3   = glm::vec3;
    using vec4   = glm::vec4;

    using mat2x2 = glm::mat2x2;
    using mat2x3 = glm::mat2x3;
    using mat2x4 = glm::mat2x4;

    using mat3x2 = glm::mat3x2;
    using mat3x3 = glm::mat3x3;
    using mat3x4 = glm::mat3x4;
    
    using mat4x2 = glm::mat4x2;
    using mat4x3 = glm::mat4x3;
    using mat4x4 = glm::mat4x4;
    #endif
} // namespace Dragon