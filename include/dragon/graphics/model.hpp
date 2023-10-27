#pragma once

#include <vector>

#include "rotation.hpp"
#include "vertex.hpp"
#include "window.hpp"

namespace Dragon::Graphics {
    /**
     * @brief Model class. Represents a model in game space (render layer > 0)
    */
    class DGGRAPHICSAPI Model
    {
    private:
        //Rotation rotation; /**<Represents this model's current rotation.*/
        //std::vector<Model>; /**Child models that will be moved an equal proportion when this model moves*/
        /* data */
        std::vector<Vertex> verts;

        Dragon::Buffer vertexBuffer;
    public:
        Model(Engine* parent, Window* owner, std::vector<Vertex> &verts);
        void close(Engine* parent, Window* owner);
    };
}