// Createor.h: interface for the CCreateor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEOR_H__8CEE8C1E_A910_42F5_95C3_67A0958BBF28__INCLUDED_)
#define AFX_CREATEOR_H__8CEE8C1E_A910_42F5_95C3_67A0958BBF28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCreateor  
{
public:
	void * CreateChild(CArchive &ar, long *nType);
	CCreateor();
	virtual ~CCreateor();

};

#endif // !defined(AFX_CREATEOR_H__8CEE8C1E_A910_42F5_95C3_67A0958BBF28__INCLUDED_)
