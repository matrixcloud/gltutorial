#include "cglwidget.h"
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QVector3D>
#include <QImage>
#include <QDebug>
#include <QFileDialog>
#include "objloader.h"
#include "vboindexer.h"
#include "tangentspace.h"
#include <QMatrix3x3>

CGLWidget::CGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_program(0),
      diffuseTexture(0),
      normalTexture(0),
      specularTexture(0),
      m_indexBuff(QOpenGLBuffer::IndexBuffer)
{
}

CGLWidget::~CGLWidget()
{
    makeCurrent();
    m_vbo.destroy();
    m_uvBuff.destroy();
    delete m_program;
    delete diffuseTexture;
    delete normalTexture;
    delete specularTexture;
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
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/basic.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/basic.frag");
    m_program->link();

    m_program->bind();
    m_mvpLoc = m_program->uniformLocation("mvp");
    m_modelMatLoc = m_program->uniformLocation("m");
    m_viewMatLoc = m_program->uniformLocation("v");
    mv3x3Loc = m_program->uniformLocation("mv3x3");
    m_lightPosLoc = m_program->uniformLocation("lightPosition_worldspace");
    diffuseSamplerLoc = m_program->uniformLocation("diffuseTextureSampler");
    normalSamplerLoc = m_program->uniformLocation("normalTextureSampler");
    specularSamplerLoc = m_program->uniformLocation("specularTextureSampler");

    m_vbo.create();
    m_vbo.bind();

    m_uvBuff.create();
    m_uvBuff.bind();

    m_normalBuff.create();
    m_normalBuff.bind();

    m_indexBuff.create();
    m_indexBuff.bind();

    m_tangentBuff.create();
    m_tangentBuff.bind();

    m_bitangentBuff.create();
    m_bitangentBuff.create();

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
    glDisable(GL_CULL_FACE);

    QMatrix4x4 m,v,p;
    m.setToIdentity();
    m.translate(QVector3D(0, -0.8, 0));
    v.lookAt(QVector3D(0, 0, 5), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    p.perspective(45.0f,  4.0f / 3.0f, 0.1f, 100.0f);
    QMatrix4x4 mvp = p * v * m;
    QMatrix4x4 mv4x4 = v * m;
    QMatrix3x3 mv3x3 = mv4x4.normalMatrix();

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_mvpLoc, mvp);
    m_program->setUniformValue(m_modelMatLoc, m);
    m_program->setUniformValue(m_viewMatLoc, v);
    m_program->setUniformValue(mv3x3Loc, mv3x3);
    QVector3D lightPos_worldspace(-6, 0, 4);
    m_program->setUniformValue(m_lightPosLoc, lightPos_worldspace);

    diffuseTexture->bind(GL_TEXTURE0);
    m_program->setUniformValue(diffuseSamplerLoc, 0);
    normalTexture->bind(GL_TEXTURE1);
    m_program->setUniformValue(normalSamplerLoc, 1);
    specularTexture->bind(GL_TEXTURE2);
    m_program->setUniformValue(specularSamplerLoc, 2);
    m_indexBuff.bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
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

    m_normalBuff.bind();
    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    m_tangentBuff.bind();
    f->glEnableVertexAttribArray(3);
    f->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    m_bitangentBuff.bind();
    f->glEnableVertexAttribArray(4);
    f->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}


void CGLWidget::loadGLTexture()
{
    diffuseTexture = new QOpenGLTexture(QImage(":/texture/diffuse.DDS").mirrored());
    normalTexture = new QOpenGLTexture(QImage(":/texture/normal.bmp").mirrored());
    specularTexture = new QOpenGLTexture(QImage(":/texture/specular.DDS").mirrored());
}

void CGLWidget::openFileSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
                        tr("Obj File(*.obj)"));
    QVector<QVector3D> vertices;
    QVector<QVector2D> uvs;
    QVector<QVector3D> normals;
    OBJLoader::load(fileName, vertices, uvs, normals);
    TangentSpace::computeTangentBasics(
                vertices, uvs, normals,
                tangents, bitangents);
    VBOIndexer::indexVBO_TBN(
                vertices, uvs, normals, tangents, bitangents,
                indices, indexedVertices, indexedUVs, indexedNormals,indexedTangents, indexedBitangents);

    m_vbo.bind();
    m_vbo.allocate(&indexedVertices[0], indexedVertices.size() * sizeof(QVector3D));
    m_uvBuff.bind();
    m_uvBuff.allocate(&indexedUVs[0], indexedUVs.size() * sizeof(QVector2D));
    m_normalBuff.bind();
    m_normalBuff.allocate(&indexedNormals[0], indexedNormals.size() * sizeof(QVector3D));
    m_indexBuff.bind();
    m_indexBuff.allocate(&indices[0], indices.size() * sizeof(unsigned short));
    m_tangentBuff.bind();
    m_tangentBuff.allocate(&indexedTangents[0], indexedTangents.size() * sizeof(QVector3D));
    m_bitangentBuff.bind();
    m_bitangentBuff.allocate(&indexedBitangents[0], indexedBitangents.size() * sizeof(QVector3D));
}
