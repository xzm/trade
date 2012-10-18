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

//
// Mostly based on the Paolo Messina's idea of the resizable dialog
// with anchoring its inner child windows to its borders
//
// The following changes have been made:
//   - simplified repositioning
//   - redrawing appearance
//   - system menu
//
// Original credits:
//
// Copyright (C) 2000 by Paolo Messina
// (ppescher@yahoo.com)
//

#if (!defined __EXT_RESIZABLE_DIALOG_H)
	#include <ExtResizableDialog.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_TIMER_SYS_TRACKING 301

/////////////////////////////////////////////////////////////////////////////
// CExtResizableDialog

IMPLEMENT_DYNAMIC( CExtResizableDialog, CDialog )

CExtResizableDialog::CExtResizableDialog()
	: m_bInitDone( false )
	, m_bSysMenuTracking( false )
	, m_bUseMinTrack( true )
	, m_bUseMaxTrack( false )
	, m_bUseMaxRect( false )
	, m_bShowGrip( true )
	, m_bEnableSaveRestore( false )
{
}

CExtResizableDialog::CExtResizableDialog(
	UINT nIDTemplate,
	CWnd * pParentWnd
	)
	: CDialog(nIDTemplate, pParentWnd)
	, m_bInitDone( false )
	, m_bSysMenuTracking( false )
	, m_bUseMinTrack( true )
	, m_bUseMaxTrack( false )
	, m_bUseMaxRect( false )
	, m_bShowGrip( true )
	, m_bEnableSaveRestore( false )
{
}

CExtResizableDialog::CExtResizableDialog(
	LPCTSTR lpszTemplateName,
	CWnd * pParentWnd
	)
	: CDialog(lpszTemplateName, pParentWnd)
	, m_bInitDone( false )
	, m_bSysMenuTracking( false )
	, m_bUseMinTrack( true )
	, m_bUseMaxTrack( false )
	, m_bUseMaxRect( false )
	, m_bShowGrip( true )
	, m_bEnableSaveRestore( false )
{
}

BEGIN_MESSAGE_MAP(CExtResizableDialog, CDialog)
	//{{AFX_MSG_MAP(CExtResizableDialog)
	ON_WM_NCHITTEST()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CExtResizableDialog message handlers


BOOL CExtResizableDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// gets the template size as the min track size
	CRect rc;
	GetWindowRect(&rc);
	m_ptMinTrackSize.x = rc.Width();
	m_ptMinTrackSize.y = rc.Height();

	m_bInitDone = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExtResizableDialog::OnDestroy() 
{
	if( m_bEnableSaveRestore )
		SaveWindowRect();

	CDialog::OnDestroy();
}

void CExtResizableDialog::OnPaint() 
{
CPaintDC dc( this );
	if( !m_bShowGrip || IsZoomed() )
		return;
CRect rcGrip;
	GetClientRect( &rcGrip );
	rcGrip.left = rcGrip.right - ::GetSystemMetrics( SM_CXVSCROLL );
	rcGrip.top = rcGrip.bottom - ::GetSystemMetrics( SM_CYHSCROLL );
CFont * pOldFont =
		dc.SelectObject(
			&g_PaintManager->m_FontMarlett
			);
	ASSERT( pOldFont != NULL );
	COLORREF clrOld = dc.SetTextColor(
		g_PaintManager->GetColor(
			CExtPaintManager::CLR_TEXT_OUT
			)
		);
	int nOldBkMode = dc.SetBkMode(TRANSPARENT);
	dc.DrawText(
		_T("o"), 1, rcGrip,
		DT_SINGLELINE|DT_RIGHT|DT_BOTTOM
		);
	dc.SetBkMode(nOldBkMode);
	dc.SetTextColor(clrOld);
	dc.SelectObject(pOldFont);
}

void CExtResizableDialog::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if( nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW )
		return;		// arrangement not needed

	if( m_bInitDone )
		ArrangeLayout( cx, cy );
}

