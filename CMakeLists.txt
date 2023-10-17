option(DragonGraphics_2D_RENDER ON)

add_subdirectory(external/glm)
add_subdirectory(external/glfw)

add_library(dggraphics-${DragonEngine_VERSION_MAJOR}
    "src/Engine.cpp"
    "src/Window.cpp"
)

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

# Create an alias for the library
add_library(Dragon::Graphics ALIAS dggraphics-${DragonEngine_VERSION_MAJOR})