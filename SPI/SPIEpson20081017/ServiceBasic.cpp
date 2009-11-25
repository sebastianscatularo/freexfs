// ServiceBasic.cpp: implementation of the CServiceBasic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "epson2090spi.h"
#include "helper.h"
#include "WorkThread.h"
#include "ServiceBasic.h"
#include "Xfsconf.h"

#include "XFSForm.h"
#include "XFSField.h"
#include "XFSFrame.h"
#include "XFSSubform.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern CWorkThread *g_pDefaultThread;
extern DWORD g_dwTraceLevel;
extern CCriticalSection g_CriticalSection;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CServiceBasic::CServiceBasic()
{
	m_strAppID = "";
	m_dwTimeOut = 0;
	m_dwTraceLevel = 0;
	m_pServiceThread = 0;
	m_hWND = 0;
	m_dwSrvcVersionsRequired = 0;
	m_lpSrvcVersion = 0;  // no delete
	m_lpSPIVersion = 0;   // no delete
	m_lpRequestID = 0;
	m_bAutoDeleteRequestID = FALSE;
	m_hService = 0;
	m_strLogicalName = "";
	m_hResult = 0;

	m_dwCommand = 0;
	m_lpCmdData = 0;
	m_dwCategory = 0;
	m_lpQueryDetails = 0;
	m_lppResult = 0;

	m_hWndReg = 0;
	m_dwEventClass = 0;

	m_hLib= 0;
	m_pDC = 0;

	m_strFormName = "";
	m_strFieldName = "";

}

CServiceBasic::~CServiceBasic()
{
	if(m_bAutoDeleteRequestID && m_lpRequestID)  delete m_lpRequestID;
	if(m_lpQueryDetails) free(m_lpQueryDetails);
}

BOOL CServiceBasic::LoadSPI(CString strPath)
{
	return TRUE;
}

CString CServiceBasic::FindSPIPath(CString strLogicalService)
{
	CAutoLock AutoLock(&g_CriticalSection);

	DWORD dwResult = 0;
	HKEY hKeyLogicalService;
	HRESULT hr = WFMCreateKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalService,&dwResult); //phkResult, lpdwDisposition )
		
	if(hr != WFS_SUCCESS) return "";

	if(dwResult == WFS_CFG_CREATED_NEW_KEY)
	{
		WFMDeleteKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,"LOGICAL_SERVICES");
		return "";
	}

	HKEY hKeyTarget;
	hr = WFMCreateKey(hKeyLogicalService,
		strLogicalService.GetBuffer(0), //lpszSubKey, 
		&hKeyTarget,&dwResult); //phkResult, lpdwDisposition )
	
	if(hr != WFS_SUCCESS) return "";

	if(dwResult == WFS_CFG_CREATED_NEW_KEY)
	{
		WFMDeleteKey(hKeyLogicalService,strLogicalService.GetBuffer(0));
		WFMCloseKey (hKeyLogicalService);
		return "";
	}

	char buf[2048];
	DWORD len = 2048;
	hr = WFMQueryValue (hKeyTarget,"provider",buf, &len);
	if(hr != WFS_SUCCESS) return "";
	buf[len] = 0;
	CString strProvider = buf;
	WFMCloseKey (hKeyTarget);

	HKEY hKeyServiceProviders;
	hr = WFMCreateKey(WFS_CFG_MACHINE_XFS_ROOT,
		"SERVICE_PROVIDERS", //lpszSubKey, 
		&hKeyServiceProviders,&dwResult);
	if(hr != WFS_SUCCESS) return "";

	if(dwResult == WFS_CFG_CREATED_NEW_KEY)
	{
		WFMDeleteKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,"SERVICE_PROVIDERS");
		return "";
	}


	HKEY hKeyTarget1;
	hr = WFMCreateKey(hKeyServiceProviders,
		strProvider.GetBuffer(0), //lpszSubKey, 
		&hKeyTarget1,&dwResult);
	if(hr != WFS_SUCCESS) return "";
	if(dwResult == WFS_CFG_CREATED_NEW_KEY)
	{
		WFMDeleteKey (hKeyServiceProviders,strProvider.GetBuffer(0));
		WFMCloseKey (hKeyServiceProviders);
		return "";
	}

	len = 2048;
	hr = WFMQueryValue (hKeyTarget1,"dllname",buf, &len);
	if(hr != WFS_SUCCESS) return "";
	buf[len] = 0;
	CString strOut = buf;

	return strOut;

}

