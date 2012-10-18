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

#include "stdafx.h"

#if( !defined __EXTMINIDOCKFRAMEWND_H)
	#include "ExtMiniDockFrameWnd.h"
#endif

#if (!defined __EXTDOCKBAR_H)
	#include "ExtDockBar.h"
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define __REMOVED_NONFIXMODE_STYLES \
	FWS_SNAPTOBARS
	//|MFS_THICKFRAME|MFS_4THICKFRAME
	//|MFS_MOVEFRAME|MFS_SYNCACTIVE

/////////////////////////////////////////////////////////////////////////////
// CExtMiniDockFrameWnd

bool CExtMiniDockFrameWnd::g_bOnWndPosChgProcessing = false;
bool CExtMiniDockFrameWnd::g_bAutoMakeTopmostOnMouseEvent = true;

IMPLEMENT_DYNCREATE(CExtMiniDockFrameWnd, CMiniDockFrameWnd);

BEGIN_MESSAGE_MAP(CExtMiniDockFrameWnd, CMiniDockFrameWnd)
    //{{AFX_MSG_MAP(CExtMiniDockFrameWnd)
	ON_WM_NCMOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCRBUTTONDOWN()
    ON_WM_GETMINMAXINFO()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_CANCELMODE()
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
/// CMiniDockFrameWnd::Create replacement
BOOL CExtMiniDockFrameWnd::MDFW_Create(
	CWnd* pParent,
	DWORD dwBarStyle
	)
{
	// set m_bInRecalcLayout to avoid flashing during creation
	// RecalcLayout will be called once something is docked
	m_bInRecalcLayout = TRUE;

DWORD dwStyle =
		WS_POPUP|WS_CAPTION|WS_SYSMENU
		|MFS_MOVEFRAME|MFS_4THICKFRAME
		|MFS_SYNCACTIVE|MFS_BLOCKSYSMENU
		|FWS_SNAPTOBARS
		;

	if( dwBarStyle & CBRS_SIZE_DYNAMIC )
		dwStyle &= ~MFS_MOVEFRAME;

DWORD dwExStyle = 0;
	if(	!MFW_CreateEx(
			dwExStyle,
			NULL,
			&afxChNil,
			dwStyle,
			rcDefault,
			pParent,
			0
			)
		)
	{
		m_bInRecalcLayout = FALSE;
		return FALSE;
	}
	dwStyle =
		dwBarStyle &
		(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT) ?
			CBRS_ALIGN_LEFT : CBRS_ALIGN_TOP;
	dwStyle |=
		dwBarStyle & CBRS_FLOAT_MULTI;
CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->DeleteMenu(SC_SIZE, MF_BYCOMMAND);
	pSysMenu->DeleteMenu(SC_MINIMIZE, MF_BYCOMMAND);
	pSysMenu->DeleteMenu(SC_MAXIMIZE, MF_BYCOMMAND);
	pSysMenu->DeleteMenu(SC_RESTORE, MF_BYCOMMAND);
CString strHide;
	if( strHide.LoadString(AFX_IDS_HIDE) )
	{
		pSysMenu->DeleteMenu(SC_CLOSE, MF_BYCOMMAND);
		pSysMenu->AppendMenu(MF_STRING|MF_ENABLED, SC_CLOSE, strHide);
	}

	// must initially create with parent frame as parent
	if(	!m_wndDockBar.Create(
			pParent,
			WS_CHILD|WS_VISIBLE|dwStyle,
			AFX_IDW_DOCKBAR_FLOAT
			)
		)
	{
		m_bInRecalcLayout = FALSE;
		return FALSE;
	}

	// set parent to CMiniDockFrameWnd
	m_wndDockBar.SetParent(this);
	m_bInRecalcLayout = FALSE;

	return TRUE;
}

/// CMiniFrameWnd::CreateEx replacement
BOOL CExtMiniDockFrameWnd::MFW_CreateEx(
	DWORD dwExStyle,
	LPCTSTR lpClassName,
	LPCTSTR lpszWindowName,
	DWORD dwStyle,
	const RECT& rc,
	CWnd* pParentWnd,
	UINT nID
	)
{
	m_strCaption = lpszWindowName;
	return
		CWnd::CreateEx(
			dwExStyle,
			lpClassName ?
				lpClassName
				:
				AfxRegisterWndClass(
					CS_DBLCLKS, // | CS_SAVEBITS
					::LoadCursor(
					NULL,
					IDC_ARROW
					)
				),
			lpszWindowName,
			dwStyle,
			rc.left,
			rc.top,
			rc.right - rc.left,
			rc.bottom - rc.top,
			pParentWnd->GetSafeHwnd(),
			(HMENU)nID
			);
}
*/

