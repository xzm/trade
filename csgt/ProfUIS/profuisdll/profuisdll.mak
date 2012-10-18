# Microsoft Developer Studio Generated NMAKE File, Based on profuisdll.dsp
!IF "$(CFG)" == ""
CFG=profuisdll - Win32 Debug
!MESSAGE No configuration specified. Defaulting to profuisdll - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "profuisdll - Win32 Release" && "$(CFG)" != "profuisdll - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "profuisdll.mak" CFG="profuisdll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "profuisdll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "profuisdll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "profuisdll - Win32 Release"

OUTDIR=.\..\Bin
INTDIR=.\..\Bin\Release\profuisdll
# Begin Custom Macros
OutDir=.\..\Bin
# End Custom Macros

ALL : "$(OUTDIR)\ProfUIS21.dll"


CLEAN :
	-@erase "$(INTDIR)\exdlgbase.obj"
	-@erase "$(INTDIR)\ExtBtnOnFlat.obj"
	-@erase "$(INTDIR)\ExtButton.obj"
	-@erase "$(INTDIR)\ExtCmdManager.obj"
	-@erase "$(INTDIR)\ExtColorCtrl.obj"
	-@erase "$(INTDIR)\ExtColorDlg.obj"
	-@erase "$(INTDIR)\ExtComboBox.obj"
	-@erase "$(INTDIR)\ExtControlBar.obj"
	-@erase "$(INTDIR)\ExtDockBar.obj"
	-@erase "$(INTDIR)\ExtEdit.obj"
	-@erase "$(INTDIR)\ExtHook.obj"
	-@erase "$(INTDIR)\ExtMenuControlBar.obj"
	-@erase "$(INTDIR)\ExtMiniDockFrameWnd.obj"
	-@erase "$(INTDIR)\ExtPaintManager.obj"
	-@erase "$(INTDIR)\ExtPopupMenuWnd.obj"
	-@erase "$(INTDIR)\ExtResizableDialog.obj"
	-@erase "$(INTDIR)\ExtStatusControlBar.obj"
	-@erase "$(INTDIR)\ExtToolControlBar.obj"
	-@erase "$(INTDIR)\profuisdll.obj"
	-@erase "$(INTDIR)\profuisdll.pch"
	-@erase "$(INTDIR)\profuisdll.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ProfUIS21.dll"
	-@erase "$(OUTDIR)\ProfUIS21.exp"
	-@erase "$(OUTDIR)\ProfUIS21.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "__PROF_UIS_IMPL__" /Fp"$(INTDIR)\profuisdll.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\profuisdll.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\profuisdll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\ProfUIS21.pdb" /machine:I386 /def:".\profuisdll.def" /out:"$(OUTDIR)\ProfUIS21.dll" /implib:"$(OUTDIR)\ProfUIS21.lib" 
DEF_FILE= \
	".\profuisdll.def"
LINK32_OBJS= \
	"$(INTDIR)\exdlgbase.obj" \
	"$(INTDIR)\ExtBtnOnFlat.obj" \
	"$(INTDIR)\ExtButton.obj" \
	"$(INTDIR)\ExtCmdManager.obj" \
	"$(INTDIR)\ExtColorCtrl.obj" \
	"$(INTDIR)\ExtColorDlg.obj" \
	"$(INTDIR)\ExtComboBox.obj" \
	"$(INTDIR)\ExtControlBar.obj" \
	"$(INTDIR)\ExtDockBar.obj" \
	"$(INTDIR)\ExtEdit.obj" \
	"$(INTDIR)\ExtHook.obj" \
	"$(INTDIR)\ExtMenuControlBar.obj" \
	"$(INTDIR)\ExtMiniDockFrameWnd.obj" \
	"$(INTDIR)\ExtPaintManager.obj" \
	"$(INTDIR)\ExtPopupMenuWnd.obj" \
	"$(INTDIR)\ExtResizableDialog.obj" \
	"$(INTDIR)\ExtToolControlBar.obj" \
	"$(INTDIR)\profuisdll.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\profuisdll.res" \
	"$(INTDIR)\ExtStatusControlBar.obj"

"$(OUTDIR)\ProfUIS21.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "profuisdll - Win32 Debug"

OUTDIR=.\..\Bin
INTDIR=.\..\Bin\Debug\profuisdll
# Begin Custom Macros
OutDir=.\..\Bin
# End Custom Macros

ALL : "$(OUTDIR)\ProfUIS21d.dll"


