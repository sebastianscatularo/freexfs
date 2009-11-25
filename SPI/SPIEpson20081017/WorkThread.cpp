// WorkThread.cpp : implementation file
//

#include "stdafx.h"
#include "epson2090spi.h"
#include "helper.h"
#include "ServiceBasic.h"
#include "WorkThread.h"
#include "xfsptr.h"
#include "myfile\\printerbase.h"
//#include "myfile\\toolsfunction.h"
#include "myfile\\xfshelper.h"
//#include "myfile\\xfsprinter.h"

#include <comdef.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void NI_MakePath(LPSTR strpath);
static int NI_StrChar(LPSTR str1,int m, unsigned char c);

void NI_MakePath(LPSTR strpath)
{
 char cbuf[1024];
 int i0,i1;
 i0 = i1 = 0;

 while((i1 = NI_StrChar(strpath,i0,0x5c))>=0)
 {
     if(i0 == 0) { i0 = i1+1; continue;}
     memcpy((LPSTR)cbuf,strpath,i1);
     cbuf[i1] = 0;
	 CreateDirectory(cbuf,NULL);
     i0=i1+1;
 }
}

static int NI_StrChar(LPSTR str1,int m, unsigned char c)
{
   int i;
   for(i=m; ;i++)
   {
    if(str1[i] == '\0') return -1;
    if(str1[i] == c)
       return i;
   }
}
/////////////////////////////////////////////////////////////////////////////
// CWorkThread

extern CCriticalSection g_CriticalSection;
extern CWorkThread *g_pDefaultThread;

IMPLEMENT_DYNCREATE(CWorkThread, CWinThread)

