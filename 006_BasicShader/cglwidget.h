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
    QOpenGLShaderProgram* m_program;
    QOpenGLTexture* m_texture;
    GLuint m_mvpLoc;
    GLuint m_modelMatLoc;
    GLuint m_viewMatLoc;
    GLuint m_lightPosLoc;
///////////OBJ data
    QVector<QVector3D> vertices;
    QVector<QVector2D> uvs;
    QVector<QVector3D> normals;

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
