#include "renderer.h"
#include "display.h"
#include "transform.h"
#include "rasterizer.h"

#include <math.h>

Renderer::Renderer() {
    m_PixelBuffer = new sf::Uint8[4 * DisplayBackend::WINDOW_WIDTH * DisplayBackend::WINDOW_HEIGHT];
    m_Camera = nullptr;
}

Renderer::~Renderer() {
    delete [] m_PixelBuffer;
}

void Renderer::renderScene(Scene scene, float cameraRotation) {
    m_Model = scene.getModel();
    m_Camera = scene.getCamera();

    // drawing mesh part
    
    Mesh* mesh = m_Model->getMesh();
    std::vector<Vector3i> indices = mesh->getVertexIndices();
    // TODO: Add vector4f coords in mesh.
    Vector3f t1, t2, t3;
    Vector4f v1, v2, v3;
    Matrix4f Model = m_Model->getModelMatrix();
    m_Camera->setPosition(Vector3f(sin(cameraRotation) * 2.f, 0, cos(cameraRotation) * 2.f));
    Matrix4f View = m_Camera->getViewMatrix();
    Matrix4f Projection = m_Camera->getProjectionMatrix();
    Matrix4f Viewport = Transform::viewportMatrix(
        DisplayBackend::WINDOW_WIDTH/8,
        DisplayBackend::WINDOW_HEIGHT/8,
        DisplayBackend::WINDOW_WIDTH*3/4,
        DisplayBackend::WINDOW_HEIGHT*3/4
    );
    Matrix4f MVP = Projection * View * Model;
    Matrix4f Transformation = Viewport * MVP;

    for (int i = 0; i < mesh->getNumFaces(); i++) {
        t1 = mesh->getVertex(indices[i].x);
        t2 = mesh->getVertex(indices[i].y);
        t3 = mesh->getVertex(indices[i].z);
        v1.x = t1.x;
        v1.y = t1.y;
        v1.z = t1.z;
        v1.w = 1;
        v2.x = t2.x;
        v2.y = t2.y;
        v2.z = t2.z;
        v2.w = 1;
        v3.x = t3.x;
        v3.y = t3.y;
        v3.z = t3.z;
        v3.w = 1;

        v1 = Transformation * v1;
        v2 = Transformation * v2;
        v3 = Transformation * v3;
        v1 = v1 / v1.w;
        v2 = v2 / v2.w;
        v3 = v3 / v3.w;
        Rasterizer::drawLine(Vector2i(v1.x, v1.y), Vector2i(v2.x, v2.y), sf::Color::White, m_PixelBuffer);
        Rasterizer::drawLine(Vector2i(v2.x, v2.y), Vector2i(v3.x, v3.y), sf::Color::White, m_PixelBuffer);
        Rasterizer::drawLine(Vector2i(v1.x, v1.y), Vector2i(v3.x, v3.y), sf::Color::White, m_PixelBuffer);
    }
}

sf::Uint8* Renderer::getPixelBuffer() { return m_PixelBuffer; };