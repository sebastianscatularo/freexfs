// Createor.cpp: implementation of the CCreateor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "$$root$$.h"
#include "Createor.h"

#include "XFSField.h"
#include "XFSForm.h"
#include "XFSFrame.h"
#include "XFSSubform.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCreateor::CCreateor()
{

}

CCreateor::~CCreateor()
{

}

void * CCreateor::CreateChild(CArchive &ar, long *nType)
{
		void *pOut = NULL;
		*nType = 0; 
		CString str;
		BOOL bRet = FALSE;
		long nIndex = 0;
		bRet = ar.ReadString(str);
		nIndex = str.Find("XFSFIELD");
		if(nIndex > 0)
		{
			*nType = 1;
			CXFSField *pXFSField = new CXFSField;

			str = str.Mid(nIndex + strlen("XFSFIELD"));
			nIndex = str.Find("\"");
			str = str.Mid(nIndex + 1);
			nIndex = str.Find("\"");
			str = str.Left(nIndex);
			//strcpy(pXFSField->fieldname,(LPCSTR)str);
			pXFSField->m_strFieldName = str;
			pXFSField->m_nNoHead = TRUE;
			pOut =  pXFSField;
		}
		else 
		{
			nIndex = str.Find("XFSFRAME");
			if(nIndex > 0)
			{
				*nType = 2;
				CXFSFrame *pXFSFrame = new CXFSFrame;

				str = str.Mid(nIndex + strlen("XFSFRAME"));
				nIndex = str.Find("\"");
				str = str.Mid(nIndex + 1);
				nIndex = str.Find("\"");
				str = str.Left(nIndex);
				//strcpy(pXFSFrame->framename,(LPCSTR)str);
				pXFSFrame ->m_strFrameName = str;
				pXFSFrame->m_nNoHead = TRUE;
				pOut =  pXFSFrame;
			}
			else
			{
				nIndex = str.Find("XFSSUBFORM");
				if(nIndex > 0)
				{
					*nType = 3;
					CXFSSubform *pXFSSubform = new CXFSSubform;

					str = str.Mid(nIndex + strlen("XFSSUBFORM"));
					nIndex = str.Find("\"");
					str = str.Mid(nIndex + 1);
					nIndex = str.Find("\"");
					str = str.Left(nIndex);
					//strcpy(pXFSSubform->subformname,(LPCSTR)str);
					pXFSSubform->m_strSubFormName = str;
					pXFSSubform->m_nNoHead = TRUE;
					pOut =  pXFSSubform;
				}
				else
				{
					nIndex = str.Find("XFSFORM");
					if(nIndex > 0)
					{
						*nType = 4;
						CXFSForm *pXFSForm = new CXFSForm;

						str = str.Mid(nIndex + strlen("XFSFORM"));
						nIndex = str.Find("\"");
						str = str.Mid(nIndex + 1);
						nIndex = str.Find("\"");
						str = str.Left(nIndex);
						//strcpy(pXFSForm->formname,(LPCSTR)str);
						pXFSForm->m_strFormName = str;
						pXFSForm->m_nNoHead = TRUE;
						pOut =  pXFSForm;
					}
				}
			}
		}
		return pOut;
}
