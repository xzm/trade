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


#if !defined(AFX_GUIMINISPLITTER_H__F242B3CD_66D9_4046_912A_28A47FEE2D66__INCLUDED_)
#define AFX_GUIMINISPLITTER_H__F242B3CD_66D9_4046_912A_28A47FEE2D66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuiMiniSplitter.h : header file
//
#include "GuiMiniTool.h"
#include "GuiLib.h" 


class GUILIBDLLEXPORT CGuiMiniSplitter : public CWnd
{
// Construction
public:
	CGuiMiniSplitter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiMiniSplitter)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGuiMiniSplitter();
	void    SetColor(COLORREF m_clrFace,COLORREF m_clrBorder);
	BOOL    AddMiniToolLeft(CGuiMiniTool * m_ToolLeft);
	BOOL    AddMiniToolRight(CGuiMiniTool * m_ToolRight);
	void	DrawSplitter(CRect rc,CDC* mdc);
	void	RecalLayout();
protected:
	int npos;	//splitter position
	CGuiMiniTool*	m_ToolLeft;
	CGuiMiniTool*	m_ToolRight;
	CRect m_rcSplitter;
	CRect m_rcSplitterOld;
	COLORREF m_clrface;	
	COLORREF m_clrborder;
	BOOL	 bIniciaArrastre;
	// Generated message map functions
protected:
	//{{AFX_MSG(CGuiMiniSplitter)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysColorChange();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIMINISPLITTER_H__F242B3CD_66D9_4046_912A_28A47FEE2D66__INCLUDED_)
