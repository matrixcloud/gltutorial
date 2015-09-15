#include "cglwidget.h"
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <iostream>
#include <QVector3D>
#include <QImage>

//triangle vertex data
static const GLfloat vertexData[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

static const GLfloat uvData[] = {
    0.000059f, 1.0f-0.000004f,
    0.000103f, 1.0f-0.336048f,
    0.335973f, 1.0f-0.335903f,
    1.000023f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.336024f, 1.0f-0.671877f,
    0.667969f, 1.0f-0.671889f,
    1.000023f, 1.0f-0.000013f,
    0.668104f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.000059f, 1.0f-0.000004f,
    0.335973f, 1.0f-0.335903f,
    0.336098f, 1.0f-0.000071f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.336024f, 1.0f-0.671877f,
    1.000004f, 1.0f-0.671847f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.668104f, 1.0f-0.000013f,
    0.335973f, 1.0f-0.335903f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.668104f, 1.0f-0.000013f,
    0.336098f, 1.0f-0.000071f,
    0.000103f, 1.0f-0.336048f,
    0.000004f, 1.0f-0.671870f,
    0.336024f, 1.0f-0.671877f,
    0.000103f, 1.0f-0.336048f,
    0.336024f, 1.0f-0.671877f,
    0.335973f, 1.0f-0.335903f,
    0.667969f, 1.0f-0.671889f,
    1.000004f, 1.0f-0.671847f,
    0.667979f, 1.0f-0.335851f
};

CGLWidget::CGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_program(0),
      m_texture(0)
{

}

CGLWidget::~CGLWidget()
{
    makeCurrent();
    m_vbo.destroy();
    m_uvBuff.destroy();
    delete m_program;
    delete m_texture;
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

    m_uvBuff.create();
    m_uvBuff.bind();
    m_uvBuff.allocate(uvData, sizeof(uvData));

    setupVertexAttribs();
    loadGLTexture();
}

void CGLWidget::resizeGL(int w, int h)
{

}

void CGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    QMatrix4x4 m, v, p;
    m.setToIdentity();
    v.lookAt(QVector3D(4, 3, -3), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    p.perspective(45.0f,  4.0f / 3.0f, 0.1f, 100.0f);
    QMatrix4x4 mvp = p * v * m;

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_mvpLoc, mvp);

    m_texture->bind();
    glDrawArrays(GL_TRIANGLES, 0, 12*3);//
    m_program->release();
}

void CGLWidget::setupVertexAttribs()
{
    m_vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,  (void*)0);
    m_vbo.release();

    m_uvBuff.bind();
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    m_uvBuff.release();
}

void CGLWidget::loadGLTexture()
{
    QImage buff;
    if(!buff.load(":/texture/tex.bmp"))
    {
        std::cout << "can't load texture file..." << std::endl;
        QImage dummy(128, 128, QImage::Format::Format_RGB32);
        dummy.fill(QColor(Qt::green).rgb());
        buff = dummy;
    }

    m_texture = new QOpenGLTexture(buff.mirrored());
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
}
