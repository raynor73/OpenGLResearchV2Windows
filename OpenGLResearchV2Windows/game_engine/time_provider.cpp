#include "time_provider.h"

using namespace GameEngine;
using namespace std;

TimeProvider::TimeProvider() {
    m_startTimestamp = chrono::high_resolution_clock::now();
}

uint64_t TimeProvider::getTimestamp() {
    return (chrono::high_resolution_clock::now() - m_startTimestamp).count();
}

const float TimeProvider::NANOS_IN_SECOND = 1e9f;
