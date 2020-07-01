#ifndef SHADER_H
#define SHADER_H

#include "vector3d.h"
#include "matrix.h"

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
// This implementation uses phong reflection model and gourard shading.
struct GouraudShader : IShader {
    Matrix4f MVP, N;
    Vector3f varIntensity, varSpec, nrm;
    Vector3f light = normalize(Vector3f{1, 1, 1});
    Vector3i rgb{255, 255, 255};
    Texture* diffuse_map;

    Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, int nth) override {
        nrm = multMatrixDir(N, normal);     // correct normal
        Vector3f reflection = normalize(nrm * (dot(light, nrm) * 2.f) - light);
        varSpec[nth] = pow(std::max(reflection.z, 0.0f), 32);
        varIntensity[nth] = std::max(0.f, dot(nrm, light));
        Vector4f gl_Vertex = Vector4f(vertex.x, vertex.y, vertex.z, 1.f);
        return MVP * gl_Vertex;
    }

    Vector3f fragment(Vector2f uv, const Vector3f& barycentric) override {
        float intensity = dot(varIntensity, barycentric);
        float spec = dot(varSpec, barycentric);
        float a = intensity + .6 * spec;
        Vector3i color = diffuse_map->getColor(uv.x * diffuse_map->width, uv.y * diffuse_map->height);
        float r = 5 + color.x*(intensity + .6 * spec);
        float g = 5 + color.y*(intensity + .6 * spec);
        float b = 5 + color.z*(intensity + .6 * spec);
        return Vector3f(r, g, b);
    }
};

#endif      // SHADER_H
