#include "tangentspace.h"

TangentSpace::TangentSpace()
{

}

TangentSpace::~TangentSpace()
{

}

void TangentSpace::computeTangentBasics(
        QVector<QVector3D> &vertices,
        QVector<QVector2D> &uvs,
        QVector<QVector3D> &normals,
        QVector<QVector3D> &tangents,
        QVector<QVector3D> &bitangents)
{
    for(int i = 0, n = vertices.size(); i < n; i+=3)
    {
        QVector3D &v0 = vertices[i+0];
        QVector3D &v1 = vertices[i+1];
        QVector3D &v2 = vertices[i+2];

        QVector2D &uv0 = uvs[i+0];
        QVector2D &uv1 = uvs[i+1];
        QVector2D &uv2 = uvs[i+2];

        QVector3D deltaPos1 = v1 - v0;
        QVector3D deltaPos2 = v2 - v0;
        QVector2D deltaUV1 = uv1 - uv0;
        QVector2D deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
        QVector3D tangent = (deltaPos1 * deltaUV2.y() - deltaPos2 * deltaUV1.y()) * r;
        QVector3D bitangent = (deltaPos2 * deltaUV1.x() - deltaPos1 * deltaUV2.x()) * r;

        // Set the same tangent for all three vertices of the triangle.
        // They will be merged later, in vboindexer.cpp
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }

    for (int i = 0, s = vertices.size(); i < s; i++)
    {
        QVector3D &n = normals[i];
        QVector3D &t = tangents[i];
        QVector3D &b = bitangents[i];

        // Gram-Schmidt orthogonalize
        t = (t - n * QVector3D::dotProduct(n, t)).normalized();
        // Calculate handedness
        if(QVector3D::dotProduct(QVector3D::crossProduct(n, t), b) < 0.0f)
        {
            t = t * -1.0f;
        }
    }
}

