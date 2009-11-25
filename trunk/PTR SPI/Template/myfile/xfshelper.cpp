#include "stdafx.h"
#include "xfshelper.h"
#include "xfsconf.h"
void WriteToRegister(char *strFormName,char *strFileName,int nFileNameLength)
{
	DWORD  dwResult;
	HKEY   hKeyForm;
	HRESULT hr;

	hr = WFMCreateKey(WFS_CFG_HKEY_XFS_ROOT,
		"form",
		&hKeyForm,
		&dwResult
		);
	if(hr != WFS_SUCCESS) return;

	HKEY  hKeyTarget;

	hr = WFMCreateKey(hKeyForm,
		strFormName,
		&hKeyTarget,
		&dwResult
		);

	if(hr != WFS_SUCCESS) return;

	hr = WFMSetValue(hKeyTarget,
		"name",
		strFileName,
		nFileNameLength
		);

	if(hr != WFS_SUCCESS) return ;

}

/*pFormNames is output,and nNumbers is output too*/
void EnumFormFromRegister(char *pFormNames,int &nNumbers)
{
	//DWORD  dwResult;
	HKEY   hKeyForm;
	HRESULT hr;

	hr = WFMOpenKey(
		WFS_CFG_MACHINE_XFS_ROOT,
		"form",
		&hKeyForm
		);
	if(hr != WFS_SUCCESS) return ; 

	HKEY  hKeyFormSub;

	hr = WFMOpenKey(
		hKeyForm,
		"epson",
		&hKeyFormSub);
	if(hr != WFS_SUCCESS) return ;

	char buf[500];
	memset(buf,0,sizeof(buf));
	DWORD  cchName;
	int nNum;
	nNum = 0;
	if(pFormNames == NULL)///get the length
	{
		while(1)
		{
			cchName = sizeof(buf);
			memset(buf,0,sizeof(buf));
			hr = WFMEnumKey(hKeyFormSub,
				nNum,
				buf,
				&cchName,
				NULL
				);
			if(hr == WFS_ERR_CFG_NO_MORE_ITEMS) return ;
			nNum ++;
		}
		nNumbers = nNum;
	}
	else
	{
		for(int i=0;i<nNumbers;i++)
		{
			cchName = sizeof(buf);
			memset(buf,0,sizeof(buf));
			hr = WFMEnumKey(hKeyFormSub,
				i,
				pFormNames + i*100,//100 is the length of every form name's length
				&cchName,
				NULL
				);
			if(hr == WFS_ERR_CFG_NO_MORE_ITEMS) return ;			
		}
	}
	///nNum is the number of form numbers ;
}


///through the name of the form ,then get the path of the detail of the form
char* GetFormName(char *pFormKey,char *pOut,int &nFileNameLength)
{
	char buf[300] = {0};
	DWORD  dwResult;
	HKEY   hKeyForm;
	HRESULT hr;
                    
	hr = WFMOpenKey(WFS_CFG_MACHINE_XFS_ROOT,
		"form",
		&hKeyForm
		);
	if(hr != WFS_SUCCESS) return NULL;

	HKEY  hKeyEpson;
	
	hr = WFMOpenKey(hKeyForm,"epson",&hKeyEpson);
	if(hr != WFS_SUCCESS) return NULL;


	HKEY  hKeyTarget;

	hr = WFMOpenKey(hKeyEpson,
		pFormKey,
		&hKeyTarget
		);

	if(hr != WFS_SUCCESS) return NULL;

	memset(buf,0,sizeof(buf));
	hr = WFMQueryValue(hKeyTarget,
		"name",
		buf,
		&dwResult
		);
	if(hr != WFS_SUCCESS) return NULL;

	nFileNameLength = strlen(buf);
	memcpy(pOut,buf,nFileNameLength);

	return pOut;
}