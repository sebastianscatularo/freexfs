// PTR SPI.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "PTR SPI.h"
#include "PTR SPIaw.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE PTRSPIDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("PTR SPI.AWX Initializing!\n");
		
		// Extension DLL one-time initialization
		AfxInitExtensionModule(PTRSPIDLL, hInstance);

		// Insert this DLL into the resource chain
		new CDynLinkLibrary(PTRSPIDLL);

		// Register this custom AppWizard with MFCAPWZ.DLL
		SetCustomAppWizClass(&PTRSPIaw);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("PTR SPI.AWX Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(PTRSPIDLL);
	}
	return 1;   // ok
}
