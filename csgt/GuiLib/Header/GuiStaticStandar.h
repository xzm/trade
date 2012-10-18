//-----------------------------------------------------------------------//
// This is a part of the GuiLib MFC Extention.							 //	
// Autor  :  Francisco Campos											 //
// (C) 2002 Francisco Campos <www.beyondata.com> All rights reserved     //
// This code is provided "as is", with absolutely no warranty expressed  //
// or implied. Any use is at your own risk.								 //		
// You must obtain the author's consent before you can include this code //
// in a software library.												 //
// If the source code in  this file is used in any application			 //
// then acknowledgement must be made to the author of this program		 //	
// fcampos@tutopia.com													 //
//-----------------------------------------------------------------------//
#if !defined(AFX_GUISTATICSTANDAR_H__E577069E_DA47_4E1D_9271_B7EA22634BCE__INCLUDED_)
#define AFX_GUISTATICSTANDAR_H__E577069E_DA47_4E1D_9271_B7EA22634BCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuiStaticStandar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuiStaticStandar window
#include "Guilib.h"

class GUILIBDLLEXPORT CGuiStaticStandar : public CStatic
{
// Construction
public:
	CGuiStaticStandar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiStaticStandar)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGuiStaticStandar();
	void AddChild(CWnd* pWnd);
	void RecalLayout();
	// Generated message map functions
protected:
	CWnd* m_pWnd;
	//{{AFX_MSG(CGuiStaticStandar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUISTATICSTANDAR_H__E577069E_DA47_4E1D_9271_B7EA22634BCE__INCLUDED_)
