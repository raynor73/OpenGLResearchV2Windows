#pragma once

#include <game_engine/logger.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine 
{
class WindowsLogger : public Logger, public WithoutGeneratedMethods
{
public:
    virtual void d(const std::string& tag, const std::string& msg) override;
    virtual void w(const std::string& tag, const std::string& msg) override;
    virtual void e(const std::string& tag, const std::string& msg) override;
    virtual void e(const std::string& tag, const std::string& msg, const std::exception& e) override;
};
}
