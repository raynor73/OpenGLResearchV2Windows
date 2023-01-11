#pragma once

#include <game_engine/mesh_loader.h>
#include <game_engine/read_only_fs_abstraction.h>
#include <game_engine/without_generated_methods.h>
#include <memory>

namespace GameEngine
{
class WindowsMeshLoader : public MeshLoader, public WithoutGeneratedMethods
{
    std::shared_ptr<ReadOnlyFsAbstraction> m_readOnlyFsAbstraction;

public:
    WindowsMeshLoader(
        std::shared_ptr<ReadOnlyFsAbstraction> readOnlyFsAbstraction
    ) : 
        m_readOnlyFsAbstraction(readOnlyFsAbstraction)
    {}

    virtual Mesh loadMesh(const std::string& path) override;
};
}
