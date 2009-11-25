// XFSForm.cpp: implementation of the CXFSForm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "$$root$$.h"
#include "XFSForm.h"
#include "XFSField.h"
#include "XFSFrame.h"
#include "XFSSubform.h"
#include "XFSMedia.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXFSForm::CXFSForm():m_listFields(),m_listFrames(),m_listSubforms()
{
		m_nNoHead = FALSE;

		/// xfs initialize 
		unit.m_strBase = "";
		unit.m_wX = 0;
		unit.m_wY = 0;
		size.m_wWidth = 0;
		size.m_wHeight = 0;
		alignment.m_strAlignment = "TOPLEFT";
		alignment.m_wXoffset = 0;
		alignment.m_wYoffset = 0;
		m_strOrientation = "PORTRAIT";
		m_dwSkew = 0;
		version.m_wMajor = 0;
		version.m_wMinor = 0;
		version.m_strDate = "";
		version.m_strAuthor = "";
		m_dwLanguage = 0x0409;
		m_strCopyRight = "";
		m_strTitle = "";
		m_strComment = "";
		m_strUserPrompt = "";

		m_nSizeX = 0; // drawing area in pixels 
		m_nSizeY = 0; // drawing area in pixels 

		m_dwPage = 0;
		m_lXExt = 3.75;
		m_lYExt = 1.875;
}

CXFSForm::~CXFSForm()
{

	CXFSField *pXFSField = NULL;
	CXFSFrame *pXFSFrame = NULL;
	CXFSSubform *pXFSSubform = NULL;
	
	while(!m_listFields.IsEmpty())
	{
		pXFSField = (CXFSField*)m_listFields.RemoveHead();
		delete pXFSField;
	}

	while(!m_listFrames.IsEmpty())
	{
		pXFSFrame = (CXFSFrame*)m_listFrames.RemoveHead();
		delete pXFSFrame;
	}

	while(!m_listSubforms.IsEmpty())
	{
		pXFSSubform = (CXFSSubform*)m_listSubforms.RemoveHead();
		delete pXFSSubform;
	}
}

