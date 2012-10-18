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

#if (!defined __EXT_POPUP_MENU_WND_H)
#define __EXT_POPUP_MENU_WND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtPopupMenuWnd.h : header file
//

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __AFXTEMPL_H__)
	#include <AfxTempl.h>
#endif

#if (!defined __ExtCmdManager_H)
	#include <ExtCmdManager.h>
#endif

#define __BaseClassOfCExtPopupBaseWnd CWnd 

#include <afxpriv.h>

// MODIFIED BY SUNZHENYU, correct message
inline WPARAM CommandIDToStringID( UINT nCmdID )
{
	UINT nStringID = nCmdID;
	if (nCmdID >= 0xF000 && nCmdID < 0xF1F0) // max of 31 SC_s
	{
		// special strings table entries for system commands
		nStringID = ID_COMMAND_FROM_SC(nCmdID);
		ASSERT(nStringID >= AFX_IDS_SCFIRST &&
			nStringID < AFX_IDS_SCFIRST + 31);
	}
	else if (nCmdID >= AFX_IDM_FIRST_MDICHILD)
	{
		// all MDI Child windows map to the same help id
		nStringID = AFX_IDS_MDICHILD;
	}
	return (WPARAM)(nStringID);
}

/////////////////////////////////////////////////////////////////////////////
// CExtWndShadow

class __PROF_UIS_API CExtWndShadow
{
	CBitmap m_bmp0, m_bmp1;	// photo of right & bottom shadow parts to save/restore
	COLORREF * m_pHelperDibSurface;
	CRect m_rcWndArea;		// window are not including shadow
	CRect m_rc1stArea;		// basic area
	CRect m_rc2ndArea;		// combined area
	UINT m_nShadowSize;		// shadow size (in pixels)
	UINT m_nBr0,m_nBr1;		// min/max brightness percents [0..100]
	bool m_bEnablePhotos;   // use photo bitmaps
	void _FreeWinObjects();
	void _DoPixelOvershadow(
		int nMakeSpec,
		int nPosX,
		int nPosY
		);
	bool _MakePhotos( CDC & dc, CDC & dcmm );
	bool _PaintLo( CDC & dc );
	bool _PaintHi( CDC & dc );
public:
	enum eShadowOptions
	{
		DEF_BRIGHTNESS_MIN =  70UL, // default min brightness percent
		DEF_BRIGHTNESS_MAX = 100UL, // default max brightness percent
		DEF_SHADOW_SIZE    =   4UL, // default shadow size (in pixels)
	};

	CExtWndShadow();
	~CExtWndShadow();
	
	bool Restore( CDC & dc );
	bool Paint( CDC & dc );
	bool Paint(
		CDC & dc,
		const CRect & rcWndArea,
		const CRect & rc1stArea = CRect(0,0,0,0),
		const CRect & rc2ndArea = CRect(0,0,0,0),
		UINT nShadowSize = DEF_SHADOW_SIZE,
		UINT nBr0 = DEF_BRIGHTNESS_MIN,
		UINT nBr1 = DEF_BRIGHTNESS_MAX,
		bool bEnablePhotos = true
		);
	void Destroy();
}; // class CExtWndShadow


/////////////////////////////////////////////////////////////////////////////
// CExtPopupBaseWnd

