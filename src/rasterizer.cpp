#include "rasterizer.h"
#include "math.h"
#include "camera.h"
#include "matrix.h"
#include "display.h"

Rasterizer::Rasterizer() {}

Rasterizer::~Rasterizer() {}

void Rasterizer::setPixel(int i, int j, sf::Color color, sf::Uint8* pixelBuffer) {
    size_t pixelOffset = ((DisplayBackend::WINDOW_HEIGHT - j) * DisplayBackend::WINDOW_WIDTH + i) * 4;
    pixelBuffer[pixelOffset] = color.r;
    pixelBuffer[pixelOffset + 1] = color.g;
    pixelBuffer[pixelOffset + 2] = color.b;
    pixelBuffer[pixelOffset + 3] = color.a;
}

void Rasterizer::drawLine(Vector2i p1, Vector2i p2, sf::Color color, sf::Uint8 *pixelBuffer) {
    if (p1.x < 0 || p1.y < 0) return;
    if (p2.x < 0 || p2.y < 0) return;
    if (p1.x > DisplayBackend::WINDOW_WIDTH || p1.y > DisplayBackend::WINDOW_HEIGHT) return;
    if (p2.x > DisplayBackend::WINDOW_WIDTH || p2.y > DisplayBackend::WINDOW_HEIGHT) return;

    bool steep = false;
    if (std::abs(p1.x-p2.x) < std::abs(p1.y-p2.y)) {        // line is steep
        std::swap(p1.x, p1.y);
        std::swap(p2.x, p2.y);
        steep = true;
    }
    
    if (p1.x > p2.x) {
        std::swap(p1.x, p2.x);
        std::swap(p1.y, p2.y);
    }

    int dx = p2.x-p1.x;
    int dy = p2.y-p1.y;
    
    int derror = std::abs(dy) * 2;
    int slopeError = 0;
    int y = p1.y;

    if (steep) {
        for (int x = p1.x; x <= p2.x; x++) {
            setPixel(y, x, color, pixelBuffer);
            slopeError += derror;
            if (slopeError > dx) {
                y += (p2.y > p1.y ? 1: -1);
                slopeError -= dx * 2;
            }
        }
    } else {
        for (int x = p1.x; x <= p2.x; x++) {
            setPixel(x, y, color, pixelBuffer);
            slopeError += derror;
            if (slopeError > dx) {
                y += (p2.y > p1.y ? 1: -1);
                slopeError -= dx * 2;
            }
        }
    }
}

// void Rasterizer::drawMesh(Mesh* mesh, float rot) {
//     std::vector<Vector3i> vertices = mesh->getVertexIndices();
    
//     for (int i = 0; i < mesh->getNumFaces(); i++) {
//         Vector3f t1, t2, t3;
//         t1 = mesh->getVertex(vertices[i].x);
//         t2 = mesh->getVertex(vertices[i].y);
//         t3 = mesh->getVertex(vertices[i].z);

//         Vector4f v1(t1.x, t1.y, t1.z, 1);
//         Vector4f v2(t2.x, t2.y, t2.z, 1);
//         Vector4f v3(t3.x, t3.y, t3.z, 1);

//         float camX = sin(rot) * 0.75f;
//         float camZ = cos(rot) * 0.75f;

//         Vector3f eye(camX, 0.f, camZ);
//         Vector3f at(0.0f, 0.0f, 0.0f);
//         Vector3f up(0.0f, 1.0f, 0.0f);

//         Matrix4f ModelView = Camera::lookAt(eye, at, up);
//         Matrix4f Projection = Camera::Projection(-1./norm(at-eye));
//         Matrix4f Viewport = Camera::Viewport(DisplayBackend::WINDOW_WIDTH/8, DisplayBackend::WINDOW_HEIGHT/8, DisplayBackend::WINDOW_WIDTH*3/4, DisplayBackend::WINDOW_HEIGHT*3/4);
//         v1 = (Viewport * Projection * ModelView * v1);
//         v2 = (Viewport * Projection * ModelView * v2);
//         v3 = (Viewport * Projection * ModelView * v3);

//         v1 = v1 / v1.z;
//         v2 = v2 / v2.z;
//         v3 = v3 / v3.z;

//         Vector2i p1(v1.x, v1.y);
//         Vector2i p2(v2.x, v2.y);
//         Vector2i p3(v3.x, v3.y);


//         drawLine(p1, p2, sf::Color::White);
//         drawLine(p2, p3, sf::Color::White);
//         drawLine(p1, p3, sf::Color::White);
//     }
// }
