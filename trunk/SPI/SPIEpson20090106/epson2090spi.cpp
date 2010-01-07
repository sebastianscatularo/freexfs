// epson2090spi.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "epson2090spi.h"
#include "helper.h"
#include "WorkThread.h"
#include "ServiceBasic.h"
#include "Xfsconf.h"
#include "fundef.h"


pfnWFSCancelAsyncRequest  m_pfnWFSCancelAsyncRequest = 0;
pfnWFSCancelBlockingCall  m_pfnWFSCancelBlockingCall = 0;
pfnWFSCleanUp  m_pfnWFSCleanUp = 0;
pfnWFSClose  m_pfnWFSClose = 0;
pfnWFSAsyncClose  m_pfnWFSAsyncClose = 0;
pfnWFSCreateAppHandle  m_pfnWFSCreateAppHandle = 0;
pfnWFSDeregister  m_pfnWFSDeregister = 0;
pfnWFSAsyncDeregister  m_pfnWFSAsyncDeregister = 0;
pfnWFSDestroyAppHandle  m_pfnWFSDestroyAppHandle = 0;
pfnWFSExecute  m_pfnWFSExecute = 0;
pfnWFSAsyncExecute  m_pfnWFSAsyncExecute = 0;
pfnWFSFreeResult  m_pfnWFSFreeResult = 0;
pfnWFSGetInfo  m_pfnWFSGetInfo = 0;
pfnWFSAsyncGetInfo  m_pfnWFSAsyncGetInfo = 0;
pfnWFSIsBlocking  m_pfnWFSIsBlocking = 0;
pfnWFSLock  m_pfnWFSLock = 0;
pfnWFSAsyncLock  m_pfnWFSAsyncLock = 0;
pfnWFSOpen  m_pfnWFSOpen = 0;
pfnWFSAsyncOpen  m_pfnWFSAsyncOpen = 0;
pfnWFSRegister  m_pfnWFSRegister = 0;
pfnWFSAsyncRegister  m_pfnWFSAsyncRegister = 0;
pfnWFSSetBlockingHook  m_pfnWFSSetBlockingHook = 0;
pfnWFSStartUp  m_pfnWFSStartUp = 0;
pfnWFSUnhookBlockingHook  m_pfnWFSUnhookBlockingHook = 0;
pfnWFSUnlock  m_pfnWFSUnlock = 0;
pfnWFSAsyncUnlock  m_pfnWFSAsyncUnlock = 0;
pfnWFMSetTraceLevel  m_pfnWFMSetTraceLevel = 0;

pfnWFMAllocateBuffer m_pfnWFMAllocateBuffer= 0;
pfnWFMAllocateMore   m_pfnWFMAllocateMore= 0;
pfnWFMFreeBuffer m_pfnWFMFreeBuffer= 0;
pfnWFMGetTraceLevel m_pfnWFMGetTraceLevel= 0;
pfnWFMKillTimer m_pfnWFMKillTimer= 0;
pfnWFMOutputTraceData m_pfnWFMOutputTraceData= 0;
pfnWFMReleaseDLL m_pfnWFMReleaseDLL= 0;
pfnWFMSetTimer m_pfnWFMSetTimer= 0;


CString FindXMLManagerPath();
BOOL LoadManagerFunction(CString strPath);

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
// CEpson2090spiApp

BEGIN_MESSAGE_MAP(CEpson2090spiApp, CWinApp)
	//{{AFX_MSG_MAP(CEpson2090spiApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// global variables
CWorkThread *g_pDefaultThread = 0;
CPtrList g_listCanceled;

CCriticalSection g_CriticalSection;

DWORD g_dwTraceLevel = 0;

CMapPtrToPtr g_mapService;

HMODULE g_hLib = 0;
CString g_strResult = "";

/////////////////////////////////////////////////////////////////////////////
// CEpson2090spiApp construction

BOOL IsCancel(REQUESTID ReqID);

CEpson2090spiApp::CEpson2090spiApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEpson2090spiApp object

CEpson2090spiApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEpson2090spiApp initialization

BOOL CEpson2090spiApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	CString strXMLManagerPath = FindXMLManagerPath();
	if(strXMLManagerPath != "")
	{
		BOOL bRet = LoadManagerFunction(strXMLManagerPath);
		if(!bRet) return FALSE;
	}

	return TRUE;
}

