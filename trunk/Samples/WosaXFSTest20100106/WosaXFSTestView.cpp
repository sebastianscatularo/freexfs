// WosaXFSTestView.cpp : implementation of the CWosaXFSTestView class
//

#include "stdafx.h"
#include "WosaXFSTest.h"


#include "WosaXFSTestDoc.h"
#include "WosaXFSTestView.h"

#include "fundef.h"

#ifdef _V2_
#include "..\..\XFS SDK2.0\Xfsconf.h"
#else
#include "Xfsconf.h"
#endif

#include "SelGetInfoDlg.h"
#include "SelectCmdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CEdit

/////////////////////////////////////////////////////////////////////////////
// CWosaXFSTestView

IMPLEMENT_DYNCREATE(CWosaXFSTestView, CFormView)

BEGIN_MESSAGE_MAP(CWosaXFSTestView, CFormView)
	//{{AFX_MSG_MAP(CWosaXFSTestView)
	ON_BN_CLICKED(IDC_BUTTON_AsycOPEN, OnBUTTONAsycOPEN)
	ON_BN_CLICKED(IDC_BUTTON_SYNC_CLOSE, OnButtonSyncClose)
	ON_BN_CLICKED(IDC_BUTTON_ASYNC_CLOSE, OnButtonAsyncClose)
	ON_BN_CLICKED(IDC_BUTTON_SYNC_OPEN, OnButtonSyncOpen)
	ON_BN_CLICKED(IDC_BUTTON_ASYNC_EXECUTE, OnButtonAsyncExecute)
	ON_BN_CLICKED(IDC_BUTTON_SYNC_EXECUTE, OnButtonSyncExecute)
	ON_BN_CLICKED(IDC_BUTTON_SYNC_GETINFO, OnButtonSyncGetinfo)
	ON_BN_CLICKED(IDC_BUTTON_ASYNC_GETINFO, OnButtonAsyncGetinfo)
	ON_BN_CLICKED(IDC_BUTTON_SYNC_REGISTER, OnButtonSyncRegister)
	ON_BN_CLICKED(IDC_BUTTON_ASYNC_REGISTER, OnButtonAsyncRegister)
	ON_BN_CLICKED(IDC_BUTTON_ASYNC_DEREGISTER, OnButtonAsyncDeregister)
	ON_BN_CLICKED(IDC_BUTTON_ASYNC_LOCK, OnButtonAsyncLock)
	ON_BN_CLICKED(IDC_BUTTON_ASYNC_UNLOCK, OnButtonAsyncUnlock)
	ON_BN_CLICKED(IDC_BUTTON_SYNC_DEREGISTER, OnButtonSyncDeregister)
	ON_BN_CLICKED(IDC_BUTTON_SYNC_LOCK, OnButtonSyncLock)
	ON_BN_CLICKED(IDC_BUTTON_SYNC_UNLOCK, OnButtonSyncUnlock)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_START_UP, OnButtonStartUp)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_UP, OnButtonCleanUp)
	ON_BN_CLICKED(IDC_BUTTON_XFSMgr, OnBUTTONXFSMgr)
	ON_COMMAND(ID_SETTINGS_WORKDIR, OnSettingsWorkdir)
	ON_COMMAND(ID_ACTION_SCRIPTS, OnActionScripts)
	ON_COMMAND(ID_SETTINGS_GetInfo, OnSETTINGSGetInfo)
	ON_CBN_KILLFOCUS(IDC_COMBO_SERVICE, OnKillfocusComboService)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVICE, OnSelchangeComboService)
	ON_CBN_EDITCHANGE(IDC_COMBO_SERVICE, OnEditchangeComboService)
	ON_BN_CLICKED(IDC_BUTTON_EXE_CMD, OnButtonExeCmd)
	ON_BN_CLICKED(IDC_BUTTON_GETINFO_CAT, OnButtonGetinfoCat)
	ON_COMMAND(ID_SETTINGS_Execute, OnSETTINGSExecute)
	ON_EN_CHANGE(IDC_EDIT_VERSION_SPI_H, OnChangeEditVersionSpiH)
	ON_EN_CHANGE(IDC_EDIT_VERSION_SPI_L, OnChangeEditVersionSpiL)
	ON_EN_CHANGE(IDC_EDIT_VERSION_XFS_H, OnChangeEditVersionXfsH)
	ON_EN_CHANGE(IDC_EDIT_VERSION_XFS_L, OnChangeEditVersionXfsL)
	ON_COMMAND(ID_ACTION_LOAD_EXE_DATA, OnActionLoadExeData)
	ON_COMMAND(ID_SETTINGS_ENABLE_MULTIPLE, OnSettingsEnableMultiple)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_ENABLE_MULTIPLE, OnUpdateSettingsEnableMultiple)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_MESSAGE(WFS_OPEN_COMPLETE, OnCompleteOpen)
	ON_MESSAGE(WFS_CLOSE_COMPLETE, OnCompleteClose)
	ON_MESSAGE(WFS_EXECUTE_COMPLETE, OnCompleteExecute)
	ON_MESSAGE(WFS_GETINFO_COMPLETE, OnCompleteGetInfo)
	ON_MESSAGE(WFS_REGISTER_COMPLETE, OnCompleteRegister)
	ON_MESSAGE(WFS_DEREGISTER_COMPLETE, OnCompleteDeregister)
	ON_MESSAGE(WFS_LOCK_COMPLETE, OnCompleteLock)
	ON_MESSAGE(WFS_UNLOCK_COMPLETE, OnCompleteUnock)

	ON_MESSAGE(WFS_EXECUTE_EVENT, OnExecuteEvent)
	ON_MESSAGE(WFS_SERVICE_EVENT, OnServiceEvent)
	ON_MESSAGE(WFS_USER_EVENT, OnUserEvent)
	ON_MESSAGE(WFS_SYSTEM_EVENT, OnSystemEvent)
	ON_MESSAGE(WFS_TIMER_EVENT, OnTimerEvent)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWosaXFSTestView construction/destruction

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

CWosaXFSTestView::CWosaXFSTestView()
	: CFormView(CWosaXFSTestView::IDD)
{
	//{{AFX_DATA_INIT(CWosaXFSTestView)
	m_strResult = _T("");
	m_strVersionSPI = _T("00028803");
	m_strVersionXFS = _T("00029903");
	m_strXFSPath = _T("");
	m_strFormName = _T("");
	m_strLocalService = _T("");
	m_strSelectField = _T("");
	m_strSelectedMedia = _T("");
	m_strDevClass = _T("");
	m_strVersionSPI_H = _T("3.88");
	m_strVersionSPI_L = _T("3.00");
	m_strVersionXFS_H = _T("3.99");
	m_strVersionXFS_L = _T("3.00");
	m_strEventReceived = _T("");
	m_nEventRangeHigh = 0xffff;
	m_nEventRangeLow = 0x1000;
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_pAsyncService = 0;
	m_hSyncService = 0;
	m_hLib = 0;
	m_bStartUp = FALSE;
	//LoadManagerFunction("..\\dllDebug\\NI_XFSMgr.dll");

	m_IPAsyncOpen = 0;
	m_IPAsyncOpenGetInfo = 0;
	m_IPGetInfoAsyncExecute = 0;
	m_IPAsyncExecute = 0;

	m_strTextOutExecute = _T("");
	m_pTextOutExecute = NULL;

	m_strWorkingDir = "e:\\WosaTest";
	m_strStartup = "";

	m_dwCategoryIndex = 0;
	m_hAppSync = 0;
	m_IPAsyncGetInfo = 0;
	m_dwCommandIndex = 1;
	m_strExeDataFile = "";

	m_bEnableMultipleFields = FALSE;

#ifdef _V2_
	m_strVersionSPI_H = _T("2.99");
	m_strVersionSPI_L = _T("2.00");
	m_strVersionXFS_H = _T("2.99");
	m_strVersionXFS_L = _T("2.00");
#endif
}

CWosaXFSTestView::~CWosaXFSTestView()
{
}

void CWosaXFSTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWosaXFSTestView)
	DDX_Control(pDX, IDC_COMBO_FIELDS, m_comboFields);
	DDX_Control(pDX, IDC_COMBO_MEDIA, m_comoMedia);
	DDX_Control(pDX, IDC_COMBO_SERVICE, m_comboService);
	DDX_Control(pDX, IDC_COMBO_FORMS, m_comboForms);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_strResult);
	DDX_Text(pDX, IDC_EDIT_VERSION_SPI, m_strVersionSPI);
	DDX_Text(pDX, IDC_EDIT_VERSION_XFS, m_strVersionXFS);
	DDX_Text(pDX, IDC_EDIT_XFS_PATH, m_strXFSPath);
	DDX_CBString(pDX, IDC_COMBO_FORMS, m_strFormName);
	DDX_CBString(pDX, IDC_COMBO_SERVICE, m_strLocalService);
	DDX_CBString(pDX, IDC_COMBO_FIELDS, m_strSelectField);
	DDX_CBString(pDX, IDC_COMBO_MEDIA, m_strSelectedMedia);
	DDX_Text(pDX, IDC_EDIT_DEVICE_CLASS, m_strDevClass);
	DDX_Text(pDX, IDC_EDIT_VERSION_SPI_H, m_strVersionSPI_H);
	DDX_Text(pDX, IDC_EDIT_VERSION_SPI_L, m_strVersionSPI_L);
	DDX_Text(pDX, IDC_EDIT_VERSION_XFS_H, m_strVersionXFS_H);
	DDX_Text(pDX, IDC_EDIT_VERSION_XFS_L, m_strVersionXFS_L);
	DDX_Text(pDX, IDC_EDIT_EVENT_RECEIVED, m_strEventReceived);
	DDX_Text(pDX, IDC_EDIT_EVENTRANGE_HIGH, m_nEventRangeHigh);
	DDX_Text(pDX, IDC_EDIT_EVENTRANGE_LOW, m_nEventRangeLow);
	//}}AFX_DATA_MAP

	ScrollResult();
	//UpdateVersion();
}

BOOL CWosaXFSTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CWosaXFSTestView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	//AfxMessageBox(FindXMLManagerPath());

	//LoadManagerFunction("..\\dllDebug\\NI_XFSMgr.dll");

	GetDlgItem(IDC_EDIT_GETINFO_CATEGORY)->SetWindowText(pPTRCategory[m_dwCategoryIndex]);
	GetDlgItem(IDC_EDIT_EXE_COMMAND)->SetWindowText(pPTRCommand[m_dwCommandIndex]);

	ShowButtons(0);

}

/////////////////////////////////////////////////////////////////////////////
// CWosaXFSTestView printing

BOOL CWosaXFSTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWosaXFSTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWosaXFSTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CWosaXFSTestView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CWosaXFSTestView diagnostics

#ifdef _DEBUG
void CWosaXFSTestView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWosaXFSTestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CWosaXFSTestDoc* CWosaXFSTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWosaXFSTestDoc)));
	return (CWosaXFSTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWosaXFSTestView message handlers

void CWosaXFSTestView::OnButtonSyncOpen()
{

	UpdateData(TRUE);

	WriteScript("LogicalService", m_strLocalService, _T(""));
	WriteScript("Do", _T("SyncOpen"), _T(""));


	if(m_bStartUp == FALSE)
	{
		m_strResult += "SyncOpen Failed, not started up.\r\n";
		UpdateData(FALSE);
		return;
	}

	
	
	if(m_hAppSync) pfnWFSDestroyAppHandle(m_hAppSync);
	HRESULT hr = m_pfnWFSCreateAppHandle(&m_hAppSync);

	HSERVICE hService = 0;

	WFSVERSION WFSVersion1;
	WFSVERSION WFSVersion2;


	hr = (*m_pfnWFSOpen) (m_strLocalService.GetBuffer(0), 
		m_hAppSync,//WFS_DEFAULT_HAPP, //hApp, //WFS_DEFAULT_HAPP, //HAPP hApp, 
		"MySession", //NULL,  //LPSTR lpszAppID, 
		WFS_TRACE_ALL_API, //NULL, //DWORD dwTraceLevel, 
		WFS_INDEFINITE_WAIT, //DWORD dwTimeOut, 
		0x00020003, //DWORD dwSrvcVersionsRequired, 
		&WFSVersion1, //LPWFSVERSION lpSrvcVersion, 
		&WFSVersion2, //LPWFSVERSION lpSPIVersion, 
		&hService  // LPHSERVICE lphService
		);
	if(hr == 0) m_hSyncService = hService; else m_hSyncService = 0;
	
	if(hr == 0)
	{
		CString str;
		str.Format("OK SyncOpen Service ID = %d SPI version High %04X Low %04X\r\n",m_hSyncService,WFSVersion2.wHighVersion,
			WFSVersion2.wLowVersion);
		
		m_strResult += str;
	}
	else
	{
		CString str;
		str.Format("Failed SyncOpen SPI version High %04X Low %04X\r\n",WFSVersion2.wHighVersion,
			WFSVersion2.wLowVersion);
		m_strResult += str;
	}
	UpdateData(FALSE);

	// query all forms

	{
		WFSRESULT *pResult = 0;
		(*m_pfnWFSGetInfo) (m_hSyncService,WFS_INF_PTR_FORM_LIST,
			NULL, 10000, &pResult);

		if(pResult == 0)
		{
			CString str;
			str.Format("Get forms failed.");
			m_strResult += str;
			UpdateData(FALSE);
			return;
		}
		if(pResult== 0 || (pResult && pResult->hResult != S_OK))
		{
			CString str;
			str.Format("Get forms failed. Result = %d\r\n", pResult->hResult);
			m_strResult += str;
			UpdateData(FALSE);
			m_pfnWFSFreeResult(pResult);
			return;
		}

		m_comboForms.ResetContent();
		char *lp = (char *)pResult->lpBuffer;
		while(_tcslen(lp) > 0)
		{
			m_comboForms.AddString(CString(lp));
			char *lp1 = _tcschr(lp,0);
			lp = lp1+1;
		}

		m_comboForms.SetCurSel(0);
		UpdateData(TRUE);
		m_pfnWFSFreeResult(pResult);
	}	

	ShowButtons(2);
	
}

void CWosaXFSTestView::OnBUTTONAsycOPEN() 
{

	UpdateData(TRUE);

	WriteScript("LogicalService", m_strLocalService, _T(""));
	WriteScript("Do", _T("AsycOPEN"), _T(""));

	if(m_bStartUp == FALSE)
	{
		m_strResult+= "AsyncOpen Failed, not started up.\r\n";
		UpdateData(FALSE);
		return;
	}

	HSERVICE *pService = 0; //(HSERVICE *)calloc(1,sizeof(HSERVICE *));

	if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(sizeof(HSERVICE), WFS_MEM_ZEROINIT, (void **)&pService);
	else return ; //pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 


	if(m_pAsyncService) 
	{
		//free(m_pAsyncService);
		m_pfnWFMFreeBuffer(m_pAsyncService);
		m_pAsyncService = pService;
	}
	else
	{
		m_pAsyncService = pService;
	}

	WFSVERSION *pWFSVersion1 = 0; //(WFSVERSION *)calloc(1,sizeof(WFSVERSION));
	WFSVERSION *pWFSVersion2 = 0;

	if(m_pfnWFMAllocateBuffer) m_pfnWFMAllocateBuffer(sizeof(WFSVERSION), WFS_MEM_ZEROINIT, (void **)&pWFSVersion1);
	else return ; //pWFSVersion1 = (WFSVERSION *)calloc(1,sizeof(WFSVERSION)); 

	//WFSVERSION *pWFSVersion2 = (WFSVERSION *)calloc(1,sizeof(WFSVERSION));
	if(m_pfnWFMAllocateBuffer) m_pfnWFMAllocateBuffer(sizeof(WFSVERSION), WFS_MEM_ZEROINIT, (void **)&pWFSVersion2);
	else return ;//;pWFSVersion2 = (WFSVERSION *)calloc(1,sizeof(WFSVERSION)); 

	REQUESTID *pID = 0; //(REQUESTID *)calloc(1,sizeof(REQUESTID)); 
	//if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(sizeof(REQUESTID), WFS_MEM_ZEROINIT, (void **)&pID);
	//else return ; //pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 
	
	pID = &m_IPAsyncOpen;
	*pID = 0x00;

	HRESULT hr = m_pfnWFSAsyncOpen(m_strLocalService.GetBuffer(0),  // lpszLogicalName, 
		WFS_DEFAULT_HAPP, //hApp,
		"MySession", //lpszAppID, 
		NULL, //WFS_TRACE_ALL_API, //dwTraceLevel, 
		WFS_INDEFINITE_WAIT, //dwTimeOut,
		pService, //lphService, 
		this->m_hWnd, //hWnd, 
		0x00020003, //,dwSrvcVersionsRequired, 
		pWFSVersion1, //lpSrvcVersion,
		pWFSVersion2, 
		pID); //, lpRequestID )


	/*

	hr = WFSAsyncExecute( hService, 
		0, //dwCommand, 
		"", //lpCmdData,
		WFS_INDEFINITE_WAIT, //dwTimeOut, 
		this->m_hWnd,//hWnd,
		&rID);  //lpRequestID )
		*/

	//m_IPAsyncOpen = *pID;

	if(hr == 0)
	{
		CString str;
		str.Format("OK pID = %d SPI version High %04X Low %04X\r\n", *pID,pWFSVersion2->wHighVersion,
			pWFSVersion2->wLowVersion);
		m_strResult += str;
	}
	else
	{
		m_strResult+= "AsyncOpen Failed\r\n";
	}
	UpdateData(FALSE);

	m_pfnWFMFreeBuffer(pWFSVersion1);
	m_pfnWFMFreeBuffer(pWFSVersion2);
	//m_pfnWFMFreeBuffer(pID);


	ShowButtons(3);

	return;



}

LRESULT CWosaXFSTestView::OnCompleteOpen(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("OK Async Open Complete Result = %d pID = %d\r\n", pResult->hResult,pResult->RequestID);
	m_strResult += str;
	UpdateData(FALSE);


	if(m_pAsyncService && pResult->RequestID == m_IPAsyncOpen) 
	{
		//REQUESTID *pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 
		//*pID = 0x00;

		REQUESTID *pID = 0;  //(REQUESTID *)calloc(1,sizeof(REQUESTID)); 
		//if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(sizeof(REQUESTID), 
		//	WFS_MEM_ZEROINIT, (void **)&pID);
		//else return 0; //pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 
		
		pID = &m_IPAsyncOpenGetInfo ;
		*pID = 0x00;
		(*m_pfnWFSAsyncGetInfo) (*m_pAsyncService,WFS_INF_PTR_FORM_LIST, NULL, 
			10000,m_hWnd, pID);
		//m_IPAsyncOpenGetInfo = *pID;

		//m_pfnWFMFreeBuffer(pID);
	}

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}

LRESULT CWosaXFSTestView::OnCompleteClose(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("OK Async Close Complete Result = %d\r\n", pResult->hResult);
	m_strResult += str;

	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);

	if(m_pAsyncService) 
	{
		//free(m_pAsyncService);
		m_pfnWFMFreeBuffer(m_pAsyncService);
		m_pAsyncService = 0;
	}
	
	return 0;
}

