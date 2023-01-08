#pragma once

#include <string>
#include <vector>

namespace GameEngine
{
class ReadOnlyFsAbstraction {

public:
    virtual ~ReadOnlyFsAbstraction() = default;

    virtual bool isFileExists(const std::string& path) = 0;

    virtual std::vector<uint8_t> readBinaryFileContent(const std::string& path) = 0;
    virtual std::string readTextFileContent(const std::string& path) = 0;
};
}