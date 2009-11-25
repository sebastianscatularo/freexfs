#include "stdafx.h"
#include "xfstoolsfunction.h"

BOOL XFSIsNeedToDrawThisPage(CString strSource,int nPageNo)
{
	///header	
	if(strSource == "")
		return TRUE;

	BOOL bRet = FALSE;
	
	int nIndex;
	if(strSource == "ALL")//ALL print all header
	{
		return TRUE;
	}
	CString str;
	str = strSource;
	do{
		nIndex = str.Find(",");
		if(nIndex == -1)
		{
			CString strTemp;
			strTemp = str;
			int nTemp;
			nTemp = strTemp.Find("-");
			if(nTemp == -1)
			{
				nTemp = atoi(strTemp);
				if(nTemp == nPageNo)
					return TRUE;
				return bRet = FALSE;
			}
			else
			{
				strTemp = str.Left(nTemp);
				int nFrom,nTo;
				nFrom = atoi(strTemp);
				strTemp = str.Mid(nTemp+1);
				nTo = atoi(strTemp);
				if(nPageNo <= nTo && nPageNo >= nFrom)
				{
					bRet = TRUE;
					return TRUE;
				}

			}
		}
		else 
		{
			CString strTemp;
			strTemp = str.Left(nIndex);			
			int nTemp;
			nTemp = strTemp.Find("-");
			if(nTemp == -1)
				{
				nTemp = atoi(strTemp);
				if(nTemp == nPageNo)
					return TRUE;
			}
			else
			{
				strTemp = str.Left(nTemp);
				int nFrom,nTo;
				nFrom = atoi(strTemp);
				strTemp = str.Mid(nTemp+1);
				nTo = atoi(strTemp);
				if(nPageNo < nTo && nPageNo > nFrom)
				{
					bRet = TRUE;
					return TRUE;
				}

			}
		}
		nIndex = str.Find(",");
		if(nIndex == -1)
			break;
		str = str.Mid(nIndex + 1);

	}while(str.GetLength());
	
	
//header 
	return bRet;
}


int XFSGetSize(CDC *pDC,CString strBase,CSize sizeIn,CSize unit,CSize& sizeOut,float xExt,float yExt)
{
	int nSizeX,nSizeY;
	int nDPIX = pDC->GetDeviceCaps(LOGPIXELSX);
	int nDPIY = pDC->GetDeviceCaps(LOGPIXELSY);

	if(strBase == "MM")
	{
		nSizeX = (int)(sizeIn.cx * ((nDPIX/25.4) / unit.cx));
		nSizeY = (int)((double)sizeIn.cy * ((double)(nDPIY/25.4) / unit.cy));
	}
	else if(strBase == "INCH")
	{
		double xx;
		xx = sizeIn.cx * nDPIX / unit.cx;
		nSizeX = (int)((double)sizeIn.cx * ((double)nDPIX / unit.cx));
		nSizeY = (int)((double)sizeIn.cy * ((double)nDPIY / unit.cy));
	}
	else if(strBase == "ROWCOLUMN")
	{
		nSizeX = (int)((double)sizeIn.cx  / unit.cx);
		nSizeY = (int)((double)sizeIn.cy  / unit.cy);
	}
	sizeOut.cx = nSizeX;
	sizeOut.cy = nSizeY;

	sizeOut.cx = (long)((float)sizeOut.cx/xExt);
	sizeOut.cy = (long)((float)sizeOut.cy/yExt);

	return 1;
}

void XFSOffsetForm(const CString strAlignment,const CSize sizePaper,const CSize sizeOffset,CSize &sizeOut)
{
	sizeOut = sizeOffset;

	if(strAlignment == "TOPLEFT")
	{
		sizeOut = sizeOffset;
	}
	else if(strAlignment == "TOPRIGHT")
	{
		sizeOut.cx = sizePaper.cx - sizeOffset.cx;
	}
	else if(strAlignment == "BOTTOMLEFT")
	{
		sizeOut.cy = sizePaper.cy - sizeOffset.cy;
	}
	else if(strAlignment == "BOTTOMRIGHT")
	{
		sizeOut.cx = sizePaper.cx - sizeOffset.cx;
		sizeOut.cy = sizePaper.cy - sizeOffset.cy;
	}
}