LRESULT CWosaXFSTestView::OnCompleteExecute(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("OK Async Execute Complete Result = %d pID = %d\r\n", pResult->hResult,pResult->RequestID);
	m_strResult += str;


	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);

	
	return 0;
}

LRESULT CWosaXFSTestView::OnCompleteGetInfo(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);

	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("OK Async GetInfo Complete Result = %d  pID = %d\r\n", pResult->hResult,pResult->RequestID);
	m_strResult += str;
	UpdateData(FALSE);

	if(pResult == 0)
	{
		CString str;
		str.Format("GetInfo failed. Result = %d\r\n", pResult->hResult);
		m_strResult += str;
		UpdateData(FALSE);
		return 0;
	}

	if(m_IPAsyncOpenGetInfo == pResult->RequestID)  // form list
	{
		m_comboForms.ResetContent();
		char *lp = (char *)pResult->lpBuffer;
		while(lp && _tcslen(lp) > 0)
		{
			m_comboForms.AddString(CString(lp));
			char *lp1 = _tcschr(lp,0);
			lp = lp1+1;
		}

		m_comboForms.SetCurSel(0);
		UpdateData(TRUE);
		CString str;
		str.Format("OK Async GetInfo Complete Fill Combo\r\n");
		m_strResult += str;
		UpdateData(FALSE);
	}
	if(m_IPAsyncGetInfo == pResult->RequestID)
	{
		switch(m_dwCategoryIndex)
		{
			case 0:
			{
				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get Status failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					pResult = 0;
					return 0;
				}
				LPWFSPTRSTATUS lpStatus = (LPWFSPTRSTATUS)pResult->lpBuffer;
				if(lpStatus == 0)
				{
					CString str;
					str.Format("Get Status failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return 0;
				}

				ShowStatus(lpStatus);
				
				UpdateData(FALSE);
			}
			break;
			case 1:
			{
				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get Capabilities failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return 0;
				}
				LPWFSPTRCAPS lpCaps = (LPWFSPTRCAPS)pResult->lpBuffer;
				if(lpCaps == 0)
				{
					CString str;
					str.Format("Get Capabilities failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return 0;
				}

				ShowCaps(lpCaps);

				UpdateData(FALSE);
			}
			break;
			case 2:
			{
				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get forms failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					HRESULT hr = pResult->hResult;
					m_pfnWFSFreeResult(pResult);
					return hr;
				}

				CString strOut = "Forms: \r\n";
				char *lp = (char *)pResult->lpBuffer;
				while(lp && _tcslen(lp) > 0)
				{
					strOut += "  " + CString(lp) + "\r\n";
					char *lp1 = _tcschr(lp,0);
					lp = lp1+1;
				}
				m_strResult += strOut;
				UpdateData(FALSE);
			}
			break;
/*
			case 3:
			{
				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get media list failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					pResult = 0;
					return 0;
				}

				CString strOut = "Media: \r\n";
				char *lp = (char *)pResult->lpBuffer;
				while(lp && _tcslen(lp) > 0)
				{
					strOut += "  " + CString(lp) + "\r\n";
					char *lp1 = _tcschr(lp,0);
					lp = lp1+1;
				}
				m_strResult += strOut;
				UpdateData(FALSE);
			}
			break;
*/

			case 3:  // media list
			{
				m_comoMedia.ResetContent();

				//m_pfnWFSGetInfo (m_hSyncService,WFS_INF_PTR_MEDIA_LIST,
				//NULL, 10000, &pResult);
				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get media list failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return 0;
				}

				CString strOut = "Media: \r\n";
				char *lp = (char *)pResult->lpBuffer;
				while(lp && _tcslen(lp) > 0)
				{
					m_comoMedia.AddString(CString(lp));

					strOut += "  " + CString(lp) + "\r\n";
					char *lp1 = _tcschr(lp,0);
					lp = lp1+1;
				}
				m_strResult += strOut;
				UpdateData(FALSE);

				m_comoMedia.SetCurSel(0);
			}
			break;
			case 4:
			{

				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get fields failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return pResult->hResult;
				}

				LPWFSFRMHEADER lpFormHeader = (LPWFSFRMHEADER )pResult->lpBuffer;

				ShowFormHeader(lpFormHeader);
				m_comboFields.ResetContent();

				char *lp = (char *)lpFormHeader->lpszFields;
				CString strOut = "Fields of Form " + m_strFormName + ":\r\n";
				while(lp && _tcslen(lp) > 0)
				{
					m_comboFields.AddString(lp);

					strOut += " " + CString(lp) + "\r\n";
					char *lp1 = _tcschr(lp,0);
					lp = lp1+1;
				}
				if(m_comboFields.GetCount() > 0) m_comboFields.GetLBText(0,m_strSelectField);

				m_strResult += strOut;
				UpdateData(FALSE);
				str.Format("OK Async GetInfo Form Complete.\r\n");
				m_strResult += str;
				UpdateData(FALSE);
			}
			break;
			case 5:
			{
				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get Media %s of %s failed. Result = %d\r\n",m_strSelectField,m_strFormName, pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return 0;
				}
				LPWFSFRMMEDIA lpWFSFRMMEDIA = (LPWFSFRMMEDIA )pResult->lpBuffer;

				if(lpWFSFRMMEDIA) ShowMediaField(lpWFSFRMMEDIA);
				UpdateData(FALSE);
			}
			break;
			case 6:
			{
				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get fields failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return pResult->hResult;
				}

				//LPWFSFRMFIELD lpFromField = (LPWFSFRMFIELD)pResult->lpBuffer;
				//ShowFromField(lpFromField);
				LPWFSFRMFIELD *lpFromFieldList = (LPWFSFRMFIELD *)pResult->lpBuffer;
				if(lpFromFieldList) ShowFromFieldList(lpFromFieldList);

				UpdateData(FALSE);
			}
			break;
		}
		
	}

	
	if(m_IPGetInfoAsyncExecute == pResult->RequestID)  // query form
	{
		WFSPTRPRINTFORM *pwfsform = 0;
		m_pfnWFMAllocateBuffer(sizeof(WFSPTRPRINTFORM), WFS_MEM_ZEROINIT, (void **)&pwfsform);

		if(m_strExeDataFile == "")
		{

			CStringArray FieldArray;
			{
				if(pResult && pResult->hResult != S_OK)
				{
					CString str;
					str.Format("Get fields failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return 0;
				}

				LPWFSFRMHEADER lpFormHeader = (LPWFSFRMHEADER )pResult->lpBuffer;
				char *lp = (char *)lpFormHeader->lpszFields;
				while(lp && _tcslen(lp) > 0)
				{
					FieldArray.Add(CString(lp));
					char *lp1 = _tcschr(lp,0);
					lp = lp1+1;
				}
			

				m_pfnWFSFreeResult(pResult);
				pResult = 0;
			}
			
			CString strTextOut = "";
			long nCountNeeded = 0;
			long nFields = FieldArray.GetSize();
			for(long n = 0; n<nFields; n++)
			{
				CString strNum = "";
				strNum.Format("%d:",n+1);
				CString str = FieldArray.GetAt(n);
				CString str1 = strNum+FieldArray.GetAt(n);
				strTextOut += str + "[0]=" + str1;
				nCountNeeded += strTextOut.GetLength() + sizeof(TCHAR);
			}

			nCountNeeded += sizeof(TCHAR);

			m_pfnWFMAllocateMore(nCountNeeded,pwfsform, (void **)&pwfsform->lpszFields);
			{
				long nOffset = 0;
				for(long n = 0; n<nFields; n++)
				{
					CString strNum = "";
					strNum.Format("%d:",n+1);
					CString str = FieldArray.GetAt(n);
					CString str1 = strNum+FieldArray.GetAt(n);
					strTextOut = str + "[0]=" + str1;
					memcpy(pwfsform->lpszFields+nOffset,strTextOut,strTextOut.GetLength());
					nOffset += strTextOut.GetLength() + sizeof(TCHAR);
				}
			}
		}
		else  // from the data file
		{
			CFile cf;
			if(cf.Open(m_strExeDataFile, CFile::modeRead ))
			{
				long nLen = cf.GetLength();
				m_pfnWFMAllocateMore(nLen+5, pwfsform, (void **)&pwfsform->lpszFields);
				cf.Read(pwfsform->lpszFields,nLen);
				cf.Close();
			}
		}



		WFSRESULT *pResult = 0;

		m_pfnWFMAllocateMore(m_strFormName.GetLength()+1, pwfsform, (void **)&pwfsform->lpszFormName);
		memcpy(pwfsform->lpszFormName,m_strFormName.GetBuffer(0),m_strFormName.GetLength());
		pwfsform->wAlignment = WFS_PTR_ALNUSEFORMDEFN;
		pwfsform->wOffsetX = WFS_PTR_OFFSETUSEFORMDEFN;
		pwfsform->wOffsetY = WFS_PTR_OFFSETUSEFORMDEFN;
		pwfsform->wResolution = WFS_PTR_RESHIGH;
#ifdef _V2_
#else
		pwfsform->wPaperSource = WFS_PTR_PAPERANY;
#endif
		pwfsform->dwMediaControl= 0;

		REQUESTID *pID = 0; //(REQUESTID *)calloc(1,sizeof(REQUESTID)); 

		pID =  &m_IPAsyncExecute;
		*pID = 0x00;
		m_pfnWFSAsyncExecute (*m_pAsyncService, WFS_CMD_PTR_PRINT_FORM,	pwfsform, 10000,
			m_hWnd, pID);

	}

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}

LRESULT CWosaXFSTestView::OnCompleteRegister(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("OK Async Register Complete Result = %d\r\n", pResult->hResult);
	m_strResult += str;
	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}

LRESULT CWosaXFSTestView::OnCompleteDeregister(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("OK Async Deregister Complete Result = %d\r\n", pResult->hResult);
	m_strResult += str;
	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}

LRESULT CWosaXFSTestView::OnCompleteLock(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("OK Async Lock Complete Result = %d\r\n", pResult->hResult);
	m_strResult += str;
	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}

LRESULT CWosaXFSTestView::OnCompleteUnock(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("OK Async Unlock Complete Result = %d\r\n", pResult->hResult);
	m_strResult += str;
	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}

void CWosaXFSTestView::OnButtonSyncClose() 
{
	
	if(m_hSyncService)
	{
		WriteScript("Do", _T("SyncClose"), _T(""));

		HRESULT hr = 0;
		hr = (*m_pfnWFSClose)(m_hSyncService);
		m_hSyncService = 0;

		CString str;
		str.Format("OK Sync Close Result = %d\r\n", hr);
		m_strResult += str;
		UpdateData(FALSE);

		if(m_hAppSync) pfnWFSDestroyAppHandle(m_hAppSync);
		m_hAppSync = 0;

	}

	ShowButtons(1);
	
}

void CWosaXFSTestView::OnButtonAsyncClose() 
{
	if(m_pAsyncService) 
	{
		WriteScript("Do", _T("AsyncClose"), _T(""));

		REQUESTID *pID = 0; //(REQUESTID *)calloc(1,sizeof(REQUESTID)); 
		if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(sizeof(REQUESTID), WFS_MEM_ZEROINIT, (void **)&pID);
		else return ; //pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 

		*pID = 0x00;
		m_pfnWFSAsyncClose (*m_pAsyncService, m_hWnd, pID);
		m_pfnWFMFreeBuffer(pID);
	}

	ShowButtons(1);
}

void CWosaXFSTestView::OnButtonAsyncExecute() 
{
	UpdateData(TRUE);
	CString strCommand = "";
	strCommand.Format("%d",m_dwCommandIndex);
	WriteScript("CommandIndex", strCommand, _T(""));

	switch(m_dwCommandIndex)
	{
		case 0:
		break;
		case 1:
			OnButtonAsyncExecute_PrintForm();
		break;
		case 2:
		break;
		case 3:
		break;
		case 4:
		break;
		case 5:
		break;
		case 6:
		break;
		case 7:
		break;
		case 8:
		break;
		case 9:
		break;
	}
}

void CWosaXFSTestView::OnButtonAsyncExecute_PrintForm() 
{
	if(m_pAsyncService) 
	{
		UpdateData(TRUE);

		WriteScript("Form", m_strFormName, _T(""));
		WriteScript("Media", m_strSelectedMedia, _T(""));
		WriteScript("Do", _T("SyncExecute"), _T(""));

		//REQUESTID *pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 
		REQUESTID *pID = 0; //(REQUESTID *)calloc(1,sizeof(REQUESTID)); 
		//if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(sizeof(REQUESTID), WFS_MEM_ZEROINIT, (void **)&pID);
		//else return ; //pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 

		pID = &m_IPGetInfoAsyncExecute;
		*pID = 0x00;


		WFSPTRPRINTFORM *pwfsform = 0;
		m_pfnWFMAllocateBuffer(sizeof(WFSPTRPRINTFORM), WFS_MEM_ZEROINIT, (void **)&pwfsform);
		if(m_strExeDataFile == "")
		{
			CStringArray FieldArray;
			{
				WFSRESULT *pResult = 0;

				char *pData = 0;
				if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(m_strFormName.GetLength() + 1, 
				WFS_MEM_ZEROINIT, (void **)&pData);
				memcpy(pData,m_strFormName.GetBuffer(0),m_strFormName.GetLength());

				(*m_pfnWFSGetInfo) (m_hSyncService,WFS_INF_PTR_QUERY_FORM,
					pData, 10000, &pResult);

				m_pfnWFMFreeBuffer(pData);

				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					if(pResult) str.Format("Get fields failed. Result = %d\r\n", pResult->hResult);
					else str.Format("Get fields failed");
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return;
				}

				LPWFSFRMHEADER lpFormHeader = (LPWFSFRMHEADER )pResult->lpBuffer;
				char *lp = (char *)lpFormHeader->lpszFields;
				while(_tcslen(lp) > 0)
				{
					FieldArray.Add(CString(lp));
					char *lp1 = _tcschr(lp,0);
					lp = lp1+1;
				}
			

				m_pfnWFSFreeResult(pResult);
			}
			
			CString strTextOut = "";
			long nCountNeeded = 0;
			long nFields = FieldArray.GetSize();
			for(long n = 0; n<nFields; n++)
			{
				if(m_bEnableMultipleFields)
				{
					UpdateData(TRUE);

					CString strSelectField = FieldArray.GetAt(n);

					WFSRESULT *pResult = 0;
					WFSPTRQUERYFIELD *pqueryField = 0;
					m_pfnWFMAllocateBuffer(sizeof(WFSPTRQUERYFIELD), WFS_MEM_ZEROINIT, (void **)&pqueryField);

					m_pfnWFMAllocateMore(m_strFormName.GetLength() + 1,pqueryField, (void **)&pqueryField->lpszFormName);
					memcpy(pqueryField->lpszFormName, m_strFormName.GetBuffer(0),m_strFormName.GetLength());
					
					{
						m_pfnWFMAllocateMore(strSelectField.GetLength() + 1,pqueryField, (void **)&pqueryField->lpszFieldName);
						memcpy(pqueryField->lpszFieldName, strSelectField.GetBuffer(0),strSelectField.GetLength());
				
					}

					m_pfnWFSGetInfo (m_hSyncService,WFS_INF_PTR_QUERY_FIELD,
					pqueryField, 10000, &pResult);

					m_pfnWFMFreeBuffer(pqueryField);

					if(pResult == 0 || (pResult && pResult->hResult != S_OK))
					{
						CString str;
						if(pResult) str.Format("Get field %s of %s failed. Result = %d\r\n",m_strSelectField,m_strFormName, pResult->hResult);
						else str.Format("Get field %s of %s failed. \r\n",m_strSelectField,m_strFormName);
						m_strResult += str;
						UpdateData(FALSE);
						m_pfnWFSFreeResult(pResult);
						return;
					}

					LPWFSFRMFIELD *lpFromFieldList = (LPWFSFRMFIELD *)pResult->lpBuffer;
					if(lpFromFieldList) 
					{
						LPWFSFRMFIELD lpFromField = lpFromFieldList[0];
						long nIndex = lpFromField->wIndexCount;
						if(nIndex <= 0) nIndex = 1;
						for(long iField =0; iField<nIndex; iField++)
						{
							CString str ;
							str.Format("%s[%d]=%d:%s(%d)",FieldArray.GetAt(n),iField,n+1,FieldArray.GetAt(n),iField);
							nCountNeeded += str.GetLength() + sizeof(TCHAR);
						}
						nCountNeeded += sizeof(TCHAR) * 5;
					}
					m_pfnWFSFreeResult(pResult);
				}
				else
				{
					CString strNum = "";
					strNum.Format("%d:",n+1);
					CString str = FieldArray.GetAt(n);
					CString str1 = strNum+FieldArray.GetAt(n);
					strTextOut += str + "=" + str1;
					nCountNeeded += strTextOut.GetLength() + sizeof(TCHAR);


					nCountNeeded += sizeof(TCHAR);
				}
			}
			/************************************************************/		  
			m_pfnWFMAllocateMore(nCountNeeded, pwfsform, (void **)&pwfsform->lpszFields);
			/***************************************************/
			if(m_bEnableMultipleFields)
			{
				long nOffset = 0;
				for(long n = 0; n<nFields; n++)
				{
					CString strSelectField = FieldArray.GetAt(n);

					WFSRESULT *pResult = 0;
					WFSPTRQUERYFIELD *pqueryField = 0;
					m_pfnWFMAllocateBuffer(sizeof(WFSPTRQUERYFIELD), WFS_MEM_ZEROINIT, (void **)&pqueryField);

					m_pfnWFMAllocateMore(m_strFormName.GetLength() + 1,pqueryField, (void **)&pqueryField->lpszFormName);
					memcpy(pqueryField->lpszFormName, m_strFormName.GetBuffer(0),m_strFormName.GetLength());
					
					{
						m_pfnWFMAllocateMore(strSelectField.GetLength() + 1,pqueryField, (void **)&pqueryField->lpszFieldName);
						memcpy(pqueryField->lpszFieldName, strSelectField.GetBuffer(0),strSelectField.GetLength());
				
					}

					m_pfnWFSGetInfo (m_hSyncService,WFS_INF_PTR_QUERY_FIELD,
					pqueryField, 10000, &pResult);

					m_pfnWFMFreeBuffer(pqueryField);

					if(pResult == 0 || (pResult && pResult->hResult != S_OK))
					{
						CString str;
						str.Format("Get field %s of %s failed. Result = %d\r\n",m_strSelectField,m_strFormName, pResult->hResult);
						m_strResult += str;
						UpdateData(FALSE);
						m_pfnWFSFreeResult(pResult);
						return;
					}

					LPWFSFRMFIELD *lpFromFieldList = (LPWFSFRMFIELD *)pResult->lpBuffer;
					if(lpFromFieldList) 
					{
						LPWFSFRMFIELD lpFromField = lpFromFieldList[0];
						long nIndex = lpFromField->wIndexCount;
						if(nIndex <= 0) nIndex = 1;
						for(long iField =0; iField<nIndex; iField++)
						{
							CString str ;
							str.Format("%s[%d]=%d:%s(%d)",FieldArray.GetAt(n),iField,n+1,FieldArray.GetAt(n),iField);
							memcpy(pwfsform->lpszFields+nOffset,str,str.GetLength());
							nOffset += str.GetLength() + sizeof(TCHAR);
						}
					
					}
					m_pfnWFSFreeResult(pResult);
				}
			}
			else
			{
				long nOffset = 0;
				for(long n = 0; n<nFields; n++)
				{
					CString strNum = "";
					strNum.Format("%d:",n+1);
					CString str = FieldArray.GetAt(n);
					CString str1 = strNum+FieldArray.GetAt(n);
					//CString str1 = strNum;
					strTextOut = str + "=" + str1;
					memcpy(pwfsform->lpszFields+nOffset,strTextOut,strTextOut.GetLength());
					nOffset += strTextOut.GetLength() + sizeof(TCHAR);
				}
			}
		}
		else  // from the data file
		{
			CFile cf;
			if(cf.Open(m_strExeDataFile, CFile::modeRead ))
			{
				long nLen = cf.GetLength();
				m_pfnWFMAllocateMore(nLen+5, pwfsform, (void **)&pwfsform->lpszFields);
				cf.Read(pwfsform->lpszFields,nLen);
				cf.Close();
			}
		}
/*********************************************************/	

		WFSRESULT *pResult = 0;

		if(m_strFormName != "")
		{
			m_pfnWFMAllocateMore(m_strFormName.GetLength()+1, pwfsform, (void **)&pwfsform->lpszFormName);
			memcpy(pwfsform->lpszFormName,m_strFormName.GetBuffer(0),m_strFormName.GetLength());
		}

		if(m_strSelectedMedia != "")
		{
			m_pfnWFMAllocateMore(m_strSelectedMedia.GetLength()+1, pwfsform, (void **)&pwfsform->lpszMediaName);
			memcpy(pwfsform->lpszMediaName,m_strSelectedMedia.GetBuffer(0),m_strSelectedMedia.GetLength());
		}

		pwfsform->wAlignment = WFS_PTR_ALNUSEFORMDEFN;
		pwfsform->wOffsetX = WFS_PTR_OFFSETUSEFORMDEFN;
		pwfsform->wOffsetY = WFS_PTR_OFFSETUSEFORMDEFN;
		pwfsform->wResolution = WFS_PTR_RESHIGH;
#ifdef _V2_
#else
		pwfsform->wPaperSource = WFS_PTR_PAPERANY;
#endif

		pwfsform->dwMediaControl= 0;

			
		(*m_pfnWFSAsyncExecute) (*m_pAsyncService,WFS_CMD_PTR_PRINT_FORM,
			pwfsform, 10000, m_hWnd, pID);

		m_pfnWFMFreeBuffer(pwfsform);

	}
}


