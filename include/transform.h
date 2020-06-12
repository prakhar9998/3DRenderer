#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"

namespace Transform {
    Matrix4f translate(Vector3f translateVector);
    Matrix4f scale(Vector3f scalingFactors);
    Matrix4f rotate(Vector3f angles);
    Matrix4f lookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up);
    Matrix4f viewportMatrix(int x, int y, int w, int h);
}

#endif      // TRANSFORM_H