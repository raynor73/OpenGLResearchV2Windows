#pragma once

#include "research_scene.h"
#include <third_party/imgui/imgui.h>
#include <third_party/imgui/imgui_impl_glfw.h>
#include <third_party/imgui/imgui_impl_opengl3.h>
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_shaders_repository.h>
#include <memory>
#include <glm/glm.hpp>
#include <game_engine/bitmap_data_source.h>

class ResearchScene002 : ResearchScene
{
    std::shared_ptr<GameEngine::OpenGLErrorDetector> m_openGLErrorDetector;
    std::shared_ptr<GameEngine::OpenGLShadersRepository> m_shadersRepository;
    std::shared_ptr<GameEngine::BitmapDataSource> m_bitmapDataSource;

    glm::vec3 m_up;

    bool m_isSaved;

    bool m_shouldShowDemoWindow;
    ImVec4 m_clearColor;

    glm::vec3 m_cameraPosition;
    glm::vec3 m_cameraLookAt;

    glm::mat4 m_projection;

    GLuint m_offscreenFrameBufferId;

    void renderUi();

public:
    ResearchScene002(
        std::shared_ptr<GameEngine::OpenGLErrorDetector> openGLErrorDetector,
        std::shared_ptr<GameEngine::OpenGLShadersRepository> shadersRepository,
        std::shared_ptr<GameEngine::BitmapDataSource> bitmapDataSource
    ) :
        m_openGLErrorDetector(openGLErrorDetector),
        m_shadersRepository(shadersRepository),
        m_bitmapDataSource(bitmapDataSource),
        m_up(glm::vec3(0, 1, 0)),
        m_isSaved(false),
        m_shouldShowDemoWindow(true),
        m_clearColor(ImVec4(0.45f, 0.55f, 0.60f, 1.00f))
    {}

    void start() override;
    void update() override;
};

