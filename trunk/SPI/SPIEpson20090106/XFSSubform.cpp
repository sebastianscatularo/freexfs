// XFSSubform.cpp: implementation of the CXFSSubform class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "epson2090spi.h"
#include "XFSForm.h"
#include "XFSSubform.h"
#include "XFSField.h"
#include "XFSFrame.h"
#include "XFSMedia.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXFSSubform::CXFSSubform()
{
	m_nNoHead = FALSE;
	m_dwPage = 0;


	//xfs initialize 
	size.m_wWidth = 0;
	size.m_wHeight = 0;

	m_pRootForm = 0;
}

CXFSSubform::~CXFSSubform()
{
	while(!m_listFields.IsEmpty())
	{
		CXFSField *pXFSField = (CXFSField *)m_listFields.RemoveHead();
		delete pXFSField;
	}

	while(!m_listFrames.IsEmpty())
	{
		CXFSFrame *pXFSFrame = (CXFSFrame *)m_listFrames.RemoveHead();
		delete pXFSFrame;
	}
}

void CXFSSubform::Serialize(CArchive &ar)
{
	CString str;
	if(ar.IsStoring())
	{
		ar.Write("XFSSUBFORM", strlen("XFSSUBFORM"));
		ar.Write(" \"", 2);
		ar.Write(m_strSubFormName,m_strSubFormName.GetLength());
		ar.Write("\"\r\n", 3);
		ar.Write(" BEGIN\r\n", strlen(" BEGIN\r\n"));

		// POSITION
		if(m_dwPage == 0)
		{
			str.Format("  POSITION %d %d",position.m_wX,position.m_wY);
		}
		else
		{
			str.Format("  POSITION %d, (%d, %d)",position.m_wX,position.m_wY, m_dwPage);
		}
		ar.Write(str,str.GetLength());
		ar.Write("\r\n", 2);
		// size
		if(size.m_wWidth != 0 && size.m_wHeight!=0) str.Format("  SIZE %d, %d",size.m_wWidth,size.m_wHeight);
		ar.Write(str,str.GetLength());
		ar.Write("\r\n", 2);
		// field list
		{
			POSITION pos = m_listFields.GetHeadPosition();
			while(pos)
			{
				CXFSField *pXFSField = (CXFSField *)m_listFields.GetNext(pos);
				pXFSField->Serialize(ar);
			}
		}

		// frame list
		{
			POSITION pos = m_listFrames.GetHeadPosition();
			while(pos)
			{
				CXFSFrame *pXFSFrame = (CXFSFrame *)m_listFrames.GetNext(pos);
				pXFSFrame->Serialize(ar);
			}
		}

		ar.Write(" END\r\n", strlen(" END\r\n"));
	}
	else
	{
		CString str;
		BOOL bRet = FALSE;
		long nIndex = 0;
		if(!m_nNoHead)
		{
			bRet = ar.ReadString(str);
			if(!bRet) AfxThrowFileException(CFileException::generic);
			nIndex = str.Find("XFSSUBFORM");
			str = str.Mid(nIndex + strlen("XFSSUBFORM"));
			nIndex = str.Find("\"");
			str = str.Mid(nIndex + 1);
			nIndex = str.Find("\"");
			str = str.Left(nIndex);
			//strcpy(subformname,(LPCSTR)str);
			m_strSubFormName = str;
		}
		bRet = ar.ReadString(str);  // BEGIN
		if(!bRet) AfxThrowFileException(CFileException::generic);
		while(1)
		{
			bRet = ar.ReadString(str);
			if(!bRet) AfxThrowFileException(CFileException::generic);
			str.TrimLeft();
			str.TrimRight();
			if(str == "END") break;
			long n = str.Find(" ");
			if(n > 0)
			{
				CString strAttribute = str.Left(n);
				str = str.Mid(n + 1);
				str.TrimLeft();
				if(strAttribute == "POSITION")
				{
					long n = str.Find("(");
					if(n < 0)
					{
						m_dwPage = 0;
						sscanf(str,"%d,%d",&position.m_wX,&position.m_wY);
					}
					else
					{
						long n = str.Find("POSITION");
						str = str.Mid(n + strlen("POSITION"));
						sscanf(str,"%d,(%d,%d)",&position.m_wX,&position.m_wY,&m_dwPage);
					}
				}
				if(strAttribute == "SIZE")
				{
					sscanf(str,"%d,%d",	&size.m_wWidth ,&size.m_wHeight);
				}

				if(strAttribute == "XFSFRAME")
				{
					CXFSFrame *pXFSFrame = new CXFSFrame;
					if(pXFSFrame->m_nNoHead == FALSE)
					{
						pXFSFrame->m_nNoHead = TRUE;
					}
					m_listFrames.AddTail(pXFSFrame);

					nIndex = str.Find("\"");
					str = str.Mid(nIndex + 1);
					nIndex = str.Find("\"");
					str = str.Left(nIndex);
					//strcpy(pXFSFrame->framename,(LPCSTR)str);
					pXFSFrame->m_strFrameName = str;
					pXFSFrame->m_pRootForm = this->m_pRootForm;
					pXFSFrame->m_pParentSubform = this;

					pXFSFrame->Serialize(ar);
				}

				if(strAttribute == "XFSFIELD")
				{
					CXFSField *pXFSField = new CXFSField;
					if(pXFSField->m_nNoHead == FALSE)
					{
						pXFSField->m_nNoHead = TRUE;
					}
					m_listFields.AddTail(pXFSField);

					nIndex = str.Find("\"");
					str = str.Mid(nIndex + 1);
					nIndex = str.Find("\"");
					str = str.Left(nIndex);
					//strcpy(pXFSField->fieldname,(LPCSTR)str);
					pXFSField->m_strFieldName = str;
					pXFSField->m_pRootForm = this->m_pRootForm;
					pXFSField->m_pParentSubform = this;

					pXFSField->Serialize(ar);
				}
				
			}
		}


		//bRet = ar.ReadString(str);  // END

	}
}

