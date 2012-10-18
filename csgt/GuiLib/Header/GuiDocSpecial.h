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

#if !defined(AFX_GUIDOCSPECIAL_H__18F3DFEE_82D2_4F88_AB42_7BD36F100842__INCLUDED_)
#define AFX_GUIDOCSPECIAL_H__18F3DFEE_82D2_4F88_AB42_7BD36F100842__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiLib.h"
#include "GuiTabWnd.h"
#include <Afxpriv.h>

class GUILIBDLLEXPORT CGuiDocSpecial : public CDockBar  
{
public:
	//***********************************
	CGuiDocSpecial();
	virtual ~CGuiDocSpecial();
	//***********************************
public:
		void RecalTabs();
		CSize CalcDynamicLayout(int nLength,DWORD nMode);
protected:
		CGuiTabWnd  m_tabwnd;
		
protected:

	//{{AFX_VIRTUAL(CGuiDocSpecial)
		afx_msg void OnSize (UINT  nType, int cx, int cy);	
		afx_msg void OnNcPaint();
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GUIDOCSPECIAL_H__18F3DFEE_82D2_4F88_AB42_7BD36F100842__INCLUDED_)
