#include "fps_calculator.h"

void FpsCalculator::update(float dt) {
    m_buffer.push_back(dt);
    if (m_buffer.size() > MAX_BUFFER_SIZE) {
        m_buffer.pop_front();
    }

    float sum = 0;
    for (auto value : m_buffer) {
        sum += value;
    }

    m_internalFps = m_buffer.size() / sum;

    m_elapsedTime += dt;
    if (m_elapsedTime >= REFRESH_PERIOD_SECONDS) {
        m_elapsedTime = 0;
        m_fps = m_internalFps;
    }
}
