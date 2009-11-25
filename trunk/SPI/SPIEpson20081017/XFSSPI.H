/******************************************************************************
*                                                                             *
* xfsspi.h      XFS - SPI functions, types, and definitions                   *
*                                                                             *
*               Version 3.00  --  10/18/00                                    *
*                                                                             *
******************************************************************************/

#ifndef __inc_xfsspi__h
#define __inc_xfsspi__h

#ifdef __cplusplus
extern "C" {
#endif

#include <xfsapi.h>

typedef HANDLE HPROVIDER; 

#include <xfsconf.h>
#include <xfsadmin.h>

/*   be aware of alignment   */
#pragma pack(push,1)

/****** SPI functions ********************************************************/

//AFX_EXT_API HRESULT extern WINAPI WFPCancelAsyncRequest ( HSERVICE hService, REQUESTID RequestID);
 __declspec(dllexport) HRESULT  WFPCancelAsyncRequest ( HSERVICE hService, REQUESTID RequestID);

 __declspec(dllexport) HRESULT  WFPClose ( HSERVICE hService, HWND hWnd, REQUESTID ReqID);

 __declspec(dllexport) HRESULT  WFPDeregister ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, REQUESTID ReqID);

 __declspec(dllexport) HRESULT  WFPExecute ( HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, HWND hWnd, REQUESTID ReqID);

 __declspec(dllexport) HRESULT  WFPGetInfo ( HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, HWND hWnd, REQUESTID ReqID);

 __declspec(dllexport) HRESULT  WFPLock ( HSERVICE hService, DWORD dwTimeOut, HWND hWnd, REQUESTID ReqID);

 __declspec(dllexport) HRESULT  WFPOpen ( HSERVICE hService, LPSTR lpszLogicalName, HAPP hApp, LPSTR lpszAppID, DWORD dwTraceLevel, DWORD dwTimeOut, HWND hWnd, REQUESTID ReqID, HPROVIDER hProvider, DWORD dwSPIVersionsRequired, LPWFSVERSION lpSPIVersion, DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion);

 __declspec(dllexport) HRESULT  WFPRegister ( HSERVICE hService,  DWORD dwEventClass, HWND hWndReg, HWND hWnd, REQUESTID ReqID);

 __declspec(dllexport) HRESULT  WFPSetTraceLevel ( HSERVICE hService, DWORD dwTraceLevel);

 __declspec(dllexport) HRESULT  WFPUnloadService (  );

 __declspec(dllexport) HRESULT  WFPUnlock ( HSERVICE hService, HWND hWnd, REQUESTID ReqID);


/*   restore alignment   */
#pragma pack(pop)

#ifdef __cplusplus
}    /*extern "C"*/
#endif

#endif  /* __inc_xfsspi__h */