void XFSSetFontStyle(CDC* pDC,CString strStyle,CFont &font)
{
	LOGFONT lf;
	CFont *fontTemp;
	fontTemp = pDC->GetCurrentFont();

	fontTemp->GetLogFont(&lf);
	int nIndex;
	DWORD dwStyle = 0;
	
	CString strTemp;
	strTemp = strStyle;
	CString str2;
	str2 = strStyle;

	nIndex = strTemp.Find("|");
	while(strTemp.GetLength())
	{
		if(nIndex != -1)
			str2 = strTemp.Left(nIndex);
		else 
		{
			str2 =strTemp;
			strTemp = "";
		}

		if(str2 == "NORMAL")
		{
			lf.lfWeight |= FW_NORMAL;
		}
		else if(str2 == "BOLD")
		{
			lf.lfWeight |= FW_BOLD;
		}
		else if(str2 == "ITALIC")
		{
			lf.lfItalic = TRUE;
		}
		else if(str2 == "UNDER")
		{
			lf.lfUnderline = TRUE;
		}
		else if(str2 == "DOUBLEUNDER"){}
		else if(str2 == "DOUBLE")
		{
			lf.lfWidth *= 2;
		}
		else if(str2 == "TRIPLE")
		{
			lf.lfWidth *=3;
		}
		else if(str2 == "QUADRUPLE")
		{
			lf.lfWidth *= 4;
		}
		else if(str2 == "STRIKETHROUGH")
		{
			lf.lfStrikeOut = TRUE;
		}
		else if(str2 == "ROTATE90")
		{
			lf.lfEscapement = 900;
			lf.lfOrientation = 900;
		}
		else if(str2 == "ROTATE270")
		{
			lf.lfEscapement = 2700;
			lf.lfOrientation = 2700;
		}
		else if(str2 == "UPSIDEDOWN"){}
		else if(str2 == "PROPORTIONAL"){}
		else if(str2 == "DOUBLEHIGH"){}
		else if(str2 == "TRIPLEHIGH"){}
		else if(str2 == "QUADRUPLEHIGH"){}
		else if(str2 == "CONDENSED"){}
		else if(str2 == "SUPERSCRIPT"){}
		else if(str2 == "SUBSCRIPT"){}
		else if(str2 == "OVERSCORE"){}
		else if(str2 == "LEFTERQUALITY"){}
		else if(str2 == "NEARLEFTERQUALITY"){}
		else if(str2 == "DOUBLESTRIKE"){}
		else if(str2 == "OPAQUE"){}


		strTemp = strTemp.Mid(nIndex + 1);
		nIndex = strStyle.Find("|");

	}
	
	font.CreateFontIndirect(&lf);
}


void XFSHAlignment(CString strBuffer,UINT &uFormat)
{
	uFormat = 0;

	if(strBuffer == "LEFT")
	{
		uFormat= uFormat | DT_LEFT;
	}
	else if(strBuffer == "RIGHT")
	{
		uFormat= uFormat | DT_RIGHT;
	}
	else if(strBuffer == "CENTER")
	{
		uFormat= uFormat | DT_CENTER;
	}
	else
	{
		uFormat= uFormat | DT_LEFT;
	}
}

void XFSVAlignment(CString strBuffer,UINT &uFormat)
{
	if(strBuffer == "BOTTOM")
	{
		uFormat= uFormat | DT_BOTTOM;
	}
	else if(strBuffer == "TOP")
	{
		uFormat= uFormat | DT_TOP ;
	}
	else if(strBuffer == "CENTER")
	{
		uFormat= uFormat | DT_VCENTER;
	}
	else
	{
		uFormat= uFormat | DT_BOTTOM;
	}
}

void XFSSetColor(CString strColor,COLORREF &rColor)
{
	if(strColor == "BLACK")
	{
		rColor = RGB(0,0,0);		
	}
	else if(strColor == "WHITE")
	{
		rColor = RGB(255,255,255);
	}
	else if(strColor == "GRAY")
	{
		rColor = RGB(127,127,127);
	}
	else if(strColor == "RED")
	{
		rColor = RGB(255,0,0);
	}
	else if(strColor == "BLUE")
	{
		rColor = RGB(0,0,255);
	}
	else if(strColor == "GREEN")
	{
		rColor = RGB(0,255,0);
	}
	else if(strColor == "YELLOW")
	{
		rColor = RGB(255,255,0);
	}
	else
	{
		rColor = RGB(0,0,0);
	}
}


//frame
///pen
void XFSSetPenStyle(CDC* pDC,CString strStyle,CPen &pen)
{
	CPen *pPen;
	LOGPEN lp;
	pPen = pDC->GetCurrentPen();
	pPen->GetLogPen(&lp);

}
