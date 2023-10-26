#include <dragon/graphics.hpp>
#include <array>

VkAttachmentDescription createColorAttachment(VkFormat format) {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.flags = 0;

    return colorAttachment;
}
VkAttachmentReference createColorAttachmentRef() {
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    return colorAttachmentRef;
}
VkSubpassDescription createSubpass() {
    VkAttachmentReference ref = createColorAttachmentRef();
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &ref;

    return subpass;
}
VkRenderPassCreateInfo createRenderPassInfo(VkFormat format) {
    VkAttachmentDescription desc = createColorAttachment(format);
    VkSubpassDescription subpass = createSubpass();
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &desc;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    return renderPassInfo;
}
VkShaderModule createShaderModule(VkDevice device, size_t size, const uint32_t* source) {
    VkShaderModuleCreateInfo shaderCreateInfo{};
    shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderCreateInfo.codeSize = _VERT_IN_SOURCE_SIZE;
    shaderCreateInfo.pCode = source;

    VkShaderModule shaderModule;
    VkResult result = vkCreateShaderModule(device, &shaderCreateInfo, nullptr, &shaderModule);
    if(result != VK_SUCCESS) {
        throw fmt::format("Shader Module creation failed with {}", string_VkResult(result));
    }
    return shaderModule;
}
std::array<VkPipelineShaderStageCreateInfo, 2> createShaderStages(VkDevice device, VkShaderModule &vert, VkShaderModule &frag) {
    vert = createShaderModule(device, _VERT_IN_SOURCE_SIZE, reinterpret_cast<const uint32_t*>(_VERT_IN_SOURCE));

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vert;
    vertShaderStageInfo.pName = "main";

    frag = createShaderModule(device, _FRAG_IN_SOURCE_SIZE, reinterpret_cast<const uint32_t*>(_FRAG_IN_SOURCE));

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = frag;
    fragShaderStageInfo.pName = "main";

    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {vertShaderStageInfo, fragShaderStageInfo};

    return shaderStages;
}
VkPipelineVertexInputStateCreateInfo createVertexInputInfo() {
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    return vertexInputInfo;
}
VkPipelineInputAssemblyStateCreateInfo createInputAssembly() {
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    return inputAssembly;
}
VkPipelineViewportStateCreateInfo createViewportState() {
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    return viewportState;
}
VkPipelineRasterizationStateCreateInfo createRasterizer() {
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    return rasterizer;
}
VkPipelineMultisampleStateCreateInfo createMultisampling() {
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    return multisampling;
}
VkPipelineColorBlendAttachmentState createColorBlendAttachment() {
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    return colorBlendAttachment;
}
VkPipelineColorBlendStateCreateInfo createColorBlending() {
    VkPipelineColorBlendAttachmentState attachment = createColorBlendAttachment();
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &attachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    return colorBlending;
}
VkPipelineDynamicStateCreateInfo createDynamicStates() {
    std::vector states = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = states.size();
    dynamicState.pDynamicStates = states.data();

    return dynamicState;
}
VkPipelineLayoutCreateInfo createPipelineLayoutInfo() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    return pipelineLayoutInfo;
}
VkGraphicsPipelineCreateInfo createPipelineInfo(VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderPass, VkShaderModule &vert, VkShaderModule &frag) {
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = createShaderStages(device, vert, frag).data();
    VkPipelineVertexInputStateCreateInfo inputState = createVertexInputInfo();
    pipelineInfo.pVertexInputState = &inputState;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = createInputAssembly();
    pipelineInfo.pInputAssemblyState = &inputAssemblyState;
    VkPipelineViewportStateCreateInfo viewportState = createViewportState();
    pipelineInfo.pViewportState = &viewportState;
    VkPipelineRasterizationStateCreateInfo rasterizationState = createRasterizer();
    pipelineInfo.pRasterizationState = &rasterizationState;
    VkPipelineMultisampleStateCreateInfo multisampleState = createMultisampling();
    pipelineInfo.pMultisampleState = &multisampleState;
    VkPipelineColorBlendStateCreateInfo colorBlendState = createColorBlending();
    pipelineInfo.pColorBlendState = &colorBlendState;
    VkPipelineDynamicStateCreateInfo dynamicState = createDynamicStates();
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    return pipelineInfo;
}
void Dragon::Graphics::Window::finalize(Dragon::Graphics::Engine* parent) {
    this->swapchain = this->recreateSwapchain(parent->getParent()->getDevice());

    VkRenderPassCreateInfo renderPass = createRenderPassInfo(this->swapchain.image_format);

    VkResult result = vkCreateRenderPass(parent->getParent()->getDevice(), &renderPass, nullptr, &this->renderPass);
    if (result != VK_SUCCESS) {
        throw fmt::format("failed to create render pass with {}", string_VkResult(result));
    }

    VkPipelineLayoutCreateInfo pipelineLayout = createPipelineLayoutInfo();

    result = vkCreatePipelineLayout(parent->getParent()->getDevice(), &pipelineLayout, nullptr, &this->pipelineLayout);
    if (result != VK_SUCCESS) {
        throw fmt::format("failed to create pipeline layout with {}", string_VkResult(result));
    }

    VkShaderModule vert, frag;
    VkGraphicsPipelineCreateInfo pipeline = createPipelineInfo(parent->getParent()->getDevice(), this->pipelineLayout, this->renderPass, vert, frag);
    result = vkCreateGraphicsPipelines(
        parent->getParent()->getDevice(), 
        VK_NULL_HANDLE, 
        1, 
        &pipeline, 
        nullptr, 
        &this->graphicsPipeline
    );
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(parent->getParent()->getDevice(), vert, nullptr);
    vkDestroyShaderModule(parent->getParent()->getDevice(), frag, nullptr);

    this->framebuffers.resize(this->swapchain.image_count);

    auto imageViews = this->swapchain.get_image_views(nullptr);
    if(!imageViews) {
        throw fmt::format("Swapchain image views failed with {}", imageViews.error().message());
    }

    for (size_t i = 0; i < this->swapchain.image_count; i++) {
        VkImageView attachments[] = {
            imageViews.value()[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = this->renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = this->swapchain.extent.width;
        framebufferInfo.height = this->swapchain.extent.height;
        framebufferInfo.layers = 1;

        result = vkCreateFramebuffer(parent->getParent()->getDevice(), &framebufferInfo, nullptr, &this->framebuffers[i]);
        if (result != VK_SUCCESS) {
            throw fmt::format("failed to create framebuffer {} with {}", i, string_VkResult(result));
        }
    }

    auto graphicsQueueIndexResult = parent->getParent()->getDevice().get_queue_index(vkb::QueueType::graphics);

    if(!graphicsQueueIndexResult) {
        throw fmt::format("failed to get graphics queue index with {}", graphicsQueueIndexResult.error().message());
    }

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = graphicsQueueIndexResult.value();

    result = vkCreateCommandPool(parent->getParent()->getDevice(), &poolInfo, nullptr, &this->commandPool);
    if(result != VK_SUCCESS) {
        throw fmt::format("failed to create command pool with {}", string_VkResult(result));
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = this->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    result = vkAllocateCommandBuffers(parent->getParent()->getDevice(), &allocInfo, &commandBuffer);
    if (result != VK_SUCCESS) {
        throw fmt::format("failed to allocate command buffers with {}", string_VkResult(result));
    }
}