#include <dragon/graphics.hpp>

Dragon::Graphics::Model::Model(Dragon::Graphics::Engine* parent, Dragon::Graphics::Window* owner, std::vector<Dragon::Graphics::Vertex> &verts) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(Dragon::Graphics::Vertex) * verts.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocInfo{};
    allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

    VkResult result = vmaCreateBuffer(parent->getParent()->getAllocator(), &bufferInfo, &allocInfo, &this->vertexBuffer.buffer, &this->vertexBuffer.allocation, nullptr);

    if (result != VK_SUCCESS) {
        throw fmt::format("vmaCreateBuffer failed with {}", string_VkResult(result));
    }
}

void Dragon::Graphics::Model::close(Dragon::Graphics::Engine* parent, Dragon::Graphics::Window* owner) {
    vmaDestroyBuffer(parent->getParent()->getAllocator(), this->vertexBuffer, this->vertexBuffer);
}

