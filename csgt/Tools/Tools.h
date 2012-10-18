// Tools.h : main header file for the TOOLS application
//

#if !defined(AFX_TOOLS_H__B8E41B62_F3FB_4A30_BC71_B869E0CF72AD__INCLUDED_)
#define AFX_TOOLS_H__B8E41B62_F3FB_4A30_BC71_B869E0CF72AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CToolsApp:
// See Tools.cpp for the implementation of this class
//

class CToolsApp : public CWinApp
{
public:
	CToolsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolsApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CToolsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLS_H__B8E41B62_F3FB_4A30_BC71_B869E0CF72AD__INCLUDED_)
