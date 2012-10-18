# Microsoft Developer Studio Project File - Name="StkUI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=StkUI - Win32 Enu Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "StkUI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StkUI.mak" CFG="StkUI - Win32 Enu Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StkUI - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "StkUI - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "StkUI - Win32 Enu Release" (based on "Win32 (x86) Application")
!MESSAGE "StkUI - Win32 Enu Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "StkUI - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\ProfUIS\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_CHS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 htmlhelp/htmlhelp.lib /nologo /subsystem:windows /machine:I386 /out:"Release/StockAna.exe" /libpath:"../StkLib/Lib" /libpath:"../StkNet/Lib" /libpath:"../ProfUIS/Bin" /libpath:"../GuiLib/Lib" /libpath:"../XPFace/Lib"

!ELSEIF  "$(CFG)" == "StkUI - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_CHS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 htmlhelp/htmlhelp.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"Debug/StockAna.exe" /libpath:"../StkLib/Lib" /libpath:"../StkNet/Lib" /libpath:"../ProfUIS/Bin" /libpath:"../GuiLib/Lib" /libpath:"../XPFace/Lib"

!ELSEIF  "$(CFG)" == "StkUI - Win32 Enu Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "StkUI___Win32_Enu_Release"
# PROP BASE Intermediate_Dir "StkUI___Win32_Enu_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "StkUI___Win32_Enu_Release"
# PROP Intermediate_Dir "StkUI___Win32_Enu_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\ProfUIS\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\ProfUIS\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "CLKLAN_ENGLISH_US" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 htmlhelp/htmlhelp.lib /nologo /subsystem:windows /machine:I386 /out:"Release/StockAna.exe" /libpath:"../StkLib/Lib" /libpath:"../StkNet/Lib" /libpath:"../ProfUIS/Bin" /libpath:"../GuiLib/Lib" /libpath:"../XPFace/Lib"
# ADD LINK32 htmlhelp/htmlhelp.lib /nologo /subsystem:windows /machine:I386 /out:"Release/StockAna.exe" /libpath:"../StkLib/Lib" /libpath:"../StkNet/Lib" /libpath:"../ProfUIS/Bin" /libpath:"../GuiLib/Lib" /libpath:"../XPFace/Lib"

!ELSEIF  "$(CFG)" == "StkUI - Win32 Enu Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "StkUI___Win32_Enu_Debug"
# PROP BASE Intermediate_Dir "StkUI___Win32_Enu_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "StkUI___Win32_Enu_Debug"
# PROP Intermediate_Dir "StkUI___Win32_Enu_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "CLKLAN_ENGLISH_US" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL" /d "AFX_RESOURCE_DLL" /d "AFX_TARG_ENU"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 htmlhelp/htmlhelp.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"Debug/StockAna.exe" /libpath:"../StkLib/Lib" /libpath:"../StkNet/Lib" /libpath:"../ProfUIS/Bin" /libpath:"../GuiLib/Lib" /libpath:"../XPFace/Lib"
# ADD LINK32 htmlhelp/htmlhelp.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"Debug/StockAna.exe" /libpath:"../StkLib/Lib" /libpath:"../StkNet/Lib" /libpath:"../ProfUIS/Bin" /libpath:"../GuiLib/Lib" /libpath:"../XPFace/Lib"

!ENDIF 

# Begin Target

# Name "StkUI - Win32 Release"
# Name "StkUI - Win32 Debug"
# Name "StkUI - Win32 Enu Release"
# Name "StkUI - Win32 Enu Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AfxCore.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\SListCombos.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StkUI.cpp
# End Source File
# Begin Source File

SOURCE=.\StkUI.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AfxCore.h
# End Source File
# Begin Source File

SOURCE=.\AfxCore.inl
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SListCombos.h
# End Source File
# Begin Source File

SOURCE=.\StaticDoc.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StkUI.h
# End Source File
# End Group
# Begin Group "ParamDlg Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ParamDlg\36BIASDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\36BIASDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\ADDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\ADDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\ARBRDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\ARBRDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\ASIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\ASIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\ATRDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\ATRDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\BBIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\BBIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\BIASDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\BIASDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\BOLLDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\BOLLDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CCIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CCIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CDPDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CDPDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CRDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CRDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CVDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CVDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CWDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CWDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CYODlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\CYODlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\DCYODlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\DCYODlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\DJDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\DJDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\DMIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\DMIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\DMKIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\DMKIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\DPERDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\DPERDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\DPODlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\DPODlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\EMVDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\EMVDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\HLCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\HLCDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\HSLDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\HSLDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\KDJDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\KDJDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\KSTDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\KSTDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MACDDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MACDDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MADlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MADlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MAOSCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MAOSCDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MFIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MFIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MIKEDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MIKEDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MOBVDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MOBVDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MTMDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\MTMDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\NVIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\NVIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\NVRSIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\NVRSIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\OBVDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\OBVDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\OSCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\OSCDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\PCNTDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\PCNTDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\PSYDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\PSYDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\PVDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\PVDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\PVIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\PVIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\RDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\RDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\REIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\REIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\ROCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\ROCDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\RSIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\RSIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\SARDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\SARDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\SetParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\SetParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\UOSDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\UOSDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\VMACDDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\VMACDDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\VolumeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\VolumeDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\VRDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\VRDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\VROCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\VROCDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\VRSIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\VRSIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\WVADDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg\WVADDlg.h
# End Source File
# End Group
# Begin Group "View Files"

# PROP Default_Filter "h;c;cpp"
# Begin Source File

SOURCE=.\View\BaseView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\BaseView.h
# End Source File
# Begin Source File

