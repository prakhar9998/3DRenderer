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
    virtual Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, const Vector3f& texture, int nth) = 0;
    virtual Vector3f fragment(const Vector3f& barycentric) = 0;
};

// simplest and fastest shader; uses face normals to compute light values.
struct FlatShader : IShader {
    Matrix4f MVP;
    float varying_intensity;
    Vector3f light;
    // TODO: Remove light and add it to the scene class.
    // I am declaring it here for testing purposes.;
    
    Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, const Vector3f& texture, int nth) override {
        light = Vector3f(0.f, 0.f, 1.f);
        varying_intensity = std::max(0.f, dot(normal, light));
        Vector4f gl_Vertex = Vector4f(vertex.x, vertex.y, vertex.z, 1.f);
        return MVP * gl_Vertex;
    }

    Vector3f fragment(const Vector3f& barycentric) override {
        assert(varying_intensity <= 1);
        return Vector3f{255 * varying_intensity, 255 * varying_intensity, 255 * varying_intensity};
    }
};

// Much better than flat shader, interpolates normal for every point and computes
// light values for every pixel on the surface. Requires more processing that flat shader.
// This implementation uses phong reflection model and gourard shading.
struct GouraudShader : IShader {
    Matrix4f MVP, N, V, MV;
    Vector3f varIntensity, varSpec, varUV[3], nrm, viewDir;
    Vector3f light;
    Vector3i rgb{255, 255, 255};
    Texture* diffuse_map;

    Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, const Vector3f& texture, int nth) override {
        nrm = normalize(multMatrixDir(N, normal));     // correct normal
        light = normalize(Vector3f{1, 1, 1});
        light = normalize(multMatrixDir(V, light));
        viewDir = normalize(multMatrixVec(MV, vertex));

        Vector3f reflection = normalize(nrm * (dot(light, nrm) * 2.f) - light);

        varUV[nth] = texture;
        varSpec[nth] = pow(std::max(dot(normalize(light-viewDir), reflection), 0.0f), 32);
        varIntensity[nth] = std::max(0.f, dot(nrm, light));

        Vector4f gl_Vertex = Vector4f(vertex.x, vertex.y, vertex.z, 1.f);
        return MVP * gl_Vertex;
    }

    Vector3f fragment(const Vector3f& barycentric) override {
        // interpolating attributes
        Vector2f uv;
        uv.x = varUV[0].x * barycentric[0] + varUV[1].x * barycentric[1] + varUV[2].x * barycentric[2];
        uv.y = varUV[0].y * barycentric[0] + varUV[1].y * barycentric[1] + varUV[2].y * barycentric[2];
        float intensity = dot(varIntensity, barycentric);
        float spec = dot(varSpec, barycentric);
        
        float a = (.1f + intensity + .6f * spec);       // multiplier ambient + diffuse + specular

        Vector3i color = diffuse_map->getColor(uv.x * diffuse_map->width, uv.y * diffuse_map->height);
        float r = std::min(color.x*a, 255.f);
        float g = std::min(color.y*a, 255.f);
        float b = std::min(color.z*a, 255.f);
        // std::cout << r << " " << g << " " << b << std::endl;
        return Vector3f(r, g, b);
    }
};

#endif      // SHADER_H
