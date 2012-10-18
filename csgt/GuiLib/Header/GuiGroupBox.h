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

#if !defined(AFX_GUIGROUPBOX_H__C7AAD4CA_1B37_42E8_8F08_AEF18BA77DFD__INCLUDED_)
#define AFX_GUIGROUPBOX_H__C7AAD4CA_1B37_42E8_8F08_AEF18BA77DFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuiGroupBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuiGroupBox window
#include "GuiLib.h"

/////////////////////////////////////////////////////////////////////////////
// CGuiComboBoxExt window

class GUILIBDLLEXPORT CGuiGroupBox : public CButton
{
// Construction
public:
	CGuiGroupBox();
	enum Aling{ALING_LEFT=0,ALING_RIGHT=1,ALING_DOWN=2, ALING_UP=3};
// Attributes
public:
   void SetStyle(Aling AlingStyle);
   Aling m_style;
   COLORREF  m_clrface;
   COLORREF  m_clrShadow;
// Operations
public:
	virtual void PreSubclassWindow();
public:
	virtual ~CGuiGroupBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGuiGroupBox)
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIGROUPBOX_H__C7AAD4CA_1B37_42E8_8F08_AEF18BA77DFD__INCLUDED_)
