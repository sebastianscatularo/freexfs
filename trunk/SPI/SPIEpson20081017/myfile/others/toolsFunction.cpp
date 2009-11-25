#include "stdafx.h"
#include "toolsfunction.h"

int TakeOutField(char *pSource,int &nLength,const char *pflag)//½«field begin end È¥µô
{
	char *pTemp;
	int  nTemp;
	char buf[500]={0};//save the remain string


	pTemp = strstr(pSource,pflag);
	if(pTemp == NULL)
		return 0;
	nTemp = strlen(pTemp);
	memcpy(buf,pSource,nLength-nTemp);


	nTemp = strlen(pflag);
	pTemp += nTemp;
	

	pTemp = strstr(pTemp,"begin");	
	nTemp= strlen("begin");
	if(pTemp == NULL)
		return -1;
	pTemp += nTemp;

	pTemp = strstr(pTemp,"end");
	nTemp= strlen("end");
	if(pTemp == NULL)
		return -2;
	pTemp += nTemp;

	nTemp = strlen(pTemp);
	memcpy(buf+strlen(buf),pSource+nLength-nTemp,nTemp);

	sprintf(pSource,buf);
	nLength = strlen(buf);
	return 1;

}

int GetFieldFromform(const char *pSource,const int nLength,char *pOut,int &nOut,const char *pflag)//get field \frame \*****subform***** from buffer
{
	char *pTemp;
	int nFrom,nTo;
	//char buf[500] = {0};

	pTemp = strstr(pSource,pflag);
	if(pTemp == NULL)
	{
		return 0;
	}
	nFrom = pTemp - pSource;
	pTemp = strstr(pTemp,"END");
	if(pTemp == NULL) return -2;
	nTo = pTemp - pSource;
	nTo += strlen("end");
	char *p;
	p = new char[nTo - nFrom + 10];
	memset(p,0,nTo-nFrom+10);
	memcpy(p,pSource+nFrom,nTo-nFrom);
	memset(pOut,0,nLength);
	memcpy(pOut,p,nTo-nFrom);
	nOut = nTo-nFrom;
	delete p;
	return 1;
}

int  GetSubFormFromform(const char *pSource,const int nLength,char *pOut,int &nOut)
{
	char *pTemp= NULL;
	char *pTemp2= NULL;
	char *pTemp3 = NULL;
	int nFrom,nTo;

	char *pReturn = NULL;


	pTemp = strstr(pSource,"subform");
	
	if(pTemp == NULL)
		return 0;
	nFrom = pTemp - pSource;

	pTemp = strstr(pTemp,"begin");
	pTemp += strlen("begin");

	pTemp2 = pTemp;
	pTemp3 = pTemp;


	while(1)
	{
		pTemp2 = strstr(pTemp2,"begin");

		pTemp3 = strstr(pTemp3,"end");
		if(pTemp3)
			nTo = pTemp3 - pSource+3;

		if( (pTemp2 > pTemp3) || (pTemp2 == NULL) ) // ok;
		{
			break;
		}
		pTemp2 += strlen("begin");
		pTemp3 += strlen("end");
	}
	pTemp = new char[nTo-nFrom + 10];
	memset(pTemp,0,nTo-nFrom+10);
	memcpy(pTemp,pSource+nFrom,nTo-nFrom);	

	memset(pOut,0,nOut);
	memcpy(pOut,pTemp,nTo-nFrom);//return value
	nOut = nTo - nFrom;
	
	delete pTemp;
	return 1;

}

