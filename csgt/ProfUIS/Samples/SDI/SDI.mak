# Microsoft Developer Studio Generated NMAKE File, Based on SDI.dsp
!IF "$(CFG)" == ""
CFG=SDI - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SDI - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SDI - Win32 Release" && "$(CFG)" != "SDI - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SDI.mak" CFG="SDI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SDI - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SDI - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "SDI - Win32 Release"

OUTDIR=.\..\..\Bin
INTDIR=.\..\..\Bin\Release\SDI
# Begin Custom Macros
OutDir=.\..\..\Bin
# End Custom Macros

ALL : "$(OUTDIR)\SDI.exe"


CLEAN :
	-@erase "$(INTDIR)\ChildView.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\SDI.obj"
	-@erase "$(INTDIR)\SDI.pch"
	-@erase "$(INTDIR)\SDI.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SDI.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\SDI.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\SDI.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SDI.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\SDI.pdb" /machine:I386 /out:"$(OUTDIR)\SDI.exe" /libpath:"..\..\Bin" 
LINK32_OBJS= \
	"$(INTDIR)\ChildView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\SDI.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SDI.res"

"$(OUTDIR)\SDI.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SDI - Win32 Debug"

OUTDIR=.\..\..\Bin
INTDIR=.\..\..\Bin\Debug\SDI
# Begin Custom Macros
OutDir=.\..\..\Bin
# End Custom Macros

ALL : "$(OUTDIR)\SDI-d.exe"


CLEAN :
	-@erase "$(INTDIR)\ChildView.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\SDI.obj"
	-@erase "$(INTDIR)\SDI.pch"
	-@erase "$(INTDIR)\SDI.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SDI-d.exe"
	-@erase "$(OUTDIR)\SDI-d.ilk"
	-@erase "$(OUTDIR)\SDI-d.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\SDI.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\SDI.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SDI.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\SDI-d.pdb" /debug /machine:I386 /out:"$(OUTDIR)\SDI-d.exe" /pdbtype:sept /libpath:"..\..\Bin" 
LINK32_OBJS= \
	"$(INTDIR)\ChildView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\SDI.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SDI.res"

"$(OUTDIR)\SDI-d.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("SDI.dep")
!INCLUDE "SDI.dep"
!ELSE 
!MESSAGE Warning: cannot find "SDI.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SDI - Win32 Release" || "$(CFG)" == "SDI - Win32 Debug"
SOURCE=.\ChildView.cpp

"$(INTDIR)\ChildView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SDI.pch"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SDI.pch"


SOURCE=.\SDI.cpp

"$(INTDIR)\SDI.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SDI.pch"


SOURCE=.\SDI.rc

"$(INTDIR)\SDI.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SDI - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\SDI.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SDI.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SDI - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\SDI.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SDI.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

