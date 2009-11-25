// SelectCmdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wosaxfstest.h"
#include "SelectCmdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectCmdDlg dialog


CSelectCmdDlg::CSelectCmdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectCmdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectCmdDlg)
	m_nCommandIndex = 1;
	//}}AFX_DATA_INIT
}


void CSelectCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectCmdDlg)
	DDX_Radio(pDX, IDC_RADIO_CONTROL_MEDIA, m_nCommandIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectCmdDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectCmdDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectCmdDlg message handlers
