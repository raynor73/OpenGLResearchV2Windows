#include "research_scene_001.h"
#include <rendering_engine/utils.h>

void ResearchScene001::start()
{
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
    GLfloat vertices[NumVertices][2] = {
    { -0.90, -0.90 }, // Triangle 1
    { 0.85, -0.90 },
    { -0.90, 0.85 },
    { 0.90, -0.85 }, // Triangle 2
    { 0.90, 0.90 },
    { -0.85, 0.90 }
    };
    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    m_shadersRepository->createVertexShader("vertexShader", g_vertexShaderSource);
    m_shadersRepository->createFragmentShader("fragmentShader", g_fragmentShaderSource);
    m_shadersRepository->createShaderProgram("shaderProgram", "vertexShader", "fragmentShader");

    GLuint program = m_shadersRepository->getShaderProgramContainer("shaderProgram").shaderProgram();
    glUseProgram(program);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT,
        GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);

    m_openGLErrorDetector->checkOpenGLErrors("ResearchScene001::start()");
}

void ResearchScene001::update()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();

    m_openGLErrorDetector->checkOpenGLErrors("ResearchScene001::update()");
}
