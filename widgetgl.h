#ifndef WIDGETGL_H
#define WIDGETGL_H

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QTimer>
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

    // Key event
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void initShaders();
    void initCube(float width);

    // Camera movement
    void moveCameraUp();

    void moveCameraDown();

    void moveCameraLeft();

    void moveCameraRight();

    void moveCameraForward();

    void moveCameraBackward();

private:
    // Model view projection matrix
    QMatrix4x4 m_projectionMatrix;

    // Shader program
    QOpenGLShaderProgram m_shaderProgram;

    QVector2D m_mousePosition;

    QVector<Engine3D *> m_objects;
    QVector<Transformational *> m_transformObjects;

    Camera *m_camera;
    QTimer m_timerMoveUp;
    QTimer m_timerMoveDown;
    QTimer m_timerMoveLeft;
    QTimer m_timerMoveRight;
    QTimer m_timerMoveForward;
    QTimer m_timerMoveBackward;
    float cameraSpeed = 0.1f;
    uint timerSpeed = 14;
};

#endif // WIDGETGL_H
