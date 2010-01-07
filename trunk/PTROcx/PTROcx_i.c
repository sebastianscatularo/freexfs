/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Dec 18 23:52:07 2009
 */
/* Compiler settings for D:\OpenXFSV0.0.4\PTROcx\PTROcx.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IXFSPrint = {0xB7F5141D,0x9002,0x4128,{0xAB,0x29,0x54,0xA4,0x43,0xEF,0x5F,0xC6}};


const IID LIBID_PTROCXLib = {0x021C4385,0x4225,0x44D2,{0x83,0x89,0xD3,0xB5,0xDB,0x67,0x2F,0xBF}};


const IID DIID__IXFSPrintEvents = {0x5BD05B62,0xDED9,0x49A6,{0x82,0x7C,0x63,0x80,0x68,0x6C,0x8E,0x69}};


const CLSID CLSID_XFSPrint = {0x153194CA,0x33B1,0x408B,{0x98,0xBF,0x26,0xF9,0x07,0xDA,0x4C,0x91}};


#ifdef __cplusplus
}
#endif

