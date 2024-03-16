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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    initShaders();
    initCube(1.0f);

    m_objects.append(new Engine3D);
    m_objects[1]->translate(QVector3D(8.0f, 0.0f, 0.0f));

    m_objects[m_objects.size() - 1]->loadObjectFromFile(":/Monkey2side.obj");

    for (int i = 0; i < m_objects.size(); ++i) {
        m_objects[i]->translate(QVector3D(0.0f, 2 * i, 0.0f));
        m_transformObjects.append(m_objects[i]);
    }
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
    }
    event->accept();
}

void WidgetGL::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
        return;

    QVector2D diff = QVector2D(event->pos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->pos());

    float angle = diff.length() / 2.0;

    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);

    m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));
    update();
}

void WidgetGL::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0) {
        m_camera->translate(QVector3D(0.0f, 0.0f, +0.25f));
    } else if (event->angleDelta().y() < 0) {
        m_camera->translate(QVector3D(0.0f, 0.0f, -0.25f));
    }

    update();
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

void WidgetGL::initCube(float width)
{
    float width_div_2 = width / 2.0f;
    QVector<VertexData> vertexes;
    //
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2),
                               QVector2D(0.0, 1.0),
                               QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),
                               QVector2D(0.0, 0.0),
                               QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2),
                               QVector2D(1.0, 1.0),
                               QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2),
                               QVector2D(1.0, 0.0),
                               QVector3D(0.0, 0.0, 1.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2),
                               QVector2D(0.0, 1.0),
                               QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2),
                               QVector2D(0.0, 0.0),
                               QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                               QVector2D(1.0, 1.0),
                               QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
                               QVector2D(1.0, 0.0),
                               QVector3D(1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2),
                               QVector2D(0.0, 1.0),
                               QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                               QVector2D(0.0, 0.0),
                               QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2),
                               QVector2D(1.0, 1.0),
                               QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                               QVector2D(1.0, 0.0),
                               QVector3D(0.0, 1.0, 0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                               QVector2D(0.0, 1.0),
                               QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
                               QVector2D(0.0, 0.0),
                               QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                               QVector2D(1.0, 1.0),
                               QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                               QVector2D(1.0, 0.0),
                               QVector3D(0.0, 0.0, -1.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2),
                               QVector2D(0.0, 1.0),
                               QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                               QVector2D(0.0, 0.0),
                               QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),
                               QVector2D(1.0, 1.0),
                               QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                               QVector2D(1.0, 0.0),
                               QVector3D(-1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2),
                               QVector2D(0.0, 1.0),
                               QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                               QVector2D(0.0, 0.0),
                               QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2),
                               QVector2D(1.0, 1.0),
                               QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
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
