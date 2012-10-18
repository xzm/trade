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
#include "GuiNormalButton.h"
#include "GuiLib.h" 

class GuiTabbedButton: public CGuiNormalButton
{
public:
	GuiTabbedButton();
	virtual ~GuiTabbedButton();
protected:
	BOOL	bPress;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
class GUILIBDLLEXPORT CGuiTabbed :	public CGuiTabWnd
{
protected:
	CGuiNormalButton m_toolBtn[3];
	CImageList   m_img;
	CSize        m_sizeBtn;
	int			 m_nDif;	
public:
	CGuiTabbed(void);
	virtual ~CGuiTabbed(void);
public:
	void	virtual Drawtabs(CDC* dc);
	CSize   GetSizeButton();
	void  OnScrollLeft();
	void  OnScrollRight();
	void  virtual OnScrollClose();
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
