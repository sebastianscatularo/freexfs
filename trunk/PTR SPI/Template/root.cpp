// $$root$$.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "$$root$$.h"
#include "helper.h"
#include "WorkThread.h"
#include "ServiceBasic.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// C$$Safe_root$$App

BEGIN_MESSAGE_MAP(C$$Safe_root$$App, CWinApp)
	//{{AFX_MSG_MAP(C$$Safe_root$$App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// global variables
CWorkThread *g_pDefaultThread = 0;
CPtrList g_listCanceled;

CCriticalSection g_CriticalSection;

DWORD g_dwTraceLevel = 0;

/////////////////////////////////////////////////////////////////////////////
// C$$Safe_root$$App construction

BOOL IsCancel(REQUESTID ReqID);

C$$Safe_root$$App::C$$Safe_root$$App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only C$$Safe_root$$App object

C$$Safe_root$$App theApp;

/////////////////////////////////////////////////////////////////////////////
// C$$Safe_root$$App initialization

BOOL C$$Safe_root$$App::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}


HRESULT WINAPI WFPCancelAsyncRequest ( HSERVICE hService, REQUESTID RequestID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);
	g_listCanceled.AddTail((void *)RequestID);
   return 0;
}

HRESULT  WINAPI WFPClose ( HSERVICE hService, HWND hWnd, REQUESTID ReqID)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if(g_pDefaultThread == 0) return WFS_ERR_INTERNAL_ERROR;
   if(IsCancel(ReqID))	return WFS_ERR_CANCELED;
   CAutoLock AutoLock(&g_CriticalSection);

   	CServiceBasic *pServiceBasic = new CServiceBasic;
	pServiceBasic->m_hService = hService;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_lpRequestID = new REQUESTID;
	*pServiceBasic->m_lpRequestID = ReqID;
	pServiceBasic->m_bAutoDeleteRequestID = TRUE;

	while(1)
	{
		BOOL b = g_pDefaultThread->PostThreadMessage(WM_NI_SP_Close,
		(WPARAM)pServiceBasic,0);
		if(b) break;
	}
   return 0;
}

HRESULT WINAPI WFPDeregister ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, REQUESTID ReqID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(g_pDefaultThread == 0) return WFS_ERR_INTERNAL_ERROR;
	if(IsCancel(ReqID))	return WFS_ERR_CANCELED;
	CAutoLock AutoLock(&g_CriticalSection);
	
	CServiceBasic *pServiceBasic = new CServiceBasic;
	pServiceBasic->m_hService = hService;
	pServiceBasic->m_dwEventClass = (WORD)dwEventClass;
	pServiceBasic->m_hWndReg = hWndReg;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_lpRequestID = new REQUESTID;
	*pServiceBasic->m_lpRequestID = ReqID;
	pServiceBasic->m_bAutoDeleteRequestID = TRUE;

	while(1)
	{
		BOOL b = g_pDefaultThread->PostThreadMessage(WM_NI_SP_Deregister,
		(WPARAM)pServiceBasic,0);
		if(b) break;
	}
   return 0;
}

HRESULT  WINAPI WFPExecute ( HSERVICE hService, DWORD dwCommand, 
					 LPVOID lpCmdData, DWORD dwTimeOut, 
					 HWND hWnd, REQUESTID ReqID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(g_pDefaultThread == 0) return WFS_ERR_INTERNAL_ERROR;
	if(IsCancel(ReqID))	return WFS_ERR_CANCELED;
	CAutoLock AutoLock(&g_CriticalSection);

	CServiceBasic *pServiceBasic = new CServiceBasic;
	pServiceBasic->m_hService = hService;
	pServiceBasic->m_dwCommand = dwCommand;
	pServiceBasic->m_lpCmdData = lpCmdData;
	pServiceBasic->m_dwTimeOut = dwTimeOut;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_lpRequestID = new REQUESTID;
	*pServiceBasic->m_lpRequestID = ReqID;
	pServiceBasic->m_bAutoDeleteRequestID = TRUE;

	while(1)
	{
		BOOL b = g_pDefaultThread->PostThreadMessage(WM_NI_SP_Execute,
		(WPARAM)pServiceBasic,0);
		if(b) break;
	}



   return 0;
}

HRESULT  WINAPI WFPGetInfo ( HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, HWND hWnd, REQUESTID ReqID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(g_pDefaultThread == 0) return WFS_ERR_INTERNAL_ERROR;
	if(IsCancel(ReqID))	return WFS_ERR_CANCELED;
	CAutoLock AutoLock(&g_CriticalSection);

	CServiceBasic *pServiceBasic = new CServiceBasic;
	pServiceBasic->m_hService = hService;
	pServiceBasic->m_dwCategory = dwCategory;
	pServiceBasic->m_lpQueryDetails = lpQueryDetails;
	pServiceBasic->m_dwTimeOut = dwTimeOut;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_lpRequestID = new REQUESTID;
	*pServiceBasic->m_lpRequestID = ReqID;
	pServiceBasic->m_bAutoDeleteRequestID = TRUE;

	while(1)
	{
		BOOL b = g_pDefaultThread->PostThreadMessage(WM_NI_SP_GetInfo,
		(WPARAM)pServiceBasic,0);
		if(b) break;
	}

	return 0;
}

HRESULT  WINAPI WFPLock ( HSERVICE hService, DWORD dwTimeOut, HWND hWnd, REQUESTID ReqID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(g_pDefaultThread == 0) return WFS_ERR_INTERNAL_ERROR;
	if(IsCancel(ReqID))	return WFS_ERR_CANCELED;
	CAutoLock AutoLock(&g_CriticalSection);

	CServiceBasic *pServiceBasic = new CServiceBasic;
	pServiceBasic->m_hService = hService;
	pServiceBasic->m_dwTimeOut = dwTimeOut;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_lpRequestID = new REQUESTID;
	*pServiceBasic->m_lpRequestID = ReqID;
	pServiceBasic->m_bAutoDeleteRequestID = TRUE;

	while(1)
	{
		BOOL b = g_pDefaultThread->PostThreadMessage(WM_NI_SP_Lock,
		(WPARAM)pServiceBasic,0);
		if(b) break;
	}



   return 0;
}

