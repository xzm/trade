# Microsoft Developer Studio Project File - Name="XPFace" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XPFace - Win32 Enu Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XPFace.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XPFace.mak" CFG="XPFace - Win32 Enu Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XPFace - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XPFace - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XPFace - Win32 Enu Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XPFace - Win32 Enu Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XPFace - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../Include" /I "../Include/GridCtrl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_DLL" /D "_MBCS" /D "XPFACE_DLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_CHS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ./htmlhelp/htmlhelp.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../StkUI/Release/XPFace.dll" /libpath:"../../GuiLib/Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XPFace - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "../Include" /I "../Include/GridCtrl" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "XPFACE_DLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_CHS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ./htmlhelp/htmlhelp.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"../../StkUI/Debug/XPFaced.dll" /libpath:"../../GuiLib/Lib"

!ELSEIF  "$(CFG)" == "XPFace - Win32 Enu Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "XPFace___Win32_Enu_Debug"
# PROP BASE Intermediate_Dir "XPFace___Win32_Enu_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Lib"
# PROP Intermediate_Dir "XPFace___Win32_Enu_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /ZI /Od /I "../Include" /I "../Include/GridCtrl" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "XPFACE_DLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "../Include" /I "../Include/GridCtrl" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "XPFACE_DLL" /D "CLKLAN_ENGLISH_US" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./htmlhelp/htmlhelp.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"../../StkUI/Debug/XPFaced.dll" /libpath:"../../GuiLib/Lib"
# ADD LINK32 ./htmlhelp/htmlhelp.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"../../StkUI/Debug/XPFaced.dll" /libpath:"../../GuiLib/Lib"

!ELSEIF  "$(CFG)" == "XPFace - Win32 Enu Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XPFace___Win32_Enu_Release"
# PROP BASE Intermediate_Dir "XPFace___Win32_Enu_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Lib"
# PROP Intermediate_Dir "XPFace___Win32_Enu_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../Include" /I "../Include/GridCtrl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_DLL" /D "_MBCS" /D "XPFACE_DLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../Include" /I "../Include/GridCtrl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_DLL" /D "_MBCS" /D "XPFACE_DLL" /D "_WINDLL" /D "_AFXDLL" /D "CLKLAN_ENGLISH_US" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./htmlhelp/htmlhelp.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../StkUI/Release/XPFace.dll" /libpath:"../../GuiLib/Lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./htmlhelp/htmlhelp.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../StkUI/Release/XPFace.dll" /libpath:"../../GuiLib/Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XPFace - Win32 Release"
# Name "XPFace - Win32 Debug"
# Name "XPFace - Win32 Enu Debug"
# Name "XPFace - Win32 Enu Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BaseTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\CJButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CJControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CJTabCtrlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DeclareDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\Dibapi.cpp
# End Source File
# Begin Source File

SOURCE=.\Draw.cpp
# End Source File
# Begin Source File

SOURCE=.\EnTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FlatListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuXP.cpp
# End Source File
# Begin Source File

SOURCE=.\ModulVer.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ThemeHelperST.cpp
# End Source File
# Begin Source File

SOURCE=.\TipDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkspBar.cpp
# End Source File
# Begin Source File

SOURCE=.\XFDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\XPFace.cpp
# End Source File
# Begin Source File

SOURCE=.\XPFace.def
# End Source File
# Begin Source File

SOURCE=.\XPFace.rc
# End Source File
# Begin Source File

SOURCE=.\XPStyleButtonST.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Include\BaseTabCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\BtnST.h
# End Source File
# Begin Source File

SOURCE=..\Include\CJButton.h
# End Source File
# Begin Source File

SOURCE=..\Include\CJControlBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\CJTabCtrlBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\DeclareDlg.h
# End Source File
# Begin Source File

SOURCE=..\Include\Dialogs.h
# End Source File
# Begin Source File

SOURCE=..\Include\Dibapi.h
# End Source File
# Begin Source File

SOURCE=..\Include\Draw.h
# End Source File
# Begin Source File

SOURCE=..\Include\EnTabCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\FlatListBox.h
# End Source File
# Begin Source File

SOURCE=..\Include\HyperLink.h
# End Source File
# Begin Source File

SOURCE=..\Include\LocalProperty.h
# End Source File
# Begin Source File

SOURCE=..\Include\MagicEdit.h
# End Source File
# Begin Source File

SOURCE=..\Include\MenuButton.h
# End Source File
# Begin Source File

SOURCE=..\Include\MenuXP.h
# End Source File
# Begin Source File

SOURCE=..\Include\ModulVer.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=..\Include\SplashDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Include\TextToolBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\ThemeHelperST.h
# End Source File
# Begin Source File

SOURCE=..\Include\TipDialog.h
# End Source File
# Begin Source File

SOURCE=..\Include\Tools.h
# End Source File
# Begin Source File

SOURCE=..\Include\WorkspBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\XFDialog.h
# End Source File
# Begin Source File

SOURCE=..\Include\XPFace.h
# End Source File
# Begin Source File

SOURCE=..\Include\XPStyleButtonST.h
# End Source File
# End Group
# Begin Group "GridCtrl Files"

# PROP Default_Filter "h;cpp"
# Begin Source File

SOURCE=..\Include\GridCtrl\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\GridCtrl\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\GridCtrl\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\GridCtrl\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\InPlaceList.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\GridCtrl\InPlaceList.h
# End Source File
# Begin Source File

SOURCE=..\Include\GridCtrl\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\GridCtrl\TitleTip.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\leftlogo.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