CWorkThread * CServiceBasic::GetThread()
{
	return g_pDefaultThread;
}

void CServiceBasic::Destroy()
{
	if(m_bAutoDeleteRequestID && m_lpRequestID) delete m_lpRequestID;
	delete this;
}

CString CServiceBasic::GetPrinterDeviceName(CString strLogicalService)
{
	CAutoLock AutoLock(&g_CriticalSection);

	DWORD dwResult = 0;
	HKEY hKeyLogicalService;
	HRESULT hr = WFMCreateKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalService,&dwResult); //phkResult, lpdwDisposition )
		
	if(hr != WFS_SUCCESS) return "";
	if(dwResult == WFS_CFG_CREATED_NEW_KEY)
	{
		WFMDeleteKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,"LOGICAL_SERVICES");
		return "";
	}

	HKEY hKeyTarget;
	hr = WFMCreateKey(hKeyLogicalService,
		strLogicalService.GetBuffer(0), //lpszSubKey, 
		&hKeyTarget,&dwResult); //phkResult, lpdwDisposition )
	
	if(hr != WFS_SUCCESS) return "";

	if(dwResult == WFS_CFG_CREATED_NEW_KEY)
	{
		WFMDeleteKey(hKeyLogicalService,strLogicalService.GetBuffer(0));
		WFMCloseKey (hKeyLogicalService);
		return "";
	}

	char buf[2048];
	DWORD len = 2048;
	hr = WFMQueryValue (hKeyTarget,"DeviceName",buf, &len);
	if(hr != WFS_SUCCESS) return "";
	buf[len] = 0;
	CString strDeviceName = buf;

	WFMCloseKey (hKeyTarget);
	WFMCloseKey (hKeyLogicalService);


	return strDeviceName;
}

CString CServiceBasic::GetFormFilePath(CString strLogicalService, CString strFormName)
{
	CAutoLock AutoLock(&g_CriticalSection);

	DWORD dwResult = 0;
	HKEY hKeyLogicalService;
	HRESULT hr = WFMCreateKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalService,&dwResult); //phkResult, lpdwDisposition )
		
	if(hr != WFS_SUCCESS) return "";
	if(dwResult == WFS_CFG_CREATED_NEW_KEY)
	{
		WFMDeleteKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,"LOGICAL_SERVICES");
		return "";
	}

	HKEY hKeyTarget;
	hr = WFMCreateKey(hKeyLogicalService,
		strLogicalService.GetBuffer(0), //lpszSubKey, 
		&hKeyTarget,&dwResult); //phkResult, lpdwDisposition )
	
	if(hr != WFS_SUCCESS) return "";

	if(dwResult == WFS_CFG_CREATED_NEW_KEY)
	{
		WFMDeleteKey(hKeyLogicalService,strLogicalService.GetBuffer(0));
		WFMCloseKey (hKeyLogicalService);
		return "";
	}

	HKEY hKeyTarget1;
	hr = WFMCreateKey(hKeyTarget,
		"Forms", //lpszSubKey, 
		&hKeyTarget1,&dwResult); //phkResult, lpdwDisposition )
	
	if(hr != WFS_SUCCESS) return "";

	if(dwResult == WFS_CFG_CREATED_NEW_KEY)
	{
		WFMDeleteKey(hKeyTarget,"Forms");
		WFMCloseKey (hKeyTarget);
		return "";
	}

	char buf[2048];
	DWORD len = 2048;
	hr = WFMQueryValue (hKeyTarget1,strFormName.GetBuffer(0),buf, &len);
	if(hr != WFS_SUCCESS) return "";
	buf[len] = 0;
	CString strFormFilePath = buf;

	WFMCloseKey (hKeyTarget1);
	WFMCloseKey (hKeyTarget);
	WFMCloseKey (hKeyLogicalService);


	return strFormFilePath;
}

