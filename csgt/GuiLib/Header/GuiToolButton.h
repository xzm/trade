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

#pragma once


// CGuiToolButton
#include "GuiLib.h"



class GUILIBDLLEXPORT   CGuiToolButton : public CButton
{
	DECLARE_DYNAMIC(CGuiToolButton)

public:
	CGuiToolButton();
	virtual ~CGuiToolButton();
public:
	void SetToolTip(LPCTSTR szToolTip);
	void SetCaption(CString szCaption);
	void SethIcon(HICON hIcon);
	void SetColor(COLORREF clColor);
	void RecalSize();
	void ShowDark(BOOL bShow);
	CSize GetSizeButton();
    void ShowMenu();
	void SetScrollButton(BOOL bScroll=TRUE);
	void SetFontColor(COLORREF clrFont);
protected:
	CToolTipCtrl m_toolTip;
	HICON		 m_Icon;
	CSize		 m_SizeImage;
	CSize		 m_SizeText;
	CString		 m_szText;
	COLORREF	 m_clColor;
	BOOL		 m_bMouserOver;
	BOOL		 m_ReposWindow;
	CSize		 m_szButton;
	BOOL		 m_bShowDark;
	BOOL		 m_ScrollButton;	
	COLORREF	 m_clrFont;

public:
	HMENU		 m_hMenu;
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
	afx_msg void OnSysColorChange( );
};