char*  TrimString(char *pSource,int &nLength)
{
	char *p;
	p = new char[nLength+5];

	if(p== NULL)
		return NULL;
	memset(p,0,nLength+5);

	for(int i=0,nIndex=-1;i<nLength;i++)
	{
		if(pSource[i] != ' ')
		p[++nIndex] = pSource[i];
	}
	memset(pSource,0,nLength);
	nLength = nIndex +1;//real length;

	memcpy(pSource,p,nLength);
	
	delete p;
	return pSource;
}
/*
delete the space ahead the string 
*/
char*  TrimStringBegin(char *pSource,int &nLength)
{
	char *p;
	p = new char [nLength+1];
	if(p == NULL)
		return NULL;
	int nTemp;
	nTemp = 0;
	while(1)
	{
		if(pSource[nTemp] == ' ')
			nTemp ++;
		else
			break;
	}
	memset(p,0,nLength + 1);
	
	memcpy(p,pSource+nTemp,nLength-nTemp);

	memset(pSource,0,nLength);
	
	nLength -= nTemp;
	memcpy(pSource,p,nLength);
	delete p;
	return pSource;	
}

/* take off the space character at the end of the string 
*/
char*  TrimStringEnd(char *pSource,int &nLength)
{
	strrev(pSource);
	TrimStringBegin(pSource,nLength);
	strrev(pSource);
	return pSource;
}
       
char*  TrimStringBeginAndEnd(char *pSource,int &nLength)
{
	TrimStringBegin(pSource,nLength);

	TrimStringEnd(pSource,nLength);
	return pSource;
}
char*  TrimSubString(char *pSource,int &nLength,int nFrom,int nTo)
{
	if(nTo < nFrom)
		return pSource;
	char *p = new char[nLength];
	memset(p,0,nLength);

	memcpy(p,pSource,nFrom);
	memcpy(p+nFrom,pSource+nTo,nLength-nTo);

	memset(pSource,0,nLength);
	memcpy(pSource,p,nLength-nTo+nFrom);
	nLength =nLength - nTo + nFrom;

	delete p;
	return pSource;

}

int aTodecfromhex(char *pSource,int nLength)
{
	char *p;
	int  nIndex;
	p = strstr(pSource,"x");

	nIndex = p-pSource;

	p += 1;

	char buf[100];
	memset(buf,0,sizeof(buf));
	memcpy(buf,p,nLength-nIndex);

	int  nTemp;
	nTemp = strlen(buf);

	int  nTemp2;
	int  nReturn;

	nReturn = 0;
	char bufTemp[20];
	for(int i=0;i<nTemp;i++)
	{
		memset(bufTemp,0,sizeof(bufTemp));
		memcpy(bufTemp,buf+i,1);
		nTemp2 = atoi(bufTemp);
		
		int j=nTemp2;
		int z = nTemp-i-1;
		while(z--)
		{
			nTemp2 *= 16;
		}
		nReturn += nTemp2;
	}
    return nReturn;
}
     