CStringArray *CServiceBasic::EnumFormFromRegister(CString strLogicalService)
{
	CAutoLock AutoLock(&g_CriticalSection);

	DWORD dwResult = 0;
	HKEY hKeyLogicalService;
	HRESULT hr = WFMOpenKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalService); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return NULL; 

	HKEY hKeyTarget;
	hr = WFMOpenKey(hKeyLogicalService,
		strLogicalService.GetBuffer(0), //lpszSubKey, 
		&hKeyTarget); //phkResult, lpdwDisposition )
	
	if(hr != WFS_SUCCESS) return NULL; 

	HKEY hKeyForms;
	hr = WFMOpenKey(hKeyTarget,
		"Forms", //lpszSubKey, 
		&hKeyForms); //phkResult, lpdwDisposition )
	
	if(hr != WFS_SUCCESS) return NULL; 


	char bufValue[2048];
	char bufData[2048];

	DWORD iValue = 0;
	CStringArray *pArray = 0;
	while(1)
	{
		DWORD chValue = 2048;
		DWORD chData = 2048;

		hr = WFMEnumValue(hKeyForms,iValue,bufValue, &chValue,bufData,&chData);
		if(hr == WFS_ERR_CFG_NO_MORE_ITEMS) break ;
		iValue ++;
		CString strOneForm(bufValue,chValue);
		if(pArray == 0) pArray = new CStringArray;
		pArray->Add(strOneForm);
	}

	WFMCloseKey (hKeyForms);
	WFMCloseKey (hKeyTarget);
	WFMCloseKey (hKeyLogicalService);

	return pArray;
}


