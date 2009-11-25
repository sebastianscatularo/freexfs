// XFSFrame.cpp: implementation of the CXFSFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "epson2090spi.h"
#include "XFSFrame.h"
#include "XFSForm.h"
#include "XFSSubform.h"
#include "XFSField.h"
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

class CXFSForm;
class CXFSSubform;

CXFSFrame::CXFSFrame():m_strFrames()
{
	m_nNoHead = FALSE;


	//xfs initailize 
	position.m_wX = 0;
	position.m_wY = 0;
	m_strFrames = "";
	m_strHeader = "";
	m_strFooter = "";
	m_strSide = "FRONT";
	size.m_wWidth = 0;
	size.m_wHeight = 0;
	repeatonx.m_wRepeatCount = 0;
	repeatonx.m_wXoffset = 0;
	repeatony.m_wRepeatCount = 0;
	repeatony.m_wYoffset = 0;
	m_strType = "RECTANGLE";
	m_strClass = "STATIC";
	m_strOverflow = "TERMINATE";
	m_strStyle = "SINGLE_THIN";
	m_strColor = "BLACK";
	rgbcolor.m_bRed = 0;
	rgbcolor.m_bGreen = 0;
	rgbcolor.m_bBlue = 0;
	m_strFillColor = "WHITE";
	rgbfillcolor.m_bRed = 0;
	rgbfillcolor.m_bGreen = 0;
	rgbfillcolor.m_bBlue = 0;
	m_strFillStyle = "";
	m_strSubStsign = "";
	m_strHorizontal = "LEFT";
	m_strVertical = "TOP";

	m_pRootForm = 0;
	m_pParentSubform = 0;

	m_nPositionX = 0;
	m_nPositionY = 0;

}

CXFSFrame::~CXFSFrame()
{

}

