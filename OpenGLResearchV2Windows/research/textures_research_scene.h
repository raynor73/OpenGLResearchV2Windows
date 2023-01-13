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
#include <game_engine/bitmap_data_source.h>

class TexturesResearchScene : public SceneWithTime
{
    std::shared_ptr<GameEngine::MeshLoader> m_meshLoader;
    std::shared_ptr<GameEngine::BitmapDataSource> m_bitmapDataSource;

    glm::vec3 m_up;
    glm::vec3 m_forward;

    glm::vec3 m_cameraPosition;

    glm::mat4 m_projection;

    FpsCalculator m_fpsCalculator;

    float m_xAngle, m_yAngle, m_zAngle;
    glm::vec3 m_modelScale;
    float m_mipmapBias;

    GameEngine::Mesh m_boxMesh;

    void renderUi();

public:
    TexturesResearchScene(
        std::shared_ptr<GameEngine::OpenGLErrorDetector> openGLErrorDetector,
        std::shared_ptr<GameEngine::OpenGLShadersRepository> shadersRepository,
        std::shared_ptr<GameEngine::TimeProvider> timeProvider,
        std::shared_ptr<GameEngine::MeshLoader> meshLoader,
        std::shared_ptr<GameEngine::BitmapDataSource> bitmapDataSource
    ) : SceneWithTime(openGLErrorDetector, shadersRepository, timeProvider),
        m_meshLoader(meshLoader),
        m_bitmapDataSource(bitmapDataSource),
        m_up(glm::vec3(0, 1, 0)),
        m_forward(glm::vec3(0, 0, -1))
    {}

    void start() override;

protected:
    void update(float dt) override;
};