int  GetData_FormBuffer(const char *pSource,const int nLength,char *pOut,int nOutBufferLength) 
{
    // TODO: Add your command handler code here
	/*char *p = NULL;
	p = new char[nLength + 10];
	
	if(p==NULL)
		return -2;//alloc memory fail
	memset(p,0,nLength + 10);
	memcpy(p,pSource,nLength);

	int nLen;
	char *pp;
	int  nIndexFrom,nIndexTo;
	char *pTemp;
	char *pTemp2;
	char buf[100];
	int nTemp;
	int nNumbers;

	WFSPTRFORM  *pwfsptrform;
	pwfsptrform = (WFSPTRFORM*)new WFSPTRFORM;
	memset((void*)pwfsptrform,0,sizeof(WFSPTRFORM));

	nLen = nLength;


	////step get the subform from buffer
	nTemp = 0;
	memset(p,0,nLength+10);
	memcpy(p,pSource,nLength);
	pTemp = p;
	char *pReturn=NULL;
	int  nReturn;

	pReturn = new char[nLength];
	if(pReturn == NULL)
		return -1;
	nReturn = nLength;

	WFSPTRFORM_SUBFORM *psub;
	psub = pwfsptrform->psubform;
	nNumbers = 0;
    while(1)
	{		
		pTemp = strstr(p,"XFSSUBFORM");
        if(pTemp == NULL)
			break;
		nTemp = pTemp - p;
		GetSubFormFromform(pTemp,nTemp,pReturn,nReturn);
		TrimSubString(p,nLen,nTemp,nTemp+nReturn);
		//nLen -= nReturn;
		LPWFSPTRFORM_SUBFORM pwfssubform;
		pwfssubform = (LPWFSPTRFORM_SUBFORM)new WFSPTRFORM_SUBFORM;
		memset(pwfssubform,0,sizeof(WFSPTRFORM_SUBFORM));
		GetData_SubFormBuffer(pReturn,nReturn,(char*)pwfssubform,nReturn);	
		
		psub = pwfssubform;
		psub = pwfssubform->pNext;
		nNumbers++;
	}
	pwfsptrform->dwSubForm = nNumbers;
	///now  the string is p, and length is len


	WFSPTRFORM_FIELD *pfield;
	pfield = pwfsptrform->pfield;
	nNumbers  = 0;
	while(1)
	{
		pTemp = strstr(p,"XFSFIELD");
		if(pTemp ==NULL)
			break;
		nTemp = pTemp - p;
		GetFieldFromform(pTemp,nTemp,pReturn,nReturn,"XFSFIELD");
		TrimSubString(p,nLen,nTemp,nTemp+nReturn);
		//nLen -= nReturn;
		LPWFSPTRFORM_FIELD pwfsfield;
		pwfsfield = (LPWFSPTRFORM_FIELD)new WFSPTRFORM_FIELD;
		memset(pwfsfield,0,sizeof(WFSPTRFORM_FIELD));
		GetData_FieldBuffer(pReturn,nReturn,(char*)pwfsfield,nReturn);
		pfield = pwfsfield;
		pfield = pwfsfield->pNext;
		nNumbers ++;
	}
	pwfsptrform->dwField = nNumbers;

	WFSPTRFORM_FRAME *pframe;
	pframe = pwfsptrform->pframe;
	nNumbers = 0;
	while(1)
	{
		pTemp = strstr(p,"XFSFRAME");
		if(pTemp == NULL)
			break;
		nTemp = pTemp - p ;
		GetFieldFromform(pTemp,nTemp,pReturn,nReturn,"XFSFRAME");
		TrimSubString(p,nLen,nTemp,nTemp+nReturn);
		//nLen -= nReturn;
		WFSPTRFORM_FRAME *pwfsframe;
		pwfsframe = (LPWFSPTRFORM_FRAME) new WFSPTRFORM_FRAME;
		memset(pwfsframe,0,sizeof(WFSPTRFORM_FRAME));
		GetData_FrameBuffer(pReturn,nReturn,(char*)pwfsframe,nReturn);
		pframe = pwfsframe;
		pframe = pwfsframe->pNext;
	}
	pwfsptrform->dwFrame = nNumbers;

	////now the buffer not contain subform ,and field ,and frame keywod 


	pp = strstr(p,"UNIT");//	
	if(pp)
	{
		nIndexFrom = pp-p;
		pp += strlen("UNIT");
		pTemp = strstr(pp,",");
		
		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));

		memcpy(buf,pp,nTemp);
		memcpy(pwfsptrform->unit.base,buf,strlen(buf));
		pTemp++;
		pTemp2 = strstr(pTemp,",");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		pwfsptrform->unit.x = atoi(buf);
		
		pTemp+= nTemp+1;

		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		pwfsptrform->unit.y = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}

    pp = strstr(p,"SIZE");
	if(pp)
    {
		nIndexFrom = pp-p;
		pp += strlen("SIZE");
		pTemp = strstr(pp,",");
		
		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		pwfsptrform->size.width = atoi(buf);

		pTemp++;

		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		pwfsptrform->size.height = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}

	pp = strstr(p,"ALIGNMENT");
	if(pp)
	{
		nIndexFrom = pp-p;
		pp += strlen("ALIGNMENT");
		pTemp = strstr(pp,",");		
			
		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));

		memcpy(buf,pp,nTemp);
		memcpy(pwfsptrform->alignment_form.alignment,buf,strlen(buf));


		pTemp++;
		pTemp2 = strstr(pTemp,",");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		pwfsptrform->alignment_form.xoffset = atoi(buf);
		
		pTemp+= nTemp+1;

		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		pwfsptrform->alignment_form.yoffset = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);	
		
	}

	pp = strstr(p,"ORIENTATION");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("ORIENTATION");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(pwfsptrform->orientation,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"SKEW");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("SKEW");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		pwfsptrform->skew = atoi(buf);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

    pp = strstr(p,"VERSION");//	
	if(pp)
	{
		nIndexFrom = pp-p;
		pp += strlen("VERSION");
		pTemp = strstr(pp,",");
		
		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));

		memcpy(buf,pp,nTemp);
		pwfsptrform->version.major = atoi(buf);

		pTemp++;
		pTemp2 = strstr(pTemp,",");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		pwfsptrform->version.minor = atoi(buf);

		pTemp+= nTemp+2;
		pTemp2 = strstr(pTemp,",");
		nTemp = pTemp2-pTemp-1;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		memcpy(pwfsptrform->version.date,buf,nTemp);
		
		pTemp+= nTemp+2;

		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		memcpy(pwfsptrform->version.author,buf,nTemp);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}

	pp = strstr(p,"LANGUAGE");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("LANGUAGE");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		pwfsptrform->language = aTodecfromhex(buf,strlen(buf));
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}


	pp = strstr(p,"COPYRIGHT");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("COPYRIGHT");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(pwfsptrform->copyright,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

    pp = strstr(p,"TITLE");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("TITLE");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(pwfsptrform->copyright,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"COMMENT");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("COMMENT");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(pwfsptrform->comment,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"USERPROMPT");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("USERPROMPT");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(pwfsptrform->userprompt,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	memset(pOut,0,nOutBufferLength);
	memcpy(pOut,pwfsptrform,nOutBufferLength);

	delete p;*/
	return 1;
	
}