int  CServiceBasic::GetDevStatus(LPWFSPTRSTATUS lpStatus, CString strLogicalName)
{
	CAutoLock AutoLock(&g_CriticalSection);

	int nRet = 0;
	CString strDeviceName = GetPrinterDeviceName(strLogicalName);
	HANDLE hPrinter = 0;
	BOOL bOp = OpenPrinter(
	//"EPSON LQ-2090C ESC/P2", //LPTSTR pPrinterName, // pointer to printer or server name
	strDeviceName.GetBuffer(0),
	&hPrinter , //LPHANDLE phPrinter,  // pointer to printer or server handle
	NULL //&printerDefaults //LPPRINTER_DEFAULTS pDefault 
					   // pointer to printer defaults structure
	);
	if(bOp)
	{

		PRINTER_INFO_2 *pInfo2 = NULL;
		DWORD          cbBuf;
		DWORD          cbNeeded;
		DWORD          level;
		BOOL           bOk(FALSE);

		level = 2;
		cbBuf = sizeof(PRINTER_INFO_2);
		GetPrinter(hPrinter,
			level,
			NULL,
			cbBuf,
			&cbNeeded);

		pInfo2 = (PRINTER_INFO_2*)calloc(cbNeeded,1);
		if(pInfo2 == NULL)
		{
			ClosePrinter(hPrinter);
			nRet = WFS_ERR_INTERNAL_ERROR;
			return nRet;
		}
		cbBuf = cbNeeded;
		bOk = GetPrinter(hPrinter,
			level,
			(PBYTE)pInfo2,
			cbBuf,
			&cbNeeded);

		/*
		if(pInfo2->Attributes & PRINTER_ATTRIBUTE_WORK_OFFLINE)
		{
			free(pInfo2);
			nRet = WFS_PTR_DEVOFFLINE;
		}
		else 
		{
			free(pInfo2);
			nRet = WFS_PTR_DEVONLINE;
		}
		*/

		///WFS_PTR_DEVONLINE
		//WFS_PTR_DEVOFFLINE
		//WFS_PTR_DEVPOWEROFF

		//WFS_PTR_DEVNODEVICE
		//WFS_PTR_DEVHWERROR
		//WFS_PTR_DEVUSERERROR
		//WFS_PTR_DEVBUSY

		lpStatus->fwDevice = WFS_PTR_DEVONLINE;
		lpStatus->fwMedia = WFS_PTR_MEDIAUNKNOWN;
		lpStatus->fwToner = WFS_PTR_TONERFULL;
		lpStatus->fwInk = WFS_PTR_INKNOTSUPP;
		lpStatus->fwLamp = WFS_PTR_LAMPNOTSUPP;
		lpStatus->lppRetractBins = 0;

		if(pInfo2->Status & PRINTER_STATUS_BUSY) lpStatus->fwDevice = WFS_PTR_DEVBUSY;
		if(pInfo2->Status & PRINTER_STATUS_DOOR_OPEN) lpStatus->fwDevice = WFS_PTR_DEVUSERERROR;
		if(pInfo2->Status & PRINTER_STATUS_ERROR) lpStatus->fwDevice = WFS_PTR_DEVHWERROR;
		if(pInfo2->Status & PRINTER_STATUS_INITIALIZING) lpStatus->fwDevice = WFS_PTR_DEVBUSY;
		if(pInfo2->Status & PRINTER_STATUS_IO_ACTIVE) lpStatus->fwDevice = WFS_PTR_DEVONLINE;
		//if(pInfo2->Status & PRINTER_STATUS_MANUAL_FEED) nRet = WFS_PTR_DEVBUSY;
		if(pInfo2->Status & PRINTER_STATUS_NO_TONER) lpStatus->fwToner = WFS_PTR_TONEROUT;
		if(pInfo2->Status & PRINTER_STATUS_NOT_AVAILABLE) lpStatus->fwDevice = WFS_PTR_DEVNODEVICE;
		if(pInfo2->Status & PRINTER_STATUS_OFFLINE) lpStatus->fwDevice = WFS_PTR_DEVOFFLINE;
		//if(pInfo2->Status & PRINTER_STATUS_OUT_OF_MEMORY) nRet = WFS_PTR_DEVBUSY;
		//if(pInfo2->Status & PRINTER_STATUS_OUTPUT_BIN_FULL) nRet = WFS_PTR_DEVBUSY;
		//if(pInfo2->Status & PRINTER_STATUS_PAGE_PUNT) nRet = WFS_PTR_DEVBUSY;
		if(pInfo2->Status & PRINTER_STATUS_PAPER_JAM) lpStatus->fwPaper[WFS_PTR_SUPPLYUPPER] = WFS_PTR_MEDIAJAMMED;
		if(pInfo2->Status & PRINTER_STATUS_PAPER_OUT) lpStatus->fwPaper[WFS_PTR_SUPPLYUPPER] = WFS_PTR_PAPEROUT;
		if(pInfo2->Status & PRINTER_STATUS_PAPER_PROBLEM) lpStatus->fwPaper[WFS_PTR_SUPPLYUPPER] = WFS_PTR_PAPERUNKNOWN;
		//if(pInfo2->Status & PRINTER_STATUS_PAUSED) nRet = WFS_PTR_DEVBUSY;
		//if(pInfo2->Status & PRINTER_STATUS_PENDING_DELETION) nRet = WFS_PTR_DEVBUSY;
		if(pInfo2->Status & PRINTER_STATUS_PRINTING) lpStatus->fwDevice = WFS_PTR_DEVBUSY;
		//if(pInfo2->Status & PRINTER_STATUS_PROCESSING) nRet = WFS_PTR_DEVBUSY;
		if(pInfo2->Status & PRINTER_STATUS_TONER_LOW) lpStatus->fwToner = WFS_PTR_TONERLOW;
		//if(pInfo2->Status & PRINTER_STATUS_USER_INTERVENTION) nRet = WFS_PTR_DEVBUSY;
		//if(pInfo2->Status & PRINTER_STATUS_WAITING) nRet = WFS_PTR_DEVBUSY;
		//if(pInfo2->Status & PRINTER_STATUS_WARMING_UP) nRet = WFS_PTR_DEVBUSY; 


		free(pInfo2);
		ClosePrinter(hPrinter);
	}
	return nRet;
}

