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


#if !defined(AFX_GUILINKBUTTON_H__DAEC7876_F23A_11D4_9617_0008C7B2934C__INCLUDED_)
#define AFX_GUILINKBUTTON_H__DAEC7876_F23A_11D4_9617_0008C7B2934C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGuiLinkButton window

#include "GuiLib.h"

//***********************************************************
class GUILIBDLLEXPORT CGuiLinkButton : public CButton
{
// Construction
public:
	//*****************************************
	CGuiLinkButton();
	virtual ~CGuiLinkButton();
	//*****************************************

public:

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();

public:

	void SetLink(LPCSTR lpMsg);
	void SetToolTip(LPCSTR lpToolTip);
	void SetColorNormal(COLORREF m_clnor);
	void SetColorSelect(COLORREF m_clselec);

protected:

	COLORREF	 m_clNormal;
	COLORREF	 m_clover;
	CString		 m_Link;
	CToolTipCtrl m_ToolTipCtrl;
	CFont		 m_cfont;
	int			 m_iSelected;
	CRect		 m_rectMsg;
	BOOL		 m_bFirstPas;
	HCURSOR		 m_hCursor;
	// Generated message map functions
protected:

	//{{AFX_MSG(CGuiLinkButton)
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILINKBUTTON_H__DAEC7876_F23A_11D4_9617_0008C7B2934C__INCLUDED_)
