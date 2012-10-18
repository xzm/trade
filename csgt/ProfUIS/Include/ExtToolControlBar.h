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

#if (!defined __EXT_TOOLCONTROLBAR_H)
#define __EXT_TOOLCONTROLBAR_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __EXT_CONTROLBAR_H)
	#include <ExtControlBar.h>
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CExtBarButton

class __PROF_UIS_API CExtBarButton : public CObject
{
protected:
	UINT _GetTrackPopupFlags();

public:
	DECLARE_DYNAMIC(CExtBarButton)

protected:
	CExtToolControlBar * m_pBar;
	UINT	m_nCmdID; // Command ID or ID_SEPARATOR
	UINT	m_nStyle; // button styles
	
	CSize	m_ActiveSize;
	CRect	m_ActiveRect;
	
	BOOL	m_bWrap,
			m_bVisible,m_bHover,
			m_bCtrlAutoDestroyed,
			m_bVertDocked,
			m_bPopupMenu,
			m_bAutoDestroyMenu,
			m_bAppendMdiWindowsMenu
			;

	CWnd *	m_pCtrl;
	HMENU m_hMenu;

	void _InitMembers()
	{
		m_pBar = NULL;
		m_nCmdID = ID_SEPARATOR;
		m_pCtrl = NULL;
		m_bCtrlAutoDestroyed = FALSE;
		m_nStyle = 0;
		m_ActiveSize.cx = m_ActiveSize.cy = 0;
		m_ActiveRect.SetRectEmpty();
		m_bWrap = m_bHover = FALSE;
		m_bVisible = TRUE;
		m_bVertDocked = FALSE;
		m_hMenu = NULL;
		m_bPopupMenu = FALSE;
		m_bAutoDestroyMenu = FALSE;
		m_bAppendMdiWindowsMenu = FALSE;
	};

	void _UpdateCtrl()
	{
		if( m_pCtrl == NULL )
			return;
		ASSERT_VALID( m_pCtrl );
		m_pCtrl->ShowWindow(
			(m_bVisible && (!m_bVertDocked)) ? SW_SHOW : SW_HIDE
			);
		m_pCtrl->EnableWindow( IsEnabled() );
		if( m_pCtrl->IsKindOf(RUNTIME_CLASS(CButton)) )
		{
			int nCheck = 0;
			if( IsPressed() )
				nCheck = 1;
			else if( IsIndeterminate() )
				nCheck = 2;
			((CButton *)(m_pCtrl))->SetCheck( nCheck );
		}
//		if( m_bVisible && m_bHover )
//			m_pCtrl->SetFocus();
	};
	void _UpdateCmdIdFromStyle()
	{
		if( m_nStyle & TBBS_SEPARATOR )
		{
			m_nStyle &= ~(TBBS_BUTTON);
			m_nCmdID = ID_SEPARATOR;
		}
		else
		{
			m_nStyle |= TBBS_BUTTON;
		}
	};

	void _DestroyMenu()
	{
		if( m_hMenu == NULL )
			return;
		if( m_bAutoDestroyMenu )
		{
			ASSERT( ::IsMenu(m_hMenu) );
			VERIFY( ::DestroyMenu(m_hMenu) );
		}
		m_hMenu = NULL;
	}

public:
	CExtBarButton(
		CExtToolControlBar * pBar,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0
		)
	{
		_InitMembers();
		ASSERT( pBar != NULL );
		m_pBar = pBar;
		m_nStyle = nStyle;
		SetCmdID( nCmdID );
	};
	~CExtBarButton()
	{
		_DestroyMenu();
	};

	virtual UINT GetCmdID() const
	{
		return m_nCmdID;
	};
	virtual void SetCmdID( UINT nCmdID )
	{
		m_nCmdID = nCmdID;
		if( m_nCmdID == ID_SEPARATOR )
		{
			m_nStyle |= TBBS_SEPARATOR;
			m_nStyle &= ~(TBBS_BUTTON);
		}
		else
		{
			m_nStyle |= TBBS_BUTTON;
			m_nStyle &= ~(TBBS_SEPARATOR);
		}
	};

	virtual CWnd * CtrlGet()
	{
		return m_pCtrl;
	};
	virtual void CtrlSet(
		CWnd * pCtrl,
		BOOL bCtrlAutoDestroyed
		)
	{
		if( m_pCtrl != NULL )
		{
			if( ::IsWindow(m_pCtrl->GetSafeHwnd()) )
				m_pCtrl->DestroyWindow();
			if( !m_bCtrlAutoDestroyed )
				delete m_pCtrl;
		}
		m_pCtrl = pCtrl;
		m_bCtrlAutoDestroyed = bCtrlAutoDestroyed;
#ifdef _DEBUG
		if( m_pCtrl != NULL )
		{
			ASSERT_VALID( m_pCtrl );
		}
#endif // _DEBUG
	};

