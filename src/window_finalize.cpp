#include <dragon/graphics.hpp>
#include <array>

void Dragon::Graphics::Window::createRenderPass(VkFormat format, Dragon::Device device) {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;


    VkResult result = vkCreateRenderPass(device, &renderPassInfo, nullptr, &this->renderPass);
    if (result != VK_SUCCESS) {
        throw fmt::format("failed to create render pass with {}", string_VkResult(result));
    }
}
void createPipelineLayout(Dragon::Device device, VkPipelineLayout* pipelineLayout) {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    VkResult result = vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, pipelineLayout);
    if (result != VK_SUCCESS) {
        throw fmt::format("failed to create pipeline layout with {}", string_VkResult(result));
    }
}
VkShaderModule createShaderModule(VkDevice device, size_t size, const uint32_t* source) {
    VkShaderModuleCreateInfo shaderCreateInfo{};
    shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderCreateInfo.codeSize = size;
    shaderCreateInfo.pCode = source;

    VkShaderModule shaderModule;
    VkResult result = vkCreateShaderModule(device, &shaderCreateInfo, nullptr, &shaderModule);
    if(result != VK_SUCCESS) {
        throw fmt::format("Shader Module creation failed with {}", string_VkResult(result));
    }
    return shaderModule;
}
void createPipeline(Dragon::Device device, VkPipelineLayout pipelineLayout, VkRenderPass renderPass, VkPipeline* graphicsPipeline) {
    VkShaderModule vert, frag;

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;

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

    pipelineInfo.pStages = shaderStages.data();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    pipelineInfo.pVertexInputState = &vertexInputInfo;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    pipelineInfo.pInputAssemblyState = &inputAssembly;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    pipelineInfo.pViewportState = &viewportState;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    pipelineInfo.pRasterizationState = &rasterizer;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineInfo.pMultisampleState = &multisampling;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;
    pipelineInfo.pColorBlendState = &colorBlending;

    std::vector states = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = states.size();
    dynamicState.pDynamicStates = states.data();
    pipelineInfo.pDynamicState = &dynamicState;

    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    VkResult result = vkCreateGraphicsPipelines(
        device, 
        VK_NULL_HANDLE, 
        1, 
        &pipelineInfo, 
        nullptr, 
        graphicsPipeline
    );
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(device, vert, nullptr);
    vkDestroyShaderModule(device, frag, nullptr);
}
void createSyncObjects(
    Dragon::Device device, 
    std::array<VkSemaphore, DRAGON_FRAME_RENDER_COUNT> &imageAvailableSemaphores, 
    std::array<VkSemaphore, DRAGON_FRAME_RENDER_COUNT> &renderFinishedSemaphores, 
    std::array<VkFence, DRAGON_FRAME_RENDER_COUNT> &inFlightFences
) {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for(size_t i = 0; i < DRAGON_FRAME_RENDER_COUNT; i++) {
        VkResult result = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
        if (result != VK_SUCCESS) {
            throw fmt::format("vkCreateSemaphore failed with {}", string_VkResult(result));
        }
        result = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
        if (result != VK_SUCCESS) {
            throw fmt::format("vkCreateSemaphore failed with {}", string_VkResult(result));
        }
        result = vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]);
        if(result != VK_SUCCESS) {
            throw fmt::format("vkCreateFence failed with {}", string_VkResult(result));
        }
    }
    
}
void Dragon::Graphics::Window::finalize(Dragon::Graphics::Engine* parent) {
    this->createSwapchain(parent->getParent()->getDevice());

    this->createRenderPass(this->swapchain.image_format, parent->getParent()->getDevice());
    createPipelineLayout(parent->getParent()->getDevice(), &this->pipelineLayout);
    createPipeline(parent->getParent()->getDevice(), this->pipelineLayout, this->renderPass, &this->graphicsPipeline);
    this->createFramebuffers(parent->getParent()->getDevice());
    auto graphicsQueueIndexResult = parent->getParent()->getDevice().get_queue_index(vkb::QueueType::graphics);

    if(!graphicsQueueIndexResult) {
        throw fmt::format("failed to get graphics queue index with {}", graphicsQueueIndexResult.error().message());
    }

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = graphicsQueueIndexResult.value();

    VkResult result = vkCreateCommandPool(parent->getParent()->getDevice(), &poolInfo, nullptr, &this->commandPool);
    if(result != VK_SUCCESS) {
        throw fmt::format("failed to create command pool with {}", string_VkResult(result));
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = this->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = this->commandBuffers.size();

    result = vkAllocateCommandBuffers(parent->getParent()->getDevice(), &allocInfo, this->commandBuffers.data());
    if (result != VK_SUCCESS) {
        throw fmt::format("failed to allocate command buffers with {}", string_VkResult(result));
    }

    createSyncObjects(parent->getParent()->getDevice(), this->imageAvailableSemaphores, this->renderFinishedSemaphores, this->inFlightFences);
}