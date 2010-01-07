// WosaXFSTestView.h : interface of the CWosaXFSTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WOSAXFSTESTVIEW_H__5352B80D_E8E3_4B50_B015_0828FB20A8D2__INCLUDED_)
#define AFX_WOSAXFSTESTVIEW_H__5352B80D_E8E3_4B50_B015_0828FB20A8D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef _V2_
#include "..\..\XFS SDK2.0\XFSADMIN.H"
#include "..\..\XFS SDK2.0\Xfsconf.h"
#include "..\..\XFS SDK2.0\XFSAPI.H"
#include "..\..\XFS SDK2.0\xfsptr.h"
#else
#include "..\..\Manager\NI_XFSAPI.h"
#include "..\..\Manager\NI_XFSManager.h"
#include "xfsptr.h"

#endif

class CWosaXFSTestView : public CFormView
{
protected: // create from serialization only
	CWosaXFSTestView();
	DECLARE_DYNCREATE(CWosaXFSTestView)

public:
	//{{AFX_DATA(CWosaXFSTestView)
	enum { IDD = IDD_WOSAXFSTEST_FORM };
	CComboBox	m_comboFields;
	CComboBox	m_comoMedia;
	CComboBox	m_comboService;
	CComboBox	m_comboForms;
	CString	m_strResult;
	CString	m_strVersionSPI;
	CString	m_strVersionXFS;
	CString	m_strXFSPath;
	CString	m_strFormName;
	CString	m_strLocalService;
	CString	m_strSelectField;
	CString	m_strSelectedMedia;
	CString	m_strDevClass;
	CString	m_strVersionSPI_H;
	CString	m_strVersionSPI_L;
	CString	m_strVersionXFS_H;
	CString	m_strVersionXFS_L;
	CString	m_strEventReceived;
	UINT	m_nEventRangeHigh;
	UINT	m_nEventRangeLow;
	//}}AFX_DATA

// Attributes
public:
	CWosaXFSTestDoc* GetDocument();

	REQUESTID m_IPAsyncOpen;
	REQUESTID m_IPAsyncOpenGetInfo;
	REQUESTID m_IPGetInfoAsyncExecute;
	REQUESTID m_IPAsyncExecute;
	REQUESTID m_IPAsyncGetInfo;

	WFSPTRPRINTFORM m_wfsForm;
	CString m_strTextOutExecute;
	LPTSTR m_pTextOutExecute;

	DWORD m_dwCategoryIndex; // for get info
	DWORD m_dwCommandIndex; 
	
	CString m_strExeDataFile;// for get info

	HAPP m_hAppSync;
// Operations
public:
	HSERVICE *m_pAsyncService;
	HSERVICE m_hSyncService;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWosaXFSTestView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bEnableMultipleFields;
	void ShowButtons(long nMode);
	void CWosaXFSTestView::ShowFromFieldList(LPWFSFRMFIELD *lpFromFields);
	void CWosaXFSTestView::ShowMediaField(WFSFRMMEDIA* lpWfsMedia);
	void UpdateVersion();
	CString MakeVersion(CString strLow, CString strHigh);

	void CWosaXFSTestView::OnButtonSyncExecute_PrintForm(); 
	void CWosaXFSTestView::OnButtonAsyncExecute_PrintForm();

	void CWosaXFSTestView::OnButtonSyncExecute_ControlMedia();
	
