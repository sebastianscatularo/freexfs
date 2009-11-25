// XFS SPI.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "XFS SPI.h"
#include "XFS SPIaw.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE XFSSPIDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("XFS SPI.AWX Initializing!\n");
		
		// Extension DLL one-time initialization
		AfxInitExtensionModule(XFSSPIDLL, hInstance);

		// Insert this DLL into the resource chain
		new CDynLinkLibrary(XFSSPIDLL);

		// Register this custom AppWizard with MFCAPWZ.DLL
		SetCustomAppWizClass(&XFSSPIaw);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("XFS SPI.AWX Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(XFSSPIDLL);
	}
	return 1;   // ok
}