BOOL CExtMiniDockFrameWnd::Create(CWnd* pParent, DWORD dwBarStyle)
{
//	if(!MDFW_Create(pParent, dwBarStyle))
//		return FALSE;
	if(!CMiniDockFrameWnd::Create(pParent, dwBarStyle))
		return FALSE;
	ModifyStyle( MFS_4THICKFRAME, 0 );
	ModifyStyleEx( 0, WS_EX_TOOLWINDOW );
	ModifyStyle( WS_SYSMENU, 0 );
	SetTimer(1,1,NULL);
	return TRUE;
}

void CExtMiniDockFrameWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	if( _ResizingIsInProgress() )
	{
		_ResizingEnd();
		return;
	}

CRect rcBarWnd;
	GetWindowRect( &rcBarWnd );
CPoint ptDevOffset = -rcBarWnd.TopLeft();
CPoint ptWnd( point + ptDevOffset );

CExtControlBar * pExtBar = GetControlBarExt();
	if( pExtBar != NULL && pExtBar->m_bNcAreaBtnTracking )
	{
		INT nCountOfNcButtons = pExtBar->NcButtons_GetCount();
		for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
		{
			CExtBarNcAreaButton * pBtn =
				pExtBar->NcButtons_GetAt( nBtn );
			if( !pBtn->OnQueryVisibility() )
				continue;
			if( pBtn->OnNcAreaClicked( ptWnd ) )
				return;
		} // for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
		return;
	} // if( pExtBar != NULL && pExtBar->m_bNcAreaBtnTracking )

	if( m_rcBtnHideBar.PtInRect( ptWnd ) )
	{
		CControlBar * pBar =
			GetControlBar();
		if( pBar == NULL )
			return;
		ASSERT_VALID( pBar );
		GetParentFrame()->
			ShowControlBar(
				pBar,
				FALSE,
				FALSE
				);
		return;
	}

	CMiniDockFrameWnd::OnNcLButtonUp(nHitTest, point);
}

void CExtMiniDockFrameWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
CExtControlBar * pExtBar = GetControlBarExt();
CPoint p(point);
CRect rcWnd;
	GetWindowRect( &rcWnd );
	p.Offset( -rcWnd.left, -rcWnd.top );
	if( m_rcBtnHideBar.PtInRect( p ) )
		return;
	if( nHitTest == HTCAPTION )
	{
		ActivateTopParent();
		BringWindowToTop();
		if( (m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0 )
		{
			int nPos = 1;
			CControlBar* pBar = NULL;
			while(
				pBar == NULL
				&& nPos < m_wndDockBar.m_arrBars.GetSize()
				)
				pBar =
					reinterpret_cast < CExtDockBar & >
						(m_wndDockBar). GetDockedControlBar(nPos++);
			ASSERT(pBar != NULL);
			ASSERT_KINDOF( CControlBar, pBar );
			ASSERT( pBar->m_pDockContext != NULL );
			if( pExtBar == NULL
				|| (!pExtBar->IsShowContentWhenDragging())
				)
			{
				pBar->m_pDockContext->StartDrag(point);
				return;
			}
			int nCyGripper =
				min(
					pExtBar->m_rcGrip.Width(),
					pExtBar->m_rcGrip.Height()
					);
			pExtBar->_DraggingStart(
				point,
				point
					- rcWnd.TopLeft()
					- CPoint( nCyGripper, nCyGripper/2 )
				);
			return;
		}
	}
	else if( nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST )
	{
		ActivateTopParent();
		int nPos = 1;
		CControlBar * pBar = NULL;
		while(
			pBar == NULL
			&& nPos < m_wndDockBar.m_arrBars.GetSize()
			)
			pBar =
				reinterpret_cast < CExtDockBar & >
					(m_wndDockBar).GetDockedControlBar(nPos++);
		ASSERT(pBar != NULL);
		ASSERT_KINDOF(CControlBar, pBar);
		ASSERT(pBar->m_pDockContext != NULL);
		ASSERT( (m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0 );
		if(	!pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
			CMiniDockFrameWnd::OnNcLButtonDown( nHitTest, point);
		else
		{
			CExtControlBar * pExtBar =
				(CExtControlBar *)pBar;
			if( !pExtBar->IsShowContentWhenDragging() )
			{
				pBar->m_pDockContext->StartResize(nHitTest,point);
				return;
			}
			ClientToScreen( &point );
			_ResizingStart( nHitTest, point );
		}
		return;
	}
	else if( nHitTest == HTMINBUTTON
		|| nHitTest == HTMAXBUTTON
		|| nHitTest == HTGROWBOX
		)
		return;
	
	CMiniFrameWnd::OnNcLButtonDown( nHitTest, point );
}

void CExtMiniDockFrameWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	if( nHitTest == HTCAPTION )
	{
		CRect rcWnd;
		GetWindowRect( &rcWnd );
		CPoint ptTestBtnHideBar( point - rcWnd.TopLeft() );
		if( m_rcBtnHideBar.PtInRect(ptTestBtnHideBar) )
			return;
	} // if( nHitTest == HTCAPTION )
	CMiniDockFrameWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CExtMiniDockFrameWnd::_ResizingStart(
	UINT nHitTest,
	const CPoint & point
	)
{
#ifdef _USRDLL
	// If this is a DLL, need to set up MFC state
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

	ASSERT( m_nResizingMode == HTNOWHERE );
	ASSERT( IsWindowVisible() );

	CExtPopupMenuWnd::CancelMenuTracking();

CExtControlBar * pExtBar = GetControlBarExt();
	ASSERT_VALID( pExtBar );
	if( ( ! pExtBar->IsShowContentWhenDragging() )
//		|| pExtBar->m_bFixedMode
		)
	{
		m_nResizingMode = HTNOWHERE;
		pExtBar->m_pDockContext->StartResize(nHitTest, point);
		return;
	}

	switch( nHitTest )
	{
	case HTLEFT:
	case HTRIGHT:
	case HTTOP:
	case HTTOPLEFT:
	case HTTOPRIGHT:
	case HTBOTTOM:
	case HTBOTTOMLEFT:
	case HTBOTTOMRIGHT:
		m_nResizingMode = nHitTest;
	break;
	default:
		return;
	} // switch( nHitTest )

#ifdef _DEBUG
int nPos = 1;
CControlBar * pDebugDummyBar = NULL;
	while(
		pDebugDummyBar == NULL
		&& nPos < m_wndDockBar.m_arrBars.GetSize()
		)
		pDebugDummyBar =
			reinterpret_cast<CExtDockBar&>
				(m_wndDockBar).
					GetDockedControlBar(nPos++);
	ASSERT(pDebugDummyBar != NULL);
	ASSERT_KINDOF(CExtControlBar, pDebugDummyBar);
	ASSERT(pDebugDummyBar->m_pDockContext != NULL);
	// CBRS_SIZE_DYNAMIC toolbars cannot have the CBRS_FLOAT_MULTI style
	ASSERT((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0);
#endif // _DEBUG
	
	ASSERT( pExtBar == pDebugDummyBar );
	ASSERT(pExtBar->m_pDockContext != NULL);

	if( !pExtBar->m_bFixedMode )
		ModifyStyle(__REMOVED_NONFIXMODE_STYLES, 0);
	BringWindowToTop();
	SetCapture();
	GetWindowRect( &m_rcWndResizingStart );

	while( m_nResizingMode != HTNOWHERE )
	{
		::WaitMessage();
		MSG msg;
		// Process all the messages in the message queue
		while( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
		{
			bool bStop = false;
			switch( msg.message )
			{
			case WM_ACTIVATEAPP:
			case WM_COMMAND:
				bStop = true;
			break;
			default:
				if(	WM_KEYFIRST <= msg.message
					&& msg.message <= WM_KEYLAST
					)
				bStop = true;
			break;
			} // switch( msg.message )
			if( (!bStop) &&
				GetCapture() != this
				)
				bStop = true;
			if( bStop )
			{
				_ResizingEnd();
				return;
			}
			if( !AfxGetApp()->PumpMessage() )
			{
				PostQuitMessage(0);
				return; // Signal WM_QUIT received
			} // if( !AfxGetApp()->PumpMessage() )
		} // while( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
		LONG lIdle = 0;
		while( AfxGetApp()->OnIdle(lIdle++) );
	} // while( m_nResizingMode != HTNOWHERE )
}

void CExtMiniDockFrameWnd::_ResizingEnd()
{
	//ASSERT( m_nResizingMode != HTNOWHERE );
	if( m_nResizingMode == HTNOWHERE )
		return;
	if( GetCapture() == this )
		ReleaseCapture();
CExtControlBar * pExtBar = GetControlBarExt();
	ASSERT_VALID( pExtBar );
	if( !pExtBar->m_bFixedMode )
		ModifyStyle(0, __REMOVED_NONFIXMODE_STYLES);
	m_nResizingMode = HTNOWHERE;
}

void CExtMiniDockFrameWnd::_ResizingUpdateState(
	const CPoint & point
	)
{
CPoint _pt(point);
	ASSERT( m_nResizingMode != HTNOWHERE );
CExtControlBar * pExtBar = GetControlBarExt();
	ASSERT_VALID( pExtBar );

//	BringWindowToTop();
CRect rcClient, rcWnd, rcWndOrg;
	GetClientRect(rcClient);
	GetWindowRect(rcWnd);
	rcWndOrg = rcWnd;
CSize sizeDiff = rcWnd.Size() - rcClient.Size();
	ASSERT( sizeDiff.cx >= 0 && sizeDiff.cy >= 0 );

	if( sizeDiff.cx < pExtBar->_CalcDesiredMinHW() )
		sizeDiff.cx = pExtBar->_CalcDesiredMinHW();
	if( sizeDiff.cy < pExtBar->_CalcDesiredMinVH() )
		sizeDiff.cy = pExtBar->_CalcDesiredMinVH();

UINT nHit = m_nResizingMode;
bool	bLeft=false, bTop=false, bHorz=false,
		bInvTop=false, bInvLeft=false;
CPoint ptOffsetWnd(0,0);
	if( pExtBar->m_bFixedMode )
	{
		switch( nHit )
		{
		case HTTOPLEFT:
		case HTTOPRIGHT:
			nHit = HTTOP;
		break;
		case HTBOTTOMLEFT:
		case HTBOTTOMRIGHT:
			nHit = HTBOTTOM;
		break;
		} // switch( nHit )
		switch( nHit )
		{
		case HTTOP:
//			if( _pt.y > rcWndOrg.bottom )
//				_pt.y = rcWndOrg.bottom;
			_pt.y =
				rcWndOrg.bottom
				+ rcWndOrg.top - _pt.y;
			nHit = HTBOTTOM;
			bInvLeft = true;
			bInvTop = true;
		break;
		case HTLEFT:
//			if( _pt.x > rcWndOrg.right )
//				_pt.x = rcWndOrg.right;
			_pt.x =
				rcWndOrg.right
				+ rcWndOrg.left - _pt.x;
			nHit = HTRIGHT;
			bInvLeft = true;
			bInvTop = true;
		break;
		} // switch( nHit )
	} // if( pExtBar->m_bFixedMode )
	if( bInvLeft || bInvTop || (!pExtBar->m_bFixedMode) )
	{
		CRect rcWnd;
		CRect rcClient;
		GetWindowRect( &rcWnd );
		GetClientRect( &rcClient );
		ScreenToClient( & rcWnd );
		if( bInvLeft || (!pExtBar->m_bFixedMode) )
			ptOffsetWnd.x =
				abs( rcWnd.left - rcClient.left );
		if( bInvTop || (!pExtBar->m_bFixedMode) )
			ptOffsetWnd.y =
				abs( rcWnd.top - rcClient.top );
	} // if( bInvLeft || bInvTop )

	switch( nHit )
	{
	case HTLEFT:
		bHorz = true;
		rcWnd.left = min(_pt.x,rcWnd.right);
		bLeft = true;
	break;
	case HTRIGHT:
		bHorz = true;
		rcWnd.right = max(_pt.x,rcWnd.left);
	break;
	case HTTOP:
		rcWnd.top = min(_pt.y,rcWnd.bottom);
		bTop = true;
	break;
	case HTTOPLEFT:
		rcWnd.left = min(_pt.x,rcWnd.right);
		rcWnd.top = min(_pt.y,rcWnd.bottom);
		bLeft = true;
		bTop = true;
	break;
	case HTTOPRIGHT:
		rcWnd.right = max(_pt.x,rcWnd.left);
		rcWnd.top = min(_pt.y,rcWnd.bottom);
		bTop = true;
	break;
	case HTBOTTOM:
		rcWnd.bottom = max(_pt.y,rcWnd.top);
	break;
	case HTBOTTOMLEFT:
		rcWnd.left = min(_pt.x,rcWnd.right);
		rcWnd.bottom = max(_pt.y,rcWnd.top);
		bLeft = true;
	break;
	case HTBOTTOMRIGHT:
		rcWnd.right = max(_pt.x,rcWnd.left);
		rcWnd.bottom = max(_pt.y,rcWnd.top);
	break;
	default:
		ASSERT( FALSE );
		return;
	} // switch( nHit )

	if( rcWnd.Width() < sizeDiff.cx )
		rcWnd.right = rcWnd.left + sizeDiff.cx;
	if( rcWnd.Height() < sizeDiff.cy )
		rcWnd.bottom = rcWnd.top + sizeDiff.cy;
	if( rcWnd.right > rcWndOrg.right
		&& bLeft
		)
		rcWnd.OffsetRect(
			rcWndOrg.right - rcWnd.right,
			0
			);
	if( rcWnd.bottom > rcWndOrg.bottom
		&& bTop
		)
		rcWnd.OffsetRect(
			0,
			rcWndOrg.bottom - rcWnd.bottom
			);
//	rcWnd.OffsetRect( ptOffsetWnd );
//	rcWndOrg.OffsetRect( ptOffsetWnd );
	rcClient.SetRect(
		rcWnd.left - (rcClient.left-rcWndOrg.left),
		rcWnd.top - (rcClient.top-rcWndOrg.top),
		rcWnd.right - (rcClient.right-rcWndOrg.right),
		rcWnd.bottom - (rcClient.bottom-rcWndOrg.bottom)
		);
	if( !pExtBar->m_bFixedMode )
	{
		/*
		ASSERT( (GetStyle() & __REMOVED_NONFIXMODE_STYLES) == 0 );
		pExtBar->m_sizeFloat = rcClient.Size();
		MoveWindow( &rcWnd );
		*/

		ASSERT( (GetStyle() & __REMOVED_NONFIXMODE_STYLES) == 0 );
		pExtBar->m_sizeFloated =
			rcWnd.Size()
			-ptOffsetWnd
//			- CSize(
//				afxData.cxBorder2*2,
//				afxData.cyBorder2*2
//				)
			;

		MoveWindow( &rcWnd );
		//ASSERT( pExtBar->m_sizeFloat == rcClient.Size() );
		DelayRecalcLayout();
		SetTimer(3,20,NULL);
		return;
		/*
		CRect rcResultClient;
		pExtBar->GetClientRect( &rcResultClient );
		ASSERT( rcResultClient.Size() == rcClient.Size() );
		*/
	} // if( !pExtBar->m_bFixedMode )
	else
	{
		DWORD dwMode = LM_HORZ|LM_COMMIT;
		LONG nLendth = 0;
		if( bHorz )	
			nLendth = rcClient.Width();
		else
		{
			dwMode |= LM_LENGTHY;
			nLendth = rcClient.Height();
		}
		CSize _size = pExtBar->
			CalcDynamicLayout(
				nLendth, //rcClient.Width(),
				dwMode // LM_HORZ | LM_COMMIT
				);
		pExtBar->m_sizeFloated = _size;
		if( bInvTop )
		{
			ASSERT( rcWnd.top == rcWndOrg.top );
			rcWnd.bottom = m_rcWndResizingStart.bottom;
			rcWnd.top =
				rcWnd.bottom
				- _size.cy // - (_size.cy + sizeDiff.cy)
				- ptOffsetWnd.y
				;
		}
		if( bInvLeft )
		{
			ASSERT( rcWnd.left == rcWndOrg.left );
			rcWnd.right = m_rcWndResizingStart.right;
			rcWnd.left =
				rcWnd.right
				- _size.cx // - (_size.cx + sizeDiff.cx)
				- ptOffsetWnd.x
				;
		}
	} // else from if( !pExtBar->m_bFixedMode )

/*
CSize sizePreCalc = rcClient.Size();
	if( _size != sizePreCalc )
	{
		pExtBar->m_sizeFloat = _size;
		if( bHorz )
			rcWnd.left += _size.cx-sizePreCalc.cx;
		else
			rcWnd.top += _size.cy-sizePreCalc.cy;
	}
*/

	ASSERT( pExtBar->m_pDockSite != NULL );
	ASSERT( pExtBar->m_pDockBar != NULL );
	pExtBar->m_ptFloatHelper = rcWnd.TopLeft();
	pExtBar->m_pDockSite->
		FloatControlBar(
			pExtBar,
			pExtBar->m_ptFloatHelper,
			CBRS_ALIGN_TOP
				| (pExtBar->m_pDockBar->m_dwDockStyle & CBRS_FLOAT_MULTI)
				| CBRS_SIZE_DYNAMIC
			);
	RecalcLayout();
}

CControlBar * CExtMiniDockFrameWnd::GetControlBar()
{
CWnd* pTempWnd = GetWindow( GW_CHILD );
    if( pTempWnd == NULL )
        return NULL;
	pTempWnd = pTempWnd->GetWindow( GW_CHILD );
    if( pTempWnd == NULL )
        return NULL;
    if( !pTempWnd->IsKindOf( RUNTIME_CLASS(CControlBar) ) )
        return NULL;
    return reinterpret_cast < CControlBar * >( pTempWnd );
}

CExtControlBar * CExtMiniDockFrameWnd::GetControlBarExt()
{
CControlBar * pTempWnd = GetControlBar();
    if( pTempWnd == NULL )
        return NULL;
//	ASSERT_KINDOF( CExtControlBar, pTempWnd );
    if( !pTempWnd->IsKindOf( RUNTIME_CLASS(CExtControlBar) ) )
        return NULL;
    return reinterpret_cast < CExtControlBar * >( pTempWnd );
}


void CExtMiniDockFrameWnd::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
	if( _ResizingIsInProgress() )
	{
		_ResizingEnd();
		return;
	}
	if( nHitTest == HTCAPTION )
	{
		CExtControlBar * pExtBar =
			GetControlBarExt();
		if( pExtBar != NULL )
		{
			pExtBar->_ContextMenuTrack();
			return;
		} // if( pExtBar != NULL )
	} // if( nHitTest == HTCAPTION )
	CMiniDockFrameWnd::OnNcRButtonDown(nHitTest,point);
}

void CExtMiniDockFrameWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CMiniDockFrameWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
//CRect & rcClient = (CRect &)(lpncsp->rgrc[0]);
//	rcClient.DeflateRect( 2, 2 );

}

void CExtMiniDockFrameWnd::OnNcPaint() 
{
CExtControlBar * pExtBar =
		GetControlBarExt();
bool bExtBar = false;
	if( pExtBar != NULL
		&& !pExtBar->m_bFixedMode
		)
		bExtBar = true;

CRect rcClient, rcBar;
	GetClientRect(rcClient);
	ClientToScreen(rcClient);
	GetWindowRect(rcBar);
	rcClient.OffsetRect(-rcBar.TopLeft());
	rcBar.OffsetRect(-rcBar.TopLeft());

CWindowDC dc( this );
	dc.ExcludeClipRect(rcClient);
CExtMemoryDC dcWin(&dc);

	g_PaintManager->PaintDockingFrame(
		dcWin,
		rcBar,
		rcClient,
		true,
		bExtBar
		);
CString sCaption;
	if( pExtBar != NULL )
		pExtBar->GetWindowText(sCaption);
	if( sCaption.IsEmpty() )
		GetWindowText(sCaption);

CRect rcCapt( m_rcFrameCaption );
CRect rcText( m_rcFrameCaption );
	rcText.right = m_rcBtnHideBar.left;

	if( pExtBar != NULL )
	{
		INT nCountOfNcButtons = pExtBar->NcButtons_GetCount();
		if( nCountOfNcButtons > 0 )
		{
			CExtBarNcAreaButton * pBtn =
				pExtBar->NcButtons_GetAt( nCountOfNcButtons - 1 );
			const CRect & rcBtn = *pBtn;
			rcText.right = rcBtn.left - 2;
			if( rcText.left > rcText.right )
				rcText.left = rcText.right;
		} // if( nCountOfNcButtons > 0 )
	}

bool bActive = false;
HWND hTmpWndFocus = ::GetFocus();
	if( hTmpWndFocus != NULL
		&& (  GetSafeHwnd() == hTmpWndFocus
			  || ::IsChild( GetSafeHwnd(), hTmpWndFocus )
			)
		)
		bActive = true;

	rcText.DeflateRect(2,0);
	g_PaintManager->PaintGripper(
		dcWin,
		rcCapt,
		rcText,
		bActive,
		true,
		false,
		bExtBar,
		sCaption.IsEmpty() ? NULL : (LPCTSTR)sCaption
		);

	if( pExtBar != NULL )
	{
		pExtBar->NcButtons_Paint( dcWin );
	} // if( pExtBar != NULL )
	else
	{
		bool bPushed = false, bEnabled = true;
		g_PaintManager->PaintDockingCaptionButton(
			dcWin,
			m_rcBtnHideBar,
			CExtPaintManager::__DCBT_CLOSE,
			m_bBtnHideBar,
			bPushed,
			bEnabled,
			bExtBar,
			bActive
			);
	} // else from if( pExtBar != NULL )

}

BOOL CExtMiniDockFrameWnd::OnNcActivate(BOOL bActive) 
{
BOOL rc = CMiniDockFrameWnd::OnNcActivate(bActive);
	SendMessage( WM_NCPAINT );
	if( bActive )
	{
		CExtControlBar * pExtBar =
			GetControlBarExt();
		if( pExtBar != NULL )
			pExtBar->RedrawWindow(
				NULL,
				NULL,
				RDW_INVALIDATE|RDW_UPDATENOW
					|RDW_ERASE|RDW_ALLCHILDREN
				);
	} // if( bActive )
	return rc;
}

void CExtMiniDockFrameWnd::OnSize(UINT nType, int cx, int cy) 
{
//	if( _ResizingIsInProgress() )
//		return;

	CMiniDockFrameWnd::OnSize(nType, cx, cy);


DWORD dwStyle = GetStyle();
CSize sizeFrame =
		(dwStyle & WS_THICKFRAME)
			?	CSize(
					GetSystemMetrics(SM_CXSIZEFRAME),
					GetSystemMetrics(SM_CYSIZEFRAME)
					)
			:	CSize(
					GetSystemMetrics(SM_CXFIXEDFRAME),
					GetSystemMetrics(SM_CYFIXEDFRAME)
		);
CRect rcFrameCaption;
	GetWindowRect( &rcFrameCaption );
	rcFrameCaption -= rcFrameCaption.TopLeft();
	rcFrameCaption.left  += sizeFrame.cx;
	rcFrameCaption.right -= sizeFrame.cx;
	rcFrameCaption.top   += sizeFrame.cy;
	rcFrameCaption.bottom =
		rcFrameCaption.top
		+ ::GetSystemMetrics(SM_CYSMCAPTION)
		- ::GetSystemMetrics(SM_CYSMSIZE);
	rcFrameCaption.bottom =
		rcFrameCaption.top
		+ ::GetSystemMetrics(SM_CYSMSIZE);
	m_rcFrameCaption = rcFrameCaption;

int nDummyMetric = ::GetSystemMetrics(SM_CXSMSIZE) - 1;
int nRight =
		m_rcFrameCaption.right - nDummyMetric - 1;
	m_rcBtnHideBar.SetRect(
		nRight,
		m_rcFrameCaption.top + 2,
		nRight + nDummyMetric - 2,
		m_rcFrameCaption.bottom - 2
		);

CExtControlBar * pExtBar = GetControlBarExt();
	if( pExtBar != NULL )
	{
		pExtBar->OnNcAreaButtonsReinitialize();
		pExtBar->OnNcAreaButtonsReposition();
	} // if( pExtBar != NULL )

	RedrawWindow(
		NULL,
		NULL,
		RDW_FRAME|RDW_INVALIDATE|RDW_ERASE
			|RDW_UPDATENOW|RDW_ALLCHILDREN
		);
}

void CExtMiniDockFrameWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CMiniDockFrameWnd::OnWindowPosChanged(lpwndpos);
	if(		_ResizingIsInProgress()
		||	g_bOnWndPosChgProcessing
		||	(lpwndpos->flags & SWP_NOMOVE) != 0
		)
		return;
CExtControlBar * pExtBar = GetControlBarExt();
	if( pExtBar == NULL || !pExtBar->IsFloating() )
		return;
CRect rcWnd;
	GetWindowRect( &rcWnd );
CRect rcDesktop;
	::SystemParametersInfo( SPI_GETWORKAREA, 0, &rcDesktop, 0 );
	ASSERT( !g_bOnWndPosChgProcessing );
	if( rcWnd.top < rcDesktop.top )
	{
		rcWnd.top = rcDesktop.top;
		g_bOnWndPosChgProcessing = true;
	}
	else if( rcWnd.top + ::GetSystemMetrics(SM_CYSMCAPTION) > rcDesktop.bottom )
	{
		rcWnd.top =
			max(
			rcDesktop.top, 
			rcDesktop.bottom
				- ::GetSystemMetrics(SM_CYSMCAPTION)
			);
		g_bOnWndPosChgProcessing = true;
	}
	if( !g_bOnWndPosChgProcessing )
		return;
	SetWindowPos(
		NULL,
		rcWnd.left, rcWnd.top, -1, -1,
		SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE
		);
	g_bOnWndPosChgProcessing = false;
}

void CExtMiniDockFrameWnd::OnWindowPosChanging( WINDOWPOS* lpwndpos )
{
	CMiniDockFrameWnd::OnWindowPosChanging( lpwndpos );
	ModifyStyle( WS_SYSMENU, 0 );
}

void CExtMiniDockFrameWnd::_AnalyzeButtonsHover(CPoint point)
{
CExtControlBar * pExtBar = GetControlBarExt();
	if( pExtBar != NULL )
	{
		pExtBar->NcButtons_HitTest( point );
		return;
	}
CRect rcClient, rcBar;
	GetClientRect( &rcClient );
	ClientToScreen( &rcClient );
	GetWindowRect( &rcBar );
	rcClient.OffsetRect( -rcBar.TopLeft() );
	rcBar.OffsetRect (-rcBar.TopLeft() );
	ScreenToClient( &point );
	point.y -= rcBar.top - rcClient.top;
bool bBtnHideBarHover = false;
	if( m_rcBtnHideBar.PtInRect(point) )
		bBtnHideBarHover = true;
	if(	m_bBtnHideBar != bBtnHideBarHover )
	{
		m_bBtnHideBar = bBtnHideBarHover;
		SendMessage( WM_NCPAINT );
	}
	if( m_bBtnHideBar || bBtnHideBarHover )
		SetTimer(2,20,NULL);
	else
		KillTimer(2);
}

void CExtMiniDockFrameWnd::OnNcMouseMove( UINT nHitTest, CPoint point )
{
	if( g_bAutoMakeTopmostOnMouseEvent
		&& (!CExtPopupMenuWnd::IsMenuTracking())
		)
		BringWindowToTop();
	if( _ResizingIsInProgress() )
	{
		return;
	}
	CMiniDockFrameWnd::OnNcMouseMove( nHitTest, point );
	_AnalyzeButtonsHover(point);
}

void CExtMiniDockFrameWnd::OnTimer( UINT nIDEvent )
{
CExtControlBar * pExtBar = GetControlBarExt();
	if( nIDEvent == 1 )
	{
		if( pExtBar == NULL )
			return;
		KillTimer(1);
		SetWindowPos(
			NULL,0,0,0,0,
			SWP_NOMOVE|SWP_NOACTIVATE // |SWP_NOSIZE
				|SWP_NOZORDER|SWP_NOOWNERZORDER 
				|SWP_FRAMECHANGED
			);
		return;
	}
	if( nIDEvent == 2 )
	{
		_AnalyzeButtonsHover();
		return;
	}
	if( nIDEvent == 3 )
	{
		if( pExtBar == NULL
			|| _ResizingIsInProgress()
			)
		return;
		RecalcLayout();
		KillTimer(3);
		return;
	}

	if( pExtBar == NULL )
		return;
	ASSERT_VALID( pExtBar );
	if( nIDEvent == (UINT)pExtBar->GetDlgCtrlID() )
	{
		if( _ResizingIsInProgress() )
			return;
		CPoint pt;
		VERIFY( GetCursorPos( &pt ) );
		CWnd * pTempWnd = WindowFromPoint( pt );
		int IsMouseInsideTheWindow =
			IsChild( pTempWnd ) || pTempWnd->m_hWnd == m_hWnd;

		if(	IsMouseInsideTheWindow
			|| IsChild(GetFocus())  
			|| (GetCapture() && pExtBar->m_hWnd == GetCapture()->m_hWnd) 
			|| CExtPopupMenuWnd::IsMenuTracking()
			) 
		{

			if(	IsMouseInsideTheWindow 
				&& !CExtPopupMenuWnd::IsMenuTracking()
				&& IsWindowEnabled()
				&& GetFocus()
				) 
				BringWindowToTop();
		}
	} // if( nIDEvent == (UINT)pExtBar->GetDlgCtrlID() )
}

void CExtMiniDockFrameWnd::OnClose() 
{
	CMiniDockFrameWnd::OnClose();
}

void CExtMiniDockFrameWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    CMiniDockFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CExtMiniDockFrameWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( g_bAutoMakeTopmostOnMouseEvent
		&& (!CExtPopupMenuWnd::IsMenuTracking())
		)
		BringWindowToTop();
	if( _ResizingIsInProgress() )
	{
		ClientToScreen( &point );
		_ResizingUpdateState( point );
		return;
	}
	CMiniDockFrameWnd::OnMouseMove(nFlags, point);
}

