#ifndef RENDERER_H
#define RENDERER_H

#include "scene.h"
#include <SFML/Graphics.hpp>

class Renderer {
private:
    sf::Uint8* m_PixelBuffer;
    float* m_Zbuffer;
    Camera* m_Camera;
    Model* m_Model;
    Matrix4f m_Viewport;      // viewport matrix for rendering scene
public:
    Renderer();
    ~Renderer();
    void renderScene(Scene& scene, float cameraRotation);
    sf::Uint8* getPixelBuffer();
    // Matrix4f& getViewportMatrix();
};

#endif      // RENDERER_H
