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


// CGuiStatusBar

#include "GuiLib.h" 

//****************************************************************
class GUILIBDLLEXPORT  CGuiStatusBar : public CStatusBar
{
	DECLARE_DYNAMIC(CGuiStatusBar)

public:
	enum Style{
		   DEVSTUDIO=0,
           OFFICE=1
	};
	//**************************************
	CGuiStatusBar();
	virtual ~CGuiStatusBar();
	//**************************************
public:
	
	void		 Drawpanels(CDC *pDC);
	virtual void PreSubclassWindow();
	void		 DrawLB(CRect *rcLb,CDC* pDC);
	void		 ClearRect(CDC* pDC,CRect rc);
	void		 SetStyle(Style nStyle=CGuiStatusBar::DEVSTUDIO);
protected:
	Style m_Style;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};