CWorkThread::CWorkThread()
{
	m_pDC = NULL;
	m_strLogicalName = "";
	m_nRef = 0;

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

	HRESULT hr = 0;
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	if(pServiceBasic)
	{
		int nLock = pServiceBasic->QueryLogicalServiceLock(pServiceBasic->m_strLogicalName);

		if(nLock <= 0)
		{

			m_strLogicalName = pServiceBasic->m_strLogicalName;
			// Do session initialization of the device
			//CDC dc ;
			//LPCTSTR lpszDeviceName;
			CString strDeviceName = 
				pServiceBasic->GetPrinterDeviceName(pServiceBasic->m_strLogicalName);

			if(strDeviceName == "")
			{
				hr = WFS_ERR_CONNECTION_LOST;
			}
			else
			{
			//create and save cdc 
				if(m_pDC) 
					delete m_pDC;
				m_pDC = new CDC;
				BOOL bRet = m_pDC->CreateDC(0,strDeviceName,0,0);
				if(!bRet)
				{
					if(m_pDC) delete m_pDC;
					m_pDC = 0;
					hr = WFS_ERR_CONNECTION_LOST;
				}
				//else
				else if(0)  // tang, this section seems no need.
				{
					//save logicalname 
					m_strLogicalName = pServiceBasic->m_strLogicalName;

					//set the m_pdc environment

					m_pDC->SetMapMode(MM_ANISOTROPIC);
					CSize size = CSize(800,560);
					m_pDC->SetWindowExt(size);

					int xLogPixPerInch = m_pDC->GetDeviceCaps(LOGPIXELSX);
					int yLogPixPerInch = m_pDC->GetDeviceCaps(LOGPIXELSY);
					
					int xScreenPerInch;
					int yScreenPerInch;
					CDC *pScreenDC = CDC::FromHandle(GetDC(NULL));
					if(pScreenDC)
					{
						xScreenPerInch = pScreenDC->GetDeviceCaps(LOGPIXELSX);
						yScreenPerInch = pScreenDC->GetDeviceCaps(LOGPIXELSY);
					}
					else
					{
						xScreenPerInch = yScreenPerInch = 96;
					}

					long xExt = (long)size.cx * xLogPixPerInch/xScreenPerInch;
					long yExt = (long)size.cy * yLogPixPerInch/yScreenPerInch;
					
					m_pDC->SetViewportExt((int)xExt,(int)yExt);
					//delete pServiceBasic->m_pDC;
				}
				else
				{
					//m_pDC->SetMapMode(MM_ANISOTROPIC);
					m_pDC->SetMapMode(MM_TEXT);
				}
			}
		}
		else 
		{
			hr = WFS_ERR_CANCELED;
		}

	}
	else
	{
		hr = WFS_ERR_INVALID_HSERVICE;
	}
	// Return the result

	WFSRESULT *pResult ;
	m_pfnWFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	
	pResult->hResult = hr;  // indicate the result
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
	HRESULT hr = 0;
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	if(m_pDC == 0)
		hr = WFS_ERR_DEV_NOT_READY;
	else
	{


		CPrinterBase  m_PrinterBase;
		
		// Do Execute

		// In case of the class is PTR, do the following

		switch(pServiceBasic->m_dwCommand)
		{
			case    WFS_CMD_PTR_CONTROL_MEDIA : 
				// To do
				if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLEJECT")==0)
					m_PrinterBase.ExeEpsonCommand((PBYTE)"\x0c",1);//只退单张纸
				else if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLPERFORATE"))//打孔
				{
				}
				else if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLCUT"))//切割
				{}
				else if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLSKIP"))//标记
				{}
				else if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLFLUSH"))//flush
				{
					m_PrinterBase.FlushPrinterData();
				}
				else if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLRETRACT"))//
				{}
				else if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLPARTIALCUT"))//
				{}
				else if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLALARM"))//发出声音
				{
					m_PrinterBase.ExeEpsonCommand((PBYTE)"\x07",1);
				}

				else if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLATPFORWARD"))//换页 向前
				{
					m_PrinterBase.ExeEpsonCommand((PBYTE)"\x0c",1);
				}

				else if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLATPBACKWARD"))//换页 向后
				{}

				else if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLTURNMEDIA"))//插入
				{}

				else if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLSTAMP"))//
				{}
				else if(strcmp((char*)pServiceBasic->m_lpCmdData,"WFS_PTR_CTRLPARK"))//
				{}

				break;
			case    WFS_CMD_PTR_PRINT_FORM           : 
				// To do 
				Proc_CMD_PTR_PRINT_FORM(wParam);
				break;
			case    WFS_CMD_PTR_READ_FORM            :
				// To do
				Proc_CMD_PTR_READ_FORM(wParam);
				break;
			case    WFS_CMD_PTR_RAW_DATA             :
				// To do
				Proc_CMD_PTR_RAW_DATA(wParam);
				break;
			case    WFS_CMD_PTR_MEDIA_EXTENTS        : //get the paper width and height
				// To do
				WFSPTRMEDIAUNIT wfsPtrMediaUnit;
				Proc_CMD_PTR_MEDIA_EXTENTS(wfsPtrMediaUnit.wUnitX,wfsPtrMediaUnit.wUnitY);
				break;
			case    WFS_CMD_PTR_RESET_COUNT          :
				// To do
				Proc_CMD_PTR_RESET_COUNT(wParam);
				break;
			case    WFS_CMD_PTR_READ_IMAGE           :
				// To do
				Proc_CMD_PTR_READ_IMAGE(wParam);
				break;
			case    WFS_CMD_PTR_RESET                :
				// To do
				Proc_CMD_PTR_RESET(wParam);
				break;
			case    WFS_CMD_PTR_RETRACT_MEDIA        : 
				// To do
				Proc_CMD_PTR_RETRACT_MEIDA(wParam);
				break;
			case    WFS_CMD_PTR_DISPENSE_PAPER       :
				// To do
				Proc_CMD_PTR_DISPENSE_PAPER(wParam);
				break;
			default:
			break;
		}
	}

	// Return the result

	WFSRESULT *pResult ;
	m_pfnWFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	pResult->hResult = hr;  // indicate the result
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

	HRESULT hr = 0;
	//if(pServiceBasic->m_pDC == 0)
	if(m_pDC == 0)
	{
		hr = WFS_ERR_DEV_NOT_READY;
		return hr;
	}

	//
	WFSRESULT *pResult ;
	m_pfnWFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	pResult->hResult = hr;  // indicate the result
	CTime ct= CTime::GetCurrentTime();
	ct.GetAsSystemTime( (SYSTEMTIME&) pResult->tsTimestamp );
	HWND hWnd = pServiceBasic->m_hWND;

	// Do Getinfo
	LPWFSPTRSTATUS lpStatus=NULL;
	LPWFSPTRCAPS   lpCaps=NULL;
	char *lpForms = 0;
	LPWFSFRMHEADER lpFormHeader = 0;
	LPWFSFRMFIELD *lppFormFields = 0;

	CPrinterBase m_printerBase;
	if(pServiceBasic->m_dwCategory == WFS_INF_PTR_STATUS)
	{		

		HRESULT h = m_pfnWFMAllocateMore(sizeof(WFSPTRSTATUS),pResult,(void **)&lpStatus);
		pServiceBasic->GetDevStatus(lpStatus, m_strLogicalName);
	}
	else if(pServiceBasic->m_dwCategory == WFS_INF_PTR_CAPABILITIES)
	{
			
		HRESULT h = m_pfnWFMAllocateMore(sizeof(WFSPTRCAPS),pResult,(void **)&lpCaps);
		lpCaps->wClass = WFS_SERVICE_CLASS_PTR;
		lpCaps->fwType = WFS_PTR_TYPEDOCUMENT; //WFS_PTR_TYPERECEIPT;
		lpCaps->bCompound = FALSE;
		int wXResolution,wYResolution;
		m_printerBase.GetPtrinterResolution((WORD&)wXResolution,(WORD&)wYResolution);
		//lpCaps->wResolution = pServiceBasic->m_pDC->GetDeviceCaps(LOGPIXELSX);
		lpCaps->wResolution = m_pDC->GetDeviceCaps(LOGPIXELSX); //wXResolution;
		lpCaps->fwReadForm  = 0; //WFS_PTR_READOCR;
		lpCaps->fwWriteForm = WFS_PTR_WRITETEXT;
		lpCaps->fwExtents   = 0; //WFS_PTR_EXTHORIZONTAL;
		lpCaps->fwControl   = 0; //WFS_PTR_CTRLEJECT;
		lpCaps->usMaxMediaOnStacker = 0;
		lpCaps->bAcceptMedia =TRUE;
		lpCaps->bMultiPage = TRUE;
		lpCaps->fwPaperSources = WFS_PTR_PAPERUPPER;
		lpCaps->bMediaTaken = FALSE;
		lpCaps->usRetractBins = 0; //2;
		lpCaps->fwImageType = 0; //WFS_PTR_IMAGEBMP;
	}
	else if(pServiceBasic->m_dwCategory == WFS_INF_PTR_FORM_LIST)
	{
		HRESULT  h;
		int    cbNumbers = 0;
		CStringArray *pArray = pServiceBasic->EnumFormFromRegister(m_strLogicalName);
		int   nLength;
		cbNumbers = pArray->GetSize();
		if(cbNumbers > 0)
		{
			h = m_pfnWFMAllocateMore(cbNumbers * 200,pResult,(void **)&lpForms);
			nLength = 0;
			for(int i=0;i<cbNumbers;i++)
			{
				if(nLength >0)
					memcpy(lpForms + nLength,pArray->GetAt(i).GetBuffer(0),pArray->GetAt(i).GetLength());
				else
					memcpy(lpForms,pArray->GetAt(i).GetBuffer(0),pArray->GetAt(i).GetLength());
				nLength += pArray->GetAt(i).GetLength() + 1;				
			}   
		}
		if(pArray) delete pArray;
	}
	else if(pServiceBasic->m_dwCategory == WFS_INF_PTR_QUERY_FORM)
	{
		HRESULT h = m_pfnWFMAllocateMore(sizeof(WFSFRMHEADER),pResult,(void **)&lpFormHeader);
		m_pfnWFMAllocateMore(strlen((LPCSTR)pServiceBasic->m_lpQueryDetails)+1,pResult,(void **)&lpFormHeader->lpszFormName);
		memcpy(lpFormHeader->lpszFormName,pServiceBasic->m_lpQueryDetails,strlen((LPCSTR)pServiceBasic->m_lpQueryDetails));
		
		CString strFileName = pServiceBasic->GetFormFilePath(
			m_strLogicalName,lpFormHeader->lpszFormName);
		pServiceBasic->QueryForm(pResult,lpFormHeader, strFileName);
	}
	else if(pServiceBasic->m_dwCategory == WFS_INF_PTR_QUERY_FIELD)
	{
		LPWFSPTRQUERYFIELD pWFSPTRQUERYFIELD = (LPWFSPTRQUERYFIELD)pServiceBasic->m_lpQueryDetails;

		CString strFileName = pServiceBasic->GetFormFilePath(
			m_strLogicalName,pServiceBasic->m_strFormName);
		//lpFormField->lpszFieldName = pWFSPTRQUERYFIELD->lpszFieldName;
		//pServiceBasic->QueryField(lpFormField, strFileName,pWFSPTRQUERYFIELD->lpszFieldName);
		//pServiceBasic->QueryField(&lppFormFields, strFileName,pWFSPTRQUERYFIELD);
		lppFormFields = pServiceBasic->QueryField(pResult,strFileName);
	}

	// Return the result



	if(pServiceBasic->m_dwCategory == WFS_INF_PTR_STATUS)
	    pResult->lpBuffer = lpStatus;
	else if(pServiceBasic->m_dwCategory == WFS_INF_PTR_CAPABILITIES)
		pResult->lpBuffer = lpCaps;
	else if(pServiceBasic->m_dwCategory == WFS_INF_PTR_FORM_LIST)
		pResult->lpBuffer = lpForms;
	else if(pServiceBasic->m_dwCategory == WFS_INF_PTR_QUERY_FORM) pResult->lpBuffer = lpFormHeader;
	else if(pServiceBasic->m_dwCategory == WFS_INF_PTR_QUERY_FIELD) pResult->lpBuffer = lppFormFields;


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

	HRESULT hr = 0;
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	// Do GetLock
	if(pServiceBasic)
	{
		int  nRet = pServiceBasic->SetLogicalServiceLock(m_strLogicalName, 
			1);  // lock is in Registry

	}
	else
	{
		hr = WFS_ERR_INVALID_HSERVICE;
	}


	// Return the result

	WFSRESULT *pResult ;
	m_pfnWFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	pResult->hResult = hr;  // indicate the result
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
// Do register events

	SetEventWnd(pServiceBasic->m_hService, pServiceBasic->m_dwEventClass,
		(HWND)pServiceBasic->m_hWndReg);
	//CMapPtrToPtr *pmapEvent = LocateServiceMap(pServiceBasic->m_hService);
	//pmapEvent->SetAt((void *)pServiceBasic->m_dwEventClass,(void *)pServiceBasic->m_hWndReg);

	// Return the result

	WFSRESULT *pResult ;
	HRESULT hr = m_pfnWFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
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

	HRESULT hr = 0;
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	// Do Unlock
	if(pServiceBasic)
	{
		int  nRet = pServiceBasic->SetLogicalServiceLock(pServiceBasic->m_strLogicalName, 
			0);  // lock is in Registry

	}
	else
	{
		hr = WFS_ERR_INVALID_HSERVICE;
	}

	// Return the result

	WFSRESULT *pResult ;
	m_pfnWFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
	pResult->RequestID = *pServiceBasic->m_lpRequestID;
	pResult->hService = pServiceBasic->m_hService;
	pResult->hResult = hr;  // indicate the result
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
	//if(pServiceBasic->m_pDC) delete pServiceBasic->m_pDC;
	//pServiceBasic->m_pDC = 0;

	if(m_pDC) delete m_pDC;
	m_pDC = 0;

	int  nRet = pServiceBasic->SetLogicalServiceLock(m_strLogicalName, 
			0);  // lock is in Registry

	RemoveServiceMap(pServiceBasic->m_hService);

	// Return the result

	WFSRESULT *pResult ;
	HRESULT hr = m_pfnWFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
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

	InterlockedDecrement(&m_nRef);
	if(m_nRef <= 0)
	{
		//g_pDefaultThread = 0;
		InterlockedExchange(
		  (LPLONG)&g_pDefaultThread, //LPLONG Target,
					   // pointer to the 32-bit value to exchange
		  0   // new value for the LONG value pointed to by Target
		);

		PostQuitMessage(0);
	}

	return 0;
}