BOOL CXFSSubform::ExtractFieldValue(LPCSTR psz)
{
	BOOL bRet = TRUE;
	POSITION pos = m_listFields.GetHeadPosition();
	while(pos)
	{
		CXFSField *pXFSField = (CXFSField *)m_listFields.GetNext(pos);
		bRet = bRet && pXFSField->ExtractFieldValue(psz);
	}
	return bRet;
}

BOOL CXFSSubform::Draw(CDC *pDC)
{
	BOOL bRet = TRUE;
	POSITION pos = m_listFrames.GetHeadPosition();
	while(pos)
	{
		CXFSFrame *pXFSFrame = (CXFSFrame *)m_listFrames.GetNext(pos);
		bRet = bRet && pXFSFrame->Draw(pDC);
	}

	pos = m_listFields.GetHeadPosition();
	while(pos)
	{
		CXFSField *pXFSField = (CXFSField *)m_listFields.GetNext(pos);
		bRet = bRet && pXFSField->Draw(pDC);
	}
	return bRet;
}

CXFSField * CXFSSubform::GetFieldByName(CString strFieldName)
{
	CXFSField *pField = 0;
	POSITION pos = m_listFields.GetHeadPosition();
	while(pos)
	{
		pField = (CXFSField *)m_listFields.GetNext(pos);
		if(pField->m_strFieldName == strFieldName) break;
	}
	return pField;
}

HRESULT CXFSSubform::CheckAgainstMedia(CXFSMedia *pXFSMedia)
{
	// To Do

	HRESULT hr = 0;
	POSITION pos = m_listFields.GetHeadPosition();
	while(pos)
	{
		CXFSField *pField = (CXFSField *)m_listFields.GetNext(pos);
		hr |= pField->CheckAgainstMedia(pXFSMedia);
	}

	pos = m_listFrames.GetHeadPosition();
	while(pos)
	{
		CXFSFrame *pXFSFrame = (CXFSFrame *)m_listFrames.GetNext(pos);
		hr |= pXFSFrame->CheckAgainstMedia(pXFSMedia);
	}

	return S_OK;
}