void GetData_SubFormBuffer(const char *pSource,const int nLength,char *pOut,int &nOutBufferLength)
{
	/*
	char *p;
	int nLen;
	char *pp;
	int  nIndexFrom,nIndexTo;
	char *pTemp;
	char *pTemp2;
	char buf[100];
	int nTemp;

	nLen = nLength;
	WFSPTRFORM_SUBFORM  wfsptrform_subform;
	memset((void*)&wfsptrform_subform,0,sizeof(WFSPTRFORM_SUBFORM));
	p = new char[nLength + 10];
	memset(p,0,nLen+10);
	memcpy(p,pSource,nLen);


	WFSPTRFORM_FIELD *pfield;
	pfield = wfsptrform_subform.fieldname;

	char *pReturn = NULL;
	int  nReturn =0;
	pReturn = new char[nLength+1];
	memset(pReturn,0,nLength+1);
	nReturn = nLength;
	int nNumbers ;
	nNumbers  = 0;
	while(1)
	{
		pTemp = strstr(p,"XFSFIELD");
		if(pTemp ==NULL)
			break;
		nTemp = pTemp - p;
		GetFieldFromform(pTemp,nTemp,pReturn,nReturn,"xfsfield");
		TrimSubString(p,nLen,nTemp,nTemp+nReturn);
		nLen -= nReturn;
		LPWFSPTRFORM_FIELD pwfsfield;
		pwfsfield = (LPWFSPTRFORM_FIELD)new WFSPTRFORM_FIELD;
		memset(pwfsfield,0,sizeof(WFSPTRFORM_FIELD));
		GetData_FieldBuffer(pReturn,nReturn,(char*)pwfsfield,nReturn);
		pfield = pwfsfield;
		pfield = pwfsfield->pNext;
		nNumbers ++;
	}
	wfsptrform_subform.dwField = nNumbers;

	WFSPTRFORM_FRAME *pframe;
	pframe = wfsptrform_subform.framename;
	nNumbers = 0;
	while(1)
	{
		pTemp = strstr(p,"XFSFRAME");
		if(pTemp == NULL)
			break;
		nTemp = pTemp - p ;
		GetFieldFromform(pTemp,nTemp,pReturn,nReturn,"xfsframe");
		TrimSubString(p,nLen,nTemp,nTemp+nReturn);
		nLen -= nReturn;
		WFSPTRFORM_FRAME *pwfsframe;
		pwfsframe = (LPWFSPTRFORM_FRAME) new WFSPTRFORM_FRAME;
		memset(pwfsframe,0,sizeof(WFSPTRFORM_FRAME));
		GetData_FrameBuffer(pReturn,nReturn,(char*)pwfsframe,nReturn);
		pframe = pwfsframe;
		pframe = pwfsframe->pNext;
	}
	wfsptrform_subform.dwFrame = nNumbers;


	pp = strstr(p,"POSITION");//	
	if(pp)
	{
		nIndexFrom = pp-p;
		pp += strlen("POSITION");
		pTemp = strstr(pp,",");
		if(pTemp==NULL) return;
		nTemp = pTemp-pp;

		memset(buf,0,sizeof(buf));

		memcpy(buf,pp,nTemp);
	
		wfsptrform_subform.position.x = atoi(buf);
		
		pTemp++;

		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_subform.position.y = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}

	pp = strstr(p,"SIZE");//	
	if(pp)
	{//"123ualisize5,16\r45678";
		nIndexFrom = pp-p;
		pp += strlen("SIZE");
		pTemp = strstr(pp,",");
		if(pTemp==NULL) return;
		nTemp = pTemp-pp;

		memset(buf,0,sizeof(buf));

		memcpy(buf,pp,nTemp);
	
		wfsptrform_subform.size.width = atoi(buf);
		
		pTemp++;

		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_subform.size.height = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}
	*/
}



