// SessionWnd.cpp : implementation file
//

#include "stdafx.h"
#include "NI_XFSMgr.h"
#include "SessionWnd.h"
#include "NI_XFSManager.h"
#include "ServiceBasic.h"
#include "ServicThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSessionWnd
extern CObArray g_arrayService;
CSessionWnd::CSessionWnd()
{
}

CSessionWnd::~CSessionWnd()
{
}


BEGIN_MESSAGE_MAP(CSessionWnd, CWnd)
	//{{AFX_MSG_MAP(CSessionWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_MESSAGE(WFS_OPEN_COMPLETE, OnOpenComplete)
		ON_MESSAGE(WFS_CLOSE_COMPLETE, OnCloseComplete)
		ON_MESSAGE(WFS_EXECUTE_COMPLETE, OnExecuteComplete)
		ON_MESSAGE(WFS_GETINFO_COMPLETE, OnGetInfoComplete)
		ON_MESSAGE(WFS_REGISTER_COMPLETE, OnRegisterComplete)
		ON_MESSAGE(WFS_DEREGISTER_COMPLETE, OnDergisterComplete)
		ON_MESSAGE(WFS_LOCK_COMPLETE, OnLockComplete)
		ON_MESSAGE(WFS_UNLOCK_COMPLETE, OnUnlockComplete)
		
		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// sync mode// CSessionWnd message handlers
LRESULT CSessionWnd::OnOpenComplete(WPARAM wParam, LPARAM lParam)
{
	WFSRESULT *pResult = (WFSRESULT *)lParam;
	// finding serviceBasic
	CServiceBasic *pServiceBasic = 0;
	for(long i=0; i<g_arrayService.GetSize(); i++)
	{
		pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(i);
		if(pServiceBasic->m_nRequestedID == pResult->RequestID)
		{
			pServiceBasic->m_hResult = pResult->hResult;
			break;
		}
		pServiceBasic = 0;
	}
	WFSFreeResult(pResult);
	if(pServiceBasic) pServiceBasic->m_nRequestedID = 0;
	if(pServiceBasic) pServiceBasic->m_eventOpen.SetEvent();
	return 0;
}

LRESULT CSessionWnd::OnCloseComplete(WPARAM wParam, LPARAM lParam)
{
	WFSRESULT *pResult = (WFSRESULT *)lParam;
	// finding serviceBasic
	CServiceBasic *pServiceBasic = 0;
	for(long i=0; i<g_arrayService.GetSize(); i++)
	{
		pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(i);
		if(pServiceBasic->m_nRequestedID == pResult->RequestID)
		{
			pServiceBasic->m_hResult = pResult->hResult;
			break;
		}
	    pServiceBasic = 0;
	}
	WFSFreeResult(pResult);
	if(pServiceBasic) pServiceBasic->m_nRequestedID = 0;
	if(pServiceBasic) pServiceBasic->m_eventClose.SetEvent();
	return 0;
}

LRESULT CSessionWnd::OnExecuteComplete(WPARAM wParam, LPARAM lParam)
{
	WFSRESULT *pResult = (WFSRESULT *)lParam;
	// finding serviceBasic
	CServiceBasic *pServiceBasic = 0;
	for(long i=0; i<g_arrayService.GetSize(); i++)
	{
		pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(i);
		if(pServiceBasic->m_nRequestedID == pResult->RequestID)
		{
			pServiceBasic->m_hResult = pResult->hResult;
			*pServiceBasic->m_lppResult = pResult;
			break;
		}
		pServiceBasic = 0;
	}
	if(pServiceBasic) pServiceBasic->m_nRequestedID = 0;
	if(pServiceBasic) pServiceBasic->m_eventExecute.SetEvent();
	return 0;
}

LRESULT CSessionWnd::OnGetInfoComplete(WPARAM wParam, LPARAM lParam)
{
	WFSRESULT *pResult = (WFSRESULT *)lParam;
	// finding serviceBasic
	CServiceBasic *pServiceBasic = 0;
	for(long i=0; i<g_arrayService.GetSize(); i++)
	{
		pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(i);
		if(pServiceBasic->m_nRequestedID == pResult->RequestID)
		{
			pServiceBasic->m_hResult = pResult->hResult;
			*pServiceBasic->m_lppResult = pResult;
			break;
		}
		pServiceBasic = 0;
	}
	if(pServiceBasic) pServiceBasic->m_nRequestedID = 0;
	if(pServiceBasic) pServiceBasic->m_eventGetInfo.SetEvent();
	return 0;
}

LRESULT CSessionWnd::OnRegisterComplete(WPARAM wParam, LPARAM lParam)
{
	WFSRESULT *pResult = (WFSRESULT *)lParam;
	// finding serviceBasic
	CServiceBasic *pServiceBasic = 0;
	for(long i=0; i<g_arrayService.GetSize(); i++)
	{
		pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(i);
		if(pServiceBasic->m_nRequestedID == pResult->RequestID)
		{
			pServiceBasic->m_hResult = pResult->hResult;
			break;
		}
		pServiceBasic = 0;
	}
	WFSFreeResult(pResult);
	if(pServiceBasic) pServiceBasic->m_nRequestedID = 0;
	if(pServiceBasic) pServiceBasic->m_eventRegister.SetEvent();
	return 0;
}

LRESULT CSessionWnd::OnDergisterComplete(WPARAM wParam, LPARAM lParam)
{
	WFSRESULT *pResult = (WFSRESULT *)lParam;
	// finding serviceBasic
	CServiceBasic *pServiceBasic = 0;
	for(long i=0; i<g_arrayService.GetSize(); i++)
	{
		pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(i);
		if(pServiceBasic->m_nRequestedID == pResult->RequestID)
		{
			pServiceBasic->m_hResult = pResult->hResult;
			break;
		}
		pServiceBasic = 0;
	}
	WFSFreeResult(pResult);
	if(pServiceBasic) pServiceBasic->m_nRequestedID = 0;
	if(pServiceBasic) pServiceBasic->m_eventDeregister.SetEvent();
	return 0;
}

LRESULT CSessionWnd::OnLockComplete(WPARAM wParam, LPARAM lParam)
{
	WFSRESULT *pResult = (WFSRESULT *)lParam;
	// finding serviceBasic
	CServiceBasic *pServiceBasic = 0;
	for(long i=0; i<g_arrayService.GetSize(); i++)
	{
		pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(i);
		if(pServiceBasic->m_nRequestedID == pResult->RequestID)
		{
			pServiceBasic->m_hResult = pResult->hResult;
			*pServiceBasic->m_lppResult = pResult;
			break;
		}
		pServiceBasic = 0;
	}
	if(pServiceBasic) pServiceBasic->m_nRequestedID = 0;
	if(pServiceBasic) pServiceBasic->m_eventLock.SetEvent();
	return 0;
}

LRESULT CSessionWnd::OnUnlockComplete(WPARAM wParam, LPARAM lParam)
{
	WFSRESULT *pResult = (WFSRESULT *)lParam;
	// finding serviceBasic
	CServiceBasic *pServiceBasic = 0;
	for(long i=0; i<g_arrayService.GetSize(); i++)
	{
		pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(i);
		if(pServiceBasic->m_nRequestedID == pResult->RequestID)
		{
			pServiceBasic->m_hResult = pResult->hResult;
			break;
		}
	    pServiceBasic = 0;
	}
	WFSFreeResult(pResult);
	if(pServiceBasic) pServiceBasic->m_nRequestedID = 0;
	if(pServiceBasic) pServiceBasic->m_eventUnlock.SetEvent();
	return 0;
}



