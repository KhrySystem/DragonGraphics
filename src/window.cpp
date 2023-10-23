#include <dragon/graphics.hpp>

#ifndef NDEBUG
#include <iostream>
#endif

Dragon::Graphics::Window::Window(VkInstance instance, int width, int height, std::string title) {
    GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    if(!window) {
        throw std::string("First GLFW Window was NULL, Graphics initialization cannot proceed.\n");
    }
    this->window = window;
    
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &this->surface);
    if(result != VK_SUCCESS) {
        throw fmt::format("Window Surface failed with {}", string_VkResult(result));
    }
}

int Dragon::Graphics::Window::getWindowWidth() {
    int width, height;
    glfwGetWindowSize(this->window, &width, &height);
    return width;
}

int Dragon::Graphics::Window::getWindowHeight() {
    int width, height;
    glfwGetWindowSize(this->window, &width, &height);
    return height;
}

void Dragon::Graphics::Window::finalize(Dragon::Graphics::Engine* parent) {
    this->swapchain = this->recreateSwapchain(parent->getParent()->getDevice());

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = this->swapchain.image_format;
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

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    VkResult result = vkCreateRenderPass(parent->getParent()->getDevice(), &renderPassInfo, nullptr, &this->renderPass);
    if (result != VK_SUCCESS) {
        throw fmt::format("failed to create render pass with {}", string_VkResult(result));
    }

    VkShaderModuleCreateInfo vertShaderCreateInfo{};
    vertShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertShaderCreateInfo.codeSize = _VERT_IN_SOURCE_SIZE;
    vertShaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(_VERT_IN_SOURCE);

    VkShaderModule vertShaderModule;
    result = vkCreateShaderModule(parent->getParent()->getDevice(), &vertShaderCreateInfo, nullptr, &vertShaderModule);
    if(result != VK_SUCCESS) {
        throw fmt::format("Shader Module creation failed with {}", string_VkResult(result));
    }

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkShaderModuleCreateInfo fragShaderCreateInfo{};
    fragShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragShaderCreateInfo.codeSize = _FRAG_IN_SOURCE_SIZE;
    fragShaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(_FRAG_IN_SOURCE);

    VkShaderModule fragShaderModule;
    result = vkCreateShaderModule(parent->getParent()->getDevice(), &fragShaderCreateInfo, nullptr, &fragShaderModule);
    if(result != VK_SUCCESS) {
        throw fmt::format("Shader Module creation failed with {}", string_VkResult(result));
    }

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

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

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    result = vkCreatePipelineLayout(parent->getParent()->getDevice(), &pipelineLayoutInfo, nullptr, &this->pipelineLayout);
    if (result != VK_SUCCESS) {
        throw fmt::format("failed to create pipeline layout with {}", string_VkResult(result));
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    result = vkCreateGraphicsPipelines(parent->getParent()->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->graphicsPipeline);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(parent->getParent()->getDevice(), fragShaderModule, nullptr);
    vkDestroyShaderModule(parent->getParent()->getDevice(), vertShaderModule, nullptr);

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

Dragon::Swapchain Dragon::Graphics::Window::recreateSwapchain(Dragon::Device device) {
    #ifndef NDEBUG
    std::cout << "Recreating swapchain";
    #endif
    Dragon::SwapchainBuilder builder{device};

    builder.set_old_swapchain(this->swapchain);

    Dragon::Result<Dragon::Swapchain> result = builder.build();

    if(!result) {
        throw fmt::format("Window swapchain creation failed with {}", result.error().message());
    }

    return result.value();
}

void Dragon::Graphics::Window::update(Dragon::Graphics::Engine* parent) {
    if(this->shouldClose()) return;
}

void Dragon::Graphics::Window::close(VkInstance instance, VkDevice device) {
    vkDestroyCommandPool(device, this->commandPool, nullptr);
    for (auto framebuffer : this->framebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);
    vkb::destroy_swapchain(this->swapchain);
    vkDestroySurfaceKHR(instance, this->surface, nullptr);
}