#include "renderer.h"
#include "display.h"
#include "transform.h"
#include "rasterizer.h"

#include <math.h>

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

    // drawing mesh part
    
    Mesh* mesh = m_Model->getMesh();
    std::vector<Vector3i>& indices = mesh->getVertexIndices();
    // TODO: Add vector4f coords in mesh.
    Vector3f t[3];
    Vector4f pts[3];
    
    Matrix4f Model = m_Model->getModelMatrix();
    m_Camera->setPosition(Vector3f(sin(cameraRotation) * 5.f, 0.f, cos(cameraRotation) * 5.f));
    Matrix4f View = m_Camera->getViewMatrix();
    Matrix4f Projection = m_Camera->getProjectionMatrix();
    Matrix4f Transformation = m_Viewport * Projection * View * Model;

    srand(42);      // seed for random colors
    for (int i = 0; i < mesh->getNumFaces(); i++) {
        for (int j = 0; j < 3; j++) {
            t[j] = mesh->getVertex(indices[i][j]);
            t[j] = mesh->getVertex(indices[i][j]);
            t[j] = mesh->getVertex(indices[i][j]);
        }

        // early removal of faces away from camera. Helps speed up a bit.
        Vector3f n = normalize(cross(t[1] - t[0], t[2] - t[0]));
        if (dot(m_Camera->getCameraDirection(), n) < 0) continue;

        pts[0].x = t[0].x;
        pts[0].y = t[0].y;
        pts[0].z = t[0].z;
        pts[0].w = 1;
        pts[1].x = t[1].x;
        pts[1].y = t[1].y;
        pts[1].z = t[1].z;
        pts[1].w = 1;
        pts[2].x = t[2].x;
        pts[2].y = t[2].y;
        pts[2].z = t[2].z;
        pts[2].w = 1;

        pts[0] = Transformation * pts[0];
        pts[1] = Transformation * pts[1];
        pts[2] = Transformation * pts[2];

        Rasterizer::drawTriangle(pts, sf::Color(rand() % 255, rand() % 255, rand() % 255, 255), m_PixelBuffer, m_Zbuffer);
    }
    std::fill(m_Zbuffer, m_Zbuffer + DisplayBackend::WINDOW_WIDTH*DisplayBackend::WINDOW_HEIGHT, std::numeric_limits<float>::max());
}

sf::Uint8* Renderer::getPixelBuffer() { return m_PixelBuffer; };
