#include "utils.h"

using namespace std;
using namespace GameEngine;

vector<float> GameEngine::RenderingEngine::Utils::prepareVertexData(const Mesh& mesh)
{
    vector<float> vertexData(mesh.vertices().size() * Vertex::VERTEX_COMPONENTS);

    for (uintptr_t i = 0; i < mesh.vertices().size(); i++) {
        vertexData[i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].position().x;
        vertexData[1 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].position().y;
        vertexData[2 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].position().z;

        vertexData[3 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].normal().x;
        vertexData[4 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].normal().y;
        vertexData[5 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].normal().z;

        vertexData[6 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].uv().x;
        vertexData[7 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].uv().y;

        vertexData[8 + i * Vertex::VERTEX_COMPONENTS] = float(mesh.vertices()[i].jointIndices().x);
        vertexData[9 + i * Vertex::VERTEX_COMPONENTS] = float(mesh.vertices()[i].jointIndices().y);
        vertexData[10 + i * Vertex::VERTEX_COMPONENTS] = float(mesh.vertices()[i].jointIndices().z);

        vertexData[11 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].jointWeights().x;
        vertexData[12 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].jointWeights().y;
        vertexData[13 + i * Vertex::VERTEX_COMPONENTS] = mesh.vertices()[i].jointWeights().z;
    }

    return vertexData;
}