/*
typedef struct _wfs_ptr_print_form
{
    LPSTR           lpszFormName; 
    LPSTR           lpszMediaName;
    WORD            wAlignment;
    WORD            wOffsetX;
    WORD            wOffsetY;
    WORD            wResolution;
    DWORD           dwMediaControl;
    LPSTR           lpszFields;
    LPWSTR          lpszUNICODEFields;
    WORD            wPaperSource;
} WFSPTRPRINTFORM, * LPWFSPTRPRINTFORM;
*/

void CWosaXFSTestView::OnButtonSyncExecute() 
{
	UpdateData(TRUE);
	CString strCommand = "";
	strCommand.Format("%d",m_dwCommandIndex);
	WriteScript("CommandIndex", strCommand, _T(""));

	switch(m_dwCommandIndex)
	{
		case 0:  // WFS_CMD_PTR_CONTROL_MEDIA
			OnButtonSyncExecute_ControlMedia();
		break;
		case 1:
			OnButtonSyncExecute_PrintForm();
		break;
		case 2:  // WFS_CMD_PTR_READ_FORM
		break;
		case 3:  // WFS_CMD_PTR_RAW_DATA
		break;
		case 4:  // WFS_CMD_PTR_MEDIA_EXTENTS
		break;
		case 5:  // WFS_CMD_PTR_RESET_COUNT
		break;
		case 6:  //  WFS_CMD_PTR_READ_IMAGE
		break;
		case 7:  //  WFS_CMD_PTR_RESET
		break;
		case 8:  // WFS_CMD_PTR_RETRACT_MEDIA
		break;
		case 9:  // WFS_CMD_PTR_DISPENSE_PAPER
		break;
	}
}

void CWosaXFSTestView::OnButtonSyncExecute_PrintForm() 
{
	UpdateData(TRUE);
	if(m_hSyncService)
	{
	
		WriteScript("Form", m_strFormName, _T(""));
		WriteScript("Media", m_strSelectedMedia, _T(""));
		WriteScript("Do", _T("SyncExecute"), _T(""));

		WFSPTRPRINTFORM *pwfsform = 0;
		m_pfnWFMAllocateBuffer(sizeof(WFSPTRPRINTFORM), WFS_MEM_ZEROINIT, (void **)&pwfsform);
		if(m_strExeDataFile == "")
		{
			CStringArray FieldArray;
			{
				WFSRESULT *pResult = 0;

				char *pData = 0;
				if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(m_strFormName.GetLength() + 1, 
				WFS_MEM_ZEROINIT, (void **)&pData);
				memcpy(pData,m_strFormName.GetBuffer(0),m_strFormName.GetLength());

				(*m_pfnWFSGetInfo) (m_hSyncService,WFS_INF_PTR_QUERY_FORM,
					pData, 10000, &pResult);

				m_pfnWFMFreeBuffer(pData);

				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					if(pResult) str.Format("Get fields failed. Result = %d\r\n", pResult->hResult);
					else str.Format("Get fields failed");
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return;
				}

				LPWFSFRMHEADER lpFormHeader = (LPWFSFRMHEADER )pResult->lpBuffer;
				char *lp = (char *)lpFormHeader->lpszFields;
				while(_tcslen(lp) > 0)
				{
					FieldArray.Add(CString(lp));
					char *lp1 = _tcschr(lp,0);
					lp = lp1+1;
				}
			

				m_pfnWFSFreeResult(pResult);
			}
			
			CString strTextOut = "";
			long nCountNeeded = 0;
			long nFields = FieldArray.GetSize();
			for(long n = 0; n<nFields; n++)
			{
				if(m_bEnableMultipleFields)
				{
					UpdateData(TRUE);

					CString strSelectField = FieldArray.GetAt(n);

					WFSRESULT *pResult = 0;
					WFSPTRQUERYFIELD *pqueryField = 0;
					m_pfnWFMAllocateBuffer(sizeof(WFSPTRQUERYFIELD), WFS_MEM_ZEROINIT, (void **)&pqueryField);

					m_pfnWFMAllocateMore(m_strFormName.GetLength() + 1,pqueryField, (void **)&pqueryField->lpszFormName);
					memcpy(pqueryField->lpszFormName, m_strFormName.GetBuffer(0),m_strFormName.GetLength());
					
					{
						m_pfnWFMAllocateMore(strSelectField.GetLength() + 1,pqueryField, (void **)&pqueryField->lpszFieldName);
						memcpy(pqueryField->lpszFieldName, strSelectField.GetBuffer(0),strSelectField.GetLength());
				
					}

					m_pfnWFSGetInfo (m_hSyncService,WFS_INF_PTR_QUERY_FIELD,
					pqueryField, 10000, &pResult);

					m_pfnWFMFreeBuffer(pqueryField);

					if(pResult == 0 || (pResult && pResult->hResult != S_OK))
					{
						CString str;
						if(pResult) str.Format("Get field %s of %s failed. Result = %d\r\n",m_strSelectField,m_strFormName, pResult->hResult);
						else str.Format("Get field %s of %s failed. \r\n",m_strSelectField,m_strFormName);
						m_strResult += str;
						UpdateData(FALSE);
						m_pfnWFSFreeResult(pResult);
						return;
					}

					LPWFSFRMFIELD *lpFromFieldList = (LPWFSFRMFIELD *)pResult->lpBuffer;
					if(lpFromFieldList) 
					{
						LPWFSFRMFIELD lpFromField = lpFromFieldList[0];
						long nIndex = lpFromField->wIndexCount;
						if(nIndex <= 0) nIndex = 1;
						for(long iField =0; iField<nIndex; iField++)
						{
							CString str ;
							str.Format("%s[%d]=%d:%s(%d)",FieldArray.GetAt(n),iField,n+1,FieldArray.GetAt(n),iField);
							nCountNeeded += str.GetLength() + sizeof(TCHAR);
						}
						nCountNeeded += sizeof(TCHAR) * 5;
					}
					m_pfnWFSFreeResult(pResult);
				}
				else
				{
					CString strNum = "";
					strNum.Format("%d:",n+1);
					CString str = FieldArray.GetAt(n);
					CString str1 = strNum+FieldArray.GetAt(n);
					strTextOut += str + "=" + str1;
					nCountNeeded += strTextOut.GetLength() + sizeof(TCHAR);


					nCountNeeded += sizeof(TCHAR);
				}
			}
			/************************************************************/		  
			m_pfnWFMAllocateMore(nCountNeeded, pwfsform, (void **)&pwfsform->lpszFields);
			/***************************************************/
			if(m_bEnableMultipleFields)
			{
				long nOffset = 0;
				for(long n = 0; n<nFields; n++)
				{
					CString strSelectField = FieldArray.GetAt(n);

					WFSRESULT *pResult = 0;
					WFSPTRQUERYFIELD *pqueryField = 0;
					m_pfnWFMAllocateBuffer(sizeof(WFSPTRQUERYFIELD), WFS_MEM_ZEROINIT, (void **)&pqueryField);

					m_pfnWFMAllocateMore(m_strFormName.GetLength() + 1,pqueryField, (void **)&pqueryField->lpszFormName);
					memcpy(pqueryField->lpszFormName, m_strFormName.GetBuffer(0),m_strFormName.GetLength());
					
					{
						m_pfnWFMAllocateMore(strSelectField.GetLength() + 1,pqueryField, (void **)&pqueryField->lpszFieldName);
						memcpy(pqueryField->lpszFieldName, strSelectField.GetBuffer(0),strSelectField.GetLength());
				
					}

					m_pfnWFSGetInfo (m_hSyncService,WFS_INF_PTR_QUERY_FIELD,
					pqueryField, 10000, &pResult);

					m_pfnWFMFreeBuffer(pqueryField);

					if(pResult == 0 || (pResult && pResult->hResult != S_OK))
					{
						CString str;
						str.Format("Get field %s of %s failed. Result = %d\r\n",m_strSelectField,m_strFormName, pResult->hResult);
						m_strResult += str;
						UpdateData(FALSE);
						m_pfnWFSFreeResult(pResult);
						return;
					}

					LPWFSFRMFIELD *lpFromFieldList = (LPWFSFRMFIELD *)pResult->lpBuffer;
					if(lpFromFieldList) 
					{
						LPWFSFRMFIELD lpFromField = lpFromFieldList[0];
						long nIndex = lpFromField->wIndexCount;
						if(nIndex <= 0) nIndex = 1;
						for(long iField =0; iField<nIndex; iField++)
						{
							CString str ;
							str.Format("%s[%d]=%d:%s(%d)",FieldArray.GetAt(n),iField,n+1,FieldArray.GetAt(n),iField);
							memcpy(pwfsform->lpszFields+nOffset,str,str.GetLength());
							nOffset += str.GetLength() + sizeof(TCHAR);
						}
					
					}
					m_pfnWFSFreeResult(pResult);
				}
			}
			else
			{
				long nOffset = 0;
				for(long n = 0; n<nFields; n++)
				{
					CString strNum = "";
					strNum.Format("%d:",n+1);
					CString str = FieldArray.GetAt(n);
					CString str1 = strNum+FieldArray.GetAt(n);
					//CString str1 = strNum;
					strTextOut = str + "=" + str1;
					memcpy(pwfsform->lpszFields+nOffset,strTextOut,strTextOut.GetLength());
					nOffset += strTextOut.GetLength() + sizeof(TCHAR);
				}
			}
		}
		else  // from the data file
		{
			CFile cf;
			if(cf.Open(m_strExeDataFile, CFile::modeRead ))
			{
				long nLen = cf.GetLength();
				m_pfnWFMAllocateMore(nLen+5, pwfsform, (void **)&pwfsform->lpszFields);
				cf.Read(pwfsform->lpszFields,nLen);
				cf.Close();
			}
		}
/*********************************************************/	

		WFSRESULT *pResult = 0;

		if(m_strFormName != "")
		{
			m_pfnWFMAllocateMore(m_strFormName.GetLength()+1, pwfsform, (void **)&pwfsform->lpszFormName);
			memcpy(pwfsform->lpszFormName,m_strFormName.GetBuffer(0),m_strFormName.GetLength());
		}

		if(m_strSelectedMedia != "")
		{
			m_pfnWFMAllocateMore(m_strSelectedMedia.GetLength()+1, pwfsform, (void **)&pwfsform->lpszMediaName);
			memcpy(pwfsform->lpszMediaName,m_strSelectedMedia.GetBuffer(0),m_strSelectedMedia.GetLength());
		}

		pwfsform->wAlignment = WFS_PTR_ALNUSEFORMDEFN;
		pwfsform->wOffsetX = WFS_PTR_OFFSETUSEFORMDEFN;
		pwfsform->wOffsetY = WFS_PTR_OFFSETUSEFORMDEFN;
		pwfsform->wResolution = WFS_PTR_RESHIGH;
#ifdef _V2_
#else
		pwfsform->wPaperSource = WFS_PTR_PAPERANY;
#endif

		pwfsform->dwMediaControl= 0;

			
		(*m_pfnWFSExecute) (m_hSyncService,WFS_CMD_PTR_PRINT_FORM,
			pwfsform, 10000, &pResult);

		if(pResult)
		{
			CString str;
			str.Format("OK Sync Execute Result = %d\r\n", pResult->hResult);
			m_strResult += str;
			UpdateData(FALSE);
		}
		(*m_pfnWFSFreeResult)(pResult);
		m_pfnWFMFreeBuffer(pwfsform);
	}	
}

void CWosaXFSTestView::OnButtonSyncExecute_ControlMedia() 
{
	UpdateData(TRUE);
	if(m_hSyncService)
	{
	
		//WriteScript("Form", m_strFormName, _T(""));
		WriteScript("Do", _T("SyncExecute"), _T(""));

		WFSRESULT *pResult = 0;

		DWORD dwMediaControl = WFS_PTR_CTRLFLUSH;

		(*m_pfnWFSExecute) (m_hSyncService,WFS_CMD_PTR_CONTROL_MEDIA,
			&dwMediaControl, 10000, &pResult);

		if(pResult)
		{
			CString str;
			str.Format("OK Sync Execute Result = %d\r\n", pResult->hResult);
			m_strResult += str;
			UpdateData(FALSE);
		}
		(*m_pfnWFSFreeResult)(pResult);

	}	
}

