#pragma once

#include <chrono>
#include <cstdint>

namespace GameEngine
{
class TimeProvider {

    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimestamp;

public:
    TimeProvider();

    uint64_t getTimestamp();

    static const float NANOS_IN_SECOND;
};
}
