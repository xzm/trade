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

#if (!defined __EXT_MENUCONTROLBAR_H)
	#include <ExtMenuControlBar.h>
#endif

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXTDOCKBAR_H)
	#include "ExtDockBar.h"
#endif

#if( !defined __EXTMINIDOCKFRAMEWND_H)
	#include "ExtMiniDockFrameWnd.h"
#endif

#if (!defined __ExtCmdManager_H)
	#include <ExtCmdManager.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#if (!defined __EXT_LOCALIZATION_H)
	#include <../Src/ExtLocalization.h>
#endif

#include <../profuisdll/resource.h>

#define ID_TIMER_FLAT_TRACKING 300
#define ID_TIMER_SYS_TRACKING 301

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT CExtMenuControlBar::nMsgTrackButtonMenu =
	::RegisterWindowMessage(
		_T("CExtMenuControlBar::nMsgTrackButtonMenu")
		);
UINT CExtMenuControlBar::nMsgSetMenuNULL =
	::RegisterWindowMessage(
		_T("CExtMenuControlBar::nMsgSetMenuNULL")
		);


/////////////////////////////////////////////////////////////////////////////
// CExtMenuControlBar hook sink

bool CExtMenuControlBar::OnHookWndMsg(
	LRESULT & lResult,
	HWND hWndHooked,
	UINT nMessage,
	WPARAM wParam,
	LPARAM lParam
	)
{
	if( GetSafeHwnd() == NULL || !::IsWindow( GetSafeHwnd() ) )
		return 
			CExtHookSink::OnHookWndMsg(
				lResult,
				hWndHooked,
				nMessage,
				wParam,
				lParam
				);

	if( hWndHooked == _GetHwndMainFrame() )
	{
		if( nMessage == WM_PARENTNOTIFY
			&&
			CExtPopupMenuWnd::IsMenuTracking()
			)
		{
			lResult = 0;
			return true;
		}

		if( nMessage == WM_SIZE
			//|| nMessage == WM_WINDOWPOSCHANGING
			//|| nMessage == WM_WINDOWPOSCHANGED
			//|| nMessage == WM_ACTIVATE
			|| nMessage == WM_ACTIVATEAPP
			|| nMessage == WM_COMMAND
			|| nMessage == WM_SYSCOMMAND
			)
		{
			m_bFlatTracking = FALSE;
			if( CExtMenuControlBar::g_bMenuTracking )
				CExtMenuControlBar::_CloseTrackingMenus();
			if(	CExtPopupMenuWnd::IsMenuTracking() )
				CExtPopupMenuWnd::CancelMenuTracking();
			return 
				CExtHookSink::OnHookWndMsg(
					lResult,
					hWndHooked,
					nMessage,
					wParam,
					lParam
					);
		}

		if( nMessage == WM_NCMOUSEMOVE
			&& CExtMenuControlBar::g_bMenuTracking
			&& !(m_bSysMenuTracking)
			)
		{
			CRect rcSysIcon = _GetMainFrameSysIconRect();
			CPoint point;
			VERIFY( ::GetCursorPos(&point) );
			if( rcSysIcon.PtInRect(point) )
			{
				TrackMainFrameSystemMenu(
					NULL,
					FALSE
					);
				lResult = 0;
				return true;
			}
		}

		if( (nMessage == WM_NCLBUTTONUP && wParam == HTSYSMENU)
			|| (nMessage == WM_NCRBUTTONUP && (wParam == HTCAPTION || wParam == HTSYSMENU))
			)
		{
			CExtToolControlBar::g_bMenuTrackingExpanded = false;
			TrackMainFrameSystemMenu(
				NULL,
				FALSE
				);
			lResult = 0;
			return true;
		}
		if( (nMessage == WM_NCLBUTTONDOWN || nMessage == WM_NCRBUTTONDOWN)
			&& wParam == HTSYSMENU
			)
		{
			lResult = 0;
			return true;
		}
		if( nMessage == WM_NCRBUTTONDOWN && wParam == HTCAPTION )
		{
			CExtToolControlBar::g_bMenuTrackingExpanded = false;
			CPoint point;
			VERIFY( GetCursorPos(&point) );
			TrackMainFrameSystemMenu(
				&point,
				FALSE
				);
			lResult = 0;
			return true;
		}

		if( nMessage == CExtPopupMenuWnd::nMsgPopupNext
			|| nMessage == CExtPopupMenuWnd::nMsgPopupPrev
			)
		{ // if menu next/prev popup activation messages
			int iTrackingIndex =
				GetMenuTrackingButton();
			//if( iTrackingIndex < 0 ) // menu tracked on other bar
			//{
			//	lResult = 0
			//	return true;
			//}
			//ASSERT( iTrackingIndex >= 0 );

			BOOL bDummyMax = FALSE;
			HWND hWndMdiChild = _GetActiveMdiChildWnd(bDummyMax);
			if( nMessage == CExtPopupMenuWnd::nMsgPopupPrev
				&& iTrackingIndex == 0
				)
			{
				if( (! (m_bMdiApp && hWndMdiChild != NULL ) )
					||
					(!TrackChildFrameSystemMenu(NULL,TRUE))
					)
					TrackMainFrameSystemMenu(NULL,TRUE);
				lResult = 0;
				return true;
			}

			if(	nMessage == CExtPopupMenuWnd::nMsgPopupNext
				&& m_bMdiApp
				&& m_bSysMenuTracking
				&& iTrackingIndex < 0
				&& hWndMdiChild != NULL
				&& CExtPopupMenuWnd::GetCmdTargetWnd() !=
					hWndMdiChild
				)
			{
				TrackChildFrameSystemMenu(NULL,TRUE);
				lResult = 0;
				return true;
			}
			int nCountOfButtons = GetButtonsCount();
			if( nMessage == CExtPopupMenuWnd::nMsgPopupNext
				&& iTrackingIndex == GetVisibleButton(-1,FALSE)//(nCountOfButtons-1)
				)
			{
				TrackMainFrameSystemMenu(NULL,TRUE);
				lResult = 0;
				return true;
			}

			int iNewTrackIndex =
				GetVisibleButton(
					iTrackingIndex,
					(nMessage == CExtPopupMenuWnd::nMsgPopupNext) ?
						TRUE : FALSE
					);
			if( iNewTrackIndex == iTrackingIndex )
			{
				lResult = 0; // same as tracking now
				return true;
			}
			TrackButtonMenu(iNewTrackIndex);
			lResult = 0;
			return true;
		} // if menu next/prev popup activation messages

		if( nMessage == CExtPopupMenuWnd::nMsgNotifyMenuExpanded )
		{ // if menu rarely used items was expanded
			CExtToolControlBar::g_bMenuTrackingExpanded = true;
			lResult = 0;
			return true;
		} // if menu rarely used items was expanded

		if( !CExtControlBar::IsOleIpObjActive() )
			_KillFrameMenu();
	
	} // if( hWndHooked == _GetHwndMainFrame() )
	else if( hWndHooked == _GetHwndChildFrame() )
	{

		if( nMessage == WM_PARENTNOTIFY
			&&
			CExtPopupMenuWnd::IsMenuTracking()
			)
		{
			lResult = 0;
			return true;
		}

		if( nMessage == WM_SIZE
			//|| nMessage == WM_WINDOWPOSCHANGING
			|| nMessage == WM_WINDOWPOSCHANGED
			//|| nMessage == WM_ACTIVATE
			|| nMessage == WM_ACTIVATEAPP
			|| nMessage == WM_COMMAND
			|| nMessage == WM_SYSCOMMAND
			)
		{
			m_bFlatTracking = FALSE;
			if( CExtMenuControlBar::g_bMenuTracking )
				CExtMenuControlBar::_CloseTrackingMenus();
			if(	CExtPopupMenuWnd::IsMenuTracking() )
				CExtPopupMenuWnd::CancelMenuTracking();
			if( !CExtControlBar::IsOleIpObjActive() )
				_KillFrameMenu();
			return 
				CExtHookSink::OnHookWndMsg(
					lResult,
					hWndHooked,
					nMessage,
					wParam,
					lParam
					);
		}

		if( nMessage == WM_NCMOUSEMOVE
			&& CExtMenuControlBar::g_bMenuTracking
			&& !(m_bSysMenuTracking)
			)
		{
			CRect rcSysIcon = _GetChildFrameSysIconRect();
			if( rcSysIcon.IsRectEmpty() )
			{
				lResult = 0;
				return true;
			}
			CPoint point;
			VERIFY( ::GetCursorPos(&point) );
			if( rcSysIcon.PtInRect(point) )
			{
				TrackChildFrameSystemMenu(
					NULL,
					FALSE
					);
				lResult = 0;
				return true;
			}
		}

		if( (nMessage == WM_NCLBUTTONUP && wParam == HTSYSMENU)
			|| (nMessage == WM_NCRBUTTONUP && (wParam == HTCAPTION || wParam == HTSYSMENU))
			)
		{
			CExtToolControlBar::g_bMenuTrackingExpanded = false;
			TrackChildFrameSystemMenu(
				NULL,
				FALSE
				);
			lResult = 0;
			return true;
		}
		if( (nMessage == WM_NCLBUTTONDOWN || nMessage == WM_NCRBUTTONDOWN)
			&& wParam == HTSYSMENU
			)
		{
			lResult = 0;
			return true;
		}
		if( nMessage == WM_NCRBUTTONDOWN && wParam == HTCAPTION )
		{
			CExtToolControlBar::g_bMenuTrackingExpanded = false;
			CPoint point;
			VERIFY( GetCursorPos(&point) );
			TrackChildFrameSystemMenu(
				&point,
				FALSE
				);
			lResult = 0;
			return true;
		}


		if( nMessage == CExtPopupMenuWnd::nMsgPopupNext
			|| nMessage == CExtPopupMenuWnd::nMsgPopupPrev
			)
		{ // if menu next/prev popup activation messages
			int iTrackingIndex =
				GetMenuTrackingButton();
//			if( iTrackingIndex < 0 ) // menu tracked on other bar
//			{
//				lResult = 0;
//				return true;
//			}
			//ASSERT( iTrackingIndex >= 0 );

			if( nMessage == CExtPopupMenuWnd::nMsgPopupPrev )
			{
				TrackMainFrameSystemMenu(NULL,TRUE);
				lResult = 0;
				return true;
			}

			int iNewTrackIndex = 0;
			int nCountOfButtons = GetButtonsCount();
//			if( nMessage == CExtPopupMenuWnd::nMsgPopupNext )
//			{
//			}
//			else
			{
				iNewTrackIndex =
					GetVisibleButton(
						iTrackingIndex,
						(nMessage == CExtPopupMenuWnd::nMsgPopupNext) ?
							TRUE : FALSE
						);
			}

			if( iNewTrackIndex == iTrackingIndex )
			{
				lResult = 0; // same as tracking now
				return true;
			}
			TrackButtonMenu(iNewTrackIndex);
			lResult = 0;
			return true;
		} // if menu next/prev popup activation messages

	} // else if( hWndHooked == _GetHwndChildFrame() )
	else if( hWndHooked == _GetHwndMdiArea() )
	{
		if( nMessage == WM_PARENTNOTIFY
			&&
			CExtPopupMenuWnd::IsMenuTracking()
			)
		{
			lResult = 0;
			return true;
		}

		if( nMessage == WM_MDISETMENU )
		{
			HMENU hOldMdiMenu = NULL;
			HMENU hNewMdiMenu = (HMENU)wParam;
			HMENU hNewWindowMenu = (HMENU)lParam;
			if( hNewMdiMenu != NULL )
			{
				ASSERT( ::IsMenu(hNewMdiMenu) );
				hOldMdiMenu =
					_SetMdiMenu(
						hNewMdiMenu //, hNewWindowMenu
						);
			}
			lResult = (LRESULT)hOldMdiMenu;
			return true;
		}
		if( nMessage == WM_MDIREFRESHMENU )
		{
			lResult = 0;
			return true;
		}

		if( nMessage == WM_CONTEXTMENU ) // WM_RBUTTONDOWN
		{
			_ContextMenuTrack();
			lResult = 0;
			return true;
		}
	} // else if( hWndHooked == _GetHwndMdiArea() )
	else
	{
		ASSERT( FALSE );
	}

	return 
		CExtHookSink::OnHookWndMsg(
			lResult,
			hWndHooked,
			nMessage,
			wParam,
			lParam
			);
}

