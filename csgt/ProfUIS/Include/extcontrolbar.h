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

#if (!defined __EXT_CONTROLBAR_H)
#define __EXT_CONTROLBAR_H

//
// Many thanks to Paul DiLascia & Cristi Posea, their works
// were a powerful incentive to learn more about pretty
// complicated MFC docking windows mechanism
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// extcontrolbar.h : header file
//

#if (!defined __AFXTEMPL_H__)
	#include<AfxTempl.h>
#endif

#define __TB_SEPARATOR_WIDTH__ 6
#define __TB_TEXT_MARGINE__ 5
#define __TB_BUTTON_MARGINE__ 1 // 0
#define __TB_LINE_OFFSET 6 // 5

class CExtDockBar;
class CExtMiniDockFrameWnd;
class CExtPopupMenuWnd;
class CExtControlBar;
class CExtToolControlBar;
class CExtMenuControlBar;
class CExtBarButton;
class CExtBarContentExpandButton;
class CExtBarMdiDocButton;
class CExtControlBar;

typedef
	CArray < CExtControlBar*, CExtControlBar* >
	ExtControlBarVector_t;

typedef
	CArray < CControlBar*, CControlBar* >
	MfcControlBarVector_t;


/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButton

class CExtBarNcAreaButton : public CObject
{
protected:

	CExtControlBar * m_pBar;
	CRect m_rc;
	bool m_bHover:1, m_bPushed:1, m_bDisabled:1,
		m_bPaintingCombinedArea:1;

public:

	DECLARE_DYNAMIC( CExtBarNcAreaButton )

	CExtBarNcAreaButton(
		CExtControlBar * pBar
		);
	~CExtBarNcAreaButton();

	virtual void OnNcDestroy();

	operator CSize() const
	{
		ASSERT_VALID( this );
		return m_rc.Size();
	}
	operator const CRect &() const
	{
		ASSERT_VALID( this );
		return m_rc;
	}
	operator CRect &()
	{
		ASSERT_VALID( this );
		return m_rc;
	}

	bool IsBarFixedMode() const;
	bool IsBarVisible() const;
	bool IsBarFloated() const;
	bool IsBarDocked() const;
	bool IsBarDockedHorizontally() const;
	bool IsBarDockedVertically() const;
	bool IsBarDockedAtLeft() const;
	bool IsBarDockedAtRight() const;
	bool IsBarDockedAtTop() const;
	bool IsBarDockedAtBottom() const;
	bool GetBarNcAreaInfo( // returns false if no nc area on bar
		LPRECT pRectGripper = NULL, // in window coords
		LPRECT pRectWnd = NULL, // in screen coords
		bool * pbGripperAtTop = NULL,
		bool * pbTextOnGripper = NULL
		) const;
	bool IsBarWndActive() const;
	bool IsBarSingleOnRow() const;
	bool IsBarMinimizedOnRow() const;
	bool IsBarMaximizedOnRow() const;
	bool IsBarFirstOnRow( bool bExtBar ) const;
	bool IsBarLastOnRow( bool bExtBar ) const;
	CFrameWnd * GetDockingFrame();

	virtual void OnNcAreaReposition(
		CExtBarNcAreaButton * pPrevBtn
		);

	virtual bool OnQueryVisibility() const;
	
	void NcDrawDefault(
		CDC & dc,
		int nDockBtnGlyptT
		);

	virtual void OnNcAreaDraw( CDC & dc );

	virtual UINT OnNcAreaHitTest( CPoint point );
	virtual HCURSOR OnNcAreaQueryCursor( CPoint point );

	virtual bool OnNcAreaClicked( CPoint point );

#ifdef _DEBUG
	virtual void AssertValid() const
	{
		ASSERT( m_pBar != NULL );
		CObject::AssertValid();
	}
	
	virtual void Dump( CDumpContext & dc ) const
	{
		CObject::Dump( dc );
	}
#endif

	friend class CExtControlBar;
}; // class CExtBarNcAreaButton

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonExpand

class CExtBarNcAreaButtonExpand : public CExtBarNcAreaButton
{
public:

	DECLARE_DYNAMIC( CExtBarNcAreaButtonExpand )

	CExtBarNcAreaButtonExpand(
		CExtControlBar * pBar
		);

	virtual bool OnQueryVisibility() const;
	virtual void OnNcAreaDraw( CDC & dc );
	virtual UINT OnNcAreaHitTest( CPoint point );
	virtual bool OnNcAreaClicked( CPoint point );

}; // class CExtBarNcAreaButtonExpand

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonMenu

class CExtBarNcAreaButtonMenu : public CExtBarNcAreaButton
{
public:

	DECLARE_DYNAMIC( CExtBarNcAreaButtonMenu )

	CExtBarNcAreaButtonMenu(
		CExtControlBar * pBar
		);

	static void _CbPaintCombinedContent(
		LPVOID pCookie,
		CDC & dc,
		const CWnd & refWndMenu,
		const CRect & rcExcludeArea, // in screen coords
		int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
		);

	virtual bool OnQueryVisibility() const;
	virtual void OnNcAreaDraw( CDC & dc );
	virtual UINT OnNcAreaHitTest( CPoint point );
	virtual bool OnNcAreaClicked( CPoint point );

}; // class CExtBarNcAreaButtonMenu

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonClose

class CExtBarNcAreaButtonClose : public CExtBarNcAreaButton
{
public:

	DECLARE_DYNAMIC( CExtBarNcAreaButtonClose )

	CExtBarNcAreaButtonClose(
		CExtControlBar * pBar
		);

	virtual bool OnQueryVisibility() const;
	virtual void OnNcAreaDraw( CDC & dc );
	virtual UINT OnNcAreaHitTest( CPoint point );
	virtual bool OnNcAreaClicked( CPoint point );

}; // class CExtBarNcAreaButtonClose

/////////////////////////////////////////////////////////////////////////////
// CExtControlBar window

class __PROF_UIS_API CExtControlBar : public CControlBar
{
	struct __PROF_UIS_API InternalDockBarInfo_t
	{
		CMap < UINT, UINT, DWORD, DWORD > m_mapDockBarInfo;
		InternalDockBarInfo_t();
		DWORD operator[]( UINT nDockBarID );
		void ReplaceFrameDockBar(
			UINT nDockBarID,
			DWORD dwDockStyle,
			CFrameWnd * pFrame
			);
	}; // struct InternalDockBarInfo_t

	static InternalDockBarInfo_t g_DockBarInfo;

	class __PROF_UIS_API InternalFriendlyFrameWnd : public CFrameWnd
	{
	public:
		void SetupSmilyFrameWnd(void);
		void SetDockState( const CDockState & state );
		void GetDockState( CDockState & state ) const;
	};

// Construction
public:

	DECLARE_DYNAMIC( CExtControlBar )

