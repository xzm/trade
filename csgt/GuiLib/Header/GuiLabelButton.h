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


class GUILIBDLLEXPORT CGuiLabelButton :
	public CGuiToolButton
		
{
		DECLARE_DYNAMIC(CGuiLabelButton)
public:
	CGuiLabelButton(void);
	virtual ~CGuiLabelButton(void);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void DrawUnderLine(BOOL bDraw=TRUE){m_bDrawLine=bDraw; Invalidate(); UpdateData();};
protected:
	CFont	m_cfont;
	BOOL	m_bDrawLine;
};
