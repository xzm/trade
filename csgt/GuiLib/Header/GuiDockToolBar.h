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

#if !defined(AFX_GUIDOCKTOOLBAR_H__83DAE7D7_071E_4522_A669_8E45D3089B56__INCLUDED_)
#define AFX_GUIDOCKTOOLBAR_H__83DAE7D7_071E_4522_A669_8E45D3089B56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuiDockToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuiDockToolBar window
#define AFX_TOOLBAR  0xffff
#include "GuiLib.h"


class GUILIBDLLEXPORT CGuiDockToolBar : public CControlBar
{
// Construction
public:
	CGuiDockToolBar();

// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiDockToolBar)
	public:
	virtual BOOL Create(CWnd* pParentWnd,DWORD dwAling);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual ~CGuiDockToolBar();

	// Generated message map functions
protected:
	int  m_Size;
	COLORREF m_clrface;
protected:
	//{{AFX_MSG(CGuiDockToolBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIDOCKTOOLBAR_H__83DAE7D7_071E_4522_A669_8E45D3089B56__INCLUDED_)
