// TestPTROcx.h : main header file for the TESTPTROCX application
//

#if !defined(AFX_TESTPTROCX_H__B6F1D319_BF44_4C45_B3B4_D06DCC3C4DD1__INCLUDED_)
#define AFX_TESTPTROCX_H__B6F1D319_BF44_4C45_B3B4_D06DCC3C4DD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestPTROcxApp:
// See TestPTROcx.cpp for the implementation of this class
//

class CTestPTROcxApp : public CWinApp
{
public:
	CTestPTROcxApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestPTROcxApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTestPTROcxApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPTROCX_H__B6F1D319_BF44_4C45_B3B4_D06DCC3C4DD1__INCLUDED_)
