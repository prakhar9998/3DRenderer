#include "renderer.h"
#include "display.h"
#include "transform.h"
#include "rasterizer.h"
#include "shader.h"

#include <math.h>
#include <omp.h>

Renderer::Renderer() {
    m_PixelBuffer = new sf::Uint8[4 * DisplayBackend::WINDOW_WIDTH * DisplayBackend::WINDOW_HEIGHT];
    m_Zbuffer = new float[DisplayBackend::WINDOW_WIDTH * DisplayBackend::WINDOW_HEIGHT];
    std::fill_n(m_Zbuffer, DisplayBackend::WINDOW_WIDTH * DisplayBackend::WINDOW_HEIGHT, std::numeric_limits<float>::max());
    m_Camera = nullptr;
}

Renderer::~Renderer() {
    delete [] m_PixelBuffer;
}

void Renderer::perspectiveDivide(Vector4f* verts) {
    // verts[0] = verts[0]/verts[0].w;
    // verts[1] = verts[1]/verts[1].w;
    // verts[2] = verts[2]/verts[2].w;

    for (int i = 0; i < 3; i++) {
        verts[i].x /= verts[i].w;
        verts[i].y /= verts[i].w;
        verts[i].z /= verts[i].w;
    }
}

// Matrix4f& Renderer::getViewportMatrix() { return m_Viewport; }

void Renderer::renderScene(Scene& scene, float cameraRotation) {
    m_Model = scene.getModel();
    m_Camera = scene.getCamera();

    // get the required data of the mesh. 
    Mesh* mesh = m_Model->getMesh();
    std::vector<Vector3i>& vertices = mesh->getVertexIndices();
    std::vector<Vector3i>& textures = mesh->getTextureIndices();
    std::vector<Vector3i>& normals = mesh->getNormalIndices();
    
    Matrix4f M = m_Model->getModelMatrix();
    Matrix4f MInv = inverse(m_Model->getModelMatrix());      // world to object matrix

    m_Camera->setPosition(Vector3f(sin(cameraRotation) * 3, 0, cos(cameraRotation) * 3));

    const int totalFaces = mesh->getNumFaces();
    
#pragma omp parallel
{
    PhongShader shader;
    Vector4f pts[3];
    Vector3f uv[3];
    shader.V = m_Camera->getViewMatrix();
    shader.MV = shader.V * M;
    shader.MVP = m_Camera->getProjectionMatrix() * shader.MV;
    shader.N = transpose(inverse(shader.MV));
    shader.diffuse_map = m_Model->getDiffuse();

    #pragma omp for schedule(dynamic)
    for (int i = 0; i < totalFaces; i++) {
        
        // perfrom early backface culling by checking the dot product of viewer in object space to vertex and vertex normal
        if (dot(normalize(multMatrixVec(MInv, m_Camera->getCameraDirection()) - mesh->getVertex(vertices[i][0])), mesh->getFaceNormal(i)) < 0) continue;

        // run vertex shader for every vertex.
        for (int j = 0; j < 3; j++) {
            pts[j] = shader.vertex(mesh->getVertex(vertices[i][j]), mesh->getNormal(normals[i][j]), mesh->getTexture(textures[i][j]), j);
        }

        // perform perspective divide: dividing every element of vector by homogenous coordinate
        perspectiveDivide(pts);
        // Rasterizer::drawWireframe(pts, m_PixelBuffer);
        Rasterizer::drawTriangle(pts, shader, m_PixelBuffer, m_Zbuffer);
    }
}
    std::fill(m_Zbuffer, m_Zbuffer + DisplayBackend::WINDOW_WIDTH*DisplayBackend::WINDOW_HEIGHT, std::numeric_limits<float>::max());
}

sf::Uint8* Renderer::getPixelBuffer() { return m_PixelBuffer; };
