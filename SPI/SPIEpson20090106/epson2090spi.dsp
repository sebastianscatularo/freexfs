# Microsoft Developer Studio Project File - Name="epson2090spi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=epson2090spi - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "epson2090spi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "epson2090spi.mak" CFG="epson2090spi - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "epson2090spi - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "epson2090spi - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "epson2090spi - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ..\..\lib\NI_XFSMgr.lib ..\..\lib\xfs_conf.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\dll\SPI_LQ2090.dll"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy ..\..\dll\SPI_LQ2090.dll c:\SPI_LQ2090.dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "epson2090spi - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\..\lib\xfs_conf.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\dllDebug\SPI_LQ2090.dll" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy ..\..\dllDebug\SPI_LQ2090.dll c:\SPI_LQ2090.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "epson2090spi - Win32 Release"
# Name "epson2090spi - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Createor.cpp
# End Source File
# Begin Source File

SOURCE=.\epson2090spi.cpp
# End Source File
# Begin Source File

SOURCE=.\epson2090spi.def
# End Source File
# Begin Source File

SOURCE=.\epson2090spi.rc
# End Source File
# Begin Source File

SOURCE=.\myfile\printerbase.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceBasic.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WorkThread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\XFSField.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\XFSForm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\XFSFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\myfile\xfshelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\XFSMedia.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\XFSSubform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\myfile\xfsToolsFunction.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Createor.h
# End Source File
# Begin Source File

SOURCE=.\epson2090spi.h
# End Source File
# Begin Source File

SOURCE=.\fundef.h
# End Source File
# Begin Source File

SOURCE=.\helper.h
# End Source File
# Begin Source File

SOURCE=.\myfile\printerbase.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ServiceBasic.h
# End Source File
# Begin Source File

SOURCE=..\..\common\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WorkThread.h
# End Source File
# Begin Source File

SOURCE=..\..\common\XFSField.h
# End Source File
# Begin Source File

SOURCE=..\..\common\XFSForm.h
# End Source File
# Begin Source File

SOURCE=..\..\common\XFSFrame.h
# End Source File
# Begin Source File

SOURCE=..\..\common\myfile\xfshelper.h
# End Source File
# Begin Source File

SOURCE=..\..\common\XFSMedia.h
# End Source File
# Begin Source File

SOURCE=..\..\common\myfile\xfsptrdata.h
# End Source File
# Begin Source File

SOURCE=..\..\common\XFSSPI.H
# End Source File
# Begin Source File

SOURCE=.\XFSSPI.H
# End Source File
# Begin Source File

SOURCE=..\..\common\XFSSubform.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\epson2090spi.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\Notes.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\forms\withdrawal
# End Source File
# End Target
# End Project
