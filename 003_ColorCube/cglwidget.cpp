#include "cglwidget.h"
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <iostream>
#include <QVector3D>

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

static const GLfloat colorData[] = {
    0.583f, 0.771f, 0.014f,
    0.609f, 0.115f, 0.436f,
    0.327f, 0.483f, 0.844f,
    0.822f, 0.569f, 0.201f,
    0.435f, 0.602f, 0.223f,
    0.310f, 0.747f, 0.185f,
    0.597f, 0.770f, 0.761f,
    0.559f, 0.436f, 0.730f,
    0.359f, 0.583f, 0.152f,
    0.483f, 0.596f, 0.789f,
    0.559f, 0.861f, 0.639f,
    0.195f, 0.548f, 0.859f,
    0.014f, 0.184f, 0.576f,
    0.771f, 0.328f, 0.970f,
    0.406f, 0.615f, 0.116f,
    0.676f, 0.977f, 0.133f,
    0.971f, 0.572f, 0.833f,
    0.140f, 0.616f, 0.489f,
    0.997f, 0.513f, 0.064f,
    0.945f, 0.719f, 0.592f,
    0.543f, 0.021f, 0.978f,
    0.279f, 0.317f, 0.505f,
    0.167f, 0.620f, 0.077f,
    0.347f, 0.857f, 0.137f,
    0.055f, 0.953f, 0.042f,
    0.714f, 0.505f, 0.345f,
    0.783f, 0.290f, 0.734f,
    0.722f, 0.645f, 0.174f,
    0.302f, 0.455f, 0.848f,
    0.225f, 0.587f, 0.040f,
    0.517f, 0.713f, 0.338f,
    0.053f, 0.959f, 0.120f,
    0.393f, 0.621f, 0.362f,
    0.673f, 0.211f, 0.457f,
    0.820f, 0.883f, 0.371f,
    0.982f, 0.099f, 0.879f
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
    m_cvbo.destroy();
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

    m_cvbo.create();
    m_cvbo.bind();
    m_cvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_cvbo.allocate(colorData, sizeof(colorData));

    setupVertexAttribs();
}

void CGLWidget::resizeGL(int w, int h)
{

}

void CGLWidget::paintGL()
{
    std::cout << "paint gl..." << std::endl;
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

    m_cvbo.bind();
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    m_cvbo.release();
}