	void CWosaXFSTestView::ScrollResult(); 
	void CWosaXFSTestView::ShowFromField(LPWFSFRMFIELD lpFromField);
	CString CWosaXFSTestView::GetFrmAccess(WORD fwAccess); 
	CString CWosaXFSTestView::GetServiceClass();
	HRESULT CWosaXFSTestView::GetLogicalServiceV2();
	HRESULT CWosaXFSTestView::GetLogicalServiceV3();
	void CWosaXFSTestView::ShowFormHeader(LPWFSFRMHEADER lpHeader);
	void CWosaXFSTestView::ShowStatus(LPWFSPTRSTATUS lpStatus);
	void CWosaXFSTestView::ShowCaps(LPWFSPTRCAPS lpCaps);
	CString CWosaXFSTestView::GetCharSupport(WORD fwCharSupport); 
	CString CWosaXFSTestView::GetImageSource(WORD fwImageSource);
	CString CWosaXFSTestView::GetCodelineFormat(WORD fwCodelineFormat); 
	CString CWosaXFSTestView::GetImageColorFormat(WORD fwImageColorFormat); 
	CString CWosaXFSTestView::GetImageType(WORD fwImageType); 
	CString CWosaXFSTestView::GetPaperSources(WORD fwPaperSources); 
	CString CWosaXFSTestView::GetControl(WORD fwControl); 
	CString CWosaXFSTestView::GetExtents(WORD fwExtents);
	CString CWosaXFSTestView::GetWriteForm(WORD fwWriteForm);
	CString CWosaXFSTestView::GetReadForm(WORD fwReadForm); 
	CString CWosaXFSTestView::GetCapResolutions(WORD wResolution); 
	CString GetCapTypes(WORD fwType); 
	BOOL ExecuteScripts(CString strFilePath);
	void WriteScript(CString strVerb, CString strAction, CString strParams );
	CString m_strWorkingDir;
	CString m_strStartup;
	HMODULE m_hLib;
	BOOL m_bStartUp;
	CString CWosaXFSTestView::FindXMLManagerPath();
	BOOL LoadManagerFunction(CString strPath);
	virtual ~CWosaXFSTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWosaXFSTestView)
	afx_msg void OnButton1();
	afx_msg void OnBUTTONAsycOPEN();
	afx_msg LRESULT OnCompleteOpen(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteExecute(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteGetInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteRegister(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteDeregister(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteLock(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteUnock(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTimerEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSystemEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnServiceEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExecuteEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonSyncClose();
	afx_msg void OnButtonAsyncClose();
	afx_msg void OnButtonSyncOpen();
	afx_msg void OnButtonAsyncExecute();
	afx_msg void OnButtonSyncExecute();
	afx_msg void OnButtonSyncGetinfo();
	afx_msg void OnButtonAsyncGetinfo();
	afx_msg void OnButtonSyncRegister();
	afx_msg void OnButtonAsyncRegister();
	afx_msg void OnButtonAsyncDeregister();
	afx_msg void OnButtonAsyncLock();
	afx_msg void OnButtonAsyncUnlock();
	afx_msg void OnButtonSyncDeregister();
	afx_msg void OnButtonSyncLock();
	afx_msg void OnButtonSyncUnlock();
	afx_msg void OnDestroy();
	afx_msg void OnButtonStartUp();
	afx_msg void OnButtonCleanUp();
	afx_msg void OnBUTTONXFSMgr();
	afx_msg void OnSettingsWorkdir();
	afx_msg void OnActionScripts();
	afx_msg void OnSETTINGSGetInfo();
	afx_msg void OnKillfocusComboService();
	afx_msg void OnSelchangeComboService();
	afx_msg void OnEditchangeComboService();
	afx_msg void OnButtonExeCmd();
	afx_msg void OnButtonGetinfoCat();
	afx_msg void OnSETTINGSExecute();
	afx_msg void OnChangeEditVersionSpiH();
	afx_msg void OnChangeEditVersionSpiL();
	afx_msg void OnChangeEditVersionXfsH();
	afx_msg void OnChangeEditVersionXfsL();
	afx_msg void OnActionLoadExeData();
	afx_msg void OnSettingsEnableMultiple();
	afx_msg void OnUpdateSettingsEnableMultiple(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WosaXFSTestView.cpp
inline CWosaXFSTestDoc* CWosaXFSTestView::GetDocument()
   { return (CWosaXFSTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WOSAXFSTESTVIEW_H__5352B80D_E8E3_4B50_B015_0828FB20A8D2__INCLUDED_)
