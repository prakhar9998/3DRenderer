#ifndef SHADER_H
#define SHADER_H

#include "vector3d.h"
#include "matrix.h"

#include <SFML/Graphics/Color.hpp>

/**
 * This shader class will change a lot in future, now this is just
 * to get a rough idea of how I will integrate shaders in the code.
 * */

struct IShader {
    virtual ~IShader() {}
    virtual Vector4f vertex(const Vector3f& vertex, const Vector3f& normal) = 0;
    virtual bool fragment(sf::Color& color) = 0;
};

struct FlatShader : IShader {
    Matrix4f transform;
    float varying_intensity;
    Vector3f light;
    // TODO: Remove light and add it to the scene class.
    // I am declaring it here for testing purposes.;
    
    virtual Vector4f vertex(const Vector3f& vertex, const Vector3f& normal) override {
        light = Vector3f(0.f, 0.f, 1.f);
        varying_intensity = std::max(0.f, dot(normal, light));
        Vector4f gl_Vertex = Vector4f(vertex.x, vertex.y, vertex.z, 1.f);
        return transform * gl_Vertex;
    }

    virtual bool fragment(sf::Color& color) override {
        assert(varying_intensity <= 1);
        color = sf::Color(255 * varying_intensity, 255 * varying_intensity, 255 * varying_intensity, 255);
        return false;
    }
};

#endif      // SHADER_H