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
#include "guitabwnd.h"
#include "guitoolbutton.h"
#include "GuiLib.h" 

class GUILIBDLLEXPORT CGuiTabPowerPoint :public CGuiTabWnd
{
protected:
	
	CImageList m_img;
	BOOL bAutoAjust;
	COLORREF m_clrTabs;
	CGuiToolButton  m_ToolButton;
	BOOL m_bShowButton;
public:
	CGuiTabPowerPoint(void);
	virtual ~CGuiTabPowerPoint(void);
	virtual void AjustTabs();
	void SetAutoAjust(BOOL bAjust=TRUE);
	void SetColorTabs(COLORREF clrTabs);
public:
	void	virtual Drawtabs(CDC* dc);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void    virtual Close();
	void    ShowButtonClose(BOOL bShowButton=TRUE);	
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
