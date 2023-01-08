#pragma once

#include <vector>
#include <string>

namespace GameEngine
{
class FsAbstraction 
{
public:
    virtual ~FsAbstraction() = default;

    virtual bool isFileExists(const std::string& path) = 0;

    virtual std::vector<uint8_t> readBinaryFileContent(const std::string& path) = 0;
    virtual void writeBinaryFileContent(const std::string& path, const std::vector<uint8_t>& data) = 0;

    virtual std::string readTextFileContent(const std::string& path) = 0;
    virtual void writeTextFileContent(const std::string& path, const std::string& text) = 0;

    virtual void deleteFile(const std::string& path) = 0;
};
}
