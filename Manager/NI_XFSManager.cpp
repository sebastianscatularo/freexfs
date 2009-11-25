#include "stdafx.h"
#include "NI_XFSMgr.h"
#include "NI_XFSManager.h"
#include "ServicThread.h"
#include "ServiceBasic.h"
#include <afxcoll.h>
#include "helper.h"

// global variables used.
HSERVICE g_nService = 1;
CObArray g_arrayService;
WFSVERSION g_currentVersions = {0x03,0x03,0x03,"NI_XFSMgr Versions",""};
BOOL g_bAlreadyStartUp = FALSE;
CServicThread *g_pDefaultThread = 0;
BOOL g_bIsBlocking = FALSE;
HANDLE g_BlockedHandle = 0;
long g_dwReqIDSeq = 0;
XFSBLOCKINGHOOK g_XFSBlockingHook = 0;

CCriticalSection g_CriticalSection;

// helper functions
DWORD GetMajorVersion(DWORD dwVersions);
DWORD GetMinorVersion(DWORD dwVersions);
DWORD GetUpperVersion(DWORD dwVersions);
DWORD GetLowerVersion(DWORD dwVersions);
DWORD MakeVersion(DWORD nMajor, DWORD nMinor);
DWORD MakeVersionOrder(DWORD nMajor, DWORD nMinor);

void Blocking(HANDLE hHandle,DWORD dwTimeOut);
void AsyncBlocking(HANDLE hHandle,DWORD dwTimeOut);

HRESULT  WINAPI DuplicateBuffer( LPVOID lpvOriginal, LPVOID * lppvData);
BOOL WINAPI IsValidOfBuffer( LPVOID lpvOriginal);
MemBlock *WINAPI LastBlockOfBuffer(MemBlock *pmBlock);

// Immediate
NI_XFSMANAGER_API HRESULT WINAPI WFSCancelAsyncRequest ( HSERVICE hService, REQUESTID RequestID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic)
	{
		//if(pServiceBasic->m_lpRequestID && pServiceBasic->m_bAutoDeleteRequestID == TRUE)
		//	delete pServiceBasic->m_lpRequestID;
		//pServiceBasic->m_lpRequestID = new REQUESTID;
		//*pServiceBasic->m_lpRequestID = RequestID;
		//pServiceBasic->m_bAutoDeleteRequestID = TRUE;
		pServiceBasic->m_nRequestedID = RequestID;
		//HRESULT hr = pServiceBasic->m_pfnWFPCancelAsyncRequest(pServiceBasic->m_hService,
		//	*pServiceBasic->m_lpRequestID);
		HRESULT hr = pServiceBasic->m_pfnWFPCancelAsyncRequest(pServiceBasic->m_hService,
			pServiceBasic->m_nRequestedID);
		return hr;
		//pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSCancelAsyncRequest,(WPARAM)pServiceBasic,0);
	}
	else
	{
		return WFS_ERR_INVALID_HSERVICE;
	}
   return 0;
}

// CreateThread

