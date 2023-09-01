include(${DragonEngine_SOURCE_DIR}/cmake/bin2h.cmake) 

# Create a library with the given name, and specify the sources to be compiled
add_library(dggraphics-${DragonEngine_VERSION_MAJOR}
    "engine_init.cpp"
    "engine_runtime.cpp"
    "engine_close.cpp"
)
if(BUILD_SHARED_LIBS)
	# Define the preprocessor symbol for shared library build
	target_compile_definitions(dggraphics-${DragonEngine_VERSION_MAJOR} PUBLIC -DDRAGON_BUILD_DLL)
	add_compile_definitions(DRAGON_DLL)
endif()

# Specify the include directories for the library
target_include_directories(dggraphics-${DragonEngine_VERSION_MAJOR} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/glfw
)

target_link_libraries(dggraphics-${DragonEngine_VERSION_MAJOR}
    PUBLIC
        glfw
        Dragon::Core
)

# Create an alias for the library
add_library(Dragon::Graphics ALIAS dggraphics-${DragonEngine_VERSION_MAJOR})
