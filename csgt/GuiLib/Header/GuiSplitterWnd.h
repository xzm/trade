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


// CGuiSplitterWnd
#include "GuiLib.h" 

//*****************************************************************
class GUILIBDLLEXPORT CGuiSplitterWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CGuiSplitterWnd)

public:
	//********************************************
		CGuiSplitterWnd();
		virtual ~CGuiSplitterWnd();
	//********************************************
public:
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	void SetBorderFlat(BOOL bBorder=TRUE);
protected:
	BOOL m_bBorder;
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

};


