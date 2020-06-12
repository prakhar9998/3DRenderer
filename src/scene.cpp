#include "scene.h"

Scene::Scene() {}

Scene::~Scene() {}

Model* Scene::getModel(){ 
    return m_Model;
}

Camera* Scene::getCamera() { 
    // TODO: Add a vector to hold camera, also have a default camera
    // to render the scene from.
    return m_Camera;
}

void Scene::loadModelInScene(Model* model) {
    /**
     * TODO: Add a vector to hold every model.
     * */

    m_Model = model;
}

void Scene::loadCameraInScene(Camera* camera) {
    m_Camera = camera;
}
