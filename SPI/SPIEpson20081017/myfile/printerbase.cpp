#include "stdafx.h"
#include "printerbase.h"
#include "xfsptr.h"

CPrinterBase::CPrinterBase()
{
	m_hPrinter = NULL;
}

CPrinterBase::~CPrinterBase()
{
	if(m_hPrinter)
	{
		//CloseHandle(m_hPrinter);
		ClosePrinter(m_hPrinter);
		m_hPrinter = NULL;
	}
}

/*
set the printer x and y resolutions
note: this funtion will fail ,if the access is not enough;
    to get the access ,to call openprinter function with printer_defaults,to pass the param
	printer_defaults.DesiredAccess
	//default resolution of epson printer,is (120,180), (180,180) and (360,180)
*/
int  CPrinterBase::SetPrinterResolution(int nXResolution,int nYResolution)
{
	int nRet;
	nRet = 0; 
	if(nXResolution)
	{
		return ++nRet;
	}

	if(nYResolution)
	{
		return ++nRet;
	}

	const DWORD  level= 2;
	PRINTER_INFO_2  *pInfo2 = NULL;
	DWORD  cbBuf;
	DWORD  cbNeeded;
	DWORD  dw;
	cbBuf = 3000;
    
	pInfo2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR,cbBuf);
	if(!pInfo2) return -1;
	BOOL bOk;
	bOk = GetPrinter(m_hPrinter,level,(LPBYTE)pInfo2,cbBuf,&cbNeeded);
	if(!bOk) return -1 ; 

	char *pPrinterName = "EPSON LQ-2090C ESC/P2";
	
	long lSize;
	lSize = DocumentProperties(NULL,m_hPrinter,pPrinterName,NULL,NULL,0);
	dw = GetLastError();

	DEVMODE  *pDevMode = NULL;
	pDevMode = (DEVMODE*)GlobalAlloc(GPTR,lSize+100);
	if(!pDevMode) return -1;
	//*pDevMode = *pInfo2->pDevMode;

	lSize = DocumentProperties(NULL,m_hPrinter,pPrinterName,pDevMode,NULL,DM_OUT_BUFFER);
	dw = GetLastError();

	if(nXResolution)
		pDevMode->dmPrintQuality = nXResolution;
	if(nYResolution)
		pDevMode->dmYResolution  = nYResolution;
                                             
	lSize = DocumentProperties(NULL,m_hPrinter,pPrinterName,pDevMode,pDevMode,DM_IN_BUFFER|DM_OUT_BUFFER);
	dw  =  GetLastError();
	DWORD  dwCommand;
	dwCommand = 0;
	pInfo2->pDevMode = pDevMode;
	pInfo2->pDevMode->dmFields =DM_YRESOLUTION|DM_PRINTQUALITY;

	lSize = DocumentProperties(NULL,m_hPrinter,pPrinterName,pInfo2->pDevMode,pInfo2->pDevMode,\
		DM_IN_BUFFER|DM_OUT_BUFFER);
	bOk = SetPrinter(m_hPrinter,level,(LPBYTE)pInfo2,dwCommand);
    dw = GetLastError();

	GlobalFree(pInfo2);
	GlobalFree(pDevMode);
	return nRet;
}

int  CPrinterBase::GetDevStatus()
{
	if(m_szPrinterName == NULL)
		return WFS_PTR_DEVNODEVICE;
	
	PRINTER_INFO_2 *pInfo2 = NULL;
	DWORD          cbBuf;
	DWORD          cbNeeded;
	DWORD          level;
	BOOL           bOk(FALSE);

	level = 2;
	cbBuf = sizeof(PRINTER_INFO_2);
	GetPrinter(m_hPrinter,
		level,
		NULL,
		cbBuf,
		&cbNeeded);

	pInfo2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR,cbNeeded);
	if(pInfo2 == NULL)
	{
		return WFS_ERR_INTERNAL_ERROR;
	}
	cbBuf = cbNeeded;
	bOk = GetPrinter(m_hPrinter,
		level,
		(PBYTE)pInfo2,
		cbBuf,
		&cbNeeded);
	if(!bOk)
		return WFS_ERR_INTERNAL_ERROR;
	m_devMode = *pInfo2->pDevMode;
	if(pInfo2->Attributes & PRINTER_ATTRIBUTE_WORK_OFFLINE)
	{
		GlobalFree(pInfo2);
		return WFS_PTR_DEVOFFLINE;
	}
	else 
	{
		GlobalFree(pInfo2);
		return WFS_PTR_DEVONLINE;
	}
	return 1;
}

