#include "vboindexer.h"
#include <QDebug>

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