/////////////////////////////////////////////////////////////////////////////
// CExtMenuControlBar

IMPLEMENT_DYNAMIC(CExtMenuControlBar, CExtToolControlBar)

CExtMenuControlBar::CExtMenuControlBar()
{
	m_bUpdateFileMruList = TRUE;
	m_bFlatTracking = FALSE;
	m_bSysMenuTracking = FALSE;
	m_nFlatTrackingIndex = m_nOldTrackingIndex = -1;
	m_bMdiApp = FALSE;

	m_sMdiWindowPopupName = _T("");
	m_nMdiDocButtonCmdID = 0;
	m_hWndHelperActiveChild = NULL;
}

CExtMenuControlBar::~CExtMenuControlBar()
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

BEGIN_MESSAGE_MAP(CExtMenuControlBar, CExtToolControlBar)
    //{{AFX_MSG_MAP(CExtMenuControlBar)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CExtMenuControlBar::nMsgTrackButtonMenu, OnTrackButtonMenu)
	ON_REGISTERED_MESSAGE(CExtMenuControlBar::nMsgSetMenuNULL, OnSetMenuNULL)
END_MESSAGE_MAP()

// LoadMenuBar() required only for CFrameWnd
// based windows principally without menu
BOOL CExtMenuControlBar::LoadMenuBar(
	UINT nResourceID
	)
{
	if( m_menu.GetSafeHmenu() != NULL )
	{
		VERIFY( m_menu.DestroyMenu() );
	}
	if( GetSafeHwnd() == NULL
		|| !::IsWindow( GetSafeHwnd() )
		|| m_menu.GetSafeHmenu() != NULL
		|| !IsHookedWindow( _GetHwndMainFrame() )
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}
	if( !m_menu.LoadMenu(nResourceID) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT( ::IsMenu(m_menu.GetSafeHmenu()) );
	if( !g_CmdManager->UpdateFromMenu(
			g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
			m_menu.GetSafeHmenu()
			)
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}

	return _UpdateMenuBar();
}

CMenu & CExtMenuControlBar::GetMenu()
{
	return m_menu;
}

BOOL CExtMenuControlBar::UpdateMenuBar( // update after menu changed
	BOOL bDoRecalcLayout // = TRUE
	)
{
	return UpdateMenuBar( bDoRecalcLayout );
}

BOOL CExtMenuControlBar::_UpdateMenuBar(
	BOOL bDoRecalcLayout // = TRUE
	)
{
	SetButtons(); // remove all buttons

	if( m_menu.GetSafeHmenu() != NULL )
	{
		ASSERT( ::IsMenu(m_menu.GetSafeHmenu()) );

		UINT nMenuItemCount = m_menu.GetMenuItemCount();
		for( UINT iMenu=0; iMenu<nMenuItemCount; iMenu++ )
		{
			MENUITEMINFO mii;
			::memset(&mii,0,sizeof(MENUITEMINFO));
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask =
				MIIM_CHECKMARKS
				|MIIM_DATA
				|MIIM_ID
				|MIIM_STATE
				|MIIM_SUBMENU
				|MIIM_TYPE
				;
			mii.cch = __MAX_UI_ITEM_TEXT;
			CString sText;
			mii.dwTypeData =
				sText.GetBuffer(__MAX_UI_ITEM_TEXT);
			ASSERT( mii.dwTypeData != NULL );
			if( mii.dwTypeData == NULL )
			{
				ASSERT( FALSE );
				return FALSE;
			}
			if( !m_menu.GetMenuItemInfo(
					iMenu,
					&mii,
					TRUE
					)
				)
			{
				sText.ReleaseBuffer();
				ASSERT( FALSE );
				return false;
			}
			sText.ReleaseBuffer();

			BOOL bAppendMdiWindowsMenu = FALSE;
			UINT nCmdID = 0;
			CExtCmdManager::cmd_t * p_cmd = NULL;
			if( mii.hSubMenu == NULL )
			{
				nCmdID = mii.wID;
				if( nCmdID == ID_SEPARATOR )
				{
					if( !InsertButton(
							iMenu,
							nCmdID,
							FALSE
							)
						)
					{
						ASSERT( FALSE );
						return FALSE;
					}
					continue;
				}
				ASSERT( CExtCmdManager::IsCommand(nCmdID) );
				p_cmd =
					g_CmdManager->CmdGetPtr(
						g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
						nCmdID
						);
				ASSERT( p_cmd != NULL );
			} // if( mii.hSubMenu == NULL )
			else
			{
				p_cmd =
					g_CmdManager->CmdAllocPtr(
						g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() )
						);
				if( p_cmd == NULL )
				{
					ASSERT( FALSE );
					return FALSE;
				}
				nCmdID = p_cmd->m_nCmdID;
				ASSERT( CExtCmdManager::IsCommand(nCmdID) );

				if( m_bMdiApp && !m_sMdiWindowPopupName.IsEmpty() )
				{
					CString _sText(sText);
					_sText.TrimLeft();
					_sText.TrimRight();
					while( _sText.Replace(_T("&"),_T("")) > 0 )
					{
						_sText.TrimLeft();
						_sText.TrimRight();
					}
					if( _sText == m_sMdiWindowPopupName )
						bAppendMdiWindowsMenu = TRUE;
				} // if( m_bMdiApp && !m_sMdiWindowPopupName.IsEmpty() )
			} // else from if( mii.hSubMenu == NULL )
			ASSERT( p_cmd != NULL );
			if( p_cmd->m_sToolbarText.IsEmpty() )
				p_cmd->m_sToolbarText = sText;
			if( p_cmd->m_sMenuText.IsEmpty() )
				p_cmd->m_sMenuText = sText;
			if( !InsertButton(
					iMenu,
					nCmdID,
					FALSE
					)
				)
			{
				ASSERT( FALSE );
				return FALSE;
			}

			if( mii.hSubMenu != NULL )
			{
				ASSERT( ::IsMenu(mii.hSubMenu) );
				SetButtonMenu(
					iMenu,
					mii.hSubMenu,
					FALSE,
					FALSE,
					FALSE
					);
			}
			if( bAppendMdiWindowsMenu )
			{
				VERIFY(
					MarkButtonAsMdiWindowsMenu(
						iMenu,
						TRUE
						)
					);
			}

		} // for( UINT iMenu=0; iMenu<nMenuItemCount; iMenu++ )

		ASSERT( m_pRightBtn == NULL );
		m_pRightBtn =
			new CExtBarContentExpandButton(this);
		ASSERT_VALID( m_pRightBtn );
		m_buttons.Add( m_pRightBtn );
	} // if( m_menu.GetSafeHmenu() != NULL )

	
	if( bDoRecalcLayout )
		_RecalcLayoutImpl();
/*
CFrameWnd * pFrame = _GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	ASSERT( ::IsWindow(pFrame->GetSafeHwnd()) );
	pFrame->RecalcLayout();

	// floating frame
CFrameWnd * pMiniFrame = GetParentFrame();
	if( pMiniFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		pMiniFrame->RecalcLayout();
*/

	return TRUE;
}

