#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <SFML/Graphics.hpp>
#include "mesh.h"
#include "vector3d.h"
#include "model.h"
#include "shader.h"

/**
 * Rasterizer class holds functions which are required to rasterize a triangle
 * on the screen. Contains only static methods as it doesn't hold any data,
 * maybe create a namespace instead of a class just like Transform namespace?
 * */

class Rasterizer {    
public:
    Rasterizer();
    ~Rasterizer();
    static Vector3f barycentric(const Vector4f& v0, const Vector4f& v1, const Vector4f& v2, const Vector2i& P);
    static float edgeFunction(const Vector4f& a, const Vector4f& b, const Vector2i& p);
    static float edgeFunction(const Vector4f& a, const Vector4f& b, const Vector4f& c);
    static bool isInside(const Vector4f& v0, const Vector4f& v1, const Vector4f& v2, const Vector2i& P);
    static void setPixel(int x, int y, sf::Color color, sf::Uint8* pixelBuffer);
    static void drawLine(Vector2i p1, Vector2i p2, sf::Color color, sf::Uint8* pixelBuffer);
    static void drawTriangle(Vector4f *pts, Vector3f* uv, IShader& shader, sf::Uint8* pixelBuffer, float* zbuffer);
    static void drawWireframe(Vector4f *pts, sf::Uint8* pixelBuffer);
    static Matrix4f Viewport;
};

#endif      // RASTERIZER_H
