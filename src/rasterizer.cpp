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

void Rasterizer::drawWireframe(Vector4f *pts, sf::Uint8* pixelBuffer) {
    Vector2i v[3];
    for (int i = 0; i < 3; i++) {
        v[i].x = pts[i].x/pts[i].w;
        v[i].y = pts[i].y/pts[i].w;
    }

    drawLine(v[0], v[1], sf::Color::White, pixelBuffer);
    drawLine(v[0], v[2], sf::Color::White, pixelBuffer);
    drawLine(v[1], v[2], sf::Color::White, pixelBuffer);
}

Vector3f Rasterizer::barycentric(Vector2f v0, Vector2f v1, Vector2f v2, Vector2f P) {
    // compute barycentric coordinates of the triangle represented by the given vertices

    float denom = ((v1.x-v0.x) * (v2.y-v0.y) - (v2.x-v0.x) * (v1.y-v0.y));
    if (denom < std::abs(1e-3)) return Vector3f(-1.f, 0.f, 0.f);         // degenerate triangle
    float invDenom = 1 / ((v1.x-v0.x) * (v2.y-v0.y) - (v2.x-v0.x) * (v1.y-v0.y));
    float u = ((P.x-v0.x) * (v2.y-v0.y) - (v2.x-v0.x) * (P.y-v0.y)) * invDenom;
    float v = ((v1.x-v0.x) * (P.y-v0.y) - (P.x-v0.x) * (v1.y-v0.y)) * invDenom;

    return Vector3f(1.f - u - v, u, v);
}

void Rasterizer::drawTriangle(Vector4f *pts, Vector3f* uv_coords, Texture* tex, IShader &shader, sf::Uint8* pixelBuffer, float* zbuffer) {

    // calculate bounding box of the three coordinates.
    float minX = std::min(pts[0].x/pts[0].w, std::min(pts[1].x/pts[1].w, pts[2].x/pts[2].w));
    float maxX = std::max(pts[0].x/pts[0].w, std::max(pts[1].x/pts[1].w, pts[2].x/pts[2].w));
    float minY = std::min(pts[0].y/pts[0].w, std::min(pts[1].y/pts[1].w, pts[2].y/pts[2].w));
    float maxY = std::max(pts[0].y/pts[0].w, std::max(pts[1].y/pts[1].w, pts[2].y/pts[2].w));
    
    // TODO: Add frustum clipping and remove these tests below.
    // If the model is too big, or reaches out of screen then
    // segfault occurs if not checked since these ranges are used to index in pixel and z-buffer.

    if (minX < 0 || maxX < 0 || minY < 0 || maxY < 0) return;
    if (minX > 800 || maxX > 800 || minY > 800 || maxY > 800) return;
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
            float zc = pts[0][2] * barc.x + pts[1][2] * barc.y + pts[2][2] * barc.z;        // interpolate z axis
            
            Vector2f uv(0., 0.);
            
            // interpolating uv coords to get the texture coordinates for this pixel
            for (int i = 0; i < 3; i++) {
                uv.x += uv_coords[i][0] * barc[i];
                uv.y += uv_coords[i][1] * barc[i];
            }
            
            sf::Color color = tex->getColor(uv.x * tex->width, uv.y * tex->height);

            if (shader.fragment(barc, color)) continue;
            
            // check and update z-zbuffer
            if (!(zbuffer[pixel.x + pixel.y*DisplayBackend::WINDOW_WIDTH] < zc)) {
                zbuffer[pixel.x + pixel.y*DisplayBackend::WINDOW_WIDTH] = zc;
                setPixel(pixel.x, pixel.y, color, pixelBuffer);
            }
        }
    }
}
