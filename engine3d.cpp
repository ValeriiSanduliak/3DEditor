#include "engine3d.h"
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <iostream>
Engine3D::Engine3D() {}

void Engine3D::loadObjectFromFile(const QString &filename)
{
    QFile objFile(filename);
    if (!objFile.exists()) {
        qDebug() << "File not fount";
        return;
    }

    objFile.open(QIODevice::ReadOnly);

    QTextStream input(&objFile);

    QVector<QVector3D> coords;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normals;

    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;
    Object3D *object = 0;
    QString mtlName;
    try {
        while (!input.atEnd()) {
            QString str = input.readLine();
            QStringList list = str.split(" ");
            if (list[0] == "#") {
                qDebug() << "This is comment : " << str;
                continue;
            } else if (list[0] == "mtllib") {
                QFileInfo info(filename);
                m_materialLibrary.loadMaterialFromFile(
                    QString("%1/%2").arg(info.absolutePath()).arg(list[1]));
                qDebug() << "This is file material : " << str;
                continue;
            } else if (list[0] == "v") {
                coords.append(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
                //          qDebug() << list[1].toFloat() << list[2].toFloat() << list[3].toFloat() ;
                continue;
            } else if (list[0] == "vt") {
                texCoords.append(QVector2D(list[1].toFloat(), list[2].toFloat()));
                continue;
            } else if (list[0] == "vn") {
                normals.append(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
                continue;
            } else if (list[0] == "f") {
                for (int i = 1; i <= 3; ++i) {
                    QStringList vert = list[i].split("/");

                    bool conversionOk = false;
                    long index0 = vert[0].toLong(&conversionOk);
                    if (!conversionOk) {
                        throw std::runtime_error("Invalid file");
                    }

                    long index1 = vert[1].toLong(&conversionOk);
                    if (!conversionOk) {
                        throw std::runtime_error("Invalid file");
                    }

                    long index2 = vert[2].toLong(&conversionOk);
                    if (!conversionOk) {
                        throw std::runtime_error("Invalid file");
                    }

                    vertexes.append(
                        VertexData(coords[index0 - 1], texCoords[index1 - 1], normals[index2 - 1]));
                    indexes.append(indexes.size());
                }
                continue;
            } else if (list[0] == "usemtl") {
                if (object) {
                    object->init(vertexes, indexes, m_materialLibrary.getMaterial(mtlName));
                }
                mtlName = list[1];
                addObject(object);
                object = new Object3D;
                vertexes.clear();
                indexes.clear();
            }
        }
        if (object) {
            object->init(vertexes, indexes, m_materialLibrary.getMaterial(mtlName));
        }
    } catch (std::runtime_error &e) {
        QMessageBox::critical(0, "Error", e.what());
        if (object) {
            delete object;
        }
        objFile.close();
        return;
    }
    addObject(object);

    objFile.close();
}

void Engine3D::addObject(Object3D *object)
{
    if (!object)
        return;

    for (int i = 0; i < m_objects.size(); ++i) {
        if (m_objects[i] == object)
            return;
    }
    m_objects.append(object);
}

Object3D *Engine3D::getObject(quint32 index)
{
    if (index < m_objects.size()) {
        return m_objects[index];
    } else {
        // Handle out-of-bounds index gracefully
        qDebug() << "Error: Invalid object index";
        return nullptr;
    }
}

void Engine3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    for (int i = 0; i < m_objects.size(); ++i)
        m_objects[i]->draw(program, functions);
}

void Engine3D::rotate(const QQuaternion &r)
{
    for (int i = 0; i < m_objects.size(); ++i)
        m_objects[i]->rotate(r);
}

void Engine3D::translate(const QVector3D &t)
{
    for (int i = 0; i < m_objects.size(); ++i)
        m_objects[i]->translate(t);
}

void Engine3D::scale(const float &s)
{
    for (int i = 0; i < m_objects.size(); ++i)
        m_objects[i]->scale(s);
}

void Engine3D::setGlobalTransform(const QMatrix4x4 &g)
{
    for (int i = 0; i < m_objects.size(); ++i)
        m_objects[i]->setGlobalTransform(g);
}