	CExtControlBar();

// Attributes
public:
	bool m_bGripperStaticallyAtTop:1;
	bool m_bAllowContractExpand:1;
	bool m_bDoNotEraseClientBackground:1;
	static const UINT g_nMsgQueryIcon;
protected:
	bool m_bFixedMode:1;
	INT m_nGripHeightAtTop, m_nGripWidthAtLeft;
	INT m_nSeparatorHeight, m_nSeparatorWidth;
	INT m_nMinHW, m_nMinVH;
	INT m_nTrackerOffset, m_nMetricOffset;

private:
	static ExtControlBarVector_t g_AllBars;
	// sizes in docked/state
	CSize m_sizeDockedH, m_sizeDockedV, m_sizeFloated;
	CRect m_rcGrip,m_rcRowResize,m_rcRowRecalc,m_rcRowRecalcUp;
	bool m_bRowResizing:1;
	bool m_bRowRecalcing:1;
	bool m_bDragging:1;
	bool m_bEnableProfile:1; // enable/disable profile loading / saving
	bool m_bReposSingleChildMode:1;
	CPoint m_ptHelperDraggingOffset, m_ptFloatHelper;
	bool m_bWindowActive:1;
	bool m_bTopRecalcing:1;
	bool m_bNcAreaBtnTracking:1;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtControlBar)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtControlBar();

	void SetInitDesiredSizeVertical(CSize size_data)
	{
		m_sizeDockedV = size_data;
	};
	void SetInitDesiredSizeHorizontal(CSize size_data)
	{
		m_sizeDockedH = size_data;
	};
	void SetInitDesiredSizeFloating(CSize size_data)
	{
		m_sizeFloated = size_data;
	};

	void ProfileOperations_Enable(
		bool bEnable = true
		)
	{
		m_bEnableProfile = bEnable;
	};
	bool ProfileOperations_GetCheck()
	{
		return m_bEnableProfile;
	};

    virtual bool SerializeState(
		CArchive & ar
		);

	static bool ProfileBarStateLoad(
		CFrameWnd * pFrame,
		LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		LPWINDOWPLACEMENT pFrameWp = NULL // need serialize frame's WP
		);
	static bool ProfileBarStateSave(
		CFrameWnd * pFrame,
		LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		LPWINDOWPLACEMENT pFrameWp = NULL // need serialize frame's WP
		);
	static bool ProfileBarStateSerialize(
		CArchive & ar,
		CFrameWnd * pFrame,
		LPWINDOWPLACEMENT pFrameWp = NULL // need serialize frame's WP
		);

    virtual BOOL Create(
		LPCTSTR lpszWindowName,
		CWnd * pParentWnd,
        UINT nID = AFX_IDW_DIALOGBAR,
		DWORD dwStyle =
			WS_CHILD|WS_VISIBLE
			|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS
			|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
			|CBRS_HIDE_INPLACE
		);

	static bool IsOleIpObjActive(
		CFrameWnd * pFrameSearch = NULL
		);
	virtual bool SafeDisplayBar();

	static void FrameEnableDocking(
		CFrameWnd * pFrame,
		DWORD dwDockStyle = CBRS_ALIGN_ANY,
		bool bReplaceFloatingWnd = true
		);
	static void SetupFloatingFrameWnd(
		CFrameWnd * pFrame
		);

	bool IsSingleOnRow() const;
	void MakeSingleOnRow();

	bool IsMinimizedOnRow() const;
	bool IsMaximizedOnRow() const;
	void MaximizeOnRow();
	void MinimizeOnRow();

	BOOL IsFixedMode() const
	{
		return m_bFixedMode;
	};
	virtual BOOL IsNeedFocusOnShowControlBar() const
	{
		return !IsFixedMode();
	};
	UINT GetSafeDockBarDlgCtrlID() const;
	bool IsDockedAtTop() const;
	bool IsDockedAtBottom() const;
	bool IsDockedAtLeft() const;
	bool IsDockedAtRight() const;
	bool IsDockedHorizontally() const;
	bool IsDockedVertically() const;
	bool IsDocked() const;
	bool IsFloating() const;
	
	virtual bool IsBarWithGripper(
		bool * pbGripperAtTop = NULL,
		bool * pbTextOnGripper = NULL
		) const;
	
	virtual CSize CalcFixedLayout(
		BOOL bStretch,
		BOOL bHorz
		);
	virtual CSize CalcDynamicLayout(
		int nLength,
		DWORD nMode
		);

	virtual void DoPaint( CDC* pDC );
	virtual void OnUpdateCmdUI(
		CFrameWnd * pTarget,
		BOOL bDisableIfNoHndler
		);

private:
	
	class FriendlyDockBarHack;

	INT _CalcDesiredMinHW() const;
	INT _CalcDesiredMinVH() const;

	CSize _CalcDesiredLayout(
		BOOL bHorz,
		BOOL bForceFullRow = FALSE
		);

protected:
	enum eGFECB_t
	{
		GFECB_GET_ALL = 0L,
		GFECB_GET_DOCKED = 1L,
		GFECB_GET_FLOATING = 2L,
	};
    
	static int _GetDockedExtBars(
		ExtControlBarVector_t & vBars,
		CExtControlBar * pExcludeBar = NULL
		);

    static void _GetFrameControlBars(
		CFrameWnd * pFrame,
		ExtControlBarVector_t & vBars,
		eGFECB_t eCallOpt = GFECB_GET_ALL
		);
    void _GetFrameControlBars(
		ExtControlBarVector_t & vBars,
		eGFECB_t eCallOpt = GFECB_GET_ALL
		) const;

    void _GetRowBars(
		MfcControlBarVector_t & vBars
		) const;
	void _GetRowExtBars(
		ExtControlBarVector_t & vBars
		) const;
	bool _IsFirstBarInRow( bool bExtBar ) const;
	bool _IsLastBarInRow( bool bExtBar ) const;

    static CFrameWnd * _GetDockingFrameImpl(
		CControlBar * pBar
		);
    CFrameWnd * _GetDockingFrameImpl() const;

	virtual void _RecalcLayoutImpl();
	virtual void _RecalcNcArea();

public:
	virtual bool IsShowContentWhenDragging() const;
	virtual void OnRepositionSingleChild(
		int cx = -1, // if negative - get from client area
		int cy = -1,
		bool bRedraw = true
		);

