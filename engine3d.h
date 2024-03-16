#ifndef ENGINE3D_H
#define ENGINE3D_H

#include <QVector>
#include "materiallibrary.h"
#include "object3d.h"

class Engine3D : public Transformational
{
public:
    Engine3D();
    void loadObjectFromFile(const QString &filename);
    void addObject(Object3D *object);
    Object3D *getObject(quint32 index);
    void calculateTBN(QVector<VertexData> &vertData);

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);

private:
    QVector<Object3D *> m_objects;
    MaterialLibrary m_materialLibrary;
};

#endif // ENGINE3D_H
