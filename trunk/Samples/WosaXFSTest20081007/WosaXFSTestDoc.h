// WosaXFSTestDoc.h : interface of the CWosaXFSTestDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WOSAXFSTESTDOC_H__B67D6DB5_42DD_429C_8D3B_3CC99DEEEF47__INCLUDED_)
#define AFX_WOSAXFSTESTDOC_H__B67D6DB5_42DD_429C_8D3B_3CC99DEEEF47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWosaXFSTestDoc : public CDocument
{
protected: // create from serialization only
	CWosaXFSTestDoc();
	DECLARE_DYNCREATE(CWosaXFSTestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWosaXFSTestDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWosaXFSTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWosaXFSTestDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WOSAXFSTESTDOC_H__B67D6DB5_42DD_429C_8D3B_3CC99DEEEF47__INCLUDED_)