NI_XFSMANAGER_API HRESULT WINAPI WFSCancelBlockingCall ( DWORD dwThreadID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking && g_BlockedHandle) 
	{
		long nCount = g_arrayService.GetSize();
		for(long n=0; n<nCount; n++)
		{
			CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(n);
			if(pServiceBasic && pServiceBasic->m_pServiceThread &&
			//	pServiceBasic->m_lpRequestID &&
			pServiceBasic->m_pServiceThread->m_nThreadID == dwThreadID)
			{
				WFSCancelAsyncRequest (pServiceBasic->m_hService,
					//*pServiceBasic->m_lpRequestID);
					pServiceBasic->m_nRequestedID);
			}
		}
		::SetEvent(g_BlockedHandle);
	}
	else 
	{
		return WFS_ERR_NO_BLOCKING_CALL;
	}

   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSCleanUp()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);
	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	g_bAlreadyStartUp = FALSE;
	long nCount = g_arrayService.GetSize();
	for(long n=0; n<nCount; n++)
	{
		CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(n);
		if(pServiceBasic)
		{
			if(pServiceBasic->m_hWND == 0)
			{
				WFSClose(pServiceBasic->m_hService);
			}
			else
			{
				REQUESTID rid = 0;
				WFSAsyncClose (pServiceBasic->m_hService, pServiceBasic->m_hWND, &rid);
			}
		}
	}	
   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSClose ( HSERVICE hService)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic)
	{
		pServiceBasic->m_hResult = WFS_ERR_INTERNAL_ERROR;
		pServiceBasic->m_hWND = 0;
		//pServiceBasic->m_lpRequestID = 0;
		pServiceBasic->m_nRequestedID = 0;
		
		pServiceBasic->m_eventClose.ResetEvent();

		while(1)
		{
			BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSClose,(WPARAM)pServiceBasic,0);
			if(b) break;
		}
		/*
		if(pServiceBasic->m_dwTimeOut == WFS_INDEFINITE_WAIT)
			::WaitForSingleObject(pServiceBasic->m_eventClose.m_hObject,INFINITE);
		else
			::WaitForSingleObject(pServiceBasic->m_eventClose.m_hObject,pServiceBasic->m_dwTimeOut);
		*/
		AutoLock.Unlock();
		Blocking(pServiceBasic->m_eventClose.m_hObject,pServiceBasic->m_dwTimeOut);

		BOOL bRet =  pServiceBasic->m_hResult;

		pServiceBasic->Destroy();
	
		return bRet;
	}
	else
	{
		return WFS_ERR_INVALID_HSERVICE;
	}
   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSAsyncClose ( HSERVICE hService, HWND hWnd, LPREQUESTID lpRequestID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic)
	{
		InterlockedIncrement(&g_dwReqIDSeq);
		*lpRequestID = g_dwReqIDSeq;
		pServiceBasic->m_hWND = hWnd;
		pServiceBasic->m_nRequestedID = g_dwReqIDSeq;

		while(1)
		{
			BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSAsyncClose,(WPARAM)pServiceBasic,0);
			if(b) break;
		}
		AutoLock.Unlock();
		AsyncBlocking(pServiceBasic->m_eventClose.m_hObject,pServiceBasic->m_dwTimeOut);

	
	}
	else
	{
		return WFS_ERR_INVALID_HSERVICE;
	}
   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSCreateAppHandle ( LPHAPP lphApp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	CServicThread *pThread = new CServicThread;
	pThread->CreateThread();
	*lphApp = pThread;
	if(pThread == 0) return WFS_ERR_INVALID_POINTER;
   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSDeregister ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize() ) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	//if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;

	pServiceBasic->m_nRequestedID = 0;
	pServiceBasic->m_hWND = 0;
	pServiceBasic->m_hWndReg = hWndReg;
	pServiceBasic->m_dwEventClass = dwEventClass;

	pServiceBasic->m_eventDeregister.ResetEvent();

	while(1)
	{
		BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSDeregister,
			(WPARAM)pServiceBasic,0);
		if(b) break;
	}

	AutoLock.Unlock();
	Blocking(pServiceBasic->m_eventDeregister.m_hObject,pServiceBasic->m_dwTimeOut);

	return pServiceBasic->m_hResult;
   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSAsyncDeregister ( HSERVICE hService, DWORD dwEventClass, 
											  HWND hWndReg, HWND hWnd, LPREQUESTID lpRequestID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize() ) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	//if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;
	InterlockedIncrement(&g_dwReqIDSeq);
	*lpRequestID = g_dwReqIDSeq;

	pServiceBasic->m_nRequestedID = g_dwReqIDSeq;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_hWndReg = hWndReg;
	pServiceBasic->m_dwEventClass = dwEventClass;

	while(1)
	{
		BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSAsyncDeregister,
			(WPARAM)pServiceBasic,0);
		if(b) break;
	}

	AutoLock.Unlock();
	AsyncBlocking(pServiceBasic->m_eventDeregister.m_hObject,pServiceBasic->m_dwTimeOut);


   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSDestroyAppHandle (HAPP hApp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	CServicThread *pThread = (CServicThread *)hApp;
	if(hApp == WFS_DEFAULT_HAPP) pThread = g_pDefaultThread;
	if(hApp == g_pDefaultThread) hApp == WFS_DEFAULT_HAPP;
	if(pThread)
	{
		//pThread->PostThreadMessage(WM_QUIT,0,0);
		//::TerminateThread(pThread->m_hThread,1);
		//delete pThread;
		if(pThread->m_nRefCount <= 1)
		{
			for(long i=0; i<g_arrayService.GetSize(); i++)
			{
				CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(i);
				if(pServiceBasic && pServiceBasic->m_pServiceThread == pThread)
				{
					pServiceBasic->m_pServiceThread = 0;
					delete pServiceBasic;
					g_arrayService.SetAt(i,0);
				};
			}
			if(hApp == WFS_DEFAULT_HAPP) g_pDefaultThread = 0;
		}
		pThread->Destroy();
	}
	else return WFS_ERR_INVALID_APP_HANDLE;
   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSExecute ( HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, 
									  DWORD dwTimeOut, LPWFSRESULT * lppResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize() ) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;

	//pServiceBasic->m_lpRequestID = lpRequestID;
	//pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_lppResult = lppResult;
	pServiceBasic->m_dwTimeOut = dwTimeOut;
	pServiceBasic->m_dwCommand = dwCommand;
	pServiceBasic->m_lpCmdData = lpCmdData;

	pServiceBasic->m_eventExecute.ResetEvent();

	while(1)
	{
		BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSExecute,
		(WPARAM)pServiceBasic,0);
		if(b) break;
	}

	/*
	if(dwTimeOut == WFS_INDEFINITE_WAIT)
		::WaitForSingleObject(pServiceBasic->m_eventExecute.m_hObject,INFINITE);
	else
		::WaitForSingleObject(pServiceBasic->m_eventExecute.m_hObject,dwTimeOut);
		*/

	AutoLock.Unlock();
	Blocking(pServiceBasic->m_eventExecute.m_hObject,pServiceBasic->m_dwTimeOut);

	//memcpy(*lppResult,pServiceBasic->

	return pServiceBasic->m_hResult;

}

