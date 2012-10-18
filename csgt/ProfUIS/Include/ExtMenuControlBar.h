//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///                                                                    ///
/// Professional User Interface Suite (TM)                             ///
/// Copyright (C) 2001-2002 by Foss Software Inc.                      ///
///                                                                    ///
/// http://www.fossware.com                                            ///
/// mailto:foss@fossware.com                                           ///
///                                                                    ///
///                                                                    ///
/// This software and accompanying documentation is the copyrighted    ///
/// work of Foss Software Inc.                                         ///
///                                                                    ///
/// Use of this software is governed by the following conditions:      ///
///                                                                    ///
///   a) this software is free for any kind of personal and            ///
///      commercial usage;                                             ///
///                                                                    ///
///   a) you may not reproduce, record, publish, publicly exhibit      ///
///      or distribute this source code without author's express       ///
///      prior written consent (foss@fossware.com);                    ///
///                                                                    ///
///   b) this notice should be remained intact in distributed source   ///
///      files;                                                        ///
///                                                                    ///
///   c) this software is subject to all Warranties and Disclaimers    ///
///      as set forth below;                                           ///
///                                                                    ///
/// Warranties and Disclaimers:                                        ///
///                                                                    ///
/// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND     ///
/// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,      ///
/// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.             ///
/// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,      ///
/// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES, ///
/// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA, ///
/// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE   ///
/// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                ///
///                                                                    ///
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#if (!defined __EXT_MENUCONTROLBAR_H)
#define __EXT_MENUCONTROLBAR_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
#endif

#if (!defined __EXT_HOOK_H)
	#include "../Src/ExtHook.h"
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CExtBarMdiDocButton

class __PROF_UIS_API CExtBarMdiDocButton : public CExtBarButton
{
public:
	DECLARE_DYNAMIC(CExtBarMdiDocButton)

protected:

	HWND _GetActiveMdiChildWnd(
		BOOL & bMaximized
		);

public:
	CExtBarMdiDocButton(
		CExtToolControlBar * pBar,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		) :
		CExtBarButton(
			pBar,
			nCmdID,
			nStyle
			)
	{
	};
	~CExtBarMdiDocButton()
	{
		CExtBarButton::~CExtBarButton();
	};

	virtual UINT GetStyle() const
	{
		return
			CExtBarButton::GetStyle()
			&
			(~TBBS_DISABLED)
			;
	};
	virtual void SetStyle( UINT nStyle )
	{
		nStyle &= ~TBBS_DISABLED;
		CExtBarButton::SetStyle( nStyle );
	};
	virtual void ModifyStyle(
		UINT nAdd,
		UINT nRemove = 0
		)
	{
		nAdd &= ~TBBS_DISABLED;
		nRemove |= TBBS_DISABLED;
		CExtBarButton::ModifyStyle(
			nAdd,
			nRemove
			);
	};

	virtual HICON GetHICON();

	virtual CWnd * GetCmdTargetWnd();

	virtual BOOL PutToPopupMenu(
		CExtPopupMenuWnd * pPopup
		);

	virtual void OnTrackPopup(
		CPoint point
		);
}; // class CExtBarMdiDocButton

/////////////////////////////////////////////////////////////////////////////
// CExtBarMdiRightButton

#define __GAP_BETWEEN_WND_RIGHT_BUTTONS 2

class CExtMenuControlBar;