/*
void CExtMenuControlBar::_RemoveFrameMenu()
{
CFrameWnd * pFrame = GetParentFrame(); //_GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	ASSERT( ::IsWindow(pFrame->GetSafeHwnd()) );
	VERIFY(
		pFrame->PostMessage(
			nMsgSetMenuNULL,
			(WPARAM)(pFrame->GetSafeHwnd())
			)
		);

	ASSERT_VALID(m_pDockSite);
	if( m_pDockSite->GetMenu() )
	{
		PostMessage(
			nMsgSetMenuNULL,
			(WPARAM)m_pDockSite->GetSafeHwnd()
			);
	}
}
*/

HWND CExtMenuControlBar::_GetActiveMdiChildWnd(
	BOOL & bMaximized
	)
{
	//ASSERT( m_bMdiApp );
	bMaximized = FALSE;

	if( !m_bMdiApp )
		return NULL;

CMDIFrameWnd * pFrame =
		DYNAMIC_DOWNCAST(
			CMDIFrameWnd,
			_GetDockingFrameImpl()
			);
	ASSERT_VALID( pFrame );
HWND hWndMdiArea = pFrame->m_hWndMDIClient;
	ASSERT( hWndMdiArea != NULL );
	ASSERT( ::IsWindow(hWndMdiArea) );
BOOL bMax = FALSE;
HWND hWnd = (HWND)
		::SendMessage(
			hWndMdiArea,
			WM_MDIGETACTIVE,
			0,
			(LPARAM)&bMax
			);
	ASSERT(
		hWnd == NULL
		|| ::IsWindow( hWnd )
		);
	bMaximized = bMax;
	return hWnd;
}

void CExtMenuControlBar::OnUpdateCmdUI(
	CFrameWnd * pTarget,
	BOOL bDisableIfNoHndler
	)
{
BOOL bDoRecalcLayout = FALSE;
	if( m_bMdiApp )
	{
		bDoRecalcLayout = _InstallMdiDocButtons( FALSE );
		VERIFY( _SyncActiveMdiChild() );
	}
	CExtToolControlBar::OnUpdateCmdUI(
		pTarget,
		bDisableIfNoHndler
		);
	if( bDoRecalcLayout )
		_RecalcLayoutImpl();
}

HMENU CExtMenuControlBar::_SetMdiMenu(
	HMENU hNewMdiMenu //, HMENU hNewWindowMenu
	)
{
BOOL bDoRecalcLayout = FALSE;
HMENU hOldMdiMenu = NULL;
	if(	hNewMdiMenu != NULL
		&&	(
				(!m_menu.GetSafeHmenu())
				|| m_menu.GetSafeHmenu() != hNewMdiMenu
			)
		)
	{ // if frame window menu changed
		ASSERT( ::IsMenu(hNewMdiMenu) );
		_KillFrameMenu();
		hOldMdiMenu = m_menu.Detach();
		m_menu.Attach(hNewMdiMenu); // menu is shared with MFC
		VERIFY( _UpdateMenuBar( FALSE ) );
		bDoRecalcLayout = TRUE;
	} // if frame window menu changed

	if( m_bMdiApp )
	{
		_InstallMdiDocButtons( FALSE );
		VERIFY( _SyncActiveMdiChild() );
		bDoRecalcLayout = TRUE;
	} // if( m_bMdiApp )

	if( bDoRecalcLayout )
		_RecalcLayoutImpl();

	return hOldMdiMenu;
}

BOOL CExtMenuControlBar::_SyncActiveMdiChild()
{
BOOL bMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bMax );
	if( m_hWndHelperActiveChild == hWndActiveChild )
		return TRUE;
	if( m_hWndHelperActiveChild != NULL )
		SetupHookWndSink( m_hWndHelperActiveChild, true );
	m_hWndHelperActiveChild = hWndActiveChild;
	if( m_hWndHelperActiveChild != NULL )
		SetupHookWndSink( m_hWndHelperActiveChild );
	UpdateWindow();
	Invalidate();
	return TRUE;
}

BOOL CExtMenuControlBar::_InstallMdiDocButtons(
	BOOL bDoRecalcLayout // = TRUE
	)
{
	ASSERT( m_bMdiApp );
BOOL bMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bMax );
int nCountOfButtons = GetButtonsCount();
	if( nCountOfButtons == 0
		&& hWndActiveChild == NULL
		)
		return FALSE;
CExtBarMdiDocButton * pDocTBB =
		DYNAMIC_DOWNCAST(
			CExtBarMdiDocButton,
			_GetButtonPtr(0)
			);
int nRightIndex = nCountOfButtons - 1;
CExtBarMdiRightButton * pRightTBB =
		DYNAMIC_DOWNCAST(
			CExtBarMdiRightButton,
			_GetButtonPtr(nRightIndex)
			);
	if( pRightTBB == NULL
		&& nCountOfButtons > 1
		)
	{
		nRightIndex--;
		pRightTBB =
			DYNAMIC_DOWNCAST(
				CExtBarMdiRightButton,
				_GetButtonPtr(nRightIndex)
				);
	}
BOOL bRetVal = FALSE;
	if( hWndActiveChild == NULL || (!bMax) )
	{ // if no active MDI doc or not maximized
		if( pRightTBB != NULL )
		{
			ASSERT_VALID( pRightTBB );
			VERIFY( RemoveButton(nRightIndex,FALSE) );
			bRetVal = TRUE;
		}
		if( pDocTBB != NULL )
		{
			ASSERT_VALID( pDocTBB );
			VERIFY( RemoveButton(0,FALSE) );
			bRetVal = TRUE;
		}
	} // if no active MDI doc or not maximized
	else
	{ // if acdive MDI doc is maximized
		ASSERT( hWndActiveChild != NULL );
		ASSERT( ::IsWindow(hWndActiveChild) );
		if( m_nMdiDocButtonCmdID == 0 )
		{
			CExtCmdManager::cmd_t * p_cmd =
				g_CmdManager->CmdAllocPtr(
					g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() )
					);
			ASSERT( p_cmd != NULL );
			if( p_cmd == NULL )
				return FALSE;
			m_nMdiDocButtonCmdID = p_cmd->m_nCmdID;
			ASSERT(
				CExtCmdManager::IsCommand(
					m_nMdiDocButtonCmdID
					)
				);
		} // if( m_nMdiDocButtonCmdID == 0 )
		if( pRightTBB == NULL )
		{
			pRightTBB =
				new CExtBarMdiRightButton(
					this,
					m_nMdiDocButtonCmdID
					);
			ASSERT_VALID( pRightTBB );
			m_buttons.InsertAt(
				nRightIndex + 1,
				pRightTBB
				);
			bRetVal = TRUE;
		} // if( pRightTBB == NULL )
		if( pDocTBB == NULL )
		{
			pDocTBB =
				new CExtBarMdiDocButton(
					this,
					m_nMdiDocButtonCmdID
					);
			ASSERT_VALID( pDocTBB );
			pDocTBB->SetMenu(
				(HMENU)::GetSystemMenu(hWndActiveChild,FALSE),
				FALSE,
				FALSE
				);
			m_buttons.InsertAt(
				0,
				pDocTBB
				);
			bRetVal = TRUE;
		} // if( pDocTBB == NULL )
	} // if acdive MDI doc is maximized

	if( bDoRecalcLayout )
	{
		_RecalcLayoutImpl();
		bRetVal = FALSE;
	}
	return bRetVal;
}

void CExtMenuControlBar::OnDestroy() 
{
	RemoveAllWndHooks();
	CExtToolControlBar::OnDestroy();
}

