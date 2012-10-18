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
#include "ExtButton.h"

#include "ExtCmdManager.h"
#include "ExtPaintManager.h"
#include "ExtMemoryDC.h"
#include "ExtPopupMenuWnd.h"
#include "ExtColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExtButton::CExtButton()
{
	// RTF tool tip support
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
	m_bToolTipSet = FALSE;
#endif

	m_hIconIn = NULL;
	m_hIconOut = NULL;
	m_hCursor = NULL;
  
	m_bIsFlat = FALSE; 
	m_bDrawBorder = TRUE; 

	m_bMouseOver = m_bPushed = m_bPushTracking = m_bKeyTracking = FALSE;
  
	m_nAlign =
		CExtPaintManager::e_align_t::__ALIGN_HORIZ_CENTER
		|CExtPaintManager::e_align_t::__ALIGN_VERT
		; 
	m_bIsDefault = FALSE;

	m_ToolTip.m_hWnd = NULL;

	m_nMenuOpt = TPMX_TOPALIGN;
}

CExtButton::~CExtButton()
{
	_FreeWinObjects();
}

void CExtButton::_FreeWinObjects()
{
	if( m_hIconIn != NULL )
		::DestroyIcon( m_hIconIn );
	if( m_hIconOut != NULL )
		::DestroyIcon( m_hIconOut );
	if( m_hCursor != NULL )
		::DestroyCursor(m_hCursor);
}

IMPLEMENT_DYNCREATE(CExtButton, CButton);

BEGIN_MESSAGE_MAP(CExtButton, CButton)
    //{{AFX_MSG_MAP(CExtButton)
	ON_WM_CAPTURECHANGED()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CExtButton::SetIcon(int nIconInId, int nIconOutId)
{
HICON hIconIn;
HICON hIconOut;
HINSTANCE hInstResource =
		AfxFindResourceHandle(
			MAKEINTRESOURCE(nIconInId),
			RT_GROUP_ICON
			);

	hIconIn = (HICON)::LoadImage(
		hInstResource,
		MAKEINTRESOURCE(nIconInId),
		IMAGE_ICON,
		0,
		0,
		0
		);
	hIconOut =
		(nIconOutId == NULL) ?
			NULL
			:
			(HICON)::LoadImage(
				hInstResource,
				MAKEINTRESOURCE(nIconOutId),
				IMAGE_ICON,
				0,
				0,
				0
				);
	SetIcon(hIconIn, hIconOut);
}

void CExtButton::SetIcon(HICON hIconIn, HICON hIconOut)
{
	if( m_hIconIn != NULL)
		::DestroyIcon( m_hIconIn );
	if( m_hIconOut != NULL)
		::DestroyIcon( m_hIconOut );

	m_hIconIn = hIconIn;
	m_hIconOut =
		(hIconOut == NULL) ?
			m_hIconIn : hIconOut;
	Invalidate();
	UpdateWindow();
} 

BOOL CExtButton::SetBtnCursor(int nCursorId)
{
HINSTANCE hInstResource;
	if( m_hCursor != NULL )
		::DestroyCursor(m_hCursor);
	m_hCursor = NULL;
	if( nCursorId != -1 )
	{
		hInstResource =
			AfxFindResourceHandle(
				MAKEINTRESOURCE(nCursorId),
				RT_GROUP_CURSOR
				);
		m_hCursor =
			(HCURSOR)::LoadImage(
				hInstResource,
				MAKEINTRESOURCE(nCursorId),
				IMAGE_CURSOR,
				0,
				0,
				0
				);
		if( m_hCursor == NULL )
			return FALSE;
	}
	return TRUE;
}

void CExtButton::SetFlat(BOOL bState)
{
	m_bIsFlat = bState;
	Invalidate();
	UpdateWindow();
}

BOOL CExtButton::GetFlat()
{
	return m_bIsFlat;
}

