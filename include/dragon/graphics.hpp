#pragma once

#include <dragon/core.hpp>

#include "graphics/camera.hpp"
//#include "graphics/dyload.hpp"
#include "graphics/engine.hpp"
#include "graphics/gpu_queue_families.hpp"
#include "graphics/model_create_info.hpp"
#include "graphics/model.hpp"
#include "graphics/swap_chain_support.hpp"
#include "graphics/vertex.hpp"
#include "graphics/vulkan_debug.hpp"
#include "graphics/window_create_info.hpp"
#include "graphics/window.hpp"

#ifdef DRAGON_2D_RENDER
    #include "graphics/shaders/2d.frag.hpp"
    #include "graphics/shaders/2d.vert.hpp"
#else
    #include "graphics/shaders/3d.frag.hpp"
    #include "graphics/shaders/3d.vert.hpp"
#endif

/*
namespace Dragon {
    // Classes
    typedef DgGrEngine Engine;
    typedef DgWindow Window;
    typedef DgGPU GPU;

    // Enums and Structures
    typedef DgResult Result;
    typedef DgEngineCreateInfo EngineCreateInfo;

    // Creation Functions
    constexpr auto createEngine = dgCreateEngine;
    constexpr auto createWindow = dgCreateWindow;
    // Runtime Functions
    constexpr auto canEngineClose = dgCanEngineClose;
    constexpr auto updateEngine = dgUpdateEngine;
    // Destruction Functions
    constexpr auto destroyEngine = dgDestroyEngine;
    constexpr auto destroyWindow = dgDestroyWindow;
};*/