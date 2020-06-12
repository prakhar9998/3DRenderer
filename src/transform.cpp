#include "transform.h"

Matrix4f Transform::translate(Vector3f translateVector) {
    // translate to a position pointed to by the given vector.
    Matrix4f T;
    T = identity(T);

    T[0][3] = -translateVector[0];
    T[1][3] = -translateVector[1];
    T[2][3] = -translateVector[2];

    return T;
}

Matrix4f Transform::scale(Vector3f factors) {
    // scale across x, y and z axis according to the factors given.
    Matrix4f S;
    S = identity(S);

    S[0][0] = factors.x;
    S[1][1] = factors.y;
    S[2][2] = factors.z;
    
    return S;
}

Matrix4f Transform::rotate(Vector3f angles) {
    // the given argument gives yaw, pitch and roll respectively
    // using euler transform, the generated matrix is returned

    // x-axis rotation
    float sinAlpha = sin(angles.x);
    float cosAlpha = cos(angles.x);

    // y-axis rotation
    float sinBeta = sin(angles.y);
    float cosBeta = cos(angles.x);

    // z-axis rotation
    float sinGamma = sin(angles.z);
    float cosGamma = cos(angles.z);

    Matrix4f R;
    R = identity(R);
    
    R[0][0] = cosAlpha * cosBeta;
    R[0][1] = cosAlpha * sinBeta * sinGamma - sinAlpha * cosGamma;
    R[0][1] = cosAlpha * sinBeta * cosGamma + sinAlpha * sinGamma;

    R[1][0] = sinAlpha * cosBeta;
    R[1][1] = sinAlpha * sinBeta * sinGamma + cosAlpha * cosGamma;
    R[1][2] = sinAlpha * sinBeta * cosGamma - cosAlpha * sinGamma;

    R[2][0] = -sinBeta;
    R[2][1] = cosBeta * sinGamma;
    R[2][2] = cosBeta * cosGamma;

    return R;
}

Matrix4f Transform::lookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up) {
    Vector3f n = normalize(center - eye);
    Vector3f u = normalize(cross(n, up));
    Vector3f v = normalize(cross(u, n));

    Matrix4f c, T;
    c = identity(c);
    T = identity(T);
    for (int i = 0; i < 3; i++) {
        c[0][i] = u[i];
        c[1][i] = v[i];
        c[2][i] = -n[i];
        T[i][3] = -eye[i];
    }
    return c * T;
}

Matrix4f Transform::viewportMatrix(int x, int y, int w, int h) {
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