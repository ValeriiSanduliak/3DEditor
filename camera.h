#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector>
#include "transformational.h"
class Camera : public Transformational
{
public:
    Camera();

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions = 0);
    void rotate(const QQuaternion &r);
    void rotateX(const QQuaternion &r);
    void rotateY(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);

    const QMatrix4x4 &getViewMatrix() const;

    void updateViewMatrix();

private:
    QQuaternion m_rotate;
    QVector3D m_translate;

    float m_scale;
    QMatrix4x4 m_globalTransform;

    QMatrix4x4 m_viewMatrix;

    bool m_moveUp; // Прапорець, що вказує, чи відбувається рух вгору
    bool m_moveDown; // Прапорець, що вказує, чи відбувається рух вниз
};

#endif // CAMERA_H
