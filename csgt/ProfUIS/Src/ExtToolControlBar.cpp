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

#if _MSC_VER > 1200
#include <../Src/mfc/AfxImpl.h>
#else
#include <../Src/AfxImpl.h>
#endif // _MSC_VER > 1200

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
#endif

#if (!defined __EXT_MENUCONTROLBAR_H)
	#include <ExtMenuControlBar.h>
#endif

#if (!defined __EXTDOCKBAR_H)
	#include "ExtDockBar.h"
#endif

#if( !defined __EXTMINIDOCKFRAMEWND_H)
	#include "ExtMiniDockFrameWnd.h"
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

#if (!defined __ExtCmdManager_H)
	#include <ExtCmdManager.h>
#endif

#if (!defined __EXT_LOCALIZATION_H)
	#include <../Src/ExtLocalization.h>
#endif

#include <../profuisdll/resource.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CX_BORDER   1
#define CY_BORDER   1

#define ID_HOVER_TIMER 200
#define ID_MENUDROPDOWN_TIMER 201

/////////////////////////////////////////////////////////////////////////////
// CExtBarButton

IMPLEMENT_DYNAMIC(CExtBarButton, CObject)

bool CExtBarButton::IsAbleToTrackMenu()
{
	if( GetMenu() != NULL
		||
		IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton))
		)
		return true;
	return false;
}

CString CExtBarButton::GetText() const
{
CString sText( _T("") );
	if( IsSeparator() )
		return sText;
CExtCmdManager::cmd_t * p_cmd =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
			GetCmdID()
			);
	ASSERT( p_cmd != NULL );
	if( !p_cmd->m_sToolbarText.IsEmpty() )
		sText = p_cmd->m_sToolbarText;
int nTabChrPos = sText.Find( _T('\t') );
	if( nTabChrPos < 0 )
		return sText;
	return sText.Left( nTabChrPos );
}

BOOL CExtBarButton::PutToPopupMenu(
	CExtPopupMenuWnd * pPopup
	)
{
	ASSERT( pPopup != NULL );
	ASSERT( pPopup->GetSafeHwnd() == NULL );
HMENU hMenu = GetMenu();
	if( hMenu != NULL )
	{
		ASSERT( ::IsMenu(hMenu) );
		CExtCmdManager::cmd_t * p_cmd =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
				GetCmdID()
				);
		ASSERT( p_cmd != NULL );
		CString sText = p_cmd->m_sMenuText;
		if( sText.IsEmpty() )
			sText = p_cmd->m_sToolbarText;
		HICON hIcon =
			g_CmdManager->CmdGetHICON(
				g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
				GetCmdID()
				);
		if( hIcon != NULL )
		{
			CExtCmdManager::icon_t _icon(
				hIcon,
				true
				);
			hIcon = _icon.Detach();
		}
		VERIFY(
			pPopup->ItemInsert(
				CExtPopupMenuWnd::TYPE_POPUP,
				-1,
				(LPCSTR)sText,
				hIcon
				)
			);
		CExtPopupMenuWnd * pChildPopup =
			pPopup->ItemGetPopup(
				pPopup->ItemGetCount() - 1
				);
		ASSERT( pChildPopup != NULL );
		CMenu _menu;
		_menu.Attach( hMenu );
		pChildPopup->UpdateFromMenu(
			m_pBar->GetSafeHwnd(),
			&_menu,
			IsContainerOfPopupLikeMenu() ? true : false,
			false
			);
		_menu.Detach();
	} // if( hMenu != NULL )
	else
	{
		if( !pPopup->ItemInsert(GetCmdID()) )
		{
			ASSERT( FALSE );
			return FALSE;
		}
	} // else from if( hMenu != NULL )
	return TRUE;
}

CSize CExtBarButton::CalculateLayout(
	CDC & dc,
	CSize sizePreCalc,
	BOOL bHorz
	)
{
	ASSERT_VALID( m_pBar );
	ASSERT_VALID( (&dc) );

	if( (m_pCtrl != NULL) && (!m_bVertDocked) )
	{
		ASSERT_VALID( m_pCtrl );
		CRect rcCtrl;
		m_pCtrl->GetWindowRect( &rcCtrl );
		//return rcCtrl.Size();
		m_ActiveSize.cx = rcCtrl.Width();
		m_ActiveSize.cy = sizePreCalc.cy;
		return m_ActiveSize;
	} // if( (m_pCtrl != NULL) && (!m_bVertDocked) )
	m_ActiveSize = sizePreCalc;

	if( IsSeparator() )
	{
		if( bHorz )
			m_ActiveSize.cx = __TB_SEPARATOR_WIDTH__;
		else
			m_ActiveSize.cy = __TB_SEPARATOR_WIDTH__;
	} // if( IsSeparator() )
	else
	{
		CExtCmdManager::cmd_t * p_cmd =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
				GetCmdID()
				);
		ASSERT( p_cmd != NULL );
		CExtCmdManager::icon_t * p_icon =
			g_CmdManager->CmdGetIconPtr(
				g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
				GetCmdID()
				);
		CSize _size(0,0);
		if( p_icon != NULL )
			_size = *p_icon;
		CString sMeasureText = GetText();
		if( !sMeasureText.IsEmpty() )
		{
			static TCHAR stat_strDummyAmpSeq[] = _T("\001\001");
			sMeasureText.Replace( _T("&&"), stat_strDummyAmpSeq );
			sMeasureText.Remove( _T('&') );
			sMeasureText.Replace( stat_strDummyAmpSeq, _T("&") );
			int iPixTextWidth =
				dc.GetTextExtent(
					(LPCTSTR)sMeasureText
					).cx
					+ __TB_TEXT_MARGINE__*2;
			if( bHorz )
				_size.cx += iPixTextWidth;
			else
				_size.cy += iPixTextWidth;
		} // if( !sMeasureText.IsEmpty() )
		if( m_ActiveSize.cx < _size.cx )
			m_ActiveSize.cx = _size.cx;
		if( m_ActiveSize.cy < _size.cy )
			m_ActiveSize.cy = _size.cy;
	} // else from if( IsSeparator() )
	if( GetMenu() != NULL
		&& (!m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)))
		)
	{
		if( bHorz )
		{
			m_ActiveSize.cx +=
				__DROPDOWN_ARROW_GAP*2
				+ CExtPaintManager::g_glyph_btn_expand_bottom.Size().cx
				;
		} // if( bHorz )
		else
		{
			m_ActiveSize.cy +=
				__DROPDOWN_ARROW_GAP*2
				+ CExtPaintManager::g_glyph_btn_expand_right.Size().cy
				;
		} // else from if( bHorz )
	}
	m_ActiveSize.cx += __TB_BUTTON_MARGINE__*2;
	m_ActiveSize.cy += __TB_BUTTON_MARGINE__*2;
	return m_ActiveSize;
}

HICON CExtBarButton::GetHICON()
{
HICON hIcon =
		g_CmdManager->CmdGetHICON(
			g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
			GetCmdID()
			);
	return hIcon;
}

void CExtBarButton::Paint(
	CDC & dc,
	bool bHorz
	)
{
	ASSERT_VALID( (&dc) );

	if( (m_pCtrl != NULL) && (!m_bVertDocked) )
		return;

	ASSERT( !IsSeparator() );
CRect rcArea( m_ActiveRect );
	rcArea.DeflateRect(
		__TB_BUTTON_MARGINE__,
		__TB_BUTTON_MARGINE__
		);
	if( rcArea.right <= rcArea.left
		||
		rcArea.bottom <= rcArea.top
		)
		return;
	if( (!IsVisible())
		||
		(!dc.RectVisible(&m_ActiveRect))
		)
		return;
	ASSERT(
		m_ActiveSize.cx >= __TB_BUTTON_MARGINE__*2
		&&
		m_ActiveSize.cy >= __TB_BUTTON_MARGINE__*2
		);
bool bPushed =
		IsPressed() ? true : false;
bool bEnabled = IsDisabled() ? false : true;
bool bHover =
		(IsHover() && !CExtPopupMenuWnd::IsMenuTracking()) ?
			true : false;
bool bIndeterminate = IsIndeterminate() ? true : false;
CString sText = GetText();

	g_PaintManager->PaintPushButton(
		dc,
		bHorz,
		rcArea,
		(LPCTSTR)sText,
		GetHICON(),
		true,
		bHover,
		bPushed,
		bIndeterminate,
		bEnabled,
		true,false,false,
		CExtPaintManager::__ALIGN_HORIZ_CENTER
			| CExtPaintManager::__ALIGN_VERT,
		NULL,
		(GetMenu() != NULL
			&& (!m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)))
			) ? true : false,
		0,
		( bEnabled && (!bHover) && (!bPushed) )
		);
}

void CExtBarButton::SetMenu(
	HMENU hMenu,
	BOOL bPopupMenu,
	BOOL bAutoDestroyMenu
	)
{
	_DestroyMenu();
	m_hMenu = hMenu;
	if( m_hMenu != NULL )
	{
		ASSERT( ::IsMenu(hMenu) );
		VERIFY(
			g_CmdManager->UpdateFromMenu(
				g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
				hMenu
				)
			);
		m_bPopupMenu = bPopupMenu;
		ModifyStyle(0,TBBS_DISABLED);
		m_bAutoDestroyMenu = bAutoDestroyMenu;
	}
}

UINT CExtBarButton::_GetTrackPopupFlags()
{
	ASSERT_VALID( m_pBar );
	switch( m_pBar->GetSafeDockBarDlgCtrlID() )
	{
	case AFX_IDW_DOCKBAR_TOP:
		return TPMX_TOPALIGN;
	case AFX_IDW_DOCKBAR_BOTTOM:
		return TPMX_BOTTOMALIGN;
	case AFX_IDW_DOCKBAR_LEFT:
		return TPMX_LEFTALIGN;
	case AFX_IDW_DOCKBAR_RIGHT:
		return TPMX_RIGHTALIGN;
	default: // floating
		return TPMX_TOPALIGN;
	} // switch( m_pBar->GetSafeDockBarDlgCtrlID() )

}

CWnd * CExtBarButton::GetCmdTargetWnd()
{
	ASSERT_VALID( m_pBar );
CWnd * pWnd = m_pBar->GetOwner(); // _GetDockingFrameImpl();
	ASSERT_VALID( pWnd );
	return pWnd;
}