class __PROF_UIS_API CExtPopupBaseWnd :
	public __BaseClassOfCExtPopupBaseWnd
{
public:
	DECLARE_DYNCREATE(CExtPopupBaseWnd)

	CExtPopupBaseWnd();

	virtual BOOL DestroyWindow();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtPopupBaseWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtPopupBaseWnd();

	static UINT nMsgPopupNext;
	static UINT nMsgPopupPrev;
	static UINT nMsgNotifyMenuExpanded;
	//static UINT nMsgNotifyMenuClosed;

	enum e_combine_align_t // relatively to menu window rect
	{
		__CMBA_NONE = 0,
		__CMBA_TOP = 1,
		__CMBA_BOTTOM = 2,
		__CMBA_RIGHT = 3,
		__CMBA_LEFT = 4,
	};

	typedef
		void (*pCbPaintCombinedContent)(
			LPVOID pCookie,
			CDC & dc,
			const CWnd & refWndMenu,
			const CRect & rcExcludeArea, // in screen coords
			int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
			);

	enum e_animation_type_t
	{
		__AT_NONE = 0,
		__AT_RANDOM = 1,
		__AT_ROLL = 2,
		__AT_SLIDE = 3,
		__AT_FADE = 4,
		__AT_ROLL_AND_SREETCH = 5,
		__AT_SLIDE_AND_SREETCH = 6,
		__AT_NOISE = 7,
		__AT_BOXES = 8,
		__AT_CIRCLES = 9,
		__AT_HOLES = 10,
		
		__AT_MAX_VALID_VALUE = 10, // used for random effect selection
		__AT_CONTENT_DISPLAY = -1, // internal helper - delayed display
		__AT_CONTENT_EXPAND = -2 // internal helper - funny expand/stretch rarely used items (office XP like)
	};
	static e_animation_type_t g_DefAnimationType;

	static bool g_bUseDesktopWorkArea; // align to desktop work area (false - to screen area)
	virtual CRect _CalcDesktopRect();

protected:
	CRect m_rcExcludeArea;
	CPoint m_ptTrack;
	bool m_bExcludeAreaSpec,m_bCombineWithEA;

	LPVOID m_pCbPaintCombinedCookie;
	pCbPaintCombinedContent m_pCbPaintCombinedContent;

	e_animation_type_t m_AnimationType;

	COLORREF * m_clrFadeBitsSrc;
	COLORREF * m_clrFadeBitsDst;
	COLORREF * m_clrFadeBitsTmp;
	int m_nAnimPercent,m_nAnimPercentOld;
	bool m_bAnimFinished;

	CSize m_sizeFullItems;

	CExtWndShadow m_ShadowMain,m_ShadowCMBA;
	CBitmap
		m_bmpScreenSrc,m_bmpScreenDst,m_bmpScreenTmp;
		//m_bmpSaveBits;

//	void _BitsSave( CDC &dc );
//	void _BitsRestore();

	virtual void _InitAnimation();
	virtual void _StartAnimation();

	void _SurfacesDelete()
	{
		if( m_bmpScreenDst.GetSafeHandle() != NULL )
		{
			VERIFY(
				m_bmpScreenDst.DeleteObject()
				);
		}
		if( m_bmpScreenSrc.GetSafeHandle() != NULL )
		{
			VERIFY(
				m_bmpScreenSrc.DeleteObject()
				);
		}
		if( m_bmpScreenTmp.GetSafeHandle() != NULL )
		{
			VERIFY(
				m_bmpScreenTmp.DeleteObject()
				);
		}
		m_clrFadeBitsSrc
			= m_clrFadeBitsDst
			= m_clrFadeBitsTmp
			= NULL;
	};
	
	virtual void _EndAnimation()
	{
		_SurfacesDelete();
	};

	virtual void _DrawAnimatedState( CDC & dc );
	virtual void _DoPaint( CDC & dcPaint, bool bUseBackBuffer = true );

	virtual void _GetClientRect(RECT * pRectClient)
	{
		ASSERT( GetSafeHwnd() != NULL );
		ASSERT( pRectClient != NULL );
		GetClientRect(pRectClient);
	};
	void _GetClientRect(RECT & rectClient)
	{
		_GetClientRect( &rectClient );
	};

	// Generated message map functions
	//{{AFX_MSG(CExtPopupMenuWnd)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CExtPopupBaseWnd

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuTipWnd

class __PROF_UIS_API CExtPopupMenuTipWnd :
	public CExtPopupBaseWnd
{
	CSize m_IconSize;
	HICON m_hIcon;
	CString m_sText;
	HRGN m_hRgn;
	bool m_bFlipHorz,m_bFlipVert;

public:
	DECLARE_DYNCREATE(CExtPopupMenuTipWnd)


	CExtPopupMenuTipWnd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtPopupMenuTipWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtPopupMenuTipWnd();

	void SetIcon(HICON hIcon);
	void SetText(LPCTSTR lpszText);

	bool Show(
		CWnd * pWndParent,
		const RECT & rcExcludeArea
		);
	void Hide();

protected:
	bool GetWindowRegion(
		CDC* pDC,
		HRGN* hRegion,
		CSize *Size = NULL
		);
	void _DoPaint( CDC & dcPaint, bool bUseBackBuffer = true );

	// Generated message map functions
	//{{AFX_MSG(CExtPopupMenuTipWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CExtPopupMenuTipWnd

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuWnd

#define TPMX_LEFTALIGN       0x0001L
#define TPMX_CENTERALIGN     0x0002L
#define TPMX_RIGHTALIGN      0x0003L
#define TPMX_TOPALIGN        0x0004L
#define TPMX_VCENTERALIGN    0x0005L
#define TPMX_BOTTOMALIGN     0x0006L

#define TPMX_ALIGN_MASK      0x000FL

class __PROF_UIS_API CExtPopupMenuWnd :
	public CExtPopupBaseWnd
{
	friend class CExtControlBar;
	friend class CExtToolControlBar;
	friend class CExtMenuControlBar;
	friend class CExtInternalSmilyFrameWnd;

	class __PROF_UIS_API InternalFriendlyWinApp : CWinApp
	{
	public:
		inline static InternalFriendlyWinApp * _GetFriendlyApp()
		{
			CWinApp * pApp = ::AfxGetApp();
			ASSERT( pApp != NULL );
			return (InternalFriendlyWinApp *)pApp;
		};
		CRecentFileList * _GetRecentFileList()
		{
			ASSERT( m_pRecentFileList != NULL );
			return m_pRecentFileList;
		};
	}; // class InternalFriendlyWinApp

	// hooks
	static LRESULT CALLBACK _HookMouseProc(
		int nCode,      // hook code
		WPARAM wParam,  // message identifier
		LPARAM lParam   // mouse coordinates
		);
	static LRESULT CALLBACK _HookKeyboardProc(
		int nCode,      // hook code
		WPARAM wParam,  // virtual-key code
		LPARAM lParam   // keystroke-message information
		);

	static HHOOK g_hMouseHook;
	static HHOOK g_hKeyboardHook;
	
	static void _Hook( bool bHook = true );

public:

	// is allowed menu item positioning without
	// using only & - marked text
	static bool g_bAllowNonAccelPositioning;

	class CExtPopupMenuWndAutoPtr;
	friend class CExtPopupMenuWndAutoPtr;

	class __PROF_UIS_API CExtPopupMenuWndAutoPtr
	{
		CExtPopupMenuWnd * m_pTopMenu;

		void _Done()
		{
			CExtPopupMenuWnd::_Hook( false );

			if( m_pTopMenu != NULL )
			{
				//ASSERT( m_pTopMenu->m_bTopLevel );
				VERIFY( m_pTopMenu->_BuildItems(NULL,NULL,true) );
				delete m_pTopMenu;
				m_pTopMenu = NULL;
			}
		};

	public:
		
		CExtPopupMenuWndAutoPtr()
		{
			m_pTopMenu = NULL;

		};
		
		~CExtPopupMenuWndAutoPtr()
		{
			_Done();
		};

		bool IsEmpty()
		{
			return (m_pTopMenu == NULL) ? true : false;
		};

		void SetInstance( CExtPopupMenuWnd * pTopMenu = NULL )
		{
			//_Done();
			m_pTopMenu = pTopMenu;
		};
		CExtPopupMenuWnd * GetInstance()
		{
			return m_pTopMenu;
		};
		void DoneInstance()
		{
			_Done();
			CExtPopupMenuWnd::_PassMsgLoop();
		};
		
		void operator=( CExtPopupMenuWnd * pTopMenu )
		{
			SetInstance(pTopMenu);
		};

		CExtPopupMenuWnd * operator->()
		{
			ASSERT( m_pTopMenu != NULL );
			return m_pTopMenu;
		};

		bool IsTopPupup(
			CExtPopupMenuWnd * pTopMenu
			)
		{
			return (pTopMenu==m_pTopMenu) ? true : false;
		};

	}; // class CExtPopupMenuWndAutoPtr

public:
	DECLARE_DYNCREATE(CExtPopupMenuWnd)

	CExtPopupMenuWnd();

// Attributes
public:

	virtual int _GetCurIndex() const;

protected:
	CExtPopupMenuTipWnd m_wndToolTip;

	int m_nCurIndex,m_nDelayedFocusItemIndex;
	bool
		m_bTopLevel,
		m_bScrollingAvailable,
		m_bExpandAvailable,
		m_bExpandWasPressed
		;
	UINT m_nWaitingExpandTickCount;
	int m_nTrackFlags;

	static CExtPopupMenuWnd * g_pWndCapture;

	static HWND g_hWndCmdReciever;

	//CPoint m_ptTrack;
	int m_nDyScrollOffset;
	CExtPopupMenuWnd * m_pWndParentMenu;

	CRect
		m_rcScrollTop,
		m_rcScrollBottom,
		m_rcExpandBtn;
	int m_nScrollingDirection;

protected:
	enum spec_buttons_index_t
	{
		IDX_NOTHING = -1,
		IDX_SCROLL_TOP = -2,
		IDX_SCROLL_BOTTOM = -3,
		IDX_EXPAND = -4
	};
// Operations
public:
	/*
	BOOL LoadMenu( 
		LPCTSTR lpszResourceName
		)
	{
		return LoadMenu( 
			MAKEINTRESOURCE(lpszResourceName)
			);
	};
	*/

	virtual BOOL LoadMenu( 
		HWND hWndCmdProfileInfo,
		UINT nIDResource,
		bool bPopupMenu = true
		);
	virtual HMENU ExportToMenu(
		BOOL bDeep = TRUE
		) const;
	virtual BOOL UpdateFromMenu( 
		HWND hWndCmdProfileInfo,
		CMenu *pBuildMenu,
		bool bPopupMenu = true,
		bool bTopLevel = true
		);
	virtual BOOL UpdateMdiWindowsMenu(
		CWnd * pWndStartSearchMdiFrameWnd = NULL
		);
	virtual void SetCmdTargetToAllItems(
		HWND hWndSpecCmdReciever = NULL,
		bool bOnlyThisLevel = false
		);

	enum menu_item_type_t
	{
		TYPE_POPUP		= 0xFFFFFFFF,
		TYPE_SEPARATOR	= 0x00000000 // same as ID_SEPARATOR
	};

	virtual INT ItemGetCount() const;
	virtual UINT ItemGetCmdID( // menu_item_type_t values can be returned
		INT nPos
		) const;
	virtual HICON ItemGetIcon( INT nPos ) const;
	virtual CString ItemGetText( INT nPos ) const;
	virtual CString ItemGetAccelText( INT nPos ) const;
	virtual INT ItemFindPosForCmdID(
		UINT nCmdID,
		INT nPosStart = -1
		) const;
	virtual bool ItemSetPopupIcon(
		INT nPos,
		HICON hIcon = NULL // no icon by default
		);
	virtual bool ItemSetPopupText(
		INT nPos,
		LPCTSTR sText // NULL if empty
		);
	virtual bool ItemSetPopupAccelText(
		INT nPos,
		LPCTSTR sText // NULL if empty
		);
	virtual CExtPopupMenuWnd * ItemGetPopup(
		INT nPos
		);
	virtual const CExtPopupMenuWnd * ItemGetPopup(
		INT nPos
		) const;
	virtual BOOL ItemRemove(
		INT nPos = -1 // remove all
		);
	virtual BOOL ItemInsert(
		UINT nCmdID = TYPE_SEPARATOR,
		INT nPos = -1, // append
		LPCTSTR sText = NULL, // for TYPE_POPUP only
		HICON hIcon = NULL, // for TYPE_POPUP only
		HWND hWndSpecCmdReciever = NULL // specific command reciever
		);
	virtual BOOL ItemInsertSpecPopup( // insert specific popup implementation
		CExtPopupMenuWnd * pSpecPopup,
		INT nPos = -1, // append
		LPCTSTR sText = NULL,
		HICON hIcon = NULL
		);
	virtual INT ItemFindByAccessChar(
		TCHAR chrAccess,
		INT nStartIdx = -1,
		BOOL bRestartAt0 = TRUE
		) const;

	virtual bool IsAllItemsRarelyUsed() const;
	virtual BOOL CreatePopupMenu( HWND hWndCmdRecv ); // alternative to LoadMenu()

	static CExtPopupMenuWnd * GetTrackingMenu();
	static void CancelMenuTracking();
	inline static BOOL IsKeyPressed(
		int nVirtKey
		)
	{
		SHORT nKeyState =
			::GetKeyState( nVirtKey );
		if( (nKeyState & (1 << (sizeof(SHORT)*8-1))) != 0 )
			return TRUE;
		return FALSE;
	};
	static BOOL IsCmdKeyActivation(
		LPARAM lParam = 0
		);
	static BOOL IsMenuTracking();

	enum e_combine_exclude_area_t
	{
		__COMBINE_NONE = 0,
		__COMBINE_ANY_SUITABLE = 1,
		__COMBINE_DEFAULT = 2,
	};

	virtual BOOL TrackPopupMenu(
		UINT nFlags,
		int x,
		int y,
		HWND hWndCmdReciever = AfxGetMainWnd()->GetSafeHwnd(),
		LPCRECT lpRect = 0,
		bool bSelectAny = false,
		bool bPerformMessageLoop = false,
		bool bForceExpandRarelyUsed = false,
		e_combine_exclude_area_t eCombineExcludeAreaMode = __COMBINE_DEFAULT,
		LPVOID pCbPaintCombinedCookie = NULL,
		pCbPaintCombinedContent pCbPaintCombinedContent = NULL
		);

	static HWND GetCmdTargetWnd(
		BOOL bValidate = TRUE
		);

	static UINT g_nMsgPrepareMenu; // prepare window popup messgae

	struct MsgPrepareMenuData_t
	{
		bool m_bMenuChanged;
		bool m_bMenuCanceled;
		CExtPopupMenuWnd * m_pPopup;

		MsgPrepareMenuData_t(
			CExtPopupMenuWnd * pPopup
			)
			: m_bMenuChanged( false )
			, m_bMenuCanceled( false )
			, m_pPopup( pPopup )
		{
			ASSERT( m_pPopup != NULL );
		};
		~MsgPrepareMenuData_t()
		{
		};

		operator WPARAM() const
		{
			return WPARAM( this );
		};

		LRESULT SendMessage( HWND hWnd )
		{
			ASSERT( m_pPopup != NULL );

			ASSERT( hWnd != NULL );
			ASSERT( ::IsWindow(hWnd) );
			LRESULT lResult =
				::SendMessage(
					hWnd,
					CExtPopupMenuWnd::g_nMsgPrepareMenu,
					WPARAM(*this),
					0
					);
			
			return lResult;
		};
		LRESULT SendMessage( CWnd * pWnd )
		{
			return SendMessage( pWnd->GetSafeHwnd() );
		};
	}; // struct MsgPrepareMenuData_t

	static bool g_bMenuWithShadows; // allow shadows
	static bool g_bMenuExpanding; // allow hide rarely used
	static bool g_bMenuHighlightRarely; // display rarely used in different style
	static bool g_bMenuShowCoolTips; // allow display cool tooltips
	static bool g_bMenuExpandAnimation; // allow animation when show rarely used
protected:
	virtual bool _CreateHelper(
		CWnd * pWndCmdReciever
		);
	bool _BuildItems(
		HWND hWndCmdProfileInfo,
		CMenu * pBuildMenu,
		bool bTopLevel
		);
	void _UpdateCmdUI();
	void _Init();
	void _RecalcLayoutImpl();
	static int _GetSpecBtnHeight();
	virtual int _HitTest(const CPoint & point );
	virtual void _DrawAnimatedState( CDC & dc );
	virtual void _DoPaint( CDC & dcPaint, bool bUseBackBuffer = true );
	virtual void _GetClientRect(RECT * pRectClient);
	void _EndSequence(
		UINT nCmdID = 0,
		HWND hWndCmdReciever = NULL
		);
	static void _PassMsgLoop();

	struct __PROF_UIS_API expand_effect_rects_t
	{
		CRect m_rcSrc,m_rcDst;
		expand_effect_rects_t()
		{
			m_rcSrc.SetRectEmpty();
			m_rcDst.SetRectEmpty();
		};
		expand_effect_rects_t(
			const RECT & rcSrc,
			const RECT & rcDst
			)
		{
			m_rcSrc = rcSrc;
			m_rcDst = rcDst;
		};
		expand_effect_rects_t(
			const expand_effect_rects_t & other
			)
		{
			m_rcSrc = other.m_rcSrc;
			m_rcDst = other.m_rcDst;
		};
		expand_effect_rects_t &
			operator= (
				const expand_effect_rects_t & other
				)
		{
			m_rcSrc = other.m_rcSrc;
			m_rcDst = other.m_rcDst;
			return * this;
		};
	}; // struct expand_effect_rects_t

	typedef
		CArray < expand_effect_rects_t, expand_effect_rects_t & >
		expand_effect_rects_container_t;

	struct __PROF_UIS_API visible_item_def_t
	{
		int m_nIndex;
		bool m_bRarelyUsed;
		bool m_bRarelyUsedPrev;
		bool m_bRarelyUsedNext;
		bool m_bHelperIsSeparator;
		bool m_bHelperIsPopup;
		UINT m_nHelperCmdID;
		CRect m_rcItem;
		visible_item_def_t()
		{
			m_nIndex = -1;
			m_bRarelyUsed
				= m_bRarelyUsedPrev
				= m_bRarelyUsedNext
				= m_bHelperIsSeparator
				= m_bHelperIsPopup
				= false;
			m_nHelperCmdID = ID_SEPARATOR;
			m_rcItem.SetRectEmpty();
		};
		visible_item_def_t(
			const visible_item_def_t & other
			)
		{
			AssignFromOther(other);
		};
		~visible_item_def_t()
		{
		};
		visible_item_def_t & operator=(
			const visible_item_def_t & other
			)
		{
			AssignFromOther(other);
			return * this;
		};
		void AssignFromOther(
			const visible_item_def_t & other
			)
		{
			m_nIndex = other.m_nIndex;
			m_bRarelyUsed = other.m_bRarelyUsed;
			m_bRarelyUsedPrev = other.m_bRarelyUsedPrev;
			m_bRarelyUsedNext = other.m_bRarelyUsedNext;
			m_bHelperIsSeparator = other.m_bHelperIsSeparator;
			m_bHelperIsPopup = other.m_bHelperIsPopup;
			m_nHelperCmdID = other.m_nHelperCmdID;
			m_rcItem = other.m_rcItem;
		};
		operator int() const
		{
			return m_nIndex;
		};
	};

	typedef
		CArray < visible_item_def_t, visible_item_def_t & >
		visible_items_t;
	void _GetVisibleItems(
		CDC & dc,
		visible_items_t & v
		);

	BOOL _TrackPopupMenu(
		UINT nFlags,
		int x,
		int y,
		LPCRECT lpRect,
		bool bSelectAny = false,
		bool bForceExpandRarelyUsed = false,
		e_combine_exclude_area_t eCombineExcludeAreaMode = __COMBINE_DEFAULT,
		LPVOID pCbPaintCombinedCookie = NULL,
		pCbPaintCombinedContent pCbPaintCombinedContent = NULL
		);
public:
	void _ItemFocusCancel( BOOL bRepaint );
	void _ItemFocusDelay( int nItemIndex = IDX_NOTHING );
	void _ItemFocusSet(
		int nCurIndex,
		BOOL bEnableDropChild,
		BOOL bRepaint
		);
	void _GetItemRect(
		int nIndex,
		RECT & rectItem
		);
public:
	virtual bool _OnMouseMove(UINT nFlags, CPoint point);
	virtual bool _OnMouseClick(UINT nFlags, CPoint point);
	virtual bool _OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	virtual void _FreeWinObjects();
	virtual void _OnCancelMode();


	virtual void _SetCapture();
	static CExtPopupMenuWnd * _GetCapture();
	virtual void _ReleaseCapture();

	int _GetMaxScrollPos();
	bool _StartScrolling(int nButtonIndex);

	virtual void _SyncItems();
	void _DoExpand();
	bool _CoolTipIsVisible();
	void _CoolTipHide(
		bool bAdvOperation = true
		);

	e_combine_align_t m_eCombineAlign;
	CRgn m_rgnWnd;
	CRect m_rcClient;

	virtual CRect _CalcTrackRect();
	virtual CSize _CalcTrackSize();

	enum next_item_t
	{
		__NI_NOTHING,
		__NI_ANY,
		__NI_NEXT,
		__NI_PREV,
		__NI_PAGE_UP,
		__NI_PAGE_DOWN
	};
	
	virtual int _GetNextItem(
		next_item_t nit
		);

	/////////////////////////////////////////////////////////////////////////////
	// CExtPopupMenuWnd::menu_item_info_t
	class menu_item_info_t
	{
		CExtPopupMenuWnd * m_pWndChild;
		int m_nItemIndex;
		
		UINT m_nCmdID; // can be ID_SEPARATOR
		CString m_sPopupText,m_sPopupAccelText;
		HICON m_hIconPopup;

		CSize m_size;
		bool m_bSelected,m_bDisplayed;
		TCHAR m_cAccelChar;

		HWND m_hWndSpecCmdReciever;

	public:
		menu_item_info_t();
		~menu_item_info_t();

		bool UpdateFromMenu(
			HWND hWndCmdProfileInfo,
			CMenu * pTrackMenu,
			int nItemIndex
			);

		void AccelCharInit();
		TCHAR AccelCharGet() const;
		bool AccelCharIsSet() const;

		bool UpdateCmdManagerCommand(
			CExtCmdManager::cmd_t * p_cmd,
			int nItemIndex
			);

		void MeasureItem();

		CExtCmdManager::cmd_t * GetCmd() const;

		HWND GetCmdReciever() const
		{
			if( m_hWndSpecCmdReciever != NULL )
			{
				ASSERT( ::IsWindow(m_hWndSpecCmdReciever) );
				return m_hWndSpecCmdReciever;
			}
			HWND hWndCmdReciever =
				CExtPopupMenuWnd::GetCmdTargetWnd( FALSE );
			ASSERT( ::IsWindow(hWndCmdReciever) );
			return hWndCmdReciever;
		};
		void SetCmdReciever( HWND hWndSpecCmdReciever )
		{
			ASSERT(
				hWndSpecCmdReciever == NULL
				|| ::IsWindow(hWndSpecCmdReciever)
				);
			m_hWndSpecCmdReciever = hWndSpecCmdReciever;
		};

		void SetDisplayed( bool bDisplayed )
		{
			m_bDisplayed = bDisplayed;
		};
		bool IsDisplayed() const
		{
			return m_bDisplayed;
		};
		bool IsAllItemsRarelyUsed() const
		{
			ASSERT( IsPopup() );
			ASSERT( m_pWndChild != NULL );
			return m_pWndChild->IsAllItemsRarelyUsed();
		};

		bool IsEnabled() const
		{
			if( IsPopup() )
				return true;
			if( IsSeparator() )
				return false;
			return GetCmd()->StateIsEnable();
		};
		bool IsSelected() const
		{
			if( /*IsPopup() ||*/ IsSeparator() )
				return false;
			return m_bSelected;
		};
		void SetSelected( bool bOn = true )
		{
			m_bSelected = bOn;
		};
		bool GetRadio() const
		{
			if( IsPopup() || IsSeparator() )
				return false;
			return GetCmd()->StateGetRadio();
		};
		bool GetCheck() const
		{
			if( IsPopup() || IsSeparator() )
				return false;
			return GetCmd()->StateGetCheck();
		};
		bool GetIndeterminate() const
		{
			if( IsPopup() || IsSeparator() )
				return false;
			return GetCmd()->StateGetIndeterminate();
		};
		LPCTSTR GetText() const
		{
			if( IsSeparator() )
				return _T("");
			if( IsPopup() )
				return (LPCTSTR)m_sPopupText;
			return GetCmd()->m_sMenuText;
		};
		LPCTSTR GetAccelText() const
		{
			if( IsSeparator() )
				return _T("");
			if( IsPopup() )
				return (LPCTSTR)m_sPopupAccelText;
			return GetCmd()->m_sAccelText;
		};
		HICON GetIcon()
		{
			if( IsSeparator() )
				return NULL;
			if( IsPopup() )
				return m_hIconPopup;
			return
				g_CmdManager->CmdGetHICON(
					g_CmdManager->ProfileNameFromWnd( GetCmdReciever() ),
					m_nCmdID
					);
		};

		LPCTSTR GetTip() const
		{
			if( IsPopup() || IsSeparator() )
				return _T("");
			return GetCmd()->m_sTipStatus;
		};

		bool IsSeparator() const
		{
			bool bSeparator =
				(m_nCmdID==TYPE_SEPARATOR) ? true : false;
			return bSeparator;
		};
		void SetSeparator(
			int nItemIndex
			)
		{
			if( !IsSeparator() )
			{
				if( m_pWndChild != NULL )
					DestroyPopup();
				ASSERT( m_pWndChild == NULL );
				m_sPopupText.Empty();
				m_nCmdID = TYPE_SEPARATOR;
				m_nItemIndex = nItemIndex;
			}
			MeasureItem();
		};

		void SetPopup(
			int nItemIndex,
			LPCTSTR sText = NULL,
			HICON hIcon = NULL,
			CExtPopupMenuWnd * pSpecPopup = NULL
			)
		{
			if( m_pWndChild == NULL )
			{
				if( pSpecPopup == NULL )
					ConstructPopup();
				else
					m_pWndChild = pSpecPopup;
			}
			m_nCmdID = TYPE_POPUP;
			m_sPopupAccelText = _T("");
			m_sPopupText = _T("");
			m_nItemIndex = nItemIndex;
			ASSERT( m_hIconPopup == NULL );
			VERIFY( SetPopupIcon(hIcon) );
			VERIFY( SetPopupText(sText) );
			MeasureItem();
		};
		bool SetPopupIcon( HICON hIcon )
		{
			if( !IsPopup() )
			{
				ASSERT( FALSE );
				return false;
			}
			if( m_hIconPopup != NULL
				&& m_hIconPopup != hIcon
				)
			{
				VERIFY( DestroyIcon(m_hIconPopup) );
			}
			if( hIcon == NULL )
			{
				m_hIconPopup = NULL;
				return true;
			}
			CExtCmdManager::icon_t _icon( hIcon, true );
			m_hIconPopup = _icon.Detach();
			ASSERT( m_hIconPopup != NULL );
			return true;
		};
		bool SetPopupText( LPCTSTR sText ) // NULL if empty
		{
			if( !IsPopup() )
			{
				ASSERT( FALSE );
				return false;
			}
			m_sPopupText =
				(sText == NULL) ? _T("") : sText;
			int nPos = m_sPopupText.Find(_T('\t'));
			if( nPos >= 0 )
			{
				CString sLeft = m_sPopupText.Left(nPos);
				CString sRight = m_sPopupText.Right(m_sPopupText.GetLength()-nPos-1);
				m_sPopupText = sLeft;
				m_sPopupAccelText = sRight;
			}
			return true;
		};
		bool SetPopupAccelText( LPCTSTR sText ) // NULL if empty
		{
			if( !IsPopup() )
			{
				ASSERT( FALSE );
				return false;
			}
			m_sPopupAccelText =
				(sText == NULL) ? _T("") : sText;
			return true;
		}
		bool ConstructPopup()
		{
			ASSERT( m_hIconPopup == NULL );
			ASSERT( m_pWndChild == NULL );
			m_pWndChild = new CExtPopupMenuWnd;
			return true;
		};
		void DestroyPopup()
		{
			VERIFY( SetPopupIcon(NULL) );
			if( m_pWndChild == NULL )
				return;
			delete m_pWndChild;
			m_pWndChild = NULL;
		};
		CExtPopupMenuWnd * GetPopup()
		{
			ASSERT( m_pWndChild != NULL );
			return m_pWndChild;
		};
		const CExtPopupMenuWnd * GetPopup() const
		{
			ASSERT( m_pWndChild != NULL );
			return m_pWndChild;
		};
		bool IsPopup() const
		{
			return ( m_nCmdID == TYPE_POPUP ) ? true : false;
/*
			if( (const_cast<menu_item_info_t *>(this))->
					GetPopup() != NULL
				)
			{
				ASSERT( m_nCmdID == TYPE_POPUP );
				return true;
			}
			ASSERT( m_nCmdID != TYPE_POPUP );
			return false;
*/
		}

		UINT GetCmdID() const
		{
			if( IsSeparator() || IsPopup() )
				return (UINT)(IDC_STATIC);
			ASSERT( GetCmd()->m_nCmdID == m_nCmdID );
			return m_nCmdID;
		};

		bool IsExecutableCmdID() const
		{
			if( IsSeparator() || IsPopup() )
				return false;
			return true;
		}

		int GetMeasuredHeight() const
		{
			return m_size.cy;
		};
		int GetMeasuredWidth() const
		{
			return m_size.cx;
		};

		CRect GetBaseRect() const
		{
			CRect rcItem(
				CPoint(0,0),
				m_size
				);
			return rcItem;
		};
		
		int GetIndex() const
		{
			return m_nItemIndex;
		};
		void SetIndex( int nItemIndex )
		{
			m_nItemIndex = nItemIndex;
		};

	}; // class menu_item_info_t

	typedef
		CArray < menu_item_info_t, menu_item_info_t & >
		items_container_t;
	typedef
		const items_container_t
		const_items_container_t;
	items_container_t m_items_all;
//	items_container_t & _GetItemsContaner();
//	const_items_container_t & _GetItemsContaner() const;
	menu_item_info_t & _GetItemRef(int nIndex);
	const menu_item_info_t & _GetItemRef(int nIndex) const;
	void _InsertItem(
		int nInsertBefore,
		menu_item_info_t & mi
		);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtPopupMenuWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtPopupMenuWnd();
protected:
	virtual void _StartAnimation();
	virtual void _EndAnimation()
	{
		CExtPopupBaseWnd::_EndAnimation();
		Invalidate( FALSE );
		//UpdateWindow();
		_SetCapture();
	}

	// Generated message map functions
	//{{AFX_MSG(CExtPopupMenuWnd)
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
	afx_msg void OnCancelMode();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
/*
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
*/

	class CExtPopupMenuCmdUI;
	friend class CExtPopupMenuCmdUI;
}; // class CExtPopupMenuWnd

class __PROF_UIS_API CExtPopupColorMenuWnd :
	public CExtPopupMenuWnd
{
public:
	DECLARE_DYNCREATE(CExtPopupColorMenuWnd)

	CExtPopupColorMenuWnd();

// Attributes
public:
	static UINT nMsgNotifyColorChanged;
	static UINT nMsgNotifyColorChangedFinally;
	static UINT nMsgNotifyCustColor;

	HWND m_hWndNotifyColorChanged; // def is NULL - use command targed
	LPARAM m_lParamCockie;

	COLORREF m_clrDefault,m_clrInitial;
	bool m_bEnableBtnColorDefault;
	bool m_bEnableBtnColorCustom;

	CString m_sBtnTextColorDefault;
	CString m_sBtnTextColorCustom;

protected:
	CRect m_rcDefColorText,m_rcCustColorText;

	enum
	{
		IDX_DEFAULT_COLOR_BTN = -4,
		IDX_CUSTOM_COLOR_BTN = -5
	};

// Implementation
protected:
	int m_nColorIdxCurr, m_nInitialColorIdx;

	HWND _GetWndNotifyColorChanged();

	int _FindCellByColorRef(COLORREF clr);

	virtual bool _CreateHelper(
		CWnd * pWndCmdReciever
		);

	// To hold the colours and their names
	struct __PROF_UIS_API COLORREF_TABLE_ENTRY
	{
		COLORREF m_clr;
		LPCTSTR m_sName;
	};

	static COLORREF_TABLE_ENTRY g_colors[];

	virtual int _HitTest(const CPoint & point )
	{
		point;
		return IDX_NOTHING;
	};

	void _NotifyColorChanged(
		bool bFinal = false
		);

	CPoint _GetColorItemCoord(int nIdx);
	CRect _GetColorItemRect(int nIdx);
	int _ColorItemHitTest(const CPoint & point);

	virtual CRect _CalcTrackRect();
	virtual CSize _CalcTrackSize();
	virtual void _DoPaint( CDC & dcPaint, bool bUseBackBuffer = true );

public:
	virtual bool _OnMouseMove(UINT nFlags, CPoint point);
	virtual bool _OnMouseClick(UINT nFlags, CPoint point);
	virtual bool _OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	// Generated message map functions
	//{{AFX_MSG(CExtPopupColorMenuWnd)
	//}}AFX_MSG
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	DECLARE_MESSAGE_MAP()
}; // class CExtPopupColorMenuWnd

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __EXT_POPUP_MENU_WND_H