void CWosaXFSTestView::OnButtonSyncGetinfo() 
{
	if(m_hSyncService)
	{
		UpdateData(TRUE);
		CString strCategory = "";
		strCategory.Format("%d",m_dwCategoryIndex);
		WriteScript("CategoryIndex", strCategory, _T(""));

		WriteScript("Form", m_strFormName, _T(""));
		WriteScript("Field", m_strSelectField, _T(""));

		WriteScript("Do", _T("SyncGetinfo"), _T(""));

		WFSRESULT *pResult = 0;
		switch(m_dwCategoryIndex)
		{
			case 0:
			{
				m_pfnWFSGetInfo (m_hSyncService,WFS_INF_PTR_STATUS,
				NULL, 10000, &pResult);

				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get Status failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return;
				}
				LPWFSPTRSTATUS lpStatus = (LPWFSPTRSTATUS)pResult->lpBuffer;
				if(lpStatus == 0)
				{
					CString str;
					str.Format("Get Status failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return;
				}

				ShowStatus(lpStatus);
				
				UpdateData(FALSE);

			}
			break;
			case 1:
			{
				m_pfnWFSGetInfo (m_hSyncService,WFS_INF_PTR_CAPABILITIES,
				NULL, 10000, &pResult);
				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get Capabilities failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return;
				}
				LPWFSPTRCAPS lpCaps = (LPWFSPTRCAPS)pResult->lpBuffer;
				if(lpCaps == 0)
				{
					CString str;
					str.Format("Get Capabilities failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return;
				}

				ShowCaps(lpCaps);

				UpdateData(FALSE);

			}
			break;
			case 2:  // form list
			{
				(*m_pfnWFSGetInfo) (m_hSyncService,WFS_INF_PTR_FORM_LIST,
				NULL, 10000, &pResult);

				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get forms failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return;
				}

				CString strOut = "Forms: \r\n";

				CFile cf("c:\\FormListReturn.dat", CFile::modeCreate | CFile::modeWrite);
				cf.Write(pResult->lpBuffer,200);
				cf.Close();

				char *lp = (char *)pResult->lpBuffer;
				while(lp && _tcslen(lp) > 0)
				{
					strOut += "  " + CString(lp) + "\r\n";
					char *lp1 = _tcschr(lp,0);
					lp = lp1+1;
				}
				m_strResult += strOut;
				UpdateData(FALSE);
			}
			break;
			case 3:  // media list
			{
				m_comoMedia.ResetContent();

				m_pfnWFSGetInfo (m_hSyncService,WFS_INF_PTR_MEDIA_LIST,
				NULL, 10000, &pResult);
				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get media list failed. Result = %d\r\n", pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return;
				}

				CString strOut = "Media: \r\n";
				char *lp = (char *)pResult->lpBuffer;
				while(lp && _tcslen(lp) > 0)
				{
					m_comoMedia.AddString(CString(lp));

					strOut += "  " + CString(lp) + "\r\n";
					char *lp1 = _tcschr(lp,0);
					lp = lp1+1;
				}
				m_strResult += strOut;
				UpdateData(FALSE);

				m_comoMedia.SetCurSel(0);
			}
			break;
			case 4:  // query form
			{
				//m_pfnWFSGetInfo (m_hSyncService,WFS_INF_PTR_QUERY_FORM,
				//"MySyncGetInfo", 1000, &pResult);
				UpdateData(TRUE);
				CString strOut = "Fields of Form " + m_strFormName + ":\r\n";


				//(*m_pfnWFSGetInfo) (m_hSyncService,WFS_INF_PTR_QUERY_FORM,
				//	m_strFormName.GetBuffer(0), 10000, &pResult);

				char *pData = 0;
				if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(m_strFormName.GetLength() + 1, 
					WFS_MEM_ZEROINIT, (void **)&pData);
				memcpy(pData,m_strFormName.GetBuffer(0),m_strFormName.GetLength());
				
				(*m_pfnWFSGetInfo) (m_hSyncService,WFS_INF_PTR_QUERY_FORM,
					pData, 10000, &pResult);
				
				m_pfnWFMFreeBuffer(pData);



				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get fields of %s failed. Result = %d\r\n",m_strFormName, pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return;
				}

				LPWFSFRMHEADER lpFormHeader = (LPWFSFRMHEADER )pResult->lpBuffer;

				ShowFormHeader(lpFormHeader);
				m_comboFields.ResetContent();

				char *lp = (char *)lpFormHeader->lpszFields;
				while(lp && _tcslen(lp) > 0)
				{
					m_comboFields.AddString(lp);

					strOut += " " + CString(lp) + "\r\n";
					char *lp1 = _tcschr(lp,0);
					lp = lp1+1;
				}
				m_strResult += strOut;
				if(m_comboFields.GetCount() > 0) m_comboFields.GetLBText(0,m_strSelectField);
				UpdateData(FALSE);
			}
			break;
			case 5:  // query  media
			{
				UpdateData(TRUE);

				char *pData = 0;
				if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(m_strSelectedMedia.GetLength() + 1, 
					WFS_MEM_ZEROINIT, (void **)&pData);
				memcpy(pData,m_strSelectedMedia.GetBuffer(0),m_strSelectedMedia.GetLength());

				m_pfnWFSGetInfo (m_hSyncService,WFS_INF_PTR_QUERY_MEDIA,
				pData, 1000, &pResult);

				m_pfnWFMFreeBuffer(pData);
				
				
				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get Media %s of %s failed. Result = %d\r\n",m_strSelectField,m_strFormName, pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return;
				}
				
				LPWFSFRMMEDIA lpWFSFRMMEDIA = (LPWFSFRMMEDIA )pResult->lpBuffer;

				if(lpWFSFRMMEDIA) ShowMediaField(lpWFSFRMMEDIA);
				UpdateData(FALSE);
				//LPWFSFRMFIELD lpFromField = (LPWFSFRMFIELD)pResult->lpBuffer;
				//ShowFromField(lpFromField);
				//LPWFSFRMFIELD *lpFromFieldList = (LPWFSFRMFIELD *)pResult->lpBuffer;
				//if(lpFromFieldList) ShowFromFieldList(lpFromFieldList);
				//UpdateData(FALSE);
			}
			break;
			case 6:  // query field
			{
				UpdateData(TRUE);

				//WFSPTRQUERYFIELD queryField;
				//queryField.lpszFormName = m_strFormName.GetBuffer(0);
				//if(m_strSelectField == "") queryField.lpszFieldName = 0;
				//else queryField.lpszFieldName = m_strSelectField.GetBuffer(0);

				WFSPTRQUERYFIELD *pqueryField = 0;
				m_pfnWFMAllocateBuffer(sizeof(WFSPTRQUERYFIELD), WFS_MEM_ZEROINIT, (void **)&pqueryField);

				m_pfnWFMAllocateMore(m_strFormName.GetLength() + 1,pqueryField, (void **)&pqueryField->lpszFormName);
				memcpy(pqueryField->lpszFormName, m_strFormName.GetBuffer(0),m_strFormName.GetLength());
				//pqueryField->lpszFieldName = m_strSelectField.GetBuffer(0);
				if(m_strSelectField == "") pqueryField->lpszFieldName = 0;
				else 
				{
					//pqueryField->lpszFieldName = m_strSelectField.GetBuffer(0);
					m_pfnWFMAllocateMore(m_strSelectField.GetLength() + 1,pqueryField, (void **)&pqueryField->lpszFieldName);
					memcpy(pqueryField->lpszFieldName, m_strSelectField.GetBuffer(0),m_strSelectField.GetLength());
			
				}

				m_pfnWFSGetInfo (m_hSyncService,WFS_INF_PTR_QUERY_FIELD,
				pqueryField, 10000, &pResult);

				m_pfnWFMFreeBuffer(pqueryField);
				

				if(pResult == 0 || (pResult && pResult->hResult != S_OK))
				{
					CString str;
					str.Format("Get field %s of %s failed. Result = %d\r\n",m_strSelectField,m_strFormName, pResult->hResult);
					m_strResult += str;
					UpdateData(FALSE);
					m_pfnWFSFreeResult(pResult);
					return;
				}

				//LPWFSFRMFIELD lpFromField = (LPWFSFRMFIELD)pResult->lpBuffer;
				//ShowFromField(lpFromField);
				LPWFSFRMFIELD *lpFromFieldList = (LPWFSFRMFIELD *)pResult->lpBuffer;
				if(lpFromFieldList) ShowFromFieldList(lpFromFieldList);
				UpdateData(FALSE);
			}
			break;


		}

		if(pResult)
		{
			CString str;
			str.Format("OK Sync Getinfo Result = %d\r\n", pResult->hResult);
			m_strResult += str;
			UpdateData(FALSE);

			m_pfnWFSFreeResult(pResult);
		}
	}
	
	ScrollResult();
}

void CWosaXFSTestView::OnButtonAsyncGetinfo() 
{
	if(m_pAsyncService) 
	{
		CString strCategory = "";
		strCategory.Format("%d",m_dwCategoryIndex);
		WriteScript("CategoryIndex", strCategory, _T(""));

		WriteScript("Form", m_strFormName, _T(""));
		WriteScript("Field", m_strSelectField, _T(""));

		WriteScript("Do", _T("AsyncGetinfo"), _T(""));

		//REQUESTID *pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 
		REQUESTID *pID = 0; //(REQUESTID *)calloc(1,sizeof(REQUESTID)); 
		//if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(sizeof(REQUESTID), WFS_MEM_ZEROINIT, (void **)&pID);
		//else return ; //pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 

		pID = &m_IPAsyncGetInfo;
		*pID = 0x00;

		
	
		WFSRESULT *pResult = 0;
		switch(m_dwCategoryIndex)
		{
			case 0:
			{
				(*m_pfnWFSAsyncGetInfo) (*m_pAsyncService,WFS_INF_PTR_STATUS, "MyAsyncGetInfo", 
				10000,m_hWnd, pID);
			}
			break;
			case 1:
			{
				(*m_pfnWFSAsyncGetInfo) (*m_pAsyncService,WFS_INF_PTR_CAPABILITIES, "MyAsyncGetInfo", 
				10000,m_hWnd, pID);
			}
			break;
			case 2:
			{
				(*m_pfnWFSAsyncGetInfo) (*m_pAsyncService,WFS_INF_PTR_FORM_LIST, "MyAsyncGetInfo", 
				10000,m_hWnd, pID);
			}
			break;
			case 3:
			{
				(*m_pfnWFSAsyncGetInfo) (*m_pAsyncService,WFS_INF_PTR_MEDIA_LIST, "MyAsyncGetInfo", 
				10000,m_hWnd, pID);
			}
			break;
			case 4:
			{
				UpdateData(TRUE);
				TCHAR *pChar = 0;
				m_pfnWFMAllocateBuffer(m_strFormName.GetLength() + 2, WFS_MEM_ZEROINIT, (void **)&pChar);
				memcpy(pChar,m_strFormName.GetBuffer(0),m_strFormName.GetLength());
				(*m_pfnWFSAsyncGetInfo) (*m_pAsyncService,WFS_INF_PTR_QUERY_FORM, pChar, 
				10000,m_hWnd, pID);
				m_pfnWFMFreeBuffer(pChar);
			
			}
			break;
			case 5:
			{
				char *pData = 0;
				if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(m_strSelectedMedia.GetLength() + 1, 
					WFS_MEM_ZEROINIT, (void **)&pData);
				memcpy(pData,m_strSelectedMedia.GetBuffer(0),m_strSelectedMedia.GetLength());
				
				(*m_pfnWFSAsyncGetInfo) (*m_pAsyncService,WFS_INF_PTR_QUERY_MEDIA, pData, 
				10000,m_hWnd, pID);
				m_pfnWFMFreeBuffer(pData);
			}
			break;
			case 6:
			{
				UpdateData(TRUE);
				WFSPTRQUERYFIELD *pqueryField = 0;
				m_pfnWFMAllocateBuffer(sizeof(WFSPTRQUERYFIELD), WFS_MEM_ZEROINIT, (void **)&pqueryField);

				m_pfnWFMAllocateMore(m_strFormName.GetLength() + 1,pqueryField, (void **)&pqueryField->lpszFormName);
				memcpy(pqueryField->lpszFormName, m_strFormName.GetBuffer(0),m_strFormName.GetLength());
				//pqueryField->lpszFieldName = m_strSelectField.GetBuffer(0);
				if(m_strSelectField == "") pqueryField->lpszFieldName = 0;
				else 
				{
					//pqueryField->lpszFieldName = m_strSelectField.GetBuffer(0);
					m_pfnWFMAllocateMore(m_strSelectField.GetLength() + 1,pqueryField, (void **)&pqueryField->lpszFieldName);
					memcpy(pqueryField->lpszFieldName, m_strSelectField.GetBuffer(0),m_strSelectField.GetLength());
			
				}

				(*m_pfnWFSAsyncGetInfo)(*m_pAsyncService,WFS_INF_PTR_QUERY_FIELD,
				pqueryField, 10000,m_hWnd, pID);

				m_pfnWFMFreeBuffer(pqueryField);
			}
			break;
		}
	
		//m_pfnWFMFreeBuffer(pID);

	}
}

void CWosaXFSTestView::OnButtonSyncRegister() 
{
	if(m_hSyncService)
	{
		WriteScript("Do", _T("SyncRegister"), _T(""));

		HRESULT hr = m_pfnWFSRegister (m_hSyncService
			,SYSTEM_EVENTS | USER_EVENTS | SERVICE_EVENTS | EXECUTE_EVENTS
			, m_hWnd);

		CString str;
		str.Format("OK Sync Register Result = %d\r\n", hr);
		m_strResult += str;
		UpdateData(FALSE);
	}	
}

void CWosaXFSTestView::OnButtonAsyncRegister() 
{
	if(m_pAsyncService) 
	{

		WriteScript("Do", _T("AsyncRegister"), _T(""));

		REQUESTID *pID = 0; //(REQUESTID *)calloc(1,sizeof(REQUESTID)); 
		if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(sizeof(REQUESTID), WFS_MEM_ZEROINIT, (void **)&pID);
		else return ; //pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 

		*pID = 0x00;
		m_pfnWFSAsyncRegister (*m_pAsyncService, 
			SYSTEM_EVENTS | USER_EVENTS | SERVICE_EVENTS | EXECUTE_EVENTS,
			m_hWnd, m_hWnd, pID);
		m_pfnWFMFreeBuffer(pID);
	}
}

void CWosaXFSTestView::OnButtonAsyncDeregister() 
{
	if(m_pAsyncService) 
	{
		WriteScript("Do", _T("AsyncDeregister"), _T(""));

		REQUESTID *pID = 0; //(REQUESTID *)calloc(1,sizeof(REQUESTID)); 
		if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(sizeof(REQUESTID), WFS_MEM_ZEROINIT, (void **)&pID);
		else return ; //pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 

		*pID = 0x05;
		m_pfnWFSAsyncDeregister (*m_pAsyncService,
			SYSTEM_EVENTS | USER_EVENTS | SERVICE_EVENTS | EXECUTE_EVENTS, m_hWnd, m_hWnd, pID);
		m_pfnWFMFreeBuffer(pID);
	}
}

void CWosaXFSTestView::OnButtonAsyncLock() 
{
	if(m_pAsyncService) 
	{
		WriteScript("Do", _T("AsyncLock"), _T(""));

		REQUESTID *pID = 0; //(REQUESTID *)calloc(1,sizeof(REQUESTID)); 
		if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(sizeof(REQUESTID), WFS_MEM_ZEROINIT, (void **)&pID);
		else return ; //pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 

		*pID = 0x00;
		m_pfnWFSAsyncLock (*m_pAsyncService, 1000, m_hWnd, pID);
		m_pfnWFMFreeBuffer(pID);
	}	
}

void CWosaXFSTestView::OnButtonAsyncUnlock() 
{
	if(m_pAsyncService) 
	{
		WriteScript("Do", _T("AsyncUnlock"), _T(""));

		REQUESTID *pID = 0; //(REQUESTID *)calloc(1,sizeof(REQUESTID)); 
		if(m_pfnWFMAllocateBuffer)  m_pfnWFMAllocateBuffer(sizeof(REQUESTID), WFS_MEM_ZEROINIT, (void **)&pID);
		else return ; //pID = (REQUESTID *)calloc(1,sizeof(REQUESTID)); 

		*pID = 0x06;
		m_pfnWFSAsyncUnlock(*m_pAsyncService, m_hWnd, pID);
		m_pfnWFMFreeBuffer(pID);
	
	}	
}

void CWosaXFSTestView::OnButtonSyncDeregister() 
{
	if(m_hSyncService)
	{
		WriteScript("Do", _T("SyncDeregister"), _T(""));

		HRESULT hr = m_pfnWFSDeregister(m_hSyncService,
			SYSTEM_EVENTS | USER_EVENTS | SERVICE_EVENTS | EXECUTE_EVENTS, m_hWnd);
		
		CString str;
		str.Format("OK Sync Deregister Result = %d\r\n", hr);
		m_strResult += str;
		UpdateData(FALSE);
	}	
}

void CWosaXFSTestView::OnButtonSyncLock() 
{
	if(m_hSyncService)
	{
		WriteScript("Do", _T("SyncLock"), _T(""));

		WFSRESULT *pResult = 0;
		m_pfnWFSLock(m_hSyncService,1000,&pResult);

		CString str;
		str.Format("OK Sync Lock Result = %d\r\n", pResult->hResult);
		m_strResult += str;
		UpdateData(FALSE);


		m_pfnWFSFreeResult(pResult);
	}	
}

void CWosaXFSTestView::OnButtonSyncUnlock() 
{
	if(m_hSyncService)
	{
		WriteScript("Do", _T("SyncUnlock"), _T(""));

		HRESULT hr = m_pfnWFSUnlock(m_hSyncService);

		CString str;
		str.Format("OK Sync Unlock Result = %d\r\n", hr);
		m_strResult += str;
		UpdateData(FALSE);
	}		
}