void CExtBarButton::OnTrackPopup(
	CPoint point
	)
{
	ASSERT_VALID( m_pBar );

HMENU hMenu = GetMenu();
	if( hMenu == NULL )
	{
//		m_pBar->_SwitchMenuTrackingIndex();
		return;
	}
	ASSERT( ::IsMenu(hMenu) );
	
bool bPrevTBMT = CExtToolControlBar::g_bMenuTracking;

	if( CExtToolControlBar::g_bMenuTracking
		&& CExtPopupMenuWnd::IsMenuTracking()
		&& m_pBar->_GetIndexOf(this) ==
			m_pBar->m_nBtnIdxMenuTracking
		)
		return;

	CExtToolControlBar::_CloseTrackingMenus();

	if( m_pBar->IsFloating() )
	{
		m_pBar->ActivateTopParent();
		CFrameWnd * pFrame =
			m_pBar->GetDockingFrame();
		ASSERT_VALID( pFrame );
		pFrame->BringWindowToTop();
	}

CExtPopupMenuWnd * pPopup =
		new CExtPopupMenuWnd;
CMenu _menu;
	_menu.Attach( hMenu );
	VERIFY(
		pPopup->UpdateFromMenu(
			m_pBar->GetSafeHwnd(),
			&_menu,
			m_bPopupMenu ? true : false
			)
		);
	_menu.Detach();
	if( IsAppendMdiWindowsMenu() )
		pPopup->UpdateMdiWindowsMenu( m_pBar );

	CExtToolControlBar::g_bMenuTracking = bPrevTBMT;

CRect rcBtn = Rect();
	m_pBar->ClientToScreen( &rcBtn );
	m_pBar->ClientToScreen( &point );
CFrameWnd * pFrame = m_pBar->_GetDockingFrameImpl();
	pPopup->TrackPopupMenu(
		_GetTrackPopupFlags(),
		point.x,point.y,
		GetCmdTargetWnd()->GetSafeHwnd(), // pFrame->GetSafeHwnd()
		&rcBtn,
		(m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) ?
			true : false),
		false,
		CExtToolControlBar::g_bMenuTrackingExpanded,
		CExtPopupMenuWnd::__COMBINE_DEFAULT,
		m_pBar,
		CExtToolControlBar::_CbPaintCombinedContent
		);
	m_pBar->_SwitchMenuTrackingIndex(
		m_pBar->_GetIndexOf( this )
		);
}

void CExtBarButton::OnHover(
	CPoint point,
	bool bOn
	)
{
	ASSERT_VALID( m_pBar );
	if( bOn )
	{

///		if( CExtToolControlBar::g_bMenuTracking
///			&& CExtPopupMenuWnd::IsMenuTracking()
///			&& m_pBar->m_nBtnIdxHover != m_pBar->m_nBtnIdxMenuTracking
///			)
///			CExtPopupMenuWnd::CancelMenuTracking();
		if( CExtToolControlBar::g_bMenuTracking )
			OnTrackPopup( point );
		else
			m_pBar->GetOwner()->SendMessage(
				WM_SETMESSAGESTRING,
				CommandIDToStringID(
					CExtCmdManager::IsCommand(GetCmdID()) ?
						GetCmdID() : AFX_IDS_IDLEMESSAGE
					)
				);
		m_pBar->SetTimer(ID_HOVER_TIMER,20,NULL);
	} // if( bOn )
	else
	{
		m_pBar->KillTimer(ID_HOVER_TIMER);
	} // else from if( bOn )
}

void CExtBarButton::OnClick(
	CPoint point,
	bool bDown
	)
{
	ASSERT_VALID( m_pBar );
	if( bDown )
	{
		m_pBar->GetOwner()->SendMessage(
			WM_SETMESSAGESTRING,
			CommandIDToStringID(GetCmdID())
			);
		return;
	} // if( bDown )

	CExtToolControlBar::g_bMenuTrackingExpanded = false;

	m_pBar->GetOwner()->SendMessage(
		WM_SETMESSAGESTRING,
		AFX_IDS_IDLEMESSAGE
		);

	if( GetMenu() != NULL )
	{
		OnTrackPopup( point );
		return;
	}

	if( !CExtCmdManager::IsCommand( GetCmdID() ) )
		return;

CExtCmdManager::cmd_t * p_cmd =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( m_pBar->GetSafeHwnd() ),
			GetCmdID()
			);
	ASSERT( p_cmd != NULL );
	if( p_cmd == NULL )
		return;
	VERIFY(
		p_cmd->Deliver(
			m_pBar,
			true
			)
		);
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarContentExpandButton

IMPLEMENT_DYNAMIC(CExtBarContentExpandButton, CExtBarButton)

CSize CExtBarContentExpandButton::CalculateLayout(
	CDC & dc,
	CSize sizePreCalc,
	BOOL bHorz
	)
{
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );
	ASSERT( GetCmdID() == __ID_TOOLBAR_RIGHT_BUTTON__ );
CSize _size(
			bHorz ? __RIGHT_BUTTON_HORZ_DX__ : sizePreCalc.cx,
			bHorz ? sizePreCalc.cy : __RIGHT_BUTTON_VERT_DY__
			);
	m_ActiveSize = _size;
	m_ActiveSize.cx += __TB_BUTTON_MARGINE__*2;
	m_ActiveSize.cy += __TB_BUTTON_MARGINE__*2;
	return m_ActiveSize;
}

void CExtBarContentExpandButton::Paint(
	CDC & dc,
	bool bHorz
	)
{
	ASSERT( m_pCtrl == NULL );
	ASSERT_VALID( (&dc) );
	ASSERT( !IsSeparator() );
	ASSERT( GetCmdID() == __ID_TOOLBAR_RIGHT_BUTTON__ );
	ASSERT( !IsDisabled() );

CRect rcArea( m_ActiveRect );
	rcArea.DeflateRect(
		__TB_BUTTON_MARGINE__,
		__TB_BUTTON_MARGINE__
		);

	if( rcArea.right <= rcArea.left
		||
		rcArea.bottom <= rcArea.top
		)
		return;
	if( // (!IsVisible()) ||
		(!dc.RectVisible(&m_ActiveRect))
		)
		return;
	ASSERT(
		m_ActiveSize.cx >= __TB_BUTTON_MARGINE__*2
		&&
		m_ActiveSize.cy >= __TB_BUTTON_MARGINE__*2
		);
bool bPushed =
		IsPressed() ? true : false;
bool bHover =
		(IsHover() && !CExtPopupMenuWnd::IsMenuTracking()) ?
			true : false;
bool bBarIsCompletelyVisible =
		(GetButtons().GetSize() == 0) ? true : false;
	g_PaintManager->PaintToolbarExpandButton(
		dc,
		rcArea,
		bHorz,
		bBarIsCompletelyVisible,
		true,
		bPushed,
		bHover,
		( (!bHover) && (!bPushed) )
		);
}

BOOL CExtBarContentExpandButton::PutToPopupMenu(
	CExtPopupMenuWnd * pPopup
	)
{
	pPopup;
	ASSERT( FALSE );
	return FALSE;
}

void CExtBarContentExpandButton::OnTrackPopup(
	CPoint point
	)
{
	ASSERT_VALID( m_pBar );

	if( CExtToolControlBar::g_bMenuTracking
		&& CExtPopupMenuWnd::IsMenuTracking()
		&& m_pBar->_GetIndexOf(this) ==
			m_pBar->m_nBtnIdxMenuTracking
		)
		return;

	CExtToolControlBar::_CloseTrackingMenus();

	if( m_pBar->IsFloating() )
	{
		m_pBar->ActivateTopParent();
		CFrameWnd * pFrame =
			m_pBar->GetDockingFrame();
		ASSERT_VALID( pFrame );
		pFrame->BringWindowToTop();
	}
/*
CMenu * pSysMenu =
		GetCmdTargetWnd()->GetSystemMenu(FALSE);
	if( pSysMenu != NULL )
	{
		g_CmdManager->UpdateFromMenu(
			pSysMenu->GetSafeHmenu()
			);
	}
*/

CExtPopupMenuWnd * pPopup =
		new CExtPopupMenuWnd;
	VERIFY( pPopup->CreatePopupMenu( GetCmdTargetWnd()->GetSafeHwnd() ) );
	// append hidden buttons
int nCount = GetButtons().GetSize();
	if( nCount != 0 )
	{
		ASSERT( nCount > 0 );
		int nCountBefore = pPopup->ItemGetCount();
		for( int i=0; i< nCount; i++ )
		{
			CExtBarButton * pTBB = GetButtons() [i];
			ASSERT( pTBB != NULL );
			if( i==0 && pTBB->IsSeparator() )
				continue;
			ASSERT( !pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) );
			VERIFY( pTBB->PutToPopupMenu(pPopup) );
		} // for( int i=0; i< nCount; i++ )
		int nCountAfter = pPopup->ItemGetCount();
		ASSERT( nCountAfter >= nCountBefore );
		if( nCountAfter != nCountBefore )
		{
			// append separator
			VERIFY(
				pPopup->ItemInsert(
					CExtPopupMenuWnd::TYPE_SEPARATOR
					)
				);
		}
	} // if( nCount != 0 )
	// append show/hide popup
CExtLocalResourceHelper _LRH;
CString sShowHidePanels;
	if( !sShowHidePanels.LoadString(IDS_SHOW_HIDE_PANELS) )
	{
		ASSERT( FALSE );
		sShowHidePanels = _T("&Show/hide panels");
	}
	VERIFY(
		pPopup->ItemInsert(
			CExtPopupMenuWnd::TYPE_POPUP,
			-1,
			sShowHidePanels
			)
		);
	CExtPopupMenuWnd * pPopupBars =
		pPopup->ItemGetPopup(
			pPopup->ItemGetCount() - 1
			);
	ASSERT( pPopupBars != NULL );
	
CFrameWnd * pFrame = m_pBar->_GetDockingFrameImpl();
	CExtDockBar::_ContextMenuBuild(
		pFrame,
		pPopupBars
		);

CRect rcBtn = Rect();
	m_pBar->ClientToScreen( &rcBtn );
	m_pBar->ClientToScreen( &point );
	pPopup->TrackPopupMenu(
		_GetTrackPopupFlags(),
		point.x,point.y,
		GetCmdTargetWnd()->GetSafeHwnd(), // pFrame->GetSafeHwnd()
		&rcBtn,
		m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) ?
			true : false,
		false,
		CExtToolControlBar::g_bMenuTrackingExpanded,
		CExtPopupMenuWnd::__COMBINE_DEFAULT,
		m_pBar,
		CExtToolControlBar::_CbPaintCombinedContent
		);
	m_pBar->_SwitchMenuTrackingIndex(
		m_pBar->_GetIndexOf( this )
		);
}

void CExtBarContentExpandButton::OnHover(
	CPoint point,
	bool bOn
	)
{
	ASSERT_VALID( m_pBar );

	CExtBarButton::OnHover(
		point,
		bOn
		);
}

void CExtBarContentExpandButton::OnClick(
	CPoint point,
	bool bDown
	)
{
	ASSERT_VALID( m_pBar );
	if( bDown )
		return;

	CExtToolControlBar::g_bMenuTrackingExpanded = false;

	OnTrackPopup( point );
}

/////////////////////////////////////////////////////////////////////////////
// CExtToolControlBar

IMPLEMENT_DYNAMIC(CExtToolControlBar, CExtControlBar)

