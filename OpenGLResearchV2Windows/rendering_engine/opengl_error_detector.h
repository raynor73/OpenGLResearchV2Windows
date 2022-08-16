#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <string>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
class OpenGLErrorDetector : public WithoutGeneratedMethods
{
    bool m_isOpenGLErrorDetected;
    GLchar* m_infoBuffer;

    static const std::unordered_map<GLenum, const std::string> s_openGLErrorMap;
    static const std::unordered_map<GLenum, const std::string> s_framebufferStatusMap;

    static const std::string LOG_TAG;
    static const int INFO_BUFFER_SIZE = 1024;

public:
    OpenGLErrorDetector();
    virtual ~OpenGLErrorDetector();

    bool isOpenGLErrorDetected() const { return m_isOpenGLErrorDetected; }

    void checkOpenGLErrors(const std::string& locationName);
    void checkShaderCompilationError(GLuint shader, const std::string& locationName);
    void checkShaderLinkingError(GLuint shader, const std::string& locationName);
    void checkFramebufferStatus(const std::string& locationName);
};
}
