// ServiceBasic.cpp: implementation of the CServiceBasic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NI_XFSMgr.h"
#include "ServiceBasic.h"
#include "ServicThread.h"
#include "Xfsconf.h"
#include "NI_XFSManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CServicThread *g_pDefaultThread;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CObArray g_arrayService;
CServiceBasic::CServiceBasic()
{
	m_strAppID = "";
	m_dwTimeOut = 0;
	m_dwTraceLevel = 0;
	m_pServiceThread = 0;
	m_hWND = 0;
	m_dwSrvcVersionsRequired = 0;
	m_lpSrvcVersion = 0;  // no delete
	m_lpSPIVersion = 0;   // no delete
	//m_lpRequestID = 0;
	//m_bAutoDeleteRequestID = FALSE;
	m_nRequestedID = 0;
	m_hService = 0;
	m_strLogicalName = "";
	m_hResult = 0;

	m_dwCommand = 0;
	m_lpCmdData = 0;
	m_dwCategory = 0;
	m_lpQueryDetails = 0;
	m_lppResult = 0;

	m_hWndReg = 0;
	m_dwEventClass = 0;
	
	m_hLib= 0;
	m_pfnWFPCancelAsyncRequest= 0;
	m_pfnWFPClose= 0;
	m_pfnWFPDeregister= 0;
	m_pfnWFPExecute= 0;
	m_pfnWFPGetInfo= 0;
	m_pfnWFPLock= 0;
	m_pfnWFPOpen= 0;
	m_pfnWFPRegister= 0;
	m_pfnWFPSetTraceLevel= 0;
	m_pfnWFPUnloadService= 0;
	m_pfnWFPUnlock= 0;
}

CServiceBasic::~CServiceBasic()
{

}

BOOL CServiceBasic::LoadSPI(CString strPath)
{
	m_hLib = ::LoadLibrary(strPath);
	if(m_hLib == 0) return FALSE;

	//m_pfnWFPCancelAsyncRequest = (pfnWFPCancelAsyncRequest)GetProcAddress(m_hLib, (LPCSTR)0x0001);

	m_pfnWFPCancelAsyncRequest = (pfnWFPCancelAsyncRequest)GetProcAddress(m_hLib, "WFPCancelAsyncRequest" );
	//m_pfnWFPClose = (pfnWFPClose)GetProcAddress(m_hLib, (LPCSTR)0x0002 );
	m_pfnWFPClose = (pfnWFPClose)GetProcAddress(m_hLib, "WFPClose" );
	m_pfnWFPDeregister = (pfnWFPDeregister)GetProcAddress(m_hLib, "WFPDeregister" );
	m_pfnWFPExecute = (pfnWFPExecute)GetProcAddress(m_hLib, "WFPExecute" );
	m_pfnWFPGetInfo = (pfnWFPGetInfo)GetProcAddress(m_hLib, "WFPGetInfo" );
	m_pfnWFPLock = (pfnWFPLock)GetProcAddress(m_hLib, "WFPLock" );
	m_pfnWFPOpen = (pfnWFPOpen)GetProcAddress(m_hLib, "WFPOpen" );
	m_pfnWFPRegister = (pfnWFPRegister)GetProcAddress(m_hLib, "WFPRegister" );
	m_pfnWFPSetTraceLevel = (pfnWFPSetTraceLevel)GetProcAddress(m_hLib, "WFPSetTraceLevel" );
	m_pfnWFPUnloadService = (pfnWFPUnloadService)GetProcAddress(m_hLib, "WFPUnloadService" );
	m_pfnWFPUnlock = (pfnWFPUnlock)GetProcAddress(m_hLib, "WFPUnlock" );

	return TRUE;
}

CString CServiceBasic::FindSPIPath(CString strLogicalService)
{
	DWORD dwResult = 0;
	HKEY hKeyLogicalService;
	HRESULT hr = WFMCreateKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalService,&dwResult); //phkResult, lpdwDisposition )
		
	if(hr != WFS_SUCCESS) return "";

	HKEY hKeyTarget;
	hr = WFMCreateKey(hKeyLogicalService,
		strLogicalService.GetBuffer(0), //lpszSubKey, 
		&hKeyTarget,&dwResult); //phkResult, lpdwDisposition )
	
	if(hr != WFS_SUCCESS) return "";

	if(dwResult == WFS_CFG_CREATED_NEW_KEY)
	{
		WFMDeleteKey(hKeyLogicalService,strLogicalService.GetBuffer(0));
		return "";
	}

	char buf[2048];
	DWORD len = 2048;
	hr = WFMQueryValue (hKeyTarget,"provider",buf, &len);
	if(hr != WFS_SUCCESS) return "";
	buf[len] = 0;
	CString strProvider = buf;

	HKEY hKeyServiceProviders;
	hr = WFMCreateKey(WFS_CFG_MACHINE_XFS_ROOT,
		"SERVICE_PROVIDERS", //lpszSubKey, 
		&hKeyServiceProviders,&dwResult);
	if(hr != WFS_SUCCESS) return "";

	HKEY hKeyTarget1;
	hr = WFMCreateKey(hKeyServiceProviders,
		strProvider.GetBuffer(0), //lpszSubKey, 
		&hKeyTarget1,&dwResult);
	if(hr != WFS_SUCCESS) return "";
	if(dwResult == WFS_CFG_CREATED_NEW_KEY)
	{
		WFMDeleteKey (hKeyServiceProviders,strProvider.GetBuffer(0));
		return "";
	}

	len = 2048;
	hr = WFMQueryValue (hKeyTarget1,"dllname",buf, &len);
	if(hr != WFS_SUCCESS) return "";
	buf[len] = 0;
	CString strOut = buf;

	return strOut;

}

CServicThread * CServiceBasic::GetThread()
{
	if(m_pServiceThread == WFS_DEFAULT_HAPP) return g_pDefaultThread;
	else return m_pServiceThread;
}

void CServiceBasic::Destroy()
{
	//if(m_hService > 0)	g_arrayService.SetAt(m_hService-1,0);
	WFSDestroyAppHandle(GetThread());
	if(m_hLib) FreeLibrary(m_hLib);
	//if(m_bAutoDeleteRequestID && m_lpRequestID) delete m_lpRequestID;
}
