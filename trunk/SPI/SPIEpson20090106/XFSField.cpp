// XFSField.cpp: implementation of the CXFSField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "epson2090spi.h"
#include "XFSField.h"
#include "XFSForm.h"
#include "XFSSubform.h"
#include "myfile\\printerbase.h"
#include "XFSMedia.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXFSField::CXFSField() : m_arrayFieldData()
{
	m_nNoHead = FALSE;
	m_dwPage = 0;
//	pNext = 0;
	

	///xfsfield initialize
	position.m_wX = 0;
	position.m_wY = 0;	
	m_strFollows = "";
	m_strHeader = "";
	m_strFooter = "";
	m_strSide = "FRONT";
	size.m_wWidth = 0;
	size.m_wHeight = 0;
	index.m_wRepeatcount = 0;
	index.m_wXoffset = 0;
	index.m_wYoffset = 0;
	m_strType = "TEXT"; 
	m_strScaling = "BESTFIT"; 
	m_strBarcode = "NONE"; 
	m_strClass = "OPTIONAL"; 
	m_strAccess = "WRITE"; 
	m_strOverflow = "TERMINATE"; 
	m_strStyle = "";
	m_strCase = "";
	m_strHorizontal = "";
	m_strVertical  = "";
	m_strColor = "";
	rgbcolor.m_bRed =0;
	rgbcolor.m_bGreen = 0;
	rgbcolor.m_bBlue = 0;
	m_dwLanguage = 0x0409;
	m_strFont = "";
	m_dwPointSize = 0;
	m_dwCpi = 0;//characters per inch
	m_dwLpi = 0;//lines per inch
	m_strFormat = "";
	m_strInitialValue = "";
	
	m_strFieldValue = "";
	m_pRootForm = 0;
	m_pParentSubform = 0;

	m_nPositionX = 0;
	m_nPositionY = 0;
	m_nWidth = 0;
	m_nHeight = 0;

	m_nIndex = 0;
}

CXFSField::~CXFSField()
{

}

