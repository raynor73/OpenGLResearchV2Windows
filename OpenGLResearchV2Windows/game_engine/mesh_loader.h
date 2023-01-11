#pragma once

#include <string>
#include <game_engine/Mesh.h>

namespace GameEngine
{
class MeshLoader 
{
public:
    virtual ~MeshLoader() = default;

    virtual Mesh loadMesh(const std::string& path) = 0;
};
}
