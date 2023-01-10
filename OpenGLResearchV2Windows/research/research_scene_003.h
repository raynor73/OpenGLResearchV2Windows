#pragma once

#include "research_scene.h"
#include <third_party/imgui/imgui.h>
#include <third_party/imgui/imgui_impl_glfw.h>
#include <third_party/imgui/imgui_impl_opengl3.h>
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_shaders_repository.h>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class ResearchScene003 : public ResearchScene
{
    std::shared_ptr<GameEngine::OpenGLErrorDetector> m_openGLErrorDetector;
    std::shared_ptr<GameEngine::OpenGLShadersRepository> m_shadersRepository;

    glm::vec3 m_up;

    glm::vec3 m_cameraPosition;
    glm::vec3 m_cameraLookAt;

    glm::mat4 m_projection;

    void renderUi();

public:
    ResearchScene003(
        std::shared_ptr<GameEngine::OpenGLErrorDetector> openGLErrorDetector,
        std::shared_ptr<GameEngine::OpenGLShadersRepository> shadersRepository
    ) :
        m_openGLErrorDetector(openGLErrorDetector),
        m_shadersRepository(shadersRepository),
        m_up(glm::vec3(0, 1, 0))
    {}

    void start() override;
    void update() override;
};

