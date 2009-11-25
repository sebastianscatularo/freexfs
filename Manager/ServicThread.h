#if !defined(AFX_SERVICTHREAD_H__BA26BBE7_AD77_4075_8068_0323760D8E0E__INCLUDED_)
#define AFX_SERVICTHREAD_H__BA26BBE7_AD77_4075_8068_0323760D8E0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServicThread.h : header file
//

class CSessionWnd;

/////////////////////////////////////////////////////////////////////////////
// CServicThread thread

class CServicThread : public CWinThread
{
	DECLARE_DYNCREATE(CServicThread)
public:
	CServicThread();           // protected constructor used by dynamic creation

// Attributes
public:
	CSessionWnd *m_pWnd;
	long m_nRefCount;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServicThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	void Destroy();
	virtual ~CServicThread();

	// Generated message map functions
	//{{AFX_MSG(CServicThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	LRESULT CServicThread::OnWFSAsyncOpen(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSOpen(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSAsyncClose(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSClose(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSCancelAsyncRequest(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSAsyncExecute(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSExecute(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSAsyncGetInfo(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSGetInfo(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSAsyncRegister(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSRegister(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSAsyncDeregister(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSDeregister(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSAsyncLock(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSLock(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSAsyncUnlock(WPARAM wParam, LPARAM lParam);
	LRESULT CServicThread::OnWFSUnlock(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVICTHREAD_H__BA26BBE7_AD77_4075_8068_0323760D8E0E__INCLUDED_)