void CExtButton::SetAlign(int nAlign)
{
	switch( nAlign )
	{    
	case CExtPaintManager::e_align_t::__ALIGN_HORIZ_CENTER:
		m_nAlign = CExtPaintManager::e_align_t::__ALIGN_HORIZ_CENTER;
	break;
	case CExtPaintManager::e_align_t::__ALIGN_HORIZ_RIGHT:
		m_nAlign = CExtPaintManager::e_align_t::__ALIGN_HORIZ_RIGHT;
	break;
	case CExtPaintManager::e_align_t::__ALIGN_VERT:
		m_nAlign = CExtPaintManager::e_align_t::__ALIGN_VERT;
	break;
	} // switch( nAlign )
	Invalidate();
	UpdateWindow();
}

int CExtButton::GetAlign()
{
	return m_nAlign;
}

void CExtButton::SetDrawBorder(BOOL bEnable)
{
	m_bDrawBorder = bEnable;
	Invalidate();
	UpdateWindow();
}

bool CExtButton::_IsMouseOnButton()
{
POINT point;
	VERIFY( GetCursorPos( &point ) );
CWnd * pWndUnderMouse = WindowFromPoint( point );
	if( pWndUnderMouse->GetSafeHwnd() == m_hWnd )
		return true;
	return false;
}

void CExtButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bKeyTracking )
		return;

	CButton::OnMouseMove(nFlags, point);

	if( _IsMouseOnButton() )
	{ // if mouse is really on button
		if( GetCapture() != this )
			SetCapture();
		if( m_bMouseOver )
			return;
		m_bMouseOver = TRUE;
		m_bPushed =
			((nFlags & MK_LBUTTON) && m_bPushTracking )
				? TRUE : FALSE;
		Invalidate();
		UpdateWindow();
	} // if mouse is really on button
	else
	{ // if mouse is really NOT on button
		if( GetCapture() == this && ( !m_bPushTracking ) )
			ReleaseCapture();
		if( !m_bMouseOver )
			return;
		m_bMouseOver = m_bPushed = FALSE;
		Invalidate();
		UpdateWindow();
	} // if mouse is really NOT on button

	// RTF tool tip support
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
	if( nFlags & MK_LBUTTON )
		return;
	if( m_bToolTipSet )
		m_wndRtfToolTipCtrl.
			SetupRtfFromString(
				point, m_strToolTip );
#endif

}

void CExtButton::OnKillFocus(CWnd * pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	if( m_bMouseOver || m_bPushed )
	{
		if( GetCapture() == this )
			ReleaseCapture();
		m_bMouseOver = m_bPushed = FALSE;
		Invalidate();
		UpdateWindow();
	}
}

void CExtButton::OnCaptureChanged(CWnd *pWnd) 
{
	if( (m_bMouseOver || m_bPushed)
		&& pWnd != this
		)
	{
//		ReleaseCapture();
	// RTF tool tip support
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
		if( m_bToolTipSet )
			m_wndRtfToolTipCtrl.Hide();
#endif
		m_bMouseOver = m_bPushed = FALSE;
		Invalidate();
		UpdateWindow();
	}
	CButton::OnCaptureChanged(pWnd);
}

void CExtButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
UINT nBS = GetButtonStyle();
	if( nBS & BS_DEFPUSHBUTTON )
		m_bIsDefault = TRUE;
	if( nBS & BS_OWNERDRAW )
		SetButtonStyle( nBS & ~(BS_OWNERDRAW) );
	SetFont(
		m_bIsDefault?
			&g_PaintManager->m_FontBold
			:
			&g_PaintManager->m_FontNormal
			);
}

