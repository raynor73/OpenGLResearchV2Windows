#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace GameEngine
{
namespace Utils
{
    void logGLString(const std::string& tag, const std::string& nameString, GLenum nameCode);
    GLint glGetInteger(GLenum name);
    GLint64 glGetInteger64(GLenum name);
    void logGLInteger(const std::string& tag, const std::string& nameString, GLenum nameCode);
    void logGLInteger64(const std::string& tag, const std::string& nameString, GLenum nameCode);
}
}

