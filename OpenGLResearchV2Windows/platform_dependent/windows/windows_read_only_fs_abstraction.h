#pragma once

#include <game_engine/read_only_fs_abstraction.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
class WindowsReadOnlyFsAbstraction : public ReadOnlyFsAbstraction, public WithoutGeneratedMethods
{
public:
    virtual bool isFileExists(const std::string& path) override;

    virtual std::vector<uint8_t> readBinaryFileContent(const std::string& path) override;
    virtual std::string readTextFileContent(const std::string& path) override;
};
}
