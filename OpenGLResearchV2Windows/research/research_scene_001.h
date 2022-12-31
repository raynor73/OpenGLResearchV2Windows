#pragma once

#include "research_scene.h"
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_shaders_repository.h>
#include <memory>

class ResearchScene001 : public ResearchScene
{
    std::shared_ptr<GameEngine::OpenGLErrorDetector> m_openGLErrorDetector;
    std::shared_ptr<GameEngine::OpenGLShadersRepository> m_shadersRepository;

    const char* g_vertexShaderSource = R"(
#version 430 core

layout(location = 0) in vec4 vPosition;

void main() {
    gl_Position = vPosition;
}
)";
    const char* g_fragmentShaderSource = R"(
#version 430 core

out vec4 fColor;

void main() {
    fColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)";
    enum VAO_IDs { Triangles, NumVAOs };
    enum Buffer_IDs { ArrayBuffer, NumBuffers };
    enum Attrib_IDs { vPosition = 0 };
    GLuint VAOs[NumVAOs];
    GLuint Buffers[NumBuffers];
    static const GLuint NumVertices = 6;

public:
    ResearchScene001(
        std::shared_ptr<GameEngine::OpenGLErrorDetector> openGLErrorDetector,
        std::shared_ptr<GameEngine::OpenGLShadersRepository> shadersRepository
    ) : 
        m_openGLErrorDetector(openGLErrorDetector),
        m_shadersRepository(shadersRepository)
    {}

    void start() override;
    void update() override;
};
