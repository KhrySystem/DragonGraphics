option(DragonGraphics_2D_RENDER ON)

option(Dragon_2D_RENDER "Toggle to optionally force only 2D graphics (No camera initialization, shaders take in 2d position, etc.)"  ON)

if(NOT DEFINED Dragon_MAX_POINT_LIGHTS)
	set(Dragon_MAX_POINT_LIGHTS 128)
endif()

if(NOT DEFINED Dragon_MAX_DIRECTIONAL_LIGHTS)
	set(Dragon_MAX_DIRECTIONAL_LIGHTS 4)
endif()

if(NOT DEFINED Dragon_FRAME_RENDER_COUNT)
    set(Dragon_FRAME_RENDER_COUNT 2)
endif()

set(GLFW_BUILD_DOCS ${Dragon_BUILD_DEPENDENCY_DOCS})
set(GLFW_INSTALL ${Dragon_INSTALL})

include(cmake/create_shader.cmake)

add_subdirectory(external/glm)
add_subdirectory(external/glfw)

add_library(dggraphics-${DragonEngine_VERSION_MAJOR}
    "src/engine.cpp"
    "src/rotation.cpp"
    "src/window.cpp"
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/dragon/graphics/config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/dragon/graphics/config.hpp)

# Specify the include directories for the library
target_include_directories(dggraphics-${DragonEngine_VERSION_MAJOR} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
)

target_link_libraries(dggraphics-${DragonEngine_VERSION_MAJOR}
    PUBLIC
        glfw
        glm::glm
        Dragon::Core
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(dggraphics-${DragonEngine_VERSION_MAJOR} PUBLIC DRAGON_GRAPHICS_DLL)
    target_compile_definitions(dggraphics-${DragonEngine_VERSION_MAJOR} PRIVATE DRAGON_GRAPHICS_BUILD_DLL)
endif()

if(Dragon_2D_RENDER)
    target_compile_definitions(dggraphics-${DragonEngine_VERSION_MAJOR} PUBLIC Dragon_2D_RENDER)
endif()

# Create an alias for the library
add_library(Dragon::Graphics ALIAS dggraphics-${DragonEngine_VERSION_MAJOR})