include(${DragonEngine_SOURCE_DIR}/cmake/bin2h.cmake) 

# Create a library with the given name, and specify the sources to be compiled
add_library(dggraphics
    "engine_init.cpp"
)
if(BUILD_SHARED_LIBS)
	# Define the preprocessor symbol for shared library build
	target_compile_definitions(dggraphics PUBLIC -DDRAGON_BUILD_DLL)
	add_compile_definitions(DRAGON_DLL)
endif()

# get shader files
if(Dragon_2D_RENDER)
    set(Dragon_VERT_SHADER ${CMAKE_CURRENT_SOURCE_DIR}/shaders/2d.vert.in)
    set(Dragon_FRAG_SHADER ${CMAKE_CURRENT_SOURCE_DIR}/shaders/2d.simple.frag.in)
else()
    set(Dragon_VERT_SHADER ${CMAKE_CURRENT_SOURCE_DIR}/shaders/3d.simple.vert.in)
    set(Dragon_FRAG_SHADER ${CMAKE_CURRENT_SOURCE_DIR}/shaders/3d.simple.frag.in)
endif()


configure_file(${Dragon_FRAG_SHADER} ${CMAKE_CURRENT_BINARY_DIR}/shaders/shader.frag)
configure_file(${Dragon_VERT_SHADER} ${CMAKE_CURRENT_BINARY_DIR}/shaders/shader.vert)

if(${Vulkan_glslc_FOUND})
    message(STATUS "Using glslc to build SPIR-V Binaries")
    execute_process(
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/src/shaders/shader.frag -o ${CMAKE_CURRENT_BINARY_DIR}/src/shaders/frag.spv --target-env=vulkan1.2
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/src/shaders/shader.vert -o ${CMAKE_CURRENT_BINARY_DIR}/src/shaders/vert.spv --target-env=vulkan1.2
        OUTPUT_QUIET
    )

    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/frag.hpp")
    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/vert.hpp")
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/src/shaders/frag.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/frag.hpp" VARIABLE_NAME DRAGON_FRAG_SHADER APPEND NULL_TERMINATE)
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/src/shaders/vert.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/vert.hpp" VARIABLE_NAME DRAGON_VERT_SHADER APPEND NULL_TERMINATE)
elseif(${Vulkan_glslangValidator_FOUND})
    message(STATUS "Using glslangValidator to build SPIR-V Binaries")

    execute_process(
        COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${CMAKE_CURRENT_BINARY_DIR}/src/shaders/shader.frag -o ${CMAKE_CURRENT_BINARY_DIR}/src/shaders/frag.spv --target-env vulkan1.2
        COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${CMAKE_CURRENT_BINARY_DIR}/src/shaders/shader.vert -o ${CMAKE_CURRENT_BINARY_DIR}/src/shaders/vert.spv --target-env vulkan1.2
        OUTPUT_QUIET
    )

    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/frag.hpp")
    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/vert.hpp")
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/src/shaders/frag.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/frag.hpp" VARIABLE_NAME DRAGON_FRAG_SHADER APPEND NULL_TERMINATE)
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/src/shaders/vert.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/vert.hpp" VARIABLE_NAME DRAGON_VERT_SHADER APPEND NULL_TERMINATE)
else()
    message(WARNING "Using prebuilt SPIR-V Binaries")
endif()

# Specify the include directories for the library
target_include_directories(dggraphics PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/glfw
)

target_link_libraries(dggraphics
    PUBLIC
        glfw
        Dragon::Core
)

# Create an alias for the library
add_library(Dragon::Graphics ALIAS dggraphics)
