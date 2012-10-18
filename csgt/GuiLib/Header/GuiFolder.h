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

#if !defined(AFX_GUIFOLDER_H__44CCC3EF_FC44_4877_94C7_61A7ACB0E216__INCLUDED_)
#define AFX_GUIFOLDER_H__44CCC3EF_FC44_4877_94C7_61A7ACB0E216__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//

/////////////////////////////////////////////////////////////////////////////
// CGuiFolder window
#include "Guilib.h"

//**********************************************************
class CFolderBar
{
public:
	CFolderBar(CWnd* m_wnd,LPCSTR lp_msg,UINT uImg=(UINT)-1);
public:
	CWnd*	m_Cwnd;
	LPCSTR	m_lpMsg;
	UINT	m_img;
	CRect	m_rect;
	BOOL	m_posUp;
		
};

//*************************************************************
class GUILIBDLLEXPORT CGuiFolder : public CWnd
{

DECLARE_DYNAMIC(CGuiFolder)
	// Construction
public:
	CGuiFolder();
	virtual ~CGuiFolder();
public:
	//*******************************
	enum Style{
			mNormal	=0,
			mPush	=1,
			mFlat	=2
	};
	enum TypeFolder{
		    mOutlook=0,
			mToolBox=1
	};

	//*******************************
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow(); 
public:
	void			AddFolder(CWnd* pParent,LPCSTR lpMsg, UINT uIMg=(UINT)-1);
	void			DeleteFolder(int m_iNumFolder);
	void			RecalLayout();
	virtual void	DrawFolder(CFolderBar* cfb,Style m_Style);
	int				PointInFolder(CPoint pt);
	void			EfectoScroll();
	void			RemoveAllFolder();
	void			TypeLook(TypeFolder tol);
protected:
	CPtrArray		m_ArrayFolder;
	CToolTipCtrl	m_ToolTip;
	int				m_NumFolders;   //total de folders
	int				m_numFolderDown;
	int				m_numFolderUp;   //numero de folders arriba
	CFont*			cfont;
	int				m_iSelected;
	int				m_ultimoFolderResal;
	int				m_iPosDown; //a partir de donde se ubica el anterior folder
	int			    m_iposUp;   //a partir de donde se ubica el siguiente folder
	int				m_itemp;
	int				m_AntFolder; //folder anterior
	TypeFolder		m_typel;	//tipo de look
	int				m_iWhatFolderIsDrawNow; //que folder estoy pintando
protected:
	//{{AFX_MSG(CFolder)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIFOLDER_H__44CCC3EF_FC44_4877_94C7_61A7ACB0E216__INCLUDED_)