NI_XFSMANAGER_API HRESULT WINAPI WFSAsyncExecute ( HSERVICE hService, DWORD dwCommand, 
										   LPVOID lpCmdData, DWORD dwTimeOut, HWND hWnd, 
										   LPREQUESTID lpRequestID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize()) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	//if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;

	InterlockedIncrement(&g_dwReqIDSeq);
	*lpRequestID = g_dwReqIDSeq;

	pServiceBasic->m_nRequestedID = g_dwReqIDSeq;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_dwTimeOut = dwTimeOut;
	pServiceBasic->m_dwCommand = dwCommand;
	pServiceBasic->m_lpCmdData = lpCmdData;

	while(1)
	{
		BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSAsyncExecute,
		(WPARAM)pServiceBasic,0);
		if(b) break;
	}

	AutoLock.Unlock();
	AsyncBlocking(pServiceBasic->m_eventExecute.m_hObject,pServiceBasic->m_dwTimeOut);


   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSFreeResult ( LPWFSRESULT lpResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	return WFMFreeBuffer( (LPVOID) lpResult);
}

NI_XFSMANAGER_API HRESULT WINAPI WFSGetInfo ( HSERVICE hService, DWORD dwCategory,
									  PVOID lpQueryDetails, 
									  DWORD dwTimeOut, LPWFSRESULT * lppResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize() ) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	//if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;

	//pServiceBasic->m_lpRequestID = lpRequestID;
	//pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_dwTimeOut = dwTimeOut;
	pServiceBasic->m_dwCategory = dwCategory;
	pServiceBasic->m_lpQueryDetails = lpQueryDetails;
	pServiceBasic->m_lppResult = lppResult;

	pServiceBasic->m_eventGetInfo.ResetEvent();

	while(1)
	{
		BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSGetInfo,
		(WPARAM)pServiceBasic,0);
		if(b) break;
	}

	/*
	if(dwTimeOut == WFS_INDEFINITE_WAIT)
		::WaitForSingleObject(pServiceBasic->m_eventOpen.m_hObject,INFINITE);
	else
		::WaitForSingleObject(pServiceBasic->m_eventOpen.m_hObject,dwTimeOut);
		*/
	AutoLock.Unlock();
	Blocking(pServiceBasic->m_eventGetInfo.m_hObject,pServiceBasic->m_dwTimeOut);


   return pServiceBasic->m_hResult;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSAsyncGetInfo ( HSERVICE hService, DWORD dwCategory, 
										   LPVOID lpQueryDetails, DWORD dwTimeOut, 
										   HWND hWnd, LPREQUESTID lpRequestID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize()) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	//if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;

	InterlockedIncrement(&g_dwReqIDSeq);
	*lpRequestID = g_dwReqIDSeq;

	pServiceBasic->m_nRequestedID = g_dwReqIDSeq;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_dwTimeOut = dwTimeOut;
	pServiceBasic->m_dwCategory = dwCategory;
	pServiceBasic->m_lpQueryDetails = lpQueryDetails;
	//DuplicateBuffer(lpQueryDetails, (void **)&pServiceBasic->m_lpQueryDetails);

	//pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSAsyncGetInfo,
	//	(WPARAM)pServiceBasic,0);

	while(1)
	{
		BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSAsyncGetInfo,
		(WPARAM)pServiceBasic,0);
		if(b) break;
	}


	AutoLock.Unlock();
	AsyncBlocking(pServiceBasic->m_eventGetInfo.m_hObject,pServiceBasic->m_dwTimeOut);

   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSIsBlocking ()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	return g_bIsBlocking;

}

