#include "windows_read_only_fs_abstraction.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <game_engine/logger.h>

using namespace GameEngine;
using namespace std;

bool WindowsReadOnlyFsAbstraction::isFileExists(const string& path)
{
    ifstream f(path.c_str());
    return f.good();
}

vector<uint8_t> WindowsReadOnlyFsAbstraction::readBinaryFileContent(const string& path)
{
    ifstream f(path.c_str(), ios::in | ios::binary);
    auto fileSize = filesystem::file_size(path);
    vector<uint8_t> buffer(fileSize);
    f.read(reinterpret_cast<char *>(buffer.data()), fileSize);
    f.close();

    return buffer;
}

string WindowsReadOnlyFsAbstraction::readTextFileContent(const string& path)
{
    stringstream ss;

    ifstream f(path.c_str(), ios::in);
    ss << f.rdbuf();
    f.close();

    return ss.str();
}
