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
    virtual Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, const Vector3f& texture, 
                    const Vector3f& tangent, const Vector3f& bitangent, int nth) = 0;
    virtual Vector3f fragment(const Vector3f& barycentric) = 0;
};

// simplest and fastest shader; uses face normals to compute light values.
struct FlatShader : IShader {
    Matrix4f MVP;
    float varying_intensity;
    Vector3f light;
    // TODO: Remove light and add it to the scene class.
    // I am declaring it here for testing purposes.;
    
    Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, const Vector3f& texture, 
                    const Vector3f& tangent, const Vector3f& bitangent, int nth) override {
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
    Texture* diffuse_map;

    Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, const Vector3f& texture, 
                    const Vector3f& tangent, const Vector3f& bitangent, int nth) override {

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

// calculate attributes at every fragment as opposed to only in vertex by
// interpolating the normals in fragment shader and then calculating lighting.
// this shader uses Phong Shading and Phong reflection model
struct PhongShader : IShader {
    Matrix4f MVP, N, V, MV, M;
    Vector3f varNormal[3], varUV[3], varPos[3];        // varying attributes, set by vertex shader, used by fragment shader
    Vector3f light;                         // light pos

    Texture* diffuse_map;                   // textures to use for mapping

    Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, const Vector3f& texture, 
                    const Vector3f& tangent, const Vector3f& bitangent, int nth) override {

        varNormal[nth] = normalize(multMatrixDir(N, normal));      // correct normal in view space
        varUV[nth] = texture;
        varPos[nth] = normalize(multMatrixVec(MV, vertex));

        // setting light position. Later it'll be as an argument in vertex shader.
        light = normalize(Vector3f{1, 1, 1});
        light = normalize(multMatrixDir(M, light));     // light in view space

        Vector4f gl_Vertex = Vector4f(vertex.x, vertex.y, vertex.z, 1);

        return MVP * gl_Vertex;
    }

    Vector3f fragment(const Vector3f& barycentric) {
        Vector3f nrm, uv, viewDir;
        // interpolating normal for every fragment
        nrm.x = varNormal[0].x * barycentric[0] + varNormal[1].x * barycentric[1] + varNormal[2].x * barycentric[2];
        nrm.y = varNormal[0].y * barycentric[0] + varNormal[1].y * barycentric[1] + varNormal[2].y * barycentric[2];
        nrm.z = varNormal[0].z * barycentric[0] + varNormal[1].z * barycentric[1] + varNormal[2].z * barycentric[2];

        // interpolating uv coordinates
        uv.x = varUV[0].x * barycentric[0] + varUV[1].x * barycentric[1] + varUV[2].x * barycentric[2];
        uv.y = varUV[0].y * barycentric[0] + varUV[1].y * barycentric[1] + varUV[2].y * barycentric[2];

        // interpolating fragment position
        viewDir.x = varPos[0].x * barycentric[0] + varPos[1].x * barycentric[0] + varPos[2].x * barycentric[2];
        viewDir.y = varPos[0].y * barycentric[0] + varPos[1].y * barycentric[0] + varPos[2].y * barycentric[2];
        viewDir.z = varPos[0].z * barycentric[0] + varPos[1].z * barycentric[0] + varPos[2].z * barycentric[2];

        // setting up the necessary vectors for phong reflection
        Vector3f reflection = normalize(nrm * (dot(light, nrm) * 2.f) - light);
        viewDir = normalize(viewDir*-1.f);

        // calculating specular and diffuse values
        float spec = pow(std::max(dot(viewDir, reflection), 0.f), 32);
        float diff = std::max(0.0f, dot(nrm, light));

        uv.x *= diffuse_map->width;
        uv.y *= diffuse_map->height;
        Vector3i color = diffuse_map->getColor(uv.x, uv.y);

        // setting colors
        float r = std::min<float>(color.x * (.1 + diff + .6*spec), 255);
        float g = std::min<float>(color.y * (.1 + diff + .6*spec), 255);
        float b = std::min<float>(color.z * (.1 + diff + .6*spec), 255);

        return Vector3f(r, g, b);
    }
};

