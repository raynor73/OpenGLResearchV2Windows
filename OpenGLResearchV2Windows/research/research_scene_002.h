#pragma once

#include "research_scene.h"
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_shaders_repository.h>
#include <memory>

class ResearchScene002 : ResearchScene
{
    std::shared_ptr<GameEngine::OpenGLErrorDetector> m_openGLErrorDetector;
    std::shared_ptr<GameEngine::OpenGLShadersRepository> m_shadersRepository;

public:
    ResearchScene002(
        std::shared_ptr<GameEngine::OpenGLErrorDetector> openGLErrorDetector,
        std::shared_ptr<GameEngine::OpenGLShadersRepository> shadersRepository
    ) :
        m_openGLErrorDetector(openGLErrorDetector),
        m_shadersRepository(shadersRepository)
    {}


    void start() override;
    void update() override;
};