void CXFSField::Serialize(CArchive &ar)
{
	CString str;
	if(ar.IsStoring())
	{
		ar.Write("XFSFIELD", strlen("XFSFIELD"));
		ar.Write(" \"", 2);
		ar.Write(m_strFieldName,m_strFieldName.GetLength());
		ar.Write("\"\r\n", 3);
		// BEGIN
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
		// folows
		if(m_strFollows != "") 
		{
			str.Format("  FOLLOWS \"%s\"",m_strFollows);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		// header
		if(m_strHeader != "") 
		{
			str.Format("  HEADER %s",m_strHeader);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		// footer
		if(m_strFooter != "") 
		{
			str.Format("  FOOTER %s",m_strFooter);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		// side
		if(m_strSide != "") 
		{
			str.Format("  SIDE %s",m_strSide);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		// size
		if(size.m_wWidth != 0 && size.m_wHeight!=0) 
		{
			str.Format("  SIZE %d, %d",size.m_wWidth,size.m_wHeight);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n",2);
		}
		// INDEX
		if(index.m_wRepeatcount != 0 ) 
		{
			str.Format("  REPEATONX %d, %d, %d",index.m_wRepeatcount,index.m_wXoffset,index.m_wYoffset);		
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//type
		if(m_strType != "") 
		{
			str.Format("  TYPE %s",m_strType);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//scaling
		if(m_strScaling != "") 
		{
			str.Format("  SCALING %s",m_strScaling);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//barcode
		if(m_strBarcode != "") 
		{
			str.Format("  BARCODE %s",m_strBarcode);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//class
		if(m_strClass != "")
		{
			str.Format("  CLASS %s",m_strClass);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//access
		if(m_strAccess != "") 
		{
			str.Format("  ACCESS %s",m_strAccess);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//overflow
		if(m_strOverflow != "") 
		{
			str.Format("  OVERFLOW %s",m_strOverflow);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//style
		if(m_strStyle != "") 
		{
			str.Format("  STYLE %s",m_strStyle);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//case
		if(m_strCase != "") 
		{
			str.Format("  CASE %s",m_strCase);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//horizontal
		if(m_strHorizontal != "") 
		{
			str.Format("  HORIZONTAL %s",m_strHorizontal);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//vertical
		if(m_strVertical != "") 
		{
			str.Format("  VERTICAL %s",m_strVertical);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//color
		if(m_strColor != "") 
		{
			str.Format("  COLOR %s",m_strColor);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//rgbcolor
		if(m_strOverflow != "") 
		{
			str.Format("  RGBCOLOR %d,%d,%d",rgbcolor.m_bRed,rgbcolor.m_bGreen,rgbcolor.m_bBlue);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//language
		if(m_dwLanguage != 0) 
		{
			str.Format("  LANGUAEG %d",m_dwLanguage);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//font
		if(m_strFont != "") 
		{
			str.Format("  FONT \"%s\"",m_strFont);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		if(m_strFont != "") 
		{
			str.Format("  FONT %s",m_strFont);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//cpi
		if(m_dwCpi != 0) 
		{
			str.Format("  CPI %d",m_dwCpi);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//lpi
		if(m_dwLpi != 0) 
		{
			str.Format("  CPI %d",m_dwLpi);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//FORMAT
		if(m_strFormat != "") 
		{
			str.Format("  FORMAT \"%s\"",m_strFormat);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
		}
		//initialvalue
		if(m_strInitialValue != "") 
		{
			str.Format("  INITIALVALUE \"%s\"",m_strInitialValue);
			ar.Write(str,str.GetLength());
			ar.Write("\r\n", 2);
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
			nIndex = str.Find("XFSFIELD");
			str = str.Mid(nIndex + strlen("XFSFIELD"));
			nIndex = str.Find("\"");
			str = str.Mid(nIndex + 1);
			nIndex = str.Find("\"");
			str = str.Left(nIndex);
			//strcpy(fieldname,(LPCSTR)str);
			m_strFieldName = str;
		}
		bRet = ar.ReadString(str);  // BEGIN
		if(!bRet) AfxThrowFileException(CFileException::generic);

		long lTemp;

		while(1)
		{
			bRet = ar.ReadString(str);
			if(!bRet) AfxThrowFileException(CFileException::generic);
			str.TrimLeft();
			str.TrimRight();
			if(str == "END") break;
			lTemp = str.Find(" ");
			if(lTemp > 0)
			{
				CString strAttribute = str.Left(lTemp);
				str = str.Mid(lTemp + 1);
				str.TrimLeft();


				if(strAttribute == "POSITION")
				{
					lTemp = str.Find("(");
					if(lTemp < 0)
					{
						m_dwPage = 0;
						sscanf(str,"%d,%d",&position.m_wX,&position.m_wY);
					}
					else
					{
						lTemp = str.Find("POSITION");
						str = str.Mid(lTemp + strlen("POSITION"));
						sscanf(str,"%d,(%d,%d)",&position.m_wX,&position.m_wY,&m_dwPage);
					}
				}
				else if(strAttribute == "FOLLOWS")
				{
					lTemp = str.Find("\"");
					str = str.Mid(lTemp + 1);
					lTemp = str.Find("\"");
					str = str.Left(lTemp);
					m_strFollows = str;
				}
				else if(strAttribute == "HEADER")
				{
					lTemp = str.Find(" ");
					str = str.Mid(lTemp+1);					

					m_strHeader = str;
				}
				else if(strAttribute == "FOOTER")
				{
					lTemp = str.Find(" ");
					str = str.Mid(lTemp+1);
					

					m_strFooter = str;
				}
				else if(strAttribute == "SIDE")
				{
					m_strSide = str;
				}
				else if(strAttribute == "SIZE")
				{
					sscanf(str,"%d,%d",	&size.m_wWidth ,&size.m_wHeight);
				}
				else if(strAttribute == "INDEX")
				{
					sscanf(str,"%d,%d,%d",	&index.m_wRepeatcount ,&index.m_wXoffset,&index.m_wYoffset);
				}
				else if(strAttribute == "TYPE")
				{
					m_strType = str;
				}
				else if(strAttribute == "SCALING")
				{
					m_strScaling = str;
				}
				else if(strAttribute == "BARCODE")
				{
					m_strBarcode = str;
				}
				else if(strAttribute == "CLASS")
				{
					m_strClass = str;
				}
				else if(strAttribute == "ACCESS")
				{
					m_strAccess = str;
				}
				else if(strAttribute == "OVERFLOW")
				{
					m_strOverflow = str;
				}

				else if(strAttribute == "STYLE")
				{
					m_strStyle = str;
				}

				else if(strAttribute == "CASE")
				{
					m_strCase = str;
				}

				else if(strAttribute == "HORIZONTAL")
				{
					m_strHorizontal = str;
				}

				else if(strAttribute == "VERTICAL")
				{
					m_strVertical = str;
				}

				else if(strAttribute == "COLOR")
				{
					m_strColor = str;
				}

				else if(strAttribute == "RGBCOLOR")
				{
					sscanf(str,"%d,%d,%d",	&rgbcolor.m_bRed ,&rgbcolor.m_bGreen,&rgbcolor.m_bBlue);
				}

				else if(strAttribute == "LANGUAGE")
				{
					sscanf(str,"%x",&m_dwLanguage);
				}

				else if(strAttribute == "FONT")
				{
					lTemp = str.Find("\"");
					str = str.Mid(lTemp + 1);
					lTemp = str.Find("\"");
					str = str.Left(lTemp);
					m_strFont = str;
				}

				else if(strAttribute == "POINTSIZE")
				{
					sscanf(str,"%d",&m_dwPointSize);
				}

				else if(strAttribute == "CPI")
				{
					sscanf(str,"%d",&m_dwCpi);
				}

				else if(strAttribute == "LPI")
				{
					sscanf(str,"%d",&m_dwLpi);
				}

				else if(strAttribute == "FORMAT")
				{
					lTemp = str.Find("\"");
					str = str.Mid(lTemp + 1);
					lTemp = str.Find("\"");
					str = str.Left(lTemp);

					m_strFormat = str;
				}

				else if(strAttribute == "INITIALVALUE")
				{
					lTemp = str.Find("\"");
					str = str.Mid(lTemp + 1);
					lTemp = str.Find("\"");
					str = str.Left(lTemp);

					m_strInitialValue = str;
				}
			}
		}
	}

}

/*
BOOL CXFSField::ExtractFieldValue(LPCSTR psz)
{
	char *pFound = ::strstr(psz,"<" + m_strFieldName + ">");
	if(pFound == NULL)
	{
		return FALSE;
	}

	char *pIndexValue = ::strstr(psz,"[<");
	if(pIndexValue != 0)
	{
		pIndexValue += 2;
		char *pIndexValue1 = ::strstr(pIndexValue,">]");
		CString strIndex(pIndexValue, pIndexValue1-pIndexValue);
		m_nIndex = atol(strIndex);
	}

	pFound += strlen("<" + m_strFieldName + ">");

	char *nValue = ::strstr(pFound,"=<");
	nValue += 2;
	char *nValue1 = ::strstr(nValue,">");
	CString strValue(nValue, nValue1-nValue);
	m_strFieldValue = strValue;
	return TRUE;
	
}
*/


BOOL CXFSField::ExtractFieldValue(LPCTSTR psz)
{
	LPTSTR pAnchor = (LPTSTR)psz;
	LPTSTR pFound = 0;
	while(1)
	{
		//pFound = ::_tcsstr(pAnchor,m_strFieldName + _T("="));
		pFound = ::_tcsstr(pAnchor,m_strFieldName + _T("="));
		if(pFound == NULL)
		{
repeat:
			pFound = ::_tcsstr(pAnchor,m_strFieldName + _T("["));
			if(pFound == NULL)
			{
				pAnchor += _tcslen(pAnchor) + sizeof(TCHAR);
				if(*pAnchor != 0) continue;
				m_strFieldValue = _T("");
				return TRUE;
			}
			else
			{
				pAnchor += _tcslen(m_strFieldName + _T("["));
				//pAnchor  = pFound + sizeof(TCHAR);
				char *pFound1 = ::_tcsstr(pAnchor,_T("]"));
				CString strIndex(pAnchor, pFound1-pAnchor);
				//m_nIndex = _ttol(strIndex);
				long m_nCurrentIndex = _ttol(strIndex);
				LPTSTR pFound2 = ::_tcsstr(pFound1,_T("="));
				pFound2 += sizeof(TCHAR);
				CString strValue(pFound2);
				//m_strFieldValue = strValue;
				{
					BOOL bFound = FALSE;
					long n = m_arrayFieldData.GetSize();
					for(long i=0; i<n; i++)
					{
						if(m_arrayFieldData.GetAt(i) == strValue) 
						{
							bFound = TRUE;
							break;
						}
					}
					if(bFound == TRUE) goto repeat;
				}
				m_arrayFieldData.SetAtGrow(m_nCurrentIndex,strValue);
				pAnchor += _tcslen(pAnchor) + sizeof(TCHAR);
				if(m_nCurrentIndex > m_nIndex) m_nIndex = m_nCurrentIndex;
				continue;
			}
		}
		else
		{
			pAnchor += _tcslen(m_strFieldName + _T("="));
			//pAnchor  = pFound + sizeof(TCHAR);
			
			CString strValue(pAnchor);
			m_strFieldValue = strValue;
			pAnchor += _tcslen(pAnchor) + sizeof(TCHAR);
			break;
		}
	}
	return TRUE;
	
}

BOOL CXFSField::Draw(CDC *pDC)
{
	BOOL bRet = TRUE;
	for(long n=0; n<=m_nIndex; n++)
	{
		DrawOne(pDC, n);
	}
	return bRet;
}

BOOL CXFSField::DrawOne(CDC *pDC, long nFieldIndex)
{
	BOOL bRet = TRUE;
	
	// Follows
	XFS_POSITION positionOld = position;
	/*
	if(m_strFollows == "")
	{
	}
	else
	{
		CXFSField *  pFollowField = m_pRootForm->GetFieldByName(m_strFollows);
		if(pFollowField)
		{
			position.m_wX = position.m_wX + pFollowField->size.m_wWidth;
			position.m_wY = position.m_wY + pFollowField->size.m_wHeight;
		}
	}
	*/

	//position
	CSize sizeOut;  // position

	if(nFieldIndex == 0)
	{
		XFSGetSize(pDC,m_pRootForm->unit.m_strBase,
		CSize(position.m_wX,position.m_wY),
		CSize(m_pRootForm->unit.m_wX,m_pRootForm->unit.m_wY),
		sizeOut,m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);
	}
	else if(nFieldIndex < index.m_wRepeatcount)
	{
		XFSGetSize(pDC,m_pRootForm->unit.m_strBase,
		CSize(position.m_wX + nFieldIndex * index.m_wXoffset , position.m_wY + nFieldIndex * index.m_wYoffset),
		CSize(m_pRootForm->unit.m_wX,m_pRootForm->unit.m_wY),
		sizeOut,m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);
	}
	else return FALSE;

	position = positionOld;

	//get the form aligment
	CPrinterBase pb;
	CSize sizePaper;
	pb.GetPaperWidthAndHeight(sizePaper);//  mm/10
	//sizePaper.cx = 2100;
	//sizePaper.cy = 2000;
	XFSGetSize(pDC,"MM",CSize(sizePaper.cx/10,sizePaper.cy/10),CSize(m_pRootForm->unit.m_wX,
		m_pRootForm->unit.m_wY),sizePaper,m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);

	CSize sizeForm;
	XFSGetSize(pDC,m_pRootForm->unit.m_strBase,CSize(m_pRootForm->alignment.m_wXoffset,
		m_pRootForm->alignment.m_wYoffset),CSize(m_pRootForm->unit.m_wX,m_pRootForm->unit.m_wY),sizeForm,
		m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);

	CSize sizeFormOffset(0,0);

	XFSOffsetForm(m_pRootForm->alignment.m_strAlignment,sizePaper,sizeForm,sizeFormOffset);//relative to the form offset to the media
    
	sizeOut += sizeFormOffset;//form offset add field offset

	//follows
	if(m_strFollows == "")
	{
		m_nPositionX = sizeOut.cx;
		m_nPositionY = sizeOut.cy;
	}
	else
	{
		return FALSE;  // not implementation
	}

	///header
	if(!XFSIsNeedToDrawThisPage(m_strHeader,m_pRootForm->m_dwPage))//print the current page?  yes or no
		return FALSE;
	//header 

	//footer
	if(!XFSIsNeedToDrawThisPage(m_strFooter,m_pRootForm->m_dwPage))//print the curent page? yes or no
		return FALSE;
    //footer


	//size
	
	XFSGetSize(pDC,m_pRootForm->unit.m_strBase,CSize(size.m_wWidth,size.m_wHeight),
		CSize(m_pRootForm->unit.m_wX,m_pRootForm->unit.m_wY),sizeOut,
		m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);

	
	m_nWidth = sizeOut.cx;
	m_nHeight = sizeOut.cy;

	CRect rcDraw(CPoint(m_nPositionX,m_nPositionY), CSize(m_nWidth,m_nHeight));

	//side
	if (m_strSide == "FRONT")///default
	{
	}
	else if(m_strSide == "BACK")
	{
		// To do
	}

	UINT nFormat = 0;


	CString strToBePrinted = m_strFieldValue;
	if(m_arrayFieldData.GetSize() > 0)
	{
		strToBePrinted = m_arrayFieldData.GetAt(nFieldIndex);
	}

	if(strToBePrinted == "") strToBePrinted = m_strInitialValue;
	if(m_strCase == "NOCHANGE")
	{
	}
	else if(m_strCase == "UPPER")
	{
		strToBePrinted.MakeUpper();
	}
	else if(m_strCase == "LOWER")
	{
		strToBePrinted.MakeLower();
	}

	//overflow  
	SIZE size;
	GetTextExtentPoint(pDC->GetSafeHdc(),strToBePrinted,strToBePrinted.GetLength(),&size);
	
	if(m_strOverflow == "TERMINATE")
	{
		if (rcDraw.Width() < size.cx || rcDraw.Height() < size.cy)
			return 0;
	}
	else if(m_strOverflow == "TRUNCATE")
	{
		char *p;
		int nTemp;
		nTemp = strToBePrinted.GetLength();
		p = strToBePrinted.GetBuffer(nTemp);
		while(0)
		{			
			GetTextExtentPoint(pDC->GetSafeHdc(),p,nTemp--,&size);
			if(size.cx < rcDraw.Width())
				break;
		}
		strToBePrinted.Left(nTemp);
	}
	else if(m_strOverflow == "BESTFIT")
	{
	}
	else if(m_strOverflow == "WORDWRAP")
	{
		nFormat = nFormat | DT_END_ELLIPSIS ;
	}

	//deal style 
	//STYLE
    CFont font;
	XFSSetFontStyle(pDC,m_strStyle,font);
	pDC->SelectObject(&font);

	//HORIZONTAL
	
	XFSHAlignment(m_strHorizontal,nFormat);

    //vertical
	
	XFSVAlignment(m_strVertical,nFormat);

	//RGB rColor;

	COLORREF r;	
	
	XFSSetColor(m_strColor,r);
	pDC->SetTextColor(r);


	if(rgbcolor.m_bRed == 0 && rgbcolor.m_bGreen== 0 && rgbcolor.m_bBlue== 0)
	{
	}
	else
	{
		pDC->SetTextColor(RGB(rgbcolor.m_bRed,rgbcolor.m_bGreen,rgbcolor.m_bBlue));
	}
	
    //it's important that the keyword cpi adn lpi

	
	pDC->DrawText(strToBePrinted,rcDraw,nFormat);///pDC->SetTextJustification(550,5);
    
	//pDC->TextOut(rcDraw.left,rcDraw.top,"textout");
	/*pDC->DrawText("test",rcDraw,nFormat);
    pDC->TextOut(0,100,"11111");
	pDC->TextOut(360,100,"2222");*/
	return bRet;
}

HRESULT CXFSField::CheckAgainstMedia(CXFSMedia *pXFSMedia)
{
	// To Do 

	return S_OK;
}
