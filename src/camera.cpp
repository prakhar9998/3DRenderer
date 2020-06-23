#include "camera.h"
#include "display.h"
#include "transform.h"

#include <math.h>

Camera::Camera()
: m_Position(Vector3f(0., 0., -1.))
, m_Up(Vector3f(0., 1., 0.))
, m_Target(Vector3f(0., 0., 0.))
, m_FOV(60.)
, m_Near(.1f)
, m_Far(10.f) {}

Camera::Camera(Vector3f position, Vector3f target)
: m_Position(position)
, m_Up(Vector3f(0., 1., 0.))
, m_Target(target)
, m_FOV(60.)
, m_Near(.1f)
, m_Far(10.f) {}

Camera::~Camera() {}

// SETTER METHODS

void Camera::setPosition(Vector3f position) { 
    m_Position = position; 
}

void Camera::setTarget(Vector3f target) { m_Target = target; }

void Camera::setFOV(float fov) { m_FOV = fov; }

void Camera::setNear(float near) { m_Near = near; }

void Camera::setFar(float far) { m_Far = far; }

// build the following projection matrix for the camera
/**
 * returns the following matrix
 * 
 * 1/AR*tan(fov/2)  0               0               0
 * 0                1/tan(fov/2)    0               0
 * 0                0               (n+f)/(n-f)     (-2*f*n)/(n-f)
 * 0                0               -1              0
 * 
 * */

Matrix4f Camera::getProjectionMatrix() {
    Matrix4f P;
    P = identity(P);
    float aspectRatio = DisplayBackend::WINDOW_WIDTH/DisplayBackend::WINDOW_HEIGHT;
    float cotangent = 1 / tan((m_FOV / 2) * (M_PI / 180));     // angle in radians
    P[0][0] = cotangent / aspectRatio;
    P[1][1] = cotangent;
    // P[2][2] = (m_Near + m_Far) / (m_Near - m_Far);
    // P[2][3] = (-2 * m_Far * m_Near) / (m_Near - m_Far);
    P[2][2] = -m_Far / (m_Far - m_Near);
    P[2][3] = -m_Far * m_Near / (m_Far - m_Near);
    P[3][2] = -1;
    P[3][3] = 0;
    return P;
}

Matrix4f Camera::getViewMatrix() { return Transform::lookAt(m_Position, m_Target, m_Up); }

Vector3f Camera::getCameraDirection() {
    return m_Position - m_Target;
}