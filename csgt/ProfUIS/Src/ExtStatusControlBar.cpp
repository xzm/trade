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

// ExtStatusControlBar.cpp : implementation file
//

#include "stdafx.h"

#if (!defined __EXT_STATUSCONTROLBAR_H)
	#include <ExtStatusControlBar.h>
#endif

#if (!defined __EXTDOCKBAR_H)
	#include "ExtDockBar.h"
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if _MSC_VER > 1200
#include <../Src/mfc/AfxImpl.h>
#else
#include <../Src/AfxImpl.h>
#endif // _MSC_VER > 1200


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtStatusControlBar

IMPLEMENT_DYNAMIC( CExtStatusControlBar, CStatusBar )

CExtStatusControlBar::CExtStatusControlBar()
{
	m_clrStatusBk = (COLORREF)(-1);
}

CExtStatusControlBar::~CExtStatusControlBar()
{
}

BEGIN_MESSAGE_MAP(CExtStatusControlBar, CStatusBar)
	//{{AFX_MSG_MAP(CExtStatusControlBar)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETTINGCHANGE()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CExtStatusControlBar message handlers

void CExtStatusControlBar::OnPaint() 
{
	_SyncStatusBarColors();
    Default();
CClientDC dc( this );
    dc.SelectStockObject( NULL_BRUSH );
	for( int nItem = 1; nItem < m_nCount; nItem++ )
	{
		CRect rc;
		GetItemRect( nItem, rc );
		g_PaintManager->PaintControlBarBorders(
			CExtPaintManager::__CB_INNER_STATUSBAR_ITEM,
			0,
			dc, 
			rc
			);
	} // for( int nItem = 1; nItem < m_nCount; nItem++ )
}

void CExtStatusControlBar::OnNcPaint() 
{
	_SyncStatusBarColors();
CWindowDC dc( this );
CRect rc;
	GetWindowRect( &rc );
	ScreenToClient( &rc );
	g_PaintManager->PaintControlBarBorders(
		CExtPaintManager::__CB_OUTER_STATUSBAR,
		m_dwStyle,
		dc, 
		rc
		);
}

void CExtStatusControlBar::DrawBorders( CDC * pDC, CRect& rc )
{
	ASSERT_VALID( this );
	ASSERT_VALID( pDC );
	g_PaintManager->PaintControlBarBorders(
		CExtPaintManager::__CB_OUTER_STATUSBAR,
		m_dwStyle,
		*pDC, 
		rc
		);
}

void CExtStatusControlBar::_SyncStatusBarColors()
{
COLORREF clrStatusBkNeeded =
		g_PaintManager->GetColor(
			CExtPaintManager::CLR_3DFACE_OUT
			);
	if( m_clrStatusBk != clrStatusBkNeeded )
	{
		m_clrStatusBk = clrStatusBkNeeded;
		GetStatusBarCtrl().SetBkColor( m_clrStatusBk );
	}
}

void CExtStatusControlBar::OnSysColorChange() 
{
	CStatusBar::OnSysColorChange();
	g_PaintManager->SyncSysColors();
	g_PaintManager->InitTranslatedColors();
	_SyncStatusBarColors();
}

void CExtStatusControlBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CStatusBar::OnSettingChange(uFlags, lpszSection);
	g_PaintManager->SyncSysColors();
	g_PaintManager->InitTranslatedColors();
	_SyncStatusBarColors();
}

int CExtStatusControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if( CStatusBar::OnCreate(lpCreateStruct) == -1 )
		return -1;
	
	_SyncStatusBarColors();
	
	return 0;
}

void CExtStatusControlBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	_ContextMenuTrack();
}

CFrameWnd * CExtStatusControlBar::_GetDockingFrameImpl()
{
	ASSERT_VALID( this );

CFrameWnd * pFrame = GetParentFrame();
	ASSERT_VALID( pFrame );
	if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) )
	{
		pFrame = pFrame->GetParentFrame();
		//ASSERT_VALID( pFrame );
		ASSERT( pFrame != NULL );
		ASSERT( !pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) );
	}

	return pFrame;
}

void CExtStatusControlBar::_ContextMenuTrack()
{
CFrameWnd * pFrame = _GetDockingFrameImpl();
CExtDockBar * pDockBar = NULL;
	if( m_pDockBar != NULL )
	{
		ASSERT_VALID( m_pDockBar );
		pDockBar =
			DYNAMIC_DOWNCAST(
				CExtDockBar,
				m_pDockBar
				);
	}
	if( pDockBar == NULL )
	{
		// find any dock bar in frame
		POSITION pos =
			pFrame->m_listControlBars.GetHeadPosition();
		while( pos != NULL )
		{
			CDockBar * pTempDockBar = (CDockBar*)
				pFrame->m_listControlBars.GetNext( pos );
			if( !pTempDockBar->IsDockBar() )
				continue;
			pDockBar =
				DYNAMIC_DOWNCAST(
					CExtDockBar,
					pTempDockBar
					);
			if( pDockBar != NULL )
				break;
		}
	}
	if( pDockBar == NULL )
		return;
CPoint point;
	VERIFY( ::GetCursorPos(&point) );
CExtPopupMenuWnd * pPopup =
		new CExtPopupMenuWnd;
	VERIFY( pPopup->CreatePopupMenu(GetOwner()->GetSafeHwnd()) );
	pDockBar->_ContextMenuBuild(
		pFrame,
		pPopup
		);
	pPopup->TrackPopupMenu(
		0,
		point.x,point.y,
		GetOwner()->GetSafeHwnd() // pFrame->GetSafeHwnd()
		);
}


