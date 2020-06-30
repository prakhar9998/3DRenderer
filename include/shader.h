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
    virtual Vector3f fragment(Vector2f uv, const Vector3f& barycentric) = 0;
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

    Vector3f fragment(Vector2f uv, const Vector3f& barycentric) override {
        assert(varying_intensity <= 1);
        return Vector3f{255 * varying_intensity, 255 * varying_intensity, 255 * varying_intensity};
    }
};

// Much better than flat shader, interpolates normal for every point and computes
// light values for every pixel on the surface. Requires more processing that flat shader.
struct GourardShader : IShader {
    Matrix4f MVP, N;
    Vector3f varIntensity, nrm;
    Vector3f light = normalize(Vector3f{1, 1, 1});
    Vector3i rgb{255, 255, 255};
    Texture* diffuse;

    Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, int nth) override {
        nrm = multMatrixDir(N, normal);     // correct normal
        varIntensity[nth] = std::max(0.f, dot(nrm, light));
        Vector4f gl_Vertex = Vector4f(vertex.x, vertex.y, vertex.z, 1.f);
        return MVP * gl_Vertex;
    }

    Vector3f fragment(Vector2f uv, const Vector3f& barycentric) override {
        float intensity = dot(varIntensity, barycentric);
        Vector3i color = diffuse->getColor(uv.x * diffuse->width, uv.y * diffuse->height);
        return Vector3f(color.x, color.y, color.z) * intensity;
    }
};


#endif      // SHADER_H
