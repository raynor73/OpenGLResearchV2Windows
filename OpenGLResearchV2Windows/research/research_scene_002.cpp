#include "research_scene_002.h"
#include <rendering_engine/utils.h>
#include <game_engine/logger.h>
#include <sstream>

using namespace GameEngine;
using namespace std;

static const char* g_vertexShaderSource = R"(
#version 400 core

layout(location = 0) in vec3 positionAttribute;

void main() {
    gl_Position = vec4(positionAttribute, 1);
}
)";

static const char* g_fragmentShaderSource = R"(
#version 400 core

out vec4 outColor;

void main() {
    outColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)";

void ResearchScene002::start()
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLfloat vertices[] = {
        0.5, -0.5, 0.0,
        0.0, 0.5, 0.0,
        -0.5, -0.5, 0.0
    };
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    m_shadersRepository->createVertexShader("vertexShader", g_vertexShaderSource);
    m_shadersRepository->createFragmentShader("fragmentShader", g_fragmentShaderSource);
    m_shadersRepository->createShaderProgram("shaderProgram", "vertexShader", "fragmentShader");

    auto shaderProgram = m_shadersRepository->getShaderProgramContainer("shaderProgram").shaderProgram();
    glUseProgram(shaderProgram);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    m_openGLErrorDetector->checkOpenGLErrors("ResearchScene002::start");
}

void ResearchScene002::update()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glFlush();

    m_openGLErrorDetector->checkOpenGLErrors("ResearchScene002::update");
}
