// XFSPrint.h : Declaration of the CXFSPrint

#ifndef __XFSPRINT_H_
#define __XFSPRINT_H_

#include "resource.h"       // main symbols
#include <atlctl.h>

#include "..\Manager\NI_XFSAPI.h"
#include "..\Manager\NI_XFSManager.h"
#include "xfsptr.h"

/////////////////////////////////////////////////////////////////////////////
// CXFSPrint
class ATL_NO_VTABLE CXFSPrint : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IXFSPrint, &IID_IXFSPrint, &LIBID_PTROCXLib>,
	public CComControl<CXFSPrint>,
	public IPersistStreamInitImpl<CXFSPrint>,
	public IOleControlImpl<CXFSPrint>,
	public IOleObjectImpl<CXFSPrint>,
	public IOleInPlaceActiveObjectImpl<CXFSPrint>,
	public IViewObjectExImpl<CXFSPrint>,
	public IOleInPlaceObjectWindowlessImpl<CXFSPrint>,
	public IConnectionPointContainerImpl<CXFSPrint>,
	public IPersistStorageImpl<CXFSPrint>,
	public ISpecifyPropertyPagesImpl<CXFSPrint>,
	public IQuickActivateImpl<CXFSPrint>,
	public IDataObjectImpl<CXFSPrint>,
	public IProvideClassInfo2Impl<&CLSID_XFSPrint, &DIID__IXFSPrintEvents, &LIBID_PTROCXLib>,
	public IPropertyNotifySinkCP<CXFSPrint>,
	public CComCoClass<CXFSPrint, &CLSID_XFSPrint>,
	public IObjectSafetyImpl<CXFSPrint,INTERFACESAFE_FOR_UNTRUSTED_CALLER>  // Tang

{
public:
	CXFSPrint()
	{
	}
	HRESULT CXFSPrint::FinalConstruct();
	void CXFSPrint::FinalRelease();
	CStringArray m_arrayFields;

	CString	m_strLocalService;
	CString	m_strVersionSPI;
	CString	m_strVersionXFS;
	CString	m_strXFSPath;
	CString	m_strFormName;
	CString	m_strMediaName;

	HMODULE m_hLib;

	CString m_strResult;
	TCHAR *m_pTextOutExecute;
	BOOL m_bStartUp;

	BOOL CXFSPrint::LoadManagerFunction(CString strPath);

DECLARE_REGISTRY_RESOURCEID(IDR_XFSPRINT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXFSPrint)
	COM_INTERFACE_ENTRY(IXFSPrint)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(IObjectSafety)  // Tang
END_COM_MAP()

BEGIN_PROP_MAP(CXFSPrint)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CXFSPrint)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CXFSPrint)
	CHAIN_MSG_MAP(CComControl<CXFSPrint>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);



// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IXFSPrint
public:
	STDMETHOD(get_MediaName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_MediaName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Result)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Result)(/*[in]*/ BSTR newVal);
	STDMETHOD(DoPrint)();
	STDMETHOD(AddField)(/*[in]*/BSTR bstrTag, /*[in]*/BSTR bstrValue);
	STDMETHOD(get_FormName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FormName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_LogicalService)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_LogicalService)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_XFSMgrPath)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_XFSMgrPath)(/*[in]*/ BSTR newVal);

	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		RECT& rc = *(RECT*)di.prcBounds;
		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);

		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("ATL 3.0 : XFSPrint");
		TextOut(di.hdcDraw, 
			(rc.left + rc.right) / 2, 
			(rc.top + rc.bottom) / 2, 
			pszText, 
			lstrlen(pszText));

		return S_OK;
	}
};

#endif //__XFSPRINT_H_
