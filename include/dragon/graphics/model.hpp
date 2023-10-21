#pragma once

#include <vector>
#include "vertex.hpp"
#include "rotation.hpp"

namespace Dragon::Graphics {
    /**
     * @brief Model class. Represents a model in game space (render layer > 0)
    */
    class Model
    {
    private:
        Rotation rotation; /**<Represents this model's current rotation.*/
        //std::vector<Model>; /**Child models that will be moved an equal proportion when this model moves*/
        /* data */
    public:
    };
}