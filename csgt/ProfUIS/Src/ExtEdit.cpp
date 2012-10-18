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

#if (!defined __EXT_EDIT_H)
	#include <ExtEdit.h>
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

extern CWinApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CExtEdit

CExtEdit::CExtEdit()
{
	m_bMouseOver = false;
}

CExtEdit::~CExtEdit()
{
}

BEGIN_MESSAGE_MAP(CExtEdit, CEdit)
	//{{AFX_MSG_MAP(CExtEdit)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//	ON_WM_SYSCOLORCHANGE()

/////////////////////////////////////////////////////////////////////////////
// CExtEdit message handlers
void CExtEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetTimer(1, 50, NULL);
	CEdit::OnMouseMove(nFlags, point);
}

void CExtEdit::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent != 1 )
	{
		CEdit::OnTimer( nIDEvent );
		return;
	}
POINT pt;
VERIFY( ::GetCursorPos(&pt) );
CRect rectItem;
	GetWindowRect(&rectItem);
bool bOldMouseOver = m_bMouseOver;
	if( !rectItem.PtInRect(pt) )
	{
		KillTimer(1);
		m_bMouseOver = false;
	}
	else
		m_bMouseOver = true;
	if( bOldMouseOver != m_bMouseOver )
		_PostRedraw();
}

void CExtEdit::OnPaint() 
{
CRect rectClient;
	GetClientRect( &rectClient );
CPaintDC dcPaint( this );
CExtMemoryDC dcmm( &dcPaint, &rectClient );
	DefWindowProc( WM_PAINT, (WPARAM)dcmm.GetSafeHdc(), (LPARAM)0 );
}

void CExtEdit::_PostRedraw()
{
/*
	SetWindowPos(
		NULL,0,0,0,0,
		SWP_NOCOPYBITS|SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE
			|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOSENDCHANGING
			|SWP_FRAMECHANGED
		);
*/
	VERIFY(
		RedrawWindow(
			NULL,
			NULL,
			RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW
				|RDW_FRAME|RDW_ALLCHILDREN
			)
		);
}

void CExtEdit::_DrawEditImpl(
	CRect rectClient,
	CDC * pDC // = NULL
	)
{
bool bNeedReleaseDC = false;
	if( pDC == NULL )
	{
		pDC = GetDC();
		bNeedReleaseDC = true;
	}
	ASSERT_VALID( pDC );
	rectClient.DeflateRect(1,1);
bool bFocus = false;
	if(	GetFocus()->GetSafeHwnd() == m_hWnd
		&& (!CExtPopupMenuWnd::IsMenuTracking())
		)
		bFocus = true;
	g_PaintManager->
		PaintControlFrame(
			*pDC,
			rectClient,
			true,
			m_bMouseOver,
			IsWindowEnabled() ? true : false,
			bFocus
			);

	if( bNeedReleaseDC )
		ReleaseDC(pDC);
}

void CExtEdit::SetIndent(int margin)
{
	SetMargins(margin, 1);
}

void CExtEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	m_bMouseOver = true;
	_PostRedraw();
}

void CExtEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	m_bMouseOver = false;
	_PostRedraw();
}

BOOL CExtEdit::OnEraseBkgnd(CDC* pDC) 
{
///	return CEdit::OnEraseBkgnd(pDC);
	return TRUE;
}

void CExtEdit::OnNcPaint() 
{
CWindowDC dc(this);
CRect rcClient, rcBar;
	GetClientRect(rcClient);
	ClientToScreen(rcClient);
	GetWindowRect(rcBar);
	rcClient.OffsetRect(-rcBar.TopLeft());
	rcBar.OffsetRect(-rcBar.TopLeft());
	// client area is not our bussiness :)
	dc.ExcludeClipRect(rcClient);
CExtMemoryDC dcmm( &dc, &rcBar );
	rcBar.DeflateRect(3,3);
	_DrawEditImpl( rcBar, &dcmm );	
}

void CExtEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CEdit::OnNcCalcSize(bCalcValidRects, lpncsp);
CRect rc( lpncsp->rgrc[0] );
	rc.DeflateRect(4,4);
	lpncsp->rgrc[0] = rc;
}
