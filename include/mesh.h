#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "vector3d.h"

class Mesh {
private:
    int m_NumVertices;
    int m_NumFaces;
    std::string m_FileName;
    std::vector<Vector3f> m_Vertices;
    std::vector<Vector3f> m_Textures;
    std::vector<Vector3f> m_Normals;

    // indices which form a face
    std::vector<Vector3i> m_VertexIndices;
    std::vector<Vector3i> m_TextureIndices;
    std::vector<Vector3i> m_NormalIndices;

public:
    Mesh();
    ~Mesh();

    int getNumVertices();
    int getNumFaces();
    bool loadFile(std::string path);
    void normalizeMesh();
    Vector3f getVertex(int index);
    Vector3f getTexture(int index);
    Vector3f getNormal(int index);
    std::vector<Vector3i> getVertexIndices();
    std::vector<Vector3i> getTextureIndices();
    std::vector<Vector3i> getNormalIndices();
};

#endif      // MESH_H
