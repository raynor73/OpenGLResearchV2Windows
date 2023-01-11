#pragma once

#include "scene_with_time.h"
#include <third_party/imgui/imgui.h>
#include <third_party/imgui/imgui_impl_glfw.h>
#include <third_party/imgui/imgui_impl_opengl3.h>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "fps_calculator.h"
#include <game_engine/mesh_loader.h>

class TexturesResearchScene : public SceneWithTime
{
    std::shared_ptr<GameEngine::MeshLoader> m_meshLoader;

    glm::vec3 m_up;
    glm::vec3 m_forward;

    bool m_isAntiAliasingEnabled;

    glm::vec3 m_cameraPosition;

    glm::mat4 m_projection;

    FpsCalculator m_fpsCalculator;

    float m_zAngle;

    void renderUi();

public:
    TexturesResearchScene(
        std::shared_ptr<GameEngine::OpenGLErrorDetector> openGLErrorDetector,
        std::shared_ptr<GameEngine::OpenGLShadersRepository> shadersRepository,
        std::shared_ptr<GameEngine::TimeProvider> timeProvider,
        std::shared_ptr<GameEngine::MeshLoader> meshLoader
    ) : SceneWithTime(openGLErrorDetector, shadersRepository, timeProvider),
        m_meshLoader(meshLoader),
        m_up(glm::vec3(0, 1, 0)),
        m_forward(glm::vec3(0, 0, -1)),
        m_isAntiAliasingEnabled(false)
    {}

    void start() override;

protected:
    void update(float dt) override;
};

