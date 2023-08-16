#include <dragon/core.hpp>
#include <dragon/graphics.hpp>

void _safeAddToVector(std::vector<const char*> &vector, const char* value) {
    for(const char* v: vector) {
        if(strcmp(v, value) == 0) {
            return;
        }
    }
    vector.push_back(value);
}

DGAPI DgResult dgInitGraphicsEngine(DgEngine &engine, DgEngineCreateInfo &createInfo, DgGraphicsEngineCreateInfo &graphicsCreateInfo) {
    for(auto module : engine.submodules) {
        if(module._type == DRAGON_GRAPHICS_ENGINE_TYPE)
    }
    return DG_SUCCESS;
}