HRESULT  WINAPI WFPOpen ( HSERVICE hService, LPSTR lpszLogicalName, 
				  HAPP hApp, LPSTR lpszAppID, 
				  DWORD dwTraceLevel, DWORD dwTimeOut, 
				  HWND hWnd, REQUESTID ReqID, 
				  HPROVIDER hProvider, DWORD dwSPIVersionsRequired, 
				  LPWFSVERSION lpSPIVersion, DWORD dwSrvcVersionsRequired, 
				  LPWFSVERSION lpSrvcVersion)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(IsCancel(ReqID))	return WFS_ERR_CANCELED;

	CAutoLock AutoLock(&g_CriticalSection);

	if(g_pDefaultThread == 0)
	{
		g_pDefaultThread = new CWorkThread;
		InterlockedIncrement(&g_pDefaultThread->m_nRef);
		g_pDefaultThread->CreateThread();

	}
	else
	{
		InterlockedIncrement(&g_pDefaultThread->m_nRef);
		return 0;
	}

	lpSPIVersion->wVersion = 0x0003;  //  This is the SPI version of this.
	lpSPIVersion->wLowVersion = 0x0101; //  This is lower bound of the SPI version supported.
	lpSPIVersion->wHighVersion = 0xff03; //  This is higher bound of the SPI version supported.
	strcpy(lpSPIVersion->szDescription,"NewImage");

	CServiceBasic *pServiceBasic = new CServiceBasic;

	pServiceBasic->m_hService = hService;
	pServiceBasic->m_strLogicalName = lpszLogicalName;
	pServiceBasic->m_pServiceThread = hApp;
	pServiceBasic->m_strAppID = lpszAppID;
	pServiceBasic->m_dwTraceLevel = dwTraceLevel;
	pServiceBasic->m_dwTimeOut = dwTimeOut;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_lpRequestID = new REQUESTID;
	*pServiceBasic->m_lpRequestID = ReqID;
	pServiceBasic->m_bAutoDeleteRequestID = TRUE;
	pServiceBasic->m_hLib = hProvider;
	pServiceBasic->m_dwSrvcVersionsRequired = dwSrvcVersionsRequired;
	pServiceBasic->m_lpSrvcVersion = lpSrvcVersion;


	while(1)
	{
		BOOL b = g_pDefaultThread->PostThreadMessage(WM_NI_SP_OPEN,
		(WPARAM)pServiceBasic,0);
		if(b) break;
	}


   return 0;
}

HRESULT  WINAPI WFPRegister ( HSERVICE hService,  DWORD dwEventClass, HWND hWndReg, HWND hWnd, REQUESTID ReqID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(g_pDefaultThread == 0) return WFS_ERR_INTERNAL_ERROR;
	if(IsCancel(ReqID))	return WFS_ERR_CANCELED;
	CAutoLock AutoLock(&g_CriticalSection);

	CServiceBasic *pServiceBasic = new CServiceBasic;
	pServiceBasic->m_hService = hService;
	pServiceBasic->m_dwEventClass = (WORD)dwEventClass;
	pServiceBasic->m_hWndReg = hWndReg;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_lpRequestID = new REQUESTID;
	*pServiceBasic->m_lpRequestID = ReqID;
	pServiceBasic->m_bAutoDeleteRequestID = TRUE;

	while(1)
	{
		BOOL b = g_pDefaultThread->PostThreadMessage(WM_NI_SP_Register,
		(WPARAM)pServiceBasic,0);
		if(b) break;
	}
   return 0;
}

HRESULT  WINAPI WFPSetTraceLevel ( HSERVICE hService, DWORD dwTraceLevel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(g_pDefaultThread == 0) return WFS_ERR_INTERNAL_ERROR;
	CAutoLock AutoLock(&g_CriticalSection);
	// To do
	g_dwTraceLevel = dwTraceLevel;
    return 0;
}

HRESULT  WINAPI WFPUnloadService()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(g_pDefaultThread == 0) return WFS_ERR_INTERNAL_ERROR;
	CAutoLock AutoLock(&g_CriticalSection);
	// Do
    return 0;
}
HRESULT  WINAPI WFPUnlock ( HSERVICE hService, HWND hWnd, REQUESTID ReqID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(g_pDefaultThread == 0) return WFS_ERR_INTERNAL_ERROR;
	if(IsCancel(ReqID))	return WFS_ERR_CANCELED;
	CAutoLock AutoLock(&g_CriticalSection);

	CServiceBasic *pServiceBasic = new CServiceBasic;
	pServiceBasic->m_hService = hService;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_lpRequestID = new REQUESTID;
	*pServiceBasic->m_lpRequestID = ReqID;
	pServiceBasic->m_bAutoDeleteRequestID = TRUE;

	while(1)
	{
		BOOL b = g_pDefaultThread->PostThreadMessage(WM_NI_SP_Unlock,
		(WPARAM)pServiceBasic,0);
		if(b) break;
	}
   return 0;
}


BOOL IsCancel(REQUESTID ReqID)
{
	CAutoLock AutoLock(&g_CriticalSection);

	BOOL bRet = FALSE;
	POSITION pos = g_listCanceled.Find( (void*) ReqID,  NULL ) ;
	if(pos != NULL)
	{
		g_listCanceled.RemoveAt(pos);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