UINT CExtResizableDialog::OnNcHitTest(CPoint point) 
{
	if( m_bShowGrip )
	{
		CPoint ptClient( point );
		ScreenToClient( &ptClient );
		CRect rcGrip;
		GetClientRect( &rcGrip );
		rcGrip.left = rcGrip.right - ::GetSystemMetrics( SM_CXVSCROLL );
		rcGrip.top = rcGrip.bottom - ::GetSystemMetrics( SM_CYHSCROLL );
		if( rcGrip.PtInRect(ptClient) )
			return HTBOTTOMRIGHT;
	} // if( m_bShowGrip )
	
	return CDialog::OnNcHitTest(point);
}

void CExtResizableDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if( !m_bInitDone )
		return;

	if (m_bUseMinTrack)
		lpMMI->ptMinTrackSize = m_ptMinTrackSize;

	if (m_bUseMaxTrack)
		lpMMI->ptMaxTrackSize = m_ptMaxTrackSize;

	if (m_bUseMaxRect)
	{
		lpMMI->ptMaxPosition = m_ptMaxPos;
		lpMMI->ptMaxSize = m_ptMaxSize;
	}
}

// layout functions

bool CExtResizableDialog::AddAnchor(
	UINT nDlgCtrlID,
	const CRect & rcAnch
	)
{
	if(	GetSafeHwnd() == NULL
		|| !::IsWindow( GetSafeHwnd() )
		)
	{
		ASSERT( FALSE );
		return false;
	}
HWND hWnd = ::GetDlgItem( GetSafeHwnd(), nDlgCtrlID );
	if( hWnd == NULL || !::IsWindow(hWnd) )
	{
		ASSERT( FALSE );
		return false;
	}
	return AddAnchor( hWnd, rcAnch );
}

bool CExtResizableDialog::AddAnchor(
	HWND hWnd,
	const CRect & rcAnch
	)
{
	if(	GetSafeHwnd() == NULL
		|| !::IsWindow( GetSafeHwnd() )
		)
	{
		ASSERT( FALSE );
		return false;
	}
	return
		AddAnchor(
			hWnd,
			CSize( rcAnch.left, rcAnch.top ),
			CSize( rcAnch.right, rcAnch.bottom )
			);
}

bool CExtResizableDialog::AddAnchor(
	UINT nDlgCtrlID,
	CSize AnchLT,
	CSize AnchRB // = __RDA_NONE
	)
{
	if( AnchLT == __RDA_NONE
		|| GetSafeHwnd() == NULL
		|| !::IsWindow( GetSafeHwnd() )
		)
	{
		ASSERT( FALSE );
		return false;
	}
HWND hWnd = ::GetDlgItem( GetSafeHwnd(), nDlgCtrlID );
	if( hWnd == NULL || !::IsWindow(hWnd) )
	{
		ASSERT( FALSE );
		return false;
	}
	return AddAnchor( hWnd, AnchLT, AnchRB );
}

bool CExtResizableDialog::AddAnchor(
	HWND hWnd,
	CSize AnchLT,
	CSize AnchRB // = __RDA_NONE
	)
{
	if( AnchLT == __RDA_NONE
		|| GetSafeHwnd() == NULL
		|| !::IsWindow( GetSafeHwnd() )
		|| !::IsChild( GetSafeHwnd(), hWnd )
		)
	{
		ASSERT( FALSE );
		return false;
	}

CRect rcClient, rcItem;
	GetClientRect( &rcClient );
int cx = rcClient.Width();
int cy = rcClient.Height();
	::GetWindowRect( hWnd, &rcItem );
	ScreenToClient( &rcItem );
	
CSize MargLT, MargRB;
	if( AnchRB == __RDA_NONE )
		AnchRB = AnchLT;
	
	// calculate margin for the top-left corner
	MargLT.cx = rcItem.left - cx*AnchLT.cx/100;
	MargLT.cy = rcItem.top - cy*AnchLT.cy/100;
	
	// calculate margin for the bottom-right corner
	MargRB.cx = rcItem.right - cx*AnchRB.cx/100;
	MargRB.cy = rcItem.bottom - cy*AnchRB.cy/100;

	// add to the list
	m_mapRDI.SetAt(
		hWnd,
		RDI_t( AnchLT, MargLT, AnchRB, MargRB )
		);

	return true;
}

