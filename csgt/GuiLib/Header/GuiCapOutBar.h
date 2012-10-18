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


#if !defined(AFX_GUICAPOUTBAR_H__367A6805_A544_11D5_B916_000000000000__INCLUDED_)
#define AFX_GUICAPOUTBAR_H__367A6805_A544_11D5_B916_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AfxExt.h>
#include "GuiLib.h" 
#include "GuiMiniSplitter.h" 
#include "GuiMiniTool.h" 
class GUILIBDLLEXPORT GuiCapOutBar : public CControlBar  
{
public:
	//**************************************************
		GuiCapOutBar();
		virtual ~GuiCapOutBar();
	//**************************************************

public:

	//*********************************************************************
	BOOL	Create (DWORD dwStyle, CWnd* pParentWnd, UINT uID,int iMaxi=32);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	void	SetStyleFlag(BOOL m_bFlag);
	BOOL	SetSplitter(CGuiMiniSplitter* m_sppliter);
	BOOL	SetMiniTool(CGuiMiniTool* m_sppliter);
	void	RecalLayout();
	//**********************************************************************

protected:
	//***************************************
	int			m_iBorder;
	BOOL		m_bIsFlag;
	int			m_iHigh;
	UINT		m_uID;
	CGuiMiniSplitter*	m_miniSplitter;
	CGuiMiniTool*		m_MiniTool;
	//***************************************

protected:

	//{{AFX_MSG(GuiCapOutBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysColorChange();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	//}}AFX_MSG	

DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GUICAPOUTBAR_H__367A6805_A544_11D5_B916_000000000000__INCLUDED_)
