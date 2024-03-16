#ifndef MATERIAL_H
#define MATERIAL_H

#include <QImage>
#include <QString>
#include <QVector3D>

class Material
{
public:
    Material();
    void setName(const QString &mtlName);
    const QString &mtlName() const;

    void setDiffuseColor(const QVector3D &diffuseColor);
    const QVector3D &diffuseColor() const;

    void setDiffuseMap(const QString &filename);
    void setDiffuseMap(const QImage &imageFile);
    const QImage &diffuseMap() const;
    bool isUsingDiffuseMap() const;

private:
    QString m_mtlName;
    QVector3D m_diffuseColor;
    QImage m_diffuseMap;
    bool m_isUsingDiffuseMap;
};

#endif // MATERIAL_H
