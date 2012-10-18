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
#include "afxwin.h"

#include "GuiDocbarExten.h"
#include "GuiDocSpecial.h"
#include "GuiLib.h"
#include "GuiControlBar.h"
#include "GuiDockToolbar.h"
#include "MenuBar.h"
#include "CoolMenu.h"
#include "GuiStatusBar.h"
#include "GuiToolBarWnd.h"
#include "NewMenu.h"
class GUILIBDLLEXPORT  CGuiFrameWnd : public CFrameWnd
{
		DECLARE_DYNCREATE(CGuiFrameWnd)
protected:
	CString sProfile;
	CGuiDocBarExten m_dockbar;
	CGuiDockToolBar m_dockToolbarLeft;
	CGuiDockToolBar m_dockToolbarTop;
	CGuiDockToolBar m_dockToolbarBottom;
	CGuiDockToolBar m_dockToolbarRight;
	CMenuBar m_wndMenuBar;
	CNewMenu m_NewMenu;	
	CCoolMenuManager m_wndCool;	
	CGuiStatusBar m_wndStatusBar;
	CGuiToolBarWnd    m_wndToolBar;
	BOOL			m_MdiTabbed;
public:
	void LoadBars();
	BOOL VerifyBarState(LPCTSTR lpszProfileName);
	void DockSpecialBars();
	int InitStatusBar(const UINT* lpIDArray,int nSize);
	int InitToolBar(UINT uIDMenu);
	int InitMenu(UINT uIDMenu);
	void ShowHideBar(CGuiControlBar* pBar);
	void ShowHideBar(CControlBar* pBar);
public:
	CGuiFrameWnd(void);
	virtual ~CGuiFrameWnd(void);
	virtual void EnableDocking(DWORD dwDockStyle);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs, UINT nIconID);
	virtual BOOL DestroyWindow();
	
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
