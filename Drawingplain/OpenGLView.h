
// OpenGLView.h : interface of the COpenGLView class
//

#pragma once


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
	void RenderGrid(
		int xPointCount, 
		int yPointCount,
		double xSpacing,
		double ySpaceing,
		void (*deformFunction)(double pt[3], const double bounds[4]));
	double* ComputePointCoords(
		int index, 
		int xPointCount,
		int yPointCount,
		double xSpacing,
		double ySpaceing);
	void ComputePointCoords(
		int index, 
		int xPointCount, 
		int yPointCount, 
		double xSpacing, 
		double ySpaceing, 
		double pt[3]);
};

#ifndef _DEBUG  // debug version in OpenGLView.cpp
inline COpenGLMFCDoc* COpenGLView::GetDocument() const
   { return reinterpret_cast<COpenGLMFCDoc*>(m_pDocument); }
#endif