CExtBarButton * CExtToolControlBar::_GetButtonPtr(int nIndex) const
{
int nButtonsCount = m_buttons.GetSize();
	if( !(nIndex >= 0 && nIndex < nButtonsCount) )
	{
		ASSERT( FALSE );
		return NULL;
	}
CExtBarButton * pTBB =
		m_buttons[nIndex];
	ASSERT_VALID( pTBB );
	return pTBB;
}

void CExtToolControlBar::_RemoveAllButtonsImpl()
{
	for( INT iBtn=0; iBtn<m_buttons.GetSize(); iBtn++ )
	{
		CExtBarButton * pTBB = m_buttons[iBtn];
		ASSERT( pTBB != NULL );
		delete pTBB;
	}
	m_buttons.RemoveAll();
	m_pRightBtn = NULL;
	m_nBtnIdxCapture = -1;      // nothing captured
	m_nBtnIdxHover = -1;
	m_nBtnIdxMenuTracking = -1;
}

int CExtToolControlBar::_GetButtonsCountImpl() const
{
int nCountOfButtons = m_buttons.GetSize();
	return nCountOfButtons;
}

bool CExtToolControlBar::g_bMenuTracking = false;
bool CExtToolControlBar::g_bMenuTrackingExpanded = false;

CExtToolControlBar::CExtToolControlBar()
{
	m_bGripperStaticallyAtTop = false;

	m_pRightBtn = NULL;

	m_bFixedMode = TRUE;
	m_bReposSingleChildMode = FALSE;
	m_nBtnIdxCapture = -1;
	m_nBtnIdxHover = -1;
	m_nBtnIdxMenuTracking = -1;

	m_sizeTBB.cx = 23;
	m_sizeTBB.cy = 22;

	m_cyTopBorder = m_cyBottomBorder = 2; // aditional spaces

	m_nGripWidthAtLeft = 8;
	m_nGripHeightAtTop = 10;
}

CExtToolControlBar::~CExtToolControlBar()
{
	_RemoveAllButtonsImpl();

int nCountOfBars = g_AllBars.GetSize();
	for( int i=0; i<nCountOfBars; i++ )
	{
		CExtControlBar * pBar = g_AllBars[i];
		ASSERT( pBar != NULL );
		if( pBar == this )
		{
			g_AllBars.RemoveAt( i );
			break;
		}
	}
}

void CExtToolControlBar::SetSizes(
	SIZE sizeTBB,
	SIZE sizeImage
	)
{
	ASSERT_VALID(this);
	ASSERT(sizeTBB.cx > 0 && sizeTBB.cy > 0);

	Invalidate();
}

BOOL CExtToolControlBar::LoadToolBar(LPCTSTR lpszResourceName)
{
CToolBar _bar;
	if( !_bar.Create(
			this,
			WS_CHILD|CBRS_TOP,
			AFX_IDW_TOOLBAR + 1000
			)
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}
	if(	!_bar.LoadToolBar(
			lpszResourceName
			)
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}
	if( !g_CmdManager->UpdateFromToolBar(
			g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
			_bar
			)
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}
int nIDCount = _bar.GetToolBarCtrl().GetButtonCount();
	if( nIDCount <= 0 )
		return TRUE;
UINT * lpIDArray = new UINT[nIDCount];
	if( lpIDArray == NULL )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	for( int i=0; i<nIDCount; i++ )
	{
		UINT nID,nStyle;
		int iImage;
		_bar.GetButtonInfo(i,nID,nStyle,iImage);
		lpIDArray[i] = nID;
	};
	
BOOL bRetVal = 
		SetButtons(
			lpIDArray,
			nIDCount
			);
	delete [] lpIDArray;
	return bRetVal;
}

BOOL CExtToolControlBar::RemoveButton(
	int nPos,
	BOOL bDoRecalcLayout // = TRUE
	)
{
	if( nPos < 0 )
	{
		ASSERT( FALSE );
		return FALSE;
	}
int nCountOfButtons = _GetButtonsCountImpl();
	if( nPos >= nCountOfButtons )
	{
		ASSERT( FALSE );
		return FALSE;
	}
CExtBarButton * pTBB =
		_GetButtonPtr( nPos );
	ASSERT_VALID( pTBB );
	if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	delete pTBB;
	m_buttons.RemoveAt( nPos );
	m_nBtnIdxCapture = -1;      // nothing captured
	m_nBtnIdxHover = -1;
	m_nBtnIdxMenuTracking = -1;
	if( bDoRecalcLayout )
		_RecalcLayoutImpl();
	return TRUE;
}

BOOL CExtToolControlBar::InsertButton(
	int nPos, // = -1, // append
	UINT nCmdID, // = ID_SEPARATOR
	BOOL bDoRecalcLayout // = TRUE
	)
{
int nCountOfButtons = _GetButtonsCountImpl();
	if( nPos < 0 )
		nPos = nCountOfButtons;
	if( nPos > nCountOfButtons )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	if( nCountOfButtons > 0
		&& nPos == nCountOfButtons
		)
	{
		CExtBarButton * pTBB =
			_GetButtonPtr( nCountOfButtons - 1 );
		ASSERT_VALID( pTBB );
		if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) )
			nPos--;
	}
	try
	{
		CExtBarButton * pTBB =
			new CExtBarButton(
				this,
				nCmdID
				);
		ASSERT_VALID( pTBB );
		m_buttons.InsertAt(
			nPos,
			pTBB
			);
		if( bDoRecalcLayout )
			_RecalcLayoutImpl();
	} // try
//	catch( std::exception * pXept )
//	{
//		delete pXept;
//		ASSERT( FALSE );
//		return FALSE;
//	} // catch( std::exception * pXept )
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
		return FALSE;
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
		return FALSE;
	} // catch( ... )
	return TRUE;
}

BOOL CExtToolControlBar::SetButtons(
	const UINT * lpIDArray, // = NULL
	int nIDCount // = 0
	)
{
	ASSERT_VALID( this );
	ASSERT(
		lpIDArray == NULL
		|| nIDCount == 0
		|| AfxIsValidAddress(
			lpIDArray,
			sizeof(UINT) * nIDCount,
			FALSE
			)
		);
	_RemoveAllButtonsImpl();
	if( lpIDArray == NULL
		|| nIDCount == 0
		)
		return TRUE;
	try
	{
		for( int i = 0; i < nIDCount; i++ )
		{
			CExtBarButton * pTBB =
				new CExtBarButton(
					this,
					*lpIDArray++
					);
			ASSERT_VALID( pTBB );
			m_buttons.Add( pTBB );
		} // for( int i = 0; i < nIDCount; i++ )
		ASSERT( m_pRightBtn == NULL );
		m_pRightBtn =
			new CExtBarContentExpandButton(this);
		ASSERT_VALID( m_pRightBtn );
		m_buttons.Add( m_pRightBtn );
	} // try
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
		return FALSE;
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
		return FALSE;
	} // catch( ... )
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CExtToolControlBar attribute access

int CExtToolControlBar::CommandToIndex(UINT nIDFind) const
{
	ASSERT_VALID(this);

int nCountOfButtons = _GetButtonsCountImpl();
	for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		if( pTBB->GetCmdID() == nIDFind )
			return nBtnIdx;
	}
	return -1;
}

UINT CExtToolControlBar::GetButtonID(int nIndex) const
{
	ASSERT_VALID(this);
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
		return ID_SEPARATOR;
	ASSERT_VALID( pTBB );
	return pTBB->GetCmdID();
}

void CExtToolControlBar::GetButtonRect(int nIndex, LPRECT lpRect) const
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpRect, sizeof(RECT)));
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
	{
		::memset( lpRect, 0, sizeof(RECT) );
		return;
	}
	ASSERT_VALID( pTBB );
	*lpRect = *pTBB;
}

UINT CExtToolControlBar::GetButtonStyle(int nIndex) const
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
		return 0;
	ASSERT_VALID( pTBB );
	return pTBB->GetStyle();
}

void CExtToolControlBar::SetButtonStyle(int nIndex, UINT nStyle)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
		return;
	ASSERT_VALID( pTBB );
UINT nOldStyle = pTBB->GetStyle();
	if( nOldStyle != nStyle )
	{
		pTBB->SetStyle( nStyle );
		_InvalidateButton(nIndex);
	}
}

CWnd * CExtToolControlBar::GetButtonCtrl(
	int nIndex
	)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return NULL;
	ASSERT_VALID( pTBB );
CWnd * pCtrl = pTBB->CtrlGet();
	return pCtrl;
}

BOOL CExtToolControlBar::SetButtonCtrl(
	int nIndex,
	CWnd * pCtrl, // = NULL
	BOOL bCtrlAutoDestroyed // = TRUE
	)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return FALSE;
	ASSERT_VALID( pTBB );
	if( pCtrl != NULL )
	{
		ASSERT_VALID( pCtrl );
		DWORD dwCtrlStyle = pCtrl->GetStyle();
		ASSERT( (dwCtrlStyle&WS_CHILD) != 0 );
		if( (dwCtrlStyle&WS_CHILD) == 0 )
			return FALSE;
		ASSERT( IsChild( pCtrl ) );
		if( !IsChild( pCtrl ) )
			return FALSE;
		UINT nDlgCtrlID = (UINT)pCtrl->GetDlgCtrlID();
		ASSERT( nDlgCtrlID == pTBB->GetCmdID() );
		if( nDlgCtrlID != pTBB->GetCmdID() )
			return FALSE;
	}
	pTBB->CtrlSet(
		pCtrl,
		bCtrlAutoDestroyed
		);
	_RecalcLayoutImpl();
	return TRUE;
}

int CExtToolControlBar::GetButtonByAccessKey(TCHAR vkTCHAR)
{
int nCountOfButtons = _GetButtonsCountImpl();
	ASSERT( nCountOfButtons >= 0 );
	if( nCountOfButtons == 0 )
		return -1;
	for( int nBtnIdx=0; nBtnIdx<nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB =
			_GetButtonPtr(nBtnIdx);
		ASSERT_VALID( pTBB );
		if(	pTBB->IsSeparator()
			|| pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton))
			)
			continue;
		UINT nCmdID = pTBB->GetCmdID();
		ASSERT( CExtCmdManager::IsCommand(nCmdID) );
		CExtCmdManager::cmd_t * p_cmd =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
				nCmdID
				);
		ASSERT( p_cmd != NULL );
		if( p_cmd->m_sToolbarText.IsEmpty() )
			continue;
		int nTextLen = p_cmd->m_sToolbarText.GetLength();
		ASSERT( nTextLen > 0 );
		int nAmdIndex = p_cmd->m_sToolbarText.Find(_T('&'));
		if( nAmdIndex == nTextLen - 1 ) // !?!?!
			continue;
		TCHAR vk = p_cmd->m_sToolbarText[nAmdIndex+1];
		//ASSERT( _istalnum(vk) );
//		if( ! _istalnum(vk) )
//			continue;
		TCHAR szChar[2] = { vk, '\0'};
		::CharUpper( szChar );
		vk = szChar[0];

		if( vkTCHAR == vk )
		{
			if( !pTBB->IsVisible() )
			{
				if( m_pRightBtn != NULL )
					return _GetIndexOf( m_pRightBtn );
				continue;
			}
			return nBtnIdx;
		}
	} // for( int nBtnIdx=0; nBtnIdx<nCountOfButtons; nBtnIdx++ )
	return -1;
}

