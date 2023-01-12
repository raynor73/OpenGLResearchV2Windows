#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <gdiplusheaders.h>
#include "windows_bitmap_loader.h"
#include <game_engine/logger.h>
#include <sstream>
#include <stringapiset.h>
#include <gdiplus.h>
#include <gdiplusheaders.h>
#include <platform_dependent/windows/utils.h>

using namespace GameEngine;
using namespace std;
using namespace Gdiplus;
using namespace Windows::Utils;

BitmapInfo WindowsBitmapLoader::loadBitmap(const string& path)
{
    auto bitmap = Bitmap::FromFile(stringToWchar(path).get(), false);

    BitmapInfo bitmapInfo;
    switch (bitmap->GetPixelFormat()) {
    case PixelFormat24bppRGB:
        for (uint32_t y = 0; y < bitmap->GetHeight(); y++) {
            for (uint32_t x = 0; x < bitmap->GetWidth(); x++) {
                Color color;
                bitmap->GetPixel(x, y, &color);

                bitmapInfo.data.push_back(color.GetR());
                bitmapInfo.data.push_back(color.GetG());
                bitmapInfo.data.push_back(color.GetB());
                bitmapInfo.data.push_back(255);
            }
        }
        break;

    case PixelFormat32bppARGB:
        for (uint32_t y = 0; y < bitmap->GetHeight(); y++) {
            for (uint32_t x = 0; x < bitmap->GetWidth(); x++) {
                Color color;
                bitmap->GetPixel(x, y, &color);

                bitmapInfo.data.push_back(color.GetR());
                bitmapInfo.data.push_back(color.GetG());
                bitmapInfo.data.push_back(color.GetB());
                bitmapInfo.data.push_back(color.GetA());
            }
        }
        break;

    default:
        stringstream ss;
        ss << "Unsupported pixel format: " << bitmap->GetPixelFormat();
        throw domain_error(ss.str());
    }

    bitmapInfo.width = bitmap->GetWidth();
    bitmapInfo.height = bitmap->GetHeight();

    delete bitmap;

    return bitmapInfo;
}
