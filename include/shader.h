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
    virtual Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, int nth) = 0;
    virtual bool fragment(const Vector3f& barycentric, sf::Color& color) = 0;
};


// simplest and fastest shader; uses face normals to compute light values.
struct FlatShader : IShader {
    Matrix4f MVP;
    float varying_intensity;
    Vector3f light;
    // TODO: Remove light and add it to the scene class.
    // I am declaring it here for testing purposes.;
    
    Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, int nth) override {
        light = Vector3f(0.f, 0.f, 1.f);
        varying_intensity = std::max(0.f, dot(normal, light));
        Vector4f gl_Vertex = Vector4f(vertex.x, vertex.y, vertex.z, 1.f);
        return MVP * gl_Vertex;
    }

    bool fragment(const Vector3f& barycentric, sf::Color& color) override {
        assert(varying_intensity <= 1);
        color = sf::Color(color.r * varying_intensity, color.g * varying_intensity, color.b * varying_intensity, 255);
        return false;
    }
};


// Much better than flat shader, interpolates normal for every point and computes
// light values for every pixel on the surface. Requires more processing that flat shader.
struct GourardShader : IShader {
    Matrix4f MVP;
    Vector3f varIntensity;
    Vector3f light = Vector3f(1.f, 0.f, 1.f);

    Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, int nth) override {
        varIntensity[nth] = std::max(0.f, dot(normal, light));
        Vector4f gl_Vertex = Vector4f(vertex.x, vertex.y, vertex.z, 1.f);
        return MVP * gl_Vertex;
    }

    bool fragment(const Vector3f& barycentric, sf::Color& color) override {
        float intensity = dot(varIntensity, barycentric);
        color = sf::Color(color.r * intensity, color.g * intensity, color.b * intensity, 255);
        return false;
    }
};

#endif      // SHADER_H
