#include "cglwidget.h"
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <iostream>
#include <QVector3D>

//triangle vertex data
static const GLfloat vertexData[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f, 1.0f, 0.0f
};

CGLWidget::CGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_program(0)
{

}

CGLWidget::~CGLWidget()
{
    makeCurrent();
    m_vbo.destroy();
    delete m_program;
    m_program = 0;
    doneCurrent();
}

void CGLWidget::initializeGL()
{
    std::cout << "init gl..." << std::endl;
    this->initializeOpenGLFunctions();
    this->glClearColor(0, 0, 1, 1);

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/glsl/basic.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/glsl/basic.frag");
    m_program->link();

    m_program->bind();
    m_mvpLoc = m_program->uniformLocation("mvp");

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertexData, sizeof(vertexData));

    setupVertexAttribs();
}

void CGLWidget::resizeGL(int w, int h)
{

}

void CGLWidget::paintGL()
{
    std::cout << "paint gl..." << std::endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m, v, p;
    m.setToIdentity();
    v.lookAt(QVector3D(4, 3, 3), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    p.perspective(45.0f,  4.0f / 3.0f, 0.1f, 100.0f);
    QMatrix4x4 mvp = p * v * m;

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_mvpLoc, mvp);

    glDrawArrays(GL_TRIANGLES, 0, 3);//total 3 vertices
    m_program->release();
}

void CGLWidget::setupVertexAttribs()
{
    m_vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,  (void*)0);
    m_vbo.release();
}
