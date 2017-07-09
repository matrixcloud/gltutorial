#include "vboindexer.h"
#include <QDebug>
#include <cmath>

VBOIndexer::VBOIndexer()
{

}

VBOIndexer::~VBOIndexer()
{

}

void VBOIndexer::indexVBO(
        QVector<QVector3D> &vertices,
        QVector<QVector2D> &uvs,
        QVector<QVector3D> &normals,
        QVector<unsigned short> &indices,
        QVector<QVector3D> &indexedVertices,
        QVector<QVector2D> &indexedUVs,
        QVector<QVector3D> &indexedNormals)
{
    QMap<packed_vertex, unsigned short> vertexToOutIndex;
    for(int i = 0, n = vertices.size(); i < n; i++)
    {
        packed_vertex packed = {vertices[i], uvs[i], normals[i]};
        unsigned short index;
        bool found = getSimilarVertexIndex_fast(packed, vertexToOutIndex, index);
        if(found)
        {
            indices.push_back(index);
        }
        else
        {
            indexedVertices.push_back(vertices[i]);
            indexedUVs.push_back(uvs[i]);
            indexedNormals.push_back(normals[i]);
            unsigned short newindex = (unsigned short)indexedVertices.size() - 1;
            indices.push_back(newindex);
            vertexToOutIndex[packed] = newindex;
        }
    }
}

void VBOIndexer::indexVBO_TBN(
        QVector<QVector3D> &vertices,
        QVector<QVector2D> &uvs,
        QVector<QVector3D> &normals,
        QVector<QVector3D> &tangents,
        QVector<QVector3D> &bitangents,
        QVector<unsigned short> &indices,
        QVector<QVector3D> &indexedVertices,
        QVector<QVector2D> &indexedUVs,
        QVector<QVector3D> &indexedNormals,
        QVector<QVector3D> &indexedTangents,
        QVector<QVector3D> &indexedBitangents)
{
    for(int i = 0, n = vertices.size(); i < n; i++)
    {
        unsigned short index;
        bool found = getSimilarVertexIndex(
                        vertices[i], uvs[i], normals[i],
                        indexedVertices, indexedUVs, indexedNormals, index);
        if(found)
        {
            indices.push_back(index);

            indexedTangents[index] += tangents[i];
            indexedBitangents[index] += bitangents[i];
        }
        else
        {
            indexedVertices.push_back(vertices[i]);
            indexedUVs.push_back(uvs[i]);
            indexedNormals.push_back(normals[i]);
            indexedTangents.push_back(tangents[i]);
            indexedBitangents.push_back(bitangents[i]);
            indices.push_back(indexedVertices.size() - 1);
        }
    }
}

bool VBOIndexer::getSimilarVertexIndex_fast(
        packed_vertex &packed,
        QMap<packed_vertex, unsigned short> &vertexToOutIndex,
        unsigned short &result)
{
    QMap<packed_vertex, unsigned short>::iterator it = vertexToOutIndex.find(packed);
    if(it == vertexToOutIndex.end())
    {
        return false;
    }
    else
    {
        result = it.value();
        return true;
    }
}

bool VBOIndexer::isNear(float v1, float v2)
{
    return fabs(v1 - v2) < 0.01f;
}

bool VBOIndexer::getSimilarVertexIndex(
        QVector3D &vertex,
        QVector2D &uv,
        QVector3D &normal,
        QVector<QVector3D> &vertices,
        QVector<QVector2D> &uvs,
        QVector<QVector3D> &normals,
        unsigned short &result)
{
    for(int i = 0, n = vertices.size(); i < n; i++)
    {
        if(isNear(vertex.x(), vertices[i].x()) &&
           isNear(vertex.y(), vertices[i].x()) &&
           isNear(vertex.z(), vertices[i].z()) &&
           isNear(uv.x(), uvs[i].x()) &&
           isNear(uv.y(), uvs[i].y()) &&
           isNear(normal.x(), normals[i].x()) &&
           isNear(normal.y(), normals[i].x()) &&
           isNear(normal.z(), normals[i].z())
           )
        {
            result = i;
            return true;
        }
    }
    return false;
}