SOURCE=.\View\DrawTech.cpp
# End Source File
# Begin Source File

SOURCE=.\View\GraphView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\GraphView.h
# End Source File
# Begin Source File

SOURCE=.\View\GroupView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\GroupView.h
# End Source File
# Begin Source File

SOURCE=.\View\InfoView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\InfoView.h
# End Source File
# Begin Source File

SOURCE=.\View\MultiSort.cpp
# End Source File
# Begin Source File

SOURCE=.\View\MultiSort.h
# End Source File
# Begin Source File

SOURCE=.\View\MultiSortView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\MultiSortView.h
# End Source File
# Begin Source File

SOURCE=.\View\RealTime.cpp
# End Source File
# Begin Source File

SOURCE=.\View\RealTime.h
# End Source File
# Begin Source File

SOURCE=.\View\RealTimeView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\RealTimeView.h
# End Source File
# Begin Source File

SOURCE=.\View\SelectorView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\SelectorView.h
# End Source File
# Begin Source File

SOURCE=.\View\Simulation.cpp
# End Source File
# Begin Source File

SOURCE=.\View\Simulation.h
# End Source File
# Begin Source File

SOURCE=.\View\SimuView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\SimuView.h
# End Source File
# Begin Source File

SOURCE=.\View\SListView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\SListView.h
# End Source File
# Begin Source File

SOURCE=.\View\StockGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\View\StockGraph.h
# End Source File
# Begin Source File

SOURCE=.\View\StrategyView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\StrategyView.h
# End Source File
# Begin Source File

SOURCE=.\View\TechsView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\TechsView.h
# End Source File
# Begin Source File

SOURCE=.\View\WizardView.cpp
# End Source File
# Begin Source File

SOURCE=.\View\WizardView.h
# End Source File
# End Group
# Begin Group "Dialog Files"

# PROP Default_Filter "h;c;cpp"
# Begin Source File

SOURCE=.\Dialog\AlarmResultDlg.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialog\AlarmResultDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\AlarmSettingDlg.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialog\AlarmSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\AlarmTipDlg.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialog\AlarmTipDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\BaseIndexDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\BaseIndexDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\CacheLoadDlg.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialog\CacheLoadDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\CloseStrategyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\CloseStrategyDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\DataSrcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\DataSrcDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\DownloadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\downloaddlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\EditKDATADlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\EditKDATADlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\ExportDest.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\ExportDest.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\ExportOption.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\ExportOption.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\FilterInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\FilterInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\InstallPacDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\InstallPacDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\ListExportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\ListExportDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\NetInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\NetInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\QuoteTipDlg.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialog\QuoteTipDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\RegApplyEvaDlg.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialog\RegApplyEvaDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\RegisterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\RegisterDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SelectGrpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SelectGrpDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SelectStk.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SelectStk.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SelectTraderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SelectTraderDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetBasedataDlg.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetBasedataDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetBaseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetBaseDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetColorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetColorDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetColumnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetColumnDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetDrdataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetDrdataDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetKDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetKDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetPrpt.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetPrpt.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetRate.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetRate.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetRule.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SetRule.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SimuAddOpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SimuAddOpDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SimuAddStockDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SimuAddStockDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SimuAdvancedDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SimuAdvancedDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SimuRealOp.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SimuRealOp.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SimuReport.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SimuReport.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SimuStatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SimuStatDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\TyDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\TyDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\UpgradeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\UpgradeDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\alarm_in.ico
# End Source File
# Begin Source File

SOURCE=.\res\alarm_wa.ico
# End Source File
# Begin Source File

SOURCE=.\res\balang.bmp
# End Source File
# Begin Source File

SOURCE=.\res\balang.ico
# End Source File
# Begin Source File

SOURCE=.\res\bin00001.bin
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00017.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cancel.ico
# End Source File
# Begin Source File

SOURCE=.\res\controls6.ico
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\dlgleftl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dlgtoplo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\groupvie.bmp
# End Source File
# Begin Source File

SOURCE=.\res\helplogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00012.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00013.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00014.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00015.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_simt.ico
# End Source File
# Begin Source File

SOURCE=.\res\il_tab.bmp
# End Source File
# Begin Source File

SOURCE=.\res\imagelist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\indicato.ico
# End Source File
# Begin Source File

SOURCE=.\res\leftlogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\menuchec.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ok.ico
# End Source File
# Begin Source File

SOURCE=.\res\package.bmp
# End Source File
# Begin Source File

SOURCE=.\res\periodba.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prjview_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\simubar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\slistbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\startlog.bmp
# End Source File
# Begin Source File

SOURCE=.\res\strategy.bmp
# End Source File
# Begin Source File

SOURCE=.\res\strategy.ico
# End Source File
# Begin Source File

SOURCE=.\res\techsvie.bmp
# End Source File
# Begin Source File

SOURCE=.\res\text1.bin
# End Source File
# Begin Source File

SOURCE=.\res\text_dec.bin
# End Source File
# Begin Source File

SOURCE=.\res\text_upg.bin
# End Source File
# Begin Source File

SOURCE=.\res\toplogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\viewbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wizardhe.bmp
# End Source File
# Begin Source File

SOURCE=.\res\worksp_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\worksp_t.bmp
# End Source File
# Begin Source File

SOURCE=.\res\workspba.bmp
# End Source File
# Begin Source File

SOURCE=.\res\world.ico
# End Source File
# Begin Source File

SOURCE=.\res\zlinker.ico
# End Source File
# Begin Source File

SOURCE=.\res\zlinker.ico
# End Source File
# Begin Source File

SOURCE=.\res\zlinker1.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
