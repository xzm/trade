# Microsoft Developer Studio Generated NMAKE File, Based on ProfUIS_Controls.dsp
!IF "$(CFG)" == ""
CFG=ProfUIS_Controls - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ProfUIS_Controls - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ProfUIS_Controls - Win32 Release" && "$(CFG)" != "ProfUIS_Controls - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ProfUIS_Controls.mak" CFG="ProfUIS_Controls - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ProfUIS_Controls - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ProfUIS_Controls - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "ProfUIS_Controls - Win32 Release"

OUTDIR=.\..\..\Bin
INTDIR=.\..\..\Bin\Release\ProfUIS_Controls
# Begin Custom Macros
OutDir=.\..\..\Bin
# End Custom Macros

ALL : "$(OUTDIR)\ProfUIS_Controls.exe"


CLEAN :
	-@erase "$(INTDIR)\MainDlg.obj"
	-@erase "$(INTDIR)\ProfUIS_Controls.obj"
	-@erase "$(INTDIR)\ProfUIS_Controls.pch"
	-@erase "$(INTDIR)\ProfUIS_Controls.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ProfUIS_Controls.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ProfUIS_Controls.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\ProfUIS_Controls.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ProfUIS_Controls.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\ProfUIS_Controls.pdb" /machine:I386 /out:"$(OUTDIR)\ProfUIS_Controls.exe" /libpath:"..\..\Bin" 
LINK32_OBJS= \
	"$(INTDIR)\MainDlg.obj" \
	"$(INTDIR)\ProfUIS_Controls.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ProfUIS_Controls.res"

"$(OUTDIR)\ProfUIS_Controls.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ProfUIS_Controls - Win32 Debug"

OUTDIR=.\..\..\Bin
INTDIR=.\..\..\Bin\Debug\ProfUIS_Controls
# Begin Custom Macros
OutDir=.\..\..\Bin
# End Custom Macros

ALL : "$(OUTDIR)\ProfUIS_Controls-d.exe"


CLEAN :
	-@erase "$(INTDIR)\MainDlg.obj"
	-@erase "$(INTDIR)\ProfUIS_Controls.obj"
	-@erase "$(INTDIR)\ProfUIS_Controls.pch"
	-@erase "$(INTDIR)\ProfUIS_Controls.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ProfUIS_Controls-d.exe"
	-@erase "$(OUTDIR)\ProfUIS_Controls-d.ilk"
	-@erase "$(OUTDIR)\ProfUIS_Controls-d.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ProfUIS_Controls.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\ProfUIS_Controls.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ProfUIS_Controls.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ProfUIS_Controls-d.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ProfUIS_Controls-d.exe" /pdbtype:sept /libpath:"..\..\Bin" 
LINK32_OBJS= \
	"$(INTDIR)\MainDlg.obj" \
	"$(INTDIR)\ProfUIS_Controls.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ProfUIS_Controls.res"

"$(OUTDIR)\ProfUIS_Controls-d.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("ProfUIS_Controls.dep")
!INCLUDE "ProfUIS_Controls.dep"
!ELSE 
!MESSAGE Warning: cannot find "ProfUIS_Controls.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ProfUIS_Controls - Win32 Release" || "$(CFG)" == "ProfUIS_Controls - Win32 Debug"
SOURCE=.\MainDlg.cpp

"$(INTDIR)\MainDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ProfUIS_Controls.pch"


SOURCE=.\ProfUIS_Controls.cpp

"$(INTDIR)\ProfUIS_Controls.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ProfUIS_Controls.pch"


SOURCE=.\ProfUIS_Controls.rc

"$(INTDIR)\ProfUIS_Controls.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ProfUIS_Controls - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ProfUIS_Controls.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ProfUIS_Controls.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ProfUIS_Controls - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ProfUIS_Controls.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ProfUIS_Controls.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

