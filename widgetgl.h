#ifndef WIDGETGL_H
#define WIDGETGL_H

#include <QCheckBox>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QString>
#include <QTimer>
#include <QVector2D>
#include <QVector3D>
#include <QVector>
#include <QWheelEvent>
#include <mainwindow.h>
class Engine3D;
class Transformational;
class Camera;

class WidgetGL : public QOpenGLWidget
{
public:
    WidgetGL(QWidget *parent = nullptr);
    ~WidgetGL();

    void setCheckBox(QCheckBox *checkBox);
    void loadObjectFromFile(const QString &filename);
    void setTexture(const QString &filename);
    void setTexture(const QImage &image);

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
    void initCube(float width, float heigth, float depth);

    // Camera movement
    void moveCameraUp();

    void moveCameraDown();

    void moveCameraLeft();

    void moveCameraRight();

    void moveCameraForward();

    void moveCameraBackward();

    QVector3D screenCoordsToWorldCoords(const QVector2D &mousePosition);

    int selectObject(int xx, int yy, QVector<Engine3D *> &objs);

    void wheelEvent(QWheelEvent *event) override;

private:
    // Model view projection matrix
    QMatrix4x4 m_projectionMatrix;

    // Shader program
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLShaderProgram m_selectProgram;

    QOpenGLFramebufferObject *m_selectBuffer;

    QVector2D m_mousePosition;

    QVector<Engine3D *> m_objects;
    QVector<Transformational *> m_transformObjects;
    QVector<Transformational *> m_selectedObjects;

    Camera *m_camera;
    QTimer m_timerMoveUp;
    QTimer m_timerMoveDown;
    QTimer m_timerMoveLeft;
    QTimer m_timerMoveRight;
    QTimer m_timerMoveForward;
    QTimer m_timerMoveBackward;
    QTimer m_timerMoveObject;
    float cameraSpeed = 0.1f;
    uint timerSpeed = 14;

    int m_selectedObjectIndex = -1;

    QVector<QCheckBox *> m_checkbox; // Посилання на checkbox
};

#endif // WIDGETGL_H
