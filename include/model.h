#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "matrix.h"
#include "texture.h"

/**
 * This class contains every model which is to be rendered on the scene
 * it will contain the mesh, along with other properties like 
 * model matrix, textures and material files.
 * */


class Model {
private:
    Mesh* m_Mesh;
    Matrix4f m_ModelMatrix;
    std::string m_filepath;
    Texture *m_Normal;
    Texture *m_Diffuse;
    
public:
    Model(std::string path);
    Model();
    ~Model();
    void transformModel(Vector3f translate, Vector3f rotate, Vector3f scale);
    void loadDiffuse(std::string filename);
    void loadNormal(std::string filename);
    const Matrix4f& getModelMatrix() const;
    Texture* getDiffuse();
    Texture* getNormal();
    Mesh* getMesh();
};

#endif      // MODEL_H
