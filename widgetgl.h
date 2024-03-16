#ifndef WIDGETGL_H
#define WIDGETGL_H

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>
#include <QVector>
#include <QWheelEvent>

class Engine3D;
class Transformational;
class Camera;

class WidgetGL : public QOpenGLWidget
{
public:
    WidgetGL(QWidget *parent = nullptr);
    ~WidgetGL();

protected:
    // QOpenGLWidget interface
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // Mouse event
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void initShaders();
    void initCube(float width);

private:
    // Model view projection matrix
    QMatrix4x4 m_projectionMatrix;

    // Shader program
    QOpenGLShaderProgram m_shaderProgram;

    QVector2D m_mousePosition;

    QVector<Engine3D *> m_objects;
    QVector<Transformational *> m_transformObjects;

    Camera *m_camera;
};

#endif // WIDGETGL_H
