#include "cglwidget.h"
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QVector3D>
#include <QImage>
#include <QDebug>
#include <QFileDialog>
#include "objloader.h"


CGLWidget::CGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_program(0),
      m_texture(0)
{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
//                        tr("Obj File(*.obj)"));
//    OBJLoader::load(fileName, vertices, uvs, normals);
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
    qDebug() << "init gl...";
    this->initializeOpenGLFunctions();
    this->glClearColor(0, 0, 0, 1);

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
    m_vbo.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);

    m_uvBuff.create();
    m_uvBuff.bind();
    setupVertexAttribs();
    loadGLTexture();
}

void CGLWidget::resizeGL(int w, int h)
{
    this->glViewport(0, 0, w, h);
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
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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
    if(!buff.load(":/media/tex.bmp"))
    {
        qDebug() << "can't load texture file...";
        QImage dummy(128, 128, QImage::Format::Format_RGB32);
        dummy.fill(QColor(Qt::green).rgb());
        buff = dummy;
    }

    m_texture = new QOpenGLTexture(buff.mirrored());
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
}

void CGLWidget::openFileSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
                        tr("Obj File(*.obj)"));
    OBJLoader::load(fileName, vertices, uvs, normals);
    m_vbo.bind();
    m_vbo.allocate(&vertices[0], vertices.size() * sizeof(QVector3D));
    m_uvBuff.bind();
    m_uvBuff.allocate(&uvs[0], uvs.size() * sizeof(QVector2D));
}
