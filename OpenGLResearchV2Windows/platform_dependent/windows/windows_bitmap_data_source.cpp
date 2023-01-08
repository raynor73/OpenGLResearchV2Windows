#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <gdiplusheaders.h>
#include "windows_bitmap_data_source.h"
#include <platform_dependent/windows/utils.h>
#include <exception>

using namespace GameEngine;
using namespace std;
using namespace Windows::Utils;
using namespace Gdiplus;

WindowsBitmapDataSource::WindowsBitmapDataSource(
    shared_ptr<BitmapLoader> bitmapLoader, 
    shared_ptr<FsAbstraction> fsAbstraction
) : m_bitmapLoader(bitmapLoader), m_fsAbstraction(fsAbstraction)
{}

BitmapInfo WindowsBitmapDataSource::loadBitmap(const string& path)
{
    return m_bitmapLoader->loadBitmap(path);
}

void WindowsBitmapDataSource::saveBitmap(const BitmapInfo& bitmapInfo, const string& path)
{
    auto bitmap = new Bitmap(bitmapInfo.width, bitmapInfo.height, PixelFormat24bppRGB);

    for (uint32_t y = 0; y < bitmap->GetHeight(); y++) {
        for (uint32_t x = 0; x < bitmap->GetWidth(); x++) {
            auto basePixelIndex = (x + bitmap->GetWidth() * y) * 4;
            Color color(
                bitmapInfo.data[basePixelIndex],
                bitmapInfo.data[basePixelIndex + 1],
                bitmapInfo.data[basePixelIndex + 2]
            );
            bitmap->SetPixel(x, bitmap->GetHeight() - 1 - y, color);
        }
    }

    CLSID pngClassId;
    if (getEncoderClassId(L"image/png", &pngClassId) < 0) {
        throw domain_error("Can't get image/png encoder class ID");
    }
    if (bitmap->Save(stringToWchar(path).get(), &pngClassId, nullptr) != Ok) {
        throw domain_error("Can't save image");
    }

    delete bitmap;
}
