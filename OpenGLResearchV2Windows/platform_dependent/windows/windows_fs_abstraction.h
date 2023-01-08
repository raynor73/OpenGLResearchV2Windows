#pragma once

#include <memory>
#include <game_engine/fs_abstraction.h>
#include <game_engine/read_only_fs_abstraction.h>

namespace GameEngine
{
class ServiceLocator;

class WindowsFsAbstraction : public FsAbstraction
{
    std::shared_ptr<ReadOnlyFsAbstraction> m_readOnlyFsAbstraction;

public:
    explicit WindowsFsAbstraction(std::shared_ptr<ReadOnlyFsAbstraction> readOnlyFsAbstraction);

    virtual bool isFileExists(const std::string& path);

    virtual std::vector<uint8_t> readBinaryFileContent(const std::string& path);
    virtual void writeBinaryFileContent(const std::string& path, const std::vector<uint8_t>& data);

    virtual std::string readTextFileContent(const std::string& path);
    virtual void writeTextFileContent(const std::string& path, const std::string& text);

    virtual void deleteFile(const std::string& path);
};
}
