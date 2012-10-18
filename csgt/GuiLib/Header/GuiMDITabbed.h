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
#include "guitabbed.h"
#include "GuiLib.h" 
class GUILIBDLLEXPORT CGuiMDITabbed :public CGuiTabbed
{
public:
	CGuiMDITabbed(void);
	virtual ~CGuiMDITabbed(void);
protected:
	CWnd* pParentWindow;
	CGuiTabbed m_GuiTabbed;
	DECLARE_MESSAGE_MAP();
	void  virtual OnScrollClose();
public:
	void UpdateWindows();
	afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);

};
