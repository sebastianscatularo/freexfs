// SelGetInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wosaxfstest.h"
#include "SelGetInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelGetInfoDlg dialog


CSelGetInfoDlg::CSelGetInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelGetInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelGetInfoDlg)
	m_nRadioGetInfo = -1;
	//}}AFX_DATA_INIT
}


void CSelGetInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelGetInfoDlg)
	DDX_Radio(pDX, IDC_RADIO_WFS_INF_PTR_STATUS, m_nRadioGetInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelGetInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CSelGetInfoDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelGetInfoDlg message handlers
