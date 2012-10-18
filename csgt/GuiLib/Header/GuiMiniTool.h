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


#if !defined(AFX_GUIMINITOOL_H__D4A0E123_2E03_4BF4_B22F_8E89EC901EFD__INCLUDED_)
#define AFX_GUIMINITOOL_H__D4A0E123_2E03_4BF4_B22F_8E89EC901EFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuiMiniTool.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuiMiniTool window

#include "GuiLib.h"
#include "GuiToolButton.h"
#include "GuiComboBoxExt.h"

//esta clase da la flexibilidad de adicionar componentes
//diferentes a botones.
class CArrButton
{
public:
	CArrButton(CGuiToolButton* m_button,HICON m_hIcon,CMenu* pMenu=NULL,CString m_ToolTip=_T(""));
	CArrButton(CGuiComboBoxExt* m_pComponent);
public:
	CGuiToolButton*		   m_bt;
	CGuiComboBoxExt*      m_pwnd;
	int				   m_btipo;  //es un boton o un derivado de Cwnd

};


class GUILIBDLLEXPORT CGuiMiniTool : public CStatic
{
// Construction
public:
	CGuiMiniTool();
public:
	enum Borders{STYLEFLAT=0,STYLE3D=1,STYLEPRESS=2,STYLEFRAME=3};
	enum Aling{	ALIGN_LEFT=0,ALIGN_RIGHT=1};
// Attributes
public:
	//*********************************************************************
	void  AddButton(int nIndex,UINT Cmd,CMenu* pMenu=NULL,CString m_szCaption=_T(""),CString m_ToolTip=_T(""));
	BOOL  CreateCombo(CGuiComboBoxExt* pControl,UINT nID,int iSize,
				DWORD dwStyle=WS_CHILD|WS_VISIBLE|CBS_AUTOHSCROLL|
				CBS_DROPDOWNLIST|CBS_HASSTRINGS);
	void  SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask);
	void  DelButton(int nIndex);
	int   GetCountButton();
	void  RecalLayout();
	void  SetCaption(CString mSzCaption,Aling AlinText);
	//*********************************************************************
	void  SetBorder(Borders Style);
	void  SetColor(COLORREF m_clrface);
	void  AlingButtons(Aling Alingsb);
	void  AutoSize(BOOL mbAutosize);
	void  CalcAutoSize(CRect m_rc);
	void  SetColorCaption(COLORREF	clrCaption);
	void  Drawtext();
	HICON GetImagIcon(int nNum);
	CGuiToolButton* GetButton(UINT uID_Object);
// Operations
protected:
   CPtrArray	m_arrButtons;
   CImageList	m_imgList;
   COLORREF		m_clrface;
   int			nNumItems;
   Borders		m_border;
   Aling		m_AlingButton;		
   BOOL			m_bAutoSize;
   int			m_nResultCombo;
   int			m_nResultButtons; 
   BOOL			m_bExisteCombo;
   CFont		m_cfont;	   
   CString		m_Caption;		
   COLORREF		m_clrCaption;
   CSize		msz;
   Aling		m_AlinTexto;
   CRect	    m_rcDrawTexto;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiMiniTool)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGuiMiniTool();
	virtual void PreSubclassWindow();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	// Generated message map functions
protected:
	//{{AFX_MSG(CGuiMiniTool)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint(); 
	afx_msg void OnSysColorChange( );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIMINITOOL_H__D4A0E123_2E03_4BF4_B22F_8E89EC901EFD__INCLUDED_)
