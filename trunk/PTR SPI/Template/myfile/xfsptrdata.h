#ifndef _XFSPTRDATA_H_
#define _XFSPTRDATA_H_
#include "myfile\\xfstoolsfunction.h"


/*enum {TEXT,MICR,OCR,MSF,BARCODE,GRAPHIC,PAGEMARK};//TYPE

enum {BESTFIT_SCALIGN,ASIS,MAINTAINASPECT};//SCALIGN

enum {NONE,ABOVE,BELOW,BOTH};//BARCODE

enum {OPTIONAL_CLASS,STATIC,REQUIRED};//CLASS

enum {WRITE,READ,READWRITE};//ACCESS

enum {TERMINATE,TRUNCATE,BESTFIT_OVERFLOW,OVERWRITE,WORDWRAP};//OVERFLOW

enum {NORMAL,BOLD,ITALIC,UNDER,DOUBLEUNDER,DOUBLE_WIDTH,TRIPLE,QUADRUPLE,STRIKETHROUGH,ROTATE90,\
ROTATE270,UPSIDEDOWN,PROPORTIONAL,DOUBLEHIGH,TRIPLEHIGH,QUADRUPLEHIGH,CONDENSED,SUPERSCRIPT,\
SUBSCRIPT,OVERSCORE,LETTERQUALITY,NEARLETTERQUALITY,DOUBLESTRIKE,XFSOPAQUE};//STYLE

enum {NOCHANGE,UPPER,LOWER};//CASE

enum {LEFT,RIGHT,CENTER_HORIZONTAL,JUSTFY};//HORIZONTAL

enum {BOTTOM,CENTER_VERTICAL,TOP};//VERTICAL

enum {BLACK,WHITE,GRAY,RED,BLUE,GREEN,YELLOW};//COLOR

enum {FRONT,BACK};*/
///////////////////////////////////////////
typedef struct _XFS_UNIT{
		CString m_strBase;
		DWORD   m_wX;
		DWORD   m_wY;
}XFS_UNIT;

typedef struct _XFS_SIZE{
		DWORD   m_wWidth;
		DWORD   m_wHeight;
}XFS_SIZE;

typedef struct _XFS_ALIGNMENT{
		CString m_strAlignment;
		DWORD   m_wXoffset;
		DWORD   m_wYoffset;
}XFS_ALIGNMENT;

typedef struct _XFS_VERSION{
		DWORD   m_wMajor;
		DWORD   m_wMinor;
		CString m_strDate;
		CString m_strAuthor;
}XFS_VERSION;

typedef struct _XFS_POSITION{
	DWORD m_wX;
	DWORD m_wY;
}XFS_POSITION;

typedef struct _XFS_INDEX{
		DWORD   m_wRepeatcount;
		DWORD   m_wXoffset;
		DWORD   m_wYoffset;
	}XFS_INDEX;
typedef struct _XFS_RGBCOLOR{
		DWORD    m_bRed;
		DWORD    m_bGreen;
		DWORD    m_bBlue;
}XFS_RGBCOLOR;

typedef struct _XFS_RGBFILLCOLOR{
		DWORD    m_bRed;
		DWORD    m_bGreen;
		DWORD    m_bBlue;
}XFS_RGBFILLCOLOR;

typedef struct _XFS_REPEATONX{
		DWORD   m_wRepeatCount;
		DWORD   m_wXoffset;
	}XFS_REPEATONX;
typedef struct _XFS_REPEATONY{
		DWORD   m_wRepeatCount;
		DWORD   m_wYoffset;
}XFS_REPEATONY;

typedef struct _XFS_PRINTAREA{
	DWORD   m_wX;
	DWORD   m_wY;
	DWORD   m_wWidth;
	DWORD   m_wHeight;
}XFS_PRINTAREA;

typedef struct _XFS_RESTRICTED{
	DWORD   m_wX;
	DWORD   m_wY;
	DWORD   m_wWidth;
	DWORD   m_wHeight;
}XFS_RESTRICTED;
////////////////////////////////////////////


struct _wfs_ptr_form_field
{
	XFS_POSITION position;

	CString m_strFollows;
	CString m_strHeader;
	CString m_strFooter;
	
	CString m_strSide;

	XFS_SIZE size;
	XFS_INDEX index;

	CString m_strType;
	CString m_strScaling;
	CString m_strBarcode;
	CString m_strClass;
	CString m_strAccess;
	CString m_strOverflow;


	CString m_strStyle;
	CString m_strCase;
	CString m_strHorizontal;
	CString m_strVertical;
	CString m_strColor;
	
	XFS_RGBCOLOR rgbcolor;
	DWORD       m_dwLanguage;
	CString m_strFont;

	DWORD       m_dwPointSize;
	DWORD       m_dwCpi;
	DWORD       m_dwLpi;
	CString m_strFormat;
	CString m_strInitialValue;

	///extra
	CString m_strFieldName;
	//_wfs_ptr_form_field *pNext;

	DWORD m_dwPage;
};

struct _wfs_ptr_form_frame{
	XFS_POSITION position;
	CString m_strFrames;
	CString m_strHeader;
	CString m_strFooter;
	CString m_strSide;

	XFS_SIZE size;
	XFS_REPEATONX repeatonx;
	XFS_REPEATONY repeatony;
	
	CString m_strType;
	CString m_strClass;
	CString m_strOverflow;
	CString m_strStyle;
	CString m_strColor;
	XFS_RGBCOLOR rgbcolor;
	CString m_strFillColor;
	XFS_RGBFILLCOLOR rgbfillcolor;	
	CString m_strSubStsign;
	CString m_strTitle;
	CString m_strHorizontal;
	CString m_strVertical;
	CString m_strFillStyle;
	///extra
	CString m_strFrameName;
	_wfs_ptr_form_frame *pNext;
	DWORD m_dwPage;
};

struct _wfs_ptr_form_subform
{
	XFS_POSITION position;
	XFS_SIZE size;
	
	//WFSPTRFORM_FIELD   *m_pNextField;//point to 
    //WFSPTRFORM_FRAME   *m_pNextFrame;

	CPtrList m_listFields;
	CPtrList m_listFrames;

	//extra
	CString m_strSubFormName;
	_wfs_ptr_form_subform *pNext;
	DWORD       m_dwFields;//number of field
	DWORD       m_dwFrames;
	DWORD       m_dwSize;//size of this struct

	DWORD       m_dwPage;///?	
};


struct _wfs_ptr_form
{
	XFS_UNIT unit;
	XFS_SIZE size;
	XFS_ALIGNMENT alignment;
	
	CString m_strOrientation;
	DWORD       m_dwSkew;
	XFS_VERSION version;
	DWORD       m_dwLanguage;
	CString m_strCopyRight;
	CString m_strTitle;
	CString m_strComment;
	CString m_strUserPrompt;

	//WFSPTRFORM_FIELD    *pfield;///every fieldname take 100 byte spaces 
	//WFSPTRFORM_FRAME    *pframe;//as above
	//WFSPTRFORM_SUBFORM  *psubform;//as above
	CPtrList m_listFields;
	CPtrList m_listFrames;
	CPtrList m_listSubforms;
	CString m_strFormName;
	//////extra;

	DWORD       m_dwPage;
	DWORD       dwField;////the number of fields
	DWORD       dwFrame;////the number of frames
	DWORD       dwSubForm;////the number of subformnames;
	DWORD       dwSize;//size of this struct 
};

struct _wfs_ptr_media
{

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

#endif;