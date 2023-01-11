#pragma once

#include <vector>
#include <game_engine/vertex.h>

namespace GameEngine
{
class Mesh {

    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;

public:
    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) :
        m_vertices(vertices),
        m_indices(indices)
    {}

    const std::vector<Vertex>& vertices() const { return m_vertices; }
    const std::vector<uint16_t>& indices() const { return m_indices; }

    void setJointIndices(size_t vertexIndex, const glm::ivec3& indices) { m_vertices[vertexIndex].setJointIndices(indices); }
    void setJointWeights(size_t vertexIndex, const glm::vec3 weights) { m_vertices[vertexIndex].setJointWeights(weights); }
};
}
