#ifndef _TOOLSFUNCTION_H_
#define _TOOLSFUNCTION_H_

#include "xfsptrdata.h"


/*
return value:
0 : can't not find pflag in pSource
1:  find pflag in pSource
-1: pSource error can't find begin keyword
-2: pSource error cna't find end keyword
*/
int TakeOutField(char *pSource,int &nLength,const char *pflag);//将field \subform ...begin end 去掉

char*  TrimString(char *pSource,int &nLength);//delete space
char*  TrimStringBegin(char *pSource,int &nLength);
char*  TrimStringEnd(char *pSource,int &nLength);
       
char*  TrimStringBeginAndEnd(char *pSource,int &nLength);

char*  TrimSubString(char *pSource,int &nLength,int nFrom,int nTo);//take out from nFrom to nTo substring
int GetFieldFromform(const char *pSource,const int nLength,char *pOut,int &nOut,const char *pflag);//get field \frame from buffer


int  GetSubFormFromform(const char *pSource,const int nLength,char *pOut,int& nOut);//get subform from buffer
/*
  if return value 
                   1: this buffer has subform item
				   0: this buffer hasn't subform item
*/

int aTodecfromhexTo(char *pSource,int nLength);//// change string 0x0409 to dec


int GetData_FormBuffer(const char *pSource,const int nLength,char *pOut,int nOutBufferLength)  ;

void GetData_SubFormBuffer(const char *pSource,const int nLength,char *pOut,int &nOutBufferLength);
    
int GetData_FieldBuffer(const char *pSource,const int nLength,char *pOut,int &nOutBufferLength);

int GetData_FrameBuffer(const char *pSource,const int nLength,char *pOut,int &nOutBufferLength);


///

//英寸转换为 毫米    1英寸=  25.4毫米
float FromInchToMilliMeter(float fInch);
float FromMilliMeterToInch(float MilliMeter);


#endif;