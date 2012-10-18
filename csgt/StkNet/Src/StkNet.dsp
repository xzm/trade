# Microsoft Developer Studio Project File - Name="StkNet" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=StkNet - Win32 Enu Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "StkNet.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StkNet.mak" CFG="StkNet - Win32 Enu Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StkNet - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "StkNet - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "StkNet - Win32 Enu Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "StkNet - Win32 Enu Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "StkNet - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Lib"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXEXT" /D "STKNET_DLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_CHS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../StkUI/Release/StkNet30.dll" /libpath:"../../StkLib/Lib" /libpath:"../../GuiLib/Lib"

!ELSEIF  "$(CFG)" == "StkNet - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "STKNET_DLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_CHS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"../../StkUI/Debug/StkNet30d.dll" /libpath:"../../StkLib/Lib" /libpath:"../../GuiLib/Lib"

!ELSEIF  "$(CFG)" == "StkNet - Win32 Enu Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "StkNet___Win32_Enu_Debug"
# PROP BASE Intermediate_Dir "StkNet___Win32_Enu_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Lib"
# PROP Intermediate_Dir "StkNet___Win32_Enu_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "STKNET_DLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "STKNET_DLL" /D "CLKLAN_ENGLISH_US" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"../../StkUI/Debug/StkNet30d.dll" /libpath:"../../StkLib/Lib"
# ADD LINK32 /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"../../StkUI/Debug/StkNet30d.dll" /libpath:"../../StkLib/Lib" /libpath:"../../GuiLib/Lib"

!ELSEIF  "$(CFG)" == "StkNet - Win32 Enu Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "StkNet___Win32_Enu_Release"
# PROP BASE Intermediate_Dir "StkNet___Win32_Enu_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Lib"
# PROP Intermediate_Dir "StkNet___Win32_Enu_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXEXT" /D "STKNET_DLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXEXT" /D "STKNET_DLL" /D "_WINDLL" /D "_AFXDLL" /D "CLKLAN_ENGLISH_US" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../StkUI/Release/StkNet30.dll" /libpath:"../../StkLib/Lib"
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../StkUI/Release/StkNet30.dll" /libpath:"../../StkLib/Lib" /libpath:"../../GuiLib/Lib"

!ENDIF 

# Begin Target

# Name "StkNet - Win32 Release"
# Name "StkNet - Win32 Debug"
# Name "StkNet - Win32 Enu Debug"
# Name "StkNet - Win32 Enu Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AfxCore.cpp
# End Source File
# Begin Source File

SOURCE=.\base64.cpp
# End Source File
# Begin Source File

SOURCE=.\EditSrvDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HttpClt.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoZip.cpp
# End Source File
# Begin Source File

SOURCE=.\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\NetDB.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxySetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxySocket.cpp
# End Source File
# Begin Source File

SOURCE=.\QSProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\Secret.cpp
# End Source File
# Begin Source File

SOURCE=.\StartupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StkNet.cpp
# End Source File
# Begin Source File

SOURCE=.\StkNet.def
# End Source File
# Begin Source File

SOURCE=.\StkNet.rc
# End Source File
# Begin Source File

SOURCE=.\Strings.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLDoc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Include\AfxCore.h
# End Source File
# Begin Source File

SOURCE=..\Include\base64.h
# End Source File
# Begin Source File

SOURCE=.\EditSrvDlg.h
# End Source File
# Begin Source File

SOURCE=..\Include\HttpClt.h
# End Source File
# Begin Source File

SOURCE=..\Include\InfoZip.h
# End Source File
# Begin Source File

SOURCE=..\Include\Markup.h
# End Source File
# Begin Source File

SOURCE=..\Include\NetDB.h
# End Source File
# Begin Source File

SOURCE=.\ProxySetDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProxySocket.h
# End Source File
# Begin Source File

SOURCE=..\Include\QSProfile.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\Include\Secret.h
# End Source File
# Begin Source File

SOURCE=..\Include\Secret.inl
# End Source File
# Begin Source File

SOURCE=..\Include\StartupDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Include\StkNet.h
# End Source File
# Begin Source File

SOURCE=.\Strings.h
# End Source File
# Begin Source File

SOURCE=..\Include\UnzipDLL.h
# End Source File
# Begin Source File

SOURCE=..\Include\XMLDoc.h
# End Source File
# Begin Source File

SOURCE=..\Include\ZCallBck.h
# End Source File
# Begin Source File

SOURCE=..\Include\ZipDLL.h
# End Source File
# End Group
# Begin Group "Yongxin Files"

# PROP Default_Filter "h;c;cpp"
# Begin Source File

SOURCE=.\cppstkfilter.h
# End Source File
# Begin Source File

SOURCE=.\stkfilter.h
# End Source File
# Begin Source File

SOURCE=.\stkfmt.h
# End Source File
# Begin Source File

SOURCE=.\Yongxin.cpp
# End Source File
# Begin Source File

SOURCE=.\Yongxin.h
# End Source File
# End Group
# Begin Group "Receiver Files"

# PROP Default_Filter "h;c;cpp"
# Begin Source File

SOURCE=.\SelectEngineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectEngineDlg.h
# End Source File
# Begin Source File

SOURCE=.\StkReceiver.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\StkReceiver.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\dlgleftl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\leftlogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StkNet.rc2
# End Source File
# End Group
# Begin Group "SMailer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MUtils\Base64Helper.cpp
# End Source File
# Begin Source File

SOURCE=.\MUtils\Base64Helper.h
# End Source File
# Begin Source File

SOURCE=.\MUtils\FileHelper.h
# End Source File
# Begin Source File

SOURCE=.\SMailer\SMailer.cpp
# End Source File
# Begin Source File

SOURCE=.\SMailer\SMailer.h
# End Source File
# Begin Source File

SOURCE=.\MUtils\WinSockHelper.h
# End Source File
# End Group
# Begin Group "Tongshi Files"

# PROP Default_Filter "h;c;cpp"
# Begin Source File

SOURCE=.\STKDRV.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\STKDRV.h
# End Source File
# Begin Source File

SOURCE=.\STKDRVContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\STKDRVContainer.h
# End Source File
# Begin Source File

SOURCE=.\Stockdrv.h
# End Source File
# Begin Source File

SOURCE=.\Tongshi.cpp
# End Source File
# Begin Source File

SOURCE=.\Tongshi.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
