#pragma once

#include "research_scene.h"
#include <game_engine/time_provider.h>
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_shaders_repository.h>
#include <memory>

class SceneWithTime : public ResearchScene
{
private:
    uint64_t m_prevTimestamp;
    bool m_prevTimestampSet;

protected:
    std::shared_ptr<GameEngine::OpenGLErrorDetector> m_openGLErrorDetector;
    std::shared_ptr<GameEngine::OpenGLShadersRepository> m_shadersRepository;
    std::shared_ptr<GameEngine::TimeProvider> m_timeProvider;

public:
    SceneWithTime(
        std::shared_ptr<GameEngine::OpenGLErrorDetector> openGLErrorDetector,
        std::shared_ptr<GameEngine::OpenGLShadersRepository> shadersRepository,
        std::shared_ptr<GameEngine::TimeProvider> timeProvider
    ) : 
        m_openGLErrorDetector(openGLErrorDetector),
        m_shadersRepository(shadersRepository),
        m_timeProvider(timeProvider),
        m_prevTimestamp(0),
        m_prevTimestampSet(false)
    {}

    virtual void update() override;

protected:
    virtual void update(float dt) = 0;
};
