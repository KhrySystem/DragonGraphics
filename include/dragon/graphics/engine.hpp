#pragma once

#include <dragon/core.hpp>
#include "window.hpp"


struct DgGraphicsEngineCreateInfo_T {
    std::vector<std::string> vEnabledLayerNames;
    std::vector<std::string> vEnabledExtensionNames;

    DgBool32 enableVulkanDebug;
    std::function<int(DgGPU)> PFN_GPURatingFunc;
};

struct DgGraphicsEngine_T {
    uint32_t _type;
    VkInstance instance;
    std::vector<DgWindow*> vpWindows;
    #ifndef NDEBUG
    VkDebugUtilsMessengerEXT debugMessenger;
    #endif
};


typedef struct DgGraphicsEngineCreateInfo_T DgGraphicsEngineCreateInfo;
typedef struct DgGraphicsEngine_T DgGraphicsEngine;

DGAPI DgResult dgGraphicsInitEngine(DgGraphicsEngineCreateInfo createInfo, std::unique_ptr<DgEngine>& pEngine);
DGAPI DgBool32 dgGraphicsCanEngineClose(std::unique_ptr<DgEngine>& pEngine);
DGAPI DgResult dgGraphicsUpdateEngine(std::unique_ptr<DgEngine>& pEngine);
DGAPI void dgGraphicsDestroyEngine(std::unique_ptr<DgEngine>& pEngine);
