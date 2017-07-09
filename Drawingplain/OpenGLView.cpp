
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

// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// COpenGLView construction/destruction


static void SincModify(double pt[3], const double bounds[4])
{
	double r = sqrt(pt[0] * pt[0] + pt[1] * pt[1]);
	const double pi = 3.1415;
	pt[2] = -sin(pi*r) / (pi*r);

	//delete[] bounds;//error
}

static void terrainModify(double pt[3], const double bounds[4])
{
	CImage image;
	image.Load(_T("heightmap.png"));
	int image_w = image.GetWidth() - 1;
	int image_h = image.GetHeight() - 1;
	int image_x = int(((pt[0] - bounds[0]) * image_w) / bounds[2]);
	int image_y = int(((pt[1] - bounds[1]) * image_h) / bounds[3]);

	// Pick the gray scale value from the pixel
	COLORREF color = image.GetPixel(image_x, image_y);
	pt[2] = 0.25 * double(color) / 255.0;
}

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
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*glOrtho(-2, 2, -2, 2, -2, 2);*/
	gluPerspective(30, double(cx) / double(cy), 0.1, 20.0);
}


BOOL COpenGLView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

BOOL COpenGLView::InitializeOpenGL()
{  
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

	glClearColor(1, 1, 1, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPointSize(4);
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
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -6);
	glRotatef(-60, 1, 0, 0);

	glColor3f(0, 0, 0);
	RenderGrid(50, 50, 0.06, 0.06, SincModify);
	//RenderGrid(50, 50, 0.06, 0.06, terrainModify);//slowly
}

void COpenGLView::RenderGrid(
	int xPointCount, 
	int yPointCount, 
	double xSpacing, 
	double ySpaceing,
	void(*deformFunction)(double pt[3], const double bounds[4]))
{
	int quadCount = (xPointCount-1) * (yPointCount-1);
	double bounds[4];
	if (deformFunction)
	{
		double width = (xPointCount - 1) * xSpacing;
		double height = (yPointCount - 1) * ySpaceing;

		bounds[0] = -width / 2;
		bounds[1] = -height / 2;
		bounds[2] = width;
		bounds[3] = height;
	}

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < quadCount; i++)
	{
		int k = i + (i / (xPointCount - 1));
		int a = k;
		int b = k + 1;
		int c = k + 1 + xPointCount;
		int d = k + xPointCount;

		double aPt[3], bPt[3], cPt[3], dPt[3];
		ComputePointCoords(a, xPointCount, yPointCount, xSpacing, ySpaceing, aPt);
		ComputePointCoords(b, xPointCount, yPointCount, xSpacing, ySpaceing, bPt);
		ComputePointCoords(c, xPointCount, yPointCount, xSpacing, ySpaceing, cPt);
		ComputePointCoords(d, xPointCount, yPointCount, xSpacing, ySpaceing, dPt);

		if (deformFunction)
		{
			deformFunction(aPt, bounds);
			deformFunction(bPt, bounds);
			deformFunction(cPt, bounds);
			deformFunction(dPt, bounds);
		}

		//triangle 1
		glVertex3dv(aPt);
		glVertex3dv(cPt);
		glVertex3dv(dPt);

		//trinagle 2
		glVertex3dv(aPt);
		glVertex3dv(bPt);
		glVertex3dv(cPt);
	}
	glEnd();
}

double* COpenGLView::ComputePointCoords(int index, int xPointCount, int yPointCount, double xSpacing, double ySpaceing)
{
	static double pt[3];
	double width = (xPointCount - 1) * xSpacing;
	double height = (yPointCount - 1) * ySpaceing;
	double minX = -width / 2;
	double minY = -height / 2;

	pt[0] = minX + xSpacing * (index%xPointCount);
	pt[1] = minY + ySpaceing * (index/xPointCount);
	pt[2] = 0;

	return pt;
}

void COpenGLView::ComputePointCoords(
	int index, 
	int xPointCount,
	int yPointCount,
	double xSpacing,
	double ySpaceing,
	double pt[3])
{
	double* tmp = ComputePointCoords(index, xPointCount, yPointCount, xSpacing, ySpaceing);
	pt[0] = tmp[0];
	pt[1] = tmp[1];
	pt[2] = tmp[2];
}