// ProfUIS_Controls.h : main header file for the PROFUIS_CONTROLS application
//

#if !defined(AFX_PROFUIS_CONTROLS_H__4ABF6D1C_5BEA_4FEB_B891_B762A236AE89__INCLUDED_)
#define AFX_PROFUIS_CONTROLS_H__4ABF6D1C_5BEA_4FEB_B891_B762A236AE89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMainApp:
// See ProfUIS_Controls.cpp for the implementation of this class
//

class CMainApp : public CWinApp
{
public:
	CMainApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMainApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFUIS_CONTROLS_H__4ABF6D1C_5BEA_4FEB_B891_B762A236AE89__INCLUDED_)