BOOL CExtMenuControlBar::TranslateMainFrameMessage(MSG* pMsg) 
{
	if( GetSafeHwnd() == NULL
		|| (!::IsWindow(GetSafeHwnd()))
		)
		return  FALSE;
	ASSERT_VALID(this);
	ASSERT(pMsg);

UINT nMsg = pMsg->message;

	if(	   nMsg == WM_SYSKEYDOWN
		|| nMsg == WM_SYSKEYUP
		|| nMsg == WM_KEYDOWN
		//|| nMsg == WM_KEYUP
		)
	{
		BOOL bKeyUp =
			(	// nMsg == WM_KEYUP ||
				nMsg == WM_SYSKEYUP) ?
					TRUE : FALSE;
		BOOL bCtrl = GetKeyState(VK_CONTROL) & 0x80000000;
		BOOL bShift = GetKeyState(VK_SHIFT) & 0x80000000;
		BOOL bAlt = HIWORD(pMsg->lParam) & KF_ALTDOWN;
		TCHAR vkTCHAR = pMsg->wParam;
		
		if(		(	vkTCHAR == VK_MENU
//				||
//				( vkTCHAR == VK_F10 && !(bShift || bCtrl || bAlt) )
				)
			&&	( !bCtrl && !bShift
				)
			)
		{ // if only VK_ALT was pressed
			if( !bKeyUp )
			{
				if( g_bMenuTracking )
					_CloseTrackingMenus();
				return TRUE;
			}
			if( g_bMenuTracking )
			{
				_CloseTrackingMenus();
				return TRUE;
			}
			if( GetButtonsCount() == 0 )
				return TRUE;
			CExtToolControlBar::g_bMenuTrackingExpanded = false;
			TrackButtonMenu( 0 );
			return TRUE;
		} // if only VK_ALT was pressed

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
				if( pWnd->GetSafeHwnd() != _GetDockingFrameImpl()->GetSafeHwnd()
					&& (pWnd->GetStyle() & WS_POPUP) != 0
					)
					return TRUE;

				// track frame system menu
				CExtToolControlBar::g_bMenuTrackingExpanded = false;
				CExtToolControlBar::g_bMenuTrackingExpanded = false;
				TrackMainFrameSystemMenu(NULL,TRUE);
			}
			return TRUE;
		} // if( bAlt && vkTCHAR == VK_SPACE )

		if( bAlt /*&& _istalnum(vkTCHAR)*/ )
		{ // if VK_ALT + VK_... pressed
			WORD nMapped;
			BYTE lpKeyState[256];
			::GetKeyboardState( lpKeyState );

			::ToAsciiEx(
				vkTCHAR,
				::MapVirtualKey( vkTCHAR, 0 ),
				lpKeyState,
				&nMapped,
				1,
				::GetKeyboardLayout(
					(::AfxGetThread())->m_nThreadID
					)
				);
			TCHAR szChar[2] = { (TCHAR)nMapped, '\0'};
			::CharUpper( szChar );

			int nBtnIdx = -1;
			if( szChar[0] != 0 ) // if( nMapped != 0 )
				nBtnIdx =
					GetButtonByAccessKey(
						szChar[0] // nMapped // vkTCHAR
						);
			if( nBtnIdx < 0 )
			{
				//MessageBeep(0);
				//return TRUE;
				return FALSE;
			}
			CExtToolControlBar::g_bMenuTrackingExpanded = false;
			CExtToolControlBar::TrackButtonMenu( nBtnIdx );
			return TRUE;
		} // if VK_ALT + VK_... pressed

		if( m_bFlatTracking )
		{
			if( bKeyUp )
				return TRUE;
			bool bUpdateIndex = false;
			switch( vkTCHAR )
			{
			case VK_LEFT:
				bUpdateIndex = true;
				//m_nFlatTrackingIndex--;
				m_nFlatTrackingIndex =
					GetVisibleButton(
						m_nFlatTrackingIndex,
						FALSE
						);
			break;
			case VK_RIGHT:
				bUpdateIndex = true;
				//m_nFlatTrackingIndex++;
				m_nFlatTrackingIndex =
					GetVisibleButton(
						m_nFlatTrackingIndex,
						TRUE
						);
			break;
			case VK_RETURN:
			case VK_UP:
			case VK_DOWN:
			{
				if( CExtPopupMenuWnd::IsMenuTracking()
					|| m_nFlatTrackingIndex < 0
					)
					return TRUE;
				ASSERT( m_nFlatTrackingIndex < GetButtonsCount() );
				CExtBarButton * pTBB =
					_GetButtonPtr(m_nFlatTrackingIndex);
				ASSERT_VALID( pTBB );
				if( pTBB->IsAbleToTrackMenu() )
				{
					int nIndex = m_nFlatTrackingIndex;
					m_bFlatTracking = FALSE;
					_UpdateFlatTracking( FALSE );
					CExtToolControlBar::TrackButtonMenu(
						nIndex
						);
				}
				else
				{
					if( vkTCHAR == VK_RETURN
						&& CExtCmdManager::IsCommand(pTBB->GetCmdID())
						)
					{
						pTBB->OnClick(
							CPoint(0,0),
							false
							);
					}
				}
				return TRUE;
			}
			break;
			case VK_ESCAPE:
//				if( g_bMenuTracking
//					//&& !m_bFlatTracking
//					)
//				//if( CExtPopupMenuWnd::IsMenuTracking() )
//				{
//					m_nFlatTrackingIndex = GetMenuTrackingButton();
//					_CloseTrackingMenus();
//					if( m_nFlatTrackingIndex >= 0 )
//						m_bFlatTracking = TRUE;
//					else
//						m_bFlatTracking = FALSE;
//				}
//				else
					m_bFlatTracking = FALSE;
				_UpdateFlatTracking();
				return TRUE;
			} // switch( vkTCHAR )
			if( bUpdateIndex )
			{
				int nCountOfButtons = GetButtonsCount();
				ASSERT( nCountOfButtons > 0 );
				if( m_nFlatTrackingIndex < 0 )
					m_nFlatTrackingIndex = nCountOfButtons - 1;
				else if(m_nFlatTrackingIndex >= nCountOfButtons)
					m_nFlatTrackingIndex = 0;
			}
			_UpdateFlatTracking();
			return TRUE;
		} // if( m_bFlatTracking )
	}

	if( nMsg == WM_CONTEXTMENU )
	{
		if( g_bMenuTracking || m_bFlatTracking )
			return TRUE;
	}
	
	return FALSE;
}

CRect CExtMenuControlBar::_GetChildFrameSysIconRect()
{
CRect rc;
	rc.SetRectEmpty();
BOOL bMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bMax );
	if( hWndActiveChild == NULL )
		return rc;
	ASSERT( ::IsWindow(hWndActiveChild) );
	if( bMax )
		return rc;
CWnd * pWnd = CWnd::FromHandlePermanent(hWndActiveChild);
	if( pWnd == NULL )
	{
		ASSERT( FALSE );
		return rc;
	}
	ASSERT_VALID( pWnd );
	ASSERT( pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) );

CFrameWnd * pFrame = 
		DYNAMIC_DOWNCAST(
			CFrameWnd,
			pWnd
			);
	ASSERT( pFrame != NULL );
	if( pFrame == NULL )
	{
		ASSERT( FALSE );
		return rc;
	}
	ASSERT_VALID( pFrame );
		
CRect rcWnd,rcClient,rcHelper;
	pFrame->GetWindowRect( &rcWnd );
	pFrame->GetClientRect( &rcClient );
	rcHelper = rcWnd;
	pFrame->ScreenToClient( &rcHelper );
	ASSERT( rcHelper.top <= rcClient.top );
	int yDiff = rcClient.top - rcHelper.top;
	rc.SetRect(
		rcWnd.left + GetSystemMetrics(SM_CYFRAME),
		rcWnd.top + GetSystemMetrics(SM_CXFRAME),
		rcWnd.left + yDiff,
		rcWnd.top + yDiff
		);
	return rc;
}

CRect CExtMenuControlBar::_GetMainFrameSysIconRect()
{
CRect rc;
CRect rcWnd,rcClient,rcHelper;
CFrameWnd * pFrame =
		_GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	pFrame->GetWindowRect( &rcWnd );
	pFrame->GetClientRect( &rcClient );
	rcHelper = rcWnd;
	pFrame->ScreenToClient( &rcHelper );
	ASSERT( rcHelper.top <= rcClient.top );
	int yDiff = rcClient.top - rcHelper.top;
	rc.SetRect(
		rcWnd.left + GetSystemMetrics(SM_CYFRAME),
		rcWnd.top + GetSystemMetrics(SM_CXFRAME),
		rcWnd.left + yDiff,
		rcWnd.top + yDiff
		);
	return rc;
}

BOOL CExtMenuControlBar::_TrackFrameSystemMenu(
	CFrameWnd * pFrame,
	CPoint * pPoint, // = NULL, // NULL means calc meny track area automatically
	BOOL bSelectAny, // = FALSE
	LPCRECT rcExcludeArea, // = NULL
	UINT nTrackFlags, // = (UINT)(-1)
	BOOL bCombinedMode // = FALSE
	)
{
	if( nTrackFlags == (UINT)(-1) )
		nTrackFlags = TPMX_BOTTOMALIGN;
	ASSERT_VALID( pFrame );
//	pFrame->SendMessage( WM_CANCELMODE );
//	CExtPopupMenuWnd::_PassMsgLoop();

	if( g_bMenuTracking )
		_CloseTrackingMenus();
	if(	CExtPopupMenuWnd::IsMenuTracking() )
		CExtPopupMenuWnd::CancelMenuTracking();

CMenu * pSysMenu =
		pFrame->GetSystemMenu( FALSE );
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

//CRect rcExclude;
CPoint point;
	if( pPoint != NULL)
	{
//		rcExclude.SetRectEmpty();
		point = *pPoint;
	}
	else
	{
//		VERIFY( ::GetCursorPos(&point) );
		CRect rcWnd,rcClient,rcHelper;
		pFrame->GetWindowRect( &rcWnd );
		pFrame->GetClientRect( &rcClient );
		rcHelper = rcWnd;
		pFrame->ScreenToClient( &rcHelper );
		ASSERT( rcHelper.top <= rcClient.top );
		int yDiff = rcClient.top - rcHelper.top;
/*
		CRect rcExclude(
			rcWnd.left,
			rcWnd.top,
			rcWnd.left+yDiff,
			rcWnd.top+yDiff
			);
		point = rcExclude.CenterPoint();
*/
		point = rcWnd.TopLeft();
		point.x += GetSystemMetrics(SM_CXFRAME);
		point.y += yDiff;
	}
	pPopup->TrackPopupMenu(
		nTrackFlags,
		point.x,point.y,
		pFrame->GetSafeHwnd(),
//		(rcExclude.IsRectEmpty()) ?
//			((LPRECT)NULL) : &rcExclude
		rcExcludeArea,
		bSelectAny ? TRUE : FALSE,
		false,
		CExtToolControlBar::g_bMenuTrackingExpanded,
		CExtPopupMenuWnd::__COMBINE_DEFAULT,
		bCombinedMode ?
			this : NULL,
		bCombinedMode ?
			CExtToolControlBar::_CbPaintCombinedContent : NULL
		);

	g_bMenuTracking = true;
	return TRUE;
}

