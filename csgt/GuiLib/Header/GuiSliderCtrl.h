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


#if !defined(AFX_GUISLIDERCTRL_H__1DC8660F_0D35_408B_BD0D_95621F561841__INCLUDED_)
#define AFX_GUISLIDERCTRL_H__1DC8660F_0D35_408B_BD0D_95621F561841__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuiSliderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuiSliderCtrl window
#include "GuiLib.h"
#include "GuiDrawLayer.h"

class GUILIBDLLEXPORT CGuiSliderCtrl : public CSliderCtrl
{
// Construction
public:
	CGuiSliderCtrl();

// Attributes
public:
	
	void DrawHThumb(CDC* pDC, CRect rcClient);
	void DrawVThumb(CDC* pDC, CRect rcClient);
	void DrawSlider(CDC* pDC, CRect rcClient);
public:
	BOOL bSelected;
	COLORREF m_clrGreen;  //verder
	COLORREF m_clrOrange; //no tan naranja
	GuiDrawLayer m_dl;
	BOOL	m_Press;
public:
	virtual ~CGuiSliderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGuiSliderCtrl)
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUISLIDERCTRL_H__1DC8660F_0D35_408B_BD0D_95621F561841__INCLUDED_)
