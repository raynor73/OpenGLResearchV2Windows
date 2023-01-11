#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "windows_mesh_loader.h"
#include <game_engine/logger.h>
#include <sstream>

using namespace GameEngine;
using namespace std;

Mesh GameEngine::WindowsMeshLoader::loadMesh(const std::string& path)
{
    vector<Vertex> vertices;
    vector<uint16_t> indices;

    auto meshBytes = m_readOnlyFsAbstraction->readBinaryFileContent(path);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(
        meshBytes.data(),
        meshBytes.size(),
        aiProcess_Triangulate,
        "obj"
    );

    if (scene != nullptr) {
        if (scene->mNumMeshes > 0) {
            auto assimpMesh = scene->mMeshes[0];

            if (assimpMesh->mNumVertices == 0) {
                std::stringstream ss;
                ss << "No vertices found for mesh " << assimpMesh->mName.C_Str() << " in " << path;
                throw std::domain_error(ss.str());
            }
            if (assimpMesh->mNumFaces == 0) {
                std::stringstream ss;
                ss << "No faces found for mesh " << assimpMesh->mName.C_Str() << " in " << path;
                throw std::domain_error(ss.str());
            }

            for (uint32_t faceIndex = 0; faceIndex < assimpMesh->mNumFaces; faceIndex++) {
                auto face = assimpMesh->mFaces[faceIndex];

                if (face.mNumIndices != 3) {
                    std::stringstream ss;
                    ss << "Wrong indices count: " << face.mNumIndices << " for one of face of mesh \"" << assimpMesh->mName.C_Str() << "\" in " << path;
                    throw std::domain_error(ss.str());
                }

                for (uint32_t i = 0; i < face.mNumIndices; i++) {
                    auto index = face.mIndices[i];

                    auto assimpVertex = assimpMesh->mVertices[index];
                    auto assimpNormal = assimpMesh->mNormals[index];
                    auto assimpUv = assimpMesh->mTextureCoords[0][index];

                    Vertex vertex{
                        glm::vec3 { assimpVertex.x, assimpVertex.y, assimpVertex.z },
                        glm::vec3 { assimpNormal.x, assimpNormal.y, assimpNormal.z },
                        glm::vec2 { assimpUv.x, assimpUv.y },
                        glm::ivec3(0),
                        glm::vec3(0)
                    };

                    vertices.push_back(vertex);
                    indices.push_back(index);
                }
            }
        } else {
            std::stringstream ss;
            ss << "No meshes found in " << path;
            throw std::domain_error(ss.str());
        }
    } else {
        std::stringstream ss;
        ss << "Error importing mesh " << path;
        throw std::domain_error(ss.str());
    }

    return Mesh{ vertices, indices };
}
