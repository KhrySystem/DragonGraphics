#pragma once
#include <array>

#include <dragon/core.hpp>

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
        Dragon::vec2 position;
        #endif
        Dragon::vec4 color;

        static inline VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }

        static inline std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            #ifndef DRAGON_2D_SHADER
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT
            #else
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            #endif
            attributeDescriptions[0].offset = offsetof(Vertex, position);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            return attributeDescriptions;
        }
    };
    
}