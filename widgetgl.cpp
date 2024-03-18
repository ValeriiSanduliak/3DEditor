#include "widgetgl.h"
#include <QFile>
#include <QImage>
#include <QKeyEvent>
#include <QOpenGLContext>
#include <QString>
#include <QtMath>
#include "camera.h"
#include "material.h"
#include <engine3d.h>

WidgetGL::WidgetGL(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus); // Встановлення політики фокусу
    m_camera = new Camera;
    m_camera->translate(QVector3D(0.0f, 0.0f, -5.0f));
}

WidgetGL::~WidgetGL()
{
    delete m_camera;
    for (int i = 0; i < m_objects.size(); ++i) {
        delete m_objects[i];
    }
}

void WidgetGL::initializeGL()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    initShaders();
    initCube(1.0f, 1.0f, 1.0f);
    m_objects[m_objects.size() - 1]->translate(QVector3D(0.0f, 2.0f, 0.0f));
    m_transformObjects.append(m_objects[m_objects.size() - 1]);
    connect(&m_timerMoveUp, &QTimer::timeout, this, &WidgetGL::moveCameraUp);
    connect(&m_timerMoveDown, &QTimer::timeout, this, &WidgetGL::moveCameraDown);
    connect(&m_timerMoveLeft, &QTimer::timeout, this, &WidgetGL::moveCameraLeft);
    connect(&m_timerMoveRight, &QTimer::timeout, this, &WidgetGL::moveCameraRight);
    connect(&m_timerMoveForward, &QTimer::timeout, this, &WidgetGL::moveCameraForward);
    connect(&m_timerMoveBackward, &QTimer::timeout, this, &WidgetGL::moveCameraBackward);

    m_objects.append(new Engine3D);
    m_objects[1]->translate(QVector3D(8.0f, 0.0f, 0.0f));

    m_objects[m_objects.size() - 1]->loadObjectFromFile(":/Monkey2side.obj");
    m_objects[m_objects.size() - 1]->translate(QVector3D(0.0f, 5.0f, 0.0f));
    m_transformObjects.append(m_objects[m_objects.size() - 1]);

    initCube(1.0f, 1.0f, 1.0f);
    m_transformObjects.append(m_objects[m_objects.size() - 1]);

    initCube(40.0f, 2.0f, 40.0f);
    m_objects[m_objects.size() - 1]->translate(QVector3D(0.0f, -2.0f, 0.0f));
    m_transformObjects.append(m_objects[m_objects.size() - 1]);
}

void WidgetGL::resizeGL(int w, int h)
{
    // Set the viewport to window dimensions
    float aspect = w / (float) h;

    // Set the projection matrix
    m_projectionMatrix.setToIdentity();

    // Set the perspective projection
    m_projectionMatrix.perspective(45.0f, aspect, 0.1f, 100.0f);
}

void WidgetGL::paintGL()
{
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue("u_projectionMatrix", m_projectionMatrix);

    m_camera->draw(&m_shaderProgram);

    for (int i = 0; i < m_transformObjects.size(); ++i) {
        m_transformObjects[i]->draw(&m_shaderProgram, context()->functions());
    }

    m_shaderProgram.release();
}

void WidgetGL::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        m_mousePosition = QVector2D(event->pos());
    } else if (event->buttons() == Qt::RightButton) {
        QVector3D t = screenCoordsToWorldCoords(QVector2D(event->pos()));
        initCube(1.0f, 1.0f, 1.0f);
        m_objects[m_objects.size() - 1]->translate(t);
        m_transformObjects.append(m_objects[m_objects.size() - 1]);
        update();
    }
    event->accept();
}

void WidgetGL::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
        return;

    QVector2D diff = QVector2D(event->pos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->pos());

    // float angle = diff.length() / 2.0;
    // QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);
    // m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));

    float angleX = diff.y() / 2.0f;
    float angleY = diff.x() / 2.0f;
    m_camera->rotateX(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, angleX));
    m_camera->rotateY(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, angleY));
    update();
}

// void WidgetGL::wheelEvent(QWheelEvent *event)
// {
//     if (event->angleDelta().y() > 0) {
//         m_camera->translate(QVector3D(0.0f, 0.0f, +0.25f));
//     } else if (event->angleDelta().y() < 0) {
//         m_camera->translate(QVector3D(0.0f, 0.0f, -0.25f));
//     }

//     update();
// }

void WidgetGL::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Space:
        m_timerMoveUp.start(timerSpeed);
        break;
    case Qt::Key_Shift:
        m_timerMoveDown.start(timerSpeed);
        break;
    case Qt::Key_A:
        m_timerMoveLeft.start(timerSpeed);
        break;
    case Qt::Key_D:
        m_timerMoveRight.start(timerSpeed);
        break;
    case Qt::Key_W:
        m_timerMoveForward.start(timerSpeed);
        break;
    case Qt::Key_S:
        m_timerMoveBackward.start(timerSpeed);
        break;
    }
}

void WidgetGL::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Space:
        m_timerMoveUp.stop();
        break;
    case Qt::Key_Shift:
        m_timerMoveDown.stop();
        break;
    case Qt::Key_A:
        m_timerMoveLeft.stop();
        break;
    case Qt::Key_D:
        m_timerMoveRight.stop();
        break;
    case Qt::Key_W:
        m_timerMoveForward.stop();
        break;
    case Qt::Key_S:
        m_timerMoveBackward.stop();
        break;
    }
}