BOOL CExtButton::PreTranslateMessage(MSG* pMsg) 
{
	if( !CExtPopupMenuWnd::IsMenuTracking() )
	{
		InitToolTip();
		m_ToolTip.RelayEvent(pMsg);
	}

//	if( CExtPopupMenuWnd::IsMenuTracking() )
//		return TRUE;
	if(		(	pMsg->message == WM_KEYDOWN
				|| pMsg->message == WM_KEYUP
			)
		&&
			(	pMsg->wParam == VK_RETURN
				|| pMsg->wParam == VK_SPACE
			)
		)
	{
		if( IsWindowEnabled() && IsWindowVisible() && (!m_bPushTracking) )
		{
			if(	(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
				||
				(pMsg->message == WM_KEYUP && pMsg->wParam == VK_SPACE)
				)
			{
				if( GetCapture() == this )
					ReleaseCapture();
				m_bMouseOver = m_bPushed = m_bPushTracking = m_bKeyTracking = FALSE;
				Invalidate();
				UpdateWindow();
				_OnClick();
				return TRUE;
			}
			if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)
			{
				m_bMouseOver = m_bPushed = m_bKeyTracking = TRUE;
				SetCapture();
				Invalidate();
				UpdateWindow();
				return TRUE;
			}
		} // if( IsWindowEnabled() && IsWindowVisible() && (!m_bPushTracking) )
		return TRUE;
	}

	return CButton::PreTranslateMessage(pMsg);
}

LRESULT CExtButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
bool bInvalidate = false;
	switch( message )
	{
	case WM_LBUTTONDBLCLK:
		message = WM_LBUTTONDOWN;
	break;
	case BM_SETCHECK:
	case BM_SETSTATE:
	case BM_SETSTYLE:
#if (WINVER >= 0x0400)
	case BM_CLICK:
	case BM_SETIMAGE:
#endif
	case WM_SETTEXT:
	case WM_SIZE:
	case WM_WINDOWPOSCHANGED:
		bInvalidate = true;
	break;
	} // switch( message )

LRESULT lResult =
		CButton::DefWindowProc(message, wParam, lParam);
	if( bInvalidate )
	{
		Invalidate();
		UpdateWindow();
	}
	return lResult;
}

BOOL CExtButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if( m_hCursor != NULL )
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

void CExtButton::SetTooltipText(CString* spText, BOOL bActivate)
{
	if( spText == NULL )
		return;
	InitToolTip();
	if( m_ToolTip.GetToolCount() == 0 )
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this,(LPCTSTR)*spText,rectBtn,1);
	}
	m_ToolTip.UpdateTipText((LPCTSTR)*spText,this,1);
	m_ToolTip.Activate(bActivate);
}

void CExtButton::SetTooltipText(int nId, BOOL bActivate)
{
CString sText;
	sText.LoadString(nId);
	if( !sText.IsEmpty() )
		SetTooltipText(&sText, bActivate);
}

void CExtButton::ActivateTooltip(BOOL bActivate)
{
	if( m_ToolTip.GetToolCount() == 0 )
		return;
	m_ToolTip.Activate(bActivate);
}

BOOL CExtButton::GetDefault()
{
	return m_bIsDefault;
}

void CExtButton::InitToolTip()
{
	if( m_ToolTip.m_hWnd == NULL )
	{
		m_ToolTip.Create(this);
		m_ToolTip.Activate(FALSE);
	}
}

void CExtButton::OnSysColorChange() 
{
	CButton::OnSysColorChange();

	g_PaintManager->SyncSysColors();
	g_PaintManager->InitTranslatedColors();
}

void CExtButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// RTF tool tip support
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
	if( m_bToolTipSet )
		m_wndRtfToolTipCtrl.Hide();
#endif

	if( m_bKeyTracking )
		return;

	CButton::OnLButtonDown(nFlags, point);

	if( !_IsMouseOnButton() )
		return;
	m_bMouseOver = m_bPushed = m_bPushTracking = TRUE;
	SetCapture();
	
	Invalidate();
	UpdateWindow();
}

void CExtButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// RTF tool tip support
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
	if( m_bToolTipSet )
		m_wndRtfToolTipCtrl.Hide();
#endif

	if( m_bKeyTracking )
		return;

BOOL bWasPushed = m_bPushed && m_bPushTracking;
	m_bMouseOver = m_bPushed = m_bPushTracking = FALSE;
	if( GetCapture() == this )
		ReleaseCapture();
	Invalidate();
	UpdateWindow();

	if( !bWasPushed )
		return;

	//CButton::OnLButtonDown(nFlags, point);
	if( IsWindowEnabled()
		&& IsWindowVisible()
		)
		_OnClick();
}

