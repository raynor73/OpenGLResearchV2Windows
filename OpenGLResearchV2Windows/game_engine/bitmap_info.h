#pragma once

#include <vector>

namespace GameEngine
{
struct BitmapInfo {
    std::vector<uint8_t> data;
    uint32_t width;
    uint32_t height;
};
}
