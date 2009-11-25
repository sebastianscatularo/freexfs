// $$root$$.h : main header file for the $$ROOT$$ DLL
//

#if !defined(AFX_SPI_LQ2090_H__22BF5290_454A_45A5_8F64_F1EF0EC9D4D2__INCLUDED_)
#define AFX_SPI_LQ2090_H__22BF5290_454A_45A5_8F64_F1EF0EC9D4D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "..\..\Manager\NI_XFSAPI.h"
typedef HANDLE HPROVIDER; 
#include "..\..\Manager\NI_XFSManager.h"


/////////////////////////////////////////////////////////////////////////////
// C$$Safe_root$$App
// See $$root$$.cpp for the implementation of this class
//
#define NI_SPI_LQ2090_EXPORTS 

#ifdef NI_SPI_LQ2090_EXPORTS
//#define NI_SPI_LQ2090_API __declspec(dllexport)
#define NI_SPI_LQ2090_API __declspec(dllexport)
#else
#define NI_SPI_LQ2090_API __declspec(dllimport)
#endif

class C$$Safe_root$$App : public CWinApp
{
public:
	C$$Safe_root$$App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C$$Safe_root$$App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(C$$Safe_root$$App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



extern "C"
{
NI_SPI_LQ2090_API HRESULT  WINAPI WFPCancelAsyncRequest ( HSERVICE hService, REQUESTID RequestID);
NI_SPI_LQ2090_API HRESULT  WINAPI WFPClose ( HSERVICE hService, HWND hWnd, REQUESTID ReqID);
NI_SPI_LQ2090_API HRESULT  WINAPI WFPDeregister ( HSERVICE hService, DWORD dwEventClass, HWND hWndReg, HWND hWnd, REQUESTID ReqID);
NI_SPI_LQ2090_API HRESULT  WINAPI WFPExecute ( HSERVICE hService, DWORD dwCommand, LPVOID lpCmdData, DWORD dwTimeOut, HWND hWnd, REQUESTID ReqID);
NI_SPI_LQ2090_API HRESULT  WINAPI WFPGetInfo ( HSERVICE hService, DWORD dwCategory, LPVOID lpQueryDetails, DWORD dwTimeOut, HWND hWnd, REQUESTID ReqID);
NI_SPI_LQ2090_API HRESULT  WINAPI WFPLock ( HSERVICE hService, DWORD dwTimeOut, HWND hWnd, REQUESTID ReqID);
NI_SPI_LQ2090_API HRESULT  WINAPI WFPOpen ( HSERVICE hService, LPSTR lpszLogicalName, 
									 HAPP hApp, LPSTR lpszAppID, 
									 DWORD dwTraceLevel, DWORD dwTimeOut, 
									 HWND hWnd, REQUESTID ReqID, HPROVIDER hProvider, 
									 DWORD dwSPIVersionsRequired, LPWFSVERSION lpSPIVersion, 
									 DWORD dwSrvcVersionsRequired, LPWFSVERSION lpSrvcVersion);
NI_SPI_LQ2090_API HRESULT  WINAPI WFPRegister ( HSERVICE hService,  DWORD dwEventClass, HWND hWndReg, HWND hWnd, REQUESTID ReqID);
NI_SPI_LQ2090_API HRESULT  WINAPI WFPSetTraceLevel ( HSERVICE hService, DWORD dwTraceLevel);
NI_SPI_LQ2090_API HRESULT  WINAPI WFPUnloadService (  );
NI_SPI_LQ2090_API HRESULT  WINAPI WFPUnlock ( HSERVICE hService, HWND hWnd, REQUESTID ReqID);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPI_LQ2090_H__22BF5290_454A_45A5_8F64_F1EF0EC9D4D2__INCLUDED_)
