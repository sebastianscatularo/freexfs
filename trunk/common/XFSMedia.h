// XFSMedia.h: interface for the CXFSMedia class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFSMEDIA_H__A62CE342_FFB5_4DDD_88B6_8C725DCAE859__INCLUDED_)
#define AFX_XFSMEDIA_H__A62CE342_FFB5_4DDD_88B6_8C725DCAE859__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MYFILE\xfsptrdata.h"

class CXFSMedia: public CObject
{
public:
	CXFSMedia();
	virtual ~CXFSMedia();
	void Serialize(CArchive& ar);

	BOOL m_nNoHead;

	CString m_strType; // GENERIC (default) MULTIPART PASSBOOK 
	CString m_strSource;
	XFS_UNIT unit;
	XFS_SIZE size;
	XFS_PRINTAREA printArea;
	XFS_RESTRICTED restricted;
	CString m_strFOLD;

	DWORD m_dwSTAGGERING;
	DWORD m_dwPAGE;
	DWORD m_dwLINES;

	CString m_strMediaName;

};

#endif // !defined(AFX_XFSMEDIA_H__A62CE342_FFB5_4DDD_88B6_8C725DCAE859__INCLUDED_)
