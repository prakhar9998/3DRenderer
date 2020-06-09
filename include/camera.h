#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"

class Camera {
public:
    static Matrix4f lookAt(const Vector3f& eye, const Vector3f& at, const Vector3f& up);
    static Matrix4f Viewport(int x, int y, int w, int h);
    static Matrix4f Projection(float coeff);
};

Matrix4f Camera::lookAt(const Vector3f& eye, const Vector3f& at, const Vector3f& up) {
    Vector3f n = normalize(eye - at);
    Vector3f u = normalize(cross(up, n));
    Vector3f v = normalize(cross(n, u));

    Matrix4f c, T;
    c = identity(c);
    T = identity(T);
    for (int i = 0; i < 3; i++) {
        c[0][i] = u[i];
        c[1][i] = v[i];
        c[2][i] = n[i];
        T[i][3] = -eye[i];
    }
    return c * T;
}

Matrix4f Camera::Viewport(int x, int y, int w, int h) {
    Matrix4f vp;
    vp = identity(vp);
    vp[0][3] = x+w/2.f;
    vp[1][3] = y+h/2.f;
    vp[2][3] = 1.f;
    vp[0][0] = w/2.f;
    vp[1][1] = h/2.f;
    vp[2][2] = 0;
    return vp;
}

Matrix4f Camera::Projection(float coeff) {
    Matrix4f p;
    p = identity(p);
    p[3][2] = coeff;
    return p;
}

#endif      // CAMERA_H