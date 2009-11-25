// XFSFrame.h: interface for the CXFSFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFSFRAME_H__A429AC88_714B_4A24_8421_F438C292CB52__INCLUDED_)
#define AFX_XFSFRAME_H__A429AC88_714B_4A24_8421_F438C292CB52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MYFILE\xfsptrdata.h"
class CXFSForm;
class CXFSSubform;
class CXFSMedia;

class CXFSFrame: public CObject 
{
public:
	HRESULT CheckAgainstMedia(CXFSMedia *pXFSMedia);
	void Serialize(CArchive& ar);
	CXFSFrame();
	virtual ~CXFSFrame();

	BOOL m_nNoHead;

	CXFSForm *m_pRootForm;
	CXFSSubform *m_pParentSubform;

	BOOL CXFSFrame::Draw(CDC *pDC);

	long m_nPositionX;
	long m_nPositionY;

	XFS_POSITION position;
	CString m_strFrames;
	CString m_strHeader;
	CString m_strFooter;
	CString m_strSide;

	XFS_SIZE size;
	XFS_REPEATONX repeatonx;
	XFS_REPEATONY repeatony;
	
	CString m_strType;
	CString m_strClass;
	CString m_strOverflow;
	CString m_strStyle;
	CString m_strColor;
	XFS_RGBCOLOR rgbcolor;
	CString m_strFillColor;
	XFS_RGBFILLCOLOR rgbfillcolor;	
	CString m_strSubStsign;
	CString m_strTitle;
	CString m_strHorizontal;
	CString m_strVertical;
	CString m_strFillStyle;
	///extra
	CString m_strFrameName;
	DWORD m_dwPage;

};

#endif // !defined(AFX_XFSFRAME_H__A429AC88_714B_4A24_8421_F438C292CB52__INCLUDED_)
