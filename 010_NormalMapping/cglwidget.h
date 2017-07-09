#ifndef CGLWIDGET_H
#define CGLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include "mainwindow.h"
#include <QVector>
#include <QVector2D>
#include <QVector3D>

class CGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    CGLWidget(QWidget *parent = 0);
    ~CGLWidget();
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_uvBuff;
    QOpenGLBuffer m_normalBuff;
    QOpenGLBuffer m_indexBuff;
    QOpenGLBuffer m_tangentBuff;
    QOpenGLBuffer m_bitangentBuff;
    QOpenGLShaderProgram* m_program;
    QOpenGLTexture* diffuseTexture;
    QOpenGLTexture* normalTexture;
    QOpenGLTexture* specularTexture;
////////////attribute location
    GLuint m_mvpLoc;
    GLuint m_modelMatLoc;
    GLuint m_viewMatLoc;
    GLuint m_lightPosLoc;
    GLuint diffuseSamplerLoc;
    GLuint normalSamplerLoc;
    GLuint specularSamplerLoc;
    GLuint mv3x3Loc;
///////////OBJ data
    QVector<unsigned short> indices;
    QVector<QVector3D> tangents;
    QVector<QVector3D> bitangents;
    QVector<QVector3D> indexedVertices;
    QVector<QVector2D> indexedUVs;
    QVector<QVector3D> indexedNormals;
    QVector<QVector3D> indexedTangents;
    QVector<QVector3D> indexedBitangents;

    QMatrix4x4 mMat;
    QMatrix4x4 vMat;
    QMatrix4x4 pMat;
private:
///////GL render
    void setupVertexAttribs();
    void loadGLTexture();
private slots:
    void openFileSlot();
};

#endif // CGLWIDGET_H
