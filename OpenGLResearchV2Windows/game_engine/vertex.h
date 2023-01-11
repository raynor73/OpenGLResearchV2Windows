#pragma once

#include <glm/glm.hpp>

namespace GameEngine
{
class Vertex {

    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec2 m_uv;
    glm::ivec3 m_jointIndices;
    glm::vec3 m_jointWeights;

public:
    static constexpr int VERTEX_POSITION_COMPONENTS = 3;
    static constexpr int VERTEX_NORMAL_COMPONENTS = 3;
    static constexpr int VERTEX_UV_COMPONENTS = 2;
    static constexpr int VERTEX_JOINT_INDICES_COMPONENTS = 3;
    static constexpr int VERTEX_JOINT_WEIGHTS_COMPONENTS = 3;
    static constexpr int VERTEX_COMPONENTS =
        VERTEX_POSITION_COMPONENTS +
        VERTEX_NORMAL_COMPONENTS +
        VERTEX_UV_COMPONENTS +
        VERTEX_JOINT_INDICES_COMPONENTS +
        VERTEX_JOINT_WEIGHTS_COMPONENTS;

    Vertex(
        const glm::vec3& position,
        const glm::vec3& normal,
        const glm::vec2& uv,
        const glm::ivec3& jointIndices,
        const glm::vec3& jointWeights
    ) :
        m_position(position),
        m_normal(normal),
        m_uv(uv),
        m_jointIndices(jointIndices),
        m_jointWeights(jointWeights)
    {}

    const glm::vec3& position() const { return m_position; }
    void setPosition(const glm::vec3& position) { m_position = position; }

    const glm::vec3& normal() const { return m_normal; }
    void setNormal(const glm::vec3& normal) { m_normal = normal; }

    const glm::vec2& uv() const { return m_uv; }
    void setUv(const glm::vec2& uv) { m_uv = uv; }

    const glm::ivec3& jointIndices() const { return m_jointIndices; }
    void setJointIndices(const glm::ivec3& jointIndices) { m_jointIndices = jointIndices; }

    const glm::vec3& jointWeights() const { return m_jointWeights; }
    void setJointWeights(const glm::vec3& jointWeights) { m_jointWeights = jointWeights; }
};
}
