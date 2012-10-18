#if !defined(AFX_AFXCORE_H__2F96EC6F_0775_45AA_99EE_0FC048AD3208__INCLUDED_)
#define AFX_AFXCORE_H__2F96EC6F_0775_45AA_99EE_0FC048AD3208__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AfxCore.h : header file
//

class	CToolsApp;
class	CMainFrame;
/////////////////////////////////////////////////////////////////////////////
// The following is inline functions

// Secret Assert Valid Functions
void AfxAssertNull( );
BOOL AfxAssertValid( );
BOOL AfxAssertValidStd( );
BOOL AfxAssertValidProf( );
BOOL AfxAssertFunclimitStd( UINT nIDString = IDS_SECRET_FUNCLIMIT );
BOOL AfxAssertFunclimitProf( UINT nIDString = IDS_SECRET_FUNCLIMIT );
BOOL AfxAssertRegisterQuit( );
BOOL AfxAssertMessageQuit( );
BOOL AfxAssertQuit( );

// app and mainframe and doc functions
CToolsApp * AfxGetToolsApp( );
CMainFrame	* AfxGetMainFrame( );

/////////////////////////////////////////////////////////////////////
// The Following is command functions

void DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, CSPTime& value);

// Rigister and file functions
void AfxRegisterShellFileTypes(BOOL bCompat);
void AfxUnregisterShellFileTypes();
BOOL AfxDoPromptFileName(CString& fileName,
						 UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog );
BOOL AfxDoSelectDirectory( CString &strSrc, HWND hWnd = NULL );


#include "Tools.h"
#include "MainFrm.h"

#define _AFXCORE_INLINE AFX_INLINE
#include "AfxCore.inl"
#undef	_AFXCORE_INLINE

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AFXCORE_H__2F96EC6F_0775_45AA_99EE_0FC048AD3208__INCLUDED_)
