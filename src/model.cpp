#include "model.h"
#include "transform.h"

Model::Model(std::string path) : m_filepath(path), m_Mesh(new Mesh) {
    m_ModelMatrix = identity(m_ModelMatrix);
    m_Mesh->loadFile(path);
}

Model::~Model() {
    delete m_Mesh;
}

void Model::transformModel(Vector3f& translate, Vector3f& rotate, Vector3f& scale) {
    // takes the parameters for transformations and sets the 
    // Model matrix with transformation applied in this order
    // M = Translate * rotate * scale

    Matrix4f T = Transform::translate(translate);
    Matrix4f R = Transform::rotate(rotate);
    Matrix4f S = Transform::scale(scale);

    m_ModelMatrix = T*R*S;
}

Matrix4f Model::getModelMatrix() { return m_ModelMatrix; }