HMENU CExtToolControlBar::GetButtonMenu(
	int nIndex
	)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return NULL;
	ASSERT_VALID( pTBB );
	return ((HMENU)(*pTBB));
}

BOOL CExtToolControlBar::MarkButtonAsMdiWindowsMenu(
	int nIndex,
	BOOL bAppendMdiWindowsMenu // = TRUE
	)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return FALSE;
	ASSERT_VALID( pTBB );
	pTBB->SetAppendMdiWindowsMenu(
		bAppendMdiWindowsMenu
		);
	return TRUE;
}

BOOL CExtToolControlBar::SetButtonMenu(
	int nIndex,
	HMENU hMenu, // = NULL
	BOOL bPopupMenu, // = TRUE
	BOOL bAutoDestroyMenu, // = TRUE
	BOOL bDoRecalcLayout // = TRUE
	)
{
CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return FALSE;
	ASSERT_VALID( pTBB );
	pTBB->SetMenu(
		hMenu,
		bPopupMenu,
		bAutoDestroyMenu
		);
	if( bDoRecalcLayout )
		_RecalcLayoutImpl();
	return TRUE;
}

int CExtToolControlBar::GetVisibleButton(
	int nBtnIdx,
	BOOL bNext
	)
{
int nCountOfButtons = _GetButtonsCountImpl();
	ASSERT( nCountOfButtons >= 0 );
	if( nCountOfButtons == 0 )
		return -1;
	if( nCountOfButtons == 1 )
		return 0;
	if( bNext )
	{
		int iNewButton = nBtnIdx + 1;
		if( iNewButton >= nCountOfButtons )
			iNewButton = 0;
		while( iNewButton != nBtnIdx )
		{
			CExtBarButton * pTBB =
				_GetButtonPtr(iNewButton);
			ASSERT_VALID( pTBB );
			if( pTBB->IsVisible()
				&& (!pTBB->IsSeparator())
				&& (!pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiRightButton)))
				)
				return iNewButton;
			iNewButton++;
			if( iNewButton >= nCountOfButtons )
				iNewButton = 0;
		}
		return iNewButton;
	} // if( bNext )
	else
	{
		int iNewButton =
			nBtnIdx - 1;
		if( iNewButton < 0 )
			iNewButton = nCountOfButtons - 1;
		while( iNewButton != nBtnIdx )
		{
			CExtBarButton * pTBB =
				_GetButtonPtr(iNewButton);
			ASSERT_VALID( pTBB );
			if( pTBB->IsVisible()
				&& (!pTBB->IsSeparator())
				&& (!pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiRightButton)))
				)
				return iNewButton;
			iNewButton--;
			if( iNewButton < 0 )
				iNewButton = nCountOfButtons - 1;
		}
		return iNewButton;
	} // else from if( bNext )
}

BOOL CExtToolControlBar::TrackButtonMenu(
	int nIndex
	)
{
//	if( !IsVisible() )
//		return FALSE;
	if( !SafeDisplayBar() )
		return false;

CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT( pTBB != NULL );
	if( pTBB == NULL )
		return FALSE;
	ASSERT_VALID( pTBB );
	if( !pTBB->IsAbleToTrackMenu() )
		return FALSE;
CPoint point;
	VERIFY( ::GetCursorPos(&point) );
	ScreenToClient( &point );
	pTBB->OnTrackPopup(point);
	return TRUE;
}

CSize CExtToolControlBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	if(	(nLength == -1)
		&& !(dwMode & (LM_MRUWIDTH|LM_COMMIT))
		&&  (dwMode & (LM_HORZDOCK|LM_VERTDOCK))
		)
		return
			CalcFixedLayout(
				dwMode & LM_STRETCH,
				dwMode & LM_HORZDOCK
				);
	ASSERT(
		(dwMode&(LM_HORZ|LM_HORZDOCK))
		||
		(!(dwMode&LM_HORZDOCK))
		);
	return _CalcLayout( dwMode, nLength );
}

CSize CExtToolControlBar::CalcFixedLayout(
	BOOL bStretch,
	BOOL bHorz
	)
{
DWORD dwMode = bStretch ? LM_STRETCH : 0;
	dwMode |= bHorz ? LM_HORZ : 0;
	ASSERT(
		(dwMode&(LM_HORZ|LM_HORZDOCK))
		||
		(!(dwMode&LM_HORZDOCK))
		);
	return _CalcLayout( dwMode );
}

CSize CExtToolControlBar::_CalcLayout(
	DWORD dwMode,
	int nLength
	)
{
	ASSERT_VALID(this);
	if( _GetButtonsCountImpl() > 0 )
	{
		m_pRightBtn->ClearContent( false );
		BOOL bDynamic = m_dwStyle & CBRS_SIZE_DYNAMIC;
		INT nLengthToSizeTB =
			(dwMode & LM_HORZ) ? 32767 : 0;
		BOOL bVertSizeTB = FALSE;
		if( bDynamic )
		{
			if( dwMode & LM_MRUWIDTH )
				nLengthToSizeTB = m_nMRUWidth;
			else if( dwMode & LM_HORZDOCK )
				nLengthToSizeTB = 32767;
			else if( dwMode & LM_VERTDOCK )
				nLengthToSizeTB = 0;
			else if( nLength != -1 )
			{
				CRect rcInside( 0, 0, 0, 0 );
				_CalcInsideRect(
					rcInside,
					(dwMode & LM_HORZ)
					);
				bVertSizeTB = (dwMode & LM_LENGTHY);
				nLengthToSizeTB =
					nLength +
					( bVertSizeTB ?
						rcInside.Height() : rcInside.Width()
					);
			}
			else if( m_dwStyle & CBRS_FLOATING )
				nLengthToSizeTB = m_nMRUWidth;
		} // if( bDynamic )
		_SizeToolBar( nLengthToSizeTB, bVertSizeTB );
		CSize sizeCalcLayout =
			_CalcSize(
			(dwMode & LM_HORZ) == 0
			);
		if( m_pRightBtn != NULL
			&& m_pRightBtn->GetButtons().GetSize() == 0
			)
		{
			m_pRightBtn->ClearContent();
			sizeCalcLayout = _CalcSize( (dwMode & LM_HORZ) == 0 );
		}
		if( dwMode & LM_COMMIT )
			if( m_dwStyle & (CBRS_FLOATING|CBRS_SIZE_DYNAMIC) )
				if( dwMode & LM_HORZ )
					m_nMRUWidth = sizeCalcLayout.cx;
		CRect rcInside( 0, 0, 0, 0 );
		_CalcInsideRect(
			rcInside,
			(dwMode & LM_HORZ)
			);
		sizeCalcLayout.cy -= rcInside.Height();
		sizeCalcLayout.cx -= rcInside.Width();
		CSize sizeCalcFixedLayout =
			CControlBar::CalcFixedLayout(
				dwMode & LM_STRETCH,
				dwMode & LM_HORZ
				);
		sizeCalcLayout.cx =
			max( sizeCalcLayout.cx, sizeCalcFixedLayout.cx );
		sizeCalcLayout.cy =
			max( sizeCalcLayout.cy, sizeCalcFixedLayout.cy );
		if( IsFloating() && IsBarWithGripper() )
		{
			int nCyGripper = min( m_rcGrip.Width(), m_rcGrip.Height() );
			sizeCalcLayout.cy += nCyGripper;
		}
		_RecalcPositionsImpl();
		if( IsDockedHorizontally() )
		{
			m_sizeDockedH = sizeCalcLayout;
			m_nMinHW = sizeCalcLayout.cy;
		}
		if( IsDockedVertically() )
		{
			m_sizeDockedV = sizeCalcLayout;
			m_nMinVH = sizeCalcLayout.cx;
		}
		if( IsFloating() )
			m_sizeFloated = sizeCalcLayout;
		return sizeCalcLayout;
	} // if( _GetButtonsCountImpl() > 0 )
	else
	{
		return m_sizeTBB;
	} // else from if( _GetButtonsCountImpl() > 0 )
}

CSize CExtToolControlBar::_CalcSize( BOOL bVerticallyDocked )
{
int nCountOfButtons = _GetButtonsCountImpl();
	if( nCountOfButtons == 0 )
		return m_sizeTBB;
CClientDC dc( this );
CFont* pOldFont = (CFont*)
		dc.SelectObject(
			bVerticallyDocked ?
				&(g_PaintManager->m_FontNormalVert)
				:
				&(g_PaintManager->m_FontNormal)
			);
	ASSERT( pOldFont != NULL );
CSize sizeCalc( m_sizeTBB );
CPoint ptCurrent( __TB_BUTTON_MARGINE__, __TB_BUTTON_MARGINE__ );
	for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		if(		nBtnIdx == (nCountOfButtons-1)
			&&	m_pRightBtn != NULL
			&&	IsFloating()
			)
		{
			ASSERT_VALID( m_pRightBtn );
			ASSERT( m_pRightBtn == pTBB );
			break;
		}
		CSize sizeTBB(
			pTBB->CalculateLayout(
				dc,
				m_sizeTBB,
				!bVerticallyDocked
				)
			);
		if(		(!bVerticallyDocked)
			&&	( ptCurrent.x == __TB_BUTTON_MARGINE__
					|| pTBB->IsWrap() )
			&&	pTBB->IsSeparator()
			)
			sizeTBB = CSize( 0, 0 );
		sizeCalc.cx =
			max(
				ptCurrent.x + sizeTBB.cx,
				sizeCalc.cx
				);
		sizeCalc.cy =
			max(
				ptCurrent.y + sizeTBB.cy,
				sizeCalc.cy
				);
		if( bVerticallyDocked )
		{
			ptCurrent.x = __TB_BUTTON_MARGINE__;
			ptCurrent.y += sizeTBB.cy;
		} // if( bVerticallyDocked )
		else
		{
			ptCurrent.x += sizeTBB.cx;
			if( pTBB->IsWrap() )
			{
				ptCurrent.x = __TB_BUTTON_MARGINE__;
				ptCurrent.y += m_sizeTBB.cy + __TB_LINE_OFFSET;
			}
		} // else from if( bVerticallyDocked )
	} // for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )

	dc.SelectObject( pOldFont );

	if( bVerticallyDocked && sizeCalc.cx > __TB_LINE_OFFSET/2 )
		sizeCalc.cx -= __TB_LINE_OFFSET/2;
	if( (!bVerticallyDocked) && sizeCalc.cy > __TB_LINE_OFFSET/2 )
		sizeCalc.cy -= __TB_LINE_OFFSET/2;

	return sizeCalc;
}

