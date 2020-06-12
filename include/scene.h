#ifndef SCENE_H
#define SCENE_H

/**
 * TODO:
 * This class will contain every model that is to be rendered in the scene
 * along with the cameras and the InputManager class will handle how user
 * interacts with this scene such as camera movement. Renderer class will have a 
 * render method to render a given scene.
 * */

#include "model.h"
#include "camera.h"

#include <vector>

class Scene {
private:
    Camera* m_Camera;
    Model* m_Model;
    std::string m_Path;
public:
    Scene();
    ~Scene();
    Model* getModel();
    Camera* getCamera();
    void loadModelInScene(Model* model);
    void loadCameraInScene(Camera* camera);
};

#endif      // SCENE_H