int CEpson2090spiApp::ExitInstance()
{
	if(g_hLib)
	{
		FreeLibrary(g_hLib);
		g_hLib = 0;
	}
	return CWinApp::ExitInstance();
}


CString FindXMLManagerPath()
{
	DWORD dwResult = 0;

#ifdef _V2_
	HKEY hKeyXFS_ROOT;
	HRESULT hr = WFMOpenKey(HKEY_CLASSES_ROOT, //WOSA/XFS_ROOT/LOGICAL_SERVICES,
		"WOSA/XFS_ROOT", //lpszSubKey, 
		&hKeyXFS_ROOT); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return -1; 

	HKEY hKeyXFSManager;
	hr = WFMOpenKey(hKeyXFS_ROOT, //WOSA/XFS_ROOT/LOGICAL_SERVICES,
		"XFS_MANAGER", //lpszSubKey, 
		&hKeyXFSManager); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return -1; 
#else
	HKEY hKeyXFSManager;
	HRESULT hr = WFMOpenKey(WFS_CFG_MACHINE_XFS_ROOT,
		"XFS_MANAGER", //lpszSubKey, 
		&hKeyXFSManager);
	if(hr != S_OK) return "";
#endif


	char buf[2048];
	DWORD len = 2048;
	hr = WFMQueryValue (hKeyXFSManager,"dllname",buf, &len);
	if(hr != WFS_SUCCESS) return "";
	buf[len] = 0;
	CString strOut = buf;
#ifdef _V2_
	WFMCloseKey (hKeyXFS_ROOT);
	WFMCloseKey (hKeyXFSManager);
#else
	WFMCloseKey (hKeyXFSManager);
#endif

	return strOut;

}


