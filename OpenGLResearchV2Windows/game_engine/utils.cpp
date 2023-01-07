#include "utils.h"
#include <sstream>
#include <game_engine/logger.h>

using namespace std;
using namespace GameEngine::Utils;

void GameEngine::Utils::logGLString(const std::string& tag, const std::string& nameString, GLenum nameCode)
{
    stringstream ss;
    ss << nameString << ": " << reinterpret_cast<const char*>(glGetString(nameCode));
    L::d(tag, ss.str());
}

GLint GameEngine::Utils::glGetInteger(GLenum name)
{
    stringstream ss;
    GLint value;

    glGetIntegerv(name, &value);

    return value;
}

GLint64 GameEngine::Utils::glGetInteger64(GLenum name)
{
    stringstream ss;
    GLint64 value;

    glGetInteger64v(name, &value);

    return value;
}

void GameEngine::Utils::logGLInteger(const std::string& tag, const std::string& nameString, GLenum nameCode)
{
    stringstream ss;
    ss << nameString << ": " << glGetInteger(nameCode);
    L::d(tag, ss.str());
}

void GameEngine::Utils::logGLInteger64(const std::string& tag, const std::string& nameString, GLenum nameCode)
{
    stringstream ss;
    ss << nameString << ": " << glGetInteger64(nameCode);
    L::d(tag, ss.str());
}
