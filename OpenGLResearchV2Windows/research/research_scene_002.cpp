#include "research_scene_002.h"
#include <rendering_engine/utils.h>
#include <game_engine/logger.h>
#include <sstream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <game_engine/utils.h>

using namespace GameEngine;
using namespace std;
using namespace Utils;

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



    GLuint framebufferId;
    glGenFramebuffers(1, &framebufferId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferId);

    GLuint colorRenderbufferId;
    glGenRenderbuffers(1, &colorRenderbufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbufferId);
    auto viewportData = glGetIntegerv4(GL_VIEWPORT);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, viewportData[2], viewportData[3]);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbufferId);

    m_openGLErrorDetector->checkOpenGLErrors("ResearchScene002::start2");
    m_openGLErrorDetector->checkFramebufferStatus(GL_DRAW_FRAMEBUFFER, "ResearchScene002::start2");
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

    //glBlitFramebuffer()

    glFlush();


    if (!m_isSaved) {
        m_isSaved = true;

        BitmapInfo bitmapInfo;
        GLint viewportSize[4];
        glGetIntegerv(GL_VIEWPORT, viewportSize);
        bitmapInfo.width = viewportSize[2];
        bitmapInfo.height = viewportSize[3];
        bitmapInfo.data.resize(bitmapInfo.width * bitmapInfo.height * 4);
        auto drawFramebufferName = glGetInteger(GL_DRAW_FRAMEBUFFER_BINDING);

        stringstream ss;
        ss << "drawFramebufferName: " << drawFramebufferName;
        L::d("!@#", ss.str());
        
        glBindBuffer(GL_PIXEL_PACK_BUFFER, drawFramebufferName);
        m_openGLErrorDetector->checkOpenGLErrors("ResearchScene002::update2");
        glReadPixels(0, 0, bitmapInfo.width, bitmapInfo.height, GL_RGBA, GL_UNSIGNED_BYTE, bitmapInfo.data.data());
        m_openGLErrorDetector->checkOpenGLErrors("ResearchScene002::update3");
        m_bitmapDataSource->saveBitmap(bitmapInfo, "test.png");

        /*BitmapInfo bitmapInfo;
        bitmapInfo.width = 320;
        bitmapInfo.height = 200;
        for (int y = 0; y < bitmapInfo.height; y++) {
            for (int x = 0; x < bitmapInfo.width; x++) {
                bitmapInfo.data.push_back(0);
                bitmapInfo.data.push_back(0);
                bitmapInfo.data.push_back(128);
                bitmapInfo.data.push_back(255);
            }
        }

        m_bitmapDataSource->saveBitmap(bitmapInfo, "test.png");*/
    }

    m_openGLErrorDetector->checkOpenGLErrors("ResearchScene002::update");
}
