// TestPTROcxDoc.h : interface of the CTestPTROcxDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTPTROCXDOC_H__68614F14_C6C3_484E_8245_0B9D4E003297__INCLUDED_)
#define AFX_TESTPTROCXDOC_H__68614F14_C6C3_484E_8245_0B9D4E003297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestPTROcxDoc : public CDocument
{
protected: // create from serialization only
	CTestPTROcxDoc();
	DECLARE_DYNCREATE(CTestPTROcxDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestPTROcxDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestPTROcxDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestPTROcxDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPTROCXDOC_H__68614F14_C6C3_484E_8245_0B9D4E003297__INCLUDED_)
