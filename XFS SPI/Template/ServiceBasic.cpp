// ServiceBasic.cpp: implementation of the CServiceBasic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "$$root$$.h"
#include "WorkThread.h"
#include "ServiceBasic.h"
#include "Xfsconf.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern CWorkThread *g_pDefaultThread;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
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
	m_lpRequestID = 0;
	m_bAutoDeleteRequestID = FALSE;
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
	

}

CServiceBasic::~CServiceBasic()
{

}

BOOL CServiceBasic::LoadSPI(CString strPath)
{
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

CWorkThread * CServiceBasic::GetThread()
{
	return g_pDefaultThread;
}

void CServiceBasic::Destroy()
{
	if(m_bAutoDeleteRequestID && m_lpRequestID) delete m_lpRequestID;
	delete this;
}
