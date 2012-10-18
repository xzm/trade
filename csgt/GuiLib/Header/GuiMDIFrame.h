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

#if !defined(AFX_GUIFRAMEWND_H__B5DCA3BF_291F_4175_98E9_99A1D6E91C44__INCLUDED_)
#define AFX_GUIFRAMEWND_H__B5DCA3BF_291F_4175_98E9_99A1D6E91C44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CGuiMDIFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuiFrameWnd frame
#include "GuiDocbarExten.h"
#include "GuiDocSpecial.h"
#include "GuiLib.h"
#include "GuiControlBar.h"
#include "GuiDockToolbar.h"
#include "GuiMDITabbed.h"
#include "MenuBar.h"
#include "CoolMenu.h"
#include "GuiStatusBar.h"
#include "GuiToolBarWnd.h" 
#include "NewMenu.h"
class GUILIBDLLEXPORT CGuiMDIFrame : public CMDIFrameWnd
{
	DECLARE_DYNCREATE(CGuiMDIFrame)
protected:
	CGuiMDIFrame();           // protected constructor used by dynamic creation
	CString sProfile;
	CGuiMDITabbed m_MdiTabbed;
	CMenuBar m_wndMenuBar;
	CCoolMenuManager m_wndCool;	
	CGuiStatusBar m_wndStatusBar;
	CGuiToolBarWnd    m_wndToolBar;
	CNewMenu m_NewMenu;	
// Attributes
public:
	void LoadBars();
	BOOL VerifyBarState(LPCTSTR lpszProfileName);
// Operations
public:

	virtual void EnableDocking(DWORD dwDockStyle);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs, UINT nIconID);
	virtual BOOL DestroyWindow();
	DWORD CanDock(CRect rect, DWORD dwDockStyle,
		CDockBar** ppDockBar = NULL); // called by CDockContext
	BOOL InitMDITabbed();
	int InitStatusBar(UINT* indicators,int nSize);
	int InitToolBar(UINT uIDMenu);
	int InitMenu(UINT uIDMenu);
	void DockSpecialBars();
	void ShowHideBar(CGuiControlBar* pBar);
	void ShowHideBar(CControlBar* pBar);
protected:
	CGuiDocBarExten m_dockbar;
	CGuiDockToolBar m_dockToolbarLeft;
	CGuiDockToolBar m_dockToolbarTop;
	CGuiDockToolBar m_dockToolbarBottom;
	CGuiDockToolBar m_dockToolbarRight;
protected:
	virtual ~CGuiMDIFrame();

	// Generated message map functions
	//{{AFX_MSG(CGuiMDIFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIFRAMEWND_H__B5DCA3BF_291F_4175_98E9_99A1D6E91C44__INCLUDED_)
