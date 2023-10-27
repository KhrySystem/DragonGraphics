#include <dragon/graphics.hpp>
#ifndef NDEBUG
#include <iostream>
#endif

void Dragon::Graphics::Window::recordCommandBuffer(size_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    VkResult result = vkBeginCommandBuffer(this->commandBuffers[this->frameID], &beginInfo);
    if (result != VK_SUCCESS) {
        throw fmt::format("vkBeginCommandBuffer failed with {}", string_VkResult(result));
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = this->framebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = this->swapchain.extent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(this->commandBuffers[this->frameID], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(this->commandBuffers[this->frameID], VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(this->swapchain.extent.width);
    viewport.height = static_cast<float>(this->swapchain.extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(this->commandBuffers[this->frameID], 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = this->swapchain.extent;
    vkCmdSetScissor(this->commandBuffers[this->frameID], 0, 1, &scissor);

    vkCmdDraw(this->commandBuffers[this->frameID], 3, 1, 0, 0);

    vkCmdEndRenderPass(this->commandBuffers[this->frameID]);

    result = vkEndCommandBuffer(this->commandBuffers[this->frameID]);
    if(result != VK_SUCCESS) {
        throw fmt::format("vkEndCommandBuffer failed with {}", string_VkResult(result));
    }
}

void Dragon::Graphics::Window::update(Dragon::Graphics::Engine* parent) {
    if(this->shouldClose()) return;

    vkWaitForFences(parent->getParent()->getDevice(), 1, &this->inFlightFences[this->frameID], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(parent->getParent()->getDevice(), this->swapchain, UINT64_MAX, this->imageAvailableSemaphores[this->frameID], VK_NULL_HANDLE, &imageIndex);
    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        this->cleanupSwapchain(parent->getParent()->getDevice());
        this->createSwapchain(parent->getParent()->getDevice());
        this->createFramebuffers(parent->getParent()->getDevice());
        return;
    } else if(result != VK_SUCCESS) {
        throw fmt::format("vkAquireNextImageKHR failed with {}", string_VkResult(result));
    } 
    vkResetFences(parent->getParent()->getDevice(), 1, &this->inFlightFences[this->frameID]);
    vkResetCommandBuffer(this->commandBuffers[this->frameID], 0);
    this->recordCommandBuffer(imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {this->imageAvailableSemaphores[this->frameID]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &this->commandBuffers[this->frameID];

    VkSemaphore signalSemaphores[] = {this->renderFinishedSemaphores[this->frameID]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    result = vkQueueSubmit(parent->getGraphicsQueue(), 1, &submitInfo, this->inFlightFences[this->frameID]);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {this->swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr; // Optional

    vkQueuePresentKHR(parent->getPresentQueue(), &presentInfo);

    this->frameID = (this->frameID+1) % DRAGON_FRAME_RENDER_COUNT;
}