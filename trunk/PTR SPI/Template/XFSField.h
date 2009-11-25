// XFSField.h: interface for the CXFSField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFSFIELD_H__A266DAD5_841E_45F8_8272_D13E301487E6__INCLUDED_)
#define AFX_XFSFIELD_H__A266DAD5_841E_45F8_8272_D13E301487E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MYFILE\xfsptrdata.h"

class CXFSForm;
class CXFSSubform;
class CXFSMedia;

class CXFSField: public CObject //: public _wfs_ptr_form_field  
{
public:
	HRESULT CheckAgainstMedia(CXFSMedia *pXFSMedia);
	BOOL Draw(CDC *pDC);
	BOOL ExtractFieldValue(LPCSTR psz);
	void Serialize(CArchive& ar);
	CXFSField();
	virtual ~CXFSField();

	BOOL m_nNoHead;

	CString m_strFieldValue;

	CXFSForm *m_pRootForm;
	CXFSSubform *m_pParentSubform;

	long m_nPositionX;
	long m_nPositionY;
	long m_nWidth;
	long m_nHeight;


	//

	XFS_POSITION position;

	CString m_strFollows;
	CString m_strHeader;
	CString m_strFooter;
	
	CString m_strSide;

	XFS_SIZE size;
	XFS_INDEX index;

	CString m_strType;
	CString m_strScaling;
	CString m_strBarcode;
	CString m_strClass;
	CString m_strAccess;
	CString m_strOverflow;


	CString m_strStyle;
	CString m_strCase;
	CString m_strHorizontal;
	CString m_strVertical;
	CString m_strColor;
	
	XFS_RGBCOLOR rgbcolor;
	DWORD       m_dwLanguage;
	CString m_strFont;

	DWORD       m_dwPointSize;
	DWORD       m_dwCpi;
	DWORD       m_dwLpi;
	CString m_strFormat;
	CString m_strInitialValue;

	///extra
	CString m_strFieldName;
	//_wfs_ptr_form_field *pNext;

	DWORD m_dwPage;

	long m_nIndex;

};

#endif // !defined(AFX_XFSFIELD_H__A266DAD5_841E_45F8_8272_D13E301487E6__INCLUDED_)
