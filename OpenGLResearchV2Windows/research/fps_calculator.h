#pragma once

#include <list>

class FpsCalculator
{
    float m_fps = 0;
    float m_internalFps = 0;
    float m_elapsedTime = 0;
    std::list<float> m_buffer;

public:
    float fps() const { return m_fps; }

    void update(float dt);

private:
    const int MAX_BUFFER_SIZE = 100;
    const float REFRESH_PERIOD_SECONDS = 1;
};
