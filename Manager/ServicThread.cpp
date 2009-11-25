// ServicThread.cpp : implementation file
//

#include "stdafx.h"
#include "NI_XFSMgr.h"
#include "ServiceBasic.h"
#include "ServicThread.h"
#include "SessionWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServicThread

IMPLEMENT_DYNCREATE(CServicThread, CWinThread)

CServicThread::CServicThread()
{
	m_pWnd = 0;
	m_nRefCount = 0;
}

CServicThread::~CServicThread()
{
}

BOOL CServicThread::InitInstance()
{
	return TRUE;
}

int CServicThread::ExitInstance()
{
	if(m_pWnd) delete m_pWnd;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CServicThread, CWinThread)
	//{{AFX_MSG_MAP(CServicThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_THREAD_MESSAGE(WM_NI_WFSAsyncOpen, OnWFSAsyncOpen)
	ON_THREAD_MESSAGE(WM_NI_WFSOpen, OnWFSOpen)
	ON_THREAD_MESSAGE(WM_NI_WFSAsyncClose, OnWFSAsyncClose)
	ON_THREAD_MESSAGE(WM_NI_WFSClose, OnWFSClose)
	ON_THREAD_MESSAGE(WM_NI_WFSCancelAsyncRequest, OnWFSCancelAsyncRequest)
	ON_THREAD_MESSAGE(WM_NI_WFSAsyncExecute, OnWFSAsyncExecute)
	ON_THREAD_MESSAGE(WM_NI_WFSExecute, OnWFSExecute)
	ON_THREAD_MESSAGE(WM_NI_WFSAsyncGetInfo, OnWFSAsyncGetInfo)
	ON_THREAD_MESSAGE(WM_NI_WFSGetInfo, OnWFSGetInfo)
	ON_THREAD_MESSAGE(WM_NI_WFSAsyncRegister, OnWFSAsyncRegister)
	ON_THREAD_MESSAGE(WM_NI_WFSRegister, OnWFSRegister)
	ON_THREAD_MESSAGE(WM_NI_WFSAsyncDeregister, OnWFSAsyncDeregister)
	ON_THREAD_MESSAGE(WM_NI_WFSDeregister, OnWFSDeregister)
	ON_THREAD_MESSAGE(WM_NI_WFSAsyncLock, OnWFSAsyncLock)
	ON_THREAD_MESSAGE(WM_NI_WFSLock, OnWFSLock)
	ON_THREAD_MESSAGE(WM_NI_WFSAsyncUnlock, OnWFSAsyncUnlock)
	ON_THREAD_MESSAGE(WM_NI_WFSUnlock, OnWFSUnlock)

	
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServicThread message handlers

LRESULT CServicThread::OnWFSAsyncOpen(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	CString strDLLPath = pServiceBasic->FindSPIPath(pServiceBasic->m_strLogicalName);
	if(strDLLPath != "")
	{
		BOOL bRet = pServiceBasic->LoadSPI(strDLLPath);
		if(bRet == FALSE) return -2;
	}
	else return -1;

	DWORD dwSPIVersionsRequired = 0x00039903;

	(*pServiceBasic->m_pfnWFPOpen)(pServiceBasic->m_hService,
		pServiceBasic->m_strLogicalName.GetBuffer(0),
		pServiceBasic->m_pServiceThread,
		pServiceBasic->m_strAppID.GetBuffer(0),
		pServiceBasic->m_dwTraceLevel,
		pServiceBasic->m_dwTimeOut,
		pServiceBasic->m_hWND,
		pServiceBasic->m_nRequestedID,
		pServiceBasic->m_hLib,  // service provider
		dwSPIVersionsRequired,  //SPIVersionsRequired,
		pServiceBasic->m_lpSPIVersion,
		pServiceBasic->m_dwSrvcVersionsRequired,
		pServiceBasic->m_lpSrvcVersion
		);
	if(pServiceBasic) pServiceBasic->m_eventOpen.SetEvent();
	InterlockedIncrement(&m_nRefCount);
	return 0;
}

