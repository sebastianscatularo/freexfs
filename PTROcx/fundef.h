#ifndef _FUNDEF_H_
#define _FUNDEF_H_

#ifdef __cplusplus
extern "C" {
#endif
/////

typedef HRESULT   (WINAPI *pfnWFSCancelAsyncRequest) ( HSERVICE hService, REQUESTID RequestID);
typedef HRESULT   (WINAPI *pfnWFSCancelBlockingCall) ( DWORD dwThreadID);
typedef HRESULT   (WINAPI *pfnWFSCleanUp) ();
typedef HRESULT   (WINAPI *pfnWFSClose) ( HSERVICE hService);
typedef HRESULT   (WINAPI *pfnWFSAsyncClose) ( HSERVICE hService, HWND hWnd, LPREQUESTID lpRequestID);
typedef HRESULT   (WINAPI *pfnWFSCreateAppHandle) ( LPHAPP lphApp);
typedef HRESULT   (WINAPI *pfnWFSDeregister) ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg);
typedef HRESULT   (WINAPI *pfnWFSAsyncDeregister) ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, LPREQUESTID lpRequestID);
typedef HRESULT   (WINAPI *pfnWFSDestroyAppHandle) ( HAPP hApp);
typedef HRESULT   (WINAPI *pfnWFSExecute) ( HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, LPWFSRESULT * lppResult);
typedef HRESULT   (WINAPI *pfnWFSAsyncExecute) ( HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID);
typedef HRESULT   (WINAPI *pfnWFSFreeResult) ( LPWFSRESULT lpResult);
typedef HRESULT   (WINAPI *pfnWFSGetInfo) ( HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, LPWFSRESULT * lppResult);
typedef HRESULT   (WINAPI *pfnWFSAsyncGetInfo) ( HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, HWND hWnd, LPREQUESTID lpRequestID);
typedef HRESULT   (WINAPI *pfnWFSIsBlocking) ();
typedef HRESULT   (WINAPI *pfnWFSLock) ( HSERVICE hService, DWORD dwTimeOut , LPWFSRESULT * lppResult);
typedef HRESULT   (WINAPI *pfnWFSAsyncLock) ( HSERVICE hService, DWORD dwTimeOut, HWND hWnd,  LPREQUESTID lpRequestID);
typedef HRESULT   (WINAPI *pfnWFSOpen) ( LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion, LPWFSVERSION lpSPIVersion, LPHSERVICE lphService);
typedef HRESULT   (WINAPI *pfnWFSAsyncOpen) ( LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, LPHSERVICE lphService, HWND hWnd, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion, LPWFSVERSION lpSPIVersion, LPREQUESTID lpRequestID);
typedef HRESULT   (WINAPI *pfnWFSRegister) ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg);
typedef HRESULT   (WINAPI *pfnWFSAsyncRegister) ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, LPREQUESTID lpRequestID);
typedef HRESULT   (WINAPI *pfnWFSSetBlockingHook) ( XFSBLOCKINGHOOK lpBlockFunc, LPXFSBLOCKINGHOOK lppPrevFunc);
typedef HRESULT   (WINAPI *pfnWFSStartUp) ( DWORD dwVersionsRequired, LPWFSVERSION lpWFSVersion);
typedef HRESULT   (WINAPI *pfnWFSUnhookBlockingHook) ();
typedef HRESULT   (WINAPI *pfnWFSUnlock) ( HSERVICE hService);
typedef HRESULT   (WINAPI *pfnWFSAsyncUnlock) ( HSERVICE hService, HWND hWnd, LPREQUESTID lpRequestID);
typedef HRESULT   (WINAPI *pfnWFMSetTraceLevel) ( HSERVICE hService, DWORD dwTraceLevel);


typedef HRESULT (WINAPI *pfnWFMAllocateBuffer)( ULONG ulSize, ULONG ulFlags, LPVOID * lppvData);
typedef HRESULT (WINAPI *pfnWFMAllocateMore)( ULONG ulSize, LPVOID lpvOriginal, LPVOID * lppvData);
typedef HRESULT (WINAPI *pfnWFMFreeBuffer)( LPVOID lpvData);
typedef HRESULT (WINAPI *pfnWFMGetTraceLevel) ( HSERVICE hService, LPDWORD lpdwTraceLevel);
typedef HRESULT (WINAPI *pfnWFMKillTimer)( WORD wTimerID);
typedef HRESULT (WINAPI *pfnWFMOutputTraceData) ( LPSTR lpszData);
typedef HRESULT (WINAPI *pfnWFMReleaseDLL) ( HPROVIDER hProvider);
typedef HRESULT (WINAPI *pfnWFMSetTimer) ( HWND hWnd, LPVOID lpContext, DWORD dwTimeVal, LPWORD lpwTimerID);


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

#ifdef __cplusplus
}       /*extern "C"*/
#endif



#endif;