BOOL CPrinterBase::IsPrinterError(HANDLE hPrinter)
   {

       //JOB_INFO_2  *pJobs;
       //int         cJobs,
         //          i;
       DWORD       dwPrinterStatus(0);

       /*
        *  Get the state information for the Printer Queue and
        *  the jobs in the Printer Queue.
        */ 
       //if (!GetJob(hPrinter, &pJobs, &cJobs, &dwPrinterStatus))
	//		return FALSE;

       /*
        *  If the Printer reports an error, believe it.
        */ 
	   
       if (dwPrinterStatus &PRINTER_STATUS_ERROR)
		   AfxMessageBox("status _error");

	   if (dwPrinterStatus &PRINTER_STATUS_PAPER_JAM)
		   AfxMessageBox("PRINTER_STATUS_PAPER_JAM");

	   if (dwPrinterStatus &PRINTER_STATUS_PAPER_OUT)
		   AfxMessageBox("PRINTER_STATUS_PAPER_OUT _error");

	   if (dwPrinterStatus &PRINTER_STATUS_PAPER_PROBLEM)
		   AfxMessageBox("PRINTER_STATUS_PAPER_PROBLEM");

	   if (dwPrinterStatus &PRINTER_STATUS_OUTPUT_BIN_FULL)
		   AfxMessageBox("PRINTER_STATUS_OUTPUT_BIN_FULL");

	   if (dwPrinterStatus &PRINTER_STATUS_NOT_AVAILABLE)
		   AfxMessageBox("PRINTER_STATUS_NOT_AVAILABLE");

	   if (dwPrinterStatus &PRINTER_STATUS_NO_TONER)
		   AfxMessageBox("PRINTER_STATUS_NO_TONER");

	   if (dwPrinterStatus &PRINTER_STATUS_OUT_OF_MEMORY)
		   AfxMessageBox("PRINTER_STATUS_OUT_OF_MEMORY");

	   if (dwPrinterStatus &PRINTER_STATUS_OFFLINE)
		   AfxMessageBox("PRINTER_STATUS_OFFLINE");

	   if (dwPrinterStatus &PRINTER_STATUS_DOOR_OPEN)
		   AfxMessageBox("PRINTER_STATUS_DOOR_OPEN");
	   
	   return TRUE;           
       
}
CString CPrinterBase::GetPrinterName(HANDLE hPrinter)
{
	if(hPrinter == NULL)
	{
		return "";
	}
	const DWORD dwLevel = 2;
	PRINTER_INFO_2 *pInfo_2 = NULL;
	DWORD dwNeed;//
	DWORD dwLen;

	dwLen = 0;
	GetPrinter(hPrinter,dwLevel,(LPBYTE)pInfo_2,dwLen,&dwNeed);
	if( dwNeed <= 0)
		return "";

	pInfo_2 = (PRINTER_INFO_2 *) GlobalAlloc(GPTR,dwNeed);
	if(pInfo_2 ==NULL)
		return "";

	BOOL bOk;
	dwLen = dwNeed;
	bOk = GetPrinter(hPrinter,dwLevel,(LPBYTE)pInfo_2,dwLen,&dwNeed);

	if(!bOk)
	{
		GlobalFree(pInfo_2);
		return "";
	}
	CString str;
	str = pInfo_2->pDriverName;
	GlobalFree(pInfo_2);
	return str;


}

