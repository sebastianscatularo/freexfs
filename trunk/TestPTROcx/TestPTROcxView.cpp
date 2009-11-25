// TestPTROcxView.cpp : implementation of the CTestPTROcxView class
//

#include "stdafx.h"
#include "TestPTROcx.h"

#include "TestPTROcxDoc.h"
#include "TestPTROcxView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestPTROcxView

IMPLEMENT_DYNCREATE(CTestPTROcxView, CFormView)

BEGIN_MESSAGE_MAP(CTestPTROcxView, CFormView)
	//{{AFX_MSG_MAP(CTestPTROcxView)
	ON_BN_CLICKED(IDC_BUTTON_PRINTFORM, OnButtonPrintform)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestPTROcxView construction/destruction

CTestPTROcxView::CTestPTROcxView()
	: CFormView(CTestPTROcxView::IDD)
{
	//{{AFX_DATA_INIT(CTestPTROcxView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CTestPTROcxView::~CTestPTROcxView()
{
}

void CTestPTROcxView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestPTROcxView)
	DDX_Control(pDX, IDC_XFSPRINT1, m_xfsPrint);
	//}}AFX_DATA_MAP
}

BOOL CTestPTROcxView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CTestPTROcxView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CTestPTROcxView printing

BOOL CTestPTROcxView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestPTROcxView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestPTROcxView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTestPTROcxView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CTestPTROcxView diagnostics

#ifdef _DEBUG
void CTestPTROcxView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTestPTROcxView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTestPTROcxDoc* CTestPTROcxView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestPTROcxDoc)));
	return (CTestPTROcxDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestPTROcxView message handlers

void CTestPTROcxView::OnButtonPrintform() 
{
    m_xfsPrint.SetXFSMgrPath("msxfs.dll");
    m_xfsPrint.SetLogicalService("BouncedChecks");
    m_xfsPrint.SetFormName("NewForm1");
    m_xfsPrint.AddField("0", "051681111");
    m_xfsPrint.AddField("1", "31");
    m_xfsPrint.AddField("2", "0000148");
    m_xfsPrint.AddField("3", "256,325");
    m_xfsPrint.AddField("4", "0098");
    m_xfsPrint.AddField("5", "01");
    m_xfsPrint.AddField("6", "01");
    m_xfsPrint.AddField("7", "0098");
    m_xfsPrint.AddField("8", "09");
    m_xfsPrint.AddField("9", "10");
    m_xfsPrint.AddField("10", "1");
    m_xfsPrint.AddField("11", "010100210");
    m_xfsPrint.AddField("12", "僑銀三重");
    m_xfsPrint.AddField("13", "99 字跡模糊");
    m_xfsPrint.AddField("14", "");
    m_xfsPrint.AddField("15", "");
    m_xfsPrint.AddField("16", "A123456789");
    m_xfsPrint.AddField("17", "");
    m_xfsPrint.AddField("18", "");
    m_xfsPrint.AddField("19", "台北市台北縣XXXXXXXXXXXX");
    m_xfsPrint.AddField("20", "XXXXXXXXXXXXXXXXXX");
    m_xfsPrint.AddField("21", "066");
    m_xfsPrint.AddField("22", "02");
    m_xfsPrint.AddField("23", "28");
    m_xfsPrint.AddField("24", "010050452");
    m_xfsPrint.DoPrint();
}


