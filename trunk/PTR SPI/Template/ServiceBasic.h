// ServiceBasic.h: interface for the CServiceBasic class.
//
//////////////////////////////////////////////////////////////////////



#if !defined(AFX_SERVICEBASIC_H__024EDC75_E402_4F3D_9F63_883C143686C2__INCLUDED_)
#define AFX_SERVICEBASIC_H__024EDC75_E402_4F3D_9F63_883C143686C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xfsptr.h"
#include <afxmt.h>
#include "Winspool.h"

class CWorkThread;
class CServiceBasic  
{
public:
	int  SetLogicalServiceLock(CString strLogicalService, long nLock);  // lock is in Registry
	int  QueryLogicalServiceLock(CString strLogicalService);
	int  QueryField(LPWFSFRMFIELD lpFormField,CString strFileName,CString strFieldName);
	int  QueryForm(LPWFSFRMHEADER lpFormHeader,CString strFileName);

	int  GetDevStatus(LPWFSPTRSTATUS lpStatus);
	CStringArray *EnumFormFromRegister(CString strLogicalService);
	CString GetFormFilePath(CString strLogicalService, CString strFormName);
	CString GetPrinterDeviceName(CString strLogicalService);
	void Destroy();
	CWorkThread * GetThread();
	CString FindSPIPath(CString strLogicalService);
	BOOL LoadSPI(CString strPath);
	CServiceBasic();
	virtual ~CServiceBasic();

	CString m_strAppID;
	DWORD m_dwTraceLevel;
	DWORD m_dwTimeOut;
	void *m_pServiceThread;
	HWND m_hWND;
	DWORD m_dwSrvcVersionsRequired;
	LPWFSVERSION m_lpSrvcVersion;
	LPWFSVERSION m_lpSPIVersion;
	LPREQUESTID m_lpRequestID;
	BOOL m_bAutoDeleteRequestID;
	HSERVICE m_hService;
	CString m_strLogicalName;
	DWORD m_dwCommand;
	LPVOID m_lpCmdData;
	DWORD m_dwCategory;
	LPVOID m_lpQueryDetails;

	LPWFSRESULT *m_lppResult;

	HWND m_hWndReg;
	WORD m_dwEventClass;

	CEvent m_eventOpen;
	CEvent m_eventClose;
	CEvent m_eventExecute;
	HRESULT m_hResult;

	HANDLE m_hLib;

	CDC *m_pDC;

};


// CPrintDialog


#endif // !defined(AFX_SERVICEBASIC_H__024EDC75_E402_4F3D_9F63_883C143686C2__INCLUDED_)
