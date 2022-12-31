#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_shader_program_container.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
class OpenGLShadersRepository : public WithoutGeneratedMethods
{
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;

    std::unordered_map<std::string, GLuint> m_vertexShaders;
    std::unordered_map<std::string, GLuint> m_fragmentShaders;
    std::unordered_map<std::string, OpenGLShaderProgramContainer> m_shaderProgramContainers;

public:
    explicit OpenGLShadersRepository(std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector) :
        m_openGLErrorDetector(openGLErrorDetector) {}

    GLuint getVertexShader(const std::string& name) const;
    GLuint getFragmentShader(const std::string& name) const;
    OpenGLShaderProgramContainer getShaderProgramContainer(const std::string& name) const;

    GLuint createVertexShader(const std::string& name, const std::string& source);
    GLuint createFragmentShader(const std::string& name, const std::string& source);
    OpenGLShaderProgramContainer createShaderProgram(
        const std::string& name,
        const std::string& vertexShaderName,
        const std::string& fragmentShaderName
    );

    void removeAllShadersAndPrograms();
};
}