NI_XFSMANAGER_API HRESULT WINAPI WFSLock ( HSERVICE hService, DWORD dwTimeOut , LPWFSRESULT * lppResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize() ) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	//if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;

	pServiceBasic->m_nRequestedID = 0;
	pServiceBasic->m_hWND = 0;
	pServiceBasic->m_dwTimeOut = dwTimeOut;
	pServiceBasic->m_lppResult = lppResult;


	pServiceBasic->m_eventLock.ResetEvent();

	while(1)
	{
		BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSLock,
			(WPARAM)pServiceBasic,0);
		if(b) break;
	}

	AutoLock.Unlock();
	Blocking(pServiceBasic->m_eventLock.m_hObject,pServiceBasic->m_dwTimeOut);

	return pServiceBasic->m_hResult;

}

NI_XFSMANAGER_API HRESULT WINAPI WFSAsyncLock ( HSERVICE hService, DWORD dwTimeOut, HWND hWnd,  LPREQUESTID lpRequestID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize() ) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	//if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;

	InterlockedIncrement(&g_dwReqIDSeq);
	*lpRequestID = g_dwReqIDSeq;

	pServiceBasic->m_nRequestedID = g_dwReqIDSeq;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_dwTimeOut = dwTimeOut;

	while(1)
	{
		BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSAsyncLock,
			(WPARAM)pServiceBasic,0);
		if(b) break;
	}
	AutoLock.Unlock();
	AsyncBlocking(pServiceBasic->m_eventLock.m_hObject,pServiceBasic->m_dwTimeOut);



   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSOpen ( LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel,
								   DWORD dwTimeOut, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion, 
								   LPWFSVERSION lpSPIVersion, LPHSERVICE lphService)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(lphService)
	{
		CServiceBasic *pServiceBasic = new CServiceBasic;
		CServicThread *pThread = 0;
		if(hApp == WFS_DEFAULT_HAPP)
		{
			if(g_pDefaultThread == 0)
			{
				g_pDefaultThread = new CServicThread;
				InterlockedIncrement(&g_pDefaultThread->m_nRefCount);
				g_pDefaultThread->CreateThread();
			}
			else
			{
				InterlockedIncrement(&g_pDefaultThread->m_nRefCount);
			}
			pThread = WFS_DEFAULT_HAPP;
		}
		else 
		{
			pThread = (CServicThread *)hApp;
		}

		g_arrayService.Add((CObject *)pServiceBasic);
		*lphService = g_arrayService.GetSize(); 
		pServiceBasic->m_hService = *lphService;

		pServiceBasic->m_strAppID = lpszAppID;
		//pServiceBasic->m_dwTraceLevel = dwTraceLevel;
		WFMSetTraceLevel (*lphService,dwTraceLevel);
		pServiceBasic->m_dwTimeOut = dwTimeOut;
		pServiceBasic->m_pServiceThread = pThread;
		//pServiceBasic->m_hWND = hWnd;
		pServiceBasic->m_dwSrvcVersionsRequired = dwSrvcVersionsRequired;
		pServiceBasic->m_lpSrvcVersion = lpSrvcVersion;
		pServiceBasic->m_lpSPIVersion = lpSPIVersion;
		//pServiceBasic->m_lpRequestID = lpRequestID;
	
		
		pServiceBasic->m_strLogicalName = lpszLogicalName;
	
		pServiceBasic->m_eventOpen.ResetEvent();

		while(1)
		{
          	BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSOpen,(WPARAM)pServiceBasic,0);
			if(b) break;
		}

		/*
		if(dwTimeOut == WFS_INDEFINITE_WAIT)
			::WaitForSingleObject(pServiceBasic->m_eventOpen.m_hObject,INFINITE);
		else
			::WaitForSingleObject(pServiceBasic->m_eventOpen.m_hObject,dwTimeOut);
			*/
	AutoLock.Unlock();
	Blocking(pServiceBasic->m_eventOpen.m_hObject,pServiceBasic->m_dwTimeOut);

		return pServiceBasic->m_hResult;
	}
	else
	{
		return WFS_ERR_INVALID_POINTER;
	}

   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSAsyncOpen ( LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, LPHSERVICE lphService, HWND hWnd, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion, LPWFSVERSION lpSPIVersion, LPREQUESTID lpRequestID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);


	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(lphService)
	{
		InterlockedIncrement(&g_dwReqIDSeq);
		*lpRequestID = g_dwReqIDSeq;

		CServiceBasic *pServiceBasic = new CServiceBasic;
		CServicThread *pThread = 0;
		if(hApp == WFS_DEFAULT_HAPP)
		{
			//pThread = new CServicThread;
			//pThread->CreateThread();
			if(g_pDefaultThread == 0)
			{
				g_pDefaultThread = new CServicThread;
				InterlockedIncrement(&g_pDefaultThread->m_nRefCount);
				g_pDefaultThread->CreateThread();
			}
			else
			{
				InterlockedIncrement(&g_pDefaultThread->m_nRefCount);
			}
			pThread = WFS_DEFAULT_HAPP;
		}
		else 
		{
			pThread = (CServicThread *)hApp;
		}

		g_arrayService.Add((CObject *)pServiceBasic);
		*lphService = g_arrayService.GetSize(); 
		
		pServiceBasic->m_hService = *lphService;

		pServiceBasic->m_strAppID = lpszAppID;
		//pServiceBasic->m_dwTraceLevel = dwTraceLevel;
		WFMSetTraceLevel (*lphService,dwTraceLevel);
		pServiceBasic->m_dwTimeOut = dwTimeOut;
		pServiceBasic->m_pServiceThread = pThread;
		pServiceBasic->m_hWND = hWnd;
		pServiceBasic->m_dwSrvcVersionsRequired = dwSrvcVersionsRequired;
		pServiceBasic->m_lpSrvcVersion = lpSrvcVersion;
		pServiceBasic->m_lpSPIVersion = lpSPIVersion;
		pServiceBasic->m_nRequestedID = g_dwReqIDSeq;
	
		pServiceBasic->m_strLogicalName = lpszLogicalName;
		pServiceBasic->m_pServiceThread = pThread;

		while(1)
		{
			BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSAsyncOpen,(WPARAM)pServiceBasic,0);
			if(b) break;
		}

		AutoLock.Unlock();
		AsyncBlocking(pServiceBasic->m_eventOpen.m_hObject,pServiceBasic->m_dwTimeOut);

	}
	else
	{
		return WFS_ERR_INVALID_POINTER;
	}

   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSRegister ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize() ) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	//if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;

	pServiceBasic->m_nRequestedID = 0;
	pServiceBasic->m_hWND = 0;
	pServiceBasic->m_hWndReg = hWndReg;
	pServiceBasic->m_dwEventClass = dwEventClass;

	pServiceBasic->m_eventRegister.ResetEvent();

	while(1)
	{
		BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSRegister,
			(WPARAM)pServiceBasic,0);
		if(b) break;
	}

	AutoLock.Unlock();
	Blocking(pServiceBasic->m_eventRegister.m_hObject,pServiceBasic->m_dwTimeOut);

	return pServiceBasic->m_hResult;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSAsyncRegister ( HSERVICE hService, DWORD dwEventClass, 
											HWND hWndReg, HWND hWnd, LPREQUESTID lpRequestID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize() ) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	//if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;
	InterlockedIncrement(&g_dwReqIDSeq);
	*lpRequestID = g_dwReqIDSeq;

	pServiceBasic->m_nRequestedID = g_dwReqIDSeq;
	pServiceBasic->m_hWND = hWnd;
	pServiceBasic->m_hWndReg = hWndReg;
	pServiceBasic->m_dwEventClass = dwEventClass;

	while(1)
	{
		BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSAsyncRegister,
			(WPARAM)pServiceBasic,0);
		if(b) break;
	}
	AutoLock.Unlock();
	AsyncBlocking(pServiceBasic->m_eventRegister.m_hObject,pServiceBasic->m_dwTimeOut);




   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSSetBlockingHook ( XFSBLOCKINGHOOK lpBlockFunc, LPXFSBLOCKINGHOOK lppPrevFunc)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	*lppPrevFunc = g_XFSBlockingHook;
	g_XFSBlockingHook = lpBlockFunc;
   return 0;
}