DEVMODE * CPrinterBase::GetDevMode(HANDLE hPrinter)
{	
	if(hPrinter == NULL)
	{
		return NULL;
		//return NULL;
	}

	CString strDeviceName;
	strDeviceName = GetPrinterName(m_hPrinter);
	//DEVMODE *pDevModeIn = NULL;
	//DEVMODE *pDevModeOut = NULL;
	DWORD  fMode = DM_IN_BUFFER | DM_OUT_BUFFER;
	
	DWORD dwLen;
	DEVMODE *pDevMode = NULL;
	dwLen =DocumentProperties(NULL,m_hPrinter,strDeviceName.GetBuffer(0),pDevMode,pDevMode,0);
	
	if(dwLen < 0)
		return NULL;

	//pDevMode = (DEVMODE*)GlobalAlloc(GPTR,dwLen+10); //this should be delete by caller
	pDevMode = (DEVMODE*) GlobalAlloc(GPTR,dwLen);
	if(pDevMode == NULL)
		return NULL;

	dwLen = DocumentProperties(NULL,m_hPrinter,strDeviceName.GetBuffer(0),pDevMode,pDevMode,DM_IN_BUFFER | DM_OUT_BUFFER);
	return pDevMode;


}
HANDLE  CPrinterBase::OnOpenPrinter(LPSTR szPrinterName)
{//just open local printer and only one printer
	HANDLE hRet=NULL;

	BYTE PrinterEnum[1500]={0};
	int cbBuf;
	PRINTER_INFO_4 pInfo4[3]={0};
	PRINTER_INFO_2 *pInfo2 = NULL;
	DWORD cbNeeded;
	DWORD cReturned;

	BOOL    bOk(FALSE);
	DWORD   dwError;

	//cbBuf = sizeof(PrinterEnum);
	cbBuf = 1560;
	bOk = EnumPrinters(
		PRINTER_ENUM_LOCAL ,//Flags
		NULL,//Name
		2,//level
		NULL,//pPrinterEnum 
		0,//cbBuf 
		&cbNeeded,
		&cReturned);
	dwError = GetLastError();
	
	pInfo2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR,cbNeeded);
	if(pInfo2==NULL)
		return NULL;
	
	cbBuf = cbNeeded;
	bOk = EnumPrinters(
		PRINTER_ENUM_LOCAL,
		NULL,
		2,
		(LPBYTE)pInfo2,
		cbBuf,
		&cbNeeded,
		&cReturned);
	
	if(!bOk)
	{
		GlobalFree(pInfo2);
		return NULL;
	}
	//HANDLE hPrinter;
	
	char bufTemp[200];

	sprintf(bufTemp,pInfo2->pPrinterName);

	DEVMODE devMode;

	devMode = *pInfo2->pDevMode;
	GlobalFree(pInfo2);

	bOk = OpenPrinter(bufTemp,&m_hPrinter,NULL);
	if(!bOk)
	{
		AfxMessageBox("OpenPrinter fail");
		return NULL;
	}
	this->m_szPrinterName = bufTemp;
	//this->m_szPortName    = pInfo2->pPortName;
	hRet = m_hPrinter;
	return hRet;
}

int CPrinterBase::GetPtrinterResolution(WORD &wXResolution,WORD &wYResolution)
{
	wXResolution = m_devMode.dmPrintQuality;
	wYResolution = m_devMode.dmYResolution;
	return 1;
}

void CPrinterBase::EjectPaper()
{
	
	char buf[1];
	DWORD cWritten;
    //::FlushPrinter(m_hPrinter,buf,0,&cWritten,0);

	DOC_INFO_1 dcInfo={0};
	dcInfo.pDocName = "testDoc";
	dcInfo.pDatatype = "TEXT";

	if(!StartDocPrinter(m_hPrinter,1,(LPBYTE)&dcInfo))
	{		
		return;
	}
	StartPagePrinter(m_hPrinter);	
	
    
	EndPagePrinter(m_hPrinter);
	EndDocPrinter(m_hPrinter);
}

void  CPrinterBase::FlushPrinterData()
{
	char buf[1];
	DWORD cWritten;
//    ::FlushPrinter(m_hPrinter,buf,0,&cWritten,0);
}