void CExtResizableDialog::ArrangeLayout(
	int cx, // = -1
	int cy  // = -1
	)
{
	if( cx < 0 || cy < 0 )
	{
		CRect rcClient;
		GetClientRect( &rcClient );
		cx = rcClient.Width();
		cy = rcClient.Height();
	} // if( cx < 0 || cy < 0 )

	SetRedraw( FALSE );

POSITION pos = m_mapRDI.GetStartPosition();
	while( pos != NULL )
	{
		HWND _hWnd;
		RDI_t _rdi;
		m_mapRDI.GetNextAssoc( pos, _hWnd, _rdi );
		ASSERT( ::IsWindow( _hWnd ) );
		if( !::IsWindow( _hWnd ) )
			continue;
		int x =	_rdi.m_MargLT.cx + cx*_rdi.m_AnchLT.cx/100;
		int y = _rdi.m_MargLT.cy + cy*_rdi.m_AnchLT.cy/100;
		::SendMessage( _hWnd, WM_SETREDRAW, FALSE, 0 );
		::MoveWindow(
			_hWnd,
			x,
			y,
			_rdi.m_MargRB.cx + cx*_rdi.m_AnchRB.cx/100 - x,
			_rdi.m_MargRB.cy + cy*_rdi.m_AnchRB.cy/100 - y,
			FALSE
			);
	}

	pos = m_mapRDI.GetStartPosition();
	while( pos != NULL )
	{
		HWND _hWnd;
		RDI_t _rdi;
		m_mapRDI.GetNextAssoc( pos, _hWnd, _rdi );
		ASSERT( ::IsWindow( _hWnd ) );
		if( !::IsWindow( _hWnd ) )
			continue;
		_hWnd;
		_rdi;
		::SendMessage( _hWnd, WM_SETREDRAW, TRUE, 0 );
	}

	SetRedraw( TRUE );
	RedrawWindow(
		NULL,
		NULL,
		RDW_ERASE|RDW_INVALIDATE
			|RDW_UPDATENOW
			|RDW_ALLCHILDREN
		);
}

// protected members
void CExtResizableDialog::ShowSizeGrip(BOOL bShow)
{
bool _bShow = bShow ? true : false;
	if( m_bShowGrip != _bShow )
	{
		m_bShowGrip = _bShow;
		CRect rcGrip;
		GetClientRect( &rcGrip );
		rcGrip.left = rcGrip.right - ::GetSystemMetrics( SM_CXVSCROLL );
		rcGrip.top = rcGrip.bottom - ::GetSystemMetrics( SM_CYHSCROLL );
		InvalidateRect( &rcGrip );
	}
}

void CExtResizableDialog::SetMaximizedRect(const CRect& rc)
{
	m_bUseMaxRect = TRUE;

	m_ptMaxPos = rc.TopLeft();
	m_ptMaxSize.x = rc.Width();
	m_ptMaxSize.y = rc.Height();
}

void CExtResizableDialog::ResetMaximizedRect()
{
	m_bUseMaxRect = FALSE;
}

void CExtResizableDialog::SetMinTrackSize(const CSize& size)
{
	m_bUseMinTrack = TRUE;

	m_ptMinTrackSize.x = size.cx;
	m_ptMinTrackSize.y = size.cy;
}

void CExtResizableDialog::ResetMinTrackSize()
{
	m_bUseMinTrack = FALSE;
}

void CExtResizableDialog::SetMaxTrackSize(const CSize& size)
{
	m_bUseMaxTrack = TRUE;

	m_ptMaxTrackSize.x = size.cx;
	m_ptMaxTrackSize.y = size.cy;
}

void CExtResizableDialog::ResetMaxTrackSize()
{
	m_bUseMaxTrack = FALSE;
}

