#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"

class Camera {
private:
    Vector3f m_Position;
    Vector3f m_Up;
    Vector3f m_Target;

    float m_FOV;
    float m_Near;
    float m_Far;
public:
    Camera();
    Camera(Vector3f position, Vector3f target);
    ~Camera();
    void setPosition(Vector3f position);
    void setTarget(Vector3f target);
    void setFOV(float fov);
    void setNear(float near);
    void setFar(float far);
    Matrix4f getProjectionMatrix();
    Matrix4f getViewMatrix();
};

#endif      // CAMERA_H