BOOL CWosaXFSTestView::LoadManagerFunction(CString strPath)
{
	
	m_hLib = LoadLibrary(strPath);
	if(m_hLib == NULL)
	{
		m_strResult += _T("Load XFS Manager failed.\r\n");
		UpdateData(FALSE);
		return FALSE;
	}
	else 
	{
		m_strResult += _T("Load XFS Manager succeeded.\r\n");
		UpdateData(FALSE);
	}

	long nLen = m_strResult.GetLength();
	
	m_pfnWFSCancelBlockingCall = (pfnWFSCancelBlockingCall)GetProcAddress(m_hLib,"WFSCancelBlockingCall");
	if(m_pfnWFSCancelBlockingCall == 0) m_strResult += _T("WFSCancelBlockingCall missing.\r\n");
	m_pfnWFSCleanUp = (pfnWFSCleanUp)GetProcAddress(m_hLib,"WFSCleanUp");
	if(m_pfnWFSCleanUp == 0) m_strResult += _T("WFSCleanUp missing.\r\n");
	m_pfnWFSClose = (pfnWFSClose)GetProcAddress(m_hLib,"WFSClose");
	if(m_pfnWFSClose == 0) m_strResult += _T("WFSClose missing.\r\n");
	m_pfnWFSAsyncClose = (pfnWFSAsyncClose)GetProcAddress(m_hLib,"WFSAsyncClose");
	if(m_pfnWFSAsyncClose == 0) m_strResult += _T("WFSAsyncClose missing.\r\n");
	m_pfnWFSCreateAppHandle = (pfnWFSCreateAppHandle)GetProcAddress(m_hLib,"WFSCreateAppHandle");
	if(m_pfnWFSCreateAppHandle == 0) m_strResult += _T("WFSCreateAppHandle missing.\r\n");
	m_pfnWFSDeregister = (pfnWFSDeregister)GetProcAddress(m_hLib,"WFSDeregister");
	if(m_pfnWFSDeregister == 0) m_strResult += _T("WFSDeregister missing.\r\n");
	m_pfnWFSAsyncDeregister = (pfnWFSAsyncDeregister)GetProcAddress(m_hLib,"WFSAsyncDeregister");
	if(m_pfnWFSAsyncDeregister == 0) m_strResult += _T("WFSAsyncDeregister missing.\r\n");
	m_pfnWFSDestroyAppHandle = (pfnWFSDestroyAppHandle)GetProcAddress(m_hLib,"WFSDestroyAppHandle");
	if(m_pfnWFSDestroyAppHandle == 0) m_strResult += _T("WFSDestroyAppHandle missing.\r\n");
	m_pfnWFSExecute = (pfnWFSExecute)GetProcAddress(m_hLib,"WFSExecute");
	if(m_pfnWFSExecute == 0) m_strResult += _T("WFSExecute missing.\r\n");
	m_pfnWFSAsyncExecute = (pfnWFSAsyncExecute)GetProcAddress(m_hLib,"WFSAsyncExecute");
	if(m_pfnWFSAsyncExecute == 0) m_strResult += _T("WFSAsyncExecute missing.\r\n");
	m_pfnWFSFreeResult = (pfnWFSFreeResult)GetProcAddress(m_hLib,"WFSFreeResult");
	if(m_pfnWFSFreeResult == 0) m_strResult += _T("WFSFreeResult missing.\r\n");
	m_pfnWFSGetInfo = (pfnWFSGetInfo)GetProcAddress(m_hLib,"WFSGetInfo");
	if(m_pfnWFSGetInfo == 0) m_strResult += _T("WFSGetInfo missing.\r\n");

	m_pfnWFSAsyncGetInfo = (pfnWFSAsyncGetInfo)GetProcAddress(m_hLib,"WFSAsyncGetInfo");
	if(m_pfnWFSAsyncGetInfo == 0) m_strResult += _T("WFSAsyncGetInfo missing.\r\n");
	m_pfnWFSIsBlocking = (pfnWFSIsBlocking)GetProcAddress(m_hLib,"WFSIsBlocking");
	if(m_pfnWFSIsBlocking == 0) m_strResult += _T("WFSIsBlocking missing.\r\n");
	m_pfnWFSLock = (pfnWFSLock)GetProcAddress(m_hLib,"WFSLock");
	if(m_pfnWFSLock == 0) m_strResult += _T("WFSLock missing.\r\n");
	m_pfnWFSAsyncLock = (pfnWFSAsyncLock)GetProcAddress(m_hLib,"WFSAsyncLock");
	if(m_pfnWFSAsyncLock == 0) m_strResult += _T("WFSAsyncLock missing.\r\n");
	m_pfnWFSOpen = (pfnWFSOpen)GetProcAddress(m_hLib,"WFSOpen");
	if(m_pfnWFSOpen == 0) m_strResult += _T("WFSOpen missing.\r\n");
	m_pfnWFSAsyncOpen = (pfnWFSAsyncOpen)GetProcAddress(m_hLib,"WFSAsyncOpen");
	if(m_pfnWFSAsyncOpen == 0) m_strResult += _T("WFSAsyncOpen missing.\r\n");
	m_pfnWFSRegister = (pfnWFSRegister)GetProcAddress(m_hLib,"WFSRegister");
	if(m_pfnWFSRegister == 0) m_strResult += _T("WFSRegister missing.\r\n");
	m_pfnWFSAsyncRegister = (pfnWFSAsyncRegister)GetProcAddress(m_hLib,"WFSAsyncRegister");
	if(m_pfnWFSAsyncRegister == 0) m_strResult += _T("WFSAsyncRegister missing.\r\n");
	m_pfnWFSSetBlockingHook = (pfnWFSSetBlockingHook)GetProcAddress(m_hLib,"WFSSetBlockingHook");
	if(m_pfnWFSSetBlockingHook == 0) m_strResult += _T("WFSSetBlockingHook missing.\r\n");
	m_pfnWFSStartUp = (pfnWFSStartUp)GetProcAddress(m_hLib,"WFSStartUp");
	if(m_pfnWFSStartUp == 0) m_strResult += _T("WFSStartUp missing.\r\n");
	m_pfnWFSUnhookBlockingHook = (pfnWFSUnhookBlockingHook)GetProcAddress(m_hLib,"WFSUnhookBlockingHook");
	if(m_pfnWFSUnhookBlockingHook == 0) m_strResult += _T("WFSUnhookBlockingHook missing.\r\n");
	m_pfnWFSUnlock = (pfnWFSUnlock)GetProcAddress(m_hLib,"WFSUnlock");
	if(m_pfnWFSUnlock == 0) m_strResult += _T("WFSUnlock missing.\r\n");
	m_pfnWFSAsyncUnlock = (pfnWFSAsyncUnlock)GetProcAddress(m_hLib,"WFSAsyncUnlock");
	if(m_pfnWFSAsyncUnlock == 0) m_strResult += _T("WFSAsyncUnlock missing.\r\n");
	m_pfnWFMSetTraceLevel = (pfnWFMSetTraceLevel)GetProcAddress(m_hLib,"WFMSetTraceLevel");
	if(m_pfnWFMSetTraceLevel == 0) m_strResult += _T("WFMSetTraceLevel missing.\r\n");


	m_pfnWFMAllocateBuffer= (pfnWFMAllocateBuffer)GetProcAddress(m_hLib,"WFMAllocateBuffer");
	if(m_pfnWFMAllocateBuffer == 0) m_strResult += _T("WFMAllocateBuffer missing.\r\n");
	m_pfnWFMAllocateMore= (pfnWFMAllocateMore)GetProcAddress(m_hLib,"WFMAllocateMore");
	if(m_pfnWFMAllocateMore == 0) m_strResult += _T("WFMAllocateBuffer missing.\r\n");
	m_pfnWFMFreeBuffer= (pfnWFMFreeBuffer)GetProcAddress(m_hLib,"WFMFreeBuffer");
	if(m_pfnWFMFreeBuffer == 0) m_strResult += _T("WFMAllocateMore missing.\r\n");
	m_pfnWFMGetTraceLevel= (pfnWFMGetTraceLevel)GetProcAddress(m_hLib,"WFMGetTraceLevel");
	if(m_pfnWFMGetTraceLevel == 0) m_strResult += _T("WFMAllocateMore missing.\r\n");
	m_pfnWFMKillTimer= (pfnWFMKillTimer)GetProcAddress(m_hLib,"WFMGetTraceLevel");
	if(m_pfnWFMKillTimer == 0) m_strResult += _T("WFMGetTraceLevel missing.\r\n");
	m_pfnWFMOutputTraceData= (pfnWFMOutputTraceData)GetProcAddress(m_hLib,"WFMOutputTraceData");
	if(m_pfnWFMOutputTraceData == 0) m_strResult += _T("WFMOutputTraceData missing.\r\n");
	m_pfnWFMReleaseDLL= (pfnWFMReleaseDLL)GetProcAddress(m_hLib,"WFMReleaseDLL");
	if(m_pfnWFMReleaseDLL == 0) m_strResult += _T("WFMReleaseDLL missing.\r\n");
	m_pfnWFMSetTimer= (pfnWFMSetTimer)GetProcAddress(m_hLib,"WFMSetTimer");
	if(m_pfnWFMSetTimer == 0) m_strResult += _T("WFMSetTimer missing.\r\n");


	UpdateData(FALSE);

	if(nLen < m_strResult.GetLength()) 
	{
		FreeLibrary(m_hLib);
		m_hLib = 0;
		return FALSE;
	}
	//AfxMessageBox(strPath);

	return TRUE;
}

CString CWosaXFSTestView::FindXMLManagerPath()
{
	DWORD dwResult = 0;

#ifdef _V2_
	HKEY hKeyXFS_ROOT;
	HRESULT hr = WFMOpenKey(HKEY_CLASSES_ROOT, //WOSA/XFS_ROOT/LOGICAL_SERVICES,
		"WOSA/XFS_ROOT", //lpszSubKey, 
		&hKeyXFS_ROOT); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return -1; 

	HKEY hKeyXFSManager;
	hr = WFMOpenKey(hKeyXFS_ROOT, //WOSA/XFS_ROOT/LOGICAL_SERVICES,
		"XFS_MANAGER", //lpszSubKey, 
		&hKeyXFSManager); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return -1; 
#else
	HKEY hKeyXFSManager;
	HRESULT hr = WFMOpenKey(WFS_CFG_MACHINE_XFS_ROOT,
		"XFS_MANAGER", //lpszSubKey, 
		&hKeyXFSManager);
	if(hr != S_OK) return "";
#endif


	char buf[2048];
	DWORD len = 2048;
	hr = WFMQueryValue (hKeyXFSManager,"dllname",buf, &len);
	if(hr != WFS_SUCCESS) return "";
	buf[len] = 0;
	CString strOut = buf;
#ifdef _V2_
	WFMCloseKey (hKeyXFS_ROOT);
	WFMCloseKey (hKeyXFSManager);
#else
	WFMCloseKey (hKeyXFSManager);
#endif

	return strOut;

}

void CWosaXFSTestView::OnDestroy() 
{
	CFormView::OnDestroy();

	if(m_hAppSync) pfnWFSDestroyAppHandle(m_hAppSync);

	// TODO: Add your message handler code here
	
}

void CWosaXFSTestView::OnButtonStartUp() 
{

	UpdateData(TRUE);

	CTime ct=CTime::GetCurrentTime();
	m_strStartup = ct.FormatGmt("%Y%m%d%H%M%SZ");


	// Write Script
	WriteScript("XFSMgr", m_strXFSPath, _T(""));
	WriteScript("VersionXFS", m_strVersionXFS, _T(""));
	WriteScript("VersionSPI", m_strVersionSPI, _T(""));
	WriteScript("Do", _T("StartUp"), _T(""));
	
	//

	if(m_strXFSPath == "") m_strXFSPath = FindXMLManagerPath(); 
	m_strResult += "XFSMgr Path: " + m_strXFSPath + "\r\n";
	if(!LoadManagerFunction(m_strXFSPath))
	{
		m_strResult += _T("WFSStartUp error in loading funcitons.\r\n");
		m_bStartUp = FALSE;
	}
	else
	{
		DWORD nVersion;
		sscanf(m_strVersionXFS,"%08X",&nVersion);
		WFSVERSION WFSVersion;
		//HRESULT hr = m_pfnWFSStartUp( 0x00020003, &WFSVersion);
		HRESULT hr = (*m_pfnWFSStartUp)( nVersion, &WFSVersion);
		if(hr == S_OK)  
		{
			CString str;
			str.Format("WFSStartUp OK with version %08X\r\n", nVersion);
			m_strResult += str;
			m_bStartUp = TRUE;
		}
		else 
		{
			CString str;
			str.Format("WFSStartUp Failed with version %08X\r\n", nVersion);
			m_strResult += str;
			m_bStartUp = FALSE;
		}
	}
	UpdateData(FALSE);

	m_comboService.ResetContent();

#ifdef _V2_
	GetLogicalServiceV2();
#else
	GetLogicalServiceV3();
#endif
	GetServiceClass();

	ShowButtons(1);
}

void CWosaXFSTestView::OnButtonCleanUp() 
{

	WriteScript("Do", _T("CleanUp"), _T(""));

	if(m_bStartUp == FALSE)
	{
		m_strResult+= "CleanUp Failed, not started up.\r\n";
		UpdateData(FALSE);
		return;
	}

	OnButtonSyncClose();
	OnButtonAsyncClose();

	WriteScript("Result: \r\n", m_strResult, _T(""));

	m_strResult = "";
	UpdateData(FALSE);

	if(m_pfnWFSCleanUp) (*m_pfnWFSCleanUp)();
	if(m_hLib != NULL) 
	{
		FreeLibrary(m_hLib);
		m_hLib = 0;
	}
	m_bStartUp = FALSE;
	m_strStartup = "";

	ShowButtons(0);
}

void CWosaXFSTestView::OnBUTTONXFSMgr() 
{
	UpdateData(TRUE);
	CFileDialog dlg(TRUE,".dll");
	if(dlg.DoModal() == IDOK)
	{
		m_strXFSPath = dlg.GetPathName();
	}
	UpdateData(FALSE);
}

void CWosaXFSTestView::OnSettingsWorkdir() 
{
	UpdateData(TRUE);
	CFileDialog fd(TRUE,NULL,"No need to type");
	if(fd.DoModal() == IDOK)
	{
		CString str = fd.GetPathName();
		long n = str.ReverseFind('\\');
		m_strWorkingDir = str.Left(n);
		UpdateData(FALSE);
	}
}

