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

#if !defined(AFX_CHECKBOX_H__38E72D24_A4C7_11D5_B914_000000000000__INCLUDED_)
#define AFX_CHECKBOX_H__38E72D24_A4C7_11D5_B914_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CGuiCheckBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuiCheckBox window

#include "GuiLib.h"
#include "GuiDrawLayer.h"
class GUILIBDLLEXPORT CGuiCheckBox : public CButton
{
// Construction
public:
	CGuiCheckBox();
	virtual ~CGuiCheckBox();


// Attributes
public:

   void DrawCheck(CDC* pDC,CRect m_rcTemp);	
   void DrawOrange(CDC* pDC,CRect m_rcTemp);	
public:
   BOOL m_bCheckBtn;
   BOOL m_bPressBtn;
   BOOL m_bOldTemp;
   COLORREF m_clrHigh;
   COLORREF m_clrCheck;
   GuiDrawLayer m_wd;
   BOOL  m_bSelected;
   
public:
// Overrides
	//{{AFX_VIRTUAL(CGuiCheckBox)
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CGuiCheckBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int	 OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point); 
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKBOX_H__38E72D24_A4C7_11D5_B914_000000000000__INCLUDED_)
