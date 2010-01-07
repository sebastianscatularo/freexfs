#if !defined(AFX_SELECTCMDDLG_H__381FB1EA_E78D_4C5B_8817_61039541D92D__INCLUDED_)
#define AFX_SELECTCMDDLG_H__381FB1EA_E78D_4C5B_8817_61039541D92D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectCmdDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectCmdDlg dialog

class CSelectCmdDlg : public CDialog
{
// Construction
public:
	CSelectCmdDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectCmdDlg)
	enum { IDD = IDD_DIALOG_SELECT_CMD };
	int		m_nCommandIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectCmdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectCmdDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTCMDDLG_H__381FB1EA_E78D_4C5B_8817_61039541D92D__INCLUDED_)