void CWosaXFSTestView::WriteScript(CString strVerb, CString strAction, CString strParams)
{
	if(m_strStartup == "") return;
	CString strFile = m_strWorkingDir + "\\Script\\" + m_strStartup + ".txt";
	NI_MakePath(strFile.GetBuffer(0));
	CStdioFile cf;
	if(cf.Open(strFile,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		cf.SeekToEnd();
		cf.WriteString(strVerb + " " + strAction + " " + strParams + "\r\n");
		cf.Close();
	}
}

BOOL CWosaXFSTestView::ExecuteScripts(CString strFilePath)
{
	CStdioFile cf;
	if(cf.Open(strFilePath,CFile::modeRead))
	{
		CString str = "";
		while(1)
		{
			BOOL bRet = cf.ReadString(str);
			if(bRet == NULL) break;
			CString strVerb;
			CString strAction;
			CString strParams;
			str.TrimLeft();
			str.TrimRight();
			long n = str.Find(" ");
			strVerb = str.Left(n);
			str = str.Mid(n+1);
			str.TrimLeft();
			if(strVerb == _T("XFSMgr"))  m_strXFSPath = str;
			n = str.Find(" ");
			if(n >= 0)
			{
				strAction = str.Left(n);
				strParams = str.Mid(n+1);
				str.TrimLeft();
			}
			else
			{
				strAction = str;
				str.TrimLeft();
			}

			
			if(strVerb == _T("VersionXFS"))  m_strVersionXFS = strAction;
			if(strVerb == _T("VersionSPI"))  m_strVersionSPI = strAction;
			if(strVerb == _T("LogicalService"))  m_strLocalService = strAction;
			if(strVerb == _T("Form"))  m_strFormName = strAction;
			if(strVerb == _T("CategoryIndex"))  m_dwCategoryIndex = atol(strAction);
			if(strVerb == _T("CommandIndex"))  m_dwCommandIndex = atol(strAction);
			if(strVerb == _T("Field"))  m_strSelectField = strAction;
	
			UpdateData(FALSE);
			if(strVerb == _T("Do")) 
			{
				if(strAction == _T("AsycOPEN")) OnBUTTONAsycOPEN();
				if(strAction == _T("SyncClose")) OnButtonSyncClose();
				if(strAction == _T("AsyncClose")) OnButtonAsyncClose();
				if(strAction == _T("SyncOpen")) OnButtonSyncOpen();
				if(strAction == _T("AsyncExecute")) OnButtonAsyncExecute();
				if(strAction == _T("SyncExecute")) OnButtonSyncExecute();
				if(strAction == _T("SyncGetinfo")) OnButtonSyncGetinfo();
				if(strAction == _T("AsyncGetinfo")) OnButtonAsyncGetinfo();
				if(strAction == _T("SyncRegister")) OnButtonSyncRegister();
				if(strAction == _T("AsyncRegister")) OnButtonAsyncRegister();
				if(strAction == _T("AsyncDeregister")) OnButtonAsyncDeregister();
				if(strAction == _T("AsyncLock")) OnButtonAsyncLock();
				if(strAction == _T("AsyncUnlock")) OnButtonAsyncUnlock();
				if(strAction == _T("SyncDeregister")) OnButtonSyncDeregister();
				if(strAction == _T("SyncLock")) OnButtonSyncLock();
				if(strAction == _T("SyncUnlock")) OnButtonSyncUnlock();
				if(strAction == _T("StartUp")) OnButtonStartUp();
				if(strAction == _T("CleanUp")) OnButtonCleanUp();
			}


		}

	}
	return FALSE;

}

void CWosaXFSTestView::OnActionScripts() 
{
	UpdateData(TRUE);
	CFileDialog fd(TRUE,NULL);
	if(fd.DoModal() == IDOK)
	{
		CString strFile = fd.GetPathName();
		ExecuteScripts(strFile);
	}
}



CString CWosaXFSTestView::GetCapTypes(WORD fwType) 
{
	CString strOut = "";
	if(fwType & WFS_PTR_TYPERECEIPT) strOut+= "RECEIPT "; 
	if(fwType & WFS_PTR_TYPEPASSBOOK) strOut+= "PASSBOOK "; 
	if(fwType & WFS_PTR_TYPEJOURNAL) strOut+= "JOURNAL "; 
	if(fwType & WFS_PTR_TYPEDOCUMENT) strOut+= "DOCUMENT "; 
#ifdef _V2_
#else
	if(fwType & WFS_PTR_TYPESCANNER) strOut+= "SCANNER "; 
#endif
	return strOut;

}

CString CWosaXFSTestView::GetCapResolutions(WORD wResolution) 
{
	CString strOut = "";
	if(wResolution & WFS_PTR_RESLOW) strOut+= "RESLOW "; 
	if(wResolution & WFS_PTR_RESMED) strOut+= "RESMED "; 
	if(wResolution & WFS_PTR_RESHIGH) strOut+= "RESHIGH "; 
	if(wResolution & WFS_PTR_RESVERYHIGH) strOut+= "RESVERYHIGH "; 
	return strOut;
}

CString CWosaXFSTestView::GetReadForm(WORD fwReadForm) 
{
	CString strOut = "";
	if(fwReadForm & WFS_PTR_READOCR) strOut+= "OCR "; 
	if(fwReadForm & WFS_PTR_READMICR) strOut+= "MICR "; 
	if(fwReadForm & WFS_PTR_READMSF) strOut+= "MSF "; 
	if(fwReadForm & WFS_PTR_READBARCODE) strOut+= "BARCODE "; 
	if(fwReadForm & WFS_PTR_READPAGEMARK) strOut+= "PAGEMARK "; 
	if(fwReadForm & WFS_PTR_READIMAGE) strOut+= "IMAGE "; 
#ifdef _V2_
#else
	if(fwReadForm & WFS_PTR_READEMPTYLINE) strOut+= "EMPTYLINE "; 
#endif
	return strOut;
}

CString CWosaXFSTestView::GetWriteForm(WORD fwWriteForm) 
{
	CString strOut = "";
	if(fwWriteForm & WFS_PTR_WRITETEXT) strOut+= "TEXT "; 
	if(fwWriteForm & WFS_PTR_WRITEGRAPHICS) strOut+= "GRAPHICS "; 
	if(fwWriteForm & WFS_PTR_WRITEOCR) strOut+= "OCR "; 
	if(fwWriteForm & WFS_PTR_WRITEMICR) strOut+= "MICR "; 
	if(fwWriteForm & WFS_PTR_WRITEMSF) strOut+= "MSF "; 
	if(fwWriteForm & WFS_PTR_WRITEBARCODE) strOut+= "BARCODE "; 
	if(fwWriteForm & WFS_PTR_WRITESTAMP) strOut+= "STAMP "; 
	return strOut;
}

CString CWosaXFSTestView::GetExtents(WORD fwExtents) 
{
	CString strOut = "";
	if(fwExtents & WFS_PTR_EXTHORIZONTAL) strOut+= "HORIZONTAL "; 
	if(fwExtents & WFS_PTR_EXTVERTICAL) strOut+= "VERTICAL "; 
	return strOut;
}




CString CWosaXFSTestView::GetControl(WORD fwControl) 
{
	CString strOut = "";
	if(fwControl & WFS_PTR_CTRLEJECT) strOut+= "EJECT "; 
	if(fwControl & WFS_PTR_CTRLPERFORATE) strOut+= "PERFORATE "; 
	if(fwControl & WFS_PTR_CTRLCUT) strOut+= "CUT "; 
	if(fwControl & WFS_PTR_CTRLSKIP) strOut+= "SKIP "; 
	if(fwControl & WFS_PTR_CTRLFLUSH) strOut+= "FLUSH "; 
	if(fwControl & WFS_PTR_CTRLRETRACT) strOut+= "RETRACT "; 
	if(fwControl & WFS_PTR_CTRLSTACK) strOut+= "STACK "; 
	if(fwControl & WFS_PTR_CTRLPARTIALCUT) strOut+= "PARTIALCUT "; 
	if(fwControl & WFS_PTR_CTRLALARM) strOut+= "ALARM "; 
	if(fwControl & WFS_PTR_CTRLATPFORWARD) strOut+= "ATPFORWARD "; 
	if(fwControl & WFS_PTR_CTRLATPBACKWARD) strOut+= "ATPBACKWARD "; 
	if(fwControl & WFS_PTR_CTRLTURNMEDIA) strOut+= "TURNMEDIA "; 
	if(fwControl & WFS_PTR_CTRLSTAMP) strOut+= "STAMP "; 
#ifdef _V2_
#else
	if(fwControl & WFS_PTR_CTRLPARK) strOut+= "PARK "; 
#endif
	return strOut;
}

CString CWosaXFSTestView::GetPaperSources(WORD fwPaperSources) 
{
	CString strOut = "";
#ifdef _V2_
#else
	if(fwPaperSources & WFS_PTR_PAPERANY) strOut+= "ANY "; 
	if(fwPaperSources & WFS_PTR_PAPERUPPER) strOut+= "UPPER "; 
	if(fwPaperSources & WFS_PTR_PAPERLOWER) strOut+= "LOWER "; 
	if(fwPaperSources & WFS_PTR_PAPEREXTERNAL) strOut+= "EXTERNAL "; 
	if(fwPaperSources & WFS_PTR_PAPERAUX) strOut+= "AUX "; 
	if(fwPaperSources & WFS_PTR_PAPERAUX2) strOut+= "AUX2 "; 
	if(fwPaperSources & WFS_PTR_PAPERPARK) strOut+= "PARK "; 
#endif
	return strOut;
}

CString CWosaXFSTestView::GetImageType(WORD fwImageType) 
{
	CString strOut = "";
	if(fwImageType & WFS_PTR_IMAGETIF) strOut+= "TIF "; 
#ifdef _V2_
#else
	if(fwImageType & WFS_PTR_IMAGEWMF) strOut+= "WMF "; 
#endif
	if(fwImageType & WFS_PTR_IMAGEBMP) strOut+= "BMP "; 
	return strOut;
}

CString CWosaXFSTestView::GetImageColorFormat(WORD fwImageColorFormat) 
{
	CString strOut = "";
#ifdef _V2_
#else
	if(fwImageColorFormat & WFS_PTR_IMAGECOLORBINARY) strOut+= "BINARY "; 
	if(fwImageColorFormat & WFS_PTR_IMAGECOLORGRAYSCALE) strOut+= "GRAYSCALE "; 
	if(fwImageColorFormat & WFS_PTR_IMAGECOLORFULL) strOut+= "COLORFULL "; 
#endif
	return strOut;
}

CString CWosaXFSTestView::GetCodelineFormat(WORD fwCodelineFormat) 
{
	CString strOut = "";
#ifdef _V2_
#else
	if(fwCodelineFormat & WFS_PTR_CODELINECMC7) strOut+= "MC7 "; 
	if(fwCodelineFormat & WFS_PTR_CODELINEE13B) strOut+= "E13B "; 
	if(fwCodelineFormat & WFS_PTR_CODELINEOCR) strOut+= "OCR "; 
#endif
	return strOut;
}

CString CWosaXFSTestView::GetImageSource(WORD fwImageSource) 
{
	CString strOut = "";
#ifdef _V2_
#else
	if(fwImageSource & WFS_PTR_IMAGEFRONT) strOut+= "FRONT "; 
	if(fwImageSource & WFS_PTR_IMAGEBACK) strOut+= "BACK "; 
	if(fwImageSource & WFS_PTR_CODELINE ) strOut+= "LINE "; 
#endif
	return strOut;
}

CString CWosaXFSTestView::GetCharSupport(WORD fwCharSupport) 
{
	CString strOut = "";
#ifdef _V2_
#else
	if(fwCharSupport & WFS_PTR_ASCII) strOut+= "ASCII "; 
	if(fwCharSupport & WFS_PTR_UNICODE) strOut+= "UNICODE "; 
#endif
	return strOut;
}

CString CWosaXFSTestView::GetFrmAccess(WORD fwAccess) 
{
	CString strOut = "";
#ifdef _V2_
#else
	if(fwAccess & WFS_FRM_ACCESSREAD) strOut+= "READ "; 
	if(fwAccess & WFS_FRM_ACCESSWRITE) strOut+= "WRITE "; 
#endif
	return strOut;
}


void CWosaXFSTestView::ShowStatus(LPWFSPTRSTATUS lpStatus)
{

	CString str;
	str.Format("Status: \r\nfwDevice %d(%s)\r\n",lpStatus->fwDevice,pDevStatus[lpStatus->fwDevice]);
	m_strResult += str;
	str.Format("fwMedia %d(%s)\r\n",lpStatus->fwMedia,pDevMedia[lpStatus->fwMedia]);
	m_strResult += str;
#ifdef _V2_
#else
	str.Format("fwPaper[WFS_PTR_SUPPLYUPPER] %d(%s)\r\n",lpStatus->fwPaper[WFS_PTR_SUPPLYUPPER],pDevPaper[lpStatus->fwPaper[WFS_PTR_SUPPLYUPPER]]);
	m_strResult += str;
	str.Format("fwPaper[WFS_PTR_SUPPLYLOWER] %d(%s)\r\n",lpStatus->fwPaper[WFS_PTR_SUPPLYLOWER],pDevPaper[lpStatus->fwPaper[WFS_PTR_SUPPLYLOWER]]);
	m_strResult += str;
	str.Format("fwPaper[WFS_PTR_SUPPLYEXTERNAL] %d(%s)\r\n",lpStatus->fwPaper[WFS_PTR_SUPPLYEXTERNAL],pDevPaper[lpStatus->fwPaper[WFS_PTR_SUPPLYEXTERNAL]]);
	m_strResult += str;
	str.Format("fwPaper[WFS_PTR_SUPPLYAUX] %d(%s)\r\n",lpStatus->fwPaper[WFS_PTR_SUPPLYAUX],pDevPaper[lpStatus->fwPaper[WFS_PTR_SUPPLYAUX]]);
	m_strResult += str;
	str.Format("fwPaper[WFS_PTR_SUPPLYAUX2] %d(%s)\r\n",lpStatus->fwPaper[WFS_PTR_SUPPLYAUX2],pDevPaper[lpStatus->fwPaper[WFS_PTR_SUPPLYAUX2]]);
	m_strResult += str;
	str.Format("fwPaper[WFS_PTR_SUPPLYPARK] %d(%s)\r\n",lpStatus->fwPaper[WFS_PTR_SUPPLYPARK],pDevPaper[lpStatus->fwPaper[WFS_PTR_SUPPLYPARK]]);
	m_strResult += str;
#endif
	str.Format("fwToner %d(%s)\r\n",lpStatus->fwToner,pDevToner[lpStatus->fwToner]);
	m_strResult += str;
	str.Format("fwInk %d(%s)\r\n",lpStatus->fwInk,pDevInk[lpStatus->fwInk]);
	m_strResult += str;
	str.Format("fwLamp %d(%s)\r\n",lpStatus->fwLamp,pDevLamp[lpStatus->fwLamp]);
	m_strResult += str;
#ifdef _V2_
#else
	if(lpStatus->lppRetractBins)
	{
		str.Format("fwToner %d\r\n",lpStatus->fwToner);
		m_strResult += str;
	}
#endif

}




void CWosaXFSTestView::ShowCaps(LPWFSPTRCAPS lpCaps)
{	
	//LPWFSPTRCAPS lpCaps = (LPWFSPTRCAPS)pResult->lpBuffer;
	CString str;
	str.Format("Cap: \r\nwClass %d(%s)\r\n",lpCaps->wClass,pDevClass[lpCaps->wClass]);
	m_strResult += str;
	str.Format("nfwType 0X%08X(%s)\r\n",lpCaps->fwType,GetCapTypes(lpCaps->fwType));
	m_strResult += str;
	str.Format("bCompound %d\r\n",lpCaps->bCompound);
	m_strResult += str;
	str.Format("wResolution 0X%08X(%s)\r\n",lpCaps->wResolution,GetCapResolutions(lpCaps->wResolution) );
	m_strResult += str;
	str.Format("fwReadForm 0X%08X(%s)\r\n",lpCaps->fwReadForm,GetReadForm(lpCaps->fwReadForm) );
	m_strResult += str;
	str.Format("fwWriteForm 0X%08X(%s)\r\n",lpCaps->fwWriteForm,GetWriteForm(lpCaps->fwWriteForm) );
	m_strResult += str;
	str.Format("fwExtents 0X%08X(%s)\r\n",lpCaps->fwExtents,GetExtents(lpCaps->fwExtents) );
	m_strResult += str;
	str.Format("fwControl 0X%08X(%s)\r\n",lpCaps->fwControl,GetControl(lpCaps->fwControl) );
	m_strResult += str;
	str.Format("usMaxMediaOnStacker %d\r\n",lpCaps->usMaxMediaOnStacker);
	m_strResult += str;
	str.Format("bAcceptMedia %d\r\n",lpCaps->bAcceptMedia);
	m_strResult += str;
#ifdef _V2_
#else
	str.Format("bMultiPage %d\r\n",lpCaps->bMultiPage);
	m_strResult += str;
	str.Format("fwPaperSources %d(%s)\r\n",lpCaps->fwPaperSources,GetPaperSources(lpCaps->fwPaperSources));
	m_strResult += str;
	str.Format("bMediaTaken %d\r\n",lpCaps->bMediaTaken);
	m_strResult += str;
	str.Format("usRetractBins %d\r\n",lpCaps->usRetractBins);
	m_strResult += str;
	if(lpCaps->usRetractBins > 0)
	{
		for(long i=0; i<lpCaps->usRetractBins; i++)
		{
			USHORT u = lpCaps->lpusMaxRetract[i];
			CString str1 = "";
			str1.Format("%u ",u);
			m_strResult += str1;
		}
		m_strResult += "\r\n";
	}

	str.Format("fwImageType %d(%s)\r\n",lpCaps->fwImageType,GetImageType(lpCaps->fwPaperSources));
	m_strResult += str;
	str.Format("fwFrontImageColorFormat %d(%s)\r\n",lpCaps->fwFrontImageColorFormat,
		GetImageColorFormat(lpCaps->fwFrontImageColorFormat));
	m_strResult += str;
	str.Format("fwBackImageColorFormat %d(%s)\r\n",lpCaps->fwBackImageColorFormat,
		GetImageColorFormat(lpCaps->fwBackImageColorFormat));
	m_strResult += str;
	str.Format("fwCodelineFormat %d(%s)\r\n",lpCaps->fwCodelineFormat,
		GetCodelineFormat(lpCaps->fwCodelineFormat));
	m_strResult += str;
	str.Format("fwImageSource %d(%s)\r\n",lpCaps->fwImageSource,
		GetImageSource(lpCaps->fwImageSource));
	m_strResult += str;
	str.Format("fwCharSupport %d(%s)\r\n",lpCaps->fwCharSupport,
		GetCharSupport(lpCaps->fwCharSupport));
	m_strResult += str;
	str.Format("bDispensePaper %d\r\n",lpCaps->bDispensePaper);
	m_strResult += str;
	if(lpCaps->lpszExtra) 
	{ 
		str.Format("lpszExtra %s\r\n",lpCaps->lpszExtra);
		m_strResult += str;
	}
#endif

				
}

void CWosaXFSTestView::ShowFormHeader(LPWFSFRMHEADER lpHeader)
{	
	CString str;
	str.Format("Form Header: \r\nlpszFormName %s\r\n",lpHeader->lpszFormName);
	m_strResult += str;
	str.Format("wBase %d(%s)\r\n",lpHeader->wBase,pFormBase[lpHeader->wBase]);
	m_strResult += str;
	str.Format("wUnitX %d\r\n",lpHeader->wUnitX);
	m_strResult += str;
	str.Format("wUnitY %d\r\n",lpHeader->wUnitY);
	m_strResult += str;
	str.Format("wWidth %d\r\n",lpHeader->wWidth);
	m_strResult += str;
	str.Format("wHeight %d\r\n",lpHeader->wHeight);
	m_strResult += str;
	str.Format("wAlignment %d(%s)\r\n",lpHeader->wAlignment,pFormAlignment[lpHeader->wAlignment]);
	m_strResult += str;
	str.Format("wOrientation %d(%s)\r\n",lpHeader->wOrientation,pFormOrientation[lpHeader->wOrientation]);
	m_strResult += str;
	str.Format("wOffsetX %d\r\n",lpHeader->wOffsetX);
	m_strResult += str;
	str.Format("wOffsetY %d\r\n",lpHeader->wOffsetY);
	m_strResult += str;
	str.Format("wVersionMajor %d\r\n",lpHeader->wVersionMajor);
	m_strResult += str;
	str.Format("wVersionMinor %d\r\n",lpHeader->wVersionMinor);
	m_strResult += str;
	if(lpHeader->lpszUserPrompt) 
	{
		str.Format("lpszUserPrompt %s\r\n",lpHeader->lpszUserPrompt);
		m_strResult += str;
	}
#ifdef _V2_
#else
	str.Format("fwCharSupport %d(%s)\r\n",lpHeader->fwCharSupport,GetCharSupport(lpHeader->fwCharSupport));
	m_strResult += str;
#endif
	
}

void CWosaXFSTestView::ShowFromFieldList(LPWFSFRMFIELD *lpFromFields)
{
	long nCount = 0;
	while(1)
	{
		LPWFSFRMFIELD lpFromField = lpFromFields[nCount++];
		if(lpFromField == 0) break;
		else
		{
			ShowFromField(lpFromField);
		}
	}
}

void CWosaXFSTestView::ShowFromField(LPWFSFRMFIELD lpFromField)
{	
	CString str;
	str.Format("Field: \r\nlpszFieldName %s\r\n",lpFromField->lpszFieldName);
	m_strResult += str;
	str.Format("wIndexCount %d\r\n",lpFromField->wIndexCount);
	m_strResult += str;
	str.Format("fwType %d(%s)\r\n",lpFromField->fwType,pFrmType[lpFromField->fwType]);
	m_strResult += str;
	str.Format("fwClass %d(%s)\r\n",lpFromField->fwClass,pFrmClass[lpFromField->fwClass]);
	m_strResult += str;
	str.Format("fwAccess %d(%s)\r\n",lpFromField->fwAccess,GetFrmAccess(lpFromField->fwAccess));
	m_strResult += str;
	str.Format("fwOverflow %d(%s)\r\n",lpFromField->fwOverflow,pFrmOverflow[lpFromField->fwOverflow]);
	m_strResult += str;
	if(lpFromField->lpszInitialValue)
	{
	str.Format("lpszInitialValue %s\r\n",lpFromField->lpszInitialValue);
	m_strResult += str;
	}
#ifdef _V2_
#else
	if(lpFromField->lpszUNICODEInitialValue)
	{
	str.Format("lpszUNICODEInitialValue %s\r\n",lpFromField->lpszUNICODEInitialValue);
	m_strResult += str;
	}
#endif
	if(lpFromField->lpszFormat)
	{
	str.Format("lpszFormat %s\r\n",lpFromField->lpszFormat);
	m_strResult += str;
	}
#ifdef _V2_
#else
	if(lpFromField->lpszUNICODEFormat)
	{
	str.Format("lpszUNICODEFormat %s\r\n",lpFromField->lpszUNICODEFormat);
	m_strResult += str;
	}
#endif

}

void CWosaXFSTestView::ShowMediaField(WFSFRMMEDIA* lpWfsMedia)
{	
	CString str;
	str.Format("MediaType %d\r\n",lpWfsMedia->fwMediaType);
	m_strResult += str;
	str.Format("Base %d\r\n",lpWfsMedia->wBase);
	m_strResult += str;
	str.Format("UnitX %d\r\n",lpWfsMedia->wUnitX);
	m_strResult += str;
	str.Format("UnitY %d\r\n",lpWfsMedia->wUnitY);
	m_strResult += str;
	str.Format("SizeWidth %d\r\n",lpWfsMedia->wSizeWidth);
	m_strResult += str;
	str.Format("SizeHeight %d\r\n",lpWfsMedia->wSizeHeight);
	m_strResult += str;

	str.Format("PageCount %d\r\n",lpWfsMedia->wPageCount);
	m_strResult += str;
	str.Format("LineCount %d\r\n",lpWfsMedia->wLineCount);
	m_strResult += str;
	str.Format("PrintAreaX %d\r\n",lpWfsMedia->wPrintAreaX);
	m_strResult += str;
	str.Format("PrintAreaY %d\r\n",lpWfsMedia->wPrintAreaY);
	m_strResult += str;
	str.Format("PrintAreaWidth %d\r\n",lpWfsMedia->wPrintAreaWidth);
	m_strResult += str;
	str.Format("PrintAreaHeight %d\r\n",lpWfsMedia->wPrintAreaHeight);
	m_strResult += str;
	str.Format("RestrictedAreaX %d\r\n",lpWfsMedia->wRestrictedAreaX);
	m_strResult += str;

	str.Format("RestrictedAreaY %d\r\n",lpWfsMedia->wRestrictedAreaY);
	m_strResult += str;
	str.Format("RestrictedAreaWidth %d\r\n",lpWfsMedia->wRestrictedAreaWidth);
	m_strResult += str;
	str.Format("RestrictedAreaHeight %d\r\n",lpWfsMedia->wRestrictedAreaHeight);
	m_strResult += str;
	str.Format("Stagger %d\r\n",lpWfsMedia->wStagger);
	m_strResult += str;
	str.Format("FoldType %d\r\n",lpWfsMedia->wFoldType);
	m_strResult += str;
	str.Format("PaperSources %d\r\n",lpWfsMedia->wPaperSources);
	m_strResult += str;
}


HRESULT CWosaXFSTestView::GetLogicalServiceV3()
{
#ifdef _V2_
#else

	GetLogicalServiceV2();

	DWORD dwResult = 0;
	HKEY hKeyLogicalService;
	HRESULT hr = WFMOpenKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalService); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) 
	{
		return -1; 
	}

	long iSubkey = 0;
	while(1)
	{
		char szName[2048];
		DWORD dwLen = 2048;
		FILETIME time;
		hr = WFMEnumKey(hKeyLogicalService,iSubkey++, szName, &dwLen,&time);
		if(hr == WFS_ERR_CFG_NO_MORE_ITEMS ) break;
		if(hr != S_OK) return S_FALSE;
		m_comboService.AddString(szName);
	}
	m_comboService.SetCurSel(0);
	WFMCloseKey (hKeyLogicalService);

	GetLogicalServiceV2();
