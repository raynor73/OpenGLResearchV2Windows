#include "opengl_error_detector.h"
#include <sstream>
#include <game_engine/logger.h>

using namespace GameEngine;
using namespace std;

OpenGLErrorDetector::OpenGLErrorDetector() : m_isOpenGLErrorDetected(false) {
    m_infoBuffer = new GLchar[INFO_BUFFER_SIZE];
}

OpenGLErrorDetector::~OpenGLErrorDetector() {
    delete m_infoBuffer;
}

void OpenGLErrorDetector::checkOpenGLErrors(const string& locationName) {
    auto error = glGetError();
    while (error != GL_NO_ERROR) {
        m_isOpenGLErrorDetected = true;

        string errorDescription;
        if (s_openGLErrorMap.count(error) > 0) {
            errorDescription = s_openGLErrorMap.at(error);
        }
        else {
            stringstream ss;
            ss << "Unknown error " << error;
            errorDescription = ss.str();
        }
        stringstream ss;
        ss << "OpenGL error detected at " << locationName << ": " << errorDescription;
        L::d(LOG_TAG, ss.str());

        error = glGetError();
    }
}

void OpenGLErrorDetector::checkShaderCompilationError(GLuint shader, const string& locationName) {
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        m_isOpenGLErrorDetected = true;

        glGetShaderInfoLog(shader, INFO_BUFFER_SIZE, nullptr, m_infoBuffer);
        stringstream ss;
        ss << "OpenGL shader compilation failure detected at " << locationName << ": " << m_infoBuffer;
        L::d(LOG_TAG, ss.str());
    }
}

void OpenGLErrorDetector::checkShaderLinkingError(GLuint shader, const string& locationName) {
    GLint linkStatus;
    glGetProgramiv(shader, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        m_isOpenGLErrorDetected = true;

        stringstream ss;
        ss << "OpenGL shader linking failure detected at " << locationName;
        L::d(LOG_TAG, ss.str());

        GLint maxLength = 0;
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
        L::d("Link Error", infoLog.data());
    }
}

void OpenGLErrorDetector::checkFramebufferStatus(const string& locationName) {
    auto framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
        m_isOpenGLErrorDetected = true;
        string statusDescription;
        if (s_framebufferStatusMap.count(framebufferStatus) > 0) {
            statusDescription = s_framebufferStatusMap.at(framebufferStatus);
        }
        else {
            stringstream ss;
            ss << "Unknown status " << framebufferStatus;
            statusDescription = ss.str();
        }
        stringstream ss;
        ss << "Incomplete framebuffer status at " << locationName << ": " << statusDescription;
        L::d(LOG_TAG, ss.str());
    }
}

const unordered_map<GLenum, const string> OpenGLErrorDetector::s_openGLErrorMap{
        { GL_INVALID_ENUM, "GL_INVALID_ENUM" },
        { GL_INVALID_VALUE, "GL_INVALID_VALUE" },
        { GL_INVALID_OPERATION, "GL_INVALID_OPERATION" },
        { GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW" },
        { GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW" },
        { GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY" },
        { GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION" }
};

const unordered_map<GLenum, const string> OpenGLErrorDetector::s_framebufferStatusMap{
        { GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" },
        { GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" },
        { GL_FRAMEBUFFER_UNSUPPORTED, "GL_FRAMEBUFFER_UNSUPPORTED" }
};

const string OpenGLErrorDetector::LOG_TAG = "OpenGLErrorDetector";