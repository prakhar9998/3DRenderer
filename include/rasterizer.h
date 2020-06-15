#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <SFML/Graphics.hpp>
#include "mesh.h"
#include "vector3d.h"

/**
 * Rasterizer class holds functions which are required to rasterize a triangle
 * on the screen. Contains only static methods as it doesn't hold any data,
 * maybe create a namespace instead of a class just like Transform namespace?
 * */

class Rasterizer {    
public:
    Rasterizer();
    ~Rasterizer();
    static Vector3f barycentric(Vector2f v0, Vector2f v1, Vector2f v2, Vector2f P);
    static void setPixel(int x, int y, sf::Color color, sf::Uint8* pixelBuffer);
    static void drawLine(Vector2i p1, Vector2i p2, sf::Color color, sf::Uint8* pixelBuffer);
    static void drawTriangle(Vector4f *pts, sf::Color color, sf::Uint8* pixelBuffer);
};

#endif      // RASTERIZER_H
