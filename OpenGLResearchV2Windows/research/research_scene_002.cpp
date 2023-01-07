#include "research_scene_002.h"
#include <rendering_engine/utils.h>
#include <game_engine/logger.h>
#include <sstream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext.hpp>

using namespace GameEngine;
using namespace std;

static const char* g_vertexShaderSource = R"(
#version 400 core

layout(location = 0) in vec3 positionAttribute;
layout(location = 1) in vec3 colorAttribute;

uniform mat4 mvpMatrixUniform;

out vec3 interpolatedVertexColor;

void main() {
    gl_Position = mvpMatrixUniform * vec4(positionAttribute, 1);
    interpolatedVertexColor = colorAttribute;
}
)";

static const char* g_fragmentShaderSource = R"(
#version 400 core

in vec3 interpolatedVertexColor;
out vec4 outColor;

void main() {
    outColor = vec4(interpolatedVertexColor, 1);
}
)";

void ResearchScene002::start()
{
    m_cameraPosition = { 0, 0, 1 };
    m_cameraLookAt = { 0, 0, 0 };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /*GLfloat vertices[] = {
        0.5, -0.5, 0.0,
        1.0, 0.0, 0.0,

        0.0, 0.5, 0.0,
        0.0, 1.0, 0.0,

        -0.5, -0.5, 0.0,
        0.0, 0.0, 1.0
    };*/
    GLfloat vertices[] = {
        20, 10, 0.0,
        1.0, 0.0, 0.0,

        15, 20, 0.0,
        0.0, 1.0, 0.0,

        10, 10, 0.0,
        0.0, 0.0, 1.0
    };
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLushort indices[] = {
        0, 1, 2
    };
    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    m_shadersRepository->createVertexShader("vertexShader", g_vertexShaderSource);
    m_shadersRepository->createFragmentShader("fragmentShader", g_fragmentShaderSource);
    m_shadersRepository->createShaderProgram("shaderProgram", "vertexShader", "fragmentShader");

    auto shaderProgram = m_shadersRepository->getShaderProgramContainer("shaderProgram").shaderProgram();
    glUseProgram(shaderProgram);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);

    m_openGLErrorDetector->checkOpenGLErrors("ResearchScene002::start");

    GLfloat viewport[4];
    glGetFloatv(GL_VIEWPORT, viewport);
    m_projection = glm::ortho<float>(0, viewport[2], 0, viewport[3], 0.1f, 100.0f);
}

void ResearchScene002::update()
{
    auto modelMatrix = glm::identity<glm::mat4>();
    auto viewMatrix = glm::lookAt(m_cameraPosition, m_cameraLookAt, m_up);

    auto mvpMatrix = m_projection * viewMatrix * modelMatrix;
    auto mvpMatrixUniform = m_shadersRepository->getShaderProgramContainer("shaderProgram").mvpMatrixUniform();
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, &mvpMatrix[0][0]);

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

    glFlush();

    m_openGLErrorDetector->checkOpenGLErrors("ResearchScene002::update");
}
