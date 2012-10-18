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
#include "guifolder.h"
#include "Guilib.h"

class GUILIBDLLEXPORT CGuiVisioFolder :	public CGuiFolder
{
	DECLARE_DYNAMIC(CGuiVisioFolder)
protected:
	CImageList	m_Img;
	
public:
	void  SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask);
	virtual void DrawFolder(CFolderBar* cfb,Style m_Style);
public:
	CGuiVisioFolder(void);
	virtual ~CGuiVisioFolder(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nHitTest, CPoint point);
	
};
