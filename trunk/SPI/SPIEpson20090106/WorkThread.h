#if !defined(AFX_WORKTHREAD_H__56FFC25A_4435_4E92_8392_55C65C0C5C64__INCLUDED_)
#define AFX_WORKTHREAD_H__56FFC25A_4435_4E92_8392_55C65C0C5C64__INCLUDED_

#include "myfile\\printerbase.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkThread.h : header file
//

#define WM_NI_SP_OPEN (WM_USER+2101)
#define WM_NI_SP_Execute (WM_USER+2102)
#define WM_NI_SP_GetInfo (WM_USER+2103)
#define WM_NI_SP_Lock (WM_USER+2104)
#define WM_NI_SP_Register (WM_USER+2105)
#define WM_NI_SP_Unlock (WM_USER+2106)
#define WM_NI_SP_Close (WM_USER+2107)
#define WM_NI_SP_Deregister (WM_USER+2108)







/////////////////////////////////////////////////////////////////////////////
// CWorkThread thread

class CWorkThread : public CWinThread
{
	DECLARE_DYNCREATE(CWorkThread)
public:
	CWorkThread();           // protected constructor used by dynamic creation

	
// Attributes
public:

	LRESULT CWorkThread::OnSPOpen(WPARAM wParam, LPARAM lParam);
	LRESULT CWorkThread::OnSPExecute(WPARAM wParam, LPARAM lParam);
	LRESULT CWorkThread::OnSPGetInfo(WPARAM wParam, LPARAM lParam);
	LRESULT CWorkThread::OnSPGetLock(WPARAM wParam, LPARAM lParam);
	LRESULT CWorkThread::OnSPRegister(WPARAM wParam, LPARAM lParam);
	LRESULT CWorkThread::OnSPUnlock(WPARAM wParam, LPARAM lParam);
	LRESULT CWorkThread::OnSPClose(WPARAM wParam, LPARAM lParam);
	LRESULT CWorkThread::OnSPDeregister(WPARAM wParam, LPARAM lParam);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	long m_lYExt;
	long m_lXExt;
	CDC* m_pDC;
	CString m_strLogicalName;
	CPrinterBase m_PrinterBase;
	long m_nRef;


	LRESULT   Proc_CMD_PTR_PRINT_FORM(WPARAM wParam);
	LRESULT   Proc_CMD_PTR_READ_FORM(WPARAM wParam);
	LRESULT   Proc_CMD_PTR_RAW_DATA(WPARAM wParam);
	LRESULT   Proc_CMD_PTR_MEDIA_EXTENTS(WORD &dwPaperWidth,WORD &dwHeight);
    LRESULT   Proc_CMD_PTR_RESET_COUNT(WPARAM wParam);
    LRESULT	  Proc_CMD_PTR_READ_IMAGE(WPARAM wParam);
    LRESULT	  Proc_CMD_PTR_RESET(WPARAM wParam);
    LRESULT	  Proc_CMD_PTR_RETRACT_MEIDA(WPARAM wParam);
    LRESULT	  Proc_CMD_PTR_DISPENSE_PAPER(WPARAM wParam);

    inline char*     GetStr(char* pSourceBuffer,char *pKey)
	{
		return strstr(pSourceBuffer,pKey)+strlen(pKey);
	}
	
	
	virtual ~CWorkThread();

	// Generated message map functions
	//{{AFX_MSG(CWorkThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKTHREAD_H__56FFC25A_4435_4E92_8392_55C65C0C5C64__INCLUDED_)
