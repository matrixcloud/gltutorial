
// OpenGLView.cpp : implementation of the COpenGLView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGLMFC.h"
#endif

#include "OpenGLMFCDoc.h"
#include "OpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ShaderLoader.h"
#include "OBJLoader.h"
#include "TextureLoader.h"
#include <glm/gtc/matrix_transform.hpp>
// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// COpenGLView construction/destruction

COpenGLView::COpenGLView()
{
	// TODO: add construction code here

}

COpenGLView::~COpenGLView()
{
	delete m_pDC;
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

// COpenGLView drawing

void COpenGLView::OnDraw(CDC* /*pDC*/)
{
	COpenGLMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	// Clear out the color & depth buffers
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderScene();
	// Tell OpenGL to flush its pipeline
	::glFinish();
	// Now Swap the buffers
	::SwapBuffers(m_pDC->GetSafeHdc());
}


// COpenGLView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLMFCDoc* COpenGLView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLMFCDoc)));
	return (COpenGLMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenGLView message handlers


int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	InitializeOpenGL();
	return 0;
}


void COpenGLView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	//Make the RC non-current
	if (::wglMakeCurrent(0, 0) == FALSE)
	{
		MessageBox(_T("Could not make RC non-current"));
	}

	//Delete the rendering context
	if (::wglDeleteContext(m_hRC) == FALSE)
	{
		MessageBox(_T("Could not delete RC"));
	}
	//Delete the DC
	if (m_pDC)
	{
		delete m_pDC;
	}
	//Set it to NULL
	m_pDC = NULL;
}


void COpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (0 >= cx || 0 >= cy)
	{
		return;
	}
	// select the full client area
	::glViewport(0, 0, cx, cy);
}


BOOL COpenGLView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

BOOL COpenGLView::InitializeOpenGL()
{  
	_cprintf("Initialize OpenGL\n");

	//Get a DC for the Client Area
	m_pDC = new CClientDC(this);
	//Failure to Get DC
	if (m_pDC == NULL)
	{
		MessageBox(_T("Error Obtaining DC"));
		return FALSE;
	}
	//Failure to set the pixel format
	if (!SetupPixelFormat())
	{
		return FALSE;
	}
	//Create Rendering Context
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	//Failure to Create Rendering Context
	if (m_hRC == 0)
	{
		MessageBox(_T("Error Creating RC"));
		return FALSE;
	}
	//Make the RC Current
	if (::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC) == FALSE)
	{
		MessageBox(_T("Error making RC Current"));
		return FALSE;
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		AfxMessageBox(_T("GLEW is not initialized!"));
	}

	//Specify Black as the clear color
	::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	::glEnable(GL_DEPTH_TEST);

	InitModel();

	return TRUE;
}

BOOL COpenGLView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,                // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		16,                             // 16-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	if (m_nPixelFormat == 0)
	{
		return FALSE;
	}
	if (::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

void COpenGLView::RenderScene()
{
	RenderModel();
	RenderQuad();
}

void COpenGLView::RenderModel()
{
	glBindTexture(GL_TEXTURE_2D, modelTexture);
	glBindVertexArray(modelVAO);
	glDrawArrays(GL_TRIANGLES, 0, modelVertices.size());
}

void COpenGLView::InitModel()
{
	OBJLoader::load("./data/suzanne.obj", modelVertices, modelUVs, modelNormals);
	modelProgram = ShaderLoader::load("./data/basic.vert", "./data/basic.frag");
	mvpLoc = glGetUniformLocation(modelProgram, "mvp");
	viewMatLoc = glGetUniformLocation(modelProgram, "v");
	modelMatLoc = glGetUniformLocation(modelProgram, "m");
	texSamplerLoc = glGetUniformLocation(modelProgram, "texSampler");
	lightLoc = glGetUniformLocation(modelProgram, "lightPosition_worldspace");
	modelTexture = TextureLoader::loadDDS("./data/uvmap.DDS");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, modelTexture);


	glm::mat4 modelMatrix = glm::mat4(1.0);
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0, 0, 5),
		glm::vec3(0, 0, 0), 
		glm::vec3(0, 1, 0));
	glm::mat4 projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.001f, 100.0f);
	glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUseProgram(modelProgram);
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, &viewMatrix[0][0]);

	glm::vec3 lightPos = glm::vec3(4, 4, 4);
	glUniform3f(lightLoc, lightPos.x, lightPos.y, lightPos.z);
	glUniform1i(texSamplerLoc, 0);

	glGenVertexArrays(1, &modelVAO);
	glBindVertexArray(modelVAO);
	GLuint vertBuff, uvBuff, norBuff;
	//init vertex buffer
	glGenBuffers(1, &vertBuff);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(glm::vec3), &modelVertices[0], GL_STATIC_DRAW);
	//setup vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//init uv buffer
	glGenBuffers(1, &uvBuff);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuff);
	glBufferData(GL_ARRAY_BUFFER, modelUVs.size() * sizeof(glm::vec2), &modelUVs[0], GL_STATIC_DRAW);
	//setup uv attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	//init normal buffer
	glGenBuffers(1, &norBuff);
	glBindBuffer(GL_ARRAY_BUFFER, norBuff);
	glBufferData(GL_ARRAY_BUFFER, modelNormals.size() * sizeof(glm::vec3), &modelNormals[0], GL_STATIC_DRAW);
	//setup noram attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);
}

void COpenGLView::InitQuad()
{
	// 1. Frame buffer object
	GLuint frameBuff;
	glGenFramebuffers(1, &frameBuff);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuff);
	// 2. Create an empty texture
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1366, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 3. Create a depth buffer
	GLuint depthRenderBuff;
	glGenRenderbuffers(1, &depthRenderBuff);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuff);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1366, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuff);
	// 4. Config frame buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	// Set the list of draw buffers.
	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// The fullscreen quad's FBO
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	static const GLfloat quadData[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	GLuint quadBuff;
	glGenBuffers(1, &quadBuff);
	glBindBuffer(GL_ARRAY_BUFFER, quadBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), quadData, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);
	glEnableVertexAttribArray(0);

	// Create and compile our GLSL program from the shaders
	GLuint quadProgram = ShaderLoader::load("./data/quad.vert", "./data/quad.frag");
	GLuint texLoc = glGetUniformLocation(quadProgram, "renderedTexture");
	GLuint timeLoc = glGetUniformLocation(quadProgram, "time");

	// Use our shader
	glUseProgram(quadProgram);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	// Set our "renderedTexture" sampler to user Texture Unit 0
	glUniform1i(texLoc, 0);
	SYSTEMTIME st;
	GetSystemTime(&st);
	glUniform1f(timeLoc, (float)(st.wMilliseconds*10.0f));
}

void COpenGLView::RenderQuad()
{
	// Draw the triangles !
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
}