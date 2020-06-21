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
    m_Viewport = Transform::viewportMatrix(
        DisplayBackend::WINDOW_WIDTH/8,
        DisplayBackend::WINDOW_HEIGHT/8,
        DisplayBackend::WINDOW_WIDTH*3/4,
        DisplayBackend::WINDOW_HEIGHT*3/4
    );
}

Renderer::~Renderer() {
    delete [] m_PixelBuffer;
}

// Matrix4f& Renderer::getViewportMatrix() { return m_Viewport; }

void Renderer::renderScene(Scene& scene, float cameraRotation) {
    m_Model = scene.getModel();
    m_Camera = scene.getCamera();
    Texture* tex = m_Model->getDiffuse();

    
    // get the required data of the mesh. 
    Mesh* mesh = m_Model->getMesh();
    std::vector<Vector3i>& vertices = mesh->getVertexIndices();
    std::vector<Vector3i>& texture = mesh->getTextureIndices();
    std::vector<Vector3i>& normals = mesh->getNormalIndices();
    
    Matrix4f Model = m_Model->getModelMatrix();
    m_Camera->setPosition(Vector3f(sin(cameraRotation) * 5.f, 1.f, cos(cameraRotation) * 5.f));
    Matrix4f View = m_Camera->getViewMatrix();
    Matrix4f Projection = m_Camera->getProjectionMatrix();
    Matrix4f Transformation = m_Viewport * Projection * View * Model;

    
    const int totalFaces = mesh->getNumFaces();
    
#pragma omp parallel
{
    GourardShader shader;
    Vector4f pts[3];
    Vector3f uv[3];

    #pragma omp for
    for (int i = 0; i < totalFaces; i++) {
        
        // perfrom early backface culling. Helps speed up a bit
        if (dot(m_Camera->getCameraDirection(), mesh->getFaceNormal(i)) < 0) continue;
        shader.transform = Transformation;
        
        // run vertex shader for every vertex.
        for (int j = 0; j < 3; j++) {
            uv[j] = mesh->getTexture(texture[i][j]);
            pts[j] = shader.vertex(mesh->getVertex(vertices[i][j]), mesh->getNormal(normals[i][j]), j);
        }

        Rasterizer::drawTriangle(pts, uv, tex, shader, m_PixelBuffer, m_Zbuffer);
    }
}
    std::fill(m_Zbuffer, m_Zbuffer + DisplayBackend::WINDOW_WIDTH*DisplayBackend::WINDOW_HEIGHT, std::numeric_limits<float>::max());
}

sf::Uint8* Renderer::getPixelBuffer() { return m_PixelBuffer; };