int GetData_FieldBuffer(const char *pSource,const int nLength,char *pOut,int &nOutBufferLength)
{
	/*
	char *p = NULL;
	int nLen;
	char *pp;
	int  nIndexFrom,nIndexTo;
	char *pTemp;
	char *pTemp2;
	char buf[100];
	int nTemp;

	

	nLen = nLength;
	WFSPTRFORM_FIELD  wfsptrform_field;
	memset((void*)&wfsptrform_field,0,sizeof(WFSPTRFORM_FIELD));

	p = new char[nLen+10];
	if(p == NULL ){return -1;}
	memset(p,0,nLen + 10);

	memcpy(p,pSource,nLen);

	pp = strstr(p,"POSITION");//	
	if(pp)
	{
		nIndexFrom = pp-p;
		pp += strlen("POSITION");
		pTemp = strstr(pp,",");
		if(pTemp==NULL)
		{
			delete p;
			return -1;
		}
		nTemp = pTemp-pp;

		memset(buf,0,sizeof(buf));

		memcpy(buf,pp,nTemp);
	
		wfsptrform_field.position.x = atoi(buf);
		
		pTemp++;

		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_field.position.y = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}


	pp = strstr(p,"FOLLOWS");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("FOLLOWS");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}
		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.follows,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"HEADER");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("HEADER");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.header,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"FOOTER");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("FOOTER");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.header,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"SIDE");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("SIDE");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.side,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"SIZE");
	if(pp)
    {
		nIndexFrom = pp-p;
		pp += strlen("SIZE");
		pTemp = strstr(pp,",");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_field.size.width = atoi(buf);

		pTemp++;

		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_field.size.height = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}

	pp = strstr(p,"INDEX");
	if(pp)
    {
		nIndexFrom = pp-p;
		pp += strlen("INDEX");
		pTemp = strstr(pp,",");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}
		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_field.index.repeatcount = atoi(buf);

		pTemp++;

		pTemp2 = strstr(pTemp,",");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_field.index.xoffset = atoi(buf);

		pTemp += nTemp+1;
		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_field.index.yoffset = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}


	pp = strstr(p,"TYPE");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("TYPE");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.type,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}


	pp = strstr(p,"SCALING");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("SCALING");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.scaling,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}


	pp = strstr(p,"BARCODE");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("BARCODE");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.barcode,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"CLASS");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("CLASS");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.class_field,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"ACCESS");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("ACCESS");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.access,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"OVERFLOW");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("OVERFLOW");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.overflow,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}


	pp = strstr(p,"STYLE");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("STYLE");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_field.style=atoi(buf);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"CASE");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("CASE");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.case_field,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"HORIZONTAL");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("HORIZONTAL");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.horizontal,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"VERTICAL");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("VERTICAL");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.vertical,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"COLOR");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("COLOR");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.color,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}


	pp = strstr(p,"RGBCOLOR");
	if(pp)
    {
		nIndexFrom = pp-p;
		pp += strlen("RGBCOLOR");
		pTemp = strstr(pp,",");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}
		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_field.rgbcolor.r = atoi(buf);

		pTemp++;

		pTemp2 = strstr(pTemp,",");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_field.rgbcolor.g = atoi(buf);

		pTemp += nTemp+1;
		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_field.rgbcolor.b = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}

	pp = strstr(p,"LANGUAGE");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("LANGUAGE");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_field.language = aTodecfromhex(buf,strlen(buf));
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}


	pp = strstr(p,"FONT");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("FONT");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.font_field,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"POINTSIZE");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("POINTSIZE");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_field.pointsize = atoi(buf);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}


	pp = strstr(p,"CPI");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("CPI");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_field.cpi = atoi(buf);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"LPI");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("LPI");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_field.lpi = atoi(buf);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"FORMAT");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("FORMAT");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.format,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"INITIALAVALUE");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("INITIALAVALUE");
		pTemp = strstr(pp,"\r");
		if(pTemp == NULL)
		{
			delete p;
			return -1;
		}

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_field.initialavalue,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	nOutBufferLength = sizeof(WFSPTRFORM_FIELD);

	//copy the data into the return value;
	memset(pOut,0,nOutBufferLength + 1);
	memcpy(pOut,&wfsptrform_field,nOutBufferLength);

	delete p;
	*/
	return 1;

}



