#pragma once

#include "research_scene.h"
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

    glm::vec3 m_cameraPosition;
    glm::vec3 m_cameraLookAt;

    glm::mat4 m_projection;

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
        m_isSaved(false)
    {}


    void start() override;
    void update() override;
};