BOOL CExtMenuControlBar::TrackChildFrameSystemMenu(
	CPoint * pPoint, // = NULL, // NULL means calc meny track area automatically
	BOOL bSelectAny // = FALSE
	)
{
	if( !m_bMdiApp )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	if( GetButtonsCount() > 0 )
	{
		CExtBarButton * pTBB =
			_GetButtonPtr(0);
		ASSERT_VALID( pTBB );
		if( pTBB->IsKindOf(RUNTIME_CLASS(CExtBarMdiDocButton)) )
		{
			if( !TrackButtonMenu(0) )
			{
				ASSERT( FALSE );
				return FALSE;
			}
			return TRUE;
		}
	}

BOOL bMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bMax );
	if( hWndActiveChild == NULL )
	{
		ASSERT( FALSE );
		return FALSE;
	};
	ASSERT( ::IsWindow(hWndActiveChild) );
CWnd * pWnd = CWnd::FromHandlePermanent(hWndActiveChild);
	if( pWnd == NULL )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT_VALID( pWnd );
	ASSERT( pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) );

CFrameWnd * pFrame = 
		DYNAMIC_DOWNCAST(
			CFrameWnd,
			pWnd
			);
	ASSERT( pFrame != NULL );
	if( pFrame == NULL )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT_VALID( pFrame );

	m_bSysMenuTracking =
		_TrackFrameSystemMenu(
			pFrame,
			pPoint,
			bSelectAny
			);
	if( m_bSysMenuTracking )
		SetTimer( ID_TIMER_SYS_TRACKING, 20, NULL );
	return m_bSysMenuTracking;
}

BOOL CExtMenuControlBar::TrackMainFrameSystemMenu(
	CPoint * pPoint, // = NULL // NULL means calc meny track area automatically
	BOOL bSelectAny // = FALSE
	)
{
CFrameWnd * pFrame =
		_GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	m_bSysMenuTracking =
		_TrackFrameSystemMenu(
			pFrame,
			pPoint,
			bSelectAny
			);
	if( m_bSysMenuTracking )
		SetTimer( ID_TIMER_SYS_TRACKING, 20, NULL );
	return m_bSysMenuTracking;
}

BOOL CExtMenuControlBar::TrackButtonMenu(
	int nIndex
	)
{
	if( !SafeDisplayBar() )
		return false;

	VERIFY(
		PostMessage(
			nMsgTrackButtonMenu,
			(WPARAM)nIndex
			)
		);
	return TRUE;
//	return CExtToolControlBar::TrackButtonMenu(nIndex);
}

BOOL CExtMenuControlBar::_InvokeParentTrackButtonMenu(
	int nIndex
	)
{
	return CExtToolControlBar::TrackButtonMenu(nIndex);
}

LRESULT CExtMenuControlBar::OnSetMenuNULL(WPARAM wParam,LPARAM lParam)
{
HWND hWnd = (HWND)wParam;
	ASSERT( hWnd != NULL );
	ASSERT( ::IsWindow(hWnd) );
	VERIFY( ::SetMenu( hWnd, NULL ) );
	return 0;
}

LRESULT CExtMenuControlBar::OnTrackButtonMenu(WPARAM wParam,LPARAM lParam)
{
	if( !(::IsWindowEnabled(
			_GetDockingFrameImpl()->GetSafeHwnd()
			))
		) 
		return 0;
int nCountOfButtons = GetButtonsCount();
	ASSERT( nCountOfButtons >= 0 );
	if( nCountOfButtons == 0 )
		return 0;
int nIndex = int(wParam);
	if( nIndex < 0 || nIndex>=nCountOfButtons )
	{
		_CloseTrackingMenus();
		m_bFlatTracking = FALSE;
		return 0;
	}
CExtBarButton * pTBB = _GetButtonPtr(nIndex);
	ASSERT_VALID( pTBB );
BOOL bDroppedMode =
		CExtPopupMenuWnd::IsMenuTracking();
	if( !pTBB->IsAbleToTrackMenu() )
		bDroppedMode = FALSE;
	if( bDroppedMode )
	{
		CExtToolControlBar::TrackButtonMenu(nIndex);
		return 0;
	}
	_CloseTrackingMenus();

	m_nFlatTrackingIndex = nIndex;
	m_bFlatTracking = TRUE;
	_UpdateFlatTracking();
	SetTimer(ID_TIMER_FLAT_TRACKING,20,NULL);

	return 0;
}

void CExtMenuControlBar::_UpdateFlatTracking(
	BOOL bRepaint // = TRUE
	)
{
	if( m_nFlatTrackingIndex < 0
		&& m_nOldTrackingIndex < 0
		)
	{
		m_bFlatTracking = FALSE;
		KillTimer( ID_TIMER_FLAT_TRACKING );
		if( g_bMenuTracking )
			_CloseTrackingMenus();
		return;
	}
int nCountOfButtons = GetButtonsCount();
	if( !m_bFlatTracking )
	{
		KillTimer( ID_TIMER_FLAT_TRACKING );
		if( m_nFlatTrackingIndex >= 0 )
		{
			ASSERT( m_nFlatTrackingIndex < nCountOfButtons );
			CExtBarButton * pTBB =
				_GetButtonPtr(m_nFlatTrackingIndex);
			ASSERT_VALID( pTBB );
			pTBB->ModifyStyle(0,TBBS_PRESSED);
			if( bRepaint )
				_InvalidateButton(m_nFlatTrackingIndex);
			m_nFlatTrackingIndex = m_nOldTrackingIndex = -1;
		}
		if( g_bMenuTracking )
			_CloseTrackingMenus();
		return;
	} // if( !m_bFlatTracking )
	if( m_nFlatTrackingIndex != m_nOldTrackingIndex )
	{
		if( m_nOldTrackingIndex >= 0 )
		{
			ASSERT( m_nOldTrackingIndex < nCountOfButtons );
			CExtBarButton * pTBB =
				_GetButtonPtr(m_nOldTrackingIndex);
			ASSERT_VALID( pTBB );
			pTBB->ModifyStyle(0,TBBS_PRESSED|TBBS_CHECKED);
			if( bRepaint )
				_InvalidateButton(m_nOldTrackingIndex);
		}
		if( m_nFlatTrackingIndex >= 0 )
		{
			ASSERT( m_nFlatTrackingIndex < nCountOfButtons );
			CExtBarButton * pTBB =
				_GetButtonPtr(m_nFlatTrackingIndex);
			ASSERT_VALID( pTBB );
			pTBB->ModifyStyle(TBBS_PRESSED);
			if( bRepaint )
				_InvalidateButton(m_nFlatTrackingIndex);
		}
		m_nOldTrackingIndex = m_nFlatTrackingIndex;
	} // if( m_nFlatTrackingIndex != m_nOldTrackingIndex )
}

void CExtMenuControlBar::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == ID_TIMER_FLAT_TRACKING )
	{
		_UpdateFlatTracking();
		return;
	}
	
	if( nIDEvent == ID_TIMER_SYS_TRACKING )
	{
		if( !CExtPopupMenuWnd::IsMenuTracking() )
		{
			m_bSysMenuTracking = FALSE;
			KillTimer( ID_TIMER_SYS_TRACKING );
			_CloseTrackingMenus();
		}
		return;
	}
	
	CExtToolControlBar::OnTimer(nIDEvent);
}

void CExtMenuControlBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( !m_bFlatTracking )
	{
		CExtToolControlBar::OnMouseMove(nFlags, point);
		return;
	}
int nIndex = HitTest(point);
	if( nIndex == m_nFlatTrackingIndex
		|| nIndex < 0
		)
		return;
	//m_nFlatTrackingIndex = nIndex;
	//_UpdateFlatTracking();
	TrackButtonMenu( nIndex );
}

void CExtMenuControlBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bFlatTracking )
	{
		int nIndex = HitTest(point);
		if( nIndex >= 0 )
		{
			int nMenuIndex = GetMenuTrackingButton();
			if( nIndex == nMenuIndex )
				return;
		}
	}

	CExtToolControlBar::OnLButtonDown(nFlags, point);
}

void CExtMenuControlBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
/*
	if( !m_bFlatTracking )
	{
		CExtToolControlBar::OnLButtonUp(nFlags, point);
		return;
	}
	m_bFlatTracking = false;
	_UpdateFlatTracking();
	_CloseTrackingMenus();
*/
BOOL bRepaint = FALSE;
	if( m_bFlatTracking )
	{
		m_bFlatTracking = false;
		_UpdateFlatTracking( FALSE );
		_CloseTrackingMenus();
		bRepaint = TRUE;
	}
	CExtToolControlBar::OnLButtonUp(nFlags, point);
	if( bRepaint )
		Invalidate();
}

#define __MENUBAR_RIGHT_ADJUST_GAP 1

bool CExtMenuControlBar::g_bHideMenuBarExpandContentButton = false;

