# Microsoft Developer Studio Project File - Name="GuiLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=GuiLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GuiLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GuiLib.mak" CFG="GuiLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GuiLib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GuiLib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GuiLib - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\Header" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /def:".\GuiLib.def" /out:"../../StkUI/Release/GuiLib.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GuiLib - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\Header" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "EXTLIB_DLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /profile /debug /machine:I386 /def:".\GuiLib.def" /out:"../../StkUI/Debug/GuiLib.dll"

!ENDIF 

# Begin Target

# Name "GuiLib - Win32 Release"
# Name "GuiLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CoolMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiADODB.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiButton.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiCapOutBar.cpp
# End Source File
# Begin Source File

SOURCE=.\GuicheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiComboBoxExt.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiComboFont.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiDocBarExten.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiDockContext.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiDockToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiDocSpecial.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiDrawLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiDropDownEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiFDir.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiFile.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiImageLinkButton.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiLabelButton.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiLib.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiLib.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\GuiLib.rc
# End Source File
# Begin Source File

SOURCE=.\GuiLinkButton.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiListEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiMDIFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiMDITabbed.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiMiniSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiMiniTool.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiNormalButton.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiOfficeBar.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiOutLook.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiOutLookView.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiPanelAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiPowerPointView.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiSliderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiStaticStandar.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiSysTray.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiTabbed.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiTabPowerPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiTabWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiToolBarWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiToolButton.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiVisioFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiWorkPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiWorkTab.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\NewMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Subclass.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Header\CBaseTab.h
# End Source File
# Begin Source File

SOURCE=..\Header\CoolMenu.h
# End Source File
# Begin Source File

SOURCE=..\Header\ExtLib.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiADODB.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiBaseTab.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiButton.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiCapOutBar.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuicheckBox.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiColors.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiComboBoxExt.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiComboFont.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiContainer.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiControlBar.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiDocBarExten.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiDockContext.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiDockToolBar.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiDocSpecial.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiDrawLayer.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiDropDownEdit.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiEdit.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiFDir.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiFile.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiFolder.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiFrameWnd.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiGroupBox.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiImageLinkButton.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiLabelButton.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiLib.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiLinkButton.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiListEdit.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiMDIFrame.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiMDITabbed.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiMiniSplitter.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiMiniTool.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiNormalButton.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiOfficeBar.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiOutLook.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiOutLookView.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiPanelAccess.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiPowerPointView.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiRadioButton.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiSliderCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiSplitter.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiStaticStandar.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiStatusBar.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiSysTray.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiTabbed.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiTabPowerPoint.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiTabWnd.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiToolBarWnd.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiToolButton.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiVisioFolder.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiWorkPanel.h
# End Source File
# Begin Source File

SOURCE=..\Header\GuiWorkTab.h
# End Source File
# Begin Source File

SOURCE=..\Header\MenuBar.h
# End Source File
# Begin Source File

SOURCE=..\Header\NewMenu.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Header\Subclass.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\bitradio.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\DockOffice.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GuiLib.rc2
# End Source File
# Begin Source File

SOURCE=.\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\idc_hand.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
