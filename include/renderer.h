#ifndef RENDERER_H
#define RENDERER_H

#include "scene.h"
#include <SFML/Graphics.hpp>

class Renderer {
private:
    sf::Uint8* m_PixelBuffer;
    Camera* m_Camera;
    Model* m_Model;
public:
    Renderer();
    ~Renderer();
    void renderScene(Scene& scene, float cameraRotation);
    sf::Uint8* getPixelBuffer();
};

#endif      // RENDERER_H
