#pragma once

#include <cstdio>
#include <ios>
#include <iostream>
#include <Windows.h>
#include <memory>

#define MAX_LOADSTRING 100

namespace GameEngine
{
namespace Windows
{
namespace Utils
{
    bool redirectConsoleIO();
    bool releaseConsole();
    void adjustConsoleBuffer(int16_t minLength);
    bool createNewConsole(int16_t minLength);
    bool attachParentConsole(int16_t minLength);

    std::shared_ptr<WCHAR[]> getString(HINSTANCE hInstance, uint32_t resId);
    void showDialog(HWND hWnd, WCHAR* title, WCHAR* message);
    void showDialog(WCHAR* title, WCHAR* message);
}
}
}
