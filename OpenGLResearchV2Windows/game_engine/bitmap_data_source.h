#pragma once

#include <game_engine/bitmap_info.h>
#include <string>

namespace GameEngine
{
class BitmapDataSource {

public:
    virtual BitmapInfo loadBitmap(const std::string& path) = 0;

    virtual void saveBitmap(const BitmapInfo& bitmapInfo, const std::string& path) = 0;
};
}