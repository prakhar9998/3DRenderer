#include "rasterizer.h"
#include "math.h"
#include "camera.h"
#include "matrix.h"
#include "display.h"
#include "transform.h"

#include <omp.h>

Rasterizer::Rasterizer() {}

Rasterizer::~Rasterizer() {}

Matrix4f Rasterizer::Viewport = Transform::viewportMatrix(
    DisplayBackend::WINDOW_WIDTH/8,
    DisplayBackend::WINDOW_HEIGHT/8,
    DisplayBackend::WINDOW_WIDTH*3/4,
    DisplayBackend::WINDOW_HEIGHT*3/4
);

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

    v[0].x = (pts[0].x+1.) * DisplayBackend::WINDOW_WIDTH/2.f;
    v[0].y = (pts[0].y+1.) * DisplayBackend::WINDOW_HEIGHT/2.f;
    v[1].x = (pts[1].x+1.) * DisplayBackend::WINDOW_WIDTH/2.f;
    v[1].y = (pts[1].y+1.) * DisplayBackend::WINDOW_HEIGHT/2.f;
    v[2].x = (pts[2].x+1.) * DisplayBackend::WINDOW_WIDTH/2.f;
    v[2].y = (pts[2].y+1.) * DisplayBackend::WINDOW_HEIGHT/2.f;

    drawLine(v[0], v[1], sf::Color::White, pixelBuffer);
    drawLine(v[0], v[2], sf::Color::White, pixelBuffer);
    drawLine(v[1], v[2], sf::Color::White, pixelBuffer);
}

float Rasterizer::edgeFunction(const Vector4f& a, const Vector4f& b, const Vector2i& p) {
    // returns whether the point p is on the right side of the edge formed by vertices a and b
    return (a.y - b.y) * (p.x - a.x) - (a.x - b.x) * (p.y - a.y);
}

float Rasterizer::edgeFunction(const Vector4f& a, const Vector4f& b, const Vector4f& c) {
    return (a.y - b.y) * (c.x - a.x) - (a.x - b.x) * (c.y - a.y);
}

bool Rasterizer::isInside(const Vector4f& v0, const Vector4f& v1, const Vector4f& v2, const Vector2i& P) {
    // checks if the point P is inside the triangle formed by vertices v0, v1 and v2;

    return (edgeFunction(v0, v1, P) && edgeFunction(v1, v2, P) && edgeFunction(v2, v0, P));
}

Vector3f Rasterizer::barycentric(const Vector4f& v0, const Vector4f& v1, const Vector4f& v2, const Vector2i& P) {
    // compute barycentric coordinates of the triangle represented by the given vertices

    float denom = ((v1.x-v0.x) * (v2.y-v0.y) - (v2.x-v0.x) * (v1.y-v0.y));
    if (denom < std::abs(1e-3)) return Vector3f(-1.f, 0.f, 0.f);         // degenerate triangle
    float invDenom = 1 / ((v1.x-v0.x) * (v2.y-v0.y) - (v2.x-v0.x) * (v1.y-v0.y));
    float u = ((P.x-v0.x) * (v2.y-v0.y) - (v2.x-v0.x) * (P.y-v0.y)) * invDenom;
    float v = ((v1.x-v0.x) * (P.y-v0.y) - (P.x-v0.x) * (v1.y-v0.y)) * invDenom;

    return Vector3f(1.f - u - v, u, v);
}

