#include "windows_fs_abstraction.h"
#include <filesystem>
#include <fstream>

using namespace GameEngine;
using namespace std;

GameEngine::WindowsFsAbstraction::WindowsFsAbstraction(
    std::shared_ptr<ReadOnlyFsAbstraction> readOnlyFsAbstraction
) : m_readOnlyFsAbstraction(readOnlyFsAbstraction)
{}

bool WindowsFsAbstraction::isFileExists(const string& path)
{
    return m_readOnlyFsAbstraction->isFileExists(path);
}

vector<uint8_t> WindowsFsAbstraction::readBinaryFileContent(const string& path)
{
    return m_readOnlyFsAbstraction->readBinaryFileContent(path);
}

void WindowsFsAbstraction::writeBinaryFileContent(const string& path, const vector<uint8_t>& data)
{
    ofstream f(path.c_str(), ios::out | ios::binary);
    f.write(reinterpret_cast<const char *>(data.data()), data.size());
    f.close();
}

string WindowsFsAbstraction::readTextFileContent(const string& path)
{
    return m_readOnlyFsAbstraction->readTextFileContent(path);
}

void WindowsFsAbstraction::writeTextFileContent(const string& path, const string& text)
{
    ofstream f(path.c_str(), ios::out);
    f << text;
    f.close();
}

void WindowsFsAbstraction::deleteFile(const string& path)
{
    filesystem::remove(path);
}