class __PROF_UIS_API CExtBarMdiRightButton : public CExtBarMdiDocButton
{
	struct MdiMenuBarRightButtonsInfo_t
	{
		CWnd * m_pWndMdiChildFrame;
		CExtMenuControlBar * m_pBar;
		int m_cxIcon,m_cyIcon;
		CSize m_calcSize;
		bool
			m_bInitialized,
			m_bBtnClose,
			m_bBtnMaximize,
			m_bBtnMinimize,
			m_bBtnHelp;
		CRect
			m_rcBtnClose,
			m_rcBtnMaximize,
			m_rcBtnRestore,
			m_rcBtnMinimize,
			m_rcBtnHelp;
		MdiMenuBarRightButtonsInfo_t()
		{
			m_bInitialized
				= m_bBtnClose
				= m_bBtnMaximize
				= m_bBtnMinimize
				= m_bBtnHelp
				= false;
			m_pBar = NULL;
			m_pWndMdiChildFrame = NULL;
			m_cxIcon = ::GetSystemMetrics(SM_CXSIZE);
			m_cyIcon = ::GetSystemMetrics(SM_CYSIZE);
			ASSERT(
				m_cxIcon > __GAP_BETWEEN_WND_RIGHT_BUTTONS
				&&
				m_cyIcon > __GAP_BETWEEN_WND_RIGHT_BUTTONS
				);
			m_cxIcon -= __GAP_BETWEEN_WND_RIGHT_BUTTONS;
			m_cyIcon -= __GAP_BETWEEN_WND_RIGHT_BUTTONS;
			m_calcSize.cx = m_calcSize.cy = 0;
			EmptyLocations();
		};
		void EmptyLocations()
		{
			m_rcBtnClose.SetRectEmpty();
			m_rcBtnMaximize.SetRectEmpty();
			m_rcBtnRestore.SetRectEmpty();
			m_rcBtnMinimize.SetRectEmpty();
			m_rcBtnHelp.SetRectEmpty();
		};
		void InitLocations(
			const CRect & rcBtn,
			BOOL bHorz
			);
	};

	friend class CExtMenuControlBar;
	
public:
	DECLARE_DYNAMIC(CExtBarMdiRightButton)

public:
	CExtBarMdiRightButton(
		CExtToolControlBar * pBar,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		) :
		CExtBarMdiDocButton(
			pBar,
			nCmdID,
			nStyle
			)
	{
	};
	~CExtBarMdiRightButton()
	{
		CExtBarMdiDocButton::~CExtBarMdiDocButton();
	};

	virtual HICON GetHICON();

	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);

	virtual void Paint(
		CDC & dc,
		bool bHorz
		);

	virtual BOOL PutToPopupMenu(
		CExtPopupMenuWnd * pPopup
		);

	virtual void OnTrackPopup(
		CPoint point
		);
	virtual void OnClick(
		CPoint point,
		bool bDown
		);

}; // class CExtBarMdiRightButton

/////////////////////////////////////////////////////////////////////////////
// CExtMenuControlBar

class __PROF_UIS_API CExtMenuControlBar
	: public CExtToolControlBar
	, public CExtHookSink
{
	friend class CExtBarMdiDocButton;
	friend class CExtBarMdiRightButton;
	friend struct CExtBarMdiRightButton::MdiMenuBarRightButtonsInfo_t;

	void _GetMdiMenuBarRightButtonsInfo(
		CExtBarMdiRightButton::MdiMenuBarRightButtonsInfo_t & _info,
		CExtBarMdiRightButton * pTBB,
		BOOL bHorz
		);

	UINT m_nMdiDocButtonCmdID;
	HWND m_hWndHelperActiveChild;

	CString m_sMdiWindowPopupName;
	
	static UINT nMsgTrackButtonMenu;
	static UINT nMsgSetMenuNULL;

	BOOL m_bUpdateFileMruList,
		m_bFlatTracking,
		m_bSysMenuTracking
		;
	int m_nFlatTrackingIndex,m_nOldTrackingIndex;
	CMenu m_menu;

	void _UpdateFlatTracking(
		BOOL bRepaint = TRUE
		);
	CRect _GetMainFrameSysIconRect();
	CRect _GetChildFrameSysIconRect();

	virtual bool OnHookWndMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		UINT nMessage,
		WPARAM wParam,
		LPARAM lParam
		);

	HWND _GetHwndMainFrame();
	HWND _GetHwndChildFrame();
	HWND _GetHwndMdiArea();

	BOOL m_bMdiApp;
	BOOL _UpdateMenuBar(
		BOOL bDoRecalcLayout = TRUE
		);
	HMENU _SetMdiMenu(
		HMENU hNewMdiMenu //, HMENU hNewWindowMenu
		);
	HWND _GetActiveMdiChildWnd(
		BOOL & bMaximized
		);
	BOOL _InvokeParentTrackButtonMenu(
		int nIndex
		);
	BOOL _InstallMdiDocButtons(
		BOOL bDoRecalcLayout = TRUE
		);
	BOOL _SyncActiveMdiChild();
	/*static*/ BOOL _TrackFrameSystemMenu(
		CFrameWnd * pFrame,
		CPoint * pPoint = NULL, // NULL means calc meny track area automatically
		BOOL bSelectAny = FALSE,
		LPCRECT rcExcludeArea = NULL,
		UINT nTrackFlags = (UINT)(-1),
		BOOL bCombinedMode = FALSE
		);

