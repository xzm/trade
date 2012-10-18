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


#if !defined(AFX_GUIHEADERCTRL_H__E0AE9E10_F6F3_4C90_B19B_1DD8107B1C0D__INCLUDED_)
#define AFX_GUIHEADERCTRL_H__E0AE9E10_F6F3_4C90_B19B_1DD8107B1C0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuiHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuiHeaderCtrl window
#include "GuiLib.h" 


class GUILIBDLLEXPORT CGuiHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	//*******************************************************
	CGuiHeaderCtrl();
	virtual ~CGuiHeaderCtrl();
	//*******************************************************
protected:
	CWnd*		m_pParentWnd;
	COLORREF	m_clrFace;
	COLORREF	m_clrShadow;
	COLORREF	m_clrLight;
	CSize		m_sizeImag;
	CImageList  m_img;
	int			m_nNumbottomSelect;
	BOOL		m_bSentido;
public:
	//***********************************************************************
	void SetImageCol(int icol,int img);
	void ReDraw(); 
	void BiselaBoton(CRect rcWin,CDC* pDC);
	void SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask);
	void ClearImages();
	void DelImageCol(int icol,int img);
	void DrawTextExt(CString Cad,CDC* pDC,CRect* rc,UINT nFlags);
	void DrawArrow(CDC* pDC,CRect rc,BOOL bUp);
	void SetSortColumn(int nCol,BOOL sentido);
	int  GetSortColumn(BOOL* bSortCol );
	void CGuiHeaderCtrl::ResizeWindow();

	//***********************************************************************	
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiHeaderCtrl)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation

protected:
	//{{AFX_MSG(CGuiHeaderCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIHEADERCTRL_H__E0AE9E10_F6F3_4C90_B19B_1DD8107B1C0D__INCLUDED_)