#endif
	return S_OK;
}

HRESULT CWosaXFSTestView::GetLogicalServiceV2()
{
	DWORD dwResult = 0;
	HKEY hKeyXFS_ROOT;
	HRESULT hr = WFMOpenKey(HKEY_CLASSES_ROOT, //WOSA/XFS_ROOT/LOGICAL_SERVICES,
		"WOSA/XFS_ROOT", //lpszSubKey, 
		&hKeyXFS_ROOT); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return -1; 

	HKEY hKeyLogicalServices;
	hr = WFMOpenKey(hKeyXFS_ROOT, //WOSA/XFS_ROOT/LOGICAL_SERVICES,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalServices); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return -1; 

	long iSubkey = 0;
	while(1)
	{
		char szName[2048];
		DWORD dwLen = 2048;
		FILETIME time;
		hr = WFMEnumKey(hKeyLogicalServices,iSubkey++, szName, &dwLen,&time);
		if(hr == WFS_ERR_CFG_NO_MORE_ITEMS ) break;
		if(hr != S_OK) return S_FALSE;
		m_comboService.AddString(szName);
	}
	m_comboService.SetCurSel(0);

	WFMCloseKey (hKeyXFS_ROOT);
	WFMCloseKey (hKeyLogicalServices);
	return S_OK;
}


CString CWosaXFSTestView::GetServiceClass()
{
	CString str = "";
	UpdateData(TRUE);
	m_strDevClass = "";
	UpdateData(FALSE);
#ifdef _V2_
	HKEY hKeyXFS_ROOT;
	HRESULT hr = WFMOpenKey(HKEY_CLASSES_ROOT, //WOSA/XFS_ROOT/LOGICAL_SERVICES,
		"WOSA/XFS_ROOT", //lpszSubKey, 
		&hKeyXFS_ROOT); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return -1; 

	HKEY hKeyLogicalService;
	hr = WFMOpenKey(hKeyXFS_ROOT, //WOSA/XFS_ROOT/LOGICAL_SERVICES,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalService); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return -1; 
#else
	HKEY hKeyLogicalService;
	HRESULT hr = WFMOpenKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalService);
	if(hr != S_OK) return "";
#endif

	HKEY hKeyTarget;
	hr = WFMOpenKey(hKeyLogicalService,
		(LPTSTR)m_strLocalService.GetBuffer(0), //lpszSubKey, 
		&hKeyTarget);
	if(hr != S_OK) return "";

	char buf[2048];
	DWORD len = 2048;
	hr = WFMQueryValue (hKeyTarget,"class",buf, &len);
	if(hr != WFS_SUCCESS) return -1;
	buf[len] = 0;
	m_strDevClass = buf;
	UpdateData(FALSE);
	return buf;
}

void CWosaXFSTestView::OnKillfocusComboService() 
{
	GetServiceClass();

}

void CWosaXFSTestView::OnSelchangeComboService() 
{
	UpdateData(TRUE);
	long n = m_comboService.GetCurSel();
	if(n >= 0)
	{
		m_comboService.GetLBText(n,m_strLocalService);
		UpdateData(FALSE);
		GetServiceClass();
	}
	UpdateData(FALSE);
}

void CWosaXFSTestView::OnEditchangeComboService() 
{
	//UpdateData(TRUE);
	//GetServiceClass();
	GetDlgItem(IDC_COMBO_SERVICE)->GetWindowText(m_strLocalService);
	CString str = "";
	m_strDevClass = "";
#ifdef _V2_
	HKEY hKeyXFS_ROOT;
	HRESULT hr = WFMOpenKey(HKEY_CLASSES_ROOT, //WOSA/XFS_ROOT/LOGICAL_SERVICES,
		"WOSA/XFS_ROOT", //lpszSubKey, 
		&hKeyXFS_ROOT); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return; 

	HKEY hKeyLogicalService;
	hr = WFMOpenKey(hKeyXFS_ROOT, //WOSA/XFS_ROOT/LOGICAL_SERVICES,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalService); //phkResult, lpdwDisposition )
	if(hr != WFS_SUCCESS) return; 
#else
	HKEY hKeyLogicalService;
	HRESULT hr = WFMOpenKey(WFS_CFG_USER_DEFAULT_XFS_ROOT,
		"LOGICAL_SERVICES", //lpszSubKey, 
		&hKeyLogicalService);
	if(hr != S_OK) return;
#endif

	HKEY hKeyTarget;
	hr = WFMOpenKey(hKeyLogicalService,
		(LPTSTR)m_strLocalService.GetBuffer(0), //lpszSubKey, 
		&hKeyTarget);
	if(hr != S_OK) return;

	char buf[2048];
	DWORD len = 2048;
	hr = WFMQueryValue (hKeyTarget,"class",buf, &len);
	if(hr != WFS_SUCCESS) return;
	buf[len] = 0;
	m_strDevClass = buf;
	UpdateData(FALSE);
	return ;
}


void CWosaXFSTestView::ScrollResult() 
{
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_RESULT);
	int nLineVisible = pEdit->GetFirstVisibleLine();
	int nLineTotal = pEdit->GetLineCount();
	pEdit->LineScroll(nLineTotal - nLineVisible);
}

void CWosaXFSTestView::OnButtonExeCmd() 
{
	// TODO: Add your control notification handler code here
	OnSETTINGSExecute(); 
}

void CWosaXFSTestView::OnButtonGetinfoCat() 
{
	OnSETTINGSGetInfo(); 
	
}

void CWosaXFSTestView::OnSETTINGSGetInfo() 
{
	CSelGetInfoDlg dlg(this);

	dlg.m_nRadioGetInfo = m_dwCategoryIndex;
	if(dlg.DoModal() == IDOK)
	{
		m_dwCategoryIndex = dlg.m_nRadioGetInfo;
		GetDlgItem(IDC_EDIT_GETINFO_CATEGORY)->SetWindowText(pPTRCategory[m_dwCategoryIndex]);
	}
	
}

void CWosaXFSTestView::OnSETTINGSExecute() 
{
	CSelectCmdDlg dlg(this);

	dlg.m_nCommandIndex = m_dwCommandIndex;
	if(dlg.DoModal() == IDOK)
	{
		m_dwCommandIndex = dlg.m_nCommandIndex;
		GetDlgItem(IDC_EDIT_EXE_COMMAND)->SetWindowText(pPTRCommand[m_dwCommandIndex]);
	}
// 		GetDlgItem(IDC_EDIT_GETINFO_CATEGORY)->SetWindowText(pPTRCategory[m_dwCategoryIndex]);
	
}




CString CWosaXFSTestView::MakeVersion(CString strLow, CString strHigh)
{
	CString str = "";
	DWORD dwLowMinor, dwHighMinor;
	DWORD dwLowMajor, dwHighMajor;
	sscanf(strLow,"%d.%d",&dwLowMajor,&dwLowMinor );
	sscanf(strHigh,"%d.%d",&dwHighMajor,&dwHighMinor);
	str.Format("%02X%02X%02X%02X",dwLowMinor,dwLowMajor,dwHighMinor,dwHighMajor );
	return str;
}

void CWosaXFSTestView::UpdateVersion()
{
	CString strLow;
	CString strHigh;
	GetDlgItem(IDC_EDIT_VERSION_XFS_L)->GetWindowText(strLow);
	GetDlgItem(IDC_EDIT_VERSION_XFS_H)->GetWindowText(strHigh);
	CString str = MakeVersion(strLow,strHigh);
	GetDlgItem(IDC_EDIT_VERSION_XFS)->SetWindowText(str);

	GetDlgItem(IDC_EDIT_VERSION_SPI_L)->GetWindowText(strLow);
	GetDlgItem(IDC_EDIT_VERSION_SPI_H)->GetWindowText(strHigh);
	str = MakeVersion(strLow,strHigh);
	GetDlgItem(IDC_EDIT_VERSION_SPI)->SetWindowText(str);

}

void CWosaXFSTestView::OnChangeEditVersionSpiH() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateVersion();
}

void CWosaXFSTestView::OnChangeEditVersionSpiL() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateVersion();
}

void CWosaXFSTestView::OnChangeEditVersionXfsH() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateVersion();
}

void CWosaXFSTestView::OnChangeEditVersionXfsL() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateVersion();
}

BOOL CWosaXFSTestView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CWosaXFSTestView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::OnNotify(wParam, lParam, pResult);
}

BOOL CWosaXFSTestView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::OnCommand(wParam, lParam);
}

BOOL CWosaXFSTestView::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::OnChildNotify(message, wParam, lParam, pLResult);
}

LRESULT CWosaXFSTestView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::WindowProc(message, wParam, lParam);
}

LRESULT CWosaXFSTestView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message <= m_nEventRangeHigh && message >= m_nEventRangeLow)
	{
		CString str = "";
		str.Format("Event %08X %08X %08X\r\n",message,wParam,lParam);
		m_strEventReceived += str;
		GetDlgItem(IDC_EDIT_EVENT_RECEIVED)->SetWindowText(m_strEventReceived);
	}
	
	return CFormView::DefWindowProc(message, wParam, lParam);
}

/*
LRESULT CWosaXFSTestView::OnExecuteEvent(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("ExecuteEvent:  ID = %d\r\n", pResult->u.dwEventID);
	m_strEventReceived += str;
	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}

LRESULT CWosaXFSTestView::OnServiceEvent(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("ServiceEvent:  ID = %d\r\n", pResult->u.dwEventID);
	m_strEventReceived += str;
	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}

LRESULT CWosaXFSTestView::OnUserEvent(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("UserEvent:  ID = %d\r\n", pResult->u.dwEventID);
	m_strEventReceived += str;
	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}


LRESULT CWosaXFSTestView::OnSystemEvent(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("SystemEvent:  ID = %d\r\n", pResult->u.dwEventID);
	m_strEventReceived += str;



	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}

LRESULT CWosaXFSTestView::OnTimerEvent(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("TimerEvent:  ID = %d\r\n",wParam);
	m_strEventReceived += str;
	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}
*/


LRESULT CWosaXFSTestView::OnExecuteEvent(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("ExecuteEvent:  ID = %d\r\n", pResult->u.dwEventID);
	m_strEventReceived += str;
	UpdateData(FALSE);

	//modify by wyj 08-11-12
	DWORD dwEventID;
	dwEventID = pResult ->u.dwEventID;

	if(dwEventID == WFS_EXEE_PTR_NOMEDIA)
	{
		m_strEventReceived += "WFS_EXEE_PTR_NOMEDIA:\r\n";
		m_strEventReceived += "lpszUserPrompt:\r\n";
		m_strEventReceived += (LPSTR)pResult ->lpBuffer;
	}
	else if(dwEventID == WFS_EXEE_PTR_MEDIAINSERTED)
	{
		m_strEventReceived += "WFS_EXEE_PTR_MEDIAINSERTED:\r\n";
	}
	else if(dwEventID == WFS_EXEE_PTR_FIELDERROR)
	{
		WFSPTRFIELDFAIL wfsPtrFieldFail;
		wfsPtrFieldFail = *(LPWFSPTRFIELDFAIL)pResult->lpBuffer;
		m_strEventReceived += "WFS_EXEE_PTR_FIELDERROR:\r\n";
		m_strEventReceived += "lpszFormName:\r\n";
		m_strEventReceived += wfsPtrFieldFail.lpszFormName;
		m_strEventReceived += "lpszFieldName:\r\n";
		m_strEventReceived += wfsPtrFieldFail.lpszFieldName;
		m_strEventReceived += "\r\n";
		char buf[200]={0};
		m_strEventReceived += itoa(wfsPtrFieldFail.wFailure,buf,10);
		m_strEventReceived += "\r\n";
	}
	else if(dwEventID == WFS_EXEE_PTR_FIELDWARNING)
	{
		WFSPTRFIELDFAIL wfsPtrFieldFail;
		wfsPtrFieldFail = *(LPWFSPTRFIELDFAIL)pResult->lpBuffer;
		
		m_strEventReceived += "WFS_EXEE_FIELDWARING:\r\n";
		m_strEventReceived += "lpszFormName:\r\n";
		m_strEventReceived += wfsPtrFieldFail.lpszFormName;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpszFieldName:\r\n";
		m_strEventReceived += wfsPtrFieldFail.lpszFieldName;
		m_strEventReceived += "\r\n";
		char buf[200] = {0};
		m_strEventReceived += itoa(wfsPtrFieldFail.wFailure,buf,10);
		m_strEventReceived += "\r\n";
	}
///the follow is right ????   wrong
/*
////
	if(dwEventID == WFS_CMD_PTR_CONTROL_MEDIA)
	{//none return;
	}
	else if(dwEventID == WFS_CMD_PTR_PRINT_FORM)
	{//none return;
	}
#ifdef _V2_
#else
	else if(dwEventID == WFS_CMD_PTR_READ_FORM)
	{
		WFSPTRREADFORMOUT readFormOut;
		readFormOut = *(LPWFSPTRREADFORMOUT)pResult->lpBuffer;

		m_strEventReceived += "WFS_CMD_PTR_READ_FORM\r\n";
		m_strEventReceived += "lpszFields:\r\n";
		m_strEventReceived += readFormOut.lpszFields;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpszUNIDOCEFields:\r\n";
		m_strEventReceived += readFormOut.lpszUNICODEFields;
		m_strEventReceived += "\r\n";
	}
#endif
	else if(dwEventID == WFS_CMD_PTR_RAW_DATA)
	{
		WFSPTRRAWDATAIN wfsPtrRawDataIn;
		wfsPtrRawDataIn = *(LPWFSPTRRAWDATAIN)pResult ->lpBuffer;
		
		m_strEventReceived += "WFS_CMD_PTR_READ_FORM:\r\n";
		m_strEventReceived += "ulSize:\r\n";
		char buf[200] = {0};
		m_strEventReceived += itoa(wfsPtrRawDataIn.ulSize,buf,10);
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpbData:\r\n";
		m_strEventReceived += wfsPtrRawDataIn.lpbData;
		m_strEventReceived += "\r\n";

	}
	else if(dwEventID == WFS_CMD_PTR_MEDIA_EXTENTS)
	{	
		WFSPTRMEDIAEXT wfsPtrMediaExt;
		wfsPtrMediaExt = *(LPWFSPTRMEDIAEXT)pResult ->lpBuffer;

		m_strEventReceived += "WFS_CMD_PTR_MEIDA_EXTENTS:\r\n";

		m_strEventReceived += "ulSizeX:\r\n";
		char buf[200] = {0};
		m_strEventReceived += itoa(wfsPtrMediaExt.ulSizeX,buf,10);
		m_strEventReceived += "\r\n";

		memset(buf,0,sizeof(buf));
		m_strEventReceived += "ulSizeY:\r\n";
		m_strEventReceived += itoa(wfsPtrMediaExt.ulSizeY,buf,10);
		m_strEventReceived += "\r\n";
		
	}
	else if(dwEventID == WFS_CMD_PTR_RESET_COUNT)
	{//none return;
	}
	else if(dwEventID == WFS_CMD_PTR_READ_IMAGE)
	{
		WFSPTRIMAGE wfsPtrImage;
		wfsPtrImage = *(LPWFSPTRIMAGE)pResult->lpBuffer;

		char buf[200]={0};
		m_strEventReceived += "WFS_CMD_PTR_READ_IMAGE:\r\n";
#ifdef _V2_
#else
		m_strEventReceived += "wImageSource:\r\n";
		m_strEventReceived += itoa(wfsPtrImage.wImageSource,buf,10);
		m_strEventReceived += "\r\n";
		m_strEventReceived += "wStatus:\r\n";
		memset(buf,0,sizeof(buf));
		m_strEventReceived += itoa(wfsPtrImage.wStatus,buf,10);
		m_strEventReceived += "\r\n";
		m_strEventReceived += "ulDataLength:\r\n";
		memset(buf,0,sizeof(buf));
		m_strEventReceived += itoa(wfsPtrImage.ulDataLength,buf,10);
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpData:\r\n";
		memset(buf,0,sizeof(buf));
		BYTE byte;
		byte = *wfsPtrImage.lpbData;
		m_strEventReceived += itoa(byte,buf,10);
		m_strEventReceived += "\r\n";
#endif
	}
#ifdef _V2_
#else
	else if(dwEventID == WFS_CMD_PTR_RESET)
	{//none return;
	}
	else if(dwEventID == WFS_CMD_PTR_RETRACT_MEDIA)
	{
		m_strEventReceived += "WFS_CMD_PTR_RETRACT_MEDIA\r\n";
		m_strEventReceived += "lpusBinNumber:\r\n";
		USHORT us;
		us = *(LPUSHORT)pResult->lpBuffer;
		char buf[200]={0};
		m_strEventReceived += itoa(us,buf,10);
		m_strEventReceived += "\r\n";
	}

	else if(dwEventID == WFS_CMD_PTR_DISPENSE_PAPER)
	{//none return;
	}	
#endif
	else 
	{//default
	}
	UpdateData(FALSE);
	//end 
*/

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}