protected:
	void _RowResizingStart();
	void _RowResizingStop();
	void _RowResizingUpdateState();

	void _RowRecalcingStart();
	void _RowRecalcingStop();
	void _RowRecalcingUpdateState();
	
	bool _RowRecalcing_IsBottomEnabled();
	bool _RowRecalcing_IsTopEnabled();
	
	void _DrawResizingTracker();
	void _DrawRecalcingTracker();

	static CExtControlBar * _DraggingGetBar(); // return currently dagging controlbar
	static bool _DraggingCancel(); // returns true if dragging was really canceled
    virtual void _DraggingStart(
		const CPoint & point,
		const CPoint & pointOffset = CPoint( 0, 0 );
		);
    virtual void _DraggingStop();
    virtual void _DraggingUpdateState(
		const CPoint & point
		);
	void _CompressRowSpace(
		int nSpaceNeeded,
		MfcControlBarVector_t & arrPrevBars
		);
	virtual void _OnStartDragging();

	void _SetCursor(
		const CPoint & point
		);

	void _ContextMenuTrack();

private:
	typedef
		CArray < CExtBarNcAreaButton *, CExtBarNcAreaButton * >
		NcButtonsArr_t;
	NcButtonsArr_t m_vNcButtons;

protected:
	UINT NcButtons_HitTest( CPoint point );
	void NcButtons_Paint( CDC & dc );

public:
	INT NcButtons_GetCount() const
	{
		ASSERT( this != NULL );
		return m_vNcButtons.GetSize();
	}

	CExtBarNcAreaButton * NcButtons_GetAt( INT nIndex )
	{
		ASSERT( this != NULL );
		ASSERT( nIndex >= 0 && nIndex < m_vNcButtons.GetSize() );
		CExtBarNcAreaButton * pBtn = 
			m_vNcButtons.GetAt( nIndex );
		ASSERT_VALID( pBtn );
		return pBtn;
	}
	
	const CExtBarNcAreaButton * NcButtons_GetAt( INT nIndex ) const
	{
		ASSERT( this != NULL );
		ASSERT( nIndex >= 0 && nIndex < m_vNcButtons.GetSize() );
		const CExtBarNcAreaButton * pBtn = 
			m_vNcButtons.GetAt( nIndex );
		ASSERT_VALID( pBtn );
		return pBtn;
	}
	
	void NcButtons_SetAt(
		INT nIndex,
		CExtBarNcAreaButton * pBtn
		)
	{
		ASSERT( this != NULL && pBtn != NULL );
		if( nIndex < 0 )
		{
			m_vNcButtons.Add( pBtn );
			return;
		}
		ASSERT_VALID( pBtn );
		ASSERT( nIndex >= 0 && nIndex <= m_vNcButtons.GetSize() );
		if( nIndex != m_vNcButtons.GetSize() )
		{
			CExtBarNcAreaButton * pBtnOld =
				m_vNcButtons.GetAt( nIndex );
			ASSERT_VALID( pBtnOld );
			if( pBtnOld != pBtn )
			{
				pBtnOld->OnNcDestroy();
				m_vNcButtons.SetAt( nIndex, pBtn );
			}
			return;
		} // if( nIndex != m_vNcButtons.GetSize() )
		m_vNcButtons.Add( pBtn );
	}
	
	void NcButtons_Add( CExtBarNcAreaButton * pBtn )
	{
		NcButtons_SetAt( -1, pBtn );
	}
	
	void NcButtons_RemoveAt( INT nIndex )
	{
		ASSERT( this != NULL );
		ASSERT( nIndex >= 0 && nIndex < m_vNcButtons.GetSize() );
		CExtBarNcAreaButton * pBtnOld =
			m_vNcButtons.GetAt( nIndex );
		ASSERT_VALID( pBtnOld );
		pBtnOld->OnNcDestroy();
	}
	
	void NcButtons_RemoveAll()
	{
		INT nCount = m_vNcButtons.GetSize();
		for( INT nIndex = 0; nIndex < nCount; nIndex++ )
		{
			CExtBarNcAreaButton * pBtnOld =
				m_vNcButtons.GetAt( nIndex );
			ASSERT_VALID( pBtnOld );
			pBtnOld->OnNcDestroy();
		}
		m_vNcButtons.RemoveAll();
		m_vNcButtons.FreeExtra();
	}

	virtual void OnNcAreaButtonsReinitialize();
	virtual void OnNcAreaButtonsReposition();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtControlBar)
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	friend class CExtBarButton;
	friend class CExtBarContentExpandButton;
	friend class CExtBarMdiDocButton;
	friend class CExtDockBar;
	friend class CExtToolControlBar;
	friend class CExtMenuControlBar;
	friend class CExtMiniDockFrameWnd;
	friend class CExtBarNcAreaButton;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXT_CONTROLBAR_H
