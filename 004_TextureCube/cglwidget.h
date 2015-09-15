#ifndef CGLWIDGET_H
#define CGLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLTexture>

class CGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
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
    QOpenGLShaderProgram* m_program;
    QOpenGLTexture* m_texture;
    GLuint m_mvpLoc;
    void setupVertexAttribs();
    void loadGLTexture();
};

#endif // CGLWIDGET_H
