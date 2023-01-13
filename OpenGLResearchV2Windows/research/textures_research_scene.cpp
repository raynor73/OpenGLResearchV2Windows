#include "textures_research_scene.h"
#include <game_engine/utils.h>
#include <glm/ext/matrix_transform.hpp>
#include <rendering_engine/utils.h>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <rendering_engine/utils.h>
#include <exception>
#include <stdexcept>
#include <sstream>
#include <unordered_map>

using namespace GameEngine;
using namespace std;
using namespace GameEngine::Utils;
using namespace GameEngine::RenderingEngine::Utils;

static const char* g_vertexShaderSource = R"(
#version 400 core

layout(location = 0) in vec3 positionAttribute;
layout(location = 1) in vec2 uvAttribute;

uniform mat4 mvpMatrixUniform;

out vec2 interpolatedUV;

void main() {
    gl_Position = mvpMatrixUniform * vec4(positionAttribute, 1);
    interpolatedUV = uvAttribute;
}
)";

static const char* g_fragmentShaderSource = R"(
#version 400 core

uniform sampler2D sampler0;
uniform float bias;
uniform vec2 textureScale;

in vec2 interpolatedUV;

out vec4 outColor;

void main() {
    outColor = texture(sampler0, interpolatedUV * textureScale, bias);
}
)";

static const char* g_magFilterOptions[] = { 
    "GL_NEAREST",
    "GL_LINEAR"
};
static int g_magFilterSelectedOptionIndex = 0;

static const char* g_minFilterOptions[] = { 
    "GL_NEAREST",
    "GL_LINEAR",
    "GL_NEAREST_MIPMAP_NEAREST",
    "GL_NEAREST_MIPMAP_LINEAR",
    "GL_LINEAR_MIPMAP_NEAREST",
    "GL_LINEAR_MIPMAP_LINEAR"
};
static int g_minFilterSelectedOptionIndex = 0;

static const char* g_textureClampingModes[] = {
    "GL_CLAMP_TO_EDGE",
    "GL_CLAMP_TO_BORDER",
    "GL_REPEAT",
    "GL_MIRRORED_REPEAT"
};
static int g_selectedTextureClampingModeS = 2;
static int g_selectedTextureClampingModeT = 2;
static unordered_map<int, GLenum> g_clampingModeIndex2GLenumMap = { 
    {0, GL_CLAMP_TO_EDGE},
    {1, GL_CLAMP_TO_BORDER},
    {2, GL_REPEAT},
    {3, GL_MIRRORED_REPEAT}
};

