#ifndef TANGENTSPACE_H
#define TANGENTSPACE_H
#include <QVector>
#include <QVector3D>
#include <QVector2D>

class TangentSpace
{
public:
    TangentSpace();
    ~TangentSpace();
    static void computeTangentBasics(
            QVector<QVector3D> &vertices,
            QVector<QVector2D> &uvs,
            QVector<QVector3D> &normals,
            QVector<QVector3D> &tangents,
            QVector<QVector3D> &bitangents
            );
};

#endif // TANGENTSPACE_H
