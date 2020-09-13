#include "mesh.h"

#include <fstream>
#include <sstream>
#include <limits>
#include <math.h>
#include <vector>

Mesh::Mesh() : m_NumVertices(0), m_NumFaces(0) {};
Mesh::~Mesh() {}

int Mesh::getNumFaces() { return m_NumFaces; }
int Mesh::getNumVertices() { return m_NumVertices; }

void Mesh::computeFaceList() {
    // computes the list of all faces which a vertex is forming and stores it 
    // as a map from the vertex index to the vector of faces it is forming.
    // required for calculating smooth per vertex tangent space basis.

    Vector3i indices;
    for (int i = 0; i < m_NumFaces; i++) {
        indices = m_VertexIndices[i];
        for (int j = 0; j < 3; j++) {
            if (m_FaceList.find(indices[j]) != m_FaceList.end()) {
                // index is seen for the first time
                m_FaceList.insert({indices[j], std::vector<int>{i}});
            }
            else m_FaceList[indices[j]].push_back(i);
        }
    }
}

void Mesh::computeTangentVertex() {
    computeFaceList();

    // computes TBN for every vertex in the mesh
    Vector3i face_idxs;
    Vector3f T(0, 0, 0), B(0, 0, 0), N(0, 0, 0);

    int faceCount;      // number of faces a vertex in included in

    for (int i = 0; i < m_NumVertices; i++) {
        faceCount = 0;

        // for every vertex (index i) calculate the corresponding tanget and bitangent
        for (auto ele : m_FaceList[i]) {
             T = T + m_FaceTangents[ele];
            B = B + m_FaceBitangents[ele];
            
            ++faceCount;
        }

        if (faceCount > 0) {
            T = T / (float)faceCount;
            B = B / (float)faceCount;
            N = N / (float)faceCount;
            normalize(T);
            normalize(B);
            normalize(N);

            // tangent and bitangent for ith vertex.
            m_Tangents.push_back(T);
            m_Bitangents.push_back(B);
        }
        T.x = 0; T.y = 0; T.z = 0;
        B.x = 0; B.y = 0; B.z = 0;
    }
}

void Mesh::computeTangentFace() {
    // computes the tangent basis represented by T, B and N for normal mapping of textures.
    Vector3f uv[3];        // texture coordinates
    Vector3f v[3];         // vertex coordiantes
    for (int i = 0; i < m_NumFaces; i++) {
        for (int j = 0; j < 3; j++) {
            v[j] = m_Vertices[m_VertexIndices[i][j]];
            uv[j] = m_Textures[m_TextureIndices[i][j]];
        }

        Vector3f edge1 = v[1] - v[0];
        Vector3f edge2 = v[2] - v[0];

        Vector3f deltaUV1 = uv[1] - uv[0];
        Vector3f deltaUV2 = uv[2] - uv[0];

        // T = (E1 * deltaV2 - E2 * deltaV1) / deltaU1*deltaV2 - deltaU2*deltaV1;
        // B = (-E1 * deltaU2 - E2 * deltaU1)/ deltaU1*deltaV2 - deltaU2*deltaV1;

        float denom = 1 / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
        Vector3f tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * denom;
        Vector3f bitangent = (edge2 * deltaUV1.x - edge1 * deltaUV2.x) * denom;
            
        m_FaceTangents.push_back(tangent);
        m_FaceBitangents.push_back(bitangent);
    }
}

void Mesh::computeFaceNormals() {
    // pre-calculates surface normal for every face in the mesh for techniques likes backface culling
    Vector3f tmp[3];
    for (int i = 0; i < m_NumFaces; i++) {
        for (int j = 0; j < 3; j++) {
            tmp[j] = m_Vertices[m_VertexIndices[i][j]];
        }
        m_faceNormal.push_back(normalize(cross(tmp[1] - tmp[0], tmp[2] - tmp[0])));
    }
}

void Mesh::normalizeMesh() {
    // bring down to -1 and +1 dimensions
    Vector3f maxPosition = m_Vertices[0];
    Vector3f minPosition = m_Vertices[0];

    for (size_t i = 0; i < m_Vertices.size(); i++) {
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
    for (size_t i = 0; i < m_Vertices.size(); i++) {
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
            // char slash;
            // int x, y, z;
            // iss >> x >> y >> z;
            // x--; y--; z--;
            // m_VertexIndices.push_back(Vector3i(x, y, z));
            char slash;
            int vi[3], vti[3], vni[3];
            iss >> vi[0] >> slash >> vti[0] >> slash >> vni[0];
            iss >> vi[1] >> slash >> vti[1] >> slash >> vni[1];
            iss >> vi[2] >> slash >> vti[2] >> slash >> vni[2];
            for (int i = 0; i < 3; i++) {
                vi[i]--;
                vti[i]--;
                vni[i]--;
            }
            m_VertexIndices.push_back(Vector3i(vi[0], vi[1], vi[2]));
            m_TextureIndices.push_back(Vector3i(vti[0], vti[1], vti[2]));
            m_NormalIndices.push_back(Vector3i(vni[0], vni[1], vni[2]));
            m_NumFaces++;
        }
    }

    // for backface culling and flat shading
    computeFaceNormals();

    // compute per vertex tangent basis vectors
    computeTangentFace();
    computeTangentVertex();

    return true;
}

Vector3f& Mesh::getFaceNormal(int face_idx) {
    return m_faceNormal[face_idx];
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

Vector3f& Mesh::getTangent(int index) {
    return m_Tangents[index];
}

Vector3f& Mesh::getBitangent(int index) {
    return m_Bitangents[index];
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
