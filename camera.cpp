#include "camera.h"
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

Camera::Camera()
{
    m_scale = 1.0f;
    m_globalTransform.setToIdentity();
}

void Camera::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if (functions != 0)
        return;

    program->setUniformValue("u_viewMatrix", m_viewMatrix);
}

void Camera::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;

    updateViewMatrix();
}

void Camera::rotateX(const QQuaternion &r)
{
    m_rotateX = r * m_rotateX;
    m_rotate = m_rotateX * m_rotateY;
    updateViewMatrix();
}

void Camera::rotateY(const QQuaternion &r)
{
    m_rotateY = r * m_rotateY;
    m_rotate = m_rotateX * m_rotateY;
    updateViewMatrix();
}

void Camera::translate(const QVector3D &t)
{
    m_translate += t;

    updateViewMatrix();
}

void Camera::scale(const float &s)
{
    m_scale *= s;

    updateViewMatrix();
}

void Camera::setGlobalTransform(const QMatrix4x4 &g)
{
    m_globalTransform = g;

    updateViewMatrix();
}

const QMatrix4x4 &Camera::getViewMatrix() const
{
    return m_viewMatrix;
}

void Camera::updateViewMatrix()
{
    m_viewMatrix.setToIdentity();
    m_viewMatrix.translate(m_translate);
    m_viewMatrix.rotate(m_rotate);
    m_viewMatrix.scale(m_scale);
    m_viewMatrix = m_viewMatrix * m_globalTransform.inverted();
}