LRESULT CWosaXFSTestView::OnServiceEvent(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("ServiceEvent:  ID = %d\r\n", pResult->u.dwEventID);
	m_strEventReceived += str;
	UpdateData(FALSE);

	//add  by wyj 08-11-12
	DWORD dwEventID;
	dwEventID = pResult ->u.dwEventID;

	if(dwEventID == WFS_SRVE_PTR_MEDIATAKEN)
	{
		m_strEventReceived += "WFS_SRVE_PTR_MEDIATAKEN:\r\n";
	}
	else if(dwEventID == WFS_SRVE_PTR_MEDIAINSERTED)
	{
		m_strEventReceived += "WFS_SRVE_PTR_MEDIAINSERTED:\r\n";
	}
#ifdef _V2_
#else
	else if(dwEventID == WFS_SRVE_PTR_MEDIADETECTED)
	{
		WFSPTRMEDIADETECTED wfsPtrMediaDetected;
		wfsPtrMediaDetected = *(LPWFSPTRMEDIADETECTED)pResult->lpBuffer;

		m_strEventReceived += "WFS_SRVE_PTR_MEIDADETECTED:\r\n";
		m_strEventReceived += "wPosition:\r\n";
		char buf[200] = {0};
		m_strEventReceived += itoa(wfsPtrMediaDetected.wPosition,buf,10);
		m_strEventReceived += "\r\n";
		memset(buf,0,sizeof(buf));
		m_strEventReceived += itoa(wfsPtrMediaDetected.usRetractBinNumber,buf,10);
		m_strEventReceived += "\r\n";
		
	}
#endif

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}

LRESULT CWosaXFSTestView::OnUserEvent(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("UserEvent:  ID = %d\r\n", pResult->u.dwEventID);
	m_strEventReceived += str;
	UpdateData(FALSE);

	//add by wyj 08-11-12
	DWORD dwEventID;
	dwEventID = pResult ->u.dwEventID;
	if(dwEventID == WFS_USRE_PTR_RETRACTBINTHRESHOLD)
	{
#ifdef _V2_
#else
		WFSPTRBINTHRESHOLD wfsPtrBinThresHold;
		wfsPtrBinThresHold = *(LPWFSPTRBINTHRESHOLD)pResult->lpBuffer;
		m_strEventReceived += "WFS_USRE_PTR_RETRACTBINTHRESHOLD:\r\n";
		m_strEventReceived += "usBinNumber:\r\n";
		char buf[200] = {0};
		m_strEventReceived += itoa(wfsPtrBinThresHold.usBinNumber,buf,10);
		m_strEventReceived += "\r\n";
		m_strEventReceived += "wRetractBin:\r\n";
		memset(buf,0,sizeof(buf));
		m_strEventReceived += itoa(wfsPtrBinThresHold.wRetractBin,buf,10);
		m_strEventReceived += "\r\n";
#endif
	}
	else if(dwEventID == WFS_USRE_PTR_PAPERTHRESHOLD)
	{
#ifdef _V2_
#else
		WFSPTRPAPERTHRESHOLD wfsPtrPaperThresHold;
		wfsPtrPaperThresHold = *(LPWFSPTRPAPERTHRESHOLD)pResult->lpBuffer;
		
		m_strEventReceived += "WFS_USRE_PTR_PAPERTHRESHOLD:\r\n";
		m_strEventReceived += "wPaperSource:\r\n";
		char buf[200] = {0};
		m_strEventReceived += itoa(wfsPtrPaperThresHold.wPaperSource,buf,10);
		m_strEventReceived += "\r\n";
		m_strEventReceived += "wPaperThresHold:\r\n";
		memset(buf,0,sizeof(buf));
		m_strEventReceived += itoa(wfsPtrPaperThresHold.wPaperThreshold,buf,10);
		m_strEventReceived += "\r\n";
#endif
		
	}
	else if(dwEventID == WFS_USRE_PTR_TONERTHRESHOLD)
	{//lpwTonerThreshold;
		m_strEventReceived += "WFS_USRE_PTR_TONERTHRESHOLD:\r\n";
		m_strEventReceived += "lpwTonerthreshold:\r\n";
		DWORD dwTonerThreshold = *(LPDWORD)pResult->lpBuffer;
		char buf[200] = {0};
		m_strEventReceived += itoa(dwTonerThreshold,buf,10);
		m_strEventReceived += "\r\n";

	}
#ifdef _V2_
#else
	else if(dwEventID == WFS_USRE_PTR_LAMPTHRESHOLD)
	{//LPWORD lpwLampThreshold;
		m_strEventReceived += "WFS_USRE_PTR_LAMPTHRESHOLD:\r\n";
		m_strEventReceived += "lpwLampThreshold:\r\n";
		DWORD dwLampThreshold;
		dwLampThreshold = *(LPDWORD)pResult->lpBuffer;
		char buf[200] = {0};
		m_strEventReceived += itoa(dwLampThreshold,buf,10);
		m_strEventReceived += "\r\n";

	}
#endif
#ifdef _V2_
#else
	else if(dwEventID == WFS_USRE_PTR_INKTHRESHOLD)
	{//LPWORD lpwInkThreshold;
		m_strEventReceived += "WFS_USRE_PTR_INKTHRESHOLD:\r\n";
		m_strEventReceived += "lpwInkThreshold:\r\n";
		DWORD dwInkThreshold;
		dwInkThreshold = *(LPWORD)pResult->lpBuffer;
		char buf[200] = {0};
		m_strEventReceived += itoa(dwInkThreshold,buf,10);
		m_strEventReceived += "\r\n";
	}
#endif


	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}


LRESULT CWosaXFSTestView::OnSystemEvent(WPARAM wParam, LPARAM lParam)
{
	/*
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("SystemEvent:  ID = %d\r\n", pResult->u.dwEventID);
	m_strEventReceived += str;



	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
*/

	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("SystemEvent:  ID = %d\r\n", pResult->u.dwEventID);
	m_strEventReceived += str;

	//add by wyj 08-11-17
	/****************************
	*10.3 Device Status Changes *
	*****************************/
	DWORD dwEventID = pResult ->u.dwEventID;
	if(dwEventID == WFS_INF_PTR_STATUS)
	{
		WFSDEVSTATUS wfsDevStatus;
		wfsDevStatus = *(LPWFSDEVSTATUS)pResult ->lpBuffer;
		m_strEventReceived += "WFS_INF_PTR_STATUS:\r\n";
		m_strEventReceived += "lpszPhysicalName:\r\n";
		m_strEventReceived += wfsDevStatus.lpszPhysicalName;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpszWorkstationName:\r\n";
		m_strEventReceived += wfsDevStatus.lpszWorkstationName;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "dwState:\r\n";
		DWORD dwState;
		dwState = wfsDevStatus.dwState;
		LPSTR p;
		if(dwState == WFS_STAT_DEVONLINE)
			p ="WFS_STAT_DEVONLINE";
		else if(dwState == WFS_STAT_DEVOFFLINE)
			p = "WFS_STAT_DEVOFFLINE";
		else if(dwState == WFS_STAT_DEVPOWEROFF)
			p = "WFS_STAT_DEVPOWEROFF";
		else if(dwState == WFS_STAT_DEVNODEVICE)
			p = "WFS_STAT_DEVNODEVICE";
		else if(dwState == WFS_STAT_DEVHWERROR)
			p = "WFS_STAT_DEVHWERROR";
		else if(dwState == WFS_STAT_DEVUSERERROR)
			p = "WFS_STAT_DEVUSERERROR";
		m_strEventReceived += p;
		m_strEventReceived += "\r\n";

	}
	/*****************************
	*10.4 Undeliverable Messages *
	******************************/
	else if(dwEventID == WFS_SYSE_UNDELIVERABLE_MSG)
	{
		
		WFSUNDEVMSG wfsUndevMsg = *(LPWFSUNDEVMSG)pResult->lpBuffer;

		m_strEventReceived += "WFS_SYSE_UNDELIVERABLE_MSG:\r\n";
		m_strEventReceived += "lpszLogicalName:\r\n";
		m_strEventReceived += wfsUndevMsg .lpszLogicalName;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpszWorkstationName:\r\n";
		m_strEventReceived += wfsUndevMsg.lpszWorkstationName;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpszAppID:\r\n";
		m_strEventReceived += wfsUndevMsg.lpszAppID;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "dwSize:\r\n";
		char buf[200] = {0};
		m_strEventReceived += itoa(wfsUndevMsg.dwSize,buf,10);
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpbDescription:\r\n";
		
		/*the follow to make nil terminate*/
		if(sizeof(buf) > (sizeof(wfsUndevMsg.lpbDescription) + 2))
		{
			memcpy(buf,wfsUndevMsg.lpbDescription,sizeof(wfsUndevMsg.lpbDescription));
			m_strEventReceived += buf;
		}
		else //lower memory ,alloc and 
		{
			char *pBuffer = NULL;
			pBuffer = new char[sizeof(wfsUndevMsg.lpbDescription)+10];
			if(pBuffer != NULL)
			{
				memset(pBuffer,0,sizeof(wfsUndevMsg.lpbDescription) + 10);
				memcpy(pBuffer,wfsUndevMsg.lpbDescription,sizeof(wfsUndevMsg.lpbDescription));
				m_strEventReceived += pBuffer;
				delete pBuffer;
			}
		}
		m_strEventReceived += "\r\n";
		m_strEventReceived += "dwMsg:\r\n";
		memset(buf,0,sizeof(buf));
		m_strEventReceived += itoa(wfsUndevMsg.dwMsg,buf,10);
		m_strEventReceived += "\r\n";

		
		WFSRESULT wfsResult;
		wfsResult = *wfsUndevMsg.lpWFSResult;
		
		m_strEventReceived += "lpWFSResult:\r\n";
		m_strEventReceived += "RequestID:\r\n";
		memset(buf,0,sizeof(buf));
		m_strEventReceived += ltoa(wfsResult.RequestID,buf,10);
		m_strEventReceived += "\r\n";
		m_strEventReceived += "hService:\r\n";
		memset(buf,0,sizeof(buf));
		m_strEventReceived += itoa(wfsResult.hService,buf,10);
		m_strEventReceived += "\r\n";
		m_strEventReceived += "tsTimestamp:\r\n";
		sprintf(buf,"%d-%d-%d,%d:%d:%d:%d,week %d",wfsResult.tsTimestamp.wYear,wfsResult.tsTimestamp.wMonth,wfsResult.tsTimestamp.wDay,\
			wfsResult.tsTimestamp.wHour,wfsResult.tsTimestamp.wMinute,wfsResult.tsTimestamp.wSecond,wfsResult.tsTimestamp.wMilliseconds,\
			wfsResult.tsTimestamp.wDayOfWeek);
		m_strEventReceived += buf;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "hResult:\r\n";
		memset(buf,0,sizeof(buf));
		m_strEventReceived += ltoa(wfsResult.hResult,buf,10);
		m_strEventReceived += "\r\n";
		m_strEventReceived += "dwCommandCode or dwEventID:\r\n";
		memset(buf,0,sizeof(buf));
		m_strEventReceived += itoa(wfsResult.u.dwCommandCode,buf,10);
		m_strEventReceived += "\r\n";

	}
	/*****************************
	*10.5 Application Disconnect *
	******************************/
	else if(dwEventID == WFS_SYSE_APP_DISCONNECT)
	{
		WFSAPPDISC wfsAppDisc = *(LPWFSAPPDISC)pResult ->lpBuffer;
		m_strEventReceived += "WFS_SYSE_APP_DISCONNECT:\r\n";
		m_strEventReceived += "lpszLogicalName:\r\n";
		m_strEventReceived += wfsAppDisc.lpszLogicalName;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpszWorkstationName:\r\n";
		m_strEventReceived += wfsAppDisc.lpszWorkstationName;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpszAppID:\r\n";
		m_strEventReceived += wfsAppDisc.lpszAppID;
		m_strEventReceived += "\r\n";
	}
	/***********************************
	*10.6 Hardware and Software Errors *
	***********************************/
	else if( (dwEventID == WFS_ERR_HARDWARE_ERROR)|| (dwEventID == WFS_ERR_SOFTWARE_ERROR) ||(dwEventID == WFS_ERR_USER_ERROR))
	{
		WFSHWERROR wfsHWError;
		wfsHWError = *(LPWFSHWERROR)pResult->lpBuffer;
		if(dwEventID == WFS_ERR_HARDWARE_ERROR)
			m_strEventReceived += "WFS_ERR_HARDWARE_ERROR:\r\n";
		else if(dwEventID == WFS_ERR_SOFTWARE_ERROR)
			m_strEventReceived += "WFS_ERR_SOFTWARE_ERROR:\r\n";
		else if(dwEventID == WFS_ERR_USER_ERROR)
			m_strEventReceived += "WFS_ERR_USER_ERROR:\r\n";

		m_strEventReceived += "lpszLogicalName:\r\n";
		m_strEventReceived += wfsHWError.lpszLogicalName;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpszPhysicalName:\r\n";
		m_strEventReceived += wfsHWError.lpszPhysicalName;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpszAppID:\r\n";
		m_strEventReceived += wfsHWError.lpszAppID;
		m_strEventReceived += "\r\n";
		m_strEventReceived += "dwAction:\r\n";
		DWORD dwAction;
		dwAction = wfsHWError.dwAction;
		if(dwAction == WFS_ERR_ACT_NOACTION)
			m_strEventReceived += "WFS_ERR_ACT_NOACTION:\r\n";
		else if(dwAction == WFS_ERR_ACT_RESET)
			m_strEventReceived += "WFS_ERR_ACT_RESET:\r\n";
		else if(dwAction == WFS_ERR_ACT_SWERROR)
			m_strEventReceived += "WFS_ERR_ACT_SWERROR:\r\n";
		else if(dwAction == WFS_ERR_ACT_CONFIG)
			m_strEventReceived += "WFS_ERR_ACT_CONFIG:\r\n";
		else if(dwAction == WFS_ERR_ACT_HWCLEAR)
			m_strEventReceived += "WFS_ERR_ACT_HWCLEAR:\r\n";
		else if(dwAction == WFS_ERR_ACT_HWMAINT)
			m_strEventReceived += "WFS_ERR_ACT_HWMAINT:\r\n";
		else if(dwAction == WFS_ERR_ACT_SUSPEND)
			m_strEventReceived += "WFS_ERR_ACT_SUSPEND:\r\n";
		m_strEventReceived += "dwSize:\r\n";
		char buf[200] = {0};
		m_strEventReceived += itoa(wfsHWError.dwSize,buf,10);
		m_strEventReceived += "\r\n";
		m_strEventReceived += "lpbDescription:\r\n";
		/*the follow to make nil terminate*/
		if(sizeof(buf) > (sizeof(wfsHWError.lpbDescription) + 2))
		{
			memcpy(buf,wfsHWError.lpbDescription,sizeof(wfsHWError.lpbDescription));
			m_strEventReceived += buf;
		}
		else //lower memory ,alloc and 
		{
			char *pBuffer = NULL;
			pBuffer = new char[sizeof(wfsHWError.lpbDescription)+10];
			if(pBuffer != NULL)
			{
				memset(pBuffer,0,sizeof(wfsHWError.lpbDescription) + 10);
				memcpy(pBuffer,wfsHWError.lpbDescription,sizeof(wfsHWError.lpbDescription));
				m_strEventReceived += pBuffer;
				delete pBuffer;
			}
		}
		m_strEventReceived += "\r\n";
	}

	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;

}

LRESULT CWosaXFSTestView::OnTimerEvent(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr;
	WFSRESULT *pResult = (WFSRESULT *)lParam;

	CString str;
	str.Format("TimerEvent:  ID = %d\r\n",wParam);
	m_strEventReceived += str;
	UpdateData(FALSE);

	if(pResult) hr = m_pfnWFSFreeResult (pResult);
	
	return 0;
}




void CWosaXFSTestView::ShowButtons(long nMode)
{
	switch(nMode)
	{
			case 0:  // after clean up
			{
				GetDlgItem(IDC_BUTTON_SYNC_OPEN)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_AsycOPEN)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_CLOSE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_CLOSE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_EXECUTE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_EXECUTE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_GETINFO)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_GETINFO)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_REGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_REGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_DEREGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_DEREGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_LOCK)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_LOCK)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_UNLOCK)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_UNLOCK)->EnableWindow(FALSE);
			}
			break;
		case 1:  // after start up
			{
				GetDlgItem(IDC_BUTTON_SYNC_OPEN)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_AsycOPEN)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_SYNC_CLOSE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_CLOSE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_EXECUTE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_EXECUTE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_GETINFO)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_GETINFO)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_REGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_REGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_DEREGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_DEREGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_LOCK)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_LOCK)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_UNLOCK)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_UNLOCK)->EnableWindow(FALSE);
			}
			break;
			case 2:  // after suync  open			{
			{
				GetDlgItem(IDC_BUTTON_SYNC_OPEN)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_AsycOPEN)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_CLOSE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ASYNC_CLOSE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_EXECUTE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ASYNC_EXECUTE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_GETINFO)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ASYNC_GETINFO)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_REGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_REGISTER)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_SYNC_DEREGISTER)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ASYNC_DEREGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_LOCK)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ASYNC_LOCK)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_UNLOCK)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ASYNC_UNLOCK)->EnableWindow(FALSE);
			}
			break;
			case 3:  // after asuync  open			{
			{
				GetDlgItem(IDC_BUTTON_SYNC_OPEN)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_AsycOPEN)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_CLOSE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_CLOSE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_SYNC_EXECUTE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_EXECUTE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_SYNC_GETINFO)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_GETINFO)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ASYNC_REGISTER)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_SYNC_REGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SYNC_DEREGISTER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_DEREGISTER)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_SYNC_LOCK)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_LOCK)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_SYNC_UNLOCK)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ASYNC_UNLOCK)->EnableWindow(TRUE);
			}
			break;
	}
}

void CWosaXFSTestView::OnActionLoadExeData() 
{
	UpdateData(TRUE);
	CFileDialog dlg(TRUE,".dll");
	if(dlg.DoModal() == IDOK)
	{
		m_strExeDataFile = dlg.GetPathName();
	}
	else
		m_strExeDataFile = "";
	UpdateData(FALSE);
}

void CWosaXFSTestView::OnSettingsEnableMultiple() 
{
	m_bEnableMultipleFields = !m_bEnableMultipleFields;
	
}

void CWosaXFSTestView::OnUpdateSettingsEnableMultiple(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bEnableMultipleFields);
	
}
