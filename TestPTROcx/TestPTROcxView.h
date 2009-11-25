// TestPTROcxView.h : interface of the CTestPTROcxView class
//
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INCLUDES()
#include "xfsprint.h"
//}}AFX_INCLUDES

#if !defined(AFX_TESTPTROCXVIEW_H__294ECC1C_BD7B_4C93_ABBD_8134E3F1BCB3__INCLUDED_)
#define AFX_TESTPTROCXVIEW_H__294ECC1C_BD7B_4C93_ABBD_8134E3F1BCB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestPTROcxView : public CFormView
{
protected: // create from serialization only
	CTestPTROcxView();
	DECLARE_DYNCREATE(CTestPTROcxView)

public:
	//{{AFX_DATA(CTestPTROcxView)
	enum { IDD = IDD_TESTPTROCX_FORM };
	CXFSPrint	m_xfsPrint;
	//}}AFX_DATA

// Attributes
public:
	CTestPTROcxDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestPTROcxView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestPTROcxView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestPTROcxView)
	afx_msg void OnButtonPrintform();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TestPTROcxView.cpp
inline CTestPTROcxDoc* CTestPTROcxView::GetDocument()
   { return (CTestPTROcxDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPTROCXVIEW_H__294ECC1C_BD7B_4C93_ABBD_8134E3F1BCB3__INCLUDED_)
