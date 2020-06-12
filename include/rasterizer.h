#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <SFML/Graphics.hpp>
#include "mesh.h"
#include "vector3d.h"

class Rasterizer {    
public:
    Rasterizer();
    ~Rasterizer();
    static void setPixel(int x, int y, sf::Color color, sf::Uint8* pixelBuffer);
    static void drawLine(Vector2i p1, Vector2i p2, sf::Color color, sf::Uint8* pixelBuffer);
    // void drawMesh(Mesh* mesh, float rot);
};

#endif      // RASTERIZER_H