void CXFSForm::Serialize(CArchive &ar)
{
	CString str = "";
	if(ar.IsStoring())
	{
		ar.Write("XFSFORM", strlen("XFSFORM"));
		ar.Write(" \"", 2);
		ar.Write(m_strFormName,m_strFormName.GetLength());
		ar.Write("\"\r\n", 3);
		ar.Write(" BEGIN\r\n", strlen(" BEGIN\r\n"));

		// UNIT
		if(unit.m_wX != 0 && unit.m_wX !=0) 
		{
			str.Format("  UNIT %s, %d, %d",unit.m_strBase,unit.m_wX,unit.m_wY);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		// size
		if(size.m_wWidth != 0 && size.m_wHeight!=0) 
		{
			str.Format("  SIZE %d, %d",size.m_wWidth,size.m_wHeight);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(alignment.m_strAlignment != "")
		{
			str.Format(" ALIGNMENT %s,%d,%d",alignment.m_strAlignment,alignment.m_wXoffset,alignment.m_wYoffset);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		if(m_strOrientation != "")
		{
			str.Format("  ORIENTATION %s",m_strOrientation);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(m_dwSkew != 0)
		{
			str.Format("  SKEW %d",m_dwSkew);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(version.m_wMajor || version.m_wMinor || version.m_strDate != "" || version.m_strAuthor != "")
		{
			str.Format("  VERSION %d,%d,%s,%s",version.m_wMajor,version.m_wMinor,version.m_strDate,\
				version.m_strAuthor);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(m_strOrientation != "")
		{
			str.Format("  ORIENTATION %s",m_strOrientation);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(m_strOrientation != "")
		{
			str.Format("  ORIENTATION %s",m_strOrientation);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		str.Format(" LANGUAGE %d",m_dwLanguage);
		ar.Write(str,str.GetLength());
		ar.Write("\r\n",2);

		if(m_strCopyRight != "")
		{
			str.Format("  COPYRIGHT %s",m_strCopyRight);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(m_strTitle != "")
		{
			str.Format("  TITLE %s",m_strTitle);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(m_strComment != "")
		{
			str.Format("  COMMENT %s",m_strComment);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(m_strUserPrompt != "")
		{
			str.Format("  USERPROMPT %s",m_strUserPrompt);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		// Do   :  

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

		// Subforms list
		{
			POSITION pos = m_listSubforms.GetHeadPosition();
			while(pos)
			{
				CXFSSubform *pXFSSubform = (CXFSSubform *)m_listSubforms.GetNext(pos);
				pXFSSubform->Serialize(ar);
			}
		}
		ar.Write(" END\r\n", strlen(" END\r\n"));
	}
	else
	{
		//CString str;
		BOOL bRet = FALSE;
		long nIndex = 0;
		if(!m_nNoHead)
		{
			bRet = ar.ReadString(str);
			nIndex = str.Find("XFSFORM");
			while(1)
			{
				if(nIndex == -1)/////
				{
					bRet = ar.ReadString(str);
					if(!bRet)
						return;//
					nIndex = str.Find("XFSFORM");
				}
				else
					break;
			}
			str = str.Mid(nIndex + strlen("XFSFORM"));
			nIndex = str.Find("\"");
			str = str.Mid(nIndex + 1);
			nIndex = str.Find("\"");
			str = str.Left(nIndex);
			//strcpy(formname,(LPCSTR)str);
			m_strFormName = str;
		}
		bRet = ar.ReadString(str);  // BEGIN
		if(!bRet) AfxThrowFileException(CFileException::generic);
		str.TrimLeft();
		str.TrimRight();
		if(str != "BEGIN") AfxThrowFileException(CFileException::generic);
		
		CString strTemp;
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
				if(strAttribute == "UNIT")
				{
					//n = str.Find(" ");
					//sscanf(str.Mid(n+1),"%d,%d",&unit.m_wX ,&unit.m_wY);
					
					//str = str.Left(n);			
					//unit.m_strBase = str;

					n = str.Find(" ");

					strTemp = str.Left(n);
					unit.m_strBase = strTemp;
					strTemp = str.Mid(n+1);

					n = strTemp.Find(",");
					unit.m_wX = atoi(strTemp.Left(n));
					unit.m_wY = atoi(strTemp.Mid(n+1));
					
				}
				else if(strAttribute == "SIZE")
				{
					//sscanf(str,"%d,%d",	&size.m_dwWidth ,&size.m_dwHeight);
					n = str.Find(",");
					size.m_wWidth = atoi(str.Left(n));
					str = str.Mid(n+1);
					size.m_wHeight = atoi(str);
				}
				// Do 
				else if(strAttribute == "ALIGNMENT")
				{
					nIndex = str.Find(",");
					
					alignment.m_strAlignment = str.Left(nIndex);
					str = str.Mid(nIndex + 1);

					nIndex = str.Find(",");
					alignment.m_wXoffset = atoi(str.Left(nIndex));
					alignment.m_wYoffset = atoi(str.Mid(nIndex + 1));
					//sscanf(str,"%d,%d",&alignment.m_wXoffset,&alignment.m_wYoffset);
				}
				else if(strAttribute == "ORIENTATION")
				{
					m_strOrientation = str;
					//sscanf(str,"%s",m_strOrientation);
				}
				else if(strAttribute == "SKEW")
				{
					//sscanf(str,"%d",&m_dwSkew);
					m_dwSkew = atoi(str);
				}
				else if(strAttribute == "VERSION")
				{			
					//sscanf(str,"%d,%d",&version.m_wMajor,&version.m_wMinor);
					strTemp = str;
					nIndex = strTemp.Find(",");
					strTemp = strTemp.Left(nIndex);					
					version.m_wMajor = atoi(strTemp);

					strTemp = str.Mid(nIndex + 1);
					nIndex = strTemp.Find(",");
					version.m_wMinor = atoi(strTemp.Left(nIndex));

					nIndex = str.Find("\"");
					str = str.Mid(nIndex +1);
					nIndex = str.Find("\"");
					version.m_strDate = str.Left(nIndex);

					nIndex = str.Find("\"");
					str = str.Mid(nIndex + 1);
					nIndex = str.Find("\"");
					str =str.Mid(nIndex + 1);
					nIndex = str.Find("\"");
					version.m_strAuthor = str.Left(nIndex);					
					
				}
				else if(strAttribute == "LANGUAGE")
				{
					sscanf(str,"%x",&m_dwLanguage);									
					
				}
				else if(strAttribute == "COPYRIGHT")
				{
					m_strCopyRight = str;
					m_strCopyRight = m_strCopyRight.Right(m_strCopyRight.GetLength()-1);
					m_strCopyRight = m_strCopyRight.Left(m_strCopyRight.GetLength()-1);
				}
				else if(strAttribute == "TITLE")
				{
					m_strTitle = str;
					m_strTitle = m_strTitle.Right(m_strTitle.GetLength()-1);
					m_strTitle = m_strTitle.Left(m_strTitle.GetLength()-1);
				}
				else if(strAttribute == "COMMENT")
				{
					m_strComment = str;
					m_strComment = m_strComment.Right(m_strComment.GetLength()-1);
					m_strComment = m_strComment.Left(m_strComment.GetLength()-1);
				}
				else if(strAttribute == "USERPROMPT")
				{
					m_strUserPrompt = str;
					m_strUserPrompt = m_strUserPrompt.Right(m_strUserPrompt.GetLength()-1);
					m_strUserPrompt = m_strUserPrompt.Left(m_strUserPrompt.GetLength()-1);
				}
				
				
				else if(strAttribute == "XFSSUBFORM")
				{
					CXFSSubform *pXFSSubform = new CXFSSubform;
					if(pXFSSubform->m_nNoHead == FALSE)
					{
						pXFSSubform->m_nNoHead = TRUE;
					}
					else
					{						
					}
					
					nIndex = str.Find("\"");
					str = str.Mid(nIndex + 1);
					nIndex = str.Find("\"");
					str = str.Left(nIndex);
					//strcpy(pXFSSubform->subformname,(LPCSTR)str);
					pXFSSubform->m_strSubFormName = str;
					pXFSSubform->m_pRootForm = this;
					pXFSSubform->Serialize(ar);

					m_listSubforms.AddTail(pXFSSubform);//insert 
				}
				else if(strAttribute == "XFSFRAME")
				{
					CXFSFrame *pXFSFrame = new CXFSFrame;
					if(pXFSFrame ->m_nNoHead == FALSE)
					{
						pXFSFrame->m_nNoHead = TRUE;
					}
					nIndex = str.Find("\"");
					str = str.Mid(nIndex + 1);
					nIndex = str.Find("\"");
					str = str.Left(nIndex);
					//strcpy(pXFSFrame->framename,(LPCSTR)str);
					pXFSFrame->m_strFrameName = str;
					pXFSFrame->m_pRootForm = this;
					pXFSFrame->Serialize(ar);

					m_listFrames.AddTail(pXFSFrame);
				}
				else if(strAttribute == "XFSFIELD")
				{
					CXFSField *pXFSField = new CXFSField;
					if(pXFSField->m_nNoHead == FALSE)
					{
						pXFSField->m_nNoHead = TRUE;
					}
					
					nIndex = str.Find("\"");
					str = str.Mid(nIndex + 1);
					nIndex = str.Find("\"");
					str = str.Left(nIndex);
					//strcpy(pXFSField->fieldname,(LPCSTR)str);
					pXFSField->m_strFieldName = str;
					pXFSField->m_pRootForm = this;
					pXFSField->Serialize(ar);

					m_listFields.AddTail(pXFSField);
				}
			}
		}
		//bRet = ar.ReadString(str);  // END
	}
}

BOOL CXFSForm::ExtractFieldValue(LPCSTR psz)
{
	BOOL bRet = TRUE;
	POSITION pos = m_listFields.GetHeadPosition();
	while(pos)
	{
		CXFSField *pXFSField = (CXFSField *)m_listFields.GetNext(pos);
		bRet = bRet && pXFSField->ExtractFieldValue(psz);
	}

	pos = m_listSubforms.GetHeadPosition();
	while(pos)
	{
		CXFSSubform *pXFSSubform = (CXFSSubform *)m_listSubforms.GetNext(pos);
		bRet = bRet && pXFSSubform->ExtractFieldValue(psz);
	}

	
	return bRet;
}

BOOL CXFSForm::Draw(CDC *pDC)
{
	BOOL bRet = TRUE;

	//int nDPIX = pDC->GetDeviceCaps(LOGPIXELSX);
	//int nDPIY = pDC->GetDeviceCaps(LOGPIXELSY);	

	

	//SIZE
	CSize sizeOut;	
	XFSGetSize(pDC,unit.m_strBase,CSize(size.m_wWidth,size.m_wHeight),CSize(unit.m_wX,unit.m_wY),sizeOut,
		m_lXExt,m_lYExt);

	m_nSizeX = sizeOut.cx; // drawing area in pixels 
	m_nSizeY = sizeOut.cy; // drawing area in pixels 

	
	///orientation 
	if(m_strOrientation == "PORTRAIT")
	{}
    else if(1){}///????·½Ïò

	//skew  no implement in this version	


	POSITION pos = m_listFields.GetHeadPosition();
	while(pos)
	{
		CXFSField *pXFSField = (CXFSField *)m_listFields.GetNext(pos);
		bRet = bRet && pXFSField->Draw(pDC);
	}

	pos = m_listFrames.GetHeadPosition();
	while(pos)
	{
		CXFSFrame *pXFSFrame = (CXFSFrame *)m_listFrames.GetNext(pos);
		bRet = bRet && pXFSFrame->Draw(pDC);
	}

	pos = m_listSubforms.GetHeadPosition();
	while(pos)
	{
		CXFSSubform *pXFSSubform = (CXFSSubform *)m_listSubforms.GetNext(pos);
		bRet = bRet && pXFSSubform->Draw(pDC);
	}

	return bRet;
}

CXFSField * CXFSForm::GetFieldByName(CString strFieldName)
{
	CXFSField *pField = 0;
	POSITION pos = m_listFields.GetHeadPosition();
	while(pos)
	{
		pField = (CXFSField *)m_listFields.GetNext(pos);
		if(pField->m_strFieldName == strFieldName) 
			return pField;
	}

	pos = m_listSubforms.GetHeadPosition();
	while(pos)
	{
		CXFSSubform *pSubform = (CXFSSubform *)m_listSubforms.GetNext(pos);
		pField = pSubform->GetFieldByName(strFieldName);
		if(pField) return pField;
	}


	return pField;
}

HRESULT CXFSForm::CheckAgainstMedia(CXFSMedia *pXFSMedia)
{
	HRESULT hr = 0;

	// To Do


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

	pos = m_listSubforms.GetHeadPosition();
	while(pos)
	{
		CXFSSubform *pSubform = (CXFSSubform *)m_listSubforms.GetNext(pos);
		hr |= pSubform->CheckAgainstMedia(pXFSMedia);
	}
	return hr;
}
