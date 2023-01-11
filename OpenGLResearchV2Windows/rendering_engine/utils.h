#pragma once

#include <vector>
#include <game_engine/mesh.h>

#define BUFFER_OFFSET(a) ((void*)(a))

namespace GameEngine
{
namespace RenderingEngine
{
namespace Utils
{
    std::vector<float> prepareVertexData(const GameEngine::Mesh& mesh);
}
}
}