void TexturesResearchScene::renderUi()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow();

    ImGui::Begin("Scene");
    
    ImGui::Text("FPS: %.02f", m_fpsCalculator.fps());
    
    ImGui::Text("Model");
    ImGui::SliderFloat("x-angle", &m_xAngle, 0, 360);
    ImGui::SliderFloat("y-angle", &m_yAngle, 0, 360);
    ImGui::SliderFloat("z-angle", &m_zAngle, 0, 360);
    ImGui::SliderFloat("x-scale", &m_modelScale.x, 1, 20);
    ImGui::SliderFloat("y-scale", &m_modelScale.y, 1, 20);
    ImGui::SliderFloat("z-scale", &m_modelScale.z, 1, 20);
    ImGui::SliderFloat("mimmap bias", &m_mipmapBias, -2, 2);

    ImGui::Text("Camera");
    ImGui::SliderFloat("x", &m_cameraPosition.x, -10, 10);
    ImGui::SliderFloat("y", &m_cameraPosition.y, -10, 10);
    ImGui::SliderFloat("z", &m_cameraPosition.z, 0.1, 100);
    
    ImGui::Text("Texture");

    ImGui::SliderFloat("u-scale", &m_textureScale.x, 0.5, 4);
    ImGui::SliderFloat("v-scale", &m_textureScale.y, 0.5, 4);

    if (ImGui::BeginCombo("GL_TEXTURE_MAG_FILTER", g_magFilterOptions[g_magFilterSelectedOptionIndex], 0)) {
        for (int i = 0; i < IM_ARRAYSIZE(g_magFilterOptions); i++) {
            if (ImGui::Selectable(g_magFilterOptions[i], g_magFilterSelectedOptionIndex == i)) {
                g_magFilterSelectedOptionIndex = i;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("GL_TEXTURE_MIN_FILTER", g_minFilterOptions[g_minFilterSelectedOptionIndex], 0)) {
        for (int i = 0; i < IM_ARRAYSIZE(g_minFilterOptions); i++) {
            if (ImGui::Selectable(g_minFilterOptions[i], g_minFilterSelectedOptionIndex == i)) {
                g_minFilterSelectedOptionIndex = i;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("GL_TEXTURE_WRAP_S", g_textureClampingModes[g_selectedTextureClampingModeS], 0)) {
        for (int i = 0; i < IM_ARRAYSIZE(g_textureClampingModes); i++) {
            if (ImGui::Selectable(g_textureClampingModes[i], g_selectedTextureClampingModeS == i)) {
                g_selectedTextureClampingModeS = i;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("GL_TEXTURE_WRAP_T", g_textureClampingModes[g_selectedTextureClampingModeT], 0)) {
        for (int i = 0; i < IM_ARRAYSIZE(g_textureClampingModes); i++) {
            if (ImGui::Selectable(g_textureClampingModes[i], g_selectedTextureClampingModeT == i)) {
                g_selectedTextureClampingModeT = i;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::ColorPicker3("Border color", &m_textureBorderColor.r);

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
    m_xAngle = 0;
    m_yAngle = 0;
    m_zAngle = 0;
    m_modelScale = { 1, 1, 1 };
    m_textureScale = { 1, 1 };
    m_textureBorderColor = { 0, 0, 0, 1 };
    m_mipmapBias = 0;

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glActiveTexture(GL_TEXTURE0);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    auto redSquaresBitmap = m_bitmapDataSource->loadBitmap("bitmaps/red_test_128x128.png");
    auto greenSquaresBitmap = m_bitmapDataSource->loadBitmap("bitmaps/green_test_256x256.png");
    auto blueSquaresBitmap = m_bitmapDataSource->loadBitmap("bitmaps/blue_test_512x512.png");
    glTexStorage2D(GL_TEXTURE_2D, 3, GL_RGBA8, blueSquaresBitmap.width, blueSquaresBitmap.height);
    
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        blueSquaresBitmap.width,
        blueSquaresBitmap.height,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        blueSquaresBitmap.data.data()
    );
    glTexSubImage2D(
        GL_TEXTURE_2D,
        1,
        0,
        0,
        greenSquaresBitmap.width,
        greenSquaresBitmap.height,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        greenSquaresBitmap.data.data()
    );
    glTexSubImage2D(
        GL_TEXTURE_2D,
        2,
        0,
        0,
        redSquaresBitmap.width,
        redSquaresBitmap.height,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        redSquaresBitmap.data.data()
    );

    m_openGLErrorDetector->checkOpenGLErrors("TexturesResearchScene::start");

    glm::vec4 viewport = glGetIntegerv4(GL_VIEWPORT);
    m_projection = glm::perspective<float>(70, viewport[2] / viewport[3], 0.1f, 1000);
}

void TexturesResearchScene::update(float dt)
{
    m_fpsCalculator.update(dt);

    auto shaderProgramContainer = m_shadersRepository->getShaderProgramContainer("shaderProgram");
    glUseProgram(shaderProgramContainer.shaderProgram());

    auto biasUniform = glGetUniformLocation(shaderProgramContainer.shaderProgram(), "bias");
    auto textureScaleUniform = glGetUniformLocation(shaderProgramContainer.shaderProgram(), "textureScale");

    auto modelMatrix = glm::identity<glm::mat4>();

    auto rotation = glm::rotate(glm::identity<glm::quat>(), glm::radians(m_zAngle), glm::vec3(0, 0, 1));
    rotation = glm::rotate(rotation, glm::radians(m_xAngle), glm::vec3(1, 0, 0));
    rotation = glm::rotate(rotation, glm::radians(m_yAngle), glm::vec3(0, 1, 0));

    modelMatrix *= glm::toMat4(rotation);
    modelMatrix = glm::scale(modelMatrix, m_modelScale);

    auto viewMatrix = glm::lookAt(m_cameraPosition, m_cameraPosition + m_forward, m_up);

    auto mvpMatrix = m_projection * viewMatrix * modelMatrix;
    auto mvpMatrixUniform = shaderProgramContainer.mvpMatrixUniform();
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, &mvpMatrix[0][0]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);

    GLenum magFilter;
    switch (g_magFilterSelectedOptionIndex) {
    case 0:
        magFilter = GL_NEAREST;
        break;

    case 1:
        magFilter = GL_LINEAR;
        break;

    default: {
        stringstream ss;
        ss << "Unexpected g_magFilterSelectedOptionIndex: " << g_magFilterSelectedOptionIndex;
        throw runtime_error(ss.str());
    }
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    GLenum minFilter;
    switch (g_minFilterSelectedOptionIndex)
    {
    case 0:
        minFilter = GL_NEAREST;
        break;

    case 1:
        minFilter = GL_LINEAR;
        break;

    case 2:
        minFilter = GL_NEAREST_MIPMAP_NEAREST;
        break;

    case 3:
        minFilter = GL_NEAREST_MIPMAP_LINEAR;
        break;

    case 4:
        minFilter = GL_LINEAR_MIPMAP_NEAREST;
        break;

    case 5:
        minFilter = GL_LINEAR_MIPMAP_LINEAR;
        break;

    default: {
        stringstream ss;
        ss << "Unexpected g_minFilterSelectedOptionIndex: " << g_minFilterSelectedOptionIndex;
        throw runtime_error(ss.str());
    }
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, g_clampingModeIndex2GLenumMap[g_selectedTextureClampingModeS]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, g_clampingModeIndex2GLenumMap[g_selectedTextureClampingModeT]);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &m_textureBorderColor.r);

    glUniform1f(biasUniform, m_mipmapBias);
    glUniform2f(textureScaleUniform, m_textureScale.x, m_textureScale.y);

    glDrawElements(GL_TRIANGLES, m_boxMesh.indices().size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

    renderUi();

    m_openGLErrorDetector->checkOpenGLErrors("TexturesResearchScene::update");
}
