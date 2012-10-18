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

#if !defined(AFX_GUICONTAINER_H__0BD43E9C_E3DA_401D_9151_902189E2DB20__INCLUDED_)
#define AFX_GUICONTAINER_H__0BD43E9C_E3DA_401D_9151_902189E2DB20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuiContainer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuiContainer window

#include "GuiMiniTool.h"
#include "GuiDrawlayer.h"
#include "GuiImageLinkButton.h"
#include "GuiLabelButton.h"
#include "GuiLib.h" 
#include "GuiVisioFolder.h"
class CComponents
{

public:	
	CComponents(CGuiMiniTool* m_guiMini); 
	CComponents(CWnd* m_cwnd,BOOL bTypesm); 
public:
	CGuiMiniTool* m_guiMini;
	CWnd*		  m_cwnd;
	BOOL		  bMiniTool;
	BOOL		  bTypeSmall;
};



class GUILIBDLLEXPORT CGuiContainer : public CWnd
{
// Construction
public:
	enum Borders{STYLEFLAT=0,STYLE3D=1,STYLEPRESS=2};
	CGuiContainer();
// Attributes
public:
    BOOL AddComponen(CGuiMiniTool* m_miniTool);
	CWnd* AddComponen(CRuntimeClass* m_rtcwnd,UINT nID,DWORD dwStyle);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiContainer)
	protected:
//	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGuiContainer();
	void    SetBorder(Borders m_enborder=STYLEFLAT);
	void	SetColor(COLORREF m_clr){m_clrface=m_clr;};
	void    CalcAutoSize(CRect m_rc);
	virtual void	RecalLayout();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void	UsedAllWindow(BOOL bUseWindow); //se utiliza para que los objetos diferentes
											//a combobox y CGuiMiniTool utilicen todo el ancho de 
											//de a ventana.
	void	SetDrawBorder(BOOL bDraw=FALSE){m_bDrawBorder=bDraw;}    
	// Generated message map functions
protected:
	CPtrArray	m_arrContainer;
	int			m_nItems;
	COLORREF	m_clrface;
	Borders		m_enBorder;
	int			m_nResultCWnd;
    int			m_nResultTools; 	
	CWnd*		pParent;
	BOOL		m_bUseWin;
	BOOL		m_bDrawBorder;
protected:
	//{{AFX_MSG(CGuiContainer)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUICONTAINER_H__0BD43E9C_E3DA_401D_9151_902189E2DB20__INCLUDED_)