void CExtButton::_OnClick()
{
	if( _OnTrackPopup() )
		return;
CWnd * pWnd = GetParent();
	ASSERT( pWnd != NULL );
	ASSERT_VALID( pWnd );
	if( m_menu.GetSafeHmenu() == NULL )
	{
		HWND hWndThis = GetSafeHwnd();
		pWnd->SendMessage(
			WM_COMMAND,
			MAKEWPARAM(GetDlgCtrlID(),BN_CLICKED),
			(LPARAM)m_hWnd
			);
		if( ::IsWindow(hWndThis) )
		{
			Invalidate();
			UpdateWindow();
		}
		return;
	}
}

bool CExtButton::_IsMenuAvail()
{
	if( m_menu.GetSafeHmenu() == NULL )
		return false;
	return true;
}

bool CExtButton::_OnTrackPopup()
{
	if( m_menu.GetSafeHmenu() == NULL )
		return false;
	ASSERT( ::IsMenu(m_menu.GetSafeHmenu()) );
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
			m_menu.GetSafeHmenu()
			)
		);
CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
CWnd * pWnd = GetParent();
	ASSERT( pWnd != NULL );
	ASSERT_VALID( pWnd );
	VERIFY(
		pPopup->UpdateFromMenu( pWnd->GetSafeHwnd(), &m_menu )
		);
CRect wr;
	GetWindowRect( &wr );
CPoint pt = wr.CenterPoint();
	pPopup->TrackPopupMenu(
		m_nMenuOpt,
		pt.x,
		pt.y,
		pWnd->GetSafeHwnd(),
		&wr,
		true, // bSelectAny
		true, // bPerformMessageLoop
		false, // bForceExpandRarelyUsed
		CExtPopupMenuWnd::__COMBINE_DEFAULT,
		this,
		_CbPaintCombinedContent
		);
	return true;
}

void CExtButton::_CbPaintCombinedContent(
	LPVOID pCookie,
	CDC & dc,
	const CWnd & refWndMenu,
	const CRect & rcExcludeArea, // in screen coords
	int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
	)
{
	pCookie;
	dc;
	refWndMenu;
	rcExcludeArea;
	eCombineAlign;
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( refWndMenu.GetSafeHwnd() != NULL );

CExtButton * pBtn = (CExtButton *)pCookie;
	ASSERT( pBtn != NULL );
	ASSERT_VALID( pBtn );
	ASSERT( pBtn->IsKindOf(RUNTIME_CLASS(CExtButton)) );
	
	ASSERT( eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE );

	if( rcExcludeArea.IsRectEmpty() )
		return;

CRect rcClientBar,rcClientExcludeArea;
	pBtn->GetClientRect( &rcClientBar );
	pBtn->ClientToScreen( &rcClientBar );
	refWndMenu.GetClientRect( &rcClientExcludeArea );
	refWndMenu.ClientToScreen( &rcClientExcludeArea );
CPoint ptOffset =
		rcClientBar.TopLeft()
		- rcClientExcludeArea.TopLeft()
		;
CPoint ptViewportOrg = dc.GetViewportOrg();
	dc.SetViewportOrg( ptOffset );
	pBtn->_RenderImpl(dc,true);
	dc.SetViewportOrg( ptViewportOrg );
}

void CExtButton::_RenderImpl(
	CDC & dc,
	bool bTransparent // = false
	)
{
CRect rectClient;
	GetClientRect( &rectClient );
CString sWindowText( _T("") );
	GetWindowText( sWindowText );
bool bDrawFocusRect = (GetFocus() == this) ? true : false;
bool bDefault = GetDefault() ? true : false;
bool bPushed = (m_bPushed || GetCheck()) ? true : false;
bool bEnabled = IsWindowEnabled() ? true : false;
bool bFlat = GetFlat() ? true : false;
bool bHover =
		( m_bMouseOver
		&& !CExtPopupMenuWnd::IsMenuTracking() )
			? true : false;
HICON hIcon = (bHover) ? m_hIconIn : m_hIconOut;
	if( GetParent()->IsKindOf(RUNTIME_CLASS(CControlBar)) )
	{
		bDrawFocusRect = false;
		bFlat = true;
	}
	g_PaintManager->PaintPushButton(
		dc, true, rectClient, (LPCTSTR)sWindowText,
		hIcon, bFlat, bHover, bPushed,
		false, bEnabled,
		m_bDrawBorder ? true : false,
		bDrawFocusRect, bDefault, m_nAlign,
		NULL, _IsMenuAvail(), 0, bTransparent
		);
}

