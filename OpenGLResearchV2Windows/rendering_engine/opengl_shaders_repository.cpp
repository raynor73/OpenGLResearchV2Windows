#include "opengl_shaders_repository.h"
#include <sstream>

using namespace GameEngine;
using namespace std;

GLuint OpenGLShadersRepository::getVertexShader(const string& name) const {
    if (m_vertexShaders.count(name) == 0) {
        stringstream ss;
        ss << "Vertex shader " << name << " does not exist";
        throw domain_error(ss.str());
    }

    return m_vertexShaders.at(name);
}

GLuint OpenGLShadersRepository::getFragmentShader(const string& name) const {
    if (m_fragmentShaders.count(name) == 0) {
        stringstream ss;
        ss << "Fragment shader " << name << " does not exist";
        throw domain_error(ss.str());
    }

    return m_fragmentShaders.at(name);
}

OpenGLShaderProgramContainer OpenGLShadersRepository::getShaderProgramContainer(const string& name) const {
    if (m_shaderProgramContainers.count(name) == 0) {
        stringstream ss;
        ss << "Shader program " << name << " does not exist";
        throw domain_error(ss.str());
    }

    return m_shaderProgramContainers.at(name);
}

void OpenGLShadersRepository::removeAllShadersAndPrograms() {
    for (auto& entry : m_shaderProgramContainers) {
        glDeleteProgram(entry.second.shaderProgram());
    }
    for (auto& entry : m_vertexShaders) {
        glDeleteShader(entry.second);
    }
    for (auto& entry : m_fragmentShaders) {
        glDeleteShader(entry.second);
    }

    m_vertexShaders.clear();
    m_fragmentShaders.clear();
}

GLuint OpenGLShadersRepository::createVertexShader(
    const string& name,
    const string& source
) {
    if (m_vertexShaders.count(name) > 0) {
        stringstream ss;
        ss << "Vertex shader " << name << " already exists";
        throw domain_error(ss.str());
    }

    auto shader = glCreateShader(GL_VERTEX_SHADER);
    if (shader == 0) {
        stringstream ss;
        ss << "Failed to allocate vertex shader " << name;
        throw domain_error(ss.str());
    }
    const GLchar* strings[1];
    GLint stringSizes[1];
    strings[0] = source.c_str();
    stringSizes[0] = source.size();
    glShaderSource(shader, 1, strings, stringSizes);
    glCompileShader(shader);

    m_vertexShaders[name] = shader;

    m_openGLErrorDetector->checkShaderCompilationError(shader, "createVertexShader");

    return shader;
}

GLuint OpenGLShadersRepository::createFragmentShader(
    const string& name,
    const string& source
) {
    if (m_fragmentShaders.count(name) > 0) {
        stringstream ss;
        ss << "Fragment shader " << name << " already exists";
        throw domain_error(ss.str());
    }

    auto shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (shader == 0) {
        stringstream ss;
        ss << "Failed to allocate fragment shader " << name;
        throw domain_error(ss.str());
    }
    const GLchar* strings[1];
    GLint stringSizes[1];
    strings[0] = source.c_str();
    stringSizes[0] = source.size();
    glShaderSource(shader, 1, strings, stringSizes);
    glCompileShader(shader);

    m_fragmentShaders[name] = shader;

    m_openGLErrorDetector->checkShaderCompilationError(shader, "createFragmentShader");

    return shader;
}

OpenGLShaderProgramContainer OpenGLShadersRepository::createShaderProgram(
    const string& name,
    const string& vertexShaderName,
    const string& fragmentShaderName
) {
    if (m_shaderProgramContainers.count(name) > 0) {
        stringstream ss;
        ss << "Shader program " << name << " already exists";
        throw domain_error(ss.str());
    }

    auto shaderProgram = glCreateProgram();
    if (shaderProgram == 0) {
        stringstream ss;
        ss << "Failed to allocate shader program " << name;
        throw domain_error(ss.str());
    }

    auto vertexShader = getVertexShader(vertexShaderName);
    auto fragmentShader = getFragmentShader(fragmentShaderName);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    m_openGLErrorDetector->checkShaderLinkingError(shaderProgram, "createShaderProgram");

    auto shaderProgramContainer = OpenGLShaderProgramContainer(m_openGLErrorDetector, shaderProgram);
    m_shaderProgramContainers.insert({ name, shaderProgramContainer });

    m_openGLErrorDetector->checkOpenGLErrors("createShaderProgram");

    return shaderProgramContainer;
}
