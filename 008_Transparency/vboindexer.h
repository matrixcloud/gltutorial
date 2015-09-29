/////////////////////////////////////
// Qt下VBO索引器
// @author zcloud
// @date 2015.9.27
/////////////////////////////////////
#ifndef VBOINDEXER_H
#define VBOINDEXER_H
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMap>

struct packed_vertex{
    QVector3D position;
    QVector2D uv;
    QVector3D normal;

    bool operator<(const packed_vertex that) const
    {
        return memcmp((void*)this, (void*)&that, sizeof(packed_vertex)) > 0;
    }
};

class VBOIndexer
{
public:
    VBOIndexer();
    ~VBOIndexer();
    static void indexVBO(
            QVector<QVector3D> &vertices,
            QVector<QVector2D> &uvs,
            QVector<QVector3D> &normals,
            QVector<unsigned short> &indices,
            QVector<QVector3D> &indexedVertices,
            QVector<QVector2D> &indexedUVs,
            QVector<QVector3D> &indexedNormals);
private:
    static bool getSimilarVertexIndex_fast(
            packed_vertex &packed,
            QMap<packed_vertex, unsigned short> &vertexToOutIndex,
            unsigned short &result);
};

#endif // VBOINDEXER_H
