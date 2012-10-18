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
#include "guitoolbutton.h"
#include "GuiLib.h"

class GUILIBDLLEXPORT    CGuiImageLinkButton :
	public CGuiToolButton
{
	DECLARE_DYNAMIC(CGuiImageLinkButton)
public:
	CGuiImageLinkButton(void);
	virtual ~CGuiImageLinkButton(void);
	//esta funcion identa la cadenade caracteres a la derecha en el caso
	//que no exista imagen
	void AjustRightText(int nIdent){m_iDent=nIdent;}
protected:
	CFont	m_cfontSelect;
	CFont	m_cfontNormal;
	CRect	rectletra;
	int     m_iDent;
protected:
	DECLARE_MESSAGE_MAP()
	
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

class GUILIBDLLEXPORT    CGuiScrollButton :
	public CGuiToolButton
{
public:
	CGuiScrollButton(void);
	virtual ~CGuiScrollButton(void);

protected:
	DECLARE_MESSAGE_MAP()
	BOOL m_bMouserOvernew;
public:
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
};
