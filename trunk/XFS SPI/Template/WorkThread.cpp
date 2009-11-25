// WorkThread.cpp : implementation file
//

#include "stdafx.h"
#include "$$root$$.h"
#include "helper.h"
#include "ServiceBasic.h"
#include "WorkThread.h"
#include "xfsptr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkThread

extern CCriticalSection g_CriticalSection;

IMPLEMENT_DYNCREATE(CWorkThread, CWinThread)

CWorkThread::CWorkThread()
{

}

CWorkThread::~CWorkThread()
{
}

BOOL CWorkThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CWorkThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWorkThread, CWinThread)
	//{{AFX_MSG_MAP(CWorkThread)
		ON_THREAD_MESSAGE(WM_NI_SP_OPEN, OnSPOpen)
		ON_THREAD_MESSAGE(WM_NI_SP_Execute, OnSPExecute)
		ON_THREAD_MESSAGE(WM_NI_SP_GetInfo, OnSPGetInfo)
		ON_THREAD_MESSAGE(WM_NI_SP_Lock, OnSPGetLock)
		ON_THREAD_MESSAGE(WM_NI_SP_Register, OnSPRegister)
		ON_THREAD_MESSAGE(WM_NI_SP_Unlock, OnSPUnlock)
		ON_THREAD_MESSAGE(WM_NI_SP_Close, OnSPClose)
		ON_THREAD_MESSAGE(WM_NI_SP_Deregister, OnSPDeregister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkThread message handlers

LRESULT CWorkThread::OnSPOpen(WPARAM wParam, LPARAM lParam)
{

	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	// Do session initialization of the device



	// Return the result

	WFSRESULT *pResult ;
	HRESULT hr = ::WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	pResult->hResult = 0;  // indicate the result
	CTime ct= CTime::GetCurrentTime();
	ct.GetAsSystemTime( (SYSTEMTIME&) pResult->tsTimestamp );
	HWND hWnd = pServiceBasic->m_hWND;

	delete pServiceBasic;

	while(1)
	{
		BOOL b = ::PostMessage(hWnd,WFS_OPEN_COMPLETE,NULL,(LONG)pResult);
		if(b) break;
	}
	return 0;
}

LRESULT CWorkThread::OnSPExecute(WPARAM wParam, LPARAM lParam)
{

	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	// Do Execute

	// In case of the class is PTR, do the following

	switch(pServiceBasic->m_dwCommand)
	{
		case    WFS_CMD_PTR_CONTROL_MEDIA		 : 
			// To do
			break;
		case    WFS_CMD_PTR_PRINT_FORM           : 
			// To do
			break;
		case    WFS_CMD_PTR_READ_FORM            :
			// To do
			break;
		case    WFS_CMD_PTR_RAW_DATA             :
			// To do
			break;
		case    WFS_CMD_PTR_MEDIA_EXTENTS        : 
			// To do
			break;
		case    WFS_CMD_PTR_RESET_COUNT          :
			// To do
			break;
		case    WFS_CMD_PTR_READ_IMAGE           :
			// To do
			break;
		case    WFS_CMD_PTR_RESET                :
			// To do
			break;
		case    WFS_CMD_PTR_RETRACT_MEDIA        : 
			// To do
			break;
		case    WFS_CMD_PTR_DISPENSE_PAPER       :
			// To do
			break;
		default:
		break;
	}

	// Return the result

	WFSRESULT *pResult ;
	HRESULT hr = ::WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	pResult->hResult = 0;  // indicate the result
	CTime ct= CTime::GetCurrentTime();
	ct.GetAsSystemTime( (SYSTEMTIME&) pResult->tsTimestamp );
	HWND hWnd = pServiceBasic->m_hWND;

	delete pServiceBasic;

	while(1)
	{
		BOOL b = ::PostMessage(hWnd,WFS_EXECUTE_COMPLETE,NULL,(LONG)pResult);
		if(b) break;
	}
	return 0;
}


LRESULT CWorkThread::OnSPGetInfo(WPARAM wParam, LPARAM lParam)
{

	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	// Do Getinfo


	// Return the result

	WFSRESULT *pResult ;
	HRESULT hr = ::WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	pResult->hResult = 0;  // indicate the result
	CTime ct= CTime::GetCurrentTime();
	ct.GetAsSystemTime( (SYSTEMTIME&) pResult->tsTimestamp );
	HWND hWnd = pServiceBasic->m_hWND;

	delete pServiceBasic;

	while(1)
	{
		BOOL b = ::PostMessage(hWnd,WFS_GETINFO_COMPLETE,NULL,(LONG)pResult);
		if(b) break;
	}
	return 0;
}

LRESULT CWorkThread::OnSPGetLock(WPARAM wParam, LPARAM lParam)
{

	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	// Do GetLock


	// Return the result

	WFSRESULT *pResult ;
	HRESULT hr = ::WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	pResult->hResult = 0;  // indicate the result
	CTime ct= CTime::GetCurrentTime();
	ct.GetAsSystemTime( (SYSTEMTIME&) pResult->tsTimestamp );
	HWND hWnd = pServiceBasic->m_hWND;

	delete pServiceBasic;

	while(1)
	{
		BOOL b = ::PostMessage(hWnd,WFS_LOCK_COMPLETE,NULL,(LONG)pResult);
		if(b) break;
	}
	return 0;
}

LRESULT CWorkThread::OnSPRegister(WPARAM wParam, LPARAM lParam)
{

	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	// Do Register


	// Return the result

	WFSRESULT *pResult ;
	HRESULT hr = ::WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	pResult->hResult = 0;  // indicate the result
	CTime ct= CTime::GetCurrentTime();
	ct.GetAsSystemTime( (SYSTEMTIME&) pResult->tsTimestamp );
	HWND hWnd = pServiceBasic->m_hWND;

	delete pServiceBasic;

	while(1)
	{
		BOOL b = ::PostMessage(hWnd,WFS_REGISTER_COMPLETE,NULL,(LONG)pResult);
		if(b) break;
	}
	return 0;
}

LRESULT CWorkThread::OnSPUnlock(WPARAM wParam, LPARAM lParam)
{

	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	// Do Unlock


	// Return the result

	WFSRESULT *pResult ;
	HRESULT hr = ::WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	pResult->hResult = 0;  // indicate the result
	CTime ct= CTime::GetCurrentTime();
	ct.GetAsSystemTime( (SYSTEMTIME&) pResult->tsTimestamp );
	HWND hWnd = pServiceBasic->m_hWND;

	delete pServiceBasic;

	while(1)
	{
		BOOL b = ::PostMessage(hWnd,WFS_UNLOCK_COMPLETE,NULL,(LONG)pResult);
		if(b) break;
	}
	return 0;
}

LRESULT CWorkThread::OnSPClose(WPARAM wParam, LPARAM lParam)
{

	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	// Do Close


	// Return the result

	WFSRESULT *pResult ;
	HRESULT hr = ::WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	pResult->hResult = 0;  // indicate the result
	CTime ct= CTime::GetCurrentTime();
	ct.GetAsSystemTime( (SYSTEMTIME&) pResult->tsTimestamp );
	HWND hWnd = pServiceBasic->m_hWND;

	delete pServiceBasic;

	while(1)
	{
		BOOL b = ::PostMessage(hWnd,WFS_CLOSE_COMPLETE,NULL,(LONG)pResult);
		if(b) break;
	}
	PostQuitMessage(0);
	return 0;
}

LRESULT CWorkThread::OnSPDeregister(WPARAM wParam, LPARAM lParam)
{

	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	// Do Deregister


	// Return the result

	WFSRESULT *pResult ;
	HRESULT hr = ::WFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	pResult->hResult = 0;  // indicate the result
	CTime ct= CTime::GetCurrentTime();
	ct.GetAsSystemTime( (SYSTEMTIME&) pResult->tsTimestamp );
	HWND hWnd = pServiceBasic->m_hWND;

	delete pServiceBasic;

	while(1)
	{
		BOOL b = ::PostMessage(hWnd,WFS_DEREGISTER_COMPLETE,NULL,(LONG)pResult);
		if(b) break;
	}
	return 0;
}