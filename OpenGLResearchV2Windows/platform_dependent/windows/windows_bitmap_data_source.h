#pragma once

#include <game_engine/bitmap_data_source.h>
#include <game_engine/bitmap_loader.h>
#include <game_engine/fs_abstraction.h>
#include <memory>

namespace GameEngine
{
class WindowsBitmapDataSource : public BitmapDataSource
{
    std::shared_ptr<BitmapLoader> m_bitmapLoader;
    std::shared_ptr<FsAbstraction> m_fsAbstraction;

public:
    WindowsBitmapDataSource(std::shared_ptr<BitmapLoader> bitmapLoader, std::shared_ptr<FsAbstraction> fsAbstraction);

    BitmapInfo loadBitmap(const std::string& path) override;

    void saveBitmap(const BitmapInfo& bitmapInfo, const std::string& path) override;
};
}
