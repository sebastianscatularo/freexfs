// TestPTROcxDoc.cpp : implementation of the CTestPTROcxDoc class
//

#include "stdafx.h"
#include "TestPTROcx.h"

#include "TestPTROcxDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestPTROcxDoc

IMPLEMENT_DYNCREATE(CTestPTROcxDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestPTROcxDoc, CDocument)
	//{{AFX_MSG_MAP(CTestPTROcxDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestPTROcxDoc construction/destruction

CTestPTROcxDoc::CTestPTROcxDoc()
{
	// TODO: add one-time construction code here

}

CTestPTROcxDoc::~CTestPTROcxDoc()
{
}

BOOL CTestPTROcxDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestPTROcxDoc serialization

void CTestPTROcxDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTestPTROcxDoc diagnostics

#ifdef _DEBUG
void CTestPTROcxDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestPTROcxDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestPTROcxDoc commands
