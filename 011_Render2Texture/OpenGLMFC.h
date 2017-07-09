
// OpenGLMFC.h : main header file for the OpenGLMFC application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COpenGLMFCApp:
// See OpenGLMFC.cpp for the implementation of this class
//

class COpenGLMFCApp : public CWinApp
{
public:
	COpenGLMFCApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGLMFCApp theApp;