CLEAN :
	-@erase "$(INTDIR)\exdlgbase.obj"
	-@erase "$(INTDIR)\ExtBtnOnFlat.obj"
	-@erase "$(INTDIR)\ExtButton.obj"
	-@erase "$(INTDIR)\ExtCmdManager.obj"
	-@erase "$(INTDIR)\ExtColorCtrl.obj"
	-@erase "$(INTDIR)\ExtColorDlg.obj"
	-@erase "$(INTDIR)\ExtComboBox.obj"
	-@erase "$(INTDIR)\ExtControlBar.obj"
	-@erase "$(INTDIR)\ExtDockBar.obj"
	-@erase "$(INTDIR)\ExtEdit.obj"
	-@erase "$(INTDIR)\ExtHook.obj"
	-@erase "$(INTDIR)\ExtMenuControlBar.obj"
	-@erase "$(INTDIR)\ExtMiniDockFrameWnd.obj"
	-@erase "$(INTDIR)\ExtPaintManager.obj"
	-@erase "$(INTDIR)\ExtPopupMenuWnd.obj"
	-@erase "$(INTDIR)\ExtResizableDialog.obj"
	-@erase "$(INTDIR)\ExtStatusControlBar.obj"
	-@erase "$(INTDIR)\ExtToolControlBar.obj"
	-@erase "$(INTDIR)\profuisdll.obj"
	-@erase "$(INTDIR)\profuisdll.pch"
	-@erase "$(INTDIR)\profuisdll.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ProfUIS21d.dll"
	-@erase "$(OUTDIR)\ProfUIS21d.exp"
	-@erase "$(OUTDIR)\ProfUIS21d.ilk"
	-@erase "$(OUTDIR)\ProfUIS21d.lib"
	-@erase "$(OUTDIR)\ProfUIS21d.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "__PROF_UIS_IMPL__" /Fp"$(INTDIR)\profuisdll.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\profuisdll.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\profuisdll.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\ProfUIS21d.pdb" /debug /machine:I386 /def:".\profuisdll.def" /out:"$(OUTDIR)\ProfUIS21d.dll" /implib:"$(OUTDIR)\ProfUIS21d.lib" /pdbtype:sept 
DEF_FILE= \
	".\profuisdll.def"
LINK32_OBJS= \
	"$(INTDIR)\exdlgbase.obj" \
	"$(INTDIR)\ExtBtnOnFlat.obj" \
	"$(INTDIR)\ExtButton.obj" \
	"$(INTDIR)\ExtCmdManager.obj" \
	"$(INTDIR)\ExtColorCtrl.obj" \
	"$(INTDIR)\ExtColorDlg.obj" \
	"$(INTDIR)\ExtComboBox.obj" \
	"$(INTDIR)\ExtControlBar.obj" \
	"$(INTDIR)\ExtDockBar.obj" \
	"$(INTDIR)\ExtEdit.obj" \
	"$(INTDIR)\ExtHook.obj" \
	"$(INTDIR)\ExtMenuControlBar.obj" \
	"$(INTDIR)\ExtMiniDockFrameWnd.obj" \
	"$(INTDIR)\ExtPaintManager.obj" \
	"$(INTDIR)\ExtPopupMenuWnd.obj" \
	"$(INTDIR)\ExtResizableDialog.obj" \
	"$(INTDIR)\ExtToolControlBar.obj" \
	"$(INTDIR)\profuisdll.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\profuisdll.res" \
	"$(INTDIR)\ExtStatusControlBar.obj"

"$(OUTDIR)\ProfUIS21d.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("profuisdll.dep")
!INCLUDE "profuisdll.dep"
!ELSE 
!MESSAGE Warning: cannot find "profuisdll.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "profuisdll - Win32 Release" || "$(CFG)" == "profuisdll - Win32 Debug"
SOURCE=..\Src\exdlgbase.cpp

"$(INTDIR)\exdlgbase.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtBtnOnFlat.cpp

"$(INTDIR)\ExtBtnOnFlat.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtButton.cpp

"$(INTDIR)\ExtButton.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtCmdManager.cpp

"$(INTDIR)\ExtCmdManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtColorCtrl.cpp

"$(INTDIR)\ExtColorCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtColorDlg.cpp

"$(INTDIR)\ExtColorDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtComboBox.cpp

"$(INTDIR)\ExtComboBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtControlBar.cpp

"$(INTDIR)\ExtControlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtDockBar.cpp

"$(INTDIR)\ExtDockBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtEdit.cpp

"$(INTDIR)\ExtEdit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtHook.cpp

"$(INTDIR)\ExtHook.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtMenuControlBar.cpp

"$(INTDIR)\ExtMenuControlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtMiniDockFrameWnd.cpp

"$(INTDIR)\ExtMiniDockFrameWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtPaintManager.cpp

"$(INTDIR)\ExtPaintManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtPopupMenuWnd.cpp

"$(INTDIR)\ExtPopupMenuWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtResizableDialog.cpp

"$(INTDIR)\ExtResizableDialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\ExtStatusControlBar.cpp

!IF  "$(CFG)" == "profuisdll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GR /GX /O2 /I "..\Include" /I "..\profuisdll" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "__PROF_UIS_IMPL__" /Fp"$(INTDIR)\profuisdll.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ExtStatusControlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "profuisdll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\Include" /I "..\profuisdll" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "__PROF_UIS_IMPL__" /Fp"$(INTDIR)\profuisdll.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ExtStatusControlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Src\ExtToolControlBar.cpp

"$(INTDIR)\ExtToolControlBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\profuisdll.cpp

"$(INTDIR)\profuisdll.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\profuisdll.pch"


SOURCE=.\profuisdll.rc

"$(INTDIR)\profuisdll.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "profuisdll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GR /GX /O2 /I "..\Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "__PROF_UIS_IMPL__" /Fp"$(INTDIR)\profuisdll.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\profuisdll.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "profuisdll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "__PROF_UIS_IMPL__" /Fp"$(INTDIR)\profuisdll.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\profuisdll.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