LRESULT CServicThread::OnWFSOpen(WPARAM wParam, LPARAM lParam)
{
	if(m_pWnd == 0)
	{
		CRect rc(0,0,2,2);
		m_pWnd = new CSessionWnd;
		BOOL b= m_pWnd->CreateEx( NULL,NULL, "ThreadWin", WS_POPUP , rc, NULL, 0,  NULL);
	}

	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	CString strDLLPath = pServiceBasic->FindSPIPath(pServiceBasic->m_strLogicalName);
	if(strDLLPath != "")
	{
		BOOL bRet = pServiceBasic->LoadSPI(strDLLPath);
		if(bRet == FALSE) return -2;
	}
	else return -1;

	DWORD dwSPIVersionsRequired = 0x00039903;
	  // assign request ID

	pServiceBasic->m_nRequestedID = GetTickCount();

	(*pServiceBasic->m_pfnWFPOpen)(pServiceBasic->m_hService,
		pServiceBasic->m_strLogicalName.GetBuffer(0),
		pServiceBasic->m_pServiceThread,
		pServiceBasic->m_strAppID.GetBuffer(0),
		pServiceBasic->m_dwTraceLevel,
		pServiceBasic->m_dwTimeOut,
		m_pWnd->GetSafeHwnd(),
		pServiceBasic->m_nRequestedID,
		pServiceBasic->m_hLib,  // service provider
		dwSPIVersionsRequired,  //SPIVersionsRequired,
		pServiceBasic->m_lpSPIVersion,
		pServiceBasic->m_dwSrvcVersionsRequired,
		pServiceBasic->m_lpSrvcVersion
		);
	InterlockedIncrement(&m_nRefCount);
	return 0;
}

LRESULT CServicThread::OnWFSAsyncClose(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		HRESULT hr = (*pServiceBasic->m_pfnWFPClose)(pServiceBasic->m_hService,
			pServiceBasic->m_hWND,
			pServiceBasic->m_nRequestedID);
	}
	if(pServiceBasic) pServiceBasic->m_eventClose.SetEvent();

	return 0;
}

LRESULT CServicThread::OnWFSClose(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		pServiceBasic->m_nRequestedID = GetTickCount();
		(*pServiceBasic->m_pfnWFPClose)(pServiceBasic->m_hService,
			m_pWnd->GetSafeHwnd(),
			pServiceBasic->m_nRequestedID);
	}

	return 0;
}

void CServicThread::Destroy()
{
	InterlockedDecrement(&m_nRefCount);
	if(m_nRefCount <= 0)
		//PostQuitMessage(1);
		PostThreadMessage(WM_QUIT,0,0);
}

LRESULT CServicThread::OnWFSCancelAsyncRequest(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		HRESULT hr = (*pServiceBasic->m_pfnWFPCancelAsyncRequest)(
			pServiceBasic->m_hService,
			pServiceBasic->m_nRequestedID);
	}

	return 0;
}

LRESULT CServicThread::OnWFSAsyncExecute(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		HRESULT hr = (*pServiceBasic->m_pfnWFPExecute) ( 
			pServiceBasic->m_hService, 
			pServiceBasic->m_dwCommand,
			pServiceBasic->m_lpCmdData, 
			pServiceBasic->m_dwTimeOut,
			pServiceBasic->m_hWND,
			pServiceBasic->m_nRequestedID);

	}
	if(pServiceBasic) pServiceBasic->m_eventExecute.SetEvent();
	return 0;
}

LRESULT CServicThread::OnWFSExecute(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		pServiceBasic->m_nRequestedID = GetTickCount();
		HRESULT hr = (*pServiceBasic->m_pfnWFPExecute) ( 
			pServiceBasic->m_hService, 
			pServiceBasic->m_dwCommand,
			pServiceBasic->m_lpCmdData, 
			pServiceBasic->m_dwTimeOut,
			m_pWnd->GetSafeHwnd(),
			pServiceBasic->m_nRequestedID);
	}
	return 0;
}

LRESULT CServicThread::OnWFSAsyncGetInfo(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		HRESULT hr = (*pServiceBasic->m_pfnWFPGetInfo) ( 
			pServiceBasic->m_hService, 
			pServiceBasic->m_dwCategory,
			pServiceBasic->m_lpQueryDetails, 
			pServiceBasic->m_dwTimeOut,
			pServiceBasic->m_hWND,
			pServiceBasic->m_nRequestedID);
	}
	if(pServiceBasic) pServiceBasic->m_eventGetInfo.SetEvent();
	return 0;
}

