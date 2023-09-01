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

DGAPI DgGraphicsEngine::DgGraphicsEngine_T(DgEngine &engine, DgEngineCreateInfo &createInfo, DgGraphicsEngineCreateInfo &graphicsCreateInfo) {
    size_t index = engine.submodules.size();
    engine.submodules.push_back(self);
    DgGraphicsEngine* pEngine = std::any_cast<DgGraphicsEngine*>engine.submodules.at(index);
}