void CExtButton::_DrawImpl(
	CDC & dc,
	bool bTransparent // = false
	)
{
CRect rectClient;
	GetClientRect( &rectClient );
CExtMemoryDC dcmm( &dc, &rectClient );
	_RenderImpl(dcmm,bTransparent);
}

void CExtButton::OnPaint() 
{
CPaintDC dcPaint( this );
	_DrawImpl( dcPaint );
}

BOOL CExtButton::OnEraseBkgnd(CDC* pDC) 
{
///	return CButton::OnEraseBkgnd(pDC);
	return TRUE;
}

void CExtButton::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
CClientDC dc(this);
	_DrawImpl( dc );
}

CExtColorButton::CExtColorButton()
{
	m_bUseStdColorDlg = false;
	m_bEnableBtnColorDefault = true;
	m_bEnableBtnColorCustom = true;
	m_clrDefault = (COLORREF)(-1); // unexisting
	m_clrSelected = RGB(0,0,0);
}

CExtColorButton::~CExtColorButton()
{
	_FreeWinObjects();
}

IMPLEMENT_DYNCREATE(CExtColorButton, CExtButton);

BEGIN_MESSAGE_MAP(CExtColorButton, CExtButton)
    //{{AFX_MSG_MAP(CExtColorButton)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::nMsgNotifyColorChanged,
		OnColorChanged
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::nMsgNotifyCustColor,
		OnColorSelectCustom
		)
END_MESSAGE_MAP()

bool CExtColorButton::_IsMenuAvail()
{
	return true;
}

bool CExtColorButton::_OnTrackPopup()
{
CExtPopupColorMenuWnd * pPopup =
		new CExtPopupColorMenuWnd;
	VERIFY(
		pPopup->CreatePopupMenu( GetSafeHwnd() )
		);
	pPopup->m_bEnableBtnColorDefault =
		m_bEnableBtnColorDefault;
	pPopup->m_bEnableBtnColorCustom =
		m_bEnableBtnColorCustom;
	pPopup->m_clrDefault =
		m_clrDefault;
	pPopup->m_clrInitial =
		m_clrSelected;

CRect wr;
	GetWindowRect( &wr );
CPoint pt = wr.CenterPoint();
	pPopup->TrackPopupMenu(
		m_nMenuOpt,
		pt.x,
		pt.y,
		GetSafeHwnd(),
		&wr,
		true, // bSelectAny
		true, // bPerformMessageLoop
		false, // bForceExpandRarelyUsed
		CExtPopupMenuWnd::__COMBINE_DEFAULT,
		this,
		_CbPaintCombinedContent
		);
	return true;
}