int  CPrinterBase::ExeEpsonCommand(PBYTE pByte,DWORD cbBuf)
{
	if(m_hPrinter == NULL)
		return -1;
	const DWORD level = 1;//for winxp system

	DOC_INFO_1 docInfo1={0};
	docInfo1.pDatatype     ="RAW";
	docInfo1.pDocName      =  "doc name";
	docInfo1.pOutputFile   =  NULL;
	
	if(!StartDocPrinter(m_hPrinter,level,(LPBYTE)&docInfo1))
		return -1;

	DWORD cWritten(0);
	if(!WritePrinter(m_hPrinter,pByte,cbBuf,&cWritten))
	{
		return -1;
	}
	if(!EndDocPrinter(m_hPrinter))
	{
		return -1;
	}
	return 1;
}

int   CPrinterBase::PrinterRawData(const PBYTE  pByte,const DWORD  cbBuf)
{
	if(m_hPrinter == NULL)
		return -1;

	const DWORD level = 1;
	DOC_INFO_1 docInfo1 = {0};
	docInfo1.pDatatype = "RAW";
	docInfo1.pDocName  = "doc name";

	if(!StartDocPrinter(m_hPrinter,level,(LPBYTE)&docInfo1))
		return -1;

	DWORD  cWritten(0);
	
	if(!WritePrinter(m_hPrinter,pByte,cbBuf,&cWritten))
	{
		return -1;
	}
	if(!WritePrinter(m_hPrinter,"\n",1,&cWritten))///´¥·¢´òÓ¡ 
		return -1;

	if(!EndDocPrinter(m_hPrinter))
	{
		return -1;
	}


	return 1;
}

void  CPrinterBase::SetPrinterStartPosition()
{
}

void  CPrinterBase::SetPrinterAbsolutePosition(DWORD xPos,DWORD yPos)
{
	int nL,nH;
	BYTE pByte[100];
	int cbBuf;
	ConvertNumberToTwoPart(xPos,nL,nH);
	memset(pByte,0,sizeof(pByte));
	pByte[0] = 27;
	pByte[1] = 36;
	pByte[2] = nL;
	pByte[3] = nH;
	cbBuf = 4;
	ExeEpsonCommand(pByte,cbBuf);

	nL = 2;
	nH = 0;
	int mL,mH;
	ConvertNumberToTwoPart(yPos,mL,mH);
	memset(pByte,0,sizeof(pByte));
	pByte[0] = 27;
	pByte[1] = 40;
	pByte[2] = 86;
	pByte[3] = nL;
	pByte[4] = nH;
	pByte[5] = mL;
	pByte[6] = mH;
	cbBuf = 7;
	ExeEpsonCommand(pByte,cbBuf);
}

void  CPrinterBase::MovePrinterPosition(DWORD xPos,DWORD yPos)
{
	int nL,nH;
	BYTE pByte[100];
	int cbBuf;

	ConvertNumberToTwoPart(xPos,nL,nH);
	pByte[0] = 27;
	pByte[1] = 92;
	pByte[2] = nL;
	pByte[3] = nH;
	cbBuf = 4;
	ExeEpsonCommand(pByte,cbBuf);

	int mL,mH;

	ConvertNumberToTwoPart(yPos,mL,mH);
	memset(pByte,0,sizeof(pByte));
	pByte[0] = 27;
	pByte[1] = 40;
	pByte[2] = 118;
	pByte[3] = 2;
	pByte[4] = 0;
	pByte[5] = mL;
	pByte[6] = mH;
	cbBuf = 7;
	ExeEpsonCommand(pByte,cbBuf);
}
int CPrinterBase::GetPaperWidthAndHeight(CSize &sizePaper)
{
	DEVMODE *pDevMode = NULL;
	OnOpenPrinter();
	pDevMode = GetDevMode(m_hPrinter);
	if(pDevMode == NULL)
		return -1;
	sizePaper.cy = pDevMode->dmPaperLength;
	sizePaper.cx = pDevMode->dmPaperWidth;

	if(pDevMode)
		GlobalFree(pDevMode);
	return 1;
}

// PrinterDC