BOOL LoadManagerFunction(CString strPath)
{
	g_strResult = _T("");
	g_hLib = LoadLibrary(strPath);
	if(g_hLib == NULL)
	{
		g_strResult += _T("Load XFS Manager failed.\r\n");
		return FALSE;
	}
	else 
	{
		g_strResult += _T("Load XFS Manager succeeded.\r\n");
	}

	long nLen = g_strResult.GetLength();
	
	m_pfnWFSCancelBlockingCall = (pfnWFSCancelBlockingCall)GetProcAddress(g_hLib,"WFSCancelBlockingCall");
	if(m_pfnWFSCancelBlockingCall == 0) g_strResult += _T("WFSCancelBlockingCall missing.\r\n");
	m_pfnWFSCleanUp = (pfnWFSCleanUp)GetProcAddress(g_hLib,"WFSCleanUp");
	if(m_pfnWFSCleanUp == 0) g_strResult += _T("WFSCleanUp missing.\r\n");
	m_pfnWFSClose = (pfnWFSClose)GetProcAddress(g_hLib,"WFSClose");
	if(m_pfnWFSClose == 0) g_strResult += _T("WFSClose missing.\r\n");
	m_pfnWFSAsyncClose = (pfnWFSAsyncClose)GetProcAddress(g_hLib,"WFSAsyncClose");
	if(m_pfnWFSAsyncClose == 0) g_strResult += _T("WFSAsyncClose missing.\r\n");
	m_pfnWFSCreateAppHandle = (pfnWFSCreateAppHandle)GetProcAddress(g_hLib,"WFSCreateAppHandle");
	if(m_pfnWFSCreateAppHandle == 0) g_strResult += _T("WFSCreateAppHandle missing.\r\n");
	m_pfnWFSDeregister = (pfnWFSDeregister)GetProcAddress(g_hLib,"WFSDeregister");
	if(m_pfnWFSDeregister == 0) g_strResult += _T("WFSDeregister missing.\r\n");
	m_pfnWFSAsyncDeregister = (pfnWFSAsyncDeregister)GetProcAddress(g_hLib,"WFSAsyncDeregister");
	if(m_pfnWFSAsyncDeregister == 0) g_strResult += _T("WFSAsyncDeregister missing.\r\n");
	m_pfnWFSDestroyAppHandle = (pfnWFSDestroyAppHandle)GetProcAddress(g_hLib,"WFSDestroyAppHandle");
	if(m_pfnWFSDestroyAppHandle == 0) g_strResult += _T("WFSDestroyAppHandle missing.\r\n");
	m_pfnWFSExecute = (pfnWFSExecute)GetProcAddress(g_hLib,"WFSExecute");
	if(m_pfnWFSExecute == 0) g_strResult += _T("WFSExecute missing.\r\n");
	m_pfnWFSAsyncExecute = (pfnWFSAsyncExecute)GetProcAddress(g_hLib,"WFSAsyncExecute");
	if(m_pfnWFSAsyncExecute == 0) g_strResult += _T("WFSAsyncExecute missing.\r\n");
	m_pfnWFSFreeResult = (pfnWFSFreeResult)GetProcAddress(g_hLib,"WFSFreeResult");
	if(m_pfnWFSFreeResult == 0) g_strResult += _T("WFSFreeResult missing.\r\n");
	m_pfnWFSGetInfo = (pfnWFSGetInfo)GetProcAddress(g_hLib,"WFSGetInfo");
	if(m_pfnWFSGetInfo == 0) g_strResult += _T("WFSGetInfo missing.\r\n");

	m_pfnWFSAsyncGetInfo = (pfnWFSAsyncGetInfo)GetProcAddress(g_hLib,"WFSAsyncGetInfo");
	if(m_pfnWFSAsyncGetInfo == 0) g_strResult += _T("WFSAsyncGetInfo missing.\r\n");
	m_pfnWFSIsBlocking = (pfnWFSIsBlocking)GetProcAddress(g_hLib,"WFSIsBlocking");
	if(m_pfnWFSIsBlocking == 0) g_strResult += _T("WFSIsBlocking missing.\r\n");
	m_pfnWFSLock = (pfnWFSLock)GetProcAddress(g_hLib,"WFSLock");
	if(m_pfnWFSLock == 0) g_strResult += _T("WFSLock missing.\r\n");
	m_pfnWFSAsyncLock = (pfnWFSAsyncLock)GetProcAddress(g_hLib,"WFSAsyncLock");
	if(m_pfnWFSAsyncLock == 0) g_strResult += _T("WFSAsyncLock missing.\r\n");
	m_pfnWFSOpen = (pfnWFSOpen)GetProcAddress(g_hLib,"WFSOpen");
	if(m_pfnWFSOpen == 0) g_strResult += _T("WFSOpen missing.\r\n");
	m_pfnWFSAsyncOpen = (pfnWFSAsyncOpen)GetProcAddress(g_hLib,"WFSAsyncOpen");
	if(m_pfnWFSAsyncOpen == 0) g_strResult += _T("WFSAsyncOpen missing.\r\n");
	m_pfnWFSRegister = (pfnWFSRegister)GetProcAddress(g_hLib,"WFSRegister");
	if(m_pfnWFSRegister == 0) g_strResult += _T("WFSRegister missing.\r\n");
	m_pfnWFSAsyncRegister = (pfnWFSAsyncRegister)GetProcAddress(g_hLib,"WFSAsyncRegister");
	if(m_pfnWFSAsyncRegister == 0) g_strResult += _T("WFSAsyncRegister missing.\r\n");
	m_pfnWFSSetBlockingHook = (pfnWFSSetBlockingHook)GetProcAddress(g_hLib,"WFSSetBlockingHook");
	if(m_pfnWFSSetBlockingHook == 0) g_strResult += _T("WFSSetBlockingHook missing.\r\n");
	m_pfnWFSStartUp = (pfnWFSStartUp)GetProcAddress(g_hLib,"WFSStartUp");
	if(m_pfnWFSStartUp == 0) g_strResult += _T("WFSStartUp missing.\r\n");
	m_pfnWFSUnhookBlockingHook = (pfnWFSUnhookBlockingHook)GetProcAddress(g_hLib,"WFSUnhookBlockingHook");
	if(m_pfnWFSUnhookBlockingHook == 0) g_strResult += _T("WFSUnhookBlockingHook missing.\r\n");
	m_pfnWFSUnlock = (pfnWFSUnlock)GetProcAddress(g_hLib,"WFSUnlock");
	if(m_pfnWFSUnlock == 0) g_strResult += _T("WFSUnlock missing.\r\n");
	m_pfnWFSAsyncUnlock = (pfnWFSAsyncUnlock)GetProcAddress(g_hLib,"WFSAsyncUnlock");
	if(m_pfnWFSAsyncUnlock == 0) g_strResult += _T("WFSAsyncUnlock missing.\r\n");
	m_pfnWFMSetTraceLevel = (pfnWFMSetTraceLevel)GetProcAddress(g_hLib,"WFMSetTraceLevel");
	if(m_pfnWFMSetTraceLevel == 0) g_strResult += _T("WFMSetTraceLevel missing.\r\n");


	m_pfnWFMAllocateBuffer= (pfnWFMAllocateBuffer)GetProcAddress(g_hLib,"WFMAllocateBuffer");
	if(m_pfnWFMAllocateBuffer == 0) g_strResult += _T("WFMAllocateBuffer missing.\r\n");
	m_pfnWFMAllocateMore= (pfnWFMAllocateMore)GetProcAddress(g_hLib,"WFMAllocateMore");
	if(m_pfnWFMAllocateMore == 0) g_strResult += _T("WFMAllocateBuffer missing.\r\n");
	m_pfnWFMFreeBuffer= (pfnWFMFreeBuffer)GetProcAddress(g_hLib,"WFMFreeBuffer");
	if(m_pfnWFMFreeBuffer == 0) g_strResult += _T("WFMAllocateMore missing.\r\n");
	m_pfnWFMGetTraceLevel= (pfnWFMGetTraceLevel)GetProcAddress(g_hLib,"WFMGetTraceLevel");
	if(m_pfnWFMGetTraceLevel == 0) g_strResult += _T("WFMAllocateMore missing.\r\n");
	m_pfnWFMKillTimer= (pfnWFMKillTimer)GetProcAddress(g_hLib,"WFMGetTraceLevel");
	if(m_pfnWFMKillTimer == 0) g_strResult += _T("WFMGetTraceLevel missing.\r\n");
	m_pfnWFMOutputTraceData= (pfnWFMOutputTraceData)GetProcAddress(g_hLib,"WFMOutputTraceData");
	if(m_pfnWFMOutputTraceData == 0) g_strResult += _T("WFMOutputTraceData missing.\r\n");
	m_pfnWFMReleaseDLL= (pfnWFMReleaseDLL)GetProcAddress(g_hLib,"WFMReleaseDLL");
	if(m_pfnWFMReleaseDLL == 0) g_strResult += _T("WFMReleaseDLL missing.\r\n");
	m_pfnWFMSetTimer= (pfnWFMSetTimer)GetProcAddress(g_hLib,"WFMSetTimer");
	if(m_pfnWFMSetTimer == 0) g_strResult += _T("WFMSetTimer missing.\r\n");



	if(nLen < g_strResult.GetLength()) 
	{
		FreeLibrary(g_hLib);
		g_hLib = 0;
		return FALSE;
	}
	//AfxMessageBox(strPath);

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
	switch(pServiceBasic->m_dwCommand)
	{
	case WFS_CMD_PTR_PRINT_FORM:
		{
			LPWFSPTRPRINTFORM lpPrintForm = (LPWFSPTRPRINTFORM)lpCmdData;
			if(lpPrintForm)
			{
				LPWFSPTRPRINTFORM pWFSPTRPRINTFORM = 0;
				HRESULT h = m_pfnWFMAllocateBuffer(sizeof(WFSPTRPRINTFORM),WFS_MEM_ZEROINIT,(void **)&pWFSPTRPRINTFORM);
				
				memcpy(pWFSPTRPRINTFORM,lpPrintForm,sizeof(WFSPTRPRINTFORM));
				
				if(lpPrintForm->lpszFormName) 
				{
					m_pfnWFMAllocateMore(strlen(lpPrintForm->lpszFormName)+1, pWFSPTRPRINTFORM, (void **)&pWFSPTRPRINTFORM->lpszFormName);
					memcpy(pWFSPTRPRINTFORM->lpszFormName,lpPrintForm->lpszFormName,strlen(lpPrintForm->lpszFormName));
				}
				if(lpPrintForm->lpszMediaName) 
				{
					m_pfnWFMAllocateMore(strlen(lpPrintForm->lpszMediaName)+1, pWFSPTRPRINTFORM, (void **)&pWFSPTRPRINTFORM->lpszMediaName);
					memcpy(pWFSPTRPRINTFORM->lpszMediaName,lpPrintForm->lpszMediaName,strlen(lpPrintForm->lpszMediaName));
				}
				if(lpPrintForm->lpszFields)
				{
					BYTE *pByte = (BYTE *)lpPrintForm->lpszFields;
					BYTE *pByte0 = pByte;
					while(1)
					{
						if(*pByte == 0)
						{
							pByte ++;
							if(*pByte == 0) break;
						}
						else pByte ++;
					}
					long nLen = pByte-pByte0 + 2;

					m_pfnWFMAllocateMore(nLen, pWFSPTRPRINTFORM, (void **)&pWFSPTRPRINTFORM->lpszFields);
					memcpy(pWFSPTRPRINTFORM->lpszFields,lpPrintForm->lpszFields,nLen);
				}

			}
		}
		break;
	default:
		break;

	}
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
	switch(dwCategory)
	{
	case WFS_INF_PTR_STATUS:
	case WFS_INF_PTR_CAPABILITIES:
	case WFS_INF_PTR_FORM_LIST:
	case WFS_INF_PTR_MEDIA_LIST:
		pServiceBasic->m_lpQueryDetails = 0;
		break;
	case WFS_INF_PTR_QUERY_FORM:  // ULL terminate sz
	case WFS_INF_PTR_QUERY_MEDIA:
		{
			TCHAR *pData = (TCHAR *)lpQueryDetails;
			pServiceBasic->m_lpQueryDetails = calloc(_tcslen((TCHAR *)lpQueryDetails)+1,1);
			memcpy(pServiceBasic->m_lpQueryDetails,lpQueryDetails,_tcslen((TCHAR *)lpQueryDetails));
		}
		break;
	case WFS_INF_PTR_QUERY_FIELD:
		{
			LPWFSPTRQUERYFIELD lpQueryField = (LPWFSPTRQUERYFIELD)lpQueryDetails;
			pServiceBasic->m_strFormName = lpQueryField->lpszFormName;
			if(lpQueryField->lpszFieldName == 0) pServiceBasic->m_strFieldName = "";
			else pServiceBasic->m_strFieldName = lpQueryField->lpszFieldName;
		}
		break;

	}
	
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

// eventregistration

CMapPtrToPtr *LocateServiceMap(HSERVICE hService)
{
	CAutoLock AutoLock(&g_CriticalSection);
	CMapPtrToPtr *pmapEvent = 0;
	BOOL bRet = g_mapService.Lookup((void *)hService,(void *&)pmapEvent);
	if(bRet == FALSE)
	{
		pmapEvent = new CMapPtrToPtr;
		g_mapService.SetAt((void *)hService,(void *)pmapEvent);
		return pmapEvent;
	}
	return pmapEvent;
}

CPtrList *LocateEventWndList(HSERVICE hService, DWORD dwEventClass)
{
	CAutoLock AutoLock(&g_CriticalSection);

	CPtrList *pList = 0;
	HWND hWnd = 0;
	CMapPtrToPtr *pmapEvent = 0;
	BOOL bRet = g_mapService.Lookup((void *)hService,(void *&)pmapEvent);
	if(bRet == FALSE)
	{
		return NULL;
	}
	else
	{
		BOOL bRet = pmapEvent->Lookup((void *)dwEventClass,(void *&)pList);
		if(bRet == FALSE)
		{
			return NULL;
		}
	}
	return pList;
}

BOOL AddListEventWnd(CPtrList *pList, HWND hWnd)
{
	POSITION pos = pList->Find(hWnd);
	if(pos == NULL)
	{
		pList->AddTail(hWnd);
	}
	return TRUE;
}

BOOL  SetEventWnd(HSERVICE hService, DWORD dwEventClass, HWND hWnd)
{
	CAutoLock AutoLock(&g_CriticalSection);
	BOOL bRet = TRUE;

	if(dwEventClass & SERVICE_EVENTS) 
	{
		CPtrList *pList = LocateEventWndList(hService,SERVICE_EVENTS);
		if(pList == 0)
		{
			pList = new CPtrList;
			pList->AddHead((void *)hWnd);
			CMapPtrToPtr *pmapEvent = LocateServiceMap(hService);
			pmapEvent->SetAt((void *)SERVICE_EVENTS,(void *)pList);
		}
		else
		{
			AddListEventWnd(pList,hWnd);
		}
	}
	if(dwEventClass & USER_EVENTS)	
	{
		CPtrList *pList = LocateEventWndList(hService,USER_EVENTS);
		if(pList == 0)
		{
			pList = new CPtrList;
			pList->AddHead((void *)hWnd);
			CMapPtrToPtr *pmapEvent = LocateServiceMap(hService);
			pmapEvent->SetAt((void *)USER_EVENTS,(void *)pList);
		}
		else
		{
			AddListEventWnd(pList,hWnd);
		}
	}
	if(dwEventClass & SYSTEM_EVENTS)
	{
		CPtrList *pList = LocateEventWndList(hService,SYSTEM_EVENTS);
		if(pList == 0)
		{
			pList = new CPtrList;
			pList->AddHead((void *)hWnd);
			CMapPtrToPtr *pmapEvent = LocateServiceMap(hService);
			pmapEvent->SetAt((void *)SYSTEM_EVENTS,(void *)pList);
		}
		else
		{
			AddListEventWnd(pList,hWnd);
		}
	}
	if(dwEventClass & EXECUTE_EVENTS)
	{
		CPtrList *pList = LocateEventWndList(hService,EXECUTE_EVENTS);
		if(pList == 0)
		{
			pList = new CPtrList;
			pList->AddHead((void *)hWnd);
			CMapPtrToPtr *pmapEvent = LocateServiceMap(hService);
			pmapEvent->SetAt((void *)EXECUTE_EVENTS,(void *)pList);
		}
		else
		{
			AddListEventWnd(pList,hWnd);
		}
	}
	return bRet;
}

BOOL RemoveEventWnd(HSERVICE hService, DWORD dwEventClass, HWND hWnd)
{
	CAutoLock AutoLock(&g_CriticalSection);

	CMapPtrToPtr *pmapEvent = 0;
	BOOL bRet = g_mapService.Lookup((void *)hService,(void *&)pmapEvent);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	else
	{
		BOOL bRet = TRUE;
		if(dwEventClass & SERVICE_EVENTS) 
		{
			CPtrList *pList = LocateEventWndList(hService,SERVICE_EVENTS);
			if(pList)
			{
				POSITION pos = pList->Find( hWnd,NULL);
				pList->RemoveAt(pos);
				if(pList->IsEmpty())
				{
					BOOL bRet1 = pmapEvent->RemoveKey((void *)SERVICE_EVENTS);
					delete pList;
				}
			}
		}
		if(dwEventClass & USER_EVENTS)
		{
			CPtrList *pList = LocateEventWndList(hService,USER_EVENTS);
			if(pList)
			{
				POSITION pos = pList->Find( hWnd,NULL);
				pList->RemoveAt(pos);
				if(pList->IsEmpty())
				{
					pmapEvent->RemoveKey((void *)USER_EVENTS);
					delete pList;
				}
			}
		}
		if(dwEventClass & SYSTEM_EVENTS)
		{
			CPtrList *pList = LocateEventWndList(hService,SYSTEM_EVENTS);
			if(pList)
			{
				POSITION pos = pList->Find( hWnd,NULL);
				pList->RemoveAt(pos);
				if(pList->IsEmpty())
				{
					pmapEvent->RemoveKey((void *)SYSTEM_EVENTS);
					delete pList;
				}
			}
		}
		if(dwEventClass & EXECUTE_EVENTS)
		{
			CPtrList *pList = LocateEventWndList(hService,EXECUTE_EVENTS);
			if(pList)
			{
				POSITION pos = pList->Find( hWnd,NULL);
				pList->RemoveAt(pos);
				if(pList->IsEmpty())
				{
					pmapEvent->RemoveKey((void *)EXECUTE_EVENTS);
					delete pList;
				}
			}
		}
	}
	return TRUE;
}

BOOL RemoveServiceMap(HSERVICE hService)
{
	CAutoLock AutoLock(&g_CriticalSection);

	CMapPtrToPtr *pmapEvent = 0;

	BOOL bRet = g_mapService.Lookup((void *)hService,(void *&)pmapEvent); 
	if(bRet)
	{
		POSITION pos = pmapEvent->GetStartPosition();
		while(pos)
		{
			void *key = 0;
			CPtrList *pList = 0;
			pmapEvent->GetNextAssoc(pos ,key,(void *&)pList);
			if(pos == NULL) break;
			delete pList;
		}
		pmapEvent->RemoveAll();
		bRet = g_mapService.RemoveKey((void *)hService);
	}
	return bRet;
}

BOOL PostEventWnd(HSERVICE hService, DWORD dwEventClass,DWORD dwEvent,WPARAM wParam, LPARAM lParam)
{
	CAutoLock AutoLock(&g_CriticalSection);

	CPtrList *pList = LocateEventWndList(hService,dwEventClass);
	if(pList)
	{
		POSITION pos = pList->GetHeadPosition();
		while(pos)
		{
			HWND hWnd =  (HWND)pList->GetNext(pos);
			BOOL b = ::PostMessage(hWnd,dwEvent,wParam,lParam);
			if(b) break;
		}
	}
	return TRUE;
}

// CPTRList