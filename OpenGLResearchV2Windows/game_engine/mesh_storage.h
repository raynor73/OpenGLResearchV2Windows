#pragma once

#include <unordered_map>
#include <string>
#include <game_engine/Mesh.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
class MeshStorage : public WithoutGeneratedMethods
{
    std::unordered_map<std::string, Mesh> m_storage;

public:
    MeshStorage() = default;

    void putMesh(const std::string& name, Mesh mesh);
    Mesh getMesh(const std::string& name) const;
    void removeMesh(const std::string& name);
    void removeAllMeshes();
};
}
