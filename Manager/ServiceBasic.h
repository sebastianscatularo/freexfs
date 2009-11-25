// ServiceBasic.h: interface for the CServiceBasic class.
//
//////////////////////////////////////////////////////////////////////



#if !defined(AFX_SERVICEBASIC_H__024EDC75_E402_4F3D_9F63_883C143686C2__INCLUDED_)
#define AFX_SERVICEBASIC_H__024EDC75_E402_4F3D_9F63_883C143686C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NI_XFSAPI.H"

#define WM_NI_WFSAsyncOpen (WM_USER+1901)
#define WM_NI_WFSOpen (WM_USER+1902)
#define WM_NI_WFSAsyncClose (WM_USER+1903)
#define WM_NI_WFSClose (WM_USER+1904)
#define WM_NI_WFSCancelAsyncRequest (WM_USER+1905)
#define WM_NI_WFSAsyncExecute (WM_USER+1906)
#define WM_NI_WFSExecute (WM_USER+1907)
#define WM_NI_WFSAsyncGetInfo (WM_USER+1908)
#define WM_NI_WFSGetInfo (WM_USER+1909)
#define WM_NI_WFSAsyncRegister (WM_USER+1910)
#define WM_NI_WFSRegister (WM_USER+1911)
#define WM_NI_WFSAsyncDeregister (WM_USER+1912)
#define WM_NI_WFSDeregister (WM_USER+1913)
#define WM_NI_WFSAsyncLock (WM_USER+1914)
#define WM_NI_WFSLock (WM_USER+1915)
#define WM_NI_WFSAsyncUnlock (WM_USER+1916)
#define WM_NI_WFSUnlock (WM_USER+1917)


typedef  HRESULT  (WINAPI *pfnWFPCancelAsyncRequest) ( HSERVICE hService, REQUESTID RequestID);

typedef  HRESULT  (WINAPI *pfnWFPClose) ( HSERVICE hService, HWND hWnd, REQUESTID ReqID);

typedef  HRESULT  (WINAPI *pfnWFPDeregister) ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, REQUESTID ReqID);

typedef  HRESULT  (WINAPI *pfnWFPExecute) ( HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, HWND hWnd, REQUESTID ReqID);

typedef  HRESULT  (WINAPI *pfnWFPGetInfo) ( HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, HWND hWnd, REQUESTID ReqID);

typedef  HRESULT  (WINAPI *pfnWFPLock) ( HSERVICE hService, DWORD dwTimeOut, HWND hWnd, REQUESTID ReqID);

typedef  HRESULT  (WINAPI *pfnWFPOpen) ( HSERVICE hService, LPSTR lpszLogicalName, 
								 HAPP hApp, LPSTR lpszAppID, 
								 DWORD dwTraceLevel, DWORD dwTimeOut, 
								 HWND hWnd, REQUESTID ReqID, 
								 HPROVIDER hProvider, DWORD dwSPIVersionsRequired,
								 LPWFSVERSION lpSPIVersion, DWORD dwSrvcVersionsRequired, 
								 LPWFSVERSION lpSrvcVersion);

typedef  HRESULT  (WINAPI *pfnWFPRegister) ( HSERVICE hService,  DWORD dwEventClass, HWND hWndReg, HWND hWnd, REQUESTID ReqID);

typedef  HRESULT  (WINAPI *pfnWFPSetTraceLevel) ( HSERVICE hService, DWORD dwTraceLevel);

typedef  HRESULT  (WINAPI *pfnWFPUnloadService) (  );

typedef  HRESULT  (WINAPI *pfnWFPUnlock) ( HSERVICE hService, HWND hWnd, REQUESTID ReqID);



class CServicThread;
class CServiceBasic  
{
public:
	void Destroy();
	CServicThread * GetThread();
	CString FindSPIPath(CString strLogicalService);
	BOOL LoadSPI(CString strPath);
	CServiceBasic();
	virtual ~CServiceBasic();

	CString m_strAppID;
	DWORD m_dwTraceLevel;
	DWORD m_dwTimeOut;
	CServicThread *m_pServiceThread;
	HWND m_hWND;
	DWORD m_dwSrvcVersionsRequired;
	LPWFSVERSION m_lpSrvcVersion;
	LPWFSVERSION m_lpSPIVersion;
	//LPREQUESTID m_lpRequestID;
	//BOOL m_bAutoDeleteRequestID;
	REQUESTID m_nRequestedID;
	HSERVICE m_hService;
	CString m_strLogicalName;
	DWORD m_dwCommand;
	LPVOID m_lpCmdData;
	DWORD m_dwCategory;
	LPVOID m_lpQueryDetails;

	LPWFSRESULT *m_lppResult;

	HWND m_hWndReg;
	WORD m_dwEventClass;

	HMODULE m_hLib;
	pfnWFPCancelAsyncRequest m_pfnWFPCancelAsyncRequest;
	pfnWFPClose  m_pfnWFPClose;
	pfnWFPDeregister  m_pfnWFPDeregister;
	pfnWFPExecute  m_pfnWFPExecute;
	pfnWFPGetInfo  m_pfnWFPGetInfo;
	pfnWFPLock  m_pfnWFPLock;
	pfnWFPOpen  m_pfnWFPOpen;
	pfnWFPRegister  m_pfnWFPRegister;
	pfnWFPSetTraceLevel  m_pfnWFPSetTraceLevel;
	pfnWFPUnloadService  m_pfnWFPUnloadService;
	pfnWFPUnlock  m_pfnWFPUnlock;

	CEvent m_eventOpen;
	CEvent m_eventClose;
	CEvent m_eventExecute;
	CEvent m_eventGetInfo;
	CEvent m_eventRegister;
	CEvent m_eventDeregister;
	CEvent m_eventLock;
	CEvent m_eventUnlock;
	HRESULT m_hResult;

};


#endif // !defined(AFX_SERVICEBASIC_H__024EDC75_E402_4F3D_9F63_883C143686C2__INCLUDED_)
