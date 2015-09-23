#include "objloader.h"
#include <QFile>
#include <QString>
#include <QIODevice>
#include <QDebug>
#include <QTextStream>
#include <QStringList>

OBJLoader::OBJLoader()
{

}

OBJLoader::~OBJLoader()
{

}

void OBJLoader::load(QString fileName, QVector<QVector3D> &vertices,
                     QVector<QVector2D> &uvs, QVector<QVector3D> &normals)
{
    QVector<unsigned int> vertexIndices, uvIndices, normalIndices;
    QVector<QVector3D> tmpVertices;
    QVector<QVector2D> tmpUVs;
    QVector<QVector3D> tmpNormals;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't read " << fileName << "\n";
        return;
    }

    QTextStream in(&file);
    QString label;
    while(!in.atEnd())
    {
        in >> label;
        if(QString::compare(label, "v", Qt::CaseSensitive) == 0)
        {
            vec3 v3;
            in >> v3.x >> v3.y >> v3.z;
            QVector3D v3d(v3.x, v3.y, v3.z);
            tmpVertices.push_back(v3d);
        }
        if(QString::compare(label, "vt", Qt::CaseSensitive) == 0)
        {
            vec2 v2;
            in >> v2.x >> v2.y;
            QVector2D v2d(v2.x, v2.y);
            tmpUVs.push_back(v2d);
        }
        if(QString::compare(label, "vn", Qt::CaseSensitive) == 0)
        {
            vec3 v3;
            in >> v3.x >> v3.y >> v3.z;
            QVector3D v3d(v3.x, v3.y, v3.z);
            tmpNormals.push_back(v3d);
        }
        if(QString::compare(label, "f", Qt::CaseSensitive) == 0)
        {
            QString tmp1, tmp2, tmp3;
            in >> tmp1 >> tmp2 >> tmp3;
            QStringList slist1 = tmp1.split("/");
            QStringList slist2 = tmp2.split("/");
            QStringList slist3 = tmp3.split("/");

            vertexIndices.push_back(slist1[0].toUInt());
            uvIndices.push_back(slist1[1].toUInt());
            normalIndices.push_back(slist1[2].toUInt());

            vertexIndices.push_back(slist2[0].toUInt());
            uvIndices.push_back(slist2[1].toUInt());
            normalIndices.push_back(slist2[2].toUInt());

            vertexIndices.push_back(slist3[0].toUInt());
            uvIndices.push_back(slist3[1].toUInt());
            normalIndices.push_back(slist3[2].toUInt());
        }
    }

    //generate new vertices,uvs,normals
    for(int i = 0, n = vertexIndices.size(); i< n; i++)
    {
        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        // Get the attributes thanks to the index
        QVector3D vertex = tmpVertices[vertexIndex-1];
        QVector2D uv = tmpUVs[uvIndex-1];
        QVector3D normal = tmpNormals[normalIndex-1];

        // Put the attributes in buffers
        vertices.push_back(vertex);
        uvs.push_back(uv);
        normals.push_back(normal);
    }
}