	virtual UINT GetStyle() const
	{
		return m_nStyle;
	};
	virtual void SetStyle( UINT nStyle )
	{
		m_nStyle = nStyle;
		_UpdateCmdIdFromStyle();
		_UpdateCtrl();
	};
	virtual void ModifyStyle(
		UINT nAdd,
		UINT nRemove = 0
		)
	{
		m_nStyle |= nAdd;
		m_nStyle &= ~nRemove;
		_UpdateCmdIdFromStyle();
		_UpdateCtrl();
	};

	virtual void SetVertDocked( BOOL bVertDocked )
	{
		m_bVertDocked = bVertDocked;
	}

	virtual BOOL IsWrap() const
	{
		return m_bWrap;
	};
	virtual void SetWrap( BOOL bWrap = TRUE )
	{
		m_bWrap = bWrap;
	};

	virtual BOOL IsSeparator() const
	{
		BOOL bSeparator =
			(GetStyle() & TBBS_SEPARATOR) ?
				TRUE : FALSE;
		return bSeparator;
	};
	virtual BOOL IsDisabled() const
	{
		BOOL bDisabled =
			(GetStyle() & TBBS_DISABLED) ?
				TRUE : FALSE;
		return bDisabled;
	};
	virtual BOOL IsEnabled() const
	{
		return !( IsDisabled() );
	};
	virtual BOOL IsIndeterminate() const
	{
		BOOL bIndeterminate =
			(GetStyle() & TBBS_INDETERMINATE) ?
				TRUE : FALSE;
		return bIndeterminate;
	};
	virtual BOOL IsPressed() const
	{
		BOOL bPressed =
			(GetStyle() & (TBBS_PRESSED|TBBS_CHECKED)) ?
				TRUE : FALSE;
		return bPressed;
	};

	virtual void Show( BOOL bShow = TRUE )
	{
		m_bVisible = bShow;
		_UpdateCtrl();
	};
	virtual BOOL IsVisible() const
	{
		return m_bVisible;
	};

	virtual void SetHover( BOOL bHover = TRUE )
	{
		m_bHover = bHover;
		_UpdateCtrl();
	};
	virtual BOOL IsHover() const
	{
		return m_bHover;
	};

	virtual CString GetText() const;
	
	virtual void SetRect( const RECT & rectButton )
	{
		m_ActiveRect = rectButton;
		if( (m_pCtrl != NULL) && (!m_bVertDocked) )
		{
			ASSERT_VALID( m_pCtrl );
			//m_pCtrl->MoveWindow( &m_ActiveRect,FALSE );
			CRect rcCtrl;
			m_pCtrl->GetWindowRect( &rcCtrl );
			CRect rcMove(
				m_ActiveRect.TopLeft(),
				rcCtrl.Size()
				);
			m_pCtrl->MoveWindow( &rcMove,FALSE );
		} // if( (m_pCtrl != NULL) && (!m_bVertDocked) )
		_UpdateCtrl();
	};
	virtual CRect Rect() const
	{
		return m_ActiveRect;
	};
	operator CRect() const
	{
		return Rect();
	};

	virtual CSize Size() const
	{
		return m_ActiveSize;
	};
	operator CSize() const
	{
		return m_ActiveSize;
	};

	virtual BOOL IsContainerOfPopupLikeMenu()
	{
		ASSERT( m_hMenu != NULL );
		ASSERT( ::IsMenu(m_hMenu) );
		return m_bPopupMenu;
	}
	virtual void SetMenu(
		HMENU hMenu,
		BOOL bPopupMenu,
		BOOL bAutoDestroyMenu
		);
	const HMENU GetMenu() const
	{
		return m_hMenu;
	};
	operator const HMENU() const
	{
		return GetMenu();
	};
	virtual HMENU GetMenu()
	{
		return m_hMenu;
	};
	operator HMENU()
	{
		return GetMenu();
	};
	virtual void SetAppendMdiWindowsMenu(
		BOOL bAppendMdiWindowsMenu
		)
	{
		m_bAppendMdiWindowsMenu = bAppendMdiWindowsMenu;
	};
	virtual BOOL IsAppendMdiWindowsMenu()
	{
		return m_bAppendMdiWindowsMenu;
	};