// NOTE: this must be called after all the other settings
//       to have the dialog and its controls displayed properly
void CExtResizableDialog::EnableSaveRestore(
		LPCTSTR pszSection,
		LPCTSTR pszEntry,
		bool bForceHideWindow // = false
		)
{
	m_sSection = pszSection;
	m_sEntry = pszEntry;

	m_bEnableSaveRestore = TRUE;

	LoadWindowRect( bForceHideWindow );
}

// used to save/restore window's size and position
// either in the registry or a private .INI file
// depending on your application settings

#define PROFILE_FMT 	_T("%d,%d,%d,%d,%d,%d")

void CExtResizableDialog::SaveWindowRect()
{
	CString data;
	WINDOWPLACEMENT wp;

	ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);
	
	RECT& rc = wp.rcNormalPosition;	// alias

	data.Format(PROFILE_FMT, rc.left, rc.top,
		rc.right, rc.bottom, wp.showCmd, wp.flags);

	AfxGetApp()->WriteProfileString(m_sSection, m_sEntry, data);
}

void CExtResizableDialog::LoadWindowRect(
	bool bForceHideWindow // = false
	)
{
	CString data;
	WINDOWPLACEMENT wp;

	data = AfxGetApp()->GetProfileString(m_sSection, m_sEntry);
	
	if (data.IsEmpty())	// never saved before
		return;
	
	ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);

RECT& rc = wp.rcNormalPosition;	// alias

	if (_stscanf(data, PROFILE_FMT, &rc.left, &rc.top,
		&rc.right, &rc.bottom, &wp.showCmd, &wp.flags) == 6)
	{
		if( bForceHideWindow )
			wp.showCmd = SW_HIDE;
		SetWindowPlacement(&wp);
	}
	ArrangeLayout();
}

void CExtResizableDialog::OnOK()
{
DWORD dwStyle = GetStyle();
	if( (dwStyle & WS_CHILD) == 0 )
		CDialog::OnOK();
}

void CExtResizableDialog::OnCancel()
{
DWORD dwStyle = GetStyle();
	if( (dwStyle & WS_CHILD) == 0 )
		CDialog::OnCancel();
}

HBRUSH CExtResizableDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if( nCtlColor == CTLCOLOR_DLG
		|| nCtlColor == CTLCOLOR_STATIC
		|| nCtlColor == CTLCOLOR_MSGBOX
		)
	{
		if( nCtlColor == CTLCOLOR_STATIC )
			pDC->SetBkMode( TRANSPARENT );
		static CBrush brBackground;
		if( brBackground.GetSafeHandle() == NULL )
		{
			VERIFY(
				brBackground.CreateSolidBrush(
					g_PaintManager->GetColor(
						CExtPaintManager::CLR_3DFACE_OUT
						)
					)
				);
		} // if( brBackground.GetSafeHandle() == NULL )
		return (HBRUSH)brBackground.GetSafeHandle();
	}
HBRUSH hBrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hBrush;
}

BOOL CExtResizableDialog::OnEraseBkgnd(CDC* pDC) 
{
	CExtPaintManager::stat_ExcludeChildAreas(
		pDC->GetSafeHdc(),
		GetSafeHwnd()
		);
	return CDialog::OnEraseBkgnd(pDC);
}