void CExtMiniDockFrameWnd::OnCaptureChanged(CWnd *pWnd) 
{
	if( _ResizingIsInProgress()
		&& pWnd != this
		)
	{
		_ResizingEnd();
		return;
	}
	CMiniDockFrameWnd::OnCaptureChanged(pWnd);
}

void CExtMiniDockFrameWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( _ResizingIsInProgress() )
	{
		_ResizingEnd();
		return;
	}
	CMiniDockFrameWnd::OnLButtonUp(nFlags, point);
}

void CExtMiniDockFrameWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if( _ResizingIsInProgress() )
	{
		_ResizingEnd();
		return;
	}
	CMiniDockFrameWnd::OnRButtonDown(nFlags, point);
}

BOOL CExtMiniDockFrameWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

UINT CExtMiniDockFrameWnd::OnNcHitTest(CPoint point) 
{
UINT nHit = CMiniDockFrameWnd::OnNcHitTest(point);
CExtControlBar * pExtBar = GetControlBarExt();
	if( pExtBar != NULL )
	{
		UINT nHT = pExtBar->NcButtons_HitTest( point );
		if( nHT != HTCLIENT )
		{
			pExtBar->m_bNcAreaBtnTracking = true;
			return nHT;
		}
		ASSERT_VALID( pExtBar );
		if( pExtBar->m_bFixedMode )
		{
			switch( nHit )
			{
			case HTTOPLEFT:
			case HTTOPRIGHT:
				nHit = HTTOP;
			break;
			case HTBOTTOMLEFT:
			case HTBOTTOMRIGHT:
				nHit = HTBOTTOM;
			break;
			} // switch( nHit )
		} // if( pExtBar->m_bFixedMode )
	} // if( pExtBar != NULL )
	return nHit;
}

void CExtMiniDockFrameWnd::OnCancelMode() 
{
	CMiniDockFrameWnd::OnCancelMode();
	CExtPopupMenuWnd::CancelMenuTracking();
	ASSERT( !CExtPopupMenuWnd::IsMenuTracking() );
	if( _ResizingIsInProgress() )
		_ResizingEnd();
}

void CExtMiniDockFrameWnd::OnPaint() 
{
CPaintDC dcPaint( this );
CRect rcClient;
	GetClientRect( &rcClient );
	if( rcClient.IsRectEmpty()
		|| !dcPaint.RectVisible( &rcClient )
		)
		return;
	CExtPaintManager::stat_ExcludeChildAreas(
		dcPaint.GetSafeHdc(),
		GetSafeHwnd()
		);
COLORREF clrBack =
		g_PaintManager->GetColor(
			CExtPaintManager::CLR_3DFACE_OUT
			);
	dcPaint.FillSolidRect( &rcClient, clrBack );
}


