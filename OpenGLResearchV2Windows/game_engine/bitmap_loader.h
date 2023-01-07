#pragma once

#include <string>
#include <game_engine/bitmap_info.h>

namespace GameEngine
{
class BitmapLoader {

public:
    virtual ~BitmapLoader() = default;

    virtual BitmapInfo loadBitmap(const std::string& path) = 0;
};
}
