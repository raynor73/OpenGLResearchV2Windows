#include "textures_research_scene.h"
#include <game_engine/utils.h>
#include <glm/ext/matrix_transform.hpp>
#include <rendering_engine/utils.h>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <rendering_engine/utils.h>

using namespace GameEngine;
using namespace std;
using namespace GameEngine::Utils;
using namespace GameEngine::RenderingEngine::Utils;

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

void TexturesResearchScene::renderUi()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Scene");
    ImGui::Text("FPS: %.02f", m_fpsCalculator.fps());
    ImGui::SliderFloat("Model z-angle", &m_zAngle, 0, 360);
    ImGui::Text("Camera");
    ImGui::SliderFloat("x", &m_cameraPosition.x, -10, 10);
    ImGui::SliderFloat("y", &m_cameraPosition.y, -10, 10);
    ImGui::SliderFloat("z", &m_cameraPosition.z, -10, 10);
    ImGui::End();

    ImGui::Render();
    int display_w, display_h;
    auto viewport = glGetIntegerv4(GL_VIEWPORT);
    display_w = viewport[2];
    display_h = viewport[3];
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TexturesResearchScene::start()
{
    m_cameraPosition = { 0, 0, 5 };
    m_zAngle = 0;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    m_boxMesh = m_meshLoader->loadMesh("meshes/box.obj");

    vector<float> vertexData = prepareVertexData(m_boxMesh);
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_boxMesh.indices().size() * sizeof(uint16_t), m_boxMesh.indices().data(), GL_STATIC_DRAW);

    m_shadersRepository->createVertexShader("vertexShader", g_vertexShaderSource);
    m_shadersRepository->createFragmentShader("fragmentShader", g_fragmentShaderSource);
    m_shadersRepository->createShaderProgram("shaderProgram", "vertexShader", "fragmentShader");

    auto shaderProgram = m_shadersRepository->getShaderProgramContainer("shaderProgram").shaderProgram();
    glUseProgram(shaderProgram);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(GLfloat) * Vertex::VERTEX_COMPONENTS, 
        BUFFER_OFFSET(0)
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        sizeof(GLfloat) * Vertex::VERTEX_COMPONENTS, 
        BUFFER_OFFSET(sizeof(GLfloat) * (Vertex::VERTEX_POSITION_COMPONENTS + Vertex::VERTEX_NORMAL_COMPONENTS))
    );
    glEnableVertexAttribArray(1);

    m_openGLErrorDetector->checkOpenGLErrors("TexturesResearchScene::start");

    glm::vec4 viewport = glGetIntegerv4(GL_VIEWPORT);
    m_projection = glm::perspective<float>(70, viewport[2] / viewport[3], 0.1f, 1000);
}

void TexturesResearchScene::update(float dt)
{
    m_fpsCalculator.update(dt);

    auto shaderProgramContainer = m_shadersRepository->getShaderProgramContainer("shaderProgram");
    glUseProgram(shaderProgramContainer.shaderProgram());

    auto modelMatrix = glm::identity<glm::mat4>();
    auto rotation = glm::rotate(
        glm::identity<glm::quat>(),
        glm::radians(m_zAngle),
        glm::vec3(0, 0, 1)
    );
    modelMatrix *= glm::toMat4(rotation);
    auto viewMatrix = glm::lookAt(m_cameraPosition, m_cameraPosition + m_forward, m_up);

    auto mvpMatrix = m_projection * viewMatrix * modelMatrix;
    auto mvpMatrixUniform = shaderProgramContainer.mvpMatrixUniform();
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, &mvpMatrix[0][0]);

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, m_boxMesh.indices().size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

    renderUi();

    m_openGLErrorDetector->checkOpenGLErrors("TexturesResearchScene::update");
}