void Rasterizer::drawTriangle(Vector4f *pts, IShader &shader, sf::Uint8* pixelBuffer, float* zbuffer) {

    Vector3f zc = Vector3f(pts[0].z, pts[1].z, pts[2].z);
    Vector3f pc = Vector3f(1./pts[0].w, 1./pts[1].w, 1./pts[2].w);

    // pts[0] = Viewport * pts[0];
    // pts[1] = Viewport * pts[1];
    // pts[2] = Viewport * pts[2];

    pts[0].x = (pts[0].x+1.) * DisplayBackend::WINDOW_WIDTH/2.f;
    pts[0].y = (pts[0].y+1.) * DisplayBackend::WINDOW_HEIGHT/2.f;
    pts[1].x = (pts[1].x+1.) * DisplayBackend::WINDOW_WIDTH/2.f;
    pts[1].y = (pts[1].y+1.) * DisplayBackend::WINDOW_HEIGHT/2.f;
    pts[2].x = (pts[2].x+1.) * DisplayBackend::WINDOW_WIDTH/2.f;
    pts[2].y = (pts[2].y+1.) * DisplayBackend::WINDOW_HEIGHT/2.f;

    // calculate bounding box of the three coordinates.
    float minX = std::min(pts[0].x, std::min(pts[1].x, pts[2].x));
    float maxX = std::max(pts[0].x, std::max(pts[1].x, pts[2].x));
    float minY = std::min(pts[0].y, std::min(pts[1].y, pts[2].y));
    float maxY = std::max(pts[0].y, std::max(pts[1].y, pts[2].y));
    
    // TODO: Add frustum clipping and remove these tests below.
    // If the model is too big, or reaches out of screen then
    // segfault occurs if not checked since these ranges are used to index in pixel and z-buffer.

    if (minX < 0 || maxX < 0 || minY < 0 || maxY < 0) return;
    if (minX > 800 || maxX > 800 || minY > 800 || maxY > 800) return;
    Vector2i pixel;
    pixel.x = minX;
    pixel.y = minY;

    // values for linearly interpolating the edge function, requires less calculation
    // makes use of the fact that edgefunction is linear, so we can avoid calculating it at every step
    float w0_row = edgeFunction(pts[1], pts[2], pixel);
    float dx0 = pts[1].x - pts[2].x;
    float dy0 = pts[1].y - pts[2].y;
    float w1_row = edgeFunction(pts[2], pts[0], pixel);
    float dx1 = pts[2].x - pts[0].x;
    float dy1 = pts[2].y - pts[0].y;
    float w2_row = edgeFunction(pts[0], pts[1], pixel);
    float dx2 = pts[0].x - pts[1].x;
    float dy2 = pts[0].y - pts[1].y;
    float area = edgeFunction(pts[0], pts[1], pts[2]);
    
    float w0, w1, w2;

    for (pixel.y = minY; pixel.y < maxY; pixel.y++) {       // implicit conversion to int
        w0 = w0_row;
        w1 = w1_row;
        w2 = w2_row;
        for (pixel.x = minX; pixel.x < maxX; pixel.x++) {
            
            // if point p is inside the triangle then proceeed with calculating barycentric coordinates
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                Vector3f barc{w0/area, w1/area, w2/area};
                // Vector3f barc = barycentric(pts[0], pts[1], pts[2], pixel);

                if (barc.x < 0 || barc.y < 0 || barc.z < 0) continue;       // the point is not inside the triangle
                float z_dist = dot(zc, barc);        // interpolate z axis
                float oneOverpersc = 1/dot(pc, barc);       // interpolating inverse w coordinate
                
                // calculate perspective correct barycentric coordinates
                barc[0] = barc[0] * pc[0] * oneOverpersc;
                barc[1] = barc[1] * pc[1] * oneOverpersc;
                barc[2] = barc[2] * pc[2] * oneOverpersc;

                Vector3f rgb = shader.fragment(barc);
                sf::Color color = sf::Color(rgb.x, rgb.y, rgb.z, 0xff);
                
                // check and update z-zbuffer
                if (z_dist < zbuffer[pixel.x + pixel.y*DisplayBackend::WINDOW_WIDTH]) {
                    zbuffer[pixel.x + pixel.y*DisplayBackend::WINDOW_WIDTH] = z_dist;
                    setPixel(pixel.x, pixel.y, color, pixelBuffer);
                }
            }
            w0 += dy0;
            w1 += dy1;
            w2 += dy2;
        }

        w0_row -= dx0;
        w1_row -= dx1;
        w2_row -= dx2;
    }
}