	virtual bool IsAbleToTrackMenu();

	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);

	virtual BOOL PutToPopupMenu(
		CExtPopupMenuWnd * pPopup
		);

	virtual HICON GetHICON();

	virtual CWnd * GetCmdTargetWnd();

	virtual void Paint(
		CDC & dc,
		bool bHorz
		);

	virtual void OnTrackPopup(
		CPoint point
		);
	virtual void OnHover(
		CPoint point,
		bool bOn
		);
	virtual void OnClick(
		CPoint point,
		bool bDown
		);

}; // class CExtBarButton


/////////////////////////////////////////////////////////////////////////////
// CExtBarContentExpandButton

typedef
	CArray < CExtBarButton *, CExtBarButton * >
	_tb_buttons_container_t;

#define __ID_TOOLBAR_RIGHT_BUTTON__ ((UINT)-66)
#define __RIGHT_BUTTON_HORZ_DX__ 14
#define __RIGHT_BUTTON_VERT_DY__ 12

class CExtPopupBaseWnd;
class CExtPopupMenuWnd;

class __PROF_UIS_API CExtBarContentExpandButton : public CExtBarButton
{
public:
	DECLARE_DYNAMIC(CExtBarContentExpandButton)

protected:
	bool m_bClearContent;

	_tb_buttons_container_t m_buttons;

public:
	CExtBarContentExpandButton(
		CExtToolControlBar * pBar
		) :
		CExtBarButton(pBar)
	{
		SetCmdID( __ID_TOOLBAR_RIGHT_BUTTON__ );
	};
	~CExtBarContentExpandButton()
	{
		CExtBarButton::~CExtBarButton();
	}

	bool IsContentClear()
	{
		ASSERT_VALID( this );
		return m_bClearContent;
	};
	void ClearContent( bool bClearContent = true )
	{
		if( this == NULL )
			return;
		ASSERT_VALID( this );
		m_bClearContent = bClearContent;
	};

	const _tb_buttons_container_t & GetButtons() const
	{
		return m_buttons;
	};
	_tb_buttons_container_t & GetButtons()
	{
		return m_buttons;
	};

	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);

	virtual BOOL PutToPopupMenu(
		CExtPopupMenuWnd * pPopup
		);

	virtual void Paint(
		CDC & dc,
		bool bHorz
		);

	virtual void OnTrackPopup(
		CPoint point
		);
	virtual void OnHover(
		CPoint point,
		bool bOn
		);
	virtual void OnClick(
		CPoint point,
		bool bDown
		);

}; // class CExtBarContentExpandButton

/////////////////////////////////////////////////////////////////////////////
// CExtToolControlBar

class __PROF_UIS_API CExtToolControlBar : public CExtControlBar
{
public:
	DECLARE_DYNAMIC(CExtToolControlBar)

private:
	friend class CExtBarButton;
	friend class CExtBarContentExpandButton;
	friend class CExtBarMdiDocButton;
	class CExtToolControlBarCmdUI;
	friend class CExtPopupBaseWnd;
	friend class CExtPopupMenuWnd;

public:
	bool IsRightExpandButton(int nBtnIdx);
protected:
	CExtBarContentExpandButton * m_pRightBtn;

	_tb_buttons_container_t m_buttons;
	
	void _RemoveAllButtonsImpl();
	int _GetButtonsCountImpl() const;

// Construction
public:
	CExtToolControlBar();
	void SetSizes(
		SIZE sizeTBB,
		SIZE sizeImage = CSize(0,0) // unsed in this version
		);
				// ORIGINALLY button size should be bigger than image

	BOOL LoadToolBar(
		LPCTSTR lpszResourceName
		);
	BOOL LoadToolBar(
		UINT nIDResource
		)
	{
		return
			LoadToolBar(
				MAKEINTRESOURCE(nIDResource)
				);
	};
	
	BOOL InsertButton(
		int nPos = -1, // append
		UINT nCmdID = ID_SEPARATOR,
		BOOL bDoRecalcLayout = TRUE
		);
	BOOL RemoveButton(
		int nPos,
		BOOL bDoRecalcLayout = TRUE
		);