void CExtColorButton::_RenderImpl( // non-buffered paint
	CDC & dc,
	bool bTransparent // = false
	)
{
ICONINFO iiNewIcon;
	iiNewIcon.fIcon = true;	// set that it is an icon
	// create DC's
HDC hIconDC = ::CreateCompatibleDC( dc.GetSafeHdc() );
HDC hMaskDC = ::CreateCompatibleDC( dc.GetSafeHdc() );
	ASSERT(
		hIconDC != NULL && hMaskDC != NULL
		);
	// load bitmaps
	iiNewIcon.hbmColor = ::CreateCompatibleBitmap(
		dc.GetSafeHdc(),
		16,
		16
		);
	ASSERT( iiNewIcon.hbmColor != NULL );
	iiNewIcon.hbmMask = ::CreateCompatibleBitmap(
		hMaskDC,
		16,
		16
		);
	ASSERT( iiNewIcon.hbmMask != NULL );
HGDIOBJ hOldIconDC =
		::SelectObject(
			hIconDC,
			iiNewIcon.hbmColor
			);
	ASSERT( hOldIconDC != NULL );
HGDIOBJ hOldMaskDC =
		::SelectObject(
			hMaskDC,
			iiNewIcon.hbmMask
			);
	ASSERT( hOldMaskDC != NULL );

CBrush brColorItem( m_clrSelected );
CRect rcColorItem(0,0,16,16);
	::FillRect(
		hIconDC,
		&rcColorItem,
		(HBRUSH)brColorItem.GetSafeHandle()
		);
	::FillRect(
		hMaskDC,
		&rcColorItem,
		(HBRUSH)::GetStockObject(BLACK_BRUSH)
		);

	::SelectObject(hIconDC,hOldIconDC);
	::SelectObject(hMaskDC,hOldMaskDC);
HICON hIcon = CreateIconIndirect(&iiNewIcon);
	ASSERT( hIcon != NULL );
	// cleanup
	DeleteObject(iiNewIcon.hbmColor);
	DeleteObject(iiNewIcon.hbmMask);
	DeleteDC(hMaskDC);
	DeleteDC(hIconDC);



CRect rectClient;
	GetClientRect( &rectClient );
CString sWindowText( _T("") );
	GetWindowText( sWindowText );
bool bDrawFocusRect = (GetFocus() == this) ? true : false;
bool bDefault = GetDefault() ? true : false;
bool bPushed = (m_bPushed || GetCheck()) ? true : false;
bool bEnabled = IsWindowEnabled() ? true : false;
bool bFlat = GetFlat() ? true : false;
bool bHover =
		( m_bMouseOver
		&& !CExtPopupMenuWnd::IsMenuTracking() )
			? true : false;
	if( GetParent()->IsKindOf(RUNTIME_CLASS(CControlBar)) )
	{
		bDrawFocusRect = false;
		bFlat = true;
	}
	g_PaintManager->PaintPushButton(
		dc, true, rectClient, (LPCTSTR)sWindowText,
		hIcon, bFlat, bHover, bPushed,
		false, bEnabled,
		(m_bDrawBorder && (!CExtPopupMenuWnd::IsMenuTracking()))
			? true : false,
		(bDrawFocusRect && (!CExtPopupMenuWnd::IsMenuTracking())
			&& (!sWindowText.IsEmpty()) )
			? true : false,
		bDefault, m_nAlign,
		NULL, _IsMenuAvail(), 0, bTransparent
		);

	if( hIcon != NULL )
	{
		::DestroyIcon( hIcon );
	}
}

LRESULT CExtColorButton::OnColorChanged(WPARAM wParam, LPARAM lParam)
{
/*
	m_clrSelected = (COLORREF)wParam;
	Invalidate();
	UpdateWindow();
CExtPopupMenuWnd * pPopup =
		CExtPopupMenuWnd::GetTrackingMenu();
	if( pPopup != NULL
		&& pPopup->GetSafeHwnd() != NULL
		)
		pPopup->Invalidate();
*/

	return
		GetParent()->SendMessage(
			CExtPopupColorMenuWnd::nMsgNotifyColorChanged,
			wParam,
			lParam
			);
//	return 0;
}

LRESULT CExtColorButton::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
{
	m_clrSelected = (COLORREF)wParam;
	Invalidate();
	UpdateWindow();
	return
		GetParent()->SendMessage(
			CExtPopupColorMenuWnd::nMsgNotifyColorChangedFinally,
			wParam,
			lParam
			);
//	return 0;
}

LRESULT CExtColorButton::OnColorSelectCustom(WPARAM wParam, LPARAM lParam)
{
	if( m_bUseStdColorDlg )
	{
		CColorDialog dlg(m_clrSelected,CC_FULLOPEN,this);
		if( dlg.DoModal() != IDOK )
			return 0;
		m_clrSelected = dlg.m_cc.rgbResult;
	} // if( m_bUseStdColorDlg )
	else
	{
		CExtColorDlg dlg( m_clrSelected,m_clrSelected,this );
		if( dlg.DoModal() != IDOK )
			return 0;
		m_clrSelected = dlg.m_clrNew;
	} // else from if( m_bUseStdColorDlg )
	Invalidate();
	UpdateWindow();
	return 0;
}
