// epson2090spi.h : main header file for the EPSON2090SPI DLL
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

#include "..\..\Manager\NI_XFSAPI.h"   // You may change this to appropriate path
typedef HANDLE HPROVIDER; 
#include "..\..\Manager\NI_XFSManager.h" // You may change this to appropriate path


/////////////////////////////////////////////////////////////////////////////
// CEpson2090spiApp
// See epson2090spi.cpp for the implementation of this class
//
#define NI_SPI_LQ2090_EXPORTS 

#ifdef NI_SPI_LQ2090_EXPORTS
//#define NI_SPI_LQ2090_API __declspec(dllexport)
#define NI_SPI_LQ2090_API __declspec(dllexport)
#else
#define NI_SPI_LQ2090_API __declspec(dllimport)
#endif

class CEpson2090spiApp : public CWinApp
{
public:
	CEpson2090spiApp();

	int CEpson2090spiApp::ExitInstance();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEpson2090spiApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEpson2090spiApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CMapPtrToPtr * LocateServiceMap(HSERVICE hService);
BOOL AddListEventWnd(CPtrList *pList, HWND hWnd);
CPtrList *LocateEventWndList(HSERVICE hService, DWORD dwEventClass);
BOOL SetEventWnd(HSERVICE hService, DWORD dwEventClass, HWND hWnd);
BOOL RemoveEventWnd(HSERVICE hService, DWORD dwEventClass, HWND hWnd);
BOOL RemoveServiceMap(HSERVICE hService);
BOOL PostEventWnd(HSERVICE hService, DWORD dwEventClass,DWORD dwEvent,WPARAM wParam, LPARAM lParam);



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
