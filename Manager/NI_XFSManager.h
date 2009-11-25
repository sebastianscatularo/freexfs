
#include "NI_XFSAPI.H"
typedef HANDLE HPROVIDER; 

#ifdef NI_XFSMANAGER_EXPORTS
//#define NI_XFSMANAGER_API __declspec(dllexport)
#define NI_XFSMANAGER_API 
#else
#define NI_XFSMANAGER_API __declspec(dllimport)
#endif

//AFX_EXT_API

extern "C"
{

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSCancelAsyncRequest ( HSERVICE hService, REQUESTID RequestID);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSCancelBlockingCall ( DWORD dwThreadID);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSCleanUp ();

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSClose ( HSERVICE hService);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSAsyncClose ( HSERVICE hService, HWND hWnd, LPREQUESTID lpRequestID);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSCreateAppHandle ( LPHAPP lphApp);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSDeregister ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSAsyncDeregister ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, LPREQUESTID lpRequestID);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSDestroyAppHandle ( HAPP hApp);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSExecute ( HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, LPWFSRESULT * lppResult);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSAsyncExecute ( HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSFreeResult ( LPWFSRESULT lpResult);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSGetInfo ( HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, LPWFSRESULT * lppResult);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSAsyncGetInfo ( HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSIsBlocking ();

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSLock ( HSERVICE hService, DWORD dwTimeOut , LPWFSRESULT * lppResult);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSAsyncLock ( HSERVICE hService, DWORD dwTimeOut, HWND hWnd,  LPREQUESTID lpRequestID);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSOpen ( LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion, LPWFSVERSION lpSPIVersion, LPHSERVICE lphService);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSAsyncOpen ( LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, LPHSERVICE lphService, HWND hWnd, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion, LPWFSVERSION lpSPIVersion, LPREQUESTID lpRequestID);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSRegister ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSAsyncRegister ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, LPREQUESTID lpRequestID);

NI_XFSMANAGER_API HRESULT extern WINAPI WFSSetBlockingHook ( XFSBLOCKINGHOOK lpBlockFunc, LPXFSBLOCKINGHOOK lppPrevFunc);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSStartUp ( DWORD dwVersionsRequired, LPWFSVERSION lpWFSVersion);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSUnhookBlockingHook ();

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSUnlock ( HSERVICE hService);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFSAsyncUnlock ( HSERVICE hService, HWND hWnd, LPREQUESTID lpRequestID);

NI_XFSMANAGER_API HRESULT extern WINAPI  WFMSetTraceLevel ( HSERVICE hService, DWORD dwTraceLevel);
};

#define WFS_MEM_SHARE                        0x00000001
#define WFS_MEM_ZEROINIT                     0x00000002

/****** Support Functions ****************************************************/
extern "C"
{
NI_XFSMANAGER_API HRESULT  extern WINAPI WFMAllocateBuffer( ULONG ulSize, ULONG ulFlags, LPVOID * lppvData);

NI_XFSMANAGER_API HRESULT  extern WINAPI WFMAllocateMore( ULONG ulSize, LPVOID lpvOriginal, LPVOID * lppvData);

NI_XFSMANAGER_API HRESULT  extern WINAPI WFMFreeBuffer( LPVOID lpvData);

NI_XFSMANAGER_API HRESULT  extern WINAPI WFMGetTraceLevel ( HSERVICE hService, LPDWORD lpdwTraceLevel);

NI_XFSMANAGER_API HRESULT  extern WINAPI WFMKillTimer( WORD wTimerID);

NI_XFSMANAGER_API HRESULT  extern WINAPI WFMOutputTraceData ( LPSTR lpszData);

NI_XFSMANAGER_API HRESULT  extern WINAPI WFMReleaseDLL ( HPROVIDER hProvider);

NI_XFSMANAGER_API HRESULT  extern WINAPI WFMSetTimer ( HWND hWnd, LPVOID lpContext, DWORD dwTimeVal, LPWORD lpwTimerID);

};

typedef struct mem_block
{
	DWORD m_nLen;
	ULONG m_ulFlags;
	struct mem_block *m_pNext;
	BYTE m_pData[];
} MemBlock;

// AFX_EXPORT_API