void CExtToolControlBar::_SizeToolBar(int nLength, BOOL bVert)
{
int nCountOfButtons = _GetButtonsCountImpl();
	if( nCountOfButtons == 0 )
		return;
	if( !IsFloating() )
	{
		_WrapToolBar(
			bVert ? 32767 : nLength,
			bVert ? nLength : 32767
			);
		return;
	}
	_WrapToolBar( 0, 0 );
CSize sizeMin = _CalcSize( FALSE );
	_WrapToolBar(
		bVert ? 32767 : nLength,
		bVert ? nLength : 32767
		);
CSize sizeMax = _CalcSize( FALSE );
	while( sizeMin.cx < sizeMax.cx )
	{
		_WrapToolBar( (sizeMin.cx + sizeMax.cx) / 2 );
		CSize sizeMid = _CalcSize( FALSE );
		if( sizeMin.cx == sizeMax.cx )
			break;
		if( (!bVert) && ( nLength >= sizeMax.cx ) // nLength is horizontal length
			||
			bVert && ( nLength < sizeMid.cy ) // nLength is vertical length
			)
		{
			if( sizeMin.cx == sizeMid.cx )
			{
				_WrapToolBar( sizeMax.cx );
				break;
			}
			sizeMin = sizeMid;
		}
		else if(
			(!bVert) && (nLength < sizeMax.cx) // nLength is horizontal length
			||
			bVert && ( nLength > sizeMid.cy ) // nLength is vertical length
			)
			sizeMax = sizeMid;
		else break;
	} // while( sizeMin.cx < sizeMax.cx )
}

int CExtToolControlBar::_WrapToolBar(int nWidth, int nHeight /*= 32767*/)
{
bool bVerticallyDocked =
		((m_dwStyle & CBRS_ORIENT_HORZ) == 0)
			? true : false;
bool bFloating = IsFloating() ? true : false;
CClientDC dc( this );
CFont* pOldFont = (CFont*)
		dc.SelectObject(
			bVerticallyDocked ?
				&(g_PaintManager->m_FontNormalVert)
				:
				&(g_PaintManager->m_FontNormal)
			);
	ASSERT( pOldFont != NULL );
CRect rcClient;
	GetClientRect( &rcClient );
CPoint ptCurrent( 0, rcClient.top );
    if(		!bFloating
		&&	!bVerticallyDocked
		&&	m_pRightBtn != NULL
		)
        nWidth -=
			m_pRightBtn->CalculateLayout(
				dc, m_sizeTBB, TRUE ).cx;
int nCountOfRows = 1;
int nCountOfButtons = _GetButtonsCountImpl();
	for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		pTBB->SetWrap( FALSE );
		if( nBtnIdx == (nCountOfButtons-1)
			&& m_pRightBtn != NULL
			)
		{
			ASSERT_VALID( m_pRightBtn );
			ASSERT( m_pRightBtn == pTBB );
			break;
		}
		CSize sizeTBB =
			pTBB->CalculateLayout(
				dc,
				m_sizeTBB,
				!bVerticallyDocked
				);
		if( ptCurrent.x == 0 && pTBB->IsSeparator() )
			sizeTBB = CSize(0, 0);
		if(		nBtnIdx > 0
			&&	(bFloating || ptCurrent.y + sizeTBB.cy < nHeight)
			&&	ptCurrent.x + sizeTBB.cx > nWidth
			&&	!pTBB->IsSeparator()
			)
		{
			CExtBarButton * pPrevButton =
				_GetButtonPtr( nBtnIdx - 1 );
			ASSERT_VALID( pPrevButton );
			pPrevButton->SetWrap();
			ptCurrent.x = 0;
			ptCurrent.y += sizeTBB.cy + __TB_LINE_OFFSET;
			nCountOfRows++;
		}
		ptCurrent.x += sizeTBB.cx;
	} // for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	dc.SelectObject( pOldFont );
	return nCountOfRows;
}

void CExtToolControlBar::GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle) const
{
	ASSERT_VALID(this);

CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
	{
		ASSERT( FALSE );
		nID = 0;
		nStyle = 0;
		return;
	}
	ASSERT_VALID( pTBB );
	nID = pTBB->GetCmdID();
	nStyle = pTBB->GetStyle();
}

void CExtToolControlBar::SetButtonInfo(int nIndex, UINT nID, UINT nStyle)
{
	ASSERT_VALID(this);

CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	if( pTBB == NULL )
	{
		ASSERT( FALSE );
		return;
	}
	ASSERT_VALID( pTBB );
	pTBB->SetCmdID( nID );
	pTBB->SetStyle( nStyle );
	_InvalidateButton(nIndex);
}

void CExtToolControlBar::DoPaint(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	CExtPaintManager::stat_ExcludeChildAreas(
		*pDC,
		*this
		);
CRect rcClient;
	GetClientRect( &rcClient );
CExtMemoryDC dc( pDC, &rcClient );
	ASSERT( dc.GetSafeHdc() != NULL );
	if( dc.GetSafeHdc() != NULL )
		pDC = &dc;
	pDC->FillSolidRect(
		&rcClient,
		g_PaintManager->GetColor(
			CExtPaintManager::CLR_3DFACE_OUT
			)
		);
	pDC->SetTextColor(
		g_PaintManager->GetColor(CExtPaintManager::CLR_TEXT_OUT)
		);
	pDC->SetBkMode( TRANSPARENT );
CFont * pOldFont =
		pDC->SelectObject(
			( m_dwStyle & CBRS_ORIENT_HORZ )
				? &g_PaintManager->m_FontNormal
				: &g_PaintManager->m_FontNormalVert
			);
	ASSERT( pOldFont != NULL );
int nCountOfButtons = _GetButtonsCountImpl();
	for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		if( !pTBB->IsVisible() )
			continue;
		CRect rcTBB = *pTBB;
		if( !pDC->RectVisible( &rcTBB ) )
			continue;
		if( !pTBB->IsSeparator() )
		{
			pTBB->Paint(
				*pDC,
				( m_dwStyle & CBRS_ORIENT_HORZ ) ? true : false
				);
			continue;
		}
		bool bHorzSeparator =
			( m_dwStyle & CBRS_ORIENT_HORZ ) ? true : false;
		CRect rectSeparator( rcTBB );
		if( pTBB->IsWrap() && ( m_dwStyle & CBRS_ORIENT_HORZ ) )
		{
			rectSeparator.left = rcClient.left;
			rectSeparator.right = rcClient.right;
			rectSeparator.top = rcTBB.bottom;
			rectSeparator.bottom =
				rectSeparator.top + __TB_LINE_OFFSET;
			bHorzSeparator = false;
		}
		rectSeparator.DeflateRect(
			bHorzSeparator ? 0 : 1,
			bHorzSeparator ? 1 : 0
			);
		g_PaintManager->PaintSeparator(
			*pDC, rectSeparator, bHorzSeparator
			);
	} // for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	pDC->SelectObject( pOldFont );
}

void CExtToolControlBar::_InvalidateButton(int nIndex)
{
	ASSERT_VALID(this);
CRect rect;
	GetButtonRect(nIndex, &rect);
	InvalidateRect(rect, FALSE);
}

int CExtToolControlBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID( this );
int nToolTipHit =
		CExtControlBar::OnToolHitTest(
			point,
			pTI
			);
	if( nToolTipHit != -1 )
		return nToolTipHit;
	nToolTipHit =
		((CExtToolControlBar*)this)->
			HitTest(point);
	if( nToolTipHit < 0 )
		return -1;
CExtBarButton * pTBB =
		_GetButtonPtr( nToolTipHit );
	ASSERT_VALID( pTBB );
	if( pTBB == NULL )
		return -1;
	if(	pTBB->IsSeparator()
		||
		pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton))
		||
		pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton))
		//||
		//pTBB->IsAbleToTrackMenu()
		)
		return -1;
int nCmdID = (int)pTBB->GetCmdID();
CExtCmdManager::cmd_t * p_cmd =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
			nCmdID
			);
	if( p_cmd == NULL
		|| p_cmd->m_sTipTool.IsEmpty()
		)
		return -1;

	if( pTI != NULL )
	{
		GetButtonRect(
			nToolTipHit,
			&(pTI->rect)
			);
		pTI->uId = (UINT)nCmdID;
		pTI->hwnd = m_hWnd;
		pTI->lpszText = (LPTSTR)
			::calloc(
				(p_cmd->m_sTipTool.GetLength() + 1),
				sizeof(TCHAR)
				);
		if( pTI->lpszText != NULL )
			_tcscpy(
				pTI->lpszText,
				(LPCTSTR)p_cmd->m_sTipTool
				);
		else
			pTI->lpszText = LPSTR_TEXTCALLBACK;
	} // if( pTI != NULL )

	nToolTipHit = nCmdID;
	return nToolTipHit;
}

int CExtToolControlBar::_HitTestImpl(
	CPoint point,
	UINT nButtonStyleInclude, // = 0, // button must have style
	UINT nButtonStyleExclude // = 0  // button must have not style
	) const
{
int nCountOfButtons = _GetButtonsCountImpl();
	for( int nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
	{
		CExtBarButton * pTBB =
			_GetButtonPtr(nBtnIdx);
		ASSERT_VALID( pTBB );
		if( !(pTBB->IsVisible()) )
			continue;
		if( (pTBB->GetStyle() & nButtonStyleInclude)
				!= nButtonStyleInclude
			)
			continue;
		if( (pTBB->GetStyle() & nButtonStyleExclude)
				!= 0
			)
			continue;
		CRect rect = *pTBB;
		if( rect.PtInRect(point) )
			return nBtnIdx;
	}
	return -1; // nowhere
}

int CExtToolControlBar::HitTest(
	CPoint point // in window relative coords
	) const
{
	return
		_HitTestImpl(
			point,
			0,
			TBBS_SEPARATOR
			);
}

/////////////////////////////////////////////////////////////////////////////
// CExtToolControlBar message handlers

BEGIN_MESSAGE_MAP(CExtToolControlBar, CExtControlBar)
	//{{AFX_MSG_MAP(CExtToolControlBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CANCELMODE()
	ON_WM_SETTINGCHANGE()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExtToolControlBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    if( m_bRowResizing || m_bRowRecalcing || m_bDragging )
	{
		CExtControlBar::OnLButtonDown(nFlags, point);
		return;
	}
	if( (m_nBtnIdxCapture = HitTest(point)) < 0 ) // nothing hit
	{
		CExtControlBar::OnLButtonDown(nFlags, point);
		return;
	}

CExtBarButton* pTBB =
		_GetButtonPtr(m_nBtnIdxCapture);
	ASSERT_VALID( pTBB );
	ASSERT( !(pTBB->IsSeparator()) );

	// update the button before checking for disabled status
	_UpdateButton(m_nBtnIdxCapture);
	if( pTBB->IsDisabled() )
	{
		m_nBtnIdxCapture = -1;
		return;     // don't press it
	}

	pTBB->ModifyStyle( TBBS_PRESSED );
	_InvalidateButton(m_nBtnIdxCapture);
	UpdateWindow(); // immediate feedback
	SetCapture();

	pTBB->OnClick(point,true);
}

void CExtToolControlBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if( CExtPopupMenuWnd::IsMenuTracking()
		&& !CExtToolControlBar::g_bMenuTracking
		)
		return;

	if( IsFloating() &&
		CExtMiniDockFrameWnd::g_bAutoMakeTopmostOnMouseEvent
		)
		GetParentFrame()->BringWindowToTop();

    if( m_bRowResizing || m_bRowRecalcing || m_bDragging )
	{
		CExtControlBar::OnMouseMove(nFlags, point);
		return;
	}
bool bHoverChanged =
		_UpdateHoverButton( point );
	if( m_nBtnIdxCapture >= 0 )
	{
		CExtBarButton* pTBB =
			_GetButtonPtr(m_nBtnIdxCapture);
		ASSERT_VALID( pTBB );
		ASSERT(!(pTBB->IsSeparator()));

		UINT nNewStyle = (pTBB->GetStyle() & ~TBBS_PRESSED);
		int nBtnIdxCapture = m_nBtnIdxCapture;
		if( GetCapture() != this )
		{
			m_nBtnIdxCapture = -1; // lost capture
		}
		else
		{
			// should be pressed if still hitting the captured button
			if( HitTest(point) == m_nBtnIdxCapture )
				nNewStyle |= TBBS_PRESSED;
		}
		SetButtonStyle(nBtnIdxCapture, nNewStyle);
		UpdateWindow(); // immediate feedback
		return;
	} // if( m_nBtnIdxCapture >= 0 )

	if( bHoverChanged )
		return;
	CExtControlBar::OnMouseMove(nFlags,point);
}

bool CExtToolControlBar::_UpdateHoverButton(
	CPoint point // = CPoint(-1,-1) // default is use ::GetCursorPos()
	)
{
	if( point.x < 0 || point.y < 0 )
	{
		VERIFY( ::GetCursorPos(&point) );
		ScreenToClient( &point );
	}
bool bHoverChanged = false;
int nBtnIdxHoverNew = HitTest(point);
int nBtnIdxHoverOld =  m_nBtnIdxHover;
	if( nBtnIdxHoverOld != nBtnIdxHoverNew )
	{
		bHoverChanged = true;
		m_nBtnIdxHover = nBtnIdxHoverNew;
		if( nBtnIdxHoverOld >= 0 )
		{
			CExtBarButton * pTBB =
				_GetButtonPtr(nBtnIdxHoverOld);
			ASSERT_VALID( pTBB );
			pTBB->SetHover( FALSE );
			_InvalidateButton( nBtnIdxHoverOld );
			pTBB->OnHover( point, false );
		}
		if( m_nBtnIdxHover >= 0 )
		{
			CExtBarButton * pTBB =
				_GetButtonPtr(m_nBtnIdxHover);
			ASSERT_VALID( pTBB );
			pTBB->SetHover();
			_InvalidateButton( m_nBtnIdxHover );
			pTBB->OnHover( point, true );
			/*
			if( GetCapture() != this )
				SetCapture();
			*/
		}
//		UpdateWindow(); // immediate feedback
	} // if( nBtnIdxHoverOld != nBtnIdxHoverNew )
	/*
	if( GetCapture() == this )
		ReleaseCapture();
	*/
	return bHoverChanged;
}

void CExtToolControlBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_nBtnIdxCapture < 0
		|| m_bRowResizing || m_bRowRecalcing || m_bDragging )
	{
		CExtControlBar::OnLButtonUp(nFlags, point);
		return;     // not captured
	}

	if( m_nBtnIdxCapture != HitTest(point) )
	{
		m_nBtnIdxCapture = -1;
		return;
	}

CExtBarButton* pTBB =
		_GetButtonPtr(m_nBtnIdxCapture);
	ASSERT_VALID( pTBB );
	ASSERT( !(pTBB->IsSeparator()) );
UINT nIDCmd = 0;

UINT nNewStyle =( pTBB->GetStyle() & ~TBBS_PRESSED );
	if( GetCapture() == this )
	{
		// we did not lose the capture
		ReleaseCapture();
		if( HitTest(point) == m_nBtnIdxCapture )
		{
			// give button a chance to update
			_UpdateButton(m_nBtnIdxCapture);

			// then check for disabled state
			if( !(pTBB->IsDisabled()) )
			{
				// pressed, will send command notification
				nIDCmd = pTBB->GetCmdID();

				if( pTBB->GetStyle() & TBBS_CHECKBOX )
				{
					// auto check: three state => down
					if( nNewStyle & TBBS_INDETERMINATE )
						nNewStyle &= ~TBBS_INDETERMINATE;

					nNewStyle ^= TBBS_CHECKED;
				}
			}
		}
	}

int nBtnIdxCapture = m_nBtnIdxCapture;
	//m_nBtnIdxCapture = -1;
int nBtnIdxHover = m_nBtnIdxHover;
	m_nBtnIdxCapture = m_nBtnIdxHover = -1;

	if( nBtnIdxHover >= 0 )
	{
		CExtBarButton * pTBB =
			_GetButtonPtr(nBtnIdxHover);
		ASSERT_VALID( pTBB );
		pTBB->SetHover( FALSE );
		KillTimer(ID_HOVER_TIMER);
		_InvalidateButton(nBtnIdxHover);
		pTBB->OnHover( point, false );
	}

	pTBB->OnClick(point,false);

	if( !(::IsWindow(GetSafeHwnd())) )
		return;
	if( !g_bMenuTracking )
	{
		SetButtonStyle(nBtnIdxCapture, nNewStyle);
		_UpdateButton(nBtnIdxCapture);
	} // if( bUpdateBtnState )

	UpdateWindow(); // immediate feedback
}

void CExtToolControlBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if( m_bRowResizing || m_bRowRecalcing || m_bDragging )
	{
		CExtControlBar::OnLButtonDblClk(nFlags, point);
		return;
	}
int nBtnIdx = _HitTestImpl(point);
	if( nBtnIdx >= 0 )
		return;
	CExtControlBar::OnLButtonDblClk(nFlags,point);
}

void CExtToolControlBar::OnCancelMode()
{
	CExtControlBar::OnCancelMode();

	ASSERT( !CExtPopupMenuWnd::IsMenuTracking() );

bool bUpdateState = false;
	if( m_nBtnIdxCapture >= 0 )
	{
		CExtBarButton* pTBB =
			_GetButtonPtr(m_nBtnIdxCapture);
		ASSERT_VALID( pTBB );
		ASSERT( !(pTBB->IsSeparator()) );
		UINT nNewStyle = (pTBB->GetStyle() & ~TBBS_PRESSED);
		SetButtonStyle(m_nBtnIdxCapture, nNewStyle);
		m_nBtnIdxCapture = -1;
	}
	if( m_nBtnIdxHover >= 0 )
	{
		CExtBarButton * pTBB =
			_GetButtonPtr(m_nBtnIdxHover);
		ASSERT_VALID( pTBB );
		pTBB->SetHover();
		m_nBtnIdxHover = -1;
		bUpdateState = true;
		CPoint point;
		VERIFY( ::GetCursorPos(&point) );
		ScreenToClient( &point );
		pTBB->OnHover( point, false );
	}

	if( bUpdateState )
	{
		if( GetCapture() == this )
			ReleaseCapture();
		Invalidate();
		UpdateWindow();
	}
}

void CExtToolControlBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CExtControlBar::OnSettingChange(uFlags,lpszSection);

	_RecalcLayoutImpl();

CFrameWnd* pFrame = GetParentFrame();
	ASSERT_VALID(pFrame);
	pFrame->RecalcLayout();
}

bool CExtToolControlBar::IsRightExpandButton( int nBtnIdx )
{
CExtBarButton * pTBB =
		_GetButtonPtr( nBtnIdx );
	if( pTBB == NULL )
		return false;
	ASSERT_VALID( pTBB );
	if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarContentExpandButton)) )
		return true;
	return false;
}

class CExtToolControlBar::CExtToolControlBarCmdUI : public CCmdUI
{
	void _SetCheckImpl(
		int nCheck,
		bool bUpdaeteInCmdManager
		);

public: // re-implementations only

	virtual void Enable(BOOL bOn);

	virtual void SetRadio(BOOL bOn);

	virtual void SetCheck(int nCheck)
	{
		_SetCheckImpl(nCheck,true);
	};

	virtual void SetText(LPCTSTR lpszText)
	{
		// ignore it
	};

}; // class CExtToolControlBar::CExtToolControlBarCmdUI

void CExtToolControlBar::CExtToolControlBarCmdUI::_SetCheckImpl(
	int nCheck,
	bool bUpdaeteInCmdManager
	)
{
	ASSERT(nCheck >= 0 && nCheck <= 2); // 0=>off, 1=>on, 2=>indeterminate
CExtToolControlBar* pToolBar = (CExtToolControlBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT_KINDOF(CExtToolControlBar, pToolBar);
	ASSERT(m_nIndex < m_nIndexMax);

	if( pToolBar->IsRightExpandButton(m_nIndex) )
		return;

UINT nNewStyle =
		pToolBar->GetButtonStyle(m_nIndex)
		&
		~(TBBS_CHECKED | TBBS_INDETERMINATE);
	if( nCheck == 1 )
		nNewStyle |= TBBS_CHECKED;
	else if( nCheck == 2 )
		nNewStyle |= TBBS_INDETERMINATE;
	ASSERT( !(nNewStyle & TBBS_SEPARATOR) );
	pToolBar->SetButtonStyle(m_nIndex, nNewStyle | TBBS_CHECKBOX);

	if( bUpdaeteInCmdManager )
	{
		CExtCmdManager::cmd_t * p_cmd = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( pToolBar->GetSafeHwnd() ),
			pToolBar->GetButtonID(m_nIndex)
			);
		if( p_cmd != NULL )
		{
			p_cmd->StateSetCheck(
				(nNewStyle &
					(TBBS_CHECKED|TBBS_INDETERMINATE)
					) ? true : false
				);
		}
	}
}

void CExtToolControlBar::CExtToolControlBarCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;
CExtToolControlBar* pToolBar = (CExtToolControlBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT_KINDOF(CExtToolControlBar, pToolBar);
	ASSERT(m_nIndex < m_nIndexMax);

	if( pToolBar->IsRightExpandButton(m_nIndex) )
		return;

UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) & ~TBBS_DISABLED;
	if( !bOn )
		nNewStyle |= TBBS_DISABLED;
	ASSERT(!(nNewStyle & TBBS_SEPARATOR));
	pToolBar->SetButtonStyle(m_nIndex, nNewStyle);

CExtCmdManager::cmd_t * p_cmd = g_CmdManager->CmdGetPtr(
		g_CmdManager->ProfileNameFromWnd( pToolBar->GetSafeHwnd() ),															
		pToolBar->GetButtonID(m_nIndex)
		);
	if( p_cmd != NULL )
		p_cmd->StateEnable( bOn ? true : false );
}

