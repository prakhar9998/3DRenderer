#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <SFML/Graphics.hpp>
#include "mesh.h"
#include "vector3d.h"

class Rasterizer {
private:
    sf::Uint8* m_PixelBuffer;
    
public:
    Rasterizer();
    ~Rasterizer();
    void setPixel(int x, int y, sf::Color color);
    void drawLine(Vector2i p1, Vector2i p2, sf::Color color);
    void drawMesh(Mesh* mesh);
    sf::Uint8* getBuffer();
};

#endif      // RASTERIZER_H
