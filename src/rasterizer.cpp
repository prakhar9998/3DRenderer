#include "rasterizer.h"
#include "math.h"
#include "camera.h"
#include "matrix.h"
#include "display.h"

Rasterizer::Rasterizer() {}

Rasterizer::~Rasterizer() {}

void Rasterizer::setPixel(int i, int j, sf::Color color, sf::Uint8* pixelBuffer) {
    if (i < 0 || j < 0) return;
    if (i < 0 || j < 0) return;
    if (i > DisplayBackend::WINDOW_WIDTH || j > DisplayBackend::WINDOW_HEIGHT) return;
    if (i > DisplayBackend::WINDOW_WIDTH || j > DisplayBackend::WINDOW_HEIGHT) return;
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

Vector3f Rasterizer::barycentric(Vector2f v0, Vector2f v1, Vector2f v2, Vector2f P) {
    // compute barycentric coordinates of the triangle represented by the given vertices

    // TODO: Look for ways to optimize it further. Current bottleneck in the code.

    // the following calculations were taking too much frame time.
    // maybe pre-calculate p0 and p1 since they're independent of P ?
    // Vector2f p0 = v1 - v0;
    // Vector2f p1 = v2 - v0;
    // Vector2f p2 = P - v0;

    // float invDenom = 1 / (p0.x * p1.y - p1.x * p0.y);
    // float u = (p2.x * p1.y - p1.x * p2.y) * invDenom;
    // float v = (p0.x * p2.y - p2.x * p0.y) * invDenom;

    float invDenom = 1 / ((v1.x-v0.x) * (v2.y-v0.y) - (v2.x-v0.x) * (v1.y-v0.y));
    float u = ((P.x-v0.x) * (v2.y-v0.y) - (v2.x-v0.x) * (P.y-v0.y)) * invDenom;
    float v = ((v1.x-v0.x) * (P.y-v0.y) - (P.x-v0.x) * (v1.y-v0.y)) * invDenom;

    return Vector3f(1.f - u - v, u, v);
}

void Rasterizer::drawTriangle(Vector4f *pts, sf::Color color, sf::Uint8* pixelBuffer) {
    // calculate bounding box of the three coordinates.

    float minX = std::min(pts[0].x/pts[0].w, std::min(pts[1].x/pts[1].w, pts[2].x/pts[2].w));
    float maxX = std::max(pts[0].x/pts[0].w, std::max(pts[1].x/pts[1].w, pts[2].x/pts[2].w));
    float minY = std::min(pts[0].y/pts[0].w, std::min(pts[1].y/pts[1].w, pts[2].y/pts[2].w));
    float maxY = std::max(pts[0].y/pts[0].w, std::max(pts[1].y/pts[1].w, pts[2].y/pts[2].w));

    // std::cout << minX << " " << maxX << " " << minY << " " << maxY << std::endl;

    Vector2i pixel;
    for (pixel.x = minX; pixel.x < maxX; pixel.x++) {       // implicit conversion to int
        for (pixel.y = minY; pixel.y < maxY; pixel.y++) {
            Vector3f barc = barycentric(
                Vector2f(pts[0].x/pts[0].w, pts[0].y/pts[0].w),
                Vector2f(pts[1].x/pts[1].w, pts[1].y/pts[1].w),
                Vector2f(pts[2].x/pts[2].w, pts[2].y/pts[2].w),
                Vector2f(pixel.x, pixel.y)
            );
            if (barc.x < 0 || barc.y < 0 || barc.z < 0) continue;       // the point is not inside the triangle

            // random color for now, later shader will decide the color.
            setPixel(pixel.x, pixel.y, color, pixelBuffer);
        }
    }
}