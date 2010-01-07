// XFSPrint.cpp : Implementation of CXFSPrint

#include "stdafx.h"
#include "PTROcx.h"
#include "XFSPrint.h"
#include "fundef.h"

/////////////////////////////////////////////////////////////////////////////
// CXFSPrint
HRESULT CXFSPrint::FinalConstruct()
{
	m_strLocalService = "";
	m_strVersionSPI = "";
	m_strVersionXFS = _T("00029903");
	m_strXFSPath = "";
	m_strFormName = "";
	m_strMediaName = "";

	m_hLib = 0;
	m_pTextOutExecute = 0;

	m_bStartUp = FALSE;

	return S_OK;
}
void CXFSPrint::FinalRelease()
{
	if(m_hLib != NULL) 
	{
		FreeLibrary(m_hLib);
		m_hLib = 0;
	}
	if(m_pTextOutExecute) free(m_pTextOutExecute); 
}

STDMETHODIMP CXFSPrint::get_XFSMgrPath(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_strXFSPath.AllocSysString();

	return S_OK;
}

STDMETHODIMP CXFSPrint::put_XFSMgrPath(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_strXFSPath = CString((LPCWSTR)newVal);

	return S_OK;
}

STDMETHODIMP CXFSPrint::get_LogicalService(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_strLocalService.AllocSysString();

	return S_OK;
}

STDMETHODIMP CXFSPrint::put_LogicalService(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	m_strLocalService = CString((LPCWSTR)newVal);

	return S_OK;
}

STDMETHODIMP CXFSPrint::get_FormName(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_strFormName.AllocSysString();

	return S_OK;
}

STDMETHODIMP CXFSPrint::put_FormName(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_strFormName = CString((LPCWSTR)newVal);

	return S_OK;
}

STDMETHODIMP CXFSPrint::AddField(BSTR bstrTag, BSTR bstrValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString strTag =  CString((LPCWSTR)bstrTag);
	CString strValue =  CString((LPCWSTR)bstrValue);
	CString strOneField = strTag + "=" + strValue;
	m_arrayFields.Add(strOneField);

	return S_OK;
}

STDMETHODIMP CXFSPrint::DoPrint()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(m_bStartUp == FALSE)
	{
		if(LoadManagerFunction(m_strXFSPath) == FALSE) return S_FALSE;

		DWORD nVersion;
		_stscanf(m_strVersionXFS,_T("%08X"),&nVersion);
		WFSVERSION WFSVersion;
		//HRESULT hr = m_pfnWFSStartUp( 0x00020003, &WFSVersion);
		HRESULT hr = (*m_pfnWFSStartUp)( nVersion, &WFSVersion);
		if(hr == S_OK)  
		{
			CString str;
			str.Format(_T("WFSStartUp OK with version %08X\r\n"), nVersion);
			m_strResult += str;
			m_bStartUp = TRUE;
		}
		else 
		{
			CString str;
			str.Format(_T("WFSStartUp Failed with version %08X\r\n"), nVersion);
			m_strResult += str;
			return S_FALSE;
		}
	}


	HAPP hApp = 0;
	m_pfnWFSCreateAppHandle(&hApp);

	HSERVICE hService = 0;

	WFSVERSION WFSVersion1;
	WFSVERSION WFSVersion2;

	USES_CONVERSION;

    HRESULT hr = (*m_pfnWFSOpen) (T2A(m_strLocalService.GetBuffer(0)), 
        hApp,//WFS_DEFAULT_HAPP, //hApp, //WFS_DEFAULT_HAPP, //HAPP hApp, 
        "MySession", //NULL,  //LPSTR lpszAppID, 
        WFS_TRACE_ALL_API, //NULL, //DWORD dwTraceLevel, 
        WFS_INDEFINITE_WAIT, //DWORD dwTimeOut, 
        0x00020003, //DWORD dwSrvcVersionsRequired, 
        &WFSVersion1, //LPWFSVERSION lpSrvcVersion, 
        &WFSVersion2, //LPWFSVERSION lpSPIVersion, 
        &hService  // LPHSERVICE lphService
        );
	
	if(hr == 0)
	{
		CString str;
		str.Format(_T("OK SyncOpen Service ID = %d SPI version High %04X Low %04X\r\n"),hService,WFSVersion2.wHighVersion,
			WFSVersion2.wLowVersion);
		
		m_strResult += str;
	}
	else
	{
		CString str;
		str.Format(_T("Failed SyncOpen SPI version High %04X Low %04X\r\n"),WFSVersion2.wHighVersion,
			WFSVersion2.wLowVersion);
		m_strResult += str;
		return S_FALSE;
	}

	long nFields = m_arrayFields.GetSize();
	long nCountNeeded = 0;
	for(long i=0; i<nFields; i++)
	{
		CString str = m_arrayFields.GetAt(i);
		nCountNeeded += str.GetLength() + sizeof(TCHAR);
	}
	nCountNeeded += sizeof(TCHAR);


	if(m_pTextOutExecute) free(m_pTextOutExecute); 
	m_pTextOutExecute = (LPTSTR)calloc(nCountNeeded,sizeof(TCHAR));

	{
		long nOffset = 0;
		for(long n = 0; n<nFields; n++)
		{
			CString str = m_arrayFields.GetAt(n);
			memcpy((m_pTextOutExecute+nOffset),str.GetBuffer(0),str.GetLength() * sizeof(TCHAR));
			nOffset += str.GetLength() + sizeof(TCHAR);
		}
	}

	WFSRESULT *pResult = 0;

	/*
	WFSPTRPRINTFORM wfsform;
	memset(&wfsform,0,sizeof(WFSPTRPRINTFORM));
	wfsform.lpszFormName = m_strFormName.GetBuffer(0);
	wfsform.lpszFields = m_pTextOutExecute;
    */

	WFSPTRPRINTFORM *pwfsform = 0;
	m_pfnWFMAllocateBuffer(sizeof(WFSPTRPRINTFORM),
		WFS_MEM_ZEROINIT | WFS_MEM_SHARE, 
		(void **)&pwfsform);

	if(m_strFormName != "")
	{
		m_pfnWFMAllocateMore(m_strFormName.GetLength()+1, pwfsform, (void **)&pwfsform->lpszFormName);
		memcpy(pwfsform->lpszFormName,m_strFormName.GetBuffer(0),m_strFormName.GetLength() * sizeof(TCHAR));
	}