LRESULT CWorkThread::OnSPDeregister(WPARAM wParam, LPARAM lParam)
{

	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	// Do Deregister
	RemoveEventWnd(pServiceBasic->m_hService, pServiceBasic->m_dwEventClass,
		pServiceBasic->m_hWndReg);

	// Return the result

	WFSRESULT *pResult ;
	HRESULT hr = m_pfnWFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);
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


#include "xfsform.h"
#include "xfsfield.h"
#include "xfsframe.h"
#include "xfsMedia.h"
LRESULT CWorkThread::Proc_CMD_PTR_PRINT_FORM(WPARAM wParam)
{
	
	CServiceBasic *pServiceBasic = (CServiceBasic *)wParam;

	WFSPTRPRINTFORM *pwfsPrintForm = (WFSPTRPRINTFORM*)pServiceBasic->m_lpCmdData;

	if(pwfsPrintForm == 0 ) return WFS_ERR_PTR_FORMINVALID;
	//wfsPrintForm = *(WFSPTRPRINTFORM*)pServiceBasic->m_lpCmdData;
	//PrinterBase printerBase;

	const LPSTR lpszFormName = pwfsPrintForm->lpszFormName;
	const LPSTR lpszMediaName = pwfsPrintForm->lpszMediaName;

	CString strFileName = pServiceBasic->GetFormFilePath(
		m_strLogicalName,lpszFormName);

	if(strFileName == "")
	{
		return WFS_ERR_PTR_FORMNOTFOUND;
	}
	
// Form preprocessing
	// replace * with form name
	{
		strFileName.Replace("*",lpszFormName);
	}

	BOOL bRemote = FALSE;
	{

		CString strFileSource = "";
	
		CString strFile = strFileName;

		long n = strFile.Find(":");
		if(n == 1)
		{
			bRemote = FALSE;
			strFile.TrimRight();
			strFileSource = strFile;

			char *p = strFileSource.GetBuffer(0);
			for(long i=0; i<strFileSource.GetLength(); i++)
			{
				if(p[i] == '/') p[i] = '\\';
			}
			strFileSource.ReleaseBuffer();
		}
		else
		{
			bRemote = TRUE;
			strFile.TrimRight();
			strFileSource = strFile;

			char *p = strFileSource.GetBuffer(0);
			for(long i=0; i<strFileSource.GetLength(); i++)
			{
				if(p[i] == '\\') p[i] = '/';
			}
			strFileSource.ReleaseBuffer();
		}

		CString strTarget = strFileSource;

		char bufTempPath[2048]={0};
		DWORD nn = ::GetTempPath(
		  2048,  // size, in characters, of the buffer
		  bufTempPath      // pointer to buffer for temp. path
		);


		char buf[2048]={0};
		UINT nRet = ::GetTempFileName(
		  bufTempPath, //LPCTSTR lpPathName,  // pointer to directory name for temporary 
							   // file
		  "NI", //LPCTSTR lpPrefixString,  // pointer to filename prefix
		  0, //UINT uUnique,        // number used to create temporary filename
		  buf 
							   // pointer to buffer that receives the new 
							   // filename
		);

		if(nRet == 0)
		{
			CString m_strTempDir = "";
			char *pUser = getenv( "ProgramFiles");
			if(pUser != 0)
			{
				m_strTempDir = CString(pUser);
			}
			CString strRandom;
			srand(::GetTickCount());
			long n = rand();
			strRandom.Format("%d",n);
			CString strTempFile = m_strTempDir + "\\Temp\\NI" + strRandom;
			strcpy(buf,strTempFile);
		}


		NI_MakePath(buf);

		BOOL bRet = FALSE;
		if(!bRemote)
		{
			bRet = ::CopyFile(strTarget,buf,FALSE);
		}
		else // tang 20050728
		{
			bstr_t url(strTarget.GetBuffer(0));
			bstr_t filepath (buf);
			HRESULT hr = URLDownloadToFile(
			  NULL, //LPUNKNOWN pCaller,  // Caller's controlling IUnknown
			  url, //LPCWSTR szURL,      // Pointer to URL to be downloaded
			  filepath, //LPCTSTR szFileName, // Pointer to file name
			  0, //DWORD dwResv,       // Reserved for future use
			  0 //LPBINDSTATUSCALLBACK lpfnCB
								  // Caller's IBindStatusCallback
			);
			if(hr == 0) bRet = TRUE;
			else 
			{
				::DeleteFile(buf);
				bRet = FALSE;
			}
		}

		if(!bRet) return -1;

		strFileName = buf;
	}



	//char strFileName[2000];
	//memset(strFileName,0,sizeof(strFileName));


	//int nLen;
	//nLen = sizeof(strFileName);
	//GetFormName(lpszFormName,strFileName,nLen);
	//if(strlen(strFileName) < 0 )
	//	return -1;//can't find the form;

	///test ... 
	//CFile cfile;
	//cfile.Open(strFileName,CFile::modeReadWrite | CFile::shareExclusive,NULL);
	//CXFSForm xfsform;
	//CArchive ar(&cfile,CArchive::load);
	//xfsform.Serialize(ar); 
	

	///


	
	//process form

	CXFSForm *pform = 0;
	pform = new CXFSForm;
	CFile cfile;
	BOOL bRet;
	bRet = cfile.Open(strFileName,CFile::modeRead | CFile::shareExclusive);
	if(!bRet) return WFS_ERR_PTR_FORMNOTFOUND;
	CArchive ar(&cfile,CArchive::load);

	try
	{
		while(1)
		{
			pform->Serialize(ar);
			if(pform->m_strFormName == lpszFormName) break;
			delete pform;
			pform = new CXFSForm;
		}
	}
	catch(CFileException *e)
	{
			e->Delete();
			delete pform;
			pform = 0;
	}
	ar.Close();
	cfile.Close();

	if(pform == 0) return WFS_ERR_PTR_FORMNOTFOUND;

//
	if(pwfsPrintForm->wAlignment == WFS_PTR_ALNUSEFORMDEFN){}
	else if(pwfsPrintForm->wAlignment == WFS_PTR_ALNTOPLEFT){
		pform->alignment.m_strAlignment = "TOPLEFT";
	}
	else if(pwfsPrintForm->wAlignment == WFS_PTR_ALNTOPRIGHT){
		pform->alignment.m_strAlignment = "TOPRIGHT";
	}
	else if(pwfsPrintForm->wAlignment == WFS_PTR_ALNBOTTOMLEFT){
		pform->alignment.m_strAlignment = "BOTTOMLEFT";
	}
	else if(pwfsPrintForm->wAlignment == WFS_PTR_ALNBOTTOMRIGHT){
		pform->alignment.m_strAlignment = "BOTTOMRIGHT";
	}

	if(pwfsPrintForm->wOffsetX == WFS_PTR_OFFSETUSEFORMDEFN){}
	else{
		pform->alignment.m_wXoffset = pwfsPrintForm->wOffsetX;
	}

	if(pwfsPrintForm->wOffsetY == WFS_PTR_OFFSETUSEFORMDEFN){}
	else{
		pform->alignment.m_wYoffset = pwfsPrintForm->wOffsetY;
	}

	CXFSMedia *pMedia = 0;
	if(lpszMediaName != 0 && lpszMediaName[0] != 0)
	{
		pMedia = new CXFSMedia;
		bRet = cfile.Open(strFileName,CFile::modeRead | CFile::shareExclusive);
		if(!bRet) return WFS_ERR_PTR_FORMNOTFOUND;
		CArchive ar1(&cfile,CArchive::load);
		pMedia->Serialize(ar1);

		try
		{
			while(1)
			{
				pMedia->Serialize(ar);
				if(pMedia->m_strMediaName == lpszMediaName) break;
				delete pform;
				pMedia = new CXFSMedia;
			}
		}
		catch(CFileException *e)
		{
				e->Delete();
				delete pMedia;
				pMedia = 0;
		}
		ar1.Close();
		cfile.Close();
	}

	//if(bRemote) ::DeleteFile(strFileName);
	if(bRemote) ::DeleteFile(strFileName);

	if(pMedia && pMedia->size.m_wWidth == 0 || pMedia->size.m_wHeight == 0) 
	{
		//delete pMedia;
		//pMedia = 0;
	}

	pform->ExtractFieldValue(pwfsPrintForm->lpszFields);

	HRESULT hr = 0;
	if(pMedia) hr = pform->CheckAgainstMedia(pMedia);
	

	//m_PrinterBase.
	DOCINFO MyDocInfo;
	MyDocInfo.cbSize = sizeof(DOCINFO);
	MyDocInfo.lpszDocName=(LPSTR)"SPI";
	MyDocInfo.lpszOutput=NULL;
	if(m_pDC->StartDoc((DOCINFO far *)&MyDocInfo)>=0)
	{
		m_pDC->StartPage();
		pform->Draw(m_pDC);
		m_pDC->EndPage();
	
		if(m_pDC->EndDoc()<0)
		{
			m_pDC->AbortDoc();

			PostEventWnd(pServiceBasic->m_hService,EXECUTE_EVENTS,
				WFS_EXEE_PTR_NOMEDIA,(DWORD)"Abort Printing", 0);

		}
	}

	delete pform;


	/*
	WFSPTRFORM *pwfsptrform;
	int  nLength ;
	nLength = sizeof(WFSPTRFORM);

	pwfsptrform = new WFSPTRFORM;
	GetData_FormBuffer(pFile,nLen,(char*)pwfsptrform,nLength);

///get the struct of WFSPTRFORM now we deal with with this struct
	if(wfsPrintForm.wAlignment != WFS_PTR_ALNUSEFORMDEFN)
	{       
	
		if(wfsPrintForm.wAlignment == WFS_PTR_ALNTOPLEFT)
		{
			//sprintf(pwfsptrform->alignment_form.alignment,"TOPLEFT");
		}

		else if(wfsPrintForm.wAlignment == WFS_PTR_ALNTOPRIGHT)
		{
			//sprintf(pwfsptrform->alignment_form.alignment,"TOPRIGHT");

		}

		else if(wfsPrintForm.wAlignment == WFS_PTR_ALNBOTTOMLEFT)
		{
			sprintf(pwfsptrform->alignment_form.alignment,"BOTTOMLEFT");
		}

		else if(wfsPrintForm.wAlignment == WFS_PTR_ALNBOTTOMRIGHT)
		{
			sprintf(pwfsptrform->alignment_form.alignment,"BOTTOMRIGHT");
			
		}	

		pwfsptrform->alignment_form.xoffset = wfsPrintForm.wOffsetX;
		pwfsptrform->alignment_form.yoffset = wfsPrintForm.wOffsetY;
	}

	////SET printer resolution
	if(wfsPrintForm.wResolution == WFS_PTR_RESLOW)
	{
		printerBase.SetPrinterResolution(120);
	} 
	else if(wfsPrintForm.wResolution == WFS_PTR_RESMED)
	{
		printerBase.SetPrinterResolution(180);
	}
	else if(wfsPrintForm.wResolution == WFS_PTR_RESHIGH)
	{
		printerBase.SetPrinterResolution(180);
	}

	/////
	////print field here,and each field terminate with two null characters ,every string 
	////is null terminate
	////
	char *pFields;
	pFields = wfsPrintForm.lpszFields;
	///this string is null terminate each field  and two null terminate
//	CXfsPrinter xfsprinter;

//	xfsprinter.printField((char*)pwfsptrform,pFields);


	{///结束处理
		if(pwfsptrform) 
			delete pwfsptrform;
	}

	//when after print ,how to control media ?, to deal this process
	if(wfsPrintForm.dwMediaControl != 0)
	{
	}
	*/

    return hr;
}

