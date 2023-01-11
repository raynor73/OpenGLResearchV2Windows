#include "scene_with_time.h"

using namespace std;
using namespace GameEngine;

void SceneWithTime::update()
{
    auto currentTimestamp = m_timeProvider->getTimestamp();
    if (m_prevTimestampSet) {
        float dt = (currentTimestamp - m_prevTimestamp) / TimeProvider::NANOS_IN_SECOND;
        update(dt);
    }
    m_prevTimestamp = currentTimestamp;
    m_prevTimestampSet = true;
}