// API version negotiate  Immediate
NI_XFSMANAGER_API HRESULT WINAPI WFSStartUp ( DWORD dwVersionsRequired, LPWFSVERSION lpWFSVersion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(g_bAlreadyStartUp) return WFS_ERR_ALREADY_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	DWORD nUpper = GetUpperVersion(dwVersionsRequired);
	DWORD nLower = GetLowerVersion(dwVersionsRequired);
	DWORD nUpperOrder = MakeVersionOrder(GetMajorVersion(nUpper), GetMinorVersion(nUpper));
	DWORD nLowerOrder = MakeVersionOrder(GetMajorVersion(nLower), GetMinorVersion(nLower));
	if(lpWFSVersion) memcpy(lpWFSVersion,&g_currentVersions,sizeof(WFSVERSION)); else return WFS_ERR_INVALID_POINTER;

	DWORD nUpperOrderThis = MakeVersionOrder(GetMajorVersion(lpWFSVersion->wHighVersion), GetMinorVersion(lpWFSVersion->wHighVersion));
	DWORD nLowerOrderThis = MakeVersionOrder(GetMajorVersion(lpWFSVersion->wLowVersion), GetMinorVersion(lpWFSVersion->wLowVersion));

	if(nLowerOrder > nUpperOrderThis) return WFS_ERR_API_VER_TOO_HIGH;
	if(nUpperOrder  <  nLowerOrder) return WFS_ERR_API_VER_TOO_LOW;

	g_bAlreadyStartUp = TRUE;
   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSUnhookBlockingHook ()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	g_XFSBlockingHook = 0;
   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFSUnlock ( HSERVICE hService)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize() ) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	//if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;

	pServiceBasic->m_nRequestedID = 0;
	pServiceBasic->m_hWND = 0;


	pServiceBasic->m_eventUnlock.ResetEvent();

	while(1)
	{
		BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSUnlock,
			(WPARAM)pServiceBasic,0);
		if(b) break;
	}

	AutoLock.Unlock();
	Blocking(pServiceBasic->m_eventUnlock.m_hObject,pServiceBasic->m_dwTimeOut);

	return pServiceBasic->m_hResult;

}