struct NormalMap : IShader {
    Matrix4f MVP, N, V, MV, M;
    Vector3f varNormal[3], varUV[3], varPos[3], varTangent[3], varBitangent[3], varLight[3];        // varying attributes, set by vertex shader, used by fragment shader
    Vector3f light;                         // light pos
    Texture *diffuse_map, *normal_map;                   // textures to use for mapping
    Matrix3f TBN;
    Vector3f cameraPos;

    Vector4f vertex(const Vector3f& vertex, const Vector3f& normal, const Vector3f& texture, 
                    const Vector3f& tangent, const Vector3f& bitangent, int nth) {
        
        varNormal[nth] = normalize(multMatrixDir(M, normal));      // correct normal in view space
        // varNormal[nth] = normalize(normal);
        varUV[nth] = texture;
        
        varTangent[nth] = normalize(multMatrixDir(M, tangent));
        varBitangent[nth] = normalize(multMatrixDir(M, bitangent));
        TBN.x = varTangent[nth];
        TBN.y = varBitangent[nth];
        TBN.z = varNormal[nth];

        // setting light position. Later it'll be as an argument in vertex shader.
        light = normalize(Vector3f{1, 1, 1});
        light = normalize(multMatrixDir(M, light));     // light in view space
        varLight[nth] = TBN * light;
        
        // varPos[nth] = normalize(multMatrixVec(MV, vertex));
        varPos[nth] = TBN * normalize(multMatrixVec(MV, vertex));
        
        Vector4f gl_Vertex = Vector4f(vertex.x, vertex.y, vertex.z, 1);

        return MVP * gl_Vertex;
    }

    Vector3f fragment(const Vector3f& barycentric) {
        Vector3f uv, viewDir, lightDir;

        // interpolating uv coordinates
        uv.x = varUV[0].x * barycentric[0] + varUV[1].x * barycentric[1] + varUV[2].x * barycentric[2];
        uv.y = varUV[0].y * barycentric[0] + varUV[1].y * barycentric[1] + varUV[2].y * barycentric[2];

        lightDir.x = varLight[0].x * barycentric[0] + varLight[1].x * barycentric[1] + varLight[2].x * barycentric[2];
        lightDir.y = varLight[0].y * barycentric[0] + varLight[1].y * barycentric[1] + varLight[2].y * barycentric[2];
        lightDir.z = varLight[0].z * barycentric[0] + varLight[1].z * barycentric[1] + varLight[2].z * barycentric[2];

        viewDir.x = varPos[0].x * barycentric[0] + varPos[1].x * barycentric[0] + varPos[2].x * barycentric[2];
        viewDir.y = varPos[0].y * barycentric[0] + varPos[1].y * barycentric[0] + varPos[2].y * barycentric[2];
        viewDir.z = varPos[0].z * barycentric[0] + varPos[1].z * barycentric[0] + varPos[2].z * barycentric[2];

        Vector3i nc = normal_map->getColor(uv.x * normal_map->width, uv.y * normal_map->height);
        Vector3f n;
        n.x = (nc.x * 2.0f)/255 - 1.0f;
        n.y = (nc.y * 2.0f)/255 - 1.0f;
        n.z = (nc.z * 2.0f)/255 - 1.0f;

        float diff = std::max(0.f, dot(n, lightDir));

        Vector3f reflection = normalize(n * (dot(lightDir, n) * 2.f) - lightDir);
        viewDir = normalize(cameraPos + viewDir*-1.f);

        // calculating specular and diffuse values
        float spec = pow(std::max(dot(viewDir, reflection), 0.f), 32);

        Vector3i color = diffuse_map->getColor(uv.x * diffuse_map->width, uv.y * diffuse_map->height);

        // setting colors
        float r = std::min<float>(color.x * (.1 + diff + .6*spec), 255);
        float g = std::min<float>(color.y * (.1 + diff + .6*spec), 255);
        float b = std::min<float>(color.z * (.1 + diff + .6*spec), 255);

        return Vector3f(r, g, b);
    }
};

#endif      // SHADER_H
