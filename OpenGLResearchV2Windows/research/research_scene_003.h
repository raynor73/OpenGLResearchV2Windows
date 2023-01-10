#pragma once

#include "research_scene.h"
#include <third_party/imgui/imgui.h>
#include <third_party/imgui/imgui_impl_glfw.h>
#include <third_party/imgui/imgui_impl_opengl3.h>
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_shaders_repository.h>
#include <game_engine/time_provider.h>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "fps_calculator.h"

class ResearchScene003 : public ResearchScene
{
    std::shared_ptr<GameEngine::OpenGLErrorDetector> m_openGLErrorDetector;
    std::shared_ptr<GameEngine::OpenGLShadersRepository> m_shadersRepository;
    std::shared_ptr<GameEngine::TimeProvider> m_timeProvider;

    glm::vec3 m_up;
    glm::vec3 m_forward;
    
    uint64_t m_prevTimestamp;
    bool m_prevTimestampSet;

    glm::vec3 m_cameraPosition;

    glm::mat4 m_projection;

    FpsCalculator m_fpsCalculator;

    float m_zAngle;

    void renderUi();
    void update(float dt);

public:
    ResearchScene003(
        std::shared_ptr<GameEngine::OpenGLErrorDetector> openGLErrorDetector,
        std::shared_ptr<GameEngine::OpenGLShadersRepository> shadersRepository,
        std::shared_ptr<GameEngine::TimeProvider> timeProvider
    ) :
        m_openGLErrorDetector(openGLErrorDetector),
        m_shadersRepository(shadersRepository),
        m_timeProvider(timeProvider),
        m_up(glm::vec3(0, 1, 0)),
        m_forward(glm::vec3(0, 0, -1)),
        m_prevTimestamp(0),
        m_prevTimestampSet(false)
    {}

    void start() override;
    void update() override;
};

