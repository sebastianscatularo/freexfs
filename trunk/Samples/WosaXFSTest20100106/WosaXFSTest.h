// WosaXFSTest.h : main header file for the WOSAXFSTEST application
//

#if !defined(AFX_WOSAXFSTEST_H__C06A5BC0_2073_421D_98D6_E016C8A2D687__INCLUDED_)
#define AFX_WOSAXFSTEST_H__C06A5BC0_2073_421D_98D6_E016C8A2D687__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWosaXFSTestApp:
// See WosaXFSTest.cpp for the implementation of this class
//

class CWosaXFSTestApp : public CWinApp
{
public:
	CWosaXFSTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWosaXFSTestApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWosaXFSTestApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WOSAXFSTEST_H__C06A5BC0_2073_421D_98D6_E016C8A2D687__INCLUDED_)
