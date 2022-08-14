#pragma once

#include <memory>
#include <string>

namespace GameEngine
{
class Logger
{
public:
    virtual ~Logger() = default;

    virtual void d(const std::string& tag, const std::string& msg) = 0;
    virtual void w(const std::string& tag, const std::string& msg) = 0;
    virtual void e(const std::string& tag, const std::string& msg) = 0;
    virtual void e(const std::string& tag, const std::string& msg, const std::exception& e) = 0;
};

class L
{
    static std::shared_ptr<Logger> m_logger;

public:
    static void setLogger(std::shared_ptr<Logger> logger) {
        m_logger = logger;
    }

    static void d(const std::string& tag, const std::string& msg) {
        if (m_logger != nullptr) {
            m_logger->d(tag, msg);
        }
    }

    static void w(const std::string& tag, const std::string& msg) {
        if (m_logger != nullptr) {
            m_logger->w(tag, msg);
        }
    }

    static void e(const std::string& tag, const std::string& msg) {
        if (m_logger != nullptr) {
            m_logger->e(tag, msg);
        }
    }

    static void e(const std::string& tag, const std::string& msg, const std::exception& e) {
        if (m_logger != nullptr) {
            m_logger->e(tag, msg, e);
        }
    }
};
}
