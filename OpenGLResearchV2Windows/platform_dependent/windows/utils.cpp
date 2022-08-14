#include "utils.h"

using namespace std;

bool GameEngine::Windows::Utils::redirectConsoleIO()
{
    bool result = true;
    FILE* fp;

    // Redirect STDIN if the console has an input handle
    if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
        if (freopen_s(&fp, "CONIN$", "r", stdin) != 0)
            result = false;
        else
            setvbuf(stdin, NULL, _IONBF, 0);

    // Redirect STDOUT if the console has an output handle
    if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
        if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
            result = false;
        else
            setvbuf(stdout, NULL, _IONBF, 0);

    // Redirect STDERR if the console has an error handle
    if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
        if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0)
            result = false;
        else
            setvbuf(stderr, NULL, _IONBF, 0);

    // Make C++ standard streams point to console as well.
    std::ios::sync_with_stdio(true);

    // Clear the error state for each of the C++ standard streams.
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();

    return result;
}

bool GameEngine::Windows::Utils::releaseConsole()
{
    bool result = true;
    FILE* fp;

    // Just to be safe, redirect standard IO to NUL before releasing.

    // Redirect STDIN to NUL
    if (freopen_s(&fp, "NUL:", "r", stdin) != 0)
        result = false;
    else
        setvbuf(stdin, NULL, _IONBF, 0);

    // Redirect STDOUT to NUL
    if (freopen_s(&fp, "NUL:", "w", stdout) != 0)
        result = false;
    else
        setvbuf(stdout, NULL, _IONBF, 0);

    // Redirect STDERR to NUL
    if (freopen_s(&fp, "NUL:", "w", stderr) != 0)
        result = false;
    else
        setvbuf(stderr, NULL, _IONBF, 0);

    // Detach from console
    if (!FreeConsole())
        result = false;

    return result;
}

void GameEngine::Windows::Utils::adjustConsoleBuffer(int16_t minLength)
{
    // Set the screen buffer to be big enough to scroll some text
    CONSOLE_SCREEN_BUFFER_INFO conInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo);
    if (conInfo.dwSize.Y < minLength)
        conInfo.dwSize.Y = minLength;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), conInfo.dwSize);
}

bool GameEngine::Windows::Utils::createNewConsole(int16_t minLength)
{
    bool result = false;

    // Release any current console and redirect IO to NUL
    releaseConsole();

    // Attempt to create new console
    if (AllocConsole())
    {
        adjustConsoleBuffer(minLength);
        result = redirectConsoleIO();
    }

    return result;
}

bool GameEngine::Windows::Utils::attachParentConsole(int16_t minLength)
{
    bool result = false;

    // Release any current console and redirect IO to NUL
    releaseConsole();

    // Attempt to attach to parent process's console
    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
        adjustConsoleBuffer(minLength);
        result = redirectConsoleIO();
    }

    return result;
}

std::shared_ptr<WCHAR[]> GameEngine::Windows::Utils::getString(HINSTANCE hInstance, uint32_t resId)
{
    auto wcString = shared_ptr<WCHAR[]>(new WCHAR[MAX_LOADSTRING]);

    LoadStringW(hInstance, resId, wcString.get(), MAX_LOADSTRING);

    return wcString;
}

void GameEngine::Windows::Utils::showDialog(HWND hWnd, WCHAR* title, WCHAR* message)
{
    MessageBox(hWnd, message, title, MB_OK);
}

void GameEngine::Windows::Utils::showDialog(WCHAR* title, WCHAR* message)
{
    showDialog(NULL, title, message);
}