LRESULT CServicThread::OnWFSGetInfo(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		pServiceBasic->m_nRequestedID = GetTickCount();
		HRESULT hr = (*pServiceBasic->m_pfnWFPGetInfo) ( 
			pServiceBasic->m_hService, 
			pServiceBasic->m_dwCategory,
			pServiceBasic->m_lpQueryDetails, 
			pServiceBasic->m_dwTimeOut,
			m_pWnd->GetSafeHwnd(),
			pServiceBasic->m_nRequestedID);
	}
	return 0;
}

LRESULT CServicThread::OnWFSAsyncRegister(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		HRESULT hr = (*pServiceBasic->m_pfnWFPRegister) ( 
			pServiceBasic->m_hService, 
			pServiceBasic->m_dwEventClass,
			pServiceBasic->m_hWndReg, 
			pServiceBasic->m_hWND,
			pServiceBasic->m_nRequestedID);
	}
	if(pServiceBasic) pServiceBasic->m_eventRegister.SetEvent();
	return 0;
}

LRESULT CServicThread::OnWFSRegister(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		pServiceBasic->m_nRequestedID = GetTickCount();
		HRESULT hr = (*pServiceBasic->m_pfnWFPRegister) ( 
			pServiceBasic->m_hService, 
			pServiceBasic->m_dwEventClass,
			pServiceBasic->m_hWndReg, 
			m_pWnd->GetSafeHwnd(),
			pServiceBasic->m_nRequestedID);
	}
	return 0;
}

LRESULT CServicThread::OnWFSAsyncDeregister(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		HRESULT hr = (*pServiceBasic->m_pfnWFPDeregister) ( 
			pServiceBasic->m_hService, 
			pServiceBasic->m_dwEventClass,
			pServiceBasic->m_hWndReg, 
			pServiceBasic->m_hWND,
			pServiceBasic->m_nRequestedID);
	}
	if(pServiceBasic) pServiceBasic->m_eventDeregister.SetEvent();
	return 0;
}

LRESULT CServicThread::OnWFSDeregister(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		pServiceBasic->m_nRequestedID = GetTickCount();
		HRESULT hr = (*pServiceBasic->m_pfnWFPDeregister) ( 
			pServiceBasic->m_hService, 
			pServiceBasic->m_dwEventClass,
			pServiceBasic->m_hWndReg, 
			m_pWnd->GetSafeHwnd(),
			pServiceBasic->m_nRequestedID);
	}
	return 0;
}




LRESULT CServicThread::OnWFSAsyncLock(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		HRESULT hr = (*pServiceBasic->m_pfnWFPLock) ( 
			pServiceBasic->m_hService, 
			pServiceBasic->m_dwTimeOut,
			pServiceBasic->m_hWND,
			pServiceBasic->m_nRequestedID);
	}
	if(pServiceBasic) pServiceBasic->m_eventLock.SetEvent();
	return 0;
}

LRESULT CServicThread::OnWFSLock(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		pServiceBasic->m_nRequestedID = GetTickCount();
		HRESULT hr = (*pServiceBasic->m_pfnWFPLock) ( 
			pServiceBasic->m_hService, 
			pServiceBasic->m_dwTimeOut,
			m_pWnd->GetSafeHwnd(),
			pServiceBasic->m_nRequestedID);
	}
	return 0;
}



LRESULT CServicThread::OnWFSAsyncUnlock(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		HRESULT hr = (*pServiceBasic->m_pfnWFPUnlock) ( 
			pServiceBasic->m_hService, 
			pServiceBasic->m_hWND,
			pServiceBasic->m_nRequestedID);
	}
	if(pServiceBasic) pServiceBasic->m_eventUnlock.SetEvent();
	return 0;
}


LRESULT CServicThread::OnWFSUnlock(WPARAM wParam, LPARAM lParam)
{
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;
	if(pServiceBasic)
	{
		pServiceBasic->m_nRequestedID = GetTickCount();
		HRESULT hr = (*pServiceBasic->m_pfnWFPUnlock) ( 
			pServiceBasic->m_hService, 
			m_pWnd->GetSafeHwnd(),
			pServiceBasic->m_nRequestedID);
	}
	return 0;
}
