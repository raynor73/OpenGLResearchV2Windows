#pragma once

#include <memory>
#include <game_engine/bitmap_loader.h>

namespace GameEngine
{
class ServiceLocator;

class WindowsBitmapLoader : public BitmapLoader
{
public:
    virtual BitmapInfo loadBitmap(const std::string& path) override;
};
}
