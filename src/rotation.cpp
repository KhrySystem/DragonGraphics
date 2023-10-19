#include <dragon/graphics.hpp>
#include <cmath>

glm::vec3 Dragon::Graphics::Rotation::toEulerAngles() {
    double pitch = std::asin(
        2 * (
            this->q.x * this->q.z +
            this->q.y * this->q.w
        )
    );

    if(pitch == M_PI_2) {
        return {-2 * std::atan2(this->q.y, this->q.x), pitch, 0};
    }

    if(pitch == -M_PI_2) {
        return {2 * std::atan2(this->q.y, this->q.x), pitch, 0};
    }
    return {
        std::atan2(
            2 * ((this->q.x * this->q.y) + (this->q.z * this->q.w)), 
            std::sqrt(std::pow(this->q.x, 2) - std::pow(this->q.y, 2) - std::pow(this->q.z, 2) + std::pow(this->q.w, 2))
        ),
        pitch, 
        std::atan2(
            2 * ((this->q.x * this->q.w) + (this->q.y * this->q.z)), 
            std::sqrt(std::pow(this->q.x, 2) + std::pow(this->q.y, 2) - std::pow(this->q.z, 2) - std::pow(this->q.w, 2))
        )
    };
}

glm::mat3x3 Dragon::Graphics::Rotation::toRotationMatrix() {
    return  {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
}

glm::vec4 Dragon::Graphics::Rotation::toAxisAngle() {
    if(this->q.x == 1.0) {
        return {0, 1, 0, 0};
    }
    double theta = 2 * std::acos(this->q.x);
    double d = std::sin(theta / 2);
    return {
        theta, 
        this->q.y / d,
        this->q.z / d,
        this->q.w / d,
    };
}