// 20091218
	if(m_strMediaName != "")
	{
		m_pfnWFMAllocateMore(m_strMediaName.GetLength()+1, pwfsform, (void **)&pwfsform->lpszMediaName);
		memcpy(pwfsform->lpszMediaName,m_strMediaName.GetBuffer(0),m_strMediaName.GetLength() * sizeof(TCHAR));
	}
#ifndef _UNICODE
	m_pfnWFMAllocateMore(nCountNeeded, pwfsform, (void **)&pwfsform->lpszFields);
	memcpy(pwfsform->lpszFields, m_pTextOutExecute,nCountNeeded);
#else
	m_pfnWFMAllocateMore(nCountNeeded * sizeof(TCHAR), pwfsform, (void **)&pwfsform->lpszUNICODEFields);
	memcpy(pwfsform->lpszUNICODEFields, m_pTextOutExecute,nCountNeeded  * sizeof(TCHAR));
#endif
	
	pwfsform->wAlignment = WFS_PTR_ALNUSEFORMDEFN;
	pwfsform->wOffsetX = WFS_PTR_OFFSETUSEFORMDEFN;
	pwfsform->wOffsetY = WFS_PTR_OFFSETUSEFORMDEFN;
	pwfsform->wResolution = WFS_PTR_RESHIGH;
#ifdef _V2_
#else
	pwfsform->wPaperSource = WFS_PTR_PAPERANY;
#endif

	pwfsform->dwMediaControl= 0;

	
	(*m_pfnWFSExecute) (hService,WFS_CMD_PTR_PRINT_FORM,
		pwfsform, 10000, &pResult);

	m_pfnWFMFreeBuffer(pwfsform);

	CString str;
	if(pResult) str.Format(_T("Sync Execute Result = %d\r\n"), pResult->hResult);
	m_strResult += str;
    
	if(pResult && pResult->hResult != 0)
	{
		(*m_pfnWFSFreeResult)(pResult);
		return FALSE;
	}

    if(pResult) {
        HRESULT hr = (*m_pfnWFSFreeResult)(pResult);
    }

	// Close
	{
		hr = (*m_pfnWFSClose)(hService);
		CString str;
		str.Format(_T("OK Sync Close Result = %d\r\n"), hr);
		m_strResult += str;
        m_bStartUp = FALSE;
	}

//  hr = m_pfnWFSDestroyAppHandle(hApp);
	hr = (*m_pfnWFSCleanUp)();

	return S_OK;
}