NI_XFSMANAGER_API HRESULT WINAPI WFSAsyncUnlock ( HSERVICE hService, HWND hWnd, LPREQUESTID lpRequestID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize() ) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	//if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;
	InterlockedIncrement(&g_dwReqIDSeq);
	*lpRequestID = g_dwReqIDSeq;

	pServiceBasic->m_nRequestedID = g_dwReqIDSeq;
	pServiceBasic->m_hWND = hWnd;

	while(1)
	{
		BOOL b = pServiceBasic->GetThread()->PostThreadMessage(WM_NI_WFSAsyncUnlock,
			(WPARAM)pServiceBasic,0);
		if(b) break;
	}
	AutoLock.Unlock();
	AsyncBlocking(pServiceBasic->m_eventUnlock.m_hObject,pServiceBasic->m_dwTimeOut);


   return 0;
}

NI_XFSMANAGER_API HRESULT WINAPI WFMSetTraceLevel ( HSERVICE hService, DWORD dwTraceLevel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//CAutoLock AutoLock(&g_CriticalSection);

	if(!g_bAlreadyStartUp) return WFS_ERR_NOT_STARTED;
	if(g_bIsBlocking) return WFS_ERR_OP_IN_PROGRESS;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize()) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	//if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;
	pServiceBasic->m_dwTraceLevel = dwTraceLevel;
   return 0;
}

NI_XFSMANAGER_API HRESULT  WINAPI WFMAllocateBuffer( ULONG ulSize, ULONG ulFlags, LPVOID * lppvData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//CAutoLock AutoLock(&g_CriticalSection);

	switch(ulFlags)
	{
		case WFS_MEM_SHARE:
			{
			MemBlock *pmBlock = 0;
			pmBlock = (MemBlock *)malloc(ulSize + sizeof(MemBlock));
			if(pmBlock == 0) return WFS_ERR_OUT_OF_MEMORY;
			pmBlock->m_nLen = ulSize;
			pmBlock->m_ulFlags = WFS_MEM_SHARE;
			pmBlock->m_pNext = 0;
			
			* lppvData = pmBlock->m_pData;
			}
			break;
		case WFS_MEM_ZEROINIT:
			{
			MemBlock *pmBlock = 0;
			pmBlock = (MemBlock *)calloc(ulSize + sizeof(MemBlock), sizeof(BYTE));
			if(pmBlock == 0) return WFS_ERR_OUT_OF_MEMORY;

			pmBlock->m_nLen = ulSize;
			pmBlock->m_ulFlags = WFS_MEM_ZEROINIT;
			pmBlock->m_pNext = 0;
			
			*lppvData = pmBlock->m_pData;
			}
			break;
	}
   return WFS_SUCCESS;
}

