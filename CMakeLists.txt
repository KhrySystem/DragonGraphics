option(DragonGraphics_2D_RENDER ON)

include(cmake/create_shader.cmake)

add_subdirectory(external/glfw)
add_subdirectory(src)