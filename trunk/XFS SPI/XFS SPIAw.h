#if !defined(AFX_XFSSPIAW_H__320F527C_D90C_4922_8A03_0BDD2A23FB92__INCLUDED_)
#define AFX_XFSSPIAW_H__320F527C_D90C_4922_8A03_0BDD2A23FB92__INCLUDED_

// XFS SPIaw.h : header file
//

class CDialogChooser;

// All function calls made by mfcapwz.dll to this custom AppWizard (except for
//  GetCustomAppWizClass-- see XFS SPI.cpp) are through this class.  You may
//  choose to override more of the CCustomAppWiz virtual functions here to
//  further specialize the behavior of this custom AppWizard.
class CXFSSPIAppWiz : public CCustomAppWiz
{
public:
	virtual CAppWizStepDlg* Next(CAppWizStepDlg* pDlg);
		
	virtual void InitCustomAppWiz();
	virtual void ExitCustomAppWiz();
	virtual void CustomizeProject(IBuildProject* pProject);
};

// This declares the one instance of the CXFSSPIAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global XFSSPIaw.  (Its definition is in XFS SPIaw.cpp.)
extern CXFSSPIAppWiz XFSSPIaw;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XFSSPIAW_H__320F527C_D90C_4922_8A03_0BDD2A23FB92__INCLUDED_)