BOOL CXFSPrint::LoadManagerFunction(CString strPath)
{
	if(m_hLib != NULL) return TRUE;
	m_strResult = "";
	m_hLib = LoadLibrary(strPath);
	if(m_hLib == NULL)
	{
		return FALSE;
	}
	else 
	{

	}

	long nLen = m_strResult.GetLength();
	
	m_pfnWFSCancelBlockingCall = (pfnWFSCancelBlockingCall)GetProcAddress(m_hLib,"WFSCancelBlockingCall");
	if(m_pfnWFSCancelBlockingCall == 0) m_strResult += _T("WFSCancelBlockingCall missing.\r\n");
	m_pfnWFSCleanUp = (pfnWFSCleanUp)GetProcAddress(m_hLib,"WFSCleanUp");
	if(m_pfnWFSCleanUp == 0) m_strResult += _T("WFSCleanUp missing.\r\n");
	m_pfnWFSClose = (pfnWFSClose)GetProcAddress(m_hLib,"WFSClose");
	if(m_pfnWFSClose == 0) m_strResult += _T("WFSClose missing.\r\n");
	m_pfnWFSAsyncClose = (pfnWFSAsyncClose)GetProcAddress(m_hLib,"WFSAsyncClose");
	if(m_pfnWFSAsyncClose == 0) m_strResult += _T("WFSAsyncClose missing.\r\n");
	m_pfnWFSCreateAppHandle = (pfnWFSCreateAppHandle)GetProcAddress(m_hLib,"WFSCreateAppHandle");
	if(m_pfnWFSCreateAppHandle == 0) m_strResult += _T("WFSCreateAppHandle missing.\r\n");
	m_pfnWFSDeregister = (pfnWFSDeregister)GetProcAddress(m_hLib,"WFSDeregister");
	if(m_pfnWFSDeregister == 0) m_strResult += _T("WFSDeregister missing.\r\n");
	m_pfnWFSAsyncDeregister = (pfnWFSAsyncDeregister)GetProcAddress(m_hLib,"WFSAsyncDeregister");
	if(m_pfnWFSAsyncDeregister == 0) m_strResult += _T("WFSAsyncDeregister missing.\r\n");
	m_pfnWFSDestroyAppHandle = (pfnWFSDestroyAppHandle)GetProcAddress(m_hLib,"WFSDestroyAppHandle");
	if(m_pfnWFSDestroyAppHandle == 0) m_strResult += _T("WFSDestroyAppHandle missing.\r\n");
	m_pfnWFSExecute = (pfnWFSExecute)GetProcAddress(m_hLib,"WFSExecute");
	if(m_pfnWFSExecute == 0) m_strResult += _T("WFSExecute missing.\r\n");
	m_pfnWFSAsyncExecute = (pfnWFSAsyncExecute)GetProcAddress(m_hLib,"WFSAsyncExecute");
	if(m_pfnWFSAsyncExecute == 0) m_strResult += _T("WFSAsyncExecute missing.\r\n");
	m_pfnWFSFreeResult = (pfnWFSFreeResult)GetProcAddress(m_hLib,"WFSFreeResult");
	if(m_pfnWFSFreeResult == 0) m_strResult += _T("WFSFreeResult missing.\r\n");
	m_pfnWFSGetInfo = (pfnWFSGetInfo)GetProcAddress(m_hLib,"WFSGetInfo");
	if(m_pfnWFSGetInfo == 0) m_strResult += _T("WFSGetInfo missing.\r\n");

	m_pfnWFSAsyncGetInfo = (pfnWFSAsyncGetInfo)GetProcAddress(m_hLib,"WFSAsyncGetInfo");
	if(m_pfnWFSAsyncGetInfo == 0) m_strResult += _T("WFSAsyncGetInfo missing.\r\n");
	m_pfnWFSIsBlocking = (pfnWFSIsBlocking)GetProcAddress(m_hLib,"WFSIsBlocking");
	if(m_pfnWFSIsBlocking == 0) m_strResult += _T("WFSIsBlocking missing.\r\n");
	m_pfnWFSLock = (pfnWFSLock)GetProcAddress(m_hLib,"WFSLock");
	if(m_pfnWFSLock == 0) m_strResult += _T("WFSLock missing.\r\n");
	m_pfnWFSAsyncLock = (pfnWFSAsyncLock)GetProcAddress(m_hLib,"WFSAsyncLock");
	if(m_pfnWFSAsyncLock == 0) m_strResult += _T("WFSAsyncLock missing.\r\n");
	m_pfnWFSOpen = (pfnWFSOpen)GetProcAddress(m_hLib,"WFSOpen");
	if(m_pfnWFSOpen == 0) m_strResult += _T("WFSOpen missing.\r\n");
	m_pfnWFSAsyncOpen = (pfnWFSAsyncOpen)GetProcAddress(m_hLib,"WFSAsyncOpen");
	if(m_pfnWFSAsyncOpen == 0) m_strResult += _T("WFSAsyncOpen missing.\r\n");
	m_pfnWFSRegister = (pfnWFSRegister)GetProcAddress(m_hLib,"WFSRegister");
	if(m_pfnWFSRegister == 0) m_strResult += _T("WFSRegister missing.\r\n");
	m_pfnWFSAsyncRegister = (pfnWFSAsyncRegister)GetProcAddress(m_hLib,"WFSAsyncRegister");
	if(m_pfnWFSAsyncRegister == 0) m_strResult += _T("WFSAsyncRegister missing.\r\n");
	m_pfnWFSSetBlockingHook = (pfnWFSSetBlockingHook)GetProcAddress(m_hLib,"WFSSetBlockingHook");
	if(m_pfnWFSSetBlockingHook == 0) m_strResult += _T("WFSSetBlockingHook missing.\r\n");
	m_pfnWFSStartUp = (pfnWFSStartUp)GetProcAddress(m_hLib,"WFSStartUp");
	if(m_pfnWFSStartUp == 0) m_strResult += _T("WFSStartUp missing.\r\n");
	m_pfnWFSUnhookBlockingHook = (pfnWFSUnhookBlockingHook)GetProcAddress(m_hLib,"WFSUnhookBlockingHook");
	if(m_pfnWFSUnhookBlockingHook == 0) m_strResult += _T("WFSUnhookBlockingHook missing.\r\n");
	m_pfnWFSUnlock = (pfnWFSUnlock)GetProcAddress(m_hLib,"WFSUnlock");
	if(m_pfnWFSUnlock == 0) m_strResult += _T("WFSUnlock missing.\r\n");
	m_pfnWFSAsyncUnlock = (pfnWFSAsyncUnlock)GetProcAddress(m_hLib,"WFSAsyncUnlock");
	if(m_pfnWFSAsyncUnlock == 0) m_strResult += _T("WFSAsyncUnlock missing.\r\n");
	m_pfnWFMSetTraceLevel = (pfnWFMSetTraceLevel)GetProcAddress(m_hLib,"WFMSetTraceLevel");
	if(m_pfnWFMSetTraceLevel == 0) m_strResult += _T("WFMSetTraceLevel missing.\r\n");

	m_pfnWFMAllocateBuffer= (pfnWFMAllocateBuffer)GetProcAddress(m_hLib,"WFMAllocateBuffer");
	if(m_pfnWFMAllocateBuffer == 0) m_strResult += _T("WFMAllocateBuffer missing.\r\n");
	m_pfnWFMAllocateMore= (pfnWFMAllocateMore)GetProcAddress(m_hLib,"WFMAllocateMore");
	if(m_pfnWFMAllocateMore == 0) m_strResult += _T("WFMAllocateBuffer missing.\r\n");
    m_pfnWFMFreeBuffer= (pfnWFMFreeBuffer)GetProcAddress(m_hLib,"WFMFreeBuffer");
	if(m_pfnWFMFreeBuffer == 0) m_strResult += _T("WFMFreeBuffer missing.\r\n");
	m_pfnWFMGetTraceLevel= (pfnWFMGetTraceLevel)GetProcAddress(m_hLib,"WFMGetTraceLevel");
	if(m_pfnWFMGetTraceLevel == 0) m_strResult += _T("WFMAllocateMore missing.\r\n");
	m_pfnWFMKillTimer= (pfnWFMKillTimer)GetProcAddress(m_hLib,"WFMGetTraceLevel");
	if(m_pfnWFMKillTimer == 0) m_strResult += _T("WFMGetTraceLevel missing.\r\n");
	m_pfnWFMOutputTraceData= (pfnWFMOutputTraceData)GetProcAddress(m_hLib,"WFMOutputTraceData");
	if(m_pfnWFMOutputTraceData == 0) m_strResult += _T("WFMOutputTraceData missing.\r\n");
	m_pfnWFMReleaseDLL= (pfnWFMReleaseDLL)GetProcAddress(m_hLib,"WFMReleaseDLL");
	if(m_pfnWFMReleaseDLL == 0) m_strResult += _T("WFMReleaseDLL missing.\r\n");
	m_pfnWFMSetTimer= (pfnWFMSetTimer)GetProcAddress(m_hLib,"WFMSetTimer");
	if(m_pfnWFMSetTimer == 0) m_strResult += _T("WFMSetTimer missing.\r\n");

	if(nLen < m_strResult.GetLength()) 
	{
		FreeLibrary(m_hLib);
		m_hLib = 0;
		return FALSE;
	}

	return TRUE;
}


STDMETHODIMP CXFSPrint::get_Result(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_strResult.AllocSysString();	return S_OK;
}

STDMETHODIMP CXFSPrint::put_Result(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CXFSPrint::get_MediaName(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_strMediaName.AllocSysString();

	return S_OK;
}

STDMETHODIMP CXFSPrint::put_MediaName(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_strMediaName = CString((LPCWSTR)newVal);

	return S_OK;
}
