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


#if !defined(AFX_GUIOUTLOOKVIEW_H__C539A48F_9650_4AE2_861F_D208728C60A1__INCLUDED_)
#define AFX_GUIOUTLOOKVIEW_H__C539A48F_9650_4AE2_861F_D208728C60A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GuiLib.h"
#include "GuiFolder.h"


class GUILIBDLLEXPORT CGuiOutLookView : public CView  
{

public:
	DECLARE_DYNCREATE(CGuiOutLookView)

	//*********************************************	
		CGuiOutLookView();
		virtual ~CGuiOutLookView();
	//*********************************************

public:
		CGuiFolder cf;
protected:

		virtual void OnInitialUpdate();
		virtual void OnDraw(CDC* pDC);      // overridden to draw this view

protected:
	
#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	//{{AFX_MSG(CGuiOutLookView)
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GUIOUTLOOKVIEW_H__C539A48F_9650_4AE2_861F_D208728C60A1__INCLUDED_)
