#include "mesh.h"

#include <fstream>
#include <sstream>
#include <limits>
#include <math.h>

Mesh::Mesh() : m_NumVertices(0), m_NumFaces(0) {};
Mesh::~Mesh() {}

int Mesh::getNumFaces() { return m_NumFaces; }
int Mesh::getNumVertices() { return m_NumVertices; }

void Mesh::normalizeMesh() {
    // bring down to -1 and +1 dimensions
    Vector3f maxPosition = m_Vertices[0];
    Vector3f minPosition = m_Vertices[0];

    for (int i = 0; i < m_Vertices.size(); i++) {
        maxPosition.x = std::max(maxPosition.x, m_Vertices[i].x);
        minPosition.x = std::min(minPosition.x, m_Vertices[i].x);
        maxPosition.y = std::max(maxPosition.y, m_Vertices[i].y);
        minPosition.y = std::min(minPosition.y, m_Vertices[i].y);
        maxPosition.z = std::max(maxPosition.z, m_Vertices[i].z);
        minPosition.z = std::min(minPosition.z, m_Vertices[i].z);
    }
    float maxCoord = std::max(std::max(maxPosition.x, maxPosition.y), maxPosition.z);
    float minCoord = std::min(std::min(minPosition.x, minPosition.y), minPosition.z);
    minCoord = std::abs(minCoord);
    float scale = std::max(minCoord, maxCoord);
    for (int i = 0; i < m_Vertices.size(); i++) {
        m_Vertices[i] = m_Vertices[i]/scale;
    }
}

bool Mesh::loadFile(std::string path) {
    std::cout << "loading " << path << std::endl;
    std::ifstream infile(path);
    if (!infile.is_open()) return false;

    std::string curline;
    while(std::getline(infile, curline)) {
        std::istringstream iss(curline);
        std::string data;
        iss >> data;        // check if vertex data, texture data and so on

        if (data == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            m_Vertices.push_back(Vector3f(x, y, z));
            m_NumVertices++;
        } else if (data == "vt") {
            float u, v;
            iss >> u >> v;
            m_Textures.push_back(Vector3f(u, v, 0));
        } else if (data == "vn") {
            float i, j, k;
            iss >> i >> j >> k;
            m_Normals.push_back(Vector3f(i, j, k));
        } else if (data == "f") {
            char slash;
            int x, y, z;
            iss >> x >> y >> z;
            x--; y--; z--;
            m_VertexIndices.push_back(Vector3i(x, y, z));
            // char slash;
            // int vi[3], vti[3], vni[3];
            // iss >> vi[0] >> slash >> vti[0] >> slash >> vni[0];
            // iss >> vi[1] >> slash >> vti[1] >> slash >> vni[1];
            // iss >> vi[2] >> slash >> vti[2] >> slash >> vni[2];
            // for (int i = 0; i < 3; i++) {
            //     vi[i]--;
            //     vti[i]--;
            //     vni[i]--;
            // }
            // m_VertexIndices.push_back(Vector3i(vi[0], vi[1], vi[2]));
            // m_TextureIndices.push_back(Vector3i(vti[0], vti[1], vti[2]));
            // m_NormalIndices.push_back(Vector3i(vni[0], vni[1], vni[2]));
            m_NumFaces++;
        }
    }
    return true;
}

Vector3f& Mesh::getVertex(int index) {
    return m_Vertices[index];
}

Vector3f& Mesh::getTexture(int index) {
    return m_Textures[index];
}

Vector3f& Mesh::getNormal(int index) {
    return m_Normals[index];
}

std::vector<Vector3i>& Mesh::getVertexIndices() {
    return m_VertexIndices;
}

std::vector<Vector3i>& Mesh::getTextureIndices() {
    return m_TextureIndices;
}

std::vector<Vector3i>& Mesh::getNormalIndices() {
    return m_NormalIndices;
}
