# Microsoft Developer Studio Generated NMAKE File, Based on StateInFile.dsp
!IF "$(CFG)" == ""
CFG=StateInFile - Win32 Debug
!MESSAGE No configuration specified. Defaulting to StateInFile - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "StateInFile - Win32 Release" && "$(CFG)" != "StateInFile - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StateInFile.mak" CFG="StateInFile - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StateInFile - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "StateInFile - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "StateInFile - Win32 Release"

OUTDIR=.\..\..\Bin
INTDIR=.\..\..\Bin\Release\StateInFile
# Begin Custom Macros
OutDir=.\..\..\Bin
# End Custom Macros

ALL : "$(OUTDIR)\StateInFile.exe"


CLEAN :
	-@erase "$(INTDIR)\ChildView.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\StateInFile.obj"
	-@erase "$(INTDIR)\StateInFile.pch"
	-@erase "$(INTDIR)\StateInFile.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\StateInFile.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\StateInFile.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\StateInFile.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\StateInFile.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\StateInFile.pdb" /machine:I386 /out:"$(OUTDIR)\StateInFile.exe" /libpath:"..\..\Bin" 
LINK32_OBJS= \
	"$(INTDIR)\ChildView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\StateInFile.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StateInFile.res"

"$(OUTDIR)\StateInFile.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "StateInFile - Win32 Debug"

OUTDIR=.\..\..\Bin
INTDIR=.\..\..\Bin\Debug\StateInFile
# Begin Custom Macros
OutDir=.\..\..\Bin
# End Custom Macros

ALL : "$(OUTDIR)\StateInFile-d.exe"


CLEAN :
	-@erase "$(INTDIR)\ChildView.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\StateInFile.obj"
	-@erase "$(INTDIR)\StateInFile.pch"
	-@erase "$(INTDIR)\StateInFile.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\StateInFile-d.exe"
	-@erase "$(OUTDIR)\StateInFile-d.ilk"
	-@erase "$(OUTDIR)\StateInFile-d.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\StateInFile.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\StateInFile.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\StateInFile.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\StateInFile-d.pdb" /debug /machine:I386 /out:"$(OUTDIR)\StateInFile-d.exe" /pdbtype:sept /libpath:"..\..\Bin" 
LINK32_OBJS= \
	"$(INTDIR)\ChildView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\StateInFile.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StateInFile.res"

"$(OUTDIR)\StateInFile-d.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("StateInFile.dep")
!INCLUDE "StateInFile.dep"
!ELSE 
!MESSAGE Warning: cannot find "StateInFile.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "StateInFile - Win32 Release" || "$(CFG)" == "StateInFile - Win32 Debug"
SOURCE=.\ChildView.cpp

"$(INTDIR)\ChildView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\StateInFile.pch"


SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\StateInFile.pch"


SOURCE=.\StateInFile.cpp

"$(INTDIR)\StateInFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\StateInFile.pch"


SOURCE=.\StateInFile.rc

"$(INTDIR)\StateInFile.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "StateInFile - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\StateInFile.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StateInFile.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "StateInFile - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\StateInFile.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StateInFile.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

