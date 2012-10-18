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

#if !defined(AFX_GUIRADIOBUTTON_H__B0A3DCE3_0F46_43C1_B4EB_7EFCEA8F57E7__INCLUDED_)
#define AFX_GUIRADIOBUTTON_H__B0A3DCE3_0F46_43C1_B4EB_7EFCEA8F57E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuiRadioButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuiRadioButton window
#include "GuiLib.h" 

class GUILIBDLLEXPORT CGuiRadioButton : public CButton
{
	DECLARE_DYNAMIC(CGuiRadioButton);
// Construction
public:
	CGuiRadioButton();

// Attributes
public:
	BOOL m_SelectBtn;
	BOOL m_bOldCheck;
	BOOL m_bPressBtn;
	CRect m_rc;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiRadioButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGuiRadioButton();
	void UnCheckRadio();
	void CheckRadio(CDC* pDC,CRect m_rcTemp); 
	void DrawEllipse(CDC* pDC,CRect m_rc);
	void SetCheck(int nCheck);
	void DrawOrange(CDC* pDC,CRect m_rc);
	// Generated message map functions
protected:
	//{{AFX_MSG(CGuiRadioButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIRADIOBUTTON_H__B0A3DCE3_0F46_43C1_B4EB_7EFCEA8F57E7__INCLUDED_)
