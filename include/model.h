#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "matrix.h"

/**
 * This class contains every model which is to be rendered on the scene
 * it will contain the mesh, along with other properties like 
 * model matrix and textures. For now, it keeps track of a mesh
 * and computes model matrix.
 * */

class Model {
private:
    Mesh* m_Mesh;
    Matrix4f m_ModelMatrix;
    std::string m_filepath;
public:
    Model(std::string path);
    ~Model();
    void transformModel(Vector3f& translate, Vector3f& scale, Vector3f& rotate);
    Matrix4f getModelMatrix();
};

#endif      // MODEL_H