LRESULT CExtResizableDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( (message == WM_NCLBUTTONUP && wParam == HTSYSMENU)
		|| (message == WM_NCRBUTTONUP && (wParam == HTCAPTION || wParam == HTSYSMENU))
		)
	{
		_TrackDialogSystemMenu(
			NULL,
			FALSE
			);
		return 0;
	}
	if( (message == WM_NCLBUTTONDOWN || message == WM_NCRBUTTONDOWN)
		&& wParam == HTSYSMENU
		)
		return 0;
	if( message == WM_NCRBUTTONDOWN && wParam == HTCAPTION )
	{
///		CExtToolControlBar::g_bMenuTrackingExpanded = false;
		CPoint point;
		VERIFY( GetCursorPos(&point) );
		_TrackDialogSystemMenu(
			&point,
			FALSE
			);
		return 0;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CExtResizableDialog::PreTranslateMessage(MSG* pMsg) 
{
UINT message = pMsg->message;
WPARAM wParam = pMsg->wParam;
LPARAM lParam = pMsg->lParam;
	if(	   message == WM_SYSKEYDOWN
		|| message == WM_SYSKEYUP
		|| message == WM_KEYDOWN
		//|| message == WM_KEYUP
		)
	{
		BOOL bKeyUp =
			(	// nMsg == WM_KEYUP ||
				message == WM_SYSKEYUP) ?
					TRUE : FALSE;
//		BOOL bCtrl = GetKeyState(VK_CONTROL) & 0x80000000;
//		BOOL bShift = GetKeyState(VK_SHIFT) & 0x80000000;
		BOOL bAlt = HIWORD(lParam) & KF_ALTDOWN;
		TCHAR vkTCHAR = (TCHAR)wParam;
		if( bAlt && vkTCHAR == VK_SPACE )
		{
			if( !bKeyUp )
			{
				// allow child popup track it's system menu
				CWnd * pWnd = CWnd::GetActiveWindow();
				if( pWnd == NULL
					|| (!(::IsWindow(pWnd->GetSafeHwnd())))
					)
					return TRUE;
				if( pWnd->GetSafeHwnd() != GetSafeHwnd()
					&& (pWnd->GetStyle() & WS_POPUP) != 0
					)
					return TRUE;

				// track dialog system menu
///				CExtToolControlBar::g_bMenuTrackingExpanded = false;
///				CExtToolControlBar::g_bMenuTrackingExpanded = false;
				_TrackDialogSystemMenu(NULL,TRUE);
			}
			return 1; // 0
		} // if( bAlt && vkTCHAR == VK_SPACE )
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CExtResizableDialog::_TrackDialogSystemMenu(
	CPoint * pPoint,
	BOOL bSelectAny
	)
{
	if( (GetStyle() & WS_POPUP) == 0 )
		return FALSE;
	if(	CExtPopupMenuWnd::IsMenuTracking() )
		CExtPopupMenuWnd::CancelMenuTracking();
	m_bSysMenuTracking = FALSE;

CMenu * pSysMenu =
		GetSystemMenu( FALSE );
	if( pSysMenu == NULL )
		return FALSE;
	ASSERT( ::IsMenu(pSysMenu->GetSafeHmenu()) );
CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
	if( !pPopup->CreatePopupMenu( GetSafeHwnd() ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	if( !pPopup->UpdateFromMenu(
			GetSafeHwnd(),
			pSysMenu,
			false
			)
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}

CPoint point;
	if( pPoint != NULL)
		point = *pPoint;
	else
	{
		CRect rcWnd, rcClient, rcHelper;
		GetWindowRect( &rcWnd );
		GetClientRect( &rcClient );
		rcHelper = rcWnd;
		ScreenToClient( &rcHelper );
		ASSERT( rcHelper.top <= rcClient.top );
		int yDiff = rcClient.top - rcHelper.top;
		point = rcWnd.TopLeft();
		point.x += GetSystemMetrics(SM_CXFRAME);
		point.y += yDiff;
	}
	m_bSysMenuTracking =
		pPopup->TrackPopupMenu(
			TPMX_BOTTOMALIGN,
			point.x,point.y,
			GetSafeHwnd(),
			NULL,
			bSelectAny ? true : false
			) ? true : false;

	if( m_bSysMenuTracking )
		SetTimer( ID_TIMER_SYS_TRACKING, 20, NULL );
	return TRUE;
}

void CExtResizableDialog::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == ID_TIMER_SYS_TRACKING )
	{
		if( !CExtPopupMenuWnd::IsMenuTracking() )
		{
			m_bSysMenuTracking = FALSE;
			KillTimer( ID_TIMER_SYS_TRACKING );
		}
		return;
	}
	
	CDialog::OnTimer(nIDEvent);
}
