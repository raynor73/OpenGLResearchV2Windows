#include "mesh_storage.h"
#include <utility>
#include <sstream>
#include <exception>

using namespace GameEngine;
using namespace std;

void MeshStorage::putMesh(const string& name, Mesh mesh) {
    if (m_storage.count(name) > 0) {
        stringstream ss;
        ss << "Mesh " << name << " already stored";
        throw domain_error(ss.str());
    }

    m_storage[name] = move(mesh);
}

Mesh MeshStorage::getMesh(const string& name) const {
    if (m_storage.count(name) == 0) {
        stringstream ss;
        ss << "Mesh " << name << " not found";
        throw domain_error(ss.str());
    }

    return m_storage.at(name);
}

void MeshStorage::removeMesh(const string& name) {
    if (m_storage.count(name) == 0) {
        stringstream ss;
        ss << "No mesh " << name << " to remove";
        throw domain_error(ss.str());
    }

    m_storage.erase(name);
}

void MeshStorage::removeAllMeshes() {
    m_storage.clear();
}