#if !defined(AFX_SELGETINFODLG_H__4C9ECFAF_CDF0_4F22_84AA_EC1156C7553C__INCLUDED_)
#define AFX_SELGETINFODLG_H__4C9ECFAF_CDF0_4F22_84AA_EC1156C7553C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelGetInfoDlg.h : header file
//
// CEdit
/////////////////////////////////////////////////////////////////////////////
// CSelGetInfoDlg dialog

class CSelGetInfoDlg : public CDialog
{
// Construction
public:
	CSelGetInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelGetInfoDlg)
	enum { IDD = IDD_DIALOG_SELECT_GETINFO };
	int		m_nRadioGetInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelGetInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelGetInfoDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELGETINFODLG_H__4C9ECFAF_CDF0_4F22_84AA_EC1156C7553C__INCLUDED_)
