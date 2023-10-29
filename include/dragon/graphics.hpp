#pragma once

/**
 * @defgroup Graphics Graphics Engine
 * 
 * @brief Structs and classes pertaining to the Graphics Engine
*/

/* DGAPI is used to declare public API functions for export
 * from the DLL / shared library / dynamic library.
 */
#if defined(_WIN32) && defined(DRAGON_GRAPHICS_BUILD_DLL)
 /* We are building Dragon as a Win32 DLL */
 #define DGGRAPHICSAPI __declspec(dllexport)
#elif defined(_WIN32) && defined(DRAGON_GRAPHICS_DLL)
 /* We are calling a Win32 DLL */
 #define DGGRAPHICSAPI __declspec(dllimport)
#elif defined(__GNUC__) && defined(DRAGON_GRAPHICS_BUILD_DLL)
 /* We are building Dragon as a Unix shared library */
 #define DGGRAPHICSAPI __attribute__((visibility("default")))
#else
 #define DGGRAPHICSAPI
#endif

// Graphics dependencies provided by DragonEngine
#include <dragon/core.hpp>

// General graphics dependencies 
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Graphics src
#include "graphics/aliases.hpp"
#include "graphics/camera.hpp"
#include "graphics/config.hpp"
#include "graphics/engine.hpp"
#include "graphics/model.hpp"
#include "graphics/rotation.hpp"
#include "graphics/swapchain_builder.hpp"
#include "graphics/swapchain.hpp"
#include "graphics/vertex.hpp"
#include "graphics/window.hpp"
