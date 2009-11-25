#if !defined(AFX_PTRSPIAW_H__5869D026_36BF_49CB_80F3_1F34CDC67276__INCLUDED_)
#define AFX_PTRSPIAW_H__5869D026_36BF_49CB_80F3_1F34CDC67276__INCLUDED_


// PTR SPIaw.h : header file
//

class CDialogChooser;

// All function calls made by mfcapwz.dll to this custom AppWizard (except for
//  GetCustomAppWizClass-- see PTR SPI.cpp) are through this class.  You may
//  choose to override more of the CCustomAppWiz virtual functions here to
//  further specialize the behavior of this custom AppWizard.
class CPTRSPIAppWiz : public CCustomAppWiz
{
public:
	virtual CAppWizStepDlg* Next(CAppWizStepDlg* pDlg);
		
	virtual void InitCustomAppWiz();
	virtual void ExitCustomAppWiz();
	virtual void CustomizeProject(IBuildProject* pProject);
};

// This declares the one instance of the CPTRSPIAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global PTRSPIaw.  (Its definition is in PTR SPIaw.cpp.)
extern CPTRSPIAppWiz PTRSPIaw;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTRSPIAW_H__5869D026_36BF_49CB_80F3_1F34CDC67276__INCLUDED_)
