#include "object3d.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include "material.h"

Object3D::Object3D()
    : m_indexBuffer(QOpenGLBuffer::IndexBuffer)
    , m_diffuseMap(0)
{
    m_scale = 1.0f;
}

Object3D::Object3D(const QVector<VertexData> &vertData,
                   const QVector<GLuint> &indexes,
                   Material *material)
    : m_indexBuffer(QOpenGLBuffer::IndexBuffer)
    , m_diffuseMap(0)
{
    m_scale = 1.0f;
    init(vertData, indexes, material);
}

Object3D::~Object3D()
{
    if (m_vertexBuffer.isCreated()) {
        m_vertexBuffer.destroy();
    }
    if (m_indexBuffer.isCreated()) {
        m_indexBuffer.destroy();
    }
    if (m_diffuseMap != 0) {
        if (m_diffuseMap->isCreated()) {
            m_diffuseMap->destroy();
        }
    }
}

void Object3D::init(const QVector<VertexData> &vertData,
                    const QVector<GLuint> &indexes,
                    Material *material)
{
    if (m_vertexBuffer.isCreated()) {
        m_vertexBuffer.destroy();
    }
    if (m_indexBuffer.isCreated()) {
        m_indexBuffer.destroy();
    }
    if (m_diffuseMap != 0) {
        if (m_diffuseMap->isCreated()) {
            delete m_diffuseMap;
            m_diffuseMap = 0;
        }
    }

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertData.constData(), vertData.size() * sizeof(VertexData));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_indexBuffer.release();

    m_material = material;

    if (m_material->isUsingDiffuseMap()) {
        m_diffuseMap = new QOpenGLTexture(m_material->diffuseMap().mirrored());

        m_diffuseMap->setMinificationFilter(QOpenGLTexture::Nearest);

        m_diffuseMap->setMinificationFilter(QOpenGLTexture::Linear);

        m_diffuseMap->setWrapMode(QOpenGLTexture::Repeat);
    }
}

void Object3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if (!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated())
        return;

    if (m_material->isUsingDiffuseMap()) {
        m_diffuseMap->bind(0);
        program->setUniformValue("u_diffuseMap", 0);
    }

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(m_translate);
    modelMatrix.rotate(m_rotate);
    modelMatrix.scale(m_scale);
    modelMatrix = m_globalTransform * modelMatrix;

    program->setUniformValue("u_modelMatrix", modelMatrix);
    program->setUniformValue("materialProperty.diffuseColor", m_material->diffuseColor());
    program->setUniformValue("u_lightPosition", QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    program->setUniformValue("u_lightPower", 5.0f);
    program->setUniformValue("u_isUsingDiffuseMap", m_material->isUsingDiffuseMap());

    m_vertexBuffer.bind();

    int offset = 0;

    int vertLoc = program->attributeLocation("a_position");
    program->enableAttributeArray(vertLoc);
    program->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int textLoc = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(textLoc);
    program->setAttributeBuffer(textLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    int normtLoc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normtLoc);
    program->setAttributeBuffer(normtLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int tangentLoc = program->attributeLocation("a_tangent");
    program->enableAttributeArray(tangentLoc);
    program->setAttributeBuffer(tangentLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int bitangentLoc = program->attributeLocation("a_bitangent");
    program->enableAttributeArray(bitangentLoc);
    program->setAttributeBuffer(bitangentLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    m_indexBuffer.bind();

    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

    m_vertexBuffer.release();
    m_indexBuffer.release();

    if (m_material->isUsingDiffuseMap())
        m_diffuseMap->release();
}

void Object3D::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;
}

void Object3D::translate(const QVector3D &t)
{
    m_translate += t;
}

void Object3D::scale(const float &s)
{
    m_scale *= s;
}

void Object3D::setGlobalTransform(const QMatrix4x4 &g)
{
    m_globalTransform = g;
}

void Object3D::setTexture(QOpenGLTexture *newTexture)
{
    m_diffuseMap = newTexture;
}
