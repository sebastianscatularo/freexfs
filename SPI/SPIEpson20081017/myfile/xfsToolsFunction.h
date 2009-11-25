#ifndef _XFSTOOLSFUNCTION_H_
#define _XFSTOOLSFUNCTION_H_

/*
  strSource :this string is header or footer e.g 1 10-11 ,all 
*/
BOOL XFSIsNeedToDrawThisPage(CString strSource,int nPageNo);

/*
  Get the width and height of form ...
*/
int XFSGetSize(CDC *pDC,CString strBase,CSize sizeIn,CSize unit,CSize& sizeOut,float xExt,float yExt);
/*
deal form alignment topleft,topright .... xoffset,yoffset and so on 
*/
void XFSOffsetForm(const CString strAlignment,const CSize sizePaper,const CSize sizeOffset,CSize &sizeOut);


/*set font style*/
void XFSSetFontStyle(CDC *pDC,CString strStyle,CFont &font);

/*set pen */
void XFSSetPenStyle(CDC* pDC,CString strStyle,CPen &pen);


/*
 HORIZONTAL AND VERTICAL ALIGNMENT
*/
void XFSHAlignment(CString strBuffer,UINT &uFormat);
void XFSVAlignment(CString strBuffer,UINT &uFormat);

void XFSSetColor(CString strColor,COLORREF &rColor);

#endif;