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

#if (!defined __EXT_COMBO_BOX_H)
	#include <ExtComboBox.h>
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

/////////////////////////////////////////////////////////////////////////////
// CExtComboBox

CExtComboBox::CExtComboBox()
	: m_bAutoComplete( true )
	, m_bEnableAutoComplete( true )
	, m_bDrawing( false )
	, m_dwUpdateTimer( 10 )
	, m_dwUpdatePeriod( 50 )
{
}

CExtComboBox::~CExtComboBox()
{
}

IMPLEMENT_DYNCREATE(CExtComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CExtComboBox, CComboBox)
	//{{AFX_MSG_MAP(CExtComboBox)
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT( CBN_EDITUPDATE, OnEditCtrlUpdate )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtComboBox message handlers

void CExtComboBox::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent != m_dwUpdateTimer )
	{
		CComboBox::OnTimer( nIDEvent );
		return;
	} // if( nIDEvent != m_dwUpdateTimer )

POINT ptCursorPos;
	VERIFY( GetCursorPos( &ptCursorPos) );
CRect rcItem;
	GetWindowRect(&rcItem);

	//if( m_bLBtnDown )
	if( CExtPopupMenuWnd::IsKeyPressed(MK_LBUTTON) )
	{
		if( !GetDroppedState() )
			KillTimer( m_dwUpdateTimer );
		if( m_bDrawing )
		{
			_OnDrawComboImpl(
				GetDroppedState() ? true : false,
				false
				);
			m_bDrawing = false;
		} // if( CExtPopupMenuWnd::IsKeyPressed(MK_LBUTTON) )
		return;
	} // if( m_bLBtnDown )
	
	if( !rcItem.PtInRect(ptCursorPos) )
	{
		if( !GetDroppedState() )
			KillTimer( m_dwUpdateTimer );
		_OnDrawComboImpl( false, false );
		m_bDrawing = false;
		return;
	} // if( !rcItem.PtInRect(ptCursorPos) )

	if( m_bDrawing )
		return;
	m_bDrawing = true;
	_OnDrawComboImpl( false, true );
}

void CExtComboBox::_OnPaintImpl(
	bool bPressed,
	bool bHover
	)
{
CRect rcClient;
	GetClientRect( &rcClient );
CPaintDC dcPaint( this );
	CExtPaintManager::stat_ExcludeChildAreas(
		dcPaint.GetSafeHdc(),
		GetSafeHwnd()
		);
CExtMemoryDC dcmm( &dcPaint, &rcClient );
	DefWindowProc( WM_PAINT, (WPARAM)dcmm.GetSafeHdc(), (LPARAM)0 );
	_OnDrawComboImpl( bPressed, bHover, &dcmm );
}

void CExtComboBox::OnPaint() 
{
	if(	(GetExStyle() &
			(WS_EX_DLGMODALFRAME|WS_EX_CLIENTEDGE|WS_EX_STATICEDGE
			)) != 0
		)
		ModifyStyleEx(
			WS_EX_DLGMODALFRAME|WS_EX_CLIENTEDGE|WS_EX_STATICEDGE,
			0,
			SWP_FRAMECHANGED
			);
CPoint ptCursorPos;
	VERIFY( GetCursorPos( &ptCursorPos ) );
CRect rcItem;
	GetWindowRect( &rcItem );

	if( rcItem.PtInRect(ptCursorPos) )
	{
		if( m_bDrawing )
			return;
		_OnPaintImpl( false, true );
		return;
	} // if( rcItem.PtInRect(ptCursorPos) )

	_OnPaintImpl( false, false );
}

void CExtComboBox::_OnDrawComboImpl(
	bool bPressed,
	bool bHover,
	CDC * pDC // = NULL
	)
{

CRect rectClient;
	GetClientRect( &rectClient );
bool bCallReleaseDC = false;
	if( pDC == NULL )
	{
		pDC = GetDC();
		ASSERT( pDC != NULL );
		bCallReleaseDC = true;
	}
	
bool bEnabled = IsWindowEnabled() ? true : false;
bool bPushed =
		( bPressed )
		|| GetDroppedState()
		;
	if( !bEnabled )
	{
		bPushed = false;
		bHover = false;
	}
	if( CExtPopupMenuWnd::IsMenuTracking() )
		bHover = false;

	g_PaintManager->PaintComboFrame(
		*pDC,
		rectClient,
		bHover,
		bPushed,
		bEnabled
		);

	if( bCallReleaseDC )
		ReleaseDC( pDC );
}

BOOL CExtComboBox::PreTranslateMessage(MSG* pMsg) 
{
	if( m_bEnableAutoComplete )
	{
		if( pMsg->message == WM_KEYDOWN )
		{
			m_bAutoComplete = true;
			int nVirtKey = (int) pMsg->wParam;
			if(nVirtKey == VK_DELETE || nVirtKey == VK_BACK)
				m_bAutoComplete = false;
		}
		if( pMsg->message == WM_SYSCHAR )
			return TRUE;
	} // if( m_bEnableAutoComplete )
	else
	{
		m_bAutoComplete = false;
	} // else from if( m_bEnableAutoComplete )

		
	return CComboBox::PreTranslateMessage(pMsg);
}

void CExtComboBox::OnEditCtrlUpdate() 
{
	if( !m_bEnableAutoComplete )
	{
		Default();
		return;
	}
	if( !m_bAutoComplete ) 
		return;
CString str;
	GetWindowText( str );
int nLength = str.GetLength();
DWORD dwCurSel = GetEditSel();
WORD dStart = LOWORD( dwCurSel );
WORD dEnd   = HIWORD( dwCurSel );
	if( SelectString( -1, str ) == CB_ERR )
	{
		SetWindowText( str );
		if( dwCurSel != CB_ERR )
			SetEditSel( dStart, dEnd );
	}
	if( dEnd < nLength && dwCurSel != CB_ERR )
		SetEditSel( dStart, dEnd );
	else
		SetEditSel( nLength, -1 );
}

BOOL CExtComboBox::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
	return TRUE;
}

LRESULT CExtComboBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch( message )
	{
	case WM_ENABLE:
		{
			HWND hWndEdit =
				::GetWindow( GetSafeHwnd(), GW_CHILD );
			if( hWndEdit == NULL || !::IsWindow(hWndEdit) )
				break;
			::EnableWindow( hWndEdit, TRUE );
			::SendMessage( hWndEdit, EM_SETREADONLY, !wParam, 0L );
			Invalidate();
		}
		break;
	case WM_LBUTTONUP:
		Invalidate();
		break;
	case WM_MOUSEMOVE:
		SetTimer( m_dwUpdateTimer, m_dwUpdatePeriod, NULL );
		OnTimer( m_dwUpdateTimer );
		break;
	case WM_SETFOCUS:
		{
			LRESULT lResult =
				CComboBox::WindowProc(message, wParam, lParam);;
			_OnDrawComboImpl( false, true );
			return lResult;
		}
	case WM_KILLFOCUS:
		{
			LRESULT lResult =
				CComboBox::WindowProc(message, wParam, lParam);;
			if( GetDroppedState() )
				ShowDropDown( FALSE );
			_OnDrawComboImpl( false, false );
			return lResult;
		}
	} // switch( message )
	
	return CComboBox::WindowProc(message, wParam, lParam);
}