void WidgetGL::moveCameraUp()
{
    m_camera->translate(QVector3D(0.0f, -cameraSpeed, 0.0f));
    update();
}

void WidgetGL::moveCameraDown()
{
    m_camera->translate(QVector3D(0.0f, cameraSpeed, 0.0f));
    update();
}

void WidgetGL::moveCameraLeft()
{
    m_camera->translate(QVector3D(cameraSpeed, 0.0f, 0.0f));
    update();
}

void WidgetGL::moveCameraRight()
{
    m_camera->translate(QVector3D(-cameraSpeed, 0.0f, 0.0f));
    update();
}

void WidgetGL::moveCameraForward()
{
    m_camera->translate(QVector3D(0.0f, 0.0f, cameraSpeed));
    update();
}

void WidgetGL::moveCameraBackward()
{
    m_camera->translate(QVector3D(0.0f, 0.0f, -cameraSpeed));
    update();
}

QVector3D WidgetGL::screenCoordsToWorldCoords(const QVector2D &mousePosition)
{
    QVector4D tmp(2.0f * mousePosition.x() / width() - 1.0f,
                  -2.0f * mousePosition.y() / height() + 1.0f,
                  -1.0f,
                  1.0f);
    QVector4D tmpWorld((m_projectionMatrix.inverted() * tmp).toVector2D(), -1.0f, 0.0f);
    QVector3D Direction((m_camera->getViewMatrix().inverted() * tmpWorld).toVector3D().normalized());
    QVector3D camPosition(
        (m_camera->getViewMatrix().inverted() * QVector4D(0.0f, 0.0f, 0.0f, 1.0f)).toVector3D());

    QVector3D N(0.0f, 1.0f, 0.0f);
    float t = -QVector3D::dotProduct(camPosition, N) / QVector3D::dotProduct(Direction, N);
    QVector3D result = camPosition + Direction * t;

    return result;
}

void WidgetGL::initShaders()
{
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh"))
        close();

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh"))
        close();

    if (!m_shaderProgram.link())
        close();
}

void WidgetGL::initCube(float width, float heigth, float depth)
{
    float width_div_2 = width / 2.0f;
    float heigth_div_2 = heigth / 2.0f;
    float depth_div_2 = depth / 2.0f;
    QVector<VertexData> vertexes;
    //
    vertexes.append(VertexData(QVector3D(-width_div_2, heigth_div_2, depth_div_2),
                               QVector2D(0.0, 1.0),
                               QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -heigth_div_2, depth_div_2),
                               QVector2D(0.0, 0.0),
                               QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, heigth_div_2, depth_div_2),
                               QVector2D(1.0, 1.0),
                               QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -heigth_div_2, depth_div_2),
                               QVector2D(1.0, 0.0),
                               QVector3D(0.0, 0.0, 1.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, heigth_div_2, depth_div_2),
                               QVector2D(0.0, 1.0),
                               QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -heigth_div_2, depth_div_2),
                               QVector2D(0.0, 0.0),
                               QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, heigth_div_2, -depth_div_2),
                               QVector2D(1.0, 1.0),
                               QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -heigth_div_2, -depth_div_2),
                               QVector2D(1.0, 0.0),
                               QVector3D(1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, heigth_div_2, depth_div_2),
                               QVector2D(0.0, 1.0),
                               QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, heigth_div_2, -depth_div_2),
                               QVector2D(0.0, 0.0),
                               QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, heigth_div_2, depth_div_2),
                               QVector2D(1.0, 1.0),
                               QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, heigth_div_2, -depth_div_2),
                               QVector2D(1.0, 0.0),
                               QVector3D(0.0, 1.0, 0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, heigth_div_2, -depth_div_2),
                               QVector2D(0.0, 1.0),
                               QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -heigth_div_2, -depth_div_2),
                               QVector2D(0.0, 0.0),
                               QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, heigth_div_2, -depth_div_2),
                               QVector2D(1.0, 1.0),
                               QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -heigth_div_2, -depth_div_2),
                               QVector2D(1.0, 0.0),
                               QVector3D(0.0, 0.0, -1.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2, heigth_div_2, depth_div_2),
                               QVector2D(0.0, 1.0),
                               QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, heigth_div_2, -depth_div_2),
                               QVector2D(0.0, 0.0),
                               QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -heigth_div_2, depth_div_2),
                               QVector2D(1.0, 1.0),
                               QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -heigth_div_2, -depth_div_2),
                               QVector2D(1.0, 0.0),
                               QVector3D(-1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2, -heigth_div_2, depth_div_2),
                               QVector2D(0.0, 1.0),
                               QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -heigth_div_2, -depth_div_2),
                               QVector2D(0.0, 0.0),
                               QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -heigth_div_2, depth_div_2),
                               QVector2D(1.0, 1.0),
                               QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -heigth_div_2, -depth_div_2),
                               QVector2D(1.0, 0.0),
                               QVector3D(0.0, -1.0, 0.0)));

    QVector<GLuint> indexes;
    for (int i = 0; i < 24; i += 4) {
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    Material *newMtl = new Material;
    newMtl->setDiffuseMap(":/cube.png");
    newMtl->setDiffuseColor(QVector3D(1.0, 1.0, 1.0));

    Engine3D *newObject = new Engine3D;
    newObject->addObject(new Object3D(vertexes, indexes, newMtl));
    m_objects.append(newObject);
}
