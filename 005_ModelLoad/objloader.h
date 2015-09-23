/*************************************/
// Qt下的.obj加载器
// 注意:.obj文件是为Qt资源文件
// @author zcloud
// @date 2015.9.15
/*************************************/
#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <QVector>
#include <QString>
#include <QVector2D>
#include <QVector3D>

struct vec3{
   float x;
   float y;
   float z;
};

struct vec2{
    float x;
    float y;
};

class OBJLoader
{
public:
    OBJLoader();
    ~OBJLoader();
    static void load(QString fileName, QVector<QVector3D> &vertices,
              QVector<QVector2D> &uvs, QVector<QVector3D> &normals);
};

#endif // OBJLOADER_H
