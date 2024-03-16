#include "materiallibrary.h"
#include "material.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

MaterialLibrary::MaterialLibrary() {}

void MaterialLibrary::addMaterial(Material *material)
{
    if (!material)
        return;

    for (int i = 0; i < m_materials.size(); ++i) {
        if (m_materials[i] == material) {
            return;
        }
    }
    m_materials.append(material);
}

Material *MaterialLibrary::getMaterial(quint32 index)
{
    if (index < (quint32) m_materials.size())
        return m_materials[index];
    else
        return 0;
}

Material *MaterialLibrary::getMaterial(const QString &mtlName)
{
    for (int i = 0; i < m_materials.size(); ++i) {
        if (m_materials[i]->mtlName() == mtlName) {
            return m_materials[i];
        }
    }
    return 0;
}

quint32 MaterialLibrary::getCountMaterials()
{
    return m_materials.size();
}

void MaterialLibrary::loadMaterialFromFile(const QString &filename)
{
    QFile m_mtlFile(filename);
    if (!m_mtlFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open mtl file" << filename;
        return;
    }

    for (int i = 0; i < m_materials.size(); ++i)
        delete m_materials[i];
    m_materials.clear();

    QFileInfo fileInfo(filename);
    QTextStream m_inputStream(&m_mtlFile);

    Material *newMtl = 0;

    while (!m_inputStream.atEnd()) {
        QString str = m_inputStream.readLine();
        QStringList list = str.split(" ");

        if (list[0] == "#") {
            qDebug() << "This is comment : " << str;
            continue;
        } else if (list[0] == "newmtl") {
            addMaterial(newMtl);
            newMtl = new Material;
            newMtl->setName(list[1]);

        } else if (list[0] == "Kd") {
            newMtl->setDiffuseColor(
                QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
        } else if (list[0] == "map_Kd") {
            newMtl->setDiffuseMap(QString("%1/%2").arg(fileInfo.absolutePath()).arg(list[1]));
        }
    }

    addMaterial(newMtl);
    m_mtlFile.close();
}
