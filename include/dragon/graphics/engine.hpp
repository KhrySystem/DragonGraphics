#pragma once

#include <dragon/core.hpp>
#include "window.hpp"


struct DgGraphicsEngineCreateInfo_T {
    std::vector<std::string> vEnabledLayerNames;
    std::vector<std::string> vEnabledExtensionNames;

    DgBool32 enableVulkanDebug;
    std::function<int(DgGPU)> PFN_GPURatingFunc;
};

typedef struct DgGraphicsEngineCreateInfo_T DgGraphicsEngineCreateInfo;

typedef class DgGraphicsEngine_T {
    std::vector<DgWindow*> vpWindows;

    DgEngine* owner;
    public:
        DgGraphicsEngine_T(DgEngine &engine, DgEngineCreateInfo &createInfo, DgGraphicsEngineCreateInfo &graphicsCreateInfo);
        DgResult update();
        void close();
    
} DgGraphicsEngine;
