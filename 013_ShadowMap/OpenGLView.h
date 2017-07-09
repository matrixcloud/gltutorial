
// OpenGLView.h : interface of the COpenGLView class
//

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class COpenGLView : public CView
{
protected: // create from serialization only
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

// Attributes
public:
	COpenGLMFCDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	HGLRC m_hRC;    //Rendering Context
	CDC* m_pDC;        //Device Context
	BOOL InitializeOpenGL();    //Initialize OpenGL
	BOOL SetupPixelFormat();    //Set up the Pixel Format
	void RenderScene();            //Render the Scene
	void RenderOBJ();
	void RenderShadow();
	void Render2Texture();
	void InitData();
	GLuint m_frameBuff;
/////////////////////////OBJ
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	GLuint vertBuff, uvBuff, normalBuff;
	GLuint m_objProgram;
	GLuint m_objTexture;
	GLuint objTextureLoc;
	GLuint objMVPLoc;
	GLuint objDepthBiasLoc;
	GLuint objShadowMapLoc;
///////////////////////Depth
	GLuint m_depthProgram;
	GLuint m_depthTexture;
	glm::mat4 depthMVP;
	GLuint depthMVPLoc;
};

#ifndef _DEBUG  // debug version in OpenGLView.cpp
inline COpenGLMFCDoc* COpenGLView::GetDocument() const
   { return reinterpret_cast<COpenGLMFCDoc*>(m_pDocument); }
#endif

