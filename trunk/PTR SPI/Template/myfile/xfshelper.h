#ifndef _XFSHELPER_H_
#define _XFSHELPER_H_

/*
 form proc function
*/


//write form information to windows register
void  WriteToRegister(char *strFormName,char *strFileName);
void EnumFormFromRegister(char *pFormNames,int &nNumbers);
char* GetFormName(char *pFormKey,char *pOut,int &nFileNameLength);

#endif;