LRESULT  CWorkThread::Proc_CMD_PTR_READ_FORM(WPARAM wParam)
{
	return 0;
}

LRESULT  CWorkThread::Proc_CMD_PTR_RAW_DATA(WPARAM wParam)
{

	WFSPTRRAWDATA     wfsRawData = {0};
	WFSPTRRAWDATAIN   wfsRawDataIn = {0};

	CServiceBasic *pServiceBasic = (CServiceBasic*)wParam;
	wfsRawData = *(WFSPTRRAWDATA*)pServiceBasic->m_lpCmdData;


	WFSRESULT *pResult ;
	HRESULT hr = m_pfnWFMAllocateBuffer(sizeof(WFSRESULT), WFS_MEM_ZEROINIT, (void **)&pResult);

	if(wfsRawData.wInputData == WFS_PTR_NOINPUTDATA)
	{
	}
	else if(wfsRawData.wInputData == WFS_PTR_INPUTDATA)
	{
	}
	CPrinterBase pb;
	pb.PrinterRawData(wfsRawData.lpbData,wfsRawData.ulSize);

	return 0;
}
LRESULT  CWorkThread::Proc_CMD_PTR_MEDIA_EXTENTS(WORD &dwPaperWidth,WORD &dwPaperHeight)
{
	CPrinterBase pb;
	
	if(pb.m_hPrinter == NULL)
		return -1;

	
	dwPaperWidth = (short)pb.m_devMode.dmPaperWidth;
	dwPaperWidth = (short)pb.m_devMode.dmPelsHeight;
	return 0;
}

LRESULT   CWorkThread::Proc_CMD_PTR_RESET_COUNT(WPARAM wParam)
{
	return 0;
}
LRESULT	  CWorkThread::Proc_CMD_PTR_READ_IMAGE(WPARAM wParam)
{
	return 0;
}
LRESULT	  CWorkThread::Proc_CMD_PTR_RESET(WPARAM wParam)
{
	CPrinterBase  pb;
	pb.ExeEpsonCommand((PBYTE)"\x1b\x40",2);//initilize printer 
	pb.ExeEpsonCommand((PBYTE)"\x0c",1);//eject paper
	return 0;
}
LRESULT	  CWorkThread::Proc_CMD_PTR_RETRACT_MEIDA(WPARAM wParam)
{
	return 0; 
}
LRESULT	  CWorkThread::Proc_CMD_PTR_DISPENSE_PAPER(WPARAM wParam)
{
	return 0;
}

///help function

