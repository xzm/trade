// TestBench.h : main header file for the TESTBENCH application
//

#if !defined(AFX_TESTBENCH_H__64CA51E9_D71D_48A7_BED8_7FA541FDDCD5__INCLUDED_)
#define AFX_TESTBENCH_H__64CA51E9_D71D_48A7_BED8_7FA541FDDCD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestBenchApp:
// See TestBench.cpp for the implementation of this class
//

class CTestBenchApp : public CWinApp
{
public:
	CTestBenchApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestBenchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTestBenchApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTBENCH_H__64CA51E9_D71D_48A7_BED8_7FA541FDDCD5__INCLUDED_)
