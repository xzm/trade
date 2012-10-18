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



#if !defined(AFX_GUITOOLBARWND_H__EA24A3ED_2C8D_401D_AA89_0515F80714FA__INCLUDED_)
#define AFX_GUITOOLBARWND_H__EA24A3ED_2C8D_401D_AA89_0515F80714FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "GuiLib.h"
#include "GuiDocbarExten.h"


class GUILIBDLLEXPORT CGuiToolBarWnd : public CToolBar  
{
	DECLARE_DYNAMIC(CGuiToolBarWnd)
public:
	
	enum StyleG{
		 Office97=0,
		 Office2000=1
		 };	
	//*******************************************************	
				 CGuiToolBarWnd();
	virtual		~CGuiToolBarWnd();
	//*******************************************************

public:
	
	//***********************************************************************
	void	DrawGripper(CWindowDC* dc,CRect* rcWin);
	void	setStyleG(StyleG style) {m_style=style;OnNcPaint();}
	//************************************************************************
	void	OnNcPaint();
	//************************************************************************
	BOOL	CreateCombo(CComboBox* pControl,UINT nID,int iSize,
				DWORD dwStyle=WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST | CBS_SORT | 
                    WS_VSCROLL | WS_TABSTOP);

	BOOL	CreateCombo(CComboBoxEx* pControl,UINT nID,
				int iSize,CImageList* m_imgList,
				DWORD dwStyle=WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST | CBS_SORT | 
                    WS_VSCROLL | WS_TABSTOP);
	
	//*************************************************************************
	void	AddRowComBoboxEx(CComboBoxEx* pControl,CString szMsg,int iItem,int iImage);
	void	SetButtonDropDown(UINT nID,int iImage,UINT nMENU);
	BOOL	OnDropDown(NMHDR* pNMHDR, LRESULT* pRes);
	void	SetXp(BOOL m_bXp);
	virtual void OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler );	
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
	void	DrawArrow(CDC* pDC,CRect m_rc);
	void	SetTextButton(int IdButton,CString sztexto);
	HMENU	LoadMenu(HMENU hmenu);
	void SetRealSize();
	CSize CalcLayout(DWORD nMode, int nLength = -1);
	CSize CalcSize(TBBUTTON* pData, int nCount);
	int WrapToolBar(TBBUTTON* pData, int nCount, int nWidth);
	void SizeToolBar(TBBUTTON* pData, int nCount, int nLength, BOOL bVert = FALSE);
	void _GetButton(int nIndex, TBBUTTON* pButton) const;
	void _SetButton(int nIndex, TBBUTTON* pButton);
	int GetLastPos();
	int GetFirstPos();
	CRect GetDockRect();
	int GetHiWid();
	BOOL IsLeft();
	BOOL IsRight();
	BOOL IsTop();
	BOOL IsBottom();
	BOOL IsVert();
	BOOL IsHorz();
	BOOL IsFloating();
	
protected:
	UINT			m_style;
	BOOL			m_pinta;
	int				m_iElements;
	BOOL			m_bIsXp;
	CPoint			mHeight;
	CPtrArray		m_pArray;	//submenus para botones dropdown
	CImageList		m_imgArrow; //imagen arrow
	int				m_iWidthDrowDown;
	int				m_iSelected;
	CFont			m_cfont;
	BOOL			bVertDocked;
	BOOL			bOver;
	BOOL			bPressDown;
	UINT			nDockBarAling;
	
protected:

	//{{AFX_MSG(CGuiToolBar)
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point) ;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point) ;
	afx_msg void OnNcLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange( );
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//}}AFX_MSG(CGuiToolBar)

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GUITOOLBARWND_H__EA24A3ED_2C8D_401D_AA89_0515F80714FA__INCLUDED_)