NI_XFSMANAGER_API HRESULT  WINAPI WFMAllocateMore( ULONG ulSize, LPVOID lpvOriginal, LPVOID * lppvData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//CAutoLock AutoLock(&g_CriticalSection);
	// Find the last memory block
	*lppvData = 0;
	if(!IsValidOfBuffer(lpvOriginal)) return S_FALSE;

	MemBlock *pmBlock0 = (MemBlock *)(((BYTE *)lpvOriginal) - sizeof(MemBlock));

	while(pmBlock0 != 0)
	{
		if(pmBlock0->m_pNext == 0)
		{
			switch(pmBlock0->m_ulFlags)
			{
				case WFS_MEM_SHARE:
					{
					MemBlock *pmBlock = 0;
					pmBlock = (MemBlock *)malloc(ulSize + sizeof(MemBlock));
					if(pmBlock == 0) return WFS_ERR_OUT_OF_MEMORY;
					pmBlock->m_nLen = ulSize;
					pmBlock->m_ulFlags = WFS_MEM_SHARE;
					pmBlock->m_pNext = 0;
					
					pmBlock0->m_pNext = pmBlock;
					
					*lppvData = pmBlock->m_pData;
					}
					break;
				case WFS_MEM_ZEROINIT:
					{
					MemBlock *pmBlock = 0;
					pmBlock = (MemBlock *)calloc(ulSize + sizeof(MemBlock), sizeof(BYTE));
					if(pmBlock == 0) return WFS_ERR_OUT_OF_MEMORY;

					pmBlock->m_nLen = ulSize;
					pmBlock->m_ulFlags = WFS_MEM_ZEROINIT;
					pmBlock->m_pNext = 0;
					pmBlock0->m_pNext = pmBlock;
					
					*lppvData = pmBlock->m_pData;
					}
					break;
			}
			break;
		}
		pmBlock0 = pmBlock0->m_pNext;
	}

   return WFS_SUCCESS;
}

NI_XFSMANAGER_API HRESULT  WINAPI WFMFreeBuffer( LPVOID lpvData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//CAutoLock AutoLock(&g_CriticalSection);
	if(lpvData)
	{
		MemBlock *pmBlock0 = (MemBlock *)(((BYTE *)lpvData) - sizeof(MemBlock));

		while(pmBlock0 != 0)
		{
			MemBlock *pmBlock1 = pmBlock0 ;
			pmBlock0 = pmBlock0->m_pNext;
			free(pmBlock1);
		}
	}



	//if(lpvData) free(lpvData); else return WFS_ERR_INVALID_POINTER;
   return WFS_SUCCESS;
}

NI_XFSMANAGER_API HRESULT  WINAPI WFMGetTraceLevel ( HSERVICE hService, LPDWORD lpdwTraceLevel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//CAutoLock AutoLock(&g_CriticalSection);

	if(g_bAlreadyStartUp) return WFS_ERR_ALREADY_STARTED;
	if(hService <= 0 ) return WFS_ERR_INVALID_HSERVICE;
	if(hService > g_arrayService.GetSize()) return WFS_ERR_INVALID_HSERVICE;
	CServiceBasic *pServiceBasic = (CServiceBasic *)g_arrayService.GetAt(hService-1);
	if(pServiceBasic == 0) return WFS_ERR_INVALID_POINTER;
	if(pServiceBasic->m_pServiceThread == 0) return WFS_ERR_INVALID_HSERVICE;
	if(lpdwTraceLevel == 0) return WFS_ERR_INVALID_POINTER;
	*lpdwTraceLevel = pServiceBasic->m_dwTraceLevel;
   return 0;
}

NI_XFSMANAGER_API HRESULT  WINAPI WFMKillTimer( WORD wTimerID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//CAutoLock AutoLock(&g_CriticalSection);

   return 0;
}

NI_XFSMANAGER_API HRESULT  WINAPI WFMOutputTraceData ( LPSTR lpszData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//CAutoLock AutoLock(&g_CriticalSection);
   return 0;
}

NI_XFSMANAGER_API HRESULT  WINAPI WFMReleaseDLL ( HPROVIDER hProvider)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//CAutoLock AutoLock(&g_CriticalSection);

	FreeLibrary((HMODULE)hProvider);

   return 0;
}

NI_XFSMANAGER_API HRESULT  WINAPI WFMSetTimer ( HWND hWnd, LPVOID lpContext, DWORD dwTimeVal, LPWORD lpwTimerID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//CAutoLock AutoLock(&g_CriticalSection);

	::SetTimer(hWnd,*lpwTimerID,dwTimeVal,0);
   return 0;
}


// Helper functions
DWORD GetUpperVersion(DWORD dwVersions)
{
	return dwVersions & 0xffff;
}
DWORD GetLowerVersion(DWORD dwVersions)
{
	return (dwVersions & 0xffff0000) >> 16;
}

DWORD GetMajorVersion(DWORD dwVersions)
{
	return dwVersions & 0xff;
}
DWORD GetMinorVersion(DWORD dwVersions)
{
	return (dwVersions & 0x0000ff00) >> 8;
}

