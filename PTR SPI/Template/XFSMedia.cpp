// XFSMedia.cpp: implementation of the CXFSMedia class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "$$root$$.h"
#include "XFSMedia.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXFSMedia::CXFSMedia()
{
	m_nNoHead = FALSE;

	unit.m_strBase = "";
	unit.m_wX = 0;
	unit.m_wY = 0;
	size.m_wWidth = 0;
	size.m_wHeight = 0;

	printArea.m_wX = 0;
	printArea.m_wY = 0;
	printArea.m_wWidth = 0;
	printArea.m_wHeight = 0;

	restricted.m_wX = 0;
	restricted.m_wY = 0;
	restricted.m_wWidth = 0;
	restricted.m_wHeight = 0;

	m_strMediaName = "";
	m_strFOLD = "";
	m_strType = "";// GENERIC (default) MULTIPART PASSBOOK 
	m_strSource = "";

	m_dwSTAGGERING = 0;
	m_dwPAGE = 0;
	m_dwLINES = 0;

}

CXFSMedia::~CXFSMedia()
{

}

void CXFSMedia::Serialize(CArchive &ar)
{
	CString str = "";
	if(ar.IsStoring())
	{
		ar.Write("XFSMEDIA", strlen("XFSMEDIA"));
		ar.Write(" \"", 2);
		ar.Write(m_strMediaName,m_strMediaName.GetLength());
		ar.Write("\"\r\n", 3);
		ar.Write(" BEGIN\r\n", strlen(" BEGIN\r\n"));

		if(m_strType != "")
		{
			str.Format("  TYPE %s",m_strType);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(m_strSource != "")
		{
			str.Format("  SOURCE %s",m_strSource);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

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

		// PrintArea
		if(printArea.m_wWidth != 0 && printArea.m_wHeight!=0) 
		{
			str.Format("  PRINTAREA %d, %d",printArea.m_wX, printArea.m_wY, printArea.m_wWidth,printArea.m_wHeight);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		// restricted
		if(restricted.m_wWidth != 0 && restricted.m_wHeight!=0) 
		{
			str.Format("  RESTRICTED %d, %d",restricted.m_wX, restricted.m_wY, 
				restricted.m_wWidth,restricted.m_wHeight);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(m_strFOLD != "")
		{
			str.Format("  FOLD %s",m_strFOLD);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(m_dwSTAGGERING >= 0)
		{
			str.Format("  STAGGERING %d",m_dwSTAGGERING);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(m_dwPAGE >= 0)
		{
			str.Format("  PAGE %d",m_dwPAGE);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}

		if(m_dwLINES >= 0)
		{
			str.Format("  LINES %d",m_dwLINES);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}


		ar.Write(" END\r\n", strlen(" END\r\n"));
	}
	else
	{
		BOOL bRet = FALSE;
		long nIndex = 0;
		if(!m_nNoHead)
		{
			bRet = ar.ReadString(str);
			nIndex = str.Find("XFSMEDIA");
			while(1)
			{
				if(nIndex == -1)/////
				{
					bRet = ar.ReadString(str);
					if(!bRet)	return;//
					nIndex = str.Find("XFSMEDIA");
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
			m_strMediaName = str;
		}

		bRet = ar.ReadString(str);  // BEGIN
		if(!bRet) AfxThrowFileException(CFileException::generic);
		str.TrimLeft();
		str.TrimRight();
		if(str != "BEGIN") AfxThrowFileException(CFileException::generic);

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
				if(strAttribute == "TYPE")
				{
					m_strType = str;
				}
				else if(strAttribute == "SOURCE")
				{
					m_strSource = str;
				}
				else if(strAttribute == "UNIT")
				{
					n = str.Find(" ");
					sscanf(str.Mid(n+1),"%d,%d",&unit.m_wX ,&unit.m_wY);
					
					str = str.Left(n);			
					unit.m_strBase = str;
				}
				else if(strAttribute == "SIZE")
				{
					sscanf(str,"%d,%d",	&size.m_wWidth ,&size.m_wHeight);
				}
				else if(strAttribute == "PRINTAREA")
				{
					sscanf(str,"%d,%d,%d,%d",&printArea.m_wX ,&printArea.m_wY,
						&printArea.m_wWidth ,&printArea.m_wHeight);
				}
				else if(strAttribute == "RESTRICTED")
				{
					sscanf(str,"%d,%d,%d,%d",&restricted.m_wX ,&restricted.m_wY,
						&restricted.m_wWidth ,&restricted.m_wHeight);
				}
				else if(strAttribute == "FOLD")
				{
					m_strFOLD = str;
				}
				else if(strAttribute == "STAGGERING")
				{
					m_dwSTAGGERING = atol(str);
				}
				else if(strAttribute == "PAGE")
				{
					m_dwPAGE = atol(str);
				}
				else if(strAttribute == "LINES")
				{
					m_dwLINES = atol(str);
				}
			}
		}
	
	}
}
