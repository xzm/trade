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


// CGuiPowerPointView view
#include "GuiTabPowerPoint.h"
#include "GuiLib.h"

class GUILIBDLLEXPORT CGuiPowerPointView : public CView
{
	DECLARE_DYNCREATE(CGuiPowerPointView)
protected:
	CGuiTabPowerPoint m_tbpp;
protected:
	CGuiPowerPointView();           // protected constructor used by dynamic creation
	virtual ~CGuiPowerPointView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