DWORD MakeVersion(DWORD nMajor, DWORD nMinor)
{
	DWORD nVersion = 0;
	nVersion = nVersion | ((nMinor & 0xff) << 8) | (nMajor & 0xff);
	return nVersion;
}

DWORD MakeVersionOrder(DWORD nMajor, DWORD nMinor)
{
	DWORD nVersion = 0;
	nVersion = nVersion | ((nMinor & 0xff) ) | ((nMajor & 0xff) << 8);
	return nVersion;
}

void Blocking(HANDLE hHandle,DWORD dwTimeOut)
{
	DWORD dwMilliseconds = dwTimeOut;
	if(dwTimeOut == WFS_INDEFINITE_WAIT) dwMilliseconds = INFINITE;

	HANDLE handles[1];
	g_bIsBlocking = TRUE;
	g_BlockedHandle = hHandle;
	handles[0] = hHandle;
	DWORD dwTick1 = ::GetTickCount();
	while (MsgWaitForMultipleObjects(1, &handles[0], FALSE, dwMilliseconds, QS_ALLINPUT | QS_ALLPOSTMESSAGE) == WAIT_OBJECT_0 + 1)
    {
		DWORD dwTick2 = ::GetTickCount();
		if(dwMilliseconds != INFINITE)
		{
			if((dwTick2 - dwTick1) > dwMilliseconds) break;
		}

		if(g_XFSBlockingHook)   // execute the blocking hook
		{
			(*g_XFSBlockingHook)();
		}
        else
		{
		   MSG msg;
		   while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
    }
	g_bIsBlocking = FALSE;
	g_BlockedHandle = 0;

}

void AsyncBlocking(HANDLE hHandle,DWORD dwTimeOut)
{
	DWORD dwMilliseconds = dwTimeOut;
	if(dwTimeOut == WFS_INDEFINITE_WAIT) dwMilliseconds = INFINITE;

	HANDLE handles[1];
	handles[0] = hHandle;
	DWORD dwTick1 = ::GetTickCount();
	while (MsgWaitForMultipleObjects(1, &handles[0], FALSE, dwMilliseconds, QS_ALLINPUT | QS_ALLPOSTMESSAGE) == WAIT_OBJECT_0 + 1)
    {
		DWORD dwTick2 = ::GetTickCount();
		if(dwMilliseconds != INFINITE)
		{
			if((dwTick2 - dwTick1) > dwMilliseconds) break;
		}

		if(g_XFSBlockingHook)   // execute the blocking hook
		{
			(*g_XFSBlockingHook)();
		}
        else
		{
		   MSG msg;
		   while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
    }


}



HRESULT  WINAPI DuplicateBuffer( LPVOID lpvOriginal, LPVOID * lppvData)
{
	*lppvData = 0;
	if(!IsValidOfBuffer(lpvOriginal)) return S_FALSE;
	
	MemBlock *pmBlockHead = 0; 
	MemBlock *pmBlock0 = (MemBlock *)(((BYTE *)lpvOriginal) - sizeof(MemBlock));
	BOOL bFirst = TRUE;
	while(pmBlock0 != 0)
	{
		MemBlock *pmBlock = 0;
		pmBlock = (MemBlock *)malloc(pmBlock0->m_nLen + sizeof(MemBlock));
		memcpy(pmBlock,pmBlock0 ,pmBlock0->m_nLen + sizeof(MemBlock));
		pmBlock->m_pNext = 0;
		if(bFirst == TRUE) 
		{	pmBlockHead = pmBlock;
			bFirst == FALSE;
		}
		else
		{
			MemBlock *pLast = LastBlockOfBuffer(pmBlockHead);
			pLast->m_pNext = pmBlock;
		}

		if(pmBlock0->m_pNext == 0)
		{
			*lppvData = pmBlockHead->m_pData;
			return 0;
		}
		else
		{
			pmBlock0 = pmBlock0->m_pNext;
		}
	}
	return 0;
}

MemBlock *WINAPI LastBlockOfBuffer(MemBlock *pmBlock)
{
	while(1)
	{
		if(pmBlock->m_pNext == 0) return pmBlock;
		pmBlock = pmBlock ->m_pNext;
	}
}

BOOL WINAPI IsValidOfBuffer( LPVOID lpvOriginal)
{
	MemBlock *pmBlock = (MemBlock *)(((BYTE *)lpvOriginal) - sizeof(MemBlock));
	if(pmBlock->m_ulFlags == WFS_MEM_SHARE  || pmBlock->m_ulFlags == WFS_MEM_ZEROINIT) return TRUE;
	else return FALSE;
}