	BOOL SetButtons(
		const UINT * lpIDArray = NULL,
		int nIDCount = 0
		); // lpIDArray can be NULL to allocate empty buttons

// Attributes
public: // standard control bar things
	int CommandToIndex( UINT nIDFind ) const;
	UINT GetButtonID( int nIndex ) const;
	virtual void GetButtonRect( int nIndex, LPRECT lpRect ) const;
	UINT GetButtonStyle( int nIndex ) const;
	void SetButtonStyle( int nIndex, UINT nStyle );

public:
	// for changing button info
	void GetButtonInfo(
		int nIndex,
		UINT & nID,
		UINT & nStyle
		) const;
	void SetButtonInfo(
		int nIndex,
		UINT nID,
		UINT nStyle
		);

	CWnd * GetButtonCtrl(
		int nIndex
		);
	BOOL SetButtonCtrl(
		int nIndex,
		CWnd * pCtrl = NULL,
		BOOL bCtrlAutoDestroyed = TRUE
		);

	int GetButtonByAccessKey( TCHAR vkTCHAR );

	HMENU GetButtonMenu(
		int nIndex
		);
	BOOL SetButtonMenu(
		int nIndex,
		HMENU hMenu = NULL,
		BOOL bPopupMenu = TRUE,
		BOOL bAutoDestroyMenu = TRUE,
		BOOL bDoRecalcLayout = TRUE
		);
	BOOL MarkButtonAsMdiWindowsMenu(
		int nIndex,
		BOOL bAppendMdiWindowsMenu = TRUE
		);
	virtual BOOL TrackButtonMenu(
		int nIndex
		);
	int GetVisibleButton(
		int nBtnIdx,
		BOOL bNext
		);

	int GetButtonsCount() const
	{
		return _GetButtonsCountImpl();
	};
	int GetMenuTrackingButton() const
	{
		return m_nBtnIdxMenuTracking;
	};

// Implementation
public:
	virtual ~CExtToolControlBar();
	virtual CSize CalcFixedLayout(
		BOOL bStretch,
		BOOL bHorz
		);
    virtual CSize CalcDynamicLayout(
		int nLength,
		DWORD dwMode
		);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CExtBarButton* _GetButtonPtr( int nIndex ) const;
	void _InvalidateButton( int nIndex );
	void _UpdateButton( int nIndex );

protected:
	bool _UpdateHoverButton(
		CPoint point = CPoint(-1,-1) // default is use ::GetCursorPos()
		);

	virtual DWORD RecalcDelayShow(
		AFX_SIZEPARENTPARAMS * lpLayout
		);

	virtual void _RecalcPositionsImpl();
	virtual void _RecalcLayoutImpl();

	// input CRect should be client rectangle size
	void _CalcInsideRect(
		CRect & rect,
		BOOL bHorz
		) const;
	
	CSize _CalcLayout(
		DWORD dwMode,
		int nLength = -1
		);
	CSize _CalcSize( BOOL bVerticallyDocked );
	void _SizeToolBar(
		int nLength,
		BOOL bVert = FALSE
		);
	int _WrapToolBar(
		int nWidth,
		int nHeight = 32767
		);
	CSize _GetButtonSize()
	{
		return m_sizeTBB;
	};
	int _GetIndexOf( CExtBarButton * pTBB )
	{
		if( pTBB == NULL )
			return -1;
		int nCountOfButtons = m_buttons.GetSize();
		for( int nBtnIdx=0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
		{
			ASSERT( m_buttons[nBtnIdx] != NULL );
			if( m_buttons[nBtnIdx] == pTBB )
				return nBtnIdx;
		}
		return -1;
	};
	void _SwitchMenuTrackingIndex(
		int iNewMenuTrackingIndex = -1
		);
	static CExtToolControlBar * _GetMenuTrackingBar();
	static void _CloseTrackingMenus();

	static void _CbPaintCombinedContent(
		LPVOID pCookie,
		CDC & dc,
		const CWnd & refWndMenu,
		const CRect & rcExcludeArea, // in screen coords
		int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
		);

	static bool g_bMenuTracking;
	static bool g_bMenuTrackingExpanded;
	int m_nBtnIdxMenuTracking;
	CSize m_sizeTBB;         // size of button
	int m_nBtnIdxCapture;       // index of button with capture (-1 => none)
	int m_nBtnIdxHover;

	virtual void DoPaint( CDC* pDC );
	virtual void OnUpdateCmdUI(
		CFrameWnd* pTarget,
		BOOL bDisableIfNoHndler
		);
	int _HitTestImpl(
		CPoint point,
		UINT nButtonStyleInclude = 0, // button must have style
		UINT nButtonStyleExclude = 0  // button must have not style
		) const;
	virtual int HitTest( CPoint point ) const;
	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI
		) const;

	//{{AFX_MSG(CExtToolControlBar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __EXT_TOOLCONTROLBAR_H