void CExtMenuControlBar::_RecalcPositionsImpl()
{
	CExtToolControlBar::_RecalcPositionsImpl();

	if( g_bHideMenuBarExpandContentButton
		&& m_pRightBtn != NULL
		)
	{
		ASSERT_VALID( m_pRightBtn );
		if( m_pRightBtn->GetButtons().GetSize() == 0 )
			m_pRightBtn->Show( FALSE );
	}

	if( IsFloating() )
		return;
int nCountOfButtons = GetButtonsCount();
	if( nCountOfButtons <= 1 ) /// !?!?!?!
		return;
int nRightIndex = nCountOfButtons - 1;
CExtBarMdiRightButton * pRightTBB =
		DYNAMIC_DOWNCAST(
			CExtBarMdiRightButton,
			_GetButtonPtr(nRightIndex)
			);
	if( pRightTBB == NULL
		&& nCountOfButtons > 1
		)
	{
		nRightIndex--;
		pRightTBB =
			DYNAMIC_DOWNCAST(
				CExtBarMdiRightButton,
				_GetButtonPtr(nRightIndex)
				);
	}
	if( pRightTBB == NULL )
		return;
	if( !pRightTBB->IsVisible() )
		return;
	ASSERT( nRightIndex >= 1 );

#ifdef _DEBUG
CExtBarButton * pPrevTBB =
		_GetButtonPtr( nRightIndex - 1 );
	ASSERT_VALID( pPrevTBB );
	ASSERT( pPrevTBB->IsVisible() );
#endif // _DEBUG

CExtBarButton * pExpandTBB = NULL;
CRect rcBtnExpand(0,0,0,0);
int nAlignMargin = 0;
BOOL bHorz = IsDockedHorizontally();
CRect rcClient;
	GetClientRect( &rcClient );
	
	if( nRightIndex < (nCountOfButtons-1) )
	{ // if content-expand button exists
		pExpandTBB =
			_GetButtonPtr( nRightIndex + 1 );
		ASSERT_VALID( pExpandTBB );
//		ASSERT( pExpandTBB->IsVisible() );
		ASSERT( pExpandTBB->IsKindOf( RUNTIME_CLASS(CExtBarContentExpandButton)) );
		rcBtnExpand = pExpandTBB->Rect();
		nAlignMargin =
			bHorz ? rcBtnExpand.left : rcBtnExpand.top;
		nAlignMargin -= __MENUBAR_RIGHT_ADJUST_GAP;
	} // if content-expand button exists
	else
	{  // if content-expand button DOES NOT exists
		nAlignMargin =
			bHorz ? rcClient.right : rcClient.bottom;
		nAlignMargin -= __MENUBAR_RIGHT_ADJUST_GAP;
	}  // if content-expand button DOES NOT exists
CRect rcBtnRight = pRightTBB->Rect();
	if( bHorz && rcBtnRight.right >= nAlignMargin )
		return;
	if( (!bHorz) && rcBtnRight.bottom >= nAlignMargin )
		return;
//CRect rcBtnPrev = pPrevTBB->Rect();

	if( bHorz )
	{
		rcBtnRight.OffsetRect(
			nAlignMargin - rcBtnRight.right,
			0
			);
	} // if( bHorz )
	else
	{
		rcBtnRight.OffsetRect(
			0,
			nAlignMargin - rcBtnRight.bottom
			);
	} // else from if( bHorz )

//pExpandTBB = NULL;
	if( pExpandTBB == NULL )
	{
		pRightTBB->SetRect(rcBtnRight);
		return;
	}

	ASSERT( !rcBtnExpand.IsRectEmpty() );
CRect rcBtnExpand2(
		bHorz ? rcBtnRight.left : rcBtnExpand.left,
		bHorz ? rcBtnExpand.top : rcBtnRight.top,
		bHorz ?
			rcBtnRight.left + rcBtnExpand.Width()
			:
			rcBtnExpand.left + rcBtnExpand.Width()
			,
		bHorz ?
			rcBtnExpand.top + rcBtnExpand.Height()
			:
			rcBtnRight.top + rcBtnExpand.Height()
		);
	rcBtnRight.OffsetRect(
		bHorz ? rcBtnExpand.right - rcBtnRight.right : 0,
		bHorz ? 0 : rcBtnExpand.bottom - rcBtnRight.bottom
		);
	pRightTBB->SetRect(rcBtnRight);
	pExpandTBB->SetRect(rcBtnExpand2);

	if( g_bHideMenuBarExpandContentButton )
		pExpandTBB->Show( FALSE );
}

void CExtMenuControlBar::_OnStartDragging()
{
	if( m_bFlatTracking )
	{
		m_bFlatTracking = FALSE;
		_UpdateFlatTracking();
	}
	if( g_bMenuTracking )
		_CloseTrackingMenus();
	CExtToolControlBar::_OnStartDragging();
}

HWND CExtMenuControlBar::_GetHwndMainFrame()
{
CFrameWnd * pFrame = _GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	ASSERT( ::IsWindow(pFrame->GetSafeHwnd()) );
	return pFrame->GetSafeHwnd();
}

HWND CExtMenuControlBar::_GetHwndChildFrame()
{
	return m_hWndHelperActiveChild;
}

HWND CExtMenuControlBar::_GetHwndMdiArea()
{
	ASSERT( m_bMdiApp );

CFrameWnd * pFrame = _GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	ASSERT( ::IsWindow(pFrame->GetSafeHwnd()) );

	ASSERT_KINDOF( CMDIFrameWnd, pFrame );

HWND hWndMdiArea = ((CMDIFrameWnd*)pFrame)->m_hWndMDIClient;
	ASSERT( hWndMdiArea != NULL );
	ASSERT( ::IsWindow(hWndMdiArea) );
	
	return hWndMdiArea;
}

int CExtMenuControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CExtToolControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
CFrameWnd * pFrame = _GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
	ASSERT( ::IsWindow(pFrame->GetSafeHwnd()) );

	if(	!SetupHookWndSink(_GetHwndMainFrame()) )
	{
		ASSERT( FALSE );
		return FALSE;
	}

	if( pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)) )
	{
		m_bMdiApp = TRUE;
		if(	!SetupHookWndSink(_GetHwndMdiArea()) )
		{
			ASSERT( FALSE );
			return FALSE;
		}
			
		::SetWindowPos(
			_GetHwndMdiArea(),
			NULL,
			0,0,0,0,
			SWP_FRAMECHANGED
				|SWP_NOOWNERZORDER|SWP_NOCOPYBITS
				|SWP_NOREPOSITION
				|SWP_NOZORDER|SWP_NOACTIVATE
				|SWP_NOMOVE|SWP_NOSIZE
			);

		VERIFY( _SyncActiveMdiChild() );

	} // if( pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)) )

	ASSERT_VALID(m_pDockSite);
	if( m_pDockSite->GetMenu() )
	{
		PostMessage(
			nMsgSetMenuNULL,
			(WPARAM)m_pDockSite->GetSafeHwnd()
			);
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarMdiDocButton

IMPLEMENT_DYNAMIC(CExtBarMdiDocButton,CExtBarButton)

BOOL CExtBarMdiDocButton::PutToPopupMenu(
	CExtPopupMenuWnd * pPopup
	)
{
	ASSERT( pPopup != NULL );
	ASSERT( pPopup->GetSafeHwnd() == NULL );

	ASSERT_VALID( m_pBar );
	ASSERT( m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) );
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );

BOOL bDummyMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bDummyMax );
//	if( hWndActiveChild == NULL )
//		return;
	ASSERT( ::IsWindow( hWndActiveChild ) );

CFrameWnd * pFrame =
		DYNAMIC_DOWNCAST(
			CFrameWnd,
			CWnd::FromHandle(hWndActiveChild)
			);
CMenu * pSysMenu =
		pFrame->GetSystemMenu( FALSE );
	if( pSysMenu == NULL )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	ASSERT( ::IsMenu(pSysMenu->GetSafeHmenu()) );
HICON hIcon = GetHICON();
	if( hIcon != NULL )
	{
		CExtCmdManager::icon_t _icon(hIcon,true);
		hIcon = _icon.Detach();
	}
CExtLocalResourceHelper _LRH;
CString sActiveDocument;
	if( !sActiveDocument.LoadString(IDS_ACTIVE_DOCUMENT) )
	{
		ASSERT( FALSE );
		sActiveDocument = _T("Active document");
	}
	if( !pPopup->ItemInsert(
			CExtPopupMenuWnd::TYPE_POPUP,
			-1,
			sActiveDocument,
			hIcon
			)
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}
CExtPopupMenuWnd * pChildPopup =
		pPopup->ItemGetPopup(
			pPopup->ItemGetCount() - 1
			);
	ASSERT( pChildPopup != NULL );
	if(	!pChildPopup->UpdateFromMenu(
			m_pBar->GetSafeHwnd(),
			pSysMenu,
			false,
			false
			)
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}
	pChildPopup->
		SetCmdTargetToAllItems(
			hWndActiveChild
			);

	VERIFY(
		pPopup->ItemInsert(
			CExtPopupMenuWnd::TYPE_SEPARATOR,
			-1
			)
		);

	return TRUE;
}

HWND CExtBarMdiDocButton::_GetActiveMdiChildWnd(
	BOOL & bMaximized
	)
{
	bMaximized = FALSE;
CMDIFrameWnd * pFrame =
		DYNAMIC_DOWNCAST(
			CMDIFrameWnd,
			m_pBar->_GetDockingFrameImpl()
			);
	if( pFrame == NULL )
		return NULL;
HWND hWndMdiArea = pFrame->m_hWndMDIClient;
	ASSERT( hWndMdiArea != NULL );
	ASSERT( ::IsWindow(hWndMdiArea) );
BOOL bMax = FALSE;
HWND hWndActiveChild = (HWND)
		::SendMessage(
			hWndMdiArea,
			WM_MDIGETACTIVE,
			0,
			(LPARAM)&bMaximized
			);
	return hWndActiveChild;
}

