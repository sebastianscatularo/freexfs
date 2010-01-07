// XFSSubform.h: interface for the CXFSSubform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFSSUBFORM_H__1DC86DDE_9CA0_4A18_90B7_0F37BDD6E4AF__INCLUDED_)
#define AFX_XFSSUBFORM_H__1DC86DDE_9CA0_4A18_90B7_0F37BDD6E4AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MYFILE\xfsptrdata.h"

class CXFSForm;
class CXFSField;

class CXFSSubform 
{
public:
	HRESULT CheckAgainstMedia(CXFSMedia *pXFSMedia);
	CXFSField * GetFieldByName(CString strFieldName);
	void Serialize(CArchive& ar);
	CXFSSubform();
	virtual ~CXFSSubform();

	BOOL m_nNoHead;

	BOOL ExtractFieldValue(LPCSTR psz);

	CXFSForm *m_pRootForm;

	BOOL CXFSSubform::Draw(CDC *pDC);

		XFS_POSITION position;
	XFS_SIZE size;
	
	//WFSPTRFORM_FIELD   *m_pNextField;//point to 
    //WFSPTRFORM_FRAME   *m_pNextFrame;

	CPtrList m_listFields;
	CPtrList m_listFrames;

	//extra
	CString m_strSubFormName;

	DWORD       m_dwFields;//number of field
	DWORD       m_dwFrames;
	DWORD       m_dwSize;//size of this struct

	DWORD       m_dwPage;///?	

};

#endif // !defined(AFX_XFSSUBFORM_H__1DC86DDE_9CA0_4A18_90B7_0F37BDD6E4AF__INCLUDED_)
