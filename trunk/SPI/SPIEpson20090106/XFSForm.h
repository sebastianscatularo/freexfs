// XFSForm.h: interface for the CXFSForm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFSFORM_H__F87074CE_1365_467B_A37F_7C286D1E2462__INCLUDED_)
#define AFX_XFSFORM_H__F87074CE_1365_467B_A37F_7C286D1E2462__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MYFILE\xfsptrdata.h"

class CXFSField;
class CXFSMedia;

class CXFSForm: public CObject
{
public:
	float m_lYExt;
	float m_lXExt;
	HRESULT CheckAgainstMedia(CXFSMedia *pXFSMedia);
	CXFSField * GetFieldByName(CString strFieldName);
	void Serialize(CArchive& ar);
	CXFSForm();
	virtual ~CXFSForm();

	BOOL m_nNoHead;

	BOOL ExtractFieldValue(LPCSTR psz);
	BOOL Draw(CDC *pDC);

	int m_nSizeX; // drawing area in pixels 
	int m_nSizeY; // drawing area in pixels 

		XFS_UNIT unit;
	XFS_SIZE size;
	XFS_ALIGNMENT alignment;

	XFS_ALIGNMENT alignmentWorking;
	
	CString m_strOrientation;
	DWORD       m_dwSkew;
	XFS_VERSION version;
	DWORD       m_dwLanguage;
	CString m_strCopyRight;
	CString m_strTitle;
	CString m_strComment;
	CString m_strUserPrompt;

	//WFSPTRFORM_FIELD    *pfield;///every fieldname take 100 byte spaces 
	//WFSPTRFORM_FRAME    *pframe;//as above
	//WFSPTRFORM_SUBFORM  *psubform;//as above
	CPtrList m_listFields;
	CPtrList m_listFrames;
	CPtrList m_listSubforms;
	CString m_strFormName;
	//////extra;

	DWORD       m_dwPage;
	DWORD       dwField;////the number of fields
	DWORD       dwFrame;////the number of frames
	DWORD       dwSubForm;////the number of subformnames;
	DWORD       dwSize;//size of this struct 


};

#endif // !defined(AFX_XFSFORM_H__F87074CE_1365_467B_A37F_7C286D1E2462__INCLUDED_)