HICON CExtBarMdiDocButton::GetHICON()
{
	ASSERT_VALID( m_pBar );
BOOL bMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bMax );
	if( hWndActiveChild == NULL )
		return NULL;
	ASSERT( ::IsWindow(hWndActiveChild) );

HICON hIcon = (HICON)
	hIcon = (HICON)
		::SendMessage(
			hWndActiveChild,
			CExtControlBar::g_nMsgQueryIcon,
			0,
			0
			);
	if( hIcon == NULL )
		hIcon = (HICON)
			::SendMessage(
				hWndActiveChild,
				WM_GETICON,
				ICON_SMALL,
				0
				);
	if( hIcon == NULL )
		hIcon = (HICON)
			::SendMessage(
				hWndActiveChild,
				WM_GETICON,
				ICON_BIG,
				0
				);
	if(hIcon == NULL)
		hIcon = (HICON)
			::GetClassLong(
				hWndActiveChild,
				GCL_HICONSM
				);

	return hIcon;
}

CWnd * CExtBarMdiDocButton::GetCmdTargetWnd()
{
	ASSERT_VALID( m_pBar );
BOOL bMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bMax );
	if( hWndActiveChild == NULL )
	{
//		ASSERT( FALSE );
//		return CExtBarButton::GetCmdTargetWnd();
		return NULL;
	};
	ASSERT( ::IsWindow(hWndActiveChild) );
CWnd * pWnd = CWnd::FromHandlePermanent(hWndActiveChild);
	if( pWnd == NULL )
	{
		ASSERT( FALSE );
		//return CExtBarButton::GetCmdTargetWnd();
		return NULL;
	}
	ASSERT_VALID( pWnd );
	ASSERT( pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) );
	return pWnd;
}

void CExtBarMdiDocButton::OnTrackPopup(
	CPoint point
	)
{
	ASSERT_VALID( m_pBar );

CExtMenuControlBar * pBar =
		DYNAMIC_DOWNCAST(
			CExtMenuControlBar,
			m_pBar
			);
CWnd * pWnd = CExtBarMdiDocButton::GetCmdTargetWnd();
CFrameWnd * pFrame = 
		DYNAMIC_DOWNCAST(
			CFrameWnd,
			pWnd
			);
	ASSERT( pFrame != NULL );
	if( pBar == NULL || pFrame == NULL )
	{
		CExtBarButton::OnTrackPopup(point);
		return;
	}

	if( CExtToolControlBar::g_bMenuTracking
		&& CExtPopupMenuWnd::IsMenuTracking()
		&& pBar->_GetIndexOf(this) ==
			pBar->m_nBtnIdxMenuTracking
		)
		return;

	CExtToolControlBar::_CloseTrackingMenus();

	if( pBar->IsFloating() )
	{
		pBar->ActivateTopParent();
		CFrameWnd * pFrame =
			pBar->GetDockingFrame();
		ASSERT_VALID( pFrame );
		pFrame->BringWindowToTop();
	}

UINT nTrackFlags = TPMX_TOPALIGN;
CRect rcBtn = Rect();
//	point.x = rcBtn.left;
//	point.y = rcBtn.bottom;
	switch( m_pBar->GetSafeDockBarDlgCtrlID() )
	{
	case AFX_IDW_DOCKBAR_BOTTOM:
		nTrackFlags = TPMX_BOTTOMALIGN;
//		point.x = rcBtn.left;
//		point.y = rcBtn.bottom;
	break;
	case AFX_IDW_DOCKBAR_LEFT:
		nTrackFlags = TPMX_LEFTALIGN;
//		point.x = rcBtn.right;
//		point.y = rcBtn.top;
	break;
	case AFX_IDW_DOCKBAR_RIGHT:
		nTrackFlags = TPMX_RIGHTALIGN;
//		point.x = rcBtn.left;
//		point.y = rcBtn.top;
	break;
	} // switch( m_pBar->GetSafeDockBarDlgCtrlID() )
	pBar->ClientToScreen( &rcBtn );
//	pBar->ClientToScreen( &point );
	point = rcBtn.CenterPoint();

	pBar->m_bSysMenuTracking =
		/*CExtMenuControlBar::*/
		pBar->
		_TrackFrameSystemMenu(
			pFrame,
			&point, // NULL,
			TRUE,
			&rcBtn,
			nTrackFlags,
			TRUE
			);
	if( pBar->m_bSysMenuTracking )
		pBar->SetTimer( ID_TIMER_SYS_TRACKING, 20, NULL );
	m_pBar->_SwitchMenuTrackingIndex(
		m_pBar->_GetIndexOf( this )
		);
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarMdiRightButton

IMPLEMENT_DYNAMIC(CExtBarMdiRightButton,CExtBarMdiDocButton)

BOOL CExtBarMdiRightButton::PutToPopupMenu(
	CExtPopupMenuWnd * pPopup
	)
{
	ASSERT( pPopup != NULL );
	ASSERT( pPopup->GetSafeHwnd() == NULL );

	ASSERT_VALID( m_pBar );
	ASSERT( m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) );
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );

CExtMenuControlBar * pBar =
		DYNAMIC_DOWNCAST(
			CExtMenuControlBar,
			m_pBar
			);
	ASSERT_VALID( pBar );
BOOL bHorz = !pBar->IsDockedVertically();
MdiMenuBarRightButtonsInfo_t _info;
	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz );
	if( !_info.m_bInitialized )
		return FALSE;

BOOL bDummyMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bDummyMax );
//	if( hWndActiveChild == NULL )
//		return;
	ASSERT( ::IsWindow( hWndActiveChild ) );

int nCount = pPopup->ItemGetCount();
	if( nCount > 0 )
	{
		if( pPopup->ItemGetCmdID(nCount-1) !=
				CExtPopupMenuWnd::TYPE_SEPARATOR
				)
		{
			VERIFY(
				pPopup->ItemInsert(
					CExtPopupMenuWnd::TYPE_SEPARATOR,
					-1
					)
				);
		}
	}

	if( _info.m_bBtnHelp )
	{
		VERIFY(
			pPopup->ItemInsert(
				SC_CONTEXTHELP,
				-1,NULL,NULL,hWndActiveChild
				)
			);
	}
	if( _info.m_bBtnMinimize )
	{
		VERIFY(
			pPopup->ItemInsert(
				SC_MINIMIZE,
				-1,NULL,NULL,hWndActiveChild
				)
			);
		VERIFY(
			pPopup->ItemInsert(
				SC_RESTORE,
				-1,NULL,NULL,hWndActiveChild
				)
			);
	}
	if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize )
	{
		VERIFY(
			pPopup->ItemInsert(
				SC_MAXIMIZE,
				-1,NULL,NULL,hWndActiveChild
				)
			);
	}
	if( _info.m_bBtnClose )
	{
		VERIFY(
			pPopup->ItemInsert(
				SC_CLOSE,
				-1,NULL,NULL,hWndActiveChild
				)
			);
	}

	return TRUE;
}

HICON CExtBarMdiRightButton::GetHICON()
{
	return NULL;
}

void CExtMenuControlBar::_GetMdiMenuBarRightButtonsInfo(
	CExtBarMdiRightButton::MdiMenuBarRightButtonsInfo_t & _info,
	CExtBarMdiRightButton * pTBB,
	BOOL bHorz
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pTBB );
	_info.m_pWndMdiChildFrame =
		pTBB->GetCmdTargetWnd();
	if( _info.m_pWndMdiChildFrame == NULL )
		return;
	ASSERT( _info.m_pWndMdiChildFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) );
	ASSERT_VALID( _info.m_pWndMdiChildFrame );
	_info.m_pBar = this;
DWORD dwFrameStyle = _info.m_pWndMdiChildFrame->GetStyle();
	_info.m_bBtnClose = true;
	_info.m_bBtnMaximize =
		(dwFrameStyle & WS_MAXIMIZEBOX) ?
			true : false;
	_info.m_bBtnMinimize =
		(dwFrameStyle & WS_MINIMIZEBOX) ?
			true : false;
	_info.m_bBtnHelp =
		(_info.m_bBtnMaximize && (_info.m_pWndMdiChildFrame->GetExStyle()&WS_EX_CONTEXTHELP)) ?
			true : false;
	if( bHorz )
	{
		_info.m_calcSize.cx = __GAP_BETWEEN_WND_RIGHT_BUTTONS;
		_info.m_calcSize.cy = _info.m_cyIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
		if( _info.m_bBtnMinimize )
			_info.m_calcSize.cx +=
				(_info.m_cxIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS)*2;
		if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize )
			_info.m_calcSize.cx +=
				_info.m_cxIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
		if( _info.m_bBtnHelp )
			_info.m_calcSize.cx +=
				_info.m_cxIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
		if( _info.m_bBtnClose )
			_info.m_calcSize.cx +=
				_info.m_cxIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
	} // if( bHorz )
	else
	{
		_info.m_calcSize.cx = _info.m_cxIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
		_info.m_calcSize.cy = __GAP_BETWEEN_WND_RIGHT_BUTTONS;
		if( _info.m_bBtnMinimize )
			_info.m_calcSize.cy +=
				(_info.m_cyIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS)*2;
		if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize )
			_info.m_calcSize.cy +=
				_info.m_cyIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
		if( _info.m_bBtnHelp )
			_info.m_calcSize.cy +=
				_info.m_cyIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
		if( _info.m_bBtnClose )
			_info.m_calcSize.cy +=
				_info.m_cyIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
	} // else from if( bHorz )

	_info.m_bInitialized = true;
}