int  CServiceBasic::QueryForm(WFSRESULT *pResult,LPWFSFRMHEADER lpFormHeader,CString strFileName)
{
	CAutoLock AutoLock(&g_CriticalSection);

	int nRet = 0;

	if(strFileName == "")
	{
		return WFS_ERR_PTR_FORMNOTFOUND;
	}
	//process form

	CXFSForm *pform = 0;
	pform = new CXFSForm;
	CFile cfile;
	BOOL bRet;
	bRet = cfile.Open(strFileName,CFile::modeRead | CFile::shareExclusive);
	if(!bRet) return WFS_ERR_PTR_FORMNOTFOUND;
	CArchive ar(&cfile,CArchive::load);
	//pform->Serialize(ar);
	//ar.Close();
	//cfile.Close();

	try
	{
		while(1)
		{
			pform->Serialize(ar);
			if(pform->m_strFormName == lpFormHeader->lpszFormName) break;
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



	if(pform->unit.m_strBase == "INCH") lpFormHeader->wBase = WFS_FRM_INCH;
	if(pform->unit.m_strBase == "MM") lpFormHeader->wBase = WFS_FRM_MM;
	if(pform->unit.m_strBase == "ROWCOLUMN") lpFormHeader->wBase = WFS_FRM_ROWCOLUMN;
	
	lpFormHeader->wUnitX = pform->unit.m_wX;
	lpFormHeader->wUnitY = pform->unit.m_wX;
	lpFormHeader->wWidth = pform->size.m_wWidth;
	lpFormHeader->wHeight = pform->size.m_wHeight;

	if(pform->alignment.m_strAlignment == "TOPLEFT") lpFormHeader->wAlignment = WFS_FRM_TOPLEFT;
	if(pform->alignment.m_strAlignment == "TOPRIGHT") lpFormHeader->wAlignment = WFS_FRM_TOPRIGHT;
	if(pform->alignment.m_strAlignment == "BOTTOMLEFT") lpFormHeader->wAlignment = WFS_FRM_BOTTOMLEFT;
	if(pform->alignment.m_strAlignment == "BOTTOMRIGHT") lpFormHeader->wAlignment = WFS_FRM_BOTTOMRIGHT;

// tp do	
	
	if(pform->m_strOrientation == "PORTRAIT")
		lpFormHeader->wOffsetX = WFS_FRM_PORTRAIT;
	else 
		lpFormHeader->wOffsetY = WFS_FRM_LANDSCAPE;
	//lpFormHeader->wOrientation    = pform->m_wOrientation;
	lpFormHeader->wOffsetX        = pform ->alignment.m_wXoffset;
	lpFormHeader->wOffsetY		  = pform->alignment.m_wYoffset;
	lpFormHeader->wVersionMajor   = pform->version.m_wMajor;
	lpFormHeader->wVersionMinor   = pform->version.m_wMinor;

	m_pfnWFMAllocateMore(pform->m_strUserPrompt.GetLength()+1,lpFormHeader, (void **)&lpFormHeader->lpszUserPrompt);
	memcpy(lpFormHeader->lpszUserPrompt,pform->m_strUserPrompt.GetBuffer(0),pform->m_strUserPrompt.GetLength());
	
	lpFormHeader->fwCharSupport   = WFS_PTR_ASCII;

	/*get field name*/
	POSITION pos;
	
	pos = pform->m_listFields.GetHeadPosition();
	int nLength;
	nLength = pform->dwField;
	char *pTemp;
	nLength = 200 * pform->m_listFields.GetCount();
	//m_pfnWFMAllocateBuffer(nLength, WFS_MEM_ZEROINIT, (void **)&pTemp);
	m_pfnWFMAllocateMore(nLength, pResult, (void **)&pTemp);
	if(pTemp == NULL) return WFS_ERR_INTERNAL_ERROR;
	int  nIndex = 0;
	while(pos)
	{
		int nTemp;
		CXFSField *pfield;
		pfield = (CXFSField*)pform->m_listFields.GetNext(pos);
		memcpy(pTemp+nIndex,pfield->m_strFieldName.GetBuffer(0),nTemp = pfield->m_strFieldName.GetLength());		
		nIndex += nTemp;
		nIndex ++;//nul terminate
	}
	lpFormHeader->lpszFields = pTemp;
	//FormHeader->lpszFields = pTemp;

	delete pform;

	return nRet;
}

LPWFSFRMFIELD * CServiceBasic::QueryField(WFSRESULT *pResult,CString strFileName)
{
	CAutoLock AutoLock(&g_CriticalSection);

	LPWFSFRMFIELD *pWFSFRMFIELD = 0;

	int nRet = 0;

	if(strFileName == "")
	{
		return NULL;
	}
	//process form

	CXFSForm *pform = 0;
	pform = new CXFSForm;
	CFile cfile;
	BOOL bRet;
	bRet = cfile.Open(strFileName,CFile::modeRead | CFile::shareExclusive);
	if(!bRet) return NULL;
	CArchive ar(&cfile,CArchive::load);
	pform->Serialize(ar);
	ar.Close();
	cfile.Close();

	if(m_strFieldName != "")
	{
		HRESULT h = m_pfnWFMAllocateMore(sizeof(LPWFSFRMFIELD) * 2,pResult,(void **)&pWFSFRMFIELD);
		LPWFSFRMFIELD lpFormField = 0;
		h = m_pfnWFMAllocateMore(sizeof(WFSFRMFIELD),pResult,(void **)&lpFormField);
		pWFSFRMFIELD[0] = lpFormField;
		CXFSField *pField = pform->GetFieldByName(m_strFieldName);
		QueryOneField(pResult,lpFormField,strFileName,pField);
	}
	else
	{
		long nFields = pform->m_listFields.GetCount();
		POSITION pos = pform->m_listSubforms.GetHeadPosition();
		while(pos)
		{
			CXFSSubform *pXFSSubform = (CXFSSubform *)pform->m_listSubforms.GetNext(pos);
			nFields += pXFSSubform->m_listFields.GetCount();
		}

		HRESULT h = m_pfnWFMAllocateMore(sizeof(LPWFSFRMFIELD) * (nFields+1),
			pResult,(void **)&pWFSFRMFIELD);

		long nFieldIndex = 0;
		pos = pform->m_listFields.GetHeadPosition();
		while(pos)
		{
			CXFSField *pXFSField = (CXFSField *)pform->m_listFields.GetNext(pos);
			LPWFSFRMFIELD lpFormField = 0;
			h = m_pfnWFMAllocateMore(sizeof(WFSFRMFIELD),pResult,(void **)&lpFormField);
			pWFSFRMFIELD[nFieldIndex ++] = lpFormField;
			QueryOneField(pResult,lpFormField,strFileName,pXFSField);
		}

		pos = pform->m_listSubforms.GetHeadPosition();
		while(pos)
		{
			CXFSSubform *pXFSSubform = (CXFSSubform *)pform->m_listSubforms.GetNext(pos);
			POSITION pos1 = pXFSSubform->m_listFields.GetHeadPosition();
			while(pos1)
			{
				CXFSField *pXFSField = (CXFSField *)pXFSSubform->m_listFields.GetNext(pos1);
				LPWFSFRMFIELD lpFormField = 0;
				h = m_pfnWFMAllocateMore(sizeof(WFSFRMFIELD),pResult,(void **)&lpFormField);
				pWFSFRMFIELD[nFieldIndex ++] = lpFormField;
				QueryOneField(pResult,lpFormField,strFileName,pXFSField);
			}
		}
	
	}

	delete pform;
	return pWFSFRMFIELD;
}

// to do
int  CServiceBasic::QueryOneField(WFSRESULT *pResult,LPWFSFRMFIELD lpFormField,CString strFileName ,CXFSField *pField)
{
	HRESULT h = m_pfnWFMAllocateMore(pField->m_strFieldName.GetLength()+1,pResult,(void **)&lpFormField->lpszFieldName);
	memcpy(lpFormField->lpszFieldName,pField->m_strFieldName.GetBuffer(0),pField->m_strFieldName.GetLength());

	lpFormField->wIndexCount = pField->index.m_wRepeatcount;
    lpFormField->fwType = WFS_FRM_FIELDTEXT;///// it need deal

	if(pField->m_strClass == "OPTIONAL")
	{
		lpFormField->fwClass = WFS_FRM_CLASSOPTIONAL;
	}
	else if(pField->m_strClass == "STATIC")
	{
		lpFormField->fwClass = WFS_FRM_CLASSSTATIC;
	}
	else if(pField->m_strClass == "REQUIRED")
	{
		lpFormField->fwClass = WFS_FRM_CLASSREQUIRED;
	}
    
	if(pField->m_strAccess == "WRITE")
	{
		lpFormField->fwAccess = WFS_FRM_ACCESSWRITE;
	}
	else if(pField->m_strAccess == "READ")
	{
		lpFormField->fwAccess = WFS_FRM_ACCESSREAD;
	}
	else if(pField->m_strAccess == "READWRITE")
	{
		lpFormField->fwAccess = WFS_FRM_ACCESSREAD|WFS_FRM_ACCESSWRITE;
	}
    
	if(pField ->m_strOverflow == "TERMINATE")
	{
		lpFormField->fwOverflow = WFS_FRM_OVFTERMINATE;
	}
	else if(pField ->m_strOverflow == "TRUNCATE")
	{
		lpFormField->fwOverflow = WFS_FRM_OVFTRUNCATE;
	}
	else if(pField ->m_strOverflow == "BESTFIT")
	{
		lpFormField->fwOverflow = WFS_FRM_OVFBESTFIT;
	}
	else if(pField ->m_strOverflow == "OVERWRITE")
	{
		lpFormField->fwOverflow = WFS_FRM_OVFOVERWRITE;
	}
	else if(pField ->m_strOverflow == "WORDWRAP")
	{
		lpFormField->fwOverflow = WFS_FRM_OVFWORDWRAP;
	}
   
	if(pField->m_strInitialValue != "")
	{
		HRESULT h = m_pfnWFMAllocateMore(pField->m_strInitialValue.GetLength()+1,pResult,(void **)&lpFormField->lpszInitialValue);
		memcpy(lpFormField->lpszInitialValue,pField->m_strInitialValue.GetBuffer(0),pField->m_strInitialValue.GetLength());
	}
    lpFormField->lpszUNICODEInitialValue;
	if(pField->m_strFormat != "")
	{
		HRESULT h = m_pfnWFMAllocateMore(pField->m_strFormat.GetLength()+1,pResult,(void **)&lpFormField->lpszFormat);
		memcpy(lpFormField->lpszFormat,pField->m_strFormat.GetBuffer(0),pField->m_strFormat.GetLength());

	}
    lpFormField->lpszUNICODEFormat;

	return 0;
}

int  CServiceBasic::QueryLogicalServiceLock(CString strLogicalService)  // lock is in Registry
{
	CAutoLock AutoLock(&g_CriticalSection);

	DWORD dwResult = 0;
	HKEY hKeyLogicalService;
	HRESULT hr = WFMOpenKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalService); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return -1; 

	HKEY hKeyTarget;
	hr = WFMOpenKey(hKeyLogicalService,
		strLogicalService.GetBuffer(0), //lpszSubKey, 
		&hKeyTarget); //phkResult, lpdwDisposition )
	
	if(hr != WFS_SUCCESS) return -1;

	char buf[2048];
	DWORD len = 2048;
	hr = WFMQueryValue (hKeyTarget,"Lock",buf, &len);
	if(hr != WFS_SUCCESS) return -1;
	buf[len] = 0;

	WFMCloseKey (hKeyTarget);
	WFMCloseKey (hKeyLogicalService);

	return atol(buf);
}

int  CServiceBasic::SetLogicalServiceLock(CString strLogicalService, long nLock)  // lock is in Registry
{
	CAutoLock AutoLock(&g_CriticalSection);

	DWORD dwResult = 0;
	HKEY hKeyLogicalService;
	HRESULT hr = WFMOpenKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalService); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return -1; 

	HKEY hKeyTarget;
	hr = WFMOpenKey(hKeyLogicalService,
		strLogicalService.GetBuffer(0), //lpszSubKey, 
		&hKeyTarget); //phkResult, lpdwDisposition )
	
	if(hr != WFS_SUCCESS) return -1;

	if(nLock == 0)
		hr = WFMSetValue ( hKeyTarget, "Lock", "0", 1);
	else 
		hr = WFMSetValue ( hKeyTarget, "Lock", "1", 1);
	if(hr != WFS_SUCCESS) return -1;

	WFMCloseKey (hKeyTarget);
	WFMCloseKey (hKeyLogicalService);

	return hr;
}