int GetData_FrameBuffer(const char *pSource,const int nLength,char *pOut,int &nOutBufferLength)
{
	/*
	char *p= NULL;
	int nLen;
	char *pp;
	int  nIndexFrom,nIndexTo;
	char *pTemp;
	char *pTemp2;
	char buf[100];
	int nTemp;


	WFSPTRFORM_FRAME wfsptrform_frame;
	memset((void*)&wfsptrform_frame,0,sizeof(WFSPTRFORM_FRAME));

	p = new char[nLength];
	if(p==NULL)
		return -1;

	nLen = nLength;

	pp = strstr(p,"POSITION");//	
	if(pp)
	{
		nIndexFrom = pp-p;
		pp += strlen("POSITION");
		pTemp = strstr(pp,",");
		if(pTemp==NULL) return -1;
		nTemp = pTemp-pp;

		memset(buf,0,sizeof(buf));

		memcpy(buf,pp,nTemp);
	
		wfsptrform_frame.position.x = atoi(buf);
		
		pTemp++;

		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_frame.position.y = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}

	pp = strstr(p,"FRAMES");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("FRAMES");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.frames,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"HEADER");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("HEADER");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.header,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"FOOTER");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("FOOTER");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.header,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"SIDE");
	if(pp)
	{//"123ualiorspositsidefront\r45678";
		nIndexFrom = pp-p;

		pp+=strlen("SIDE");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.side,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"SIZE");
	if(pp)
    {
		nIndexFrom = pp-p;
		pp += strlen("SIZE");
		pTemp = strstr(pp,",");
		if(pTemp==NULL) return -1;
		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_frame.size.width = atoi(buf);

		pTemp++;

		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_frame.size.height = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}

	pp = strstr(p,"REPEATONX");
	if(pp)
    {
		nIndexFrom = pp-p;
		pp += strlen("REPEATONX");
		pTemp = strstr(pp,",");
		if(pTemp==NULL) return -1;
		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_frame.repeatonx.repeatcount = atoi(buf);

		pTemp++;

		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_frame.repeatonx.xoffset = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}

	pp = strstr(p,"REPEATONY");
	if(pp)
    {
		nIndexFrom = pp-p;
		pp += strlen("REPEATONY");
		pTemp = strstr(pp,",");
		if(pTemp==NULL) return -1;
		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_frame.repeatonx.repeatcount = atoi(buf);

		pTemp++;

		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_frame.repeatony.yoffset = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}

	pp = strstr(p,"TYPE");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("TYPE");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.type,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"CLASS");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("CLASS");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.class_frame,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"OVERFLOW");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("OVERFLOW");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.overflow,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"STYLE");
	if(pp)
	{//"123ualiorsposittyscalihngmastyle3424\r45678";
		nIndexFrom = pp-p;

		pp+=strlen("STYLE");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_frame.style=atoi(buf);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"COLOR");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("COLOR");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.color,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"RGBCOLOR");
	if(pp)
    {
		nIndexFrom = pp-p;
		pp += strlen("RGBCOLOR");
		pTemp = strstr(pp,",");
		if(pTemp==NULL) return -1;
		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_frame.rgbcolor.r = atoi(buf);

		pTemp++;

		pTemp2 = strstr(pTemp,",");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_frame.rgbcolor.g = atoi(buf);

		pTemp += nTemp+1;
		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_frame.rgbcolor.b = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}

	pp = strstr(p,"FILLCOLOR");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("FILLCOLOR");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.fillcolor,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"RGBFILLCOLOR");
	if(pp)
    {
		nIndexFrom = pp-p;
		pp += strlen("RGBFILLCOLOR");
		pTemp = strstr(pp,",");
		if(pTemp==NULL) return -1;
		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_frame.rgbcolor.r = atoi(buf);

		pTemp++;

		pTemp2 = strstr(pTemp,",");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_frame.rgbcolor.g = atoi(buf);

		pTemp += nTemp+1;
		pTemp2 = strstr(pTemp,"\r");
		nTemp = pTemp2-pTemp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pTemp,nTemp);
		wfsptrform_frame.rgbcolor.b = atoi(buf);

		pTemp2 += 1;
		nIndexTo = pTemp2 - p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);		
	}

	pp = strstr(p,"FILLSTYLE");
	if(pp)
	{//"123ualiorsposittyscalihngmastyle3424\r45678";
		nIndexFrom = pp-p;

		pp+=strlen("FILLSTYLE");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		wfsptrform_frame.fillstyle=atoi(buf);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"SUBSTSIGN");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("SUBSTSIGN");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.substsign,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);
	}

	pp = strstr(p,"TITLE");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("TITLE");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.title,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}


	pp = strstr(p,"HORIZONTAL");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("HORIZONTAL");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.horizontal,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}

	pp = strstr(p,"VERTICAL");
	if(pp)
	{
		nIndexFrom = pp-p;

		pp+=strlen("VERTICAL");
		pTemp = strstr(pp,"\r");

		nTemp = pTemp-pp;
		memset(buf,0,sizeof(buf));
		memcpy(buf,pp,nTemp);
		memcpy(wfsptrform_frame.vertical,buf,nTemp);
		pTemp +=1;
		nIndexTo = pTemp -p;
		TrimSubString(p,nLen,nIndexFrom,nIndexTo);

	}
	nOutBufferLength = sizeof(WFSPTRFORM_FRAME);
	memset(pOut,0,nOutBufferLength);
	             
	memcpy(pOut,&wfsptrform_frame,sizeof(WFSPTRFORM_FRAME));
	*/
	return 1;

}

float FromInchToMilliMeter(float fInch)
{
	return (float)(fInch * 25.4);
}

float FromMilliMeterToInch(float MilliMeter)
{
	return (float)(MilliMeter /25.4);
}