public:
    DECLARE_DYNAMIC(CExtMenuControlBar);
// Construction
public:
    CExtMenuControlBar();

// Attributes
public:
	// hide expand button on menu bar if possible ?
	static bool g_bHideMenuBarExpandContentButton;
// Operations
public:

	CMenu & GetMenu(); // get current menu
	BOOL UpdateMenuBar( // update after menu changed
		BOOL bDoRecalcLayout = TRUE
		);

	void SetUpdateFileMruList(
		BOOL bUpdateFileMruList = TRUE
		)
	{
		m_bUpdateFileMruList = bUpdateFileMruList;
	};
	//m_sMdiWindowPopupName
	void SetMdiWindowPopupName(
		LPCTSTR sMdiWindowPopupName = NULL
		)
	{
		m_sMdiWindowPopupName =
			(sMdiWindowPopupName!=NULL) ?
				sMdiWindowPopupName : _T("");
		m_sMdiWindowPopupName.TrimLeft();
		m_sMdiWindowPopupName.TrimRight();
		while( m_sMdiWindowPopupName.Replace(_T("&"),_T("")) > 0 )
		{
			m_sMdiWindowPopupName.TrimLeft();
			m_sMdiWindowPopupName.TrimRight();
		}
	};

	// LoadMenuBar() required only for CFrameWnd
	// based windows principally without menu
	BOOL LoadMenuBar(
		UINT nResourceID
		);

	BOOL TranslateMainFrameMessage(MSG* pMsg);
	BOOL TrackMainFrameSystemMenu(
		CPoint * pPoint = NULL, // NULL means calc meny track area automatically
		BOOL bSelectAny = FALSE
		);
	BOOL TrackChildFrameSystemMenu(
		CPoint * pPoint = NULL, // NULL means calc meny track area automatically
		BOOL bSelectAny = FALSE
		);
	void _KillFrameMenu();

// Overridables
protected:
	virtual void OnUpdateCmdUI(
		CFrameWnd * pTarget,
		BOOL bDisableIfNoHndler
		);
	virtual void _OnStartDragging();
	virtual BOOL TrackButtonMenu(
		int nIndex
		);
	virtual void _RecalcPositionsImpl();

// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtMenuControlBar)
	//}}AFX_VIRTUAL
// Implementation
public:
    virtual ~CExtMenuControlBar();

	BOOL IsMenuBarTracking()
	{
		if(	m_bFlatTracking
			|| m_bSysMenuTracking
			)
			return TRUE;
		return FALSE;
	};

// Generated message map functions
protected:
    //{{AFX_MSG(CExtMenuControlBar)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	LRESULT OnTrackButtonMenu(WPARAM wp,LPARAM lp);
	LRESULT OnSetMenuNULL(WPARAM wp,LPARAM lp);
    DECLARE_MESSAGE_MAP()
}; // class CExtMenuControlBar

#endif // __EXT_MENUCONTROLBAR_H

