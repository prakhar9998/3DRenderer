#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <map>
#include "vector3d.h"
#include "matrix.h"

class Mesh {
private:
    int m_NumVertices;
    int m_NumFaces;
    std::string m_FileName;
    std::vector<Vector3f> m_Vertices;
    std::vector<Vector3f> m_Textures;
    std::vector<Vector3f> m_Normals;
    std::vector<Vector3f> m_faceNormal;

    // indices which form a face
    std::vector<Vector3i> m_VertexIndices;
    std::vector<Vector3i> m_TextureIndices;
    std::vector<Vector3i> m_NormalIndices;

    // tangent basis vectors
    std::vector<Vector3f> m_Tangents;
    std::vector<Vector3f> m_Bitangents;

    std::vector<Vector3f> m_FaceTangents;
    std::vector<Vector3f> m_FaceBitangents;

    std::map<int, std::vector<int> > m_FaceList;

private:
    void computeFaceNormals();
    void computeTangentFace();
    void computeTangentVertex();
    // void computeTangentVertex(int vertexIndex, std::vector<int> faceList, Vector3f tangent, Vector3f bitangent, Vector3f normal);
    void computeFaceList();
    Matrix3f getTBNFace(int face_idx);

public:
    Mesh();
    ~Mesh();

    int getNumVertices();
    int getNumFaces();
    bool loadFile(std::string path);
    void normalizeMesh();
    Vector3f& getVertex(int index);
    Vector3f& getTexture(int index);
    Vector3f& getNormal(int index);
    Vector3f& getTangent(int face_idx);
    Vector3f& getBitangent(int index);
    Vector3f& getFaceNormal(int index);
    std::vector<Vector3i>& getVertexIndices();
    std::vector<Vector3i>& getTextureIndices();
    std::vector<Vector3i>& getNormalIndices();
};

#endif      // MESH_H
