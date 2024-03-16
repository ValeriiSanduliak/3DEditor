#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include "transformational.h"

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
class Material;

struct VertexData
{
    VertexData() {}
    VertexData(QVector3D p, QVector2D t, QVector3D n)
        : position(p)
        , texCoord(t)
        , normal(n)
    {}
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
    QVector3D tangent;
    QVector3D bitangent;
};
class Object3D : public Transformational
{
public:
    Object3D();
    Object3D(const QVector<VertexData> &vertData,
             const QVector<GLuint> &indexes,
             Material *material);
    ~Object3D();
    void init(const QVector<VertexData> &vertData,
              const QVector<GLuint> &indexes,
              Material *material);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);

    void setTexture(QOpenGLTexture *newTexture);

private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QOpenGLTexture *m_diffuseMap;

    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;

    Material *m_material;
};

#endif // OBJECT3D_H
