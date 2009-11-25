#if !defined(AFX_SESSIONWND_H__5FD0AD6D_6D47_470F_901F_719B17F96DD4__INCLUDED_)
#define AFX_SESSIONWND_H__5FD0AD6D_6D47_470F_901F_719B17F96DD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SessionWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSessionWnd window

class CSessionWnd : public CWnd
{
// Construction
public:
	CSessionWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSessionWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSessionWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSessionWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	LRESULT OnOpenComplete(WPARAM wParam, LPARAM lParam);
	LRESULT OnCloseComplete(WPARAM wParam, LPARAM lParam);
	LRESULT OnExecuteComplete(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetInfoComplete(WPARAM wParam, LPARAM lParam);
	LRESULT OnRegisterComplete(WPARAM wParam, LPARAM lParam);
	LRESULT OnDergisterComplete(WPARAM wParam, LPARAM lParam);
	LRESULT OnLockComplete(WPARAM wParam, LPARAM lParam);
	LRESULT OnUnlockComplete(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SESSIONWND_H__5FD0AD6D_6D47_470F_901F_719B17F96DD4__INCLUDED_)
