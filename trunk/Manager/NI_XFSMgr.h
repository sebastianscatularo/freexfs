// NI_XFSMgr.h : main header file for the NI_XFSMGR DLL
//

#if !defined(AFX_NI_XFSMGR_H__A3B401B4_E155_47AB_8CC4_79E5DA854CF3__INCLUDED_)
#define AFX_NI_XFSMGR_H__A3B401B4_E155_47AB_8CC4_79E5DA854CF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNI_XFSMgrApp
// See NI_XFSMgr.cpp for the implementation of this class
//

class CNI_XFSMgrApp : public CWinApp
{
public:
	CNI_XFSMgrApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNI_XFSMgrApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CNI_XFSMgrApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NI_XFSMGR_H__A3B401B4_E155_47AB_8CC4_79E5DA854CF3__INCLUDED_)
