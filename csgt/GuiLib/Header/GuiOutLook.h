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


#if !defined(AFX_GUIOUTLOOK_H__B553E721_33F1_493C_A265_BB0C32881A3F__INCLUDED_)
#define AFX_GUIOUTLOOK_H__B553E721_33F1_493C_A265_BB0C32881A3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#define WM_OUTBAR_NOTIFY			WM_USER+1
//#define NM_OB_ITEMCLICK				WM_OUTBAR_NOTIFY+1

#include "GuiLib.h"

#include "GuiToolButton.h"

//esta clase da la flexibilidad de adicionar componentes
//diferentes a botones.
class CItems
{
public:
	CItems(CGuiToolButton* m_button,HICON m_hIcon,CString m_Message,CString m_ToolTip=_T(""));
public:
	CGuiToolButton*		   m_bt;
	int					   m_btipo;  //es un boton o un derivado de Cwnd
	CString				   m_szMessage;//el mensaje bajo el boton		
};



class GUILIBDLLEXPORT  CGuiOutLook : public CWnd
{

	friend class CItems;	
	DECLARE_DYNCREATE(CGuiOutLook)

public:
	//***************************************
	CGuiOutLook();
	virtual ~CGuiOutLook();
	//***************************************

public:

	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void	AddItem(UINT Cmd,CString m_szCaption,int nIndex,CString m_ToolTip=_T(""));
	void	SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void	RecalLayout();
	
protected:
	//**************************************************
	CImageList m_imageList;
	CPtrArray  m_ArrayButtons;
	CSize      m_sizeImage;
	int		   m_ActualButtonPres;
	int        m_ultimoBotonResal;
	int        m_NumBottoms;
	COLORREF   m_cbkg;	
	int		   m_mouseMove;
	int        m_Avance;    //avance del scroll
	CRect	   m_rectArrowUp;
	CRect	   m_rectArrowDown;	
	BOOL	   m_iDrawFrameUp;		
	BOOL       m_iDrawFrameDown;	    
	UINT	   mnID;
	BOOL	   m_bPressU;
	BOOL	   m_bPressD;
	HDC		   hdc;
	BOOL	   m_bIsXp;
	//**************************************************

protected:

	//{{AFX_MSG(CGuiOutLook)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg void OnSysColorChange(); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIOUTLOOK_H__B553E721_33F1_493C_A265_BB0C32881A3F__INCLUDED_)
