# Microsoft Developer Studio Project File - Name="StkLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=StkLib - Win32 Enu Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "StkLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StkLib.mak" CFG="StkLib - Win32 Enu Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StkLib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "StkLib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "StkLib - Win32 Enu Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "StkLib - Win32 Enu Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "StkLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Lib"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STKLIB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "STKLIB_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_CHS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../StkUI/Release/StkLib30.dll"

!ELSEIF  "$(CFG)" == "StkLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STKLIB_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "STKLIB_DLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_CHS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /debug /machine:I386 /out:"../../StkUI/Debug/StkLib30d.dll"

!ELSEIF  "$(CFG)" == "StkLib - Win32 Enu Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "StkLib___Win32_Enu_Debug"
# PROP BASE Intermediate_Dir "StkLib___Win32_Enu_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Lib"
# PROP Intermediate_Dir "StkLib___Win32_Enu_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "STKLIB_DLL" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "STKLIB_DLL" /D "CLKLAN_ENGLISH_US" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /debug /machine:I386 /out:"../../StkUI/Debug/StkLib30d.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /debug /machine:I386 /out:"../../StkUI/Debug/StkLib30d.dll"

!ELSEIF  "$(CFG)" == "StkLib - Win32 Enu Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "StkLib___Win32_Enu_Release"
# PROP BASE Intermediate_Dir "StkLib___Win32_Enu_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Lib"
# PROP Intermediate_Dir "StkLib___Win32_Enu_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "STKLIB_DLL" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "STKLIB_DLL" /D "CLKLAN_ENGLISH_US" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../StkUI/Release/StkLib30.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../StkUI/Release/StkLib30.dll"

!ENDIF 

# Begin Target

# Name "StkLib - Win32 Release"
# Name "StkLib - Win32 Debug"
# Name "StkLib - Win32 Enu Debug"
# Name "StkLib - Win32 Enu Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AfxCore.cpp
# End Source File
# Begin Source File

SOURCE=.\Alarm.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseData.cpp
# End Source File
# Begin Source File

SOURCE=.\Boyermor.cpp
# End Source File
# Begin Source File

SOURCE=.\CommFuns.cpp
# End Source File
# Begin Source File

SOURCE=.\Container.cpp
# End Source File
# Begin Source File

SOURCE=.\Database.cpp
# End Source File
# Begin Source File

SOURCE=.\DayTrader.cpp
# End Source File
# Begin Source File

SOURCE=.\DRData.cpp
# End Source File
# Begin Source File

SOURCE=.\Express.cpp
# End Source File
# Begin Source File

SOURCE=.\IMCode.cpp
# End Source File
# Begin Source File

SOURCE=.\KData.cpp
# End Source File
# Begin Source File

SOURCE=.\MathParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Minute.cpp
# End Source File
# Begin Source File

SOURCE=.\Outline.cpp
# End Source File
# Begin Source File

SOURCE=.\Packets.cpp
# End Source File
# Begin Source File

SOURCE=.\Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\QianLong.cpp
# End Source File
# Begin Source File

SOURCE=.\Report.cpp
# End Source File
# Begin Source File

SOURCE=.\SelfDB.cpp
# End Source File
# Begin Source File

SOURCE=.\ShengLong.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StkLib.cpp
# End Source File
# Begin Source File

SOURCE=.\StkLib.def
# End Source File
# Begin Source File

SOURCE=.\StkLib.rc
# End Source File
# Begin Source File

SOURCE=.\Stock.cpp
# End Source File
# Begin Source File

SOURCE=.\Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Strings.cpp
# End Source File
# Begin Source File

SOURCE=.\StStore.cpp
# End Source File
# Begin Source File

SOURCE=.\TechCLK.cpp
# End Source File
# Begin Source File

SOURCE=.\TechEnergy.cpp
# End Source File
# Begin Source File

SOURCE=.\TechKLine.cpp
# End Source File
# Begin Source File

SOURCE=.\Technique.cpp
# End Source File
# Begin Source File

SOURCE=.\TechOther.cpp
# End Source File
# Begin Source File

SOURCE=.\TechSwing.cpp
# End Source File
# Begin Source File

SOURCE=.\TechTrend.cpp
# End Source File
# Begin Source File

SOURCE=.\TechUser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Include\AfxCore.h
# End Source File
# Begin Source File

SOURCE=..\Include\Alarm.h
# End Source File
# Begin Source File

SOURCE=..\Include\boyermor.h
# End Source File
# Begin Source File

SOURCE=..\Include\CommFuns.h
# End Source File
# Begin Source File

SOURCE=..\Include\Container.h
# End Source File
# Begin Source File

SOURCE=..\Include\Database.h
# End Source File
# Begin Source File

SOURCE=..\Include\DayTrader.h
# End Source File
# Begin Source File

SOURCE=..\Include\Express.h
# End Source File
# Begin Source File

SOURCE=..\Include\MathParser.h
# End Source File
# Begin Source File

SOURCE=..\Include\Packets.h
# End Source File
# Begin Source File

SOURCE=..\Include\Profile.h
# End Source File
# Begin Source File

SOURCE=.\QianLong.h
# End Source File
# Begin Source File

SOURCE=.\SelfDB.h
# End Source File
# Begin Source File

SOURCE=.\ShengLong.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Include\StkLib.h
# End Source File
# Begin Source File

SOURCE=..\Include\Stock.h
# End Source File
# Begin Source File

SOURCE=..\Include\Stock.inl
# End Source File
# Begin Source File

SOURCE=..\Include\Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Strings.h
# End Source File
# Begin Source File

SOURCE=..\Include\Technique.h
# End Source File
# Begin Source File

SOURCE=..\Include\TechUser.h
# End Source File
# End Group
# Begin Group "SPBaseClass Files"

# PROP Default_Filter "h;cpp"
# Begin Source File

SOURCE=.\SpArchive.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\SpArchive.h
# End Source File
# Begin Source File

SOURCE=..\Include\SpArchive.inl
# End Source File
# Begin Source File

SOURCE=.\SpAssert.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\SpAssert.h
# End Source File
# Begin Source File

SOURCE=.\SpColl.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\SpColl.h
# End Source File
# Begin Source File

SOURCE=..\Include\SpColl.inl
# End Source File
# Begin Source File

SOURCE=.\SpDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\SpDebug.h
# End Source File
# Begin Source File

SOURCE=..\Include\SpDefs.h
# End Source File
# Begin Source File

SOURCE=.\SpFile.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\SpFile.h
# End Source File
# Begin Source File

SOURCE=.\SpLock.h
# End Source File
# Begin Source File

SOURCE=.\SpMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\SpMemory.h
# End Source File
# Begin Source File

SOURCE=.\SpObject.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\SpObject.h
# End Source File
# Begin Source File

SOURCE=..\Include\SpObject.inl
# End Source File
# Begin Source File

SOURCE=..\Include\SpObjErr.inl
# End Source File
# Begin Source File

SOURCE=..\Include\SpPlat.h
# End Source File
# Begin Source File

SOURCE=.\SpString.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\SpString.h
# End Source File
# Begin Source File

SOURCE=..\Include\SpString.inl
# End Source File
# Begin Source File

SOURCE=.\SpTime.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\SpTime.h
# End Source File
# Begin Source File

SOURCE=.\SpValid.cpp
# End Source File
# End Group
# End Target
# End Project