void CExtBarMdiRightButton::MdiMenuBarRightButtonsInfo_t::
	InitLocations(
		const CRect & rcBtn,
		BOOL bHorz
		)
{
	ASSERT( m_bInitialized );
	EmptyLocations();
	if( rcBtn.IsRectEmpty() )
		return;
CRect rcStart(0,0,0,0);
CSize sizeOffset(0,0);
	if( bHorz )
	{
		int nOffset = (rcBtn.Height() - m_cyIcon) / 2;
		rcStart.SetRect(
			rcBtn.left + __GAP_BETWEEN_WND_RIGHT_BUTTONS,
			rcBtn.top + nOffset,
			rcBtn.left + __GAP_BETWEEN_WND_RIGHT_BUTTONS
				+ m_cxIcon,
			rcBtn.top + nOffset + m_cyIcon
			);
		sizeOffset.cx = m_cxIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
	} // if( bHorz )
	else
	{
		int nOffset = (rcBtn.Width() - m_cxIcon) / 2;
		rcStart.SetRect(
			rcBtn.left + nOffset,
			rcBtn.top + __GAP_BETWEEN_WND_RIGHT_BUTTONS,
			rcBtn.left + nOffset + m_cxIcon,
			rcBtn.top + __GAP_BETWEEN_WND_RIGHT_BUTTONS
				+ m_cxIcon
			);
		sizeOffset.cy = m_cyIcon+__GAP_BETWEEN_WND_RIGHT_BUTTONS;
	} // else from if( bHorz )
	if( m_bBtnHelp )
	{
		m_rcBtnHelp = rcStart;
		rcStart.OffsetRect(sizeOffset);
	}
	if( m_bBtnMinimize )
	{
		m_rcBtnMinimize = rcStart;
		rcStart.OffsetRect(sizeOffset);
		m_rcBtnRestore = rcStart;
		rcStart.OffsetRect(sizeOffset);
	}
	if( m_bBtnMaximize && !m_bBtnMinimize )
	{
		m_rcBtnMaximize = rcStart;
		rcStart.OffsetRect(sizeOffset);
	}
	if( m_bBtnClose )
		m_rcBtnClose = rcStart;
}

CSize CExtBarMdiRightButton::CalculateLayout(
	CDC & dc,
	CSize sizePreCalc,
	BOOL bHorz
	)
{
	ASSERT_VALID( m_pBar );
	ASSERT( m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) );
	ASSERT_VALID( (&dc) );
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );

CExtMenuControlBar * pBar =
		DYNAMIC_DOWNCAST(
			CExtMenuControlBar,
			m_pBar
			);
	ASSERT_VALID( pBar );
MdiMenuBarRightButtonsInfo_t _info;
	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz );

	//ASSERT( _info.m_bInitialized );
	if( !_info.m_bInitialized )
		return CSize(1,1);

CSize _size( _info.m_calcSize );
	if( bHorz )
	{
		if( _size.cy < sizePreCalc.cy )
			_size.cy = sizePreCalc.cy;
	}
	else
	{
		if( _size.cx < sizePreCalc.cx )
			_size.cx = sizePreCalc.cx;
	}
	return _size;
}

void CExtBarMdiRightButton::Paint(
	CDC & dc,
	bool bHorz
	)
{
	ASSERT_VALID( m_pBar );
	ASSERT( m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) );
	ASSERT_VALID( (&dc) );
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );

CExtMenuControlBar * pBar =
		DYNAMIC_DOWNCAST(
			CExtMenuControlBar,
			m_pBar
			);
	ASSERT_VALID( pBar );
MdiMenuBarRightButtonsInfo_t _info;
	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz );
	if( !_info.m_bInitialized )
		return;
	_info.InitLocations( Rect(), bHorz );

CPoint ptCursor;
VERIFY( ::GetCursorPos(&ptCursor) );
	pBar->ScreenToClient( &ptCursor );
bool bFlat = true;
bool bDrawBorder = true;
int eAlign =
		CExtPaintManager::__ALIGN_HORIZ_CENTER
			| CExtPaintManager::__ALIGN_VERT;
	if( _info.m_bBtnHelp )
	{
		g_PaintManager->PaintPushButton(
			dc,bHorz ? true : false,
			_info.m_rcBtnHelp,_T(""),NULL,bFlat,
			(IsHover() && _info.m_rcBtnHelp.PtInRect(ptCursor))
				? true : false,
			(IsPressed() && _info.m_rcBtnHelp.PtInRect(ptCursor))
				? true : false,
			false,
			IsEnabled() ? true : false,
			bDrawBorder,false,false,eAlign,
			NULL,false,SC_CONTEXTHELP
			);
	}
	if( _info.m_bBtnMinimize )
	{
		g_PaintManager->PaintPushButton(
			dc,bHorz ? true : false,
			_info.m_rcBtnRestore,_T(""),NULL,bFlat,
			(IsHover() && _info.m_rcBtnRestore.PtInRect(ptCursor))
				? true : false,
			(IsPressed() && _info.m_rcBtnRestore.PtInRect(ptCursor))
				? true : false,
			false,
			IsEnabled() ? true : false,
			bDrawBorder,false,false,eAlign,
			NULL,false,SC_RESTORE
			);
		g_PaintManager->PaintPushButton(
			dc,bHorz ? true : false,
			_info.m_rcBtnMinimize,_T(""),NULL,bFlat,
			(IsHover() && _info.m_rcBtnMinimize.PtInRect(ptCursor))
				? true : false,
			(IsPressed() && _info.m_rcBtnMinimize.PtInRect(ptCursor))
				? true : false,
			false,
			IsEnabled() ? true : false,
			bDrawBorder,false,false,eAlign,
			NULL,false,SC_MINIMIZE
			);
	}
	if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize )
	{
		g_PaintManager->PaintPushButton(
			dc,bHorz ? true : false,
			_info.m_rcBtnMaximize,_T(""),NULL,bFlat,
			(IsHover() && _info.m_rcBtnMaximize.PtInRect(ptCursor))
				? true : false,
			(IsPressed() && _info.m_rcBtnMaximize.PtInRect(ptCursor))
				? true : false,
			false,
			IsEnabled() ? true : false,
			bDrawBorder,false,false,eAlign,
			NULL,false,SC_MAXIMIZE
			);
	}
	if( _info.m_bBtnClose )
	{
		g_PaintManager->PaintPushButton(
			dc,bHorz ? true : false,
			_info.m_rcBtnClose,_T(""),NULL,bFlat,
			(IsHover() && _info.m_rcBtnClose.PtInRect(ptCursor))
				? true : false,
			(IsPressed() && _info.m_rcBtnClose.PtInRect(ptCursor))
				? true : false,
			false,
			IsEnabled() ? true : false,
			bDrawBorder,false,false,eAlign,
			NULL,false,SC_CLOSE
			);
	}
}

void CExtBarMdiRightButton::OnTrackPopup(
	CPoint point
	)
{
	point;
}

void CExtBarMdiRightButton::OnClick(
	CPoint point,
	bool bDown
	)
{
	if( bDown )
		return;

	ASSERT_VALID( m_pBar );
	ASSERT( m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) );
	ASSERT( m_pCtrl == NULL );
	ASSERT( !IsSeparator() );

CExtMenuControlBar * pBar =
		DYNAMIC_DOWNCAST(
			CExtMenuControlBar,
			m_pBar
			);
	ASSERT_VALID( pBar );
BOOL bHorz = !pBar->IsDockedVertically();
MdiMenuBarRightButtonsInfo_t _info;
	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz );
	if( !_info.m_bInitialized )
		return;
	_info.InitLocations( Rect(), bHorz );

UINT nHelperSysCmdID = 0;
	if( _info.m_bBtnHelp
		&& _info.m_rcBtnHelp.PtInRect(point)
		)
		nHelperSysCmdID = SC_CONTEXTHELP;
	else if( _info.m_bBtnMinimize
		&& _info.m_rcBtnRestore.PtInRect(point)
		)
		nHelperSysCmdID = SC_RESTORE;
	else if( _info.m_bBtnMinimize
		&& _info.m_rcBtnMinimize.PtInRect(point)
		)
		nHelperSysCmdID = SC_MINIMIZE;
	else if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize
		&& _info.m_rcBtnMaximize.PtInRect(point)
		)
		nHelperSysCmdID = SC_MAXIMIZE;
	else if( _info.m_bBtnClose
		&& _info.m_rcBtnClose.PtInRect(point)
		)
		nHelperSysCmdID = SC_CLOSE;

	if( nHelperSysCmdID == 0 )
		return;

BOOL bDummyMax = FALSE;
HWND hWndActiveChild =
		_GetActiveMdiChildWnd( bDummyMax );
	if( hWndActiveChild == NULL )
		return;
	ASSERT( ::IsWindow( hWndActiveChild ) );
	pBar->ClientToScreen( &point );
	::PostMessage(
		hWndActiveChild,
		WM_SYSCOMMAND,
		nHelperSysCmdID,
		MAKELONG( point.x, point.y )
		);
}

void CExtMenuControlBar::_KillFrameMenu()
{
CFrameWnd * pFrame = _GetDockingFrameImpl(); //GetParentFrame();
	if( ::GetMenu( pFrame->GetSafeHwnd() ) == NULL )
		return;
	// not to make MFC ignore SetMenu(NULL), post it.
	PostMessage(
		nMsgSetMenuNULL,
		(WPARAM)pFrame->GetSafeHwnd()
		);
}