void CExtToolControlBar::CExtToolControlBarCmdUI::SetRadio(BOOL bOn)
{
	_SetCheckImpl( bOn ? 1 : 0, false );
	
CExtToolControlBar* pToolBar = (CExtToolControlBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT_KINDOF(CExtToolControlBar, pToolBar);
	ASSERT(m_nIndex < m_nIndexMax);

	if( pToolBar->IsRightExpandButton(m_nIndex) )
		return;

CExtCmdManager::cmd_t * p_cmd = g_CmdManager->CmdGetPtr(
		g_CmdManager->ProfileNameFromWnd( pToolBar->GetSafeHwnd() ),
		pToolBar->GetButtonID(m_nIndex)
		);
	if( p_cmd != NULL )
		p_cmd->StateSetRadio( bOn ? true : false );
}

void CExtToolControlBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CExtControlBar::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);

//	if( CExtPopupMenuWnd::IsMenuTracking() )
//		return;

CExtToolControlBarCmdUI state;
	state.m_pOther = this;

int nCountOfButtons = _GetButtonsCountImpl();
	state.m_nIndexMax = (UINT)nCountOfButtons;
	for(	state.m_nIndex = 0;
			state.m_nIndex < state.m_nIndexMax;
			state.m_nIndex++
			)
	{
		CExtBarButton * pTBB =
			_GetButtonPtr(state.m_nIndex);
		ASSERT_VALID( pTBB );
		if( pTBB->IsAbleToTrackMenu() ) // ignore menu drop buttons
			continue;
		state.m_nID = pTBB->GetCmdID();
		if( pTBB->IsSeparator() ) // ignore separators
			continue;
		state.DoUpdate(pTarget, bDisableIfNoHndler);
	}

	// update the dialog controls added to the toolbar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

void CExtToolControlBar::_UpdateButton(int nIndex)
{
	// determine target of command update
CFrameWnd* pTarget = (CFrameWnd*)GetOwner();
	if( pTarget == NULL || !pTarget->IsFrameWnd() )
		pTarget = GetParentFrame();
	if( pTarget == NULL )
		return;
	CExtBarButton * pTBB =
		_GetButtonPtr(nIndex);
	ASSERT_VALID( pTBB );
	if( pTBB->IsSeparator() )
		return;
	if( pTBB->IsAbleToTrackMenu() )
		return;
	// send the update notification
CExtToolControlBarCmdUI state;
	state.m_pOther = this;
	state.m_nIndex = nIndex;
int nCountOfButtons = _GetButtonsCountImpl();
	state.m_nIndexMax = (UINT)nCountOfButtons;
	state.m_nID = pTBB->GetCmdID();
	state.DoUpdate(pTarget, pTarget->m_bAutoMenuEnable);
}

/////////////////////////////////////////////////////////////////////////////
// CExtToolControlBar diagnostics

#ifdef _DEBUG
void CExtToolControlBar::AssertValid() const
{
	CExtControlBar::AssertValid();
}

void CExtToolControlBar::Dump(CDumpContext& dc) const
{
	CExtControlBar::Dump(dc);
}
#endif

// input CRect should be client rectangle size
void CExtToolControlBar::_CalcInsideRect(CRect& rect, BOOL bHorz) const
{
	ASSERT_VALID(this);
	CControlBar::CalcInsideRect(rect,bHorz);
}

void CExtToolControlBar::_RecalcPositionsImpl()
{
	if( GetSafeHwnd() == NULL
		|| !::IsWindow( GetSafeHwnd() )
		)
		return;
	ASSERT_VALID(this);

int nCountOfButtons = _GetButtonsCountImpl();
	if( nCountOfButtons == 0 )
		return;

BOOL bHorz = m_dwStyle & CBRS_ORIENT_HORZ ? TRUE : FALSE;
BOOL bFloating = IsFloating();

CRect rcInner;
	GetClientRect( &rcInner );
	rcInner.DeflateRect( __TB_BUTTON_MARGINE__, __TB_BUTTON_MARGINE__ );
CPoint ptLimitTL = rcInner.TopLeft();
CPoint ptLimitBR = rcInner.BottomRight();

CClientDC dc(this);
CFont* pOldFont = (CFont*)
		dc.SelectObject(
			( m_dwStyle & CBRS_ORIENT_HORZ )
				? &g_PaintManager->m_FontNormal
				: &g_PaintManager->m_FontNormalVert
			);
	ASSERT( pOldFont != NULL );

int nReviewCount = nCountOfButtons;
CSize sizeTBBRight( 0, 0 );
	if( m_pRightBtn != NULL )
	{
		nReviewCount--;
		ASSERT_VALID( m_pRightBtn );
		ASSERT( m_buttons.GetSize() > 0 );
		ASSERT( m_buttons[nReviewCount] == m_pRightBtn );
		m_pRightBtn->GetButtons().RemoveAll();
		m_pRightBtn->ClearContent();
		sizeTBBRight =
			m_pRightBtn->CalculateLayout( dc, m_sizeTBB, bHorz );
		if( bFloating )
		{
			m_pRightBtn->Show( FALSE );
			m_pRightBtn->SetRect(
				CRect( ptLimitTL, sizeTBBRight )
				);
			if( nReviewCount == 0 )
				return;
		} // if( bFloating )
		else
		{
			m_pRightBtn->SetRect(
				CRect(
					ptLimitBR-sizeTBBRight,
					sizeTBBRight
					)
				);
			m_pRightBtn->Show( TRUE );
			if( bHorz )
				rcInner.right -=
					sizeTBBRight.cx + __TB_BUTTON_MARGINE__;
			else
				rcInner.bottom -=
					sizeTBBRight.cy + __TB_BUTTON_MARGINE__;
		} // else from if( bFloating )
	} // if( m_pRightBtn != NULL )

CArray < CRect, CRect >	arrBtnRects;
CArray < BOOL, BOOL >	arrBtnVisibility;
CArray < BOOL, BOOL >	arrBtnSeparators;
//CArray < BOOL, BOOL >	arrBtnEnabled;
	arrBtnRects.SetSize( nReviewCount );
	arrBtnVisibility.SetSize( nReviewCount );
	arrBtnSeparators.SetSize( nReviewCount );
//	arrBtnEnabled.SetSize( nReviewCount );
CSize sizeLastWrappedRow( 0, 0 );
CPoint ptBtnPosCurr( ptLimitTL );
BOOL bSeparatorPrev = FALSE;
	for( int nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		BOOL bVisibleTBB = TRUE;
		pTBB->SetVertDocked( !bHorz );
//		arrBtnEnabled.SetAt( nBtnIdx, pTBB->IsEnabled() );
		BOOL bSeparator = pTBB->IsSeparator();
		arrBtnSeparators.SetAt( nBtnIdx, bSeparator );
		if( bSeparatorPrev && bSeparator )
		{ // remove double separators
			arrBtnVisibility.SetAt( nBtnIdx, FALSE );
			continue;
		} // remove double separators
		CSize sizeTBB =
			pTBB->CalculateLayout( dc, m_sizeTBB, bHorz );
		CRect rcTBB( ptBtnPosCurr, sizeTBB );
		if( bFloating && pTBB->IsWrap() )
		{
			ptBtnPosCurr.x = ptLimitTL.x;
			ptBtnPosCurr.y += m_sizeTBB.cy + __TB_LINE_OFFSET;
			sizeLastWrappedRow = CSize( 0, 0 );
		} // if( bFloating && pTBB->IsWrap() )
		else
		{
			bool bResyncVisibleTBB = false;
			if( bHorz )
			{
				ptBtnPosCurr.x += sizeTBB.cx;
				if( ptBtnPosCurr.x > rcInner.right )
					bResyncVisibleTBB = true;
			} // if( bHorz )
			else
			{
				ptBtnPosCurr.y += sizeTBB.cy;
				if( ptBtnPosCurr.y > rcInner.bottom )
					bResyncVisibleTBB = true;
			} // else from if( bHorz )
			if( bResyncVisibleTBB )
			{
				int nSpaceToFind =
					bHorz
						? ptBtnPosCurr.x - rcInner.right
						: ptBtnPosCurr.y - rcInner.bottom;
				int nBtnWithNearestWidth = nBtnIdx;
				int nNearestMetric = bHorz ? rcTBB.Width() : rcTBB.Height();
				int nNearestDiff = nSpaceToFind - nNearestMetric;
				// hide nearest by size reviewed visible
				for( int nBtnIdx2 = nBtnIdx-1; nBtnIdx2 >= 0; nBtnIdx2-- )
				{
					if( !arrBtnVisibility[nBtnIdx2] )
						continue;
					if( arrBtnSeparators[nBtnIdx2] )
						continue;
					int nMetric =
						bHorz
							? arrBtnRects[nBtnIdx2].Width()
							: arrBtnRects[nBtnIdx2].Height();
					//if( nMetric <= nNearestMetric ) // <<-- widest search algorithm
					//	continue;
					if( nMetric > nSpaceToFind )
						continue;
					int nDiff = nSpaceToFind - nMetric;
//					if( !arrBtnEnabled[nBtnIdx2] )
//					{ // hide disabled buttons first
//						nNearestDiff = nDiff;
//						nNearestMetric = nMetric;
//						nBtnWithNearestWidth = nBtnIdx2;
//						break;
//					} // hide disabled buttons first
					if( nDiff >= nNearestDiff )
						continue;
					nNearestDiff = nDiff;
					nNearestMetric = nMetric;
					nBtnWithNearestWidth = nBtnIdx2;
				} // for( int nBtnIdx2 = nBtnIdx-1; nBtnIdx2 >= 0; nBtnIdx2-- )
				if( nBtnWithNearestWidth < nBtnIdx )
				{
					ASSERT( bVisibleTBB );
					arrBtnVisibility.SetAt( nBtnWithNearestWidth, FALSE );
					if( m_pRightBtn!=NULL && !arrBtnSeparators[nBtnWithNearestWidth] )
					{
						CExtBarButton * pTBB2 =
							_GetButtonPtr( nBtnWithNearestWidth );
						ASSERT_VALID( pTBB2 );
						m_pRightBtn->GetButtons().Add( pTBB2 );
					}
					if( nBtnWithNearestWidth > 0 && nBtnWithNearestWidth < nReviewCount-1 )
					{ // remove double separators
						if(		arrBtnSeparators[nBtnWithNearestWidth-1]
							&&	arrBtnSeparators[nBtnWithNearestWidth+1]
							)
						{
							arrBtnVisibility.SetAt( nBtnWithNearestWidth-1, FALSE );
							nNearestMetric +=
								bHorz
									? arrBtnRects[nBtnWithNearestWidth-1].Width()
									: arrBtnRects[nBtnWithNearestWidth-1].Height();
						}
					} // remove double separators
					rcTBB.OffsetRect(
						bHorz ? -nNearestMetric : 0,
						bHorz ? 0 : -nNearestMetric
						);
					(bHorz ? ptBtnPosCurr.x : ptBtnPosCurr.y) -= nNearestMetric;
					for( nBtnIdx2 = nBtnWithNearestWidth+1; nBtnIdx2 < nBtnIdx; nBtnIdx2++ )
					{
						if( !arrBtnVisibility[nBtnIdx2] )
							continue;
						CRect rcTBB2 = arrBtnRects[nBtnIdx2];
						rcTBB2.OffsetRect(
							bHorz ? -nNearestMetric : 0,
							bHorz ? 0 : -nNearestMetric
							);
						arrBtnRects.SetAt( nBtnIdx2, rcTBB2 );
					} // for( nBtnIdx2 = nBtnWithNearestWidth+1; nBtnIdx2 < nBtnIdx; nBtnIdx2++ )
				} // if( nBtnWithNearestWidth < nBtnIdx )
				else
				{
					ASSERT( nBtnWithNearestWidth == nBtnIdx );
					bVisibleTBB = FALSE;
					(bHorz ? ptBtnPosCurr.x : ptBtnPosCurr.y) -=
						(bHorz ? sizeTBB.cx : sizeTBB.cy);
				} // else from if( nBtnWithNearestWidth < nBtnIdx )
			} // if( bResyncVisibleTBB )
		} // else from if( bFloating && pTBB->IsWrap() )

		arrBtnVisibility.SetAt( nBtnIdx, bVisibleTBB );
		arrBtnRects.SetAt( nBtnIdx, rcTBB );
		if( bVisibleTBB )
			bSeparatorPrev = bSeparator;

		if( m_pRightBtn!=NULL && !bVisibleTBB && !bSeparator )
			m_pRightBtn->GetButtons().Add( pTBB );
	} // for( int nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )

	dc.SelectObject( pOldFont );
	
	for( nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )
	{
		CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
		ASSERT_VALID( pTBB );
		pTBB->Show( arrBtnVisibility[nBtnIdx] );
		pTBB->SetRect( arrBtnRects[nBtnIdx] );
	} // for( nBtnIdx = 0; nBtnIdx < nReviewCount; nBtnIdx++ )

	// insert separators into right button
	if(		bFloating
		||	m_pRightBtn == NULL
		||	m_pRightBtn->GetButtons().GetSize() == 0
		)
		return;
int nHiddenCount = m_pRightBtn->GetButtons().GetSize();
	if( nHiddenCount < 2 )
		return;
int nEndMeasure = nHiddenCount-1;
	for( int iHidden = 0; iHidden < nEndMeasure; iHidden++ )
	{
		CExtBarButton * pTbbHidden0 =
			m_pRightBtn->GetButtons().GetAt(iHidden);
		ASSERT( pTbbHidden0 != NULL );
		ASSERT( ! pTbbHidden0->IsSeparator() );
		ASSERT( ! pTbbHidden0->IsVisible() );
		CExtBarButton * pTbbHidden1 =
			m_pRightBtn->GetButtons().GetAt(iHidden+1);
		ASSERT( pTbbHidden1 != NULL );
		ASSERT( ! pTbbHidden1->IsSeparator() );
		ASSERT( ! pTbbHidden1->IsVisible() );
		CExtBarButton * pTbbSeparatorToInsert = NULL;
		int nIdx0=-1,nIdx1=-1;
		for( nBtnIdx = 0; nBtnIdx < nCountOfButtons; nBtnIdx++ )
		{
			CExtBarButton * pTBB = _GetButtonPtr( nBtnIdx );
			ASSERT_VALID( pTBB );
			if( pTBB == pTbbHidden0 )
			{
				nIdx0 = nBtnIdx;
				ASSERT( nIdx1 < 0 );
				continue;
			}
			if( nIdx0 >= 0
				&& pTBB->IsSeparator()
				&& pTbbSeparatorToInsert == NULL
				)
				pTbbSeparatorToInsert = pTBB;
			if( pTBB == pTbbHidden1 )
			{
				nIdx1 = nBtnIdx;
				ASSERT( nIdx0 >= 0 && nIdx0 < nIdx1 );
				break;
			}
		}
		ASSERT(
			nIdx0 < nIdx1
			&&
			nIdx0 >= 0 && nIdx0 < nCountOfButtons
			&& 
			nIdx1 >= 0 && nIdx1 < nCountOfButtons
			);
		if( (nIdx0+1) == nIdx1 )
			continue;
		if( pTbbSeparatorToInsert != NULL )
		{
			nEndMeasure++;
			iHidden++;
			m_pRightBtn->GetButtons().InsertAt(
				iHidden,
				pTbbSeparatorToInsert
				);
		}
	} // for( int iHidden = 0; iHidden < nEndMeasure; iHidden++ )
}