void CXFSFrame::Serialize(CArchive &ar)
{
	CString str = "";
	if(ar.IsStoring())
	{
		ar.Write("XFSFRAME", strlen("XFSFRAME"));
		ar.Write(" \"", 2);
		ar.Write(m_strFrameName,m_strFrameName.GetLength());
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

		if(m_strFrames != "")
		{
			str.Format("  FRAMES %s",m_strFrames);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strHeader != "")
		{
			str.Format("  HEADER %s",m_strHeader);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strFooter != "")
		{
			str.Format("  FOOTER %s",m_strFooter);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strSide != "")
		{
			str.Format("  SIDE %s",m_strSide);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		
		str.Format("  SIZE %d,%d",size.m_wWidth,size.m_wHeight);
		ar.Write(str,str.GetLength());
		ar.Write("\r\n", 2);

		if((repeatonx.m_wRepeatCount != 0) || (repeatonx.m_wXoffset != 0))
		{
			str.Format(" REPEATONX %d,%d",repeatonx.m_wRepeatCount,repeatonx.m_wXoffset);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}

		if((repeatony.m_wRepeatCount != 0) || (repeatony.m_wYoffset != 0))
		{
			str.Format(" REPEATONX %d,%d",repeatony.m_wRepeatCount,repeatony.m_wYoffset);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		
		if(m_strType != "")
		{
			str.Format("  TYPE %s",m_strType);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strClass != "")
		{
			str.Format("  CLASS %s",m_strClass);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strOverflow != "")
		{
			str.Format("  OVERFLOW %s",m_strOverflow);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strStyle != "")
		{
			str.Format("  STYLE %s",m_strStyle);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strColor != "")
		{
			str.Format("  COLOR %s",m_strColor);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(rgbfillcolor.m_bRed || rgbfillcolor.m_bGreen || rgbfillcolor.m_bBlue)
		{
			str.Format(" RGBFILLCOLOR %d,%d,%d",rgbfillcolor.m_bRed,rgbfillcolor.m_bGreen,rgbfillcolor.m_bBlue);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strFillStyle != "")
		{
			str.Format("  FILLSTYLE %s",m_strFillStyle);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strSubStsign != "")
		{
			str.Format("  SUBSTSIGN %s",m_strSubStsign);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strTitle != "")
		{
			str.Format("  TITLE \"%s\"",m_strTitle);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strFooter != "")
		{
			str.Format("  FOOTER %s",m_strFooter);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strHorizontal != "")
		{
			str.Format("  HORIZONTAL %s",m_strHorizontal);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
		}
		if(m_strVertical != "")
		{
			str.Format("  VERTICAL %s",m_strVertical);
			ar.Write(str,str.GetLength());
		    ar.Write("\r\n", 2);
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
			if(!bRet) AfxThrowFileException(CFileException::generic);
			nIndex = str.Find("XFSFRAME");
			str = str.Mid(nIndex + strlen("XFSFRAME"));
			nIndex = str.Find("\"");
			str = str.Mid(nIndex + 1);
			nIndex = str.Find("\"");
			str = str.Left(nIndex);
			//strcpy(framename,(LPCSTR)str);
			m_strFrameName = str;
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
				CString str1 = str.Mid(lTemp + 1);
				str1.TrimLeft();
				str = str1;
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
						long n = str.Find("POSITION");
						str = str.Mid(lTemp + strlen("POSITION"));
						sscanf(str,"%d,(%d,%d)",&position.m_wX,&position.m_wY,&m_dwPage);
					}
				}
				else if(strAttribute == "FRAMES")
				{
				
					lTemp = str.Find("\"");
					CString str1 = str.Mid(lTemp + 1);
					str = str1;
					lTemp = str.Find("\"");
					//m_strFrames = str.Left(lTemp);
					str1 = str.Left(lTemp);
					m_strFrames = str1;
				
				}
				else if(strAttribute == "HEADER")
				{					
					m_strHeader = str;
				}
				else if(strAttribute == "FOOTER")
				{
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
				else if(strAttribute == "REPEATONX")
				{
					sscanf(str,"%d,%d",	&repeatonx.m_wRepeatCount,&repeatonx.m_wXoffset);
				}
				else if(strAttribute == "REPEATONY")
				{
					sscanf(str,"%d,%d",	&repeatony.m_wRepeatCount,&repeatony.m_wYoffset);
				}
				else if(strAttribute == "TYPE")
				{
					m_strType = str;
				}
				else if(strAttribute == "CLASS")
				{
					m_strClass = str;
				}
				else if(strAttribute == "OVERFLOW")
				{
					m_strOverflow = str;
				}
				else if(strAttribute == "STYLE")
				{
					m_strStyle = str;
				}
				else if(strAttribute == "COLOR")
				{
					m_strColor = str;
				}
				else if(strAttribute == "RGBCOLOR")
				{
					sscanf(str,"%d,%d,%d",&rgbcolor.m_bRed,&rgbcolor.m_bGreen,&rgbcolor.m_bBlue);
				}
				
				else if(strAttribute == "FILLCOLOR")
				{
					m_strFillColor = str;
				}
				else if(strAttribute == "RGBFILLCOLOR")
				{
					nIndex = str.Find(",");
					rgbfillcolor.m_bRed = atoi(str.Left(nIndex));
					
					str = str.Mid(nIndex+1);
					nIndex = str.Find(",");
					rgbfillcolor.m_bGreen = atoi(str.Left(nIndex));

					str = str.Mid(nIndex + 1);
					rgbfillcolor.m_bBlue = atoi(str);					
				}
				else if(strAttribute == "FILLSTYLE")
				{
					m_strFillStyle = str;
				}
				else if(strAttribute == "SUBSTSIGN")
				{					
					m_strSubStsign = str;
				}
				else if(strAttribute == "TITLE")
				{
					lTemp = str.Find("\"");
					str = str.Mid(lTemp + 1);
					lTemp = str.Find("\"");
					str = str.Left(lTemp);
					m_strTitle = str;
				}
				else if(strAttribute == "HORIZONTAL")
				{
					m_strHorizontal = str;
				}
				else if(strAttribute == "VERTICAL")
				{
					m_strVertical = str;
				}
			}
		}
		
		//bRet = ar.ReadString(str);  // END

	}
}

BOOL CXFSFrame::Draw(CDC *pDC)
{
	BOOL bRet = TRUE;


	CSize sizeRepeat;//the offsets for the repeat count
	XFSGetSize(pDC,m_pRootForm->unit.m_strBase,CSize(repeatonx.m_wXoffset,repeatony.m_wYoffset),
		CSize(m_pRootForm->unit.m_wX,m_pRootForm->unit.m_wY),sizeRepeat,
		m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);

	CSize sizeFrame; //position of the frame 
		// size
	CSize sizeOut;

	CXFSField *pField = NULL;//first get the correct field 
	if(m_strFrames == "")
	{
		//POSITION	
		XFSGetSize(pDC,m_pRootForm->unit.m_strBase,CSize(position.m_wX,position.m_wY),
			CSize(m_pRootForm->unit.m_wX,m_pRootForm->unit.m_wY),sizeFrame,
			m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);
		XFSGetSize(pDC,m_pRootForm->unit.m_strBase,CSize(size.m_wWidth,size.m_wHeight),
			CSize(m_pRootForm->unit.m_wX,m_pRootForm->unit.m_wY),sizeOut,
			m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);
	}
	else 
	{
		POSITION pos;
		CString strTemp;
		pos = m_pRootForm->m_listFields.GetHeadPosition();

		while(pos)//get the responding field
		{
			
			pField  = (CXFSField*)m_pRootForm->m_listFields.GetNext(pos);
			if(pField->m_strFieldName == m_strFrames)
			{
				break;
			}
			
		}
		
		XFSGetSize(pDC,m_pRootForm->unit.m_strBase,
			CSize(pField->position.m_wX,pField->position.m_wY),
			CSize(m_pRootForm->unit.m_wX,m_pRootForm->unit.m_wY),
			sizeFrame,
			m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);

		if(pField->m_nIndex == 0)
		{

			XFSGetSize(pDC,m_pRootForm->unit.m_strBase,
				CSize(pField->size.m_wWidth,pField->size.m_wHeight),
				CSize(m_pRootForm->unit.m_wX,m_pRootForm->unit.m_wY),sizeOut,
				m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);
		}
		else
		{
			CSize size0(pField->size.m_wWidth + pField->index.m_wXoffset * pField->m_nIndex,
				pField->size.m_wHeight + pField->index.m_wYoffset * pField->m_nIndex);

			XFSGetSize(pDC,m_pRootForm->unit.m_strBase,size0,
			//	CSize(pField->size.m_wWidth,pField->size.m_wHeight),
			CSize(m_pRootForm->unit.m_wX,m_pRootForm->unit.m_wY),sizeOut,
			m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);
		}
	}





	//get the form aligment
	CPrinterBase pb;
	CSize sizePaper;
	pb.GetPaperWidthAndHeight(sizePaper);//  mm/10
	XFSGetSize(pDC,"MM",CSize(sizePaper.cx/10,sizePaper.cy/10),CSize(m_pRootForm->unit.m_wX,
		m_pRootForm->unit.m_wY),
		sizePaper,
		m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);

	// size of the frame
	CSize sizeForm;
	XFSGetSize(pDC,m_pRootForm->unit.m_strBase,CSize(m_pRootForm->alignment.m_wXoffset,
		m_pRootForm->alignment.m_wYoffset),CSize(m_pRootForm->unit.m_wX,m_pRootForm->unit.m_wY),
		sizeForm,
		m_pRootForm->m_lXExt,m_pRootForm->m_lYExt);

	CSize sizeFormOffset;
	//CSize sizeOut;
	//sizeOut = sizeFrame;

	XFSOffsetForm(m_pRootForm->alignment.m_strAlignment,sizePaper,
		sizeForm,sizeFormOffset);//relative to the form offset to the media
    
	//sizeOut += sizeFormOffset; //form offset add frame offset
	sizeFrame += sizeFormOffset; //form offset add frame offset
	
	//m_nPositionX = sizeOut.cx;
	//m_nPositionY = sizeOut.cy;

	m_nPositionX = sizeFrame.cx;
	m_nPositionY = sizeFrame.cy;

	//size

	//CRect rcDraw(CPoint(m_nPositionX,m_nPositionY),CSize(sizeFrame.cx,sizeFrame.cy));
	CRect rcDraw(CPoint(m_nPositionX,m_nPositionY),sizeOut);

	CRect rcDraw0 = rcDraw;


	///header
	if(!XFSIsNeedToDrawThisPage(m_strHeader,m_pRootForm->m_dwPage))//print the current page?  yes or no
		return FALSE;
	//header 

	//footer
	if(!XFSIsNeedToDrawThisPage(m_strFooter,m_pRootForm->m_dwPage))//print the curent page? yes or no
		return FALSE;

    // 
	
	//footer

	// pen color


	COLORREF crPenColor = 0;

	if(m_strColor != "")
	{
		XFSSetColor(m_strColor,crPenColor);
	}

	if(rgbcolor.m_bRed == 0 && rgbcolor.m_bGreen== 0 && rgbcolor.m_bBlue== 0)
	{
	}
	else
	{
		crPenColor = RGB(rgbcolor.m_bRed,rgbcolor.m_bGreen,rgbcolor.m_bBlue);
	}

	// fill colorRGC
	COLORREF crFillColor = 0;

	if(m_strFillColor != "")
	{
		XFSSetColor(m_strFillColor,crFillColor);
	}

	if(rgbfillcolor.m_bRed == 0 && rgbfillcolor.m_bGreen== 0 && rgbfillcolor.m_bBlue== 0)
	{
	}
	else
	{
		crFillColor = RGB(rgbfillcolor.m_bRed,rgbfillcolor.m_bGreen,rgbfillcolor.m_bBlue);
	}

	// pen
	CPen *pPen = 0;
	CPen *pPenOld = 0;

	if(m_strStyle == _T("SINGLE_THIN "))
	{
		pPen = new CPen(PS_SOLID,1,crPenColor);
	}
	else if(m_strStyle == _T("DOUBLE_THIN"))
	{
		pPen = new CPen(PS_SOLID,1,crPenColor);
	}
	else if(m_strStyle == _T("SINGLE_THICK"))
	{
		pPen = new CPen(PS_SOLID,3,crPenColor);
	}
	else if(m_strStyle == _T("DOUBLE_THICK"))
	{
		pPen = new CPen(PS_SOLID,3,crPenColor);
	}
	else if(m_strStyle == _T("DOTTED"))
	{
		pPen = new CPen(PS_DOT,1,crPenColor);
	}

	if(pPen) pPenOld = pDC->SelectObject(pPen);

	// Brush

	CBrush *pBrush = 0;
	CBrush *pBrushOld = 0;

	if(m_strFillStyle == _T("NONE"))
	{
		pBrush = new CBrush;
		pBrush->CreateStockObject(HOLLOW_BRUSH);
	}
	else if(m_strFillStyle == _T("SOLID"))
	{
		pBrush = new CBrush;
		pBrush->CreateSolidBrush(crFillColor);

	}
	else if(m_strFillStyle == _T("BDIAGONAL"))
	{
		pBrush = new CBrush;
		pBrush->CreateHatchBrush(HS_BDIAGONAL,crFillColor);
	}
	else if(m_strFillStyle == _T("CROSS"))
	{
		pBrush = new CBrush;
		pBrush->CreateHatchBrush(HS_CROSS,crFillColor);
	}
	else if(m_strFillStyle == _T("DIAGCROSS"))
	{
		pBrush = new CBrush;
		pBrush->CreateHatchBrush(HS_DIAGCROSS,crFillColor);
	}
	else if(m_strFillStyle == _T("FDIAGONAL"))
	{
		pBrush = new CBrush;
		pBrush->CreateHatchBrush(HS_FDIAGONAL,crFillColor);
	}
	else if(m_strFillStyle == _T("HORIZONTAL"))
	{
		pBrush = new CBrush;
		pBrush->CreateHatchBrush(HS_HORIZONTAL,crFillColor);
	}
	else if(m_strFillStyle == _T("VERTICAL"))
	{
		pBrush = new CBrush;
		pBrush->CreateHatchBrush(HS_VERTICAL   ,crFillColor);
	}

	//repeatonx 
	if(repeatonx.m_wRepeatCount == 0) repeatonx.m_wRepeatCount = 1;
	if(repeatony.m_wRepeatCount == 0) repeatony.m_wRepeatCount = 1;

	for(long rx=0; rx < repeatonx.m_wRepeatCount; rx++)
	{
		for(long ry=0; ry < repeatony.m_wRepeatCount; ry++)
		{
			rcDraw = rcDraw0;

			int nTemp;
			nTemp  = repeatonx.m_wRepeatCount;
			{
				rcDraw.OffsetRect(sizeRepeat.cx*rx,sizeRepeat.cy*ry);
		
				if(m_strStyle == _T("DOUBLE_THIN") || m_strStyle == _T("DOUBLE_THICK"))
				{
					CBrush BrushDouble;
					BrushDouble.CreateStockObject(HOLLOW_BRUSH);
					pBrushOld = pDC->SelectObject(&BrushDouble);

					CRect rcInflate = rcDraw;
					rcInflate.InflateRect( 5, 5 );
					if(m_strType == "RECTANGLE")
					{
						pDC->Rectangle(&rcInflate);
					}
					else if(m_strType == "ROUNDED_CORNER")
					{
						pDC->RoundRect(&rcInflate,CPoint(70,70));
					}
					else if(m_strType == "ELLIPSE")
					{
						pDC->Ellipse(&rcInflate);
					}
					pDC->SelectObject(pBrushOld);
				}

				if(pBrush) pBrushOld = pDC->SelectObject(pBrush);
				{
					if(m_strType == "RECTANGLE")
					{
						pDC->Rectangle(&rcDraw);
					}
					else if(m_strType == "ROUNDED_CORNER")
					{
						pDC->RoundRect(&rcDraw,CPoint(70,70));
					}
					else if(m_strType == "ELLIPSE")
					{
						pDC->Ellipse(&rcDraw);
					}
				}
				if(pBrushOld) pDC->SelectObject(pBrushOld);
			}
		}
	}

/*
	if(repeatonx.m_wRepeatCount >= 0)
	{
		int nTemp;
		nTemp  = repeatonx.m_wRepeatCount;
		//sizeOut.cx = m_nPositionX;///get the position and the width and height is store in sizeFrame
		//sizeOut.cy = m_nPositionY;
		while(nTemp >= 0)///draw this frame nTemp times
		{
			//rcDraw.OffsetRect(repeatonx.m_wXoffset,0);
			rcDraw.OffsetRect(sizeRepeat.cx,0);
			nTemp --;
			//pDC->Draw3dRect(rcDraw,0,0);

			if(m_strStyle == _T("DOUBLE_THIN") || m_strStyle == _T("DOUBLE_THICK"))
			{
				CBrush BrushDouble;
				BrushDouble.CreateStockObject(HOLLOW_BRUSH);
				pBrushOld = pDC->SelectObject(&BrushDouble);

				CRect rcInflate = rcDraw;
				rcInflate.InflateRect( 5, 5 );
				if(m_strType == "RECTANGLE")
				{
					pDC->Rectangle(&rcInflate);
				}
				else if(m_strType == "ROUNDED_CORNER")
				{
					pDC->RoundRect(&rcInflate,CPoint(70,70));
				}
				else if(m_strType == "ELLIPSE")
				{
					pDC->Ellipse(&rcInflate);
				}
				pDC->SelectObject(pBrushOld);
			}

			if(pBrush) pBrushOld = pDC->SelectObject(pBrush);
			{
				if(m_strType == "RECTANGLE")
				{
					pDC->Rectangle(&rcDraw);
				}
				else if(m_strType == "ROUNDED_CORNER")
				{
					pDC->RoundRect(&rcDraw,CPoint(70,70));
				}
				else if(m_strType == "ELLIPSE")
				{
					pDC->Ellipse(&rcDraw);
				}
			}
			if(pBrushOld) pDC->SelectObject(pBrushOld);
		}
	}


	//repeatony 
	rcDraw = rcDraw0;
	if(repeatony.m_wRepeatCount >= 0)
	{
		int nTemp;
		nTemp = repeatony.m_wRepeatCount;
		while(nTemp >= 0)
		{
			//rcDraw.OffsetRect(0,repeatony.m_wYoffset);
			rcDraw.OffsetRect(0,sizeRepeat.cy);
			nTemp --;
			
			if(m_strStyle == _T("DOUBLE_THIN") || m_strStyle == _T("DOUBLE_THICK"))
			{
				CBrush BrushDouble;
				BrushDouble.CreateStockObject(HOLLOW_BRUSH);
				pBrushOld = pDC->SelectObject(&BrushDouble);

				CRect rcInflate = rcDraw;
				rcInflate.InflateRect( 5, 5 );
				if(m_strType == "RECTANGLE")
				{
					pDC->Rectangle(&rcInflate);
				}
				else if(m_strType == "ROUNDED_CORNER")
				{
					pDC->RoundRect(&rcInflate,CPoint(70,70));
				}
				else if(m_strType == "ELLIPSE")
				{
					pDC->Ellipse(&rcInflate);
				}
				if(pBrushOld) pDC->SelectObject(pBrushOld);
			}

			if(pBrush) pBrushOld = pDC->SelectObject(pBrush);
			{
				if(m_strType == "RECTANGLE")
				{
					pDC->Rectangle(&rcDraw);
				}
				else if(m_strType == "ROUNDED_CORNER")
				{
					pDC->RoundRect(&rcDraw,CPoint(70,70));
				}
				else if(m_strType == "ELLIPSE")
				{
					pDC->Ellipse(&rcDraw);
				}
				if(pBrushOld) pDC->SelectObject(pBrushOld);
			}
		}
		
		//pDC->Draw3dRect(rcDraw,0,0);
	}
*/

	if(pBrushOld) pDC->SelectObject(pBrushOld);
	if(pBrush) delete pBrush;

	if(pPenOld) pDC->SelectObject(pPenOld);
	if(pPen) delete pPen;

	if(m_strOverflow == "TERMINATE"){}
	else if(m_strOverflow == "TRUNCATE"){}
	else if(m_strOverflow == "BESTFIT"){}


	/*
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

    pDC->Draw3dRect(rcDraw,0,0);
	*/
	return bRet;
}

HRESULT CXFSFrame::CheckAgainstMedia(CXFSMedia *pXFSMedia)
{
	// To Do
	return S_OK;
}
