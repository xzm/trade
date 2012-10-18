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

#if !defined(AFX_GUIDRAWLAYER_H__0BFB5AA0_94B4_47FC_8F93_84DB87A364F8__INCLUDED_)
#define AFX_GUIDRAWLAYER_H__0BFB5AA0_94B4_47FC_8F93_84DB87A364F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GuiLib.h" 


class GUILIBDLLEXPORT GuiDrawLayer  
{
public:
		GuiDrawLayer();
		virtual ~GuiDrawLayer();
public:

	//************************************************************
	void		DrawArrow(CDC* pDC,CRect m_rc,BOOL m_bDown=TRUE);
	HICON		GetIcon(int nIcon);
	static HICON		LoadIconLib(UINT uIcon);
	static COLORREF	GetRGBColorXP();
	static COLORREF	GetRGBFondoXP();
	static COLORREF	GetRGBCaptionXP();
	static COLORREF	GetRGBPressBXP();
	static COLORREF	GetRGBColorFace();
	static COLORREF	GetRGBColorShadow();
	static COLORREF	GetRGBColorBTNHigh();
	static COLORREF	GetRGBColorTabs();
	static COLORREF	GetRGBColorWhite();
	static void DrawCheck(CDC* pDC,CRect m_rcTemp);
	static BOOL		IsGrayed();
	static void DrawShade( CRect Rect,CPoint cp,CDC* pDC);
	static COLORREF DarkenColor( long lScale, COLORREF lColor);
	//************************************************************

protected:
	//**********************************
	CImageList	m_img;
	CPoint		mHeight;
	int			m_iWidthDrowDown;
	//**********************************

};

#endif // !defined(AFX_GUIDRAWLAYER_H__0BFB5AA0_94B4_47FC_8F93_84DB87A364F8__INCLUDED_)