DWORD CExtToolControlBar::RecalcDelayShow(AFX_SIZEPARENTPARAMS* lpLayout)
{
DWORD dwRes = CControlBar::RecalcDelayShow( lpLayout );
	if( !IsFloating() )
		_RecalcPositionsImpl();
	return dwRes;
}

void CExtToolControlBar::OnSize(UINT nType, int cx, int cy) 
{
	CExtControlBar::OnSize(nType, cx, cy);
	_RecalcLayoutImpl();
}

void CExtToolControlBar::_RecalcLayoutImpl()
{
	if( GetSafeHwnd() == NULL )
		return;
	CExtControlBar::_RecalcLayoutImpl();
	_RecalcPositionsImpl();
	Invalidate();
	UpdateWindow();
}

void CExtToolControlBar::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CExtControlBar::OnWindowPosChanging(lpwndpos);
	_RecalcLayoutImpl();
}

void CExtToolControlBar::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == ID_HOVER_TIMER )
	{
		_UpdateHoverButton();
		return;
	}
	if( nIDEvent == ID_MENUDROPDOWN_TIMER )
	{
//		if( !g_bMenuTracking )
//			return;
		if( m_nBtnIdxMenuTracking >= 0
			&& CExtPopupMenuWnd::IsMenuTracking()
			)
			return;
		_SwitchMenuTrackingIndex();
		if( _GetMenuTrackingBar() == NULL )
			g_bMenuTracking = false;
		return;
	}
	
	
	CExtControlBar::OnTimer(nIDEvent);
}

void CExtToolControlBar::_SwitchMenuTrackingIndex(
	int iNewMenuTrackingIndex  // = -1
	)
{
	if( m_nBtnIdxMenuTracking >= 0 )
	{
		KillTimer( ID_MENUDROPDOWN_TIMER );
//		CExtPopupMenuWnd::CancelMenuTracking();
		CExtBarButton * pTBB =
			_GetButtonPtr( m_nBtnIdxMenuTracking );
		ASSERT_VALID( pTBB );
		ASSERT( !(pTBB->IsSeparator()) );
		UINT nNewStyle =
			pTBB->GetStyle()
			&
			~(TBBS_PRESSED|TBBS_CHECKED|TBBS_INDETERMINATE);
		pTBB->SetStyle( nNewStyle );
		_InvalidateButton( m_nBtnIdxMenuTracking );
		m_nBtnIdxMenuTracking = -1;
	} // if( m_nBtnIdxMenuTracking >= 0 )

	if( iNewMenuTrackingIndex >= 0 )
	{
		m_nBtnIdxMenuTracking = iNewMenuTrackingIndex;
		CExtBarButton * pTBB =
			_GetButtonPtr( m_nBtnIdxMenuTracking );
		ASSERT_VALID( pTBB );
		ASSERT( !(pTBB->IsSeparator()) );
		UINT nNewStyle =
			pTBB->GetStyle()
			&
			~(TBBS_CHECKED|TBBS_INDETERMINATE);
		nNewStyle |= TBBS_PRESSED;
		pTBB->SetStyle( nNewStyle );
		_InvalidateButton( m_nBtnIdxMenuTracking );
		CExtToolControlBar::g_bMenuTracking = true;
		SetTimer(
			ID_MENUDROPDOWN_TIMER,
			20,
			NULL
			);
	} // if( iNewMenuTrackingIndex >= 0 )
}

CExtToolControlBar * CExtToolControlBar::_GetMenuTrackingBar()
{
	for( INT iBar=0; iBar<g_AllBars.GetSize(); ++iBar )
	{
		CExtControlBar * pBar = g_AllBars[iBar];
		ASSERT_VALID( pBar );
		CExtToolControlBar * pToolControlBar =
			DYNAMIC_DOWNCAST(CExtToolControlBar,pBar);
		if( pToolControlBar == NULL )
			continue;
		if( pToolControlBar->m_nBtnIdxMenuTracking >= 0 )
			return pToolControlBar;
	}
	return NULL;
}

void CExtToolControlBar::_CloseTrackingMenus()
{
	g_bMenuTracking = false;
	for( INT iBar=0; iBar<g_AllBars.GetSize(); ++iBar )
	{
		CExtControlBar * pBar = g_AllBars[iBar];
		//ASSERT_VALID( pBar );
		ASSERT( pBar != NULL );
		CExtToolControlBar * pToolControlBar =
			DYNAMIC_DOWNCAST(CExtToolControlBar,pBar);
		if( pToolControlBar == NULL )
			continue;
		pToolControlBar->_SwitchMenuTrackingIndex();
	}
	CExtPopupMenuWnd::CancelMenuTracking();
//	g_bMenuTracking = false;
}

void CExtToolControlBar::_CbPaintCombinedContent(
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
CExtToolControlBar * pBar = (CExtToolControlBar *)pCookie;
	ASSERT_VALID( pBar );
	ASSERT( pBar->IsKindOf(RUNTIME_CLASS(CExtToolControlBar)) );
	ASSERT( eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE );

	if( rcExcludeArea.IsRectEmpty() )
		return;
	if( pBar->m_nBtnIdxMenuTracking < 0 )
		return;
	ASSERT(
		pBar->m_nBtnIdxMenuTracking <
			pBar->GetButtonsCount()
		);
CExtBarButton * pTBB =
		pBar->_GetButtonPtr(
			pBar->m_nBtnIdxMenuTracking
			);
	ASSERT_VALID( pTBB );
UINT nStyle = pTBB->GetStyle();
BOOL bHover = pTBB->IsHover();
	pTBB->SetHover( FALSE );
	pTBB->ModifyStyle(
		0,
		TBBS_PRESSED|TBBS_CHECKED|TBBS_CHECKBOX
		);
CRect rcClientBar,rcClientExcludeArea;
	pBar->GetClientRect( &rcClientBar );
	pBar->ClientToScreen( &rcClientBar );
	refWndMenu.GetClientRect( &rcClientExcludeArea );
	refWndMenu.ClientToScreen( &rcClientExcludeArea );
CPoint ptOffset =
		rcClientBar.TopLeft()
		- rcClientExcludeArea.TopLeft()
		;
CPoint ptViewportOrg = dc.GetViewportOrg();
	dc.SetViewportOrg( ptOffset );
bool bHorz = pBar->IsDockedVertically() ? false : true;
	pTBB->Paint(dc,bHorz);
	dc.SetViewportOrg( ptViewportOrg );
	pTBB->SetHover( bHover );
	pTBB->SetStyle(nStyle);
}
