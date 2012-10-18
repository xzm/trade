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

// extcontrolbar.cpp : implementation file
//

#include "stdafx.h"

//
// Many thanks to Paul DiLascia & Cristi Posea, their works
// were a powerful incentive to learn more about pretty
// complicated MFC docking windows mechanism
//

#if (!defined __EXT_CONTROLBAR_H)
	#include <ExtControlBar.h>
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

#if (!defined __AFXPRIV_H__)
	#include <AfxPriv.h>
#endif 

#if _MSC_VER > 1200
#include <../Src/mfc/AfxImpl.h>
#else
#include <../Src/AfxImpl.h>
#endif // _MSC_VER > 1200

/*
#ifndef __AFXOLE_H__
	#include <AfxOle.h>
#endif 
*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////#define __DEBUG_PAINTING_AREAS__

#define __PLACEHODLER_BAR_PTR( __CB__ ) \
	( (HIWORD(__CB__) ) == 0 )

#define __BAR_NC_GAP_X 1 //afxData.cxBorder2
#define __BAR_NC_GAP_Y 1 //afxData.cyBorder2

#define __DOCKING_SIDE_GAP 10

static HCURSOR g_hCursorArrow	= ::LoadCursor(NULL,IDC_ARROW);
static HCURSOR g_hCursorDrag	= ::LoadCursor(NULL,IDC_SIZEALL);
static HCURSOR g_hCursorResizeV	= ::LoadCursor(NULL,IDC_SIZENS);
static HCURSOR g_hCursorResizeH	= ::LoadCursor(NULL,IDC_SIZEWE);

/////////////////////////////////////////////////////////////////////////////
// CExtControlBar::FriendlyDockBarHack

class CExtControlBar::FriendlyDockBarHack
	: CDockBar
{
public:
	friend class CExtControlBar;

	INT GetCalcExtentMax( BOOL bHorz )
	{
		ASSERT( this != NULL );
		ASSERT_KINDOF( CDockBar, this );
		return
			bHorz ? m_rectLayout.Width() : m_rectLayout.Height();
	}

	void _InsertBar( INT nPos, CControlBar * pBar )
	{
		ASSERT( nPos >= 0 && nPos < m_arrBars.GetSize() );
		m_arrBars.InsertAt( nPos, pBar );
	}
	
	void _InsertRowSplitter( INT nPos )
	{
		_InsertBar( nPos, NULL );
	}

}; // class CExtControlBar::FriendlyDockBarHack

/////////////////////////////////////////////////////////////////////////////
// CExtControlBar

IMPLEMENT_DYNAMIC( CExtControlBar, CControlBar )

ExtControlBarVector_t CExtControlBar::g_AllBars;
CExtControlBar::InternalDockBarInfo_t CExtControlBar::g_DockBarInfo;
const UINT CExtControlBar::g_nMsgQueryIcon =
	::RegisterWindowMessage(
		_T("CExtControlBar::g_nMsgQueryIcon")
		);

static CString productsection2regkeypath(
	LPCTSTR sProfileName,
	LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	LPCTSTR sSectionNameProduct // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	)
{
	return CExtCmdManager::GetSubSystemRegKeyPath(
		__PROF_UIS_REG_CONTROL_BAR,
		sProfileName,
		sSectionNameCompany,
		sSectionNameProduct
		);
}

CExtControlBar::InternalDockBarInfo_t::InternalDockBarInfo_t()
{
	m_mapDockBarInfo.SetAt(
		AFX_IDW_DOCKBAR_TOP,
		CBRS_TOP
		);
	m_mapDockBarInfo.SetAt(
		AFX_IDW_DOCKBAR_BOTTOM,
		CBRS_BOTTOM
		);
	m_mapDockBarInfo.SetAt(
		AFX_IDW_DOCKBAR_LEFT,
		CBRS_LEFT
		);
	m_mapDockBarInfo.SetAt(
		AFX_IDW_DOCKBAR_RIGHT,
		CBRS_RIGHT
		);
}

DWORD CExtControlBar::InternalDockBarInfo_t::operator[]( UINT nDockBarID )
{
	DWORD dwStyle = 0;
	VERIFY( m_mapDockBarInfo.Lookup(nDockBarID,dwStyle) );
	return dwStyle;
}

void CExtControlBar::InternalDockBarInfo_t::ReplaceFrameDockBar(
	UINT nDockBarID,
	DWORD dwDockStyle,
	CFrameWnd * pFrame
	)
{
	ASSERT_VALID(pFrame);
	ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);
DWORD dwDockBarStyle = (*this)[nDockBarID];
	if( (dwDockBarStyle&dwDockStyle&CBRS_ALIGN_ANY) == 0 )
		return;
	CDockBar * pDockBar = (CDockBar*)
		pFrame->GetControlBar(nDockBarID);
	ASSERT_VALID( pDockBar );
	ASSERT_KINDOF( CDockBar, pDockBar );
	if( pDockBar->IsKindOf(
			RUNTIME_CLASS(CExtDockBar)
			)
		)
		return;
BOOL bExplicitDelete = !pDockBar->m_bAutoDelete;
	pDockBar->m_pDockSite->RemoveControlBar( pDockBar );
	pDockBar->m_pDockSite = NULL;
	pDockBar->DestroyWindow();
	if( bExplicitDelete )
		delete pDockBar;
	pDockBar = new CExtDockBar;
	DWORD dwStyle =
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN
		|dwDockBarStyle;
	VERIFY(
		pDockBar->Create( pFrame, dwStyle, nDockBarID )
		);
}

CExtControlBar::CExtControlBar()
	: m_bGripperStaticallyAtTop( false )
	, m_bAllowContractExpand( true )
	, m_bDoNotEraseClientBackground( false )
	, m_bFixedMode( false )
	, m_nGripHeightAtTop( ::GetSystemMetrics(SM_CYSMCAPTION)+2 )
	, m_nGripWidthAtLeft( ::GetSystemMetrics(SM_CYSMCAPTION)+2 )
	, m_nSeparatorHeight( min( 4, ::GetSystemMetrics(SM_CYSIZEFRAME) ) )
	, m_nSeparatorWidth(  min( 4, ::GetSystemMetrics(SM_CXSIZEFRAME) ) )
	, m_nMinHW( ::GetSystemMetrics(SM_CYSMCAPTION)+2 + m_nSeparatorHeight )
	, m_nMinVH( ::GetSystemMetrics(SM_CYSMCAPTION)+2 + m_nSeparatorHeight )
	, m_nTrackerOffset( 0 )
	, m_nMetricOffset( 0 )
	, m_sizeDockedH( 200, 150 )
	, m_sizeDockedV( 150, 200 )
	, m_sizeFloated( 200, 200 )
	, m_bRowResizing( false )
	, m_bRowRecalcing( false )
	, m_bDragging( false )
	, m_bEnableProfile( true )
	, m_bReposSingleChildMode( true )
	, m_ptHelperDraggingOffset( 0, 0 )
	, m_ptFloatHelper( 0, 0 )
	, m_bWindowActive( false )
	, m_bTopRecalcing( false )
	, m_bNcAreaBtnTracking( false )
{
	g_AllBars.Add( this );
}

CExtControlBar::~CExtControlBar()
{
INT nCountOfExtBars = g_AllBars.GetSize();
	for( INT nExtBar = 0; nExtBar < nCountOfExtBars; nExtBar++ )
	{
		CExtControlBar * pBar = g_AllBars[ nExtBar ];
		ASSERT( pBar != NULL );
		if( pBar == this )
		{
			g_AllBars.RemoveAt( nExtBar );
			break;
		}
	} // for( INT nExtBar = 0; nExtBar < nCountOfExtBars; nExtBar++ )

	NcButtons_RemoveAll();
}


BEGIN_MESSAGE_MAP(CExtControlBar, CControlBar)
	//{{AFX_MSG_MAP(CExtControlBar)
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_CANCELMODE()
	ON_WM_CAPTURECHANGED()
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
    ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_SETTINGCHANGE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()


BOOL CExtControlBar::Create(
	LPCTSTR lpszWindowName,
	CWnd * pParentWnd,
    UINT nID, // = AFX_IDW_DIALOGBAR
	DWORD dwStyle // =
		//WS_CHILD|WS_VISIBLE
		//|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS
		//|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
		//|CBRS_HIDE_INPLACE
	)
{
    ASSERT_VALID( pParentWnd );
    ASSERT(
		!((dwStyle & CBRS_SIZE_FIXED)
		&&
        (dwStyle & CBRS_SIZE_DYNAMIC))
		);
    m_dwStyle = dwStyle & CBRS_ALL;

CString sWindowName(
		(lpszWindowName != NULL)
			? lpszWindowName : _T("")
		);
	if( lpszWindowName == NULL && nID != 0 && nID != UINT(-1) )
	{
		CExtCmdManager::cmd_t * p_cmd =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( pParentWnd->GetSafeHwnd() ),
				nID
				);
		if( p_cmd != NULL )
		{
			sWindowName = p_cmd->m_sMenuText;
			sWindowName.Replace( _T("&"), _T("") );
		}
		else
		{
			if( !sWindowName.LoadString(nID	) )
				sWindowName = _T("");
		}
	}
	if( !sWindowName.IsEmpty() )
	{
		int nNlPos = sWindowName.Find( _T('\n') );
		if( nNlPos >= 0 )
			sWindowName = sWindowName.Left( nNlPos );
	}
    
	dwStyle &= ~CBRS_ALL;
    dwStyle |= WS_CLIPCHILDREN;
CString sWndClassName(
		::AfxRegisterWndClass(
			CS_DBLCLKS,
			g_hCursorArrow,
			::GetSysColorBrush( COLOR_BTNFACE ),
			0
			)
		);

	if(	!CWnd::Create(
			LPCTSTR(sWndClassName),
			sWindowName.IsEmpty() ? _T("") : LPCTSTR(sWindowName),
			dwStyle,
			CRect( 0, 0, 0, 0 ),
			pParentWnd,
			nID
			)
		)
	{
		ASSERT( FALSE );
        return FALSE;
	}
    return TRUE;
}

void CExtControlBar::SetupFloatingFrameWnd(
	CFrameWnd * pFrame
	)	
{
	ASSERT( pFrame != NULL );
	((CExtControlBar::InternalFriendlyFrameWnd *)pFrame)->
		SetupSmilyFrameWnd();
}

void CExtControlBar::FrameEnableDocking(
	CFrameWnd * pFrame,
	DWORD dwDockStyle, // = CBRS_ALIGN_ANY
	bool bReplaceFloatingWnd // = true
	) 
{
	ASSERT_VALID(pFrame);
	ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);
	pFrame->EnableDocking( dwDockStyle );
	g_DockBarInfo.ReplaceFrameDockBar(
		AFX_IDW_DOCKBAR_TOP, dwDockStyle, pFrame );
	g_DockBarInfo.ReplaceFrameDockBar(
		AFX_IDW_DOCKBAR_BOTTOM, dwDockStyle, pFrame );
	g_DockBarInfo.ReplaceFrameDockBar(
		AFX_IDW_DOCKBAR_LEFT, dwDockStyle, pFrame );
	g_DockBarInfo.ReplaceFrameDockBar(
		AFX_IDW_DOCKBAR_RIGHT, dwDockStyle, pFrame );
	if( bReplaceFloatingWnd )
		SetupFloatingFrameWnd( pFrame );
}

bool CExtControlBar::ProfileBarStateLoad(
	CFrameWnd * pFrame,
	LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	LPWINDOWPLACEMENT pFrameWp // = NULL // need serialize frame's WP
	)
{
	ASSERT( sSectionNameCompany != NULL );
	ASSERT( sSectionNameProduct != NULL );
	ASSERT_VALID( pFrame );

LPCTSTR strProfileName =
		g_CmdManager->ProfileNameFromWnd(
			pFrame->GetSafeHwnd()
			);
	if( pFrame == NULL
		|| strProfileName == NULL
		|| strProfileName[0] == _T('\0')
		)
	{
		ASSERT( FALSE );
		return false;
	}
	ASSERT_VALID( pFrame );

LPCTSTR lpszProfileName =
		AfxGetApp()->m_pszProfileName;
	ASSERT( lpszProfileName != NULL );
	ASSERT( lpszProfileName[0] != _T('\0') );
CString sRegKeyPath=
		productsection2regkeypath(
			strProfileName,
			sSectionNameCompany,
			sSectionNameProduct
			);

bool bRetVal = false;
	try
	{
		// prepare memory file and archive,
		// get information from registry
		CMemFile _file;
		if( !CExtCmdManager::FileObjFromRegistry(
				_file,
				(LPCTSTR)sRegKeyPath
				)
			)
		{
//			ASSERT( FALSE );
			return false;
		}
		CArchive ar(
			&_file,
			CArchive::load
			);
		// do serialization
		bRetVal = ProfileBarStateSerialize(
			ar,
			pFrame,
			pFrameWp
			);
	} // try
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )
	return bRetVal;
}

bool CExtControlBar::ProfileBarStateSave(
	CFrameWnd * pFrame,
	LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	LPWINDOWPLACEMENT pFrameWp // = NULL // need serialize frame's WP
	)
{
	ASSERT( sSectionNameCompany != NULL );
	ASSERT( sSectionNameProduct != NULL );
	ASSERT_VALID( pFrame );

LPCTSTR strProfileName =
		g_CmdManager->ProfileNameFromWnd(
			pFrame->GetSafeHwnd()
			);
	if( pFrame == NULL
		|| strProfileName == NULL
		|| strProfileName[0] == _T('\0')
		)
	{
		ASSERT( FALSE );
		return false;
	}
	ASSERT_VALID( pFrame );

LPCTSTR lpszProfileName =
		AfxGetApp()->m_pszProfileName;
	ASSERT( lpszProfileName != NULL );
	ASSERT( lpszProfileName[0] != _T('\0') );
CString sRegKeyPath =
		productsection2regkeypath(
			strProfileName,
			sSectionNameCompany,
			sSectionNameProduct
			);

bool bRetVal = false;
	try
	{
		// prepare memory file and archive
		CMemFile _file;
		CArchive ar(
			&_file,
			CArchive::store
			);
		// do serialization
		ProfileBarStateSerialize(
			ar,
			pFrame,
			pFrameWp
			);
		// OK, serialization passed
		ar.Flush();
		ar.Close();
		// put information to registry
		bRetVal =
			CExtCmdManager::FileObjToRegistry(
				_file,
				(LPCTSTR)sRegKeyPath
				);
	} // try
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )

	return bRetVal;
}

void CExtControlBar::InternalFriendlyFrameWnd::SetupSmilyFrameWnd(void)
{
	m_pFloatingFrameClass =
		RUNTIME_CLASS(CExtMiniDockFrameWnd);
}

void CExtControlBar::InternalFriendlyFrameWnd::SetDockState(const CDockState& state)
{
	// first pass through barinfo's sets the m_pBar member correctly
	// creating floating frames if necessary
	for( int i = 0; i < state.m_arrBarInfo.GetSize(); i++ )
	{
		CControlBarInfo * pInfo = (CControlBarInfo *)
			state.m_arrBarInfo[i];
		ASSERT( pInfo != NULL );
		if( pInfo->m_bFloating )
		{
			// need to create floating frame to match
			CMiniDockFrameWnd* pDockFrame =
				CreateFloatingFrame(
					pInfo->m_bHorz ?
						CBRS_ALIGN_TOP : CBRS_ALIGN_LEFT
					);
			ASSERT( pDockFrame != NULL) ;
			CRect rcWnd( pInfo->m_pointPos, CSize(10, 10) );
			pDockFrame->CalcWindowRect( &rcWnd );
			pDockFrame->SetWindowPos(
				NULL,
				rcWnd.left, rcWnd.top, 0, 0,
				SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE
				);
			CDockBar * pDockBar = (CDockBar *)
				pDockFrame->GetDlgItem( AFX_IDW_DOCKBAR_FLOAT );
			ASSERT( pDockBar != NULL );
			ASSERT_KINDOF( CDockBar, pDockBar );
			pInfo->m_pBar = pDockBar;
		} // if( pInfo->m_bFloating )
		else // regular dock bar or toolbar
		{
			pInfo->m_pBar = GetControlBar( pInfo->m_nBarID );
			if( pInfo->m_pBar == NULL )
				continue;
		} // if( pInfo->m_bFloating )
		pInfo->m_pBar->m_nMRUWidth = pInfo->m_nMRUWidth;
	} // for( int i = 0; i < state.m_arrBarInfo.GetSize(); i++ )

	// the second pass will actually dock all of the control bars and
	//  set everything correctly
	for( i = 0; i < state.m_arrBarInfo.GetSize(); i++ )
	{
		CControlBarInfo * pInfo = (CControlBarInfo *)
			state.m_arrBarInfo[i];
		ASSERT( pInfo != NULL );
		if( pInfo->m_pBar != NULL )
			pInfo->m_pBar->SetBarInfo( pInfo, this );
	} // for( i = 0; i < state.m_arrBarInfo.GetSize(); i++ )

	// last pass shows all the floating windows that were previously shown
	for( i = 0; i < state.m_arrBarInfo.GetSize(); i++ )
	{
		CControlBarInfo * pInfo = (CControlBarInfo *)
			state.m_arrBarInfo[i];
		ASSERT( pInfo != NULL );
		if( pInfo->m_pBar == NULL )
			continue;
		if( !pInfo->m_bFloating )
			continue;
		CFrameWnd * pFrameWnd =
			pInfo->m_pBar->GetParentFrame();
		CDockBar * pDockBar = (CDockBar *) pInfo->m_pBar;
		ASSERT_KINDOF( CDockBar, pDockBar );
		if( pDockBar->GetDockedVisibleCount() > 0 )
		{
			pFrameWnd->RecalcLayout();
			pFrameWnd->ShowWindow( SW_SHOWNA );
		} // if( pDockBar->GetDockedVisibleCount() > 0 )
	} // for( i = 0; i < state.m_arrBarInfo.GetSize(); i++ )
	DelayRecalcLayout();
}

void CExtControlBar::InternalFriendlyFrameWnd::GetDockState(CDockState& state) const
{
	state.Clear(); //make sure dockstate is empty
	for(	POSITION pos = m_listControlBars.GetHeadPosition();
			pos != NULL;
		)
	{ // get state info for each bar
		CControlBar * pBar = (CControlBar *)
			m_listControlBars.GetNext( pos );
		ASSERT( pBar != NULL );
		CControlBarInfo* pInfo = new CControlBarInfo;
		pBar->GetBarInfo( pInfo );
		if( pInfo->m_pBar == NULL )
			continue;
		state.m_arrBarInfo.Add(pInfo);
	} // get state info for each bar
}

bool CExtControlBar::ProfileBarStateSerialize(
	CArchive & ar,
	CFrameWnd * pFrame,
	LPWINDOWPLACEMENT pFrameWp // = NULL // need serialize frame's WP
	)
{
	ASSERT( pFrame != NULL );
	ASSERT(
		::IsWindow( pFrame->GetSafeHwnd() )
		);
	ASSERT_VALID( pFrame );

bool bRetVal = false;
	try
	{
		CString sFriendlyVer;
		DWORD dwApiVer0 = g_CmdManager.GetVersionDWORD( false );
		DWORD dwApiVer1 = g_CmdManager.GetVersionDWORD( true );
		DWORD dwReserved = 0;

		if( ar.IsStoring() )
		{
			// serialize version info
			sFriendlyVer.Format(
				_T("Prof-UIS (v. %s) control bar state"),
				g_CmdManager.GetVersionString()
				);
			ar << sFriendlyVer;
			ar << dwApiVer0;
			ar << dwApiVer1;
			ar << dwReserved;
			ar << dwReserved;
			ar << dwReserved;
			ar << dwReserved;

			// serialize frame position
			WINDOWPLACEMENT _frame_wp;
			::memset( &_frame_wp, 0, sizeof(WINDOWPLACEMENT) );
			_frame_wp.length = sizeof(WINDOWPLACEMENT);
			if( !pFrame->GetWindowPlacement(&_frame_wp) )
			{
				ASSERT( FALSE );
				return false;
			};
			if( pFrameWp != NULL )
				::memcpy(
					pFrameWp,
					&_frame_wp,
					sizeof(WINDOWPLACEMENT)
					);
			CString sFrameWp;
			sFrameWp.Format(
				_T("%d,%d,%d,%d,%d,%d"),
				_frame_wp.rcNormalPosition.left,
				_frame_wp.rcNormalPosition.top,
				_frame_wp.rcNormalPosition.right,
				_frame_wp.rcNormalPosition.bottom,
				_frame_wp.showCmd,
				_frame_wp.flags
				);
			ar << sFrameWp;

			// prepare vector of control bars to serialize
			CDWordArray vBarIDs;
			ExtControlBarVector_t vBars;
			_GetFrameControlBars( pFrame, vBars );

			for( INT iBar=0; iBar<vBars.GetSize(); ++iBar )
			{
				CExtControlBar * pBar = vBars[iBar];
				ASSERT( pBar != NULL );
				ASSERT( pBar->m_bEnableProfile );
				DWORD nBarDlgCtrlID =
					pBar->GetDlgCtrlID();
				vBarIDs.Add( nBarDlgCtrlID );
			} // for( INT iBar=0; iBar<vBars.GetSize(); ++iBar )

			// serialize control bar IDs
			ASSERT( vBarIDs.GetSize() == vBars.GetSize() );
			vBarIDs.Serialize( ar );

			// serialize control bars extended info
			for( iBar=0; iBar<vBars.GetSize(); ++iBar )
			{
				CExtControlBar * pBar = vBars[iBar];
				ASSERT( pBar != NULL );
				if( !pBar->SerializeState(ar) )
				{
					ASSERT( FALSE );
					return false;
				}
			} // for( iBar=0; iBar<vBars.GetSize(); ++iBar )

			// serialize control bars MFC docking state
			CDockState state;
			((InternalFriendlyFrameWnd *)pFrame)->
				GetDockState(state);
			state.Serialize( ar );
		} // if( ar.IsStoring() )
		else
		{
			// serialize version info
			DWORD dwApiVer0a = 0, dwApiVer1a = 0;
			ar >> sFriendlyVer;
			ar >> dwApiVer0a;
			ar >> dwApiVer1a;
			ar >> dwReserved;
			ar >> dwReserved;
			ar >> dwReserved;
			ar >> dwReserved;
			if( dwApiVer1 != dwApiVer1a )
				return false;

			// serialize frame position
			CString sFrameWp;
			ar >> sFrameWp;
			if( sFrameWp.IsEmpty() )
			{
				ASSERT( FALSE );
				return false;
			}
			WINDOWPLACEMENT _frame_wp;
			::memset( &_frame_wp, 0, sizeof(WINDOWPLACEMENT) );
			_frame_wp.length = sizeof(WINDOWPLACEMENT);
			if( !pFrame->GetWindowPlacement(&_frame_wp)  )
			{
				ASSERT( FALSE );
				return false;
			}
			if(	_stscanf(
					sFrameWp,
					_T("%d,%d,%d,%d,%d,%d"),
					&_frame_wp.rcNormalPosition.left,
					&_frame_wp.rcNormalPosition.top,
					&_frame_wp.rcNormalPosition.right,
					&_frame_wp.rcNormalPosition.bottom,
					&_frame_wp.showCmd,
					&_frame_wp.flags
					) == 6
				)
			{
				_frame_wp.ptMinPosition =
					CPoint( 0, 0 );
				_frame_wp.ptMaxPosition =
					CPoint(
						-::GetSystemMetrics(SM_CXBORDER),
						-::GetSystemMetrics(SM_CYBORDER)
						);
				//if( bForceHideWindow )
				//	_frame_wp.showCmd = SW_HIDE;
//				UINT showCmdOld = _frame_wp.showCmd;
//				_frame_wp.showCmd = SW_SHOWNA;
//				pFrame->SetWindowPlacement( &_frame_wp );
//				_frame_wp.showCmd = showCmdOld;
///				if( !pFrame->IsWindowVisible() )
///					pFrame->MoveWindow( &_frame_wp.rcNormalPosition );
			}
			else
			{
				ASSERT( FALSE );
				::memset( &_frame_wp, 0, sizeof(WINDOWPLACEMENT) );
				_frame_wp.showCmd = SW_HIDE;
				return false;
			}
			if( pFrameWp != NULL )
				::memcpy(
					pFrameWp,
					&_frame_wp,
					sizeof(WINDOWPLACEMENT)
					);

			// serialize control bar IDs
			CDWordArray vBarIDs;
			vBarIDs.Serialize( ar );

			// prepare vector of control bars to serialize
			ExtControlBarVector_t vBarsSearch,vBars;
			_GetFrameControlBars( pFrame, vBarsSearch );
			int nBC = vBarIDs.GetSize();
			if( nBC != vBarsSearch.GetSize() )
			{
				ASSERT( FALSE );
				return false;
			}
			
			for( int nBar = 0; nBar < nBC; nBar++ )
			{
				DWORD nSearchBarDlgCtrlID = vBarIDs[nBar];
				CExtControlBar * pSearchBar = NULL;
				for( INT iBar=0; iBar<vBarsSearch.GetSize() && pSearchBar == NULL; iBar++ )
				{
					CExtControlBar * pBar = vBarsSearch[iBar];
					ASSERT( pBar != NULL );
					ASSERT( pBar->m_bEnableProfile );
					DWORD nBarDlgCtrlID =
						pBar->GetDlgCtrlID();
					if( nSearchBarDlgCtrlID == nBarDlgCtrlID )
					{
						pSearchBar = pBar;
						break;
					}
				} // for( INT iBar=0; iBar<vBarsSearch.GetSize() && pSearchBar == NULL; iBar++ )
				if( pSearchBar == NULL )
				{
					ASSERT( FALSE );
					return false;
				}
				vBars.Add( pSearchBar );
			} // for( int nBar = 0; nBar < nBC; nBar++ )
			ASSERT( vBars.GetSize() == nBC );

			// serialize control bars extended info
			for( INT iBar=0; iBar<vBars.GetSize(); iBar++ )
			{
				CExtControlBar * pBar = vBars[iBar];
				ASSERT( pBar != NULL );
				if( !pBar->SerializeState(ar) )
				{
					ASSERT( FALSE );
					return false;
				}
			} // for( INT iBar=0; iBar<vBars.GetSize(); iBar++ )

			// serialize control bars MFC docking state
			CDockState state;
			state.Serialize( ar );
			((InternalFriendlyFrameWnd *)pFrame)->
				SetDockState(state);
		} // else from if( ar.IsStoring() )

		// almost OK
		bRetVal = true;
	} // try
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )

	return bRetVal;
}

bool CExtControlBar::SerializeState(
	CArchive & ar
	)
{
    ASSERT_VALID( this );
    ASSERT( GetSafeHwnd() ); // must be called after Create()

	if( !m_bEnableProfile )
	{
		ASSERT( FALSE ); /// !?!?!
		return true;
	}

enum
{
	FLAG_FLOATING = 1L,
};

//#ifdef _DEBUG
int nOwnID = GetDlgCtrlID();
//#endif // _DEBUG

	if( ar.IsStoring() )
	{

//#ifdef _DEBUG
		ar << nOwnID;
//#endif // _DEBUG

		ar << m_sizeDockedH.cx;
		ar << m_sizeDockedH.cy;
		
		ar << m_sizeDockedV.cx;
		ar << m_sizeDockedV.cy;
		
		ar << m_sizeFloated.cx;
		ar << m_sizeFloated.cy;
		
		DWORD dwFlags = 0;
		if( IsFloating() )
		{
			dwFlags |= FLAG_FLOATING;
			CFrameWnd * pMiniFrame = GetParentFrame();
			ASSERT( pMiniFrame != NULL );
			ASSERT_VALID( pMiniFrame );
			ASSERT_KINDOF( CMiniDockFrameWnd, pMiniFrame );
			CRect rcWnd;
			pMiniFrame->GetWindowRect( &rcWnd );
			m_ptFloatHelper = rcWnd.TopLeft();
		}

		ar << m_ptFloatHelper.x;
		ar << m_ptFloatHelper.y;
		
		ar << dwFlags;

	} // if( ar.IsStoring() )
	else
	{

//#ifdef _DEBUG
		int nOwnID2 = 0;
		ar >> nOwnID2;
		if( nOwnID != nOwnID2 )
		{
			ASSERT( FALSE );
			return false;
		}
//#endif // _DEBUG
		
		ar >> m_sizeDockedH.cx;
		ar >> m_sizeDockedH.cy;
		
		ar >> m_sizeDockedV.cx;
		ar >> m_sizeDockedV.cy;
		
		ar >> m_sizeFloated.cx;
		ar >> m_sizeFloated.cy;
		
		ar >> m_ptFloatHelper.x;
		ar >> m_ptFloatHelper.y;
		
		DWORD dwFlags = 0;
		ar >> dwFlags;
		
		if( dwFlags & FLAG_FLOATING )
			_GetDockingFrameImpl()->
				FloatControlBar(
					this,
					m_ptFloatHelper
					);
	
	} // else from if( ar.IsStoring() )

	return true;
}

LRESULT CExtControlBar::OnSetText(WPARAM wParam, LPARAM lParam)
{
    wParam;
LRESULT lResult = CWnd::Default();
    if( IsFloating() &&
        GetParentFrame()->IsKindOf(
			RUNTIME_CLASS(CMiniDockFrameWnd))
			)
    {
        m_pDockBar->SetWindowText( (LPCTSTR) lParam );
        GetParentFrame()->DelayRecalcLayout();
    }
bool pbTextOnGripper = false;
	if( IsBarWithGripper( NULL, &pbTextOnGripper ) )
	{
		if( pbTextOnGripper )
			SendMessage( WM_NCPAINT );
	}
	return lResult;
}

int CExtControlBar::_GetDockedExtBars(
	ExtControlBarVector_t & vBars,
	CExtControlBar * pExcludeBar // = NULL
	)
{
    vBars.RemoveAll();
int nCountOfBars = g_AllBars.GetSize();
	for( int i=0; i<nCountOfBars; i++ )
	{
		CExtControlBar * pBar = g_AllBars[i];
		ASSERT( pBar != NULL );
		if( pBar == pExcludeBar
			|| (!(::IsWindow(pBar->GetSafeHwnd())))
			)
			continue;
		if( pBar->IsFloating() )
			continue;
		vBars.Add( pBar );
	}
	return vBars.GetSize();
}

void CExtControlBar::_GetFrameControlBars(
	CFrameWnd * pFrame,
	ExtControlBarVector_t & vBars,
	eGFECB_t eCallOpt // = GFECB_GET_ALL
	)
{
	ASSERT_VALID( pFrame );
	vBars.RemoveAll();
int nAllBarsCount = g_AllBars.GetSize();
	for( int nBar=0; nBar<nAllBarsCount; nBar++ )
	{
		CExtControlBar * pBar = g_AllBars[nBar];
		ASSERT( pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) );
		if( !(::IsWindow(pBar->GetSafeHwnd())) )
			continue;
		CFrameWnd * pBarFrame =
			_GetDockingFrameImpl( pBar );
		if( pBarFrame != pFrame )
			continue;
		switch( eCallOpt )
		{
		case eGFECB_t::GFECB_GET_ALL:
		break;
		case eGFECB_t::GFECB_GET_DOCKED:
			if( pBar->IsFloating() )
				continue;
		break;
		case eGFECB_t::GFECB_GET_FLOATING:
			if( !pBar->IsFloating() )
				continue;
		break;
#ifdef _DEBUG
		default:
			ASSERT( FALSE );
		break;
#endif // _DEBUG
		}; // switch( eCallOpt )
		vBars.Add( pBar );
	} // for( int nBar=0; nBar<nAllBarsCount; nBar++ )
}

void CExtControlBar::_GetFrameControlBars(
	ExtControlBarVector_t & vBars,
	eGFECB_t eCallOpt // = GFECB_GET_ALL
	) const
{
CFrameWnd * pFrame = _GetDockingFrameImpl();
	_GetFrameControlBars( pFrame, vBars, eCallOpt );
}

void CExtControlBar::_GetRowBars(
	MfcControlBarVector_t & vBars
	) const
{
    vBars.RemoveAll();
	if( m_pDockBar == NULL )
		return;
	ASSERT_VALID( m_pDockBar );
INT nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
bool bThisFound = false;
	for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )
	{
		CControlBar * pBar = (CControlBar *)
			m_pDockBar->m_arrBars[ nBar ];
		if( pBar == NULL )
		{
			if( bThisFound )
				break;
			vBars.RemoveAll();
			continue;
		}
		if( __PLACEHODLER_BAR_PTR( pBar ) )
			continue;
		ASSERT_KINDOF( CControlBar, pBar );
		if( pBar == (CControlBar *)this )
			bThisFound = true;
		if( !pBar->IsVisible() )
			continue;
		vBars.Add( pBar );
	} // for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )
}

void CExtControlBar::_GetRowExtBars(
	ExtControlBarVector_t & vBars
	) const
{
    vBars.RemoveAll();
	if( m_pDockBar == NULL )
		return;
	ASSERT_VALID( m_pDockBar );
INT nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
bool bThisFound = false;
	for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )
	{
		CControlBar * pBar = (CControlBar *)
			m_pDockBar->m_arrBars[ nBar ];
		if( pBar == NULL )
		{
			if( bThisFound )
				break;
			vBars.RemoveAll();
			continue;
		}
		if( __PLACEHODLER_BAR_PTR( pBar ) )
			continue;
		ASSERT_KINDOF( CControlBar, pBar );
		if( pBar == (CControlBar *)this )
			bThisFound = true;
		if( pBar->IsKindOf( RUNTIME_CLASS(CExtControlBar) )
			&& !((CExtControlBar *)pBar)->m_bFixedMode
			)
			vBars.Add( (CExtControlBar *)pBar );
	} // for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )
}

bool CExtControlBar::_IsFirstBarInRow( bool bExtBar ) const
{
	if( m_pDockBar == NULL )
		return false;
	ASSERT_VALID( m_pDockBar );
INT nRowCount = 0;
INT nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
	for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )
	{
		CControlBar * pBar = (CControlBar *)
			m_pDockBar->m_arrBars[ nBar ];
		if( pBar == NULL )
		{
			nRowCount = 0;
			continue;
		}
		if( __PLACEHODLER_BAR_PTR( pBar ) )
			continue;
		ASSERT_KINDOF( CControlBar, pBar );
		if( bExtBar )
		{
			CExtControlBar * pExtBar = 
				DYNAMIC_DOWNCAST( CExtControlBar, pBar );
			if( pExtBar == NULL )
				return false;
			if( pExtBar->m_bFixedMode )
				return false;
		}
		if( pBar == (CControlBar *)this )
		{
			return (nRowCount == 0) ? true : false;
		}
		nRowCount++;
	} // for( INT nBar = 0; nBar < nTotalDockCount; nBar++ )
	ASSERT( FALSE );
	return false;
}

bool CExtControlBar::_IsLastBarInRow( bool bExtBar ) const
{
	ASSERT( !IsFloating() );
	if( bExtBar )
	{
		ExtControlBarVector_t vBars;
		_GetRowExtBars( vBars );
		INT nCountOfBars = vBars.GetSize();
		ASSERT( nCountOfBars > 0 );
		if( vBars[nCountOfBars-1] == (CExtControlBar *)this )
			return true;
		return false;
	}
MfcControlBarVector_t vBars;
    _GetRowBars( vBars );
INT nCountOfBars = vBars.GetSize();
	ASSERT( nCountOfBars > 0 );
	if( vBars[nCountOfBars-1] == (CControlBar *)this )
		return true;
	return false;
}

CFrameWnd * CExtControlBar::_GetDockingFrameImpl(
	CControlBar * pBar
	)
{
	ASSERT_VALID( pBar );
	ASSERT( pBar->GetSafeHwnd() != NULL );

CFrameWnd * pFrame = pBar->GetParentFrame();
	ASSERT_VALID( pFrame );
	if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) )
	{
		pFrame = pFrame->GetParentFrame();
		ASSERT_VALID( pFrame );
		ASSERT( !pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) );
	}
	return pFrame;
}

CFrameWnd * CExtControlBar::_GetDockingFrameImpl() const
{
	return _GetDockingFrameImpl( (CControlBar *)this );
}

UINT CExtControlBar::GetSafeDockBarDlgCtrlID() const
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow( GetSafeHwnd() ) );

CWnd * pParentWnd = GetParent();
	if( pParentWnd->GetSafeHwnd() == NULL
		|| !::IsWindow( pParentWnd->GetSafeHwnd() )
		|| !pParentWnd->IsKindOf( RUNTIME_CLASS(CDockBar) )
		)
		return AFX_IDW_DOCKBAR_FLOAT; // IDC_STATIC

UINT nDlgCtrlID = pParentWnd->GetDlgCtrlID();
	ASSERT(
			nDlgCtrlID == AFX_IDW_DOCKBAR_FLOAT
		||	nDlgCtrlID == AFX_IDW_DOCKBAR_LEFT
		||	nDlgCtrlID == AFX_IDW_DOCKBAR_TOP
		||	nDlgCtrlID == AFX_IDW_DOCKBAR_RIGHT
		||	nDlgCtrlID == AFX_IDW_DOCKBAR_BOTTOM
		);
	return nDlgCtrlID;
}

bool CExtControlBar::IsDockedAtTop() const
{
	return GetSafeDockBarDlgCtrlID() == AFX_IDW_DOCKBAR_TOP;
}

bool CExtControlBar::IsDockedAtBottom() const
{
	return GetSafeDockBarDlgCtrlID() == AFX_IDW_DOCKBAR_BOTTOM;
}

bool CExtControlBar::IsDockedAtLeft() const
{
	return GetSafeDockBarDlgCtrlID() == AFX_IDW_DOCKBAR_LEFT;
}

bool CExtControlBar::IsDockedAtRight() const
{
	return GetSafeDockBarDlgCtrlID() == AFX_IDW_DOCKBAR_RIGHT;
}

bool CExtControlBar::IsDockedHorizontally() const
{
UINT nDockBarDlgCtrlID = GetSafeDockBarDlgCtrlID();
	return	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_TOP
		||	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_BOTTOM;
}

bool CExtControlBar::IsDockedVertically() const
{
UINT nDockBarDlgCtrlID = GetSafeDockBarDlgCtrlID();
	return	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_LEFT
		||	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_RIGHT;
}

bool CExtControlBar::IsDocked() const
{
UINT nDockBarDlgCtrlID = GetSafeDockBarDlgCtrlID();
	return	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_TOP
		||	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_BOTTOM
		||	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_LEFT
		||	nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_RIGHT;
}

bool CExtControlBar::IsFloating() const
{
	return !IsDocked();
}

bool CExtControlBar::IsBarWithGripper(
	bool * pbGripperAtTop, // = NULL
	bool * pbTextOnGripper // = NULL
	) const
{
	ASSERT_VALID( this );
	if( pbGripperAtTop != NULL )
		*pbGripperAtTop = false;
	if( pbTextOnGripper != NULL )
		*pbTextOnGripper = false;
	if( ! ::IsWindow(GetSafeHwnd()) )
		return false;
	if( (m_dwStyle & CBRS_GRIPPER) == 0
		|| IsFloating()
		)
		return false;
	if( pbGripperAtTop != NULL
		&&	( m_bGripperStaticallyAtTop || IsDockedVertically() )
		)
		*pbGripperAtTop = true;
	if( pbTextOnGripper != NULL )
		*pbTextOnGripper = true;
    return true;
}

CSize CExtControlBar::CalcFixedLayout(
	BOOL bStretch,
	BOOL bHorz
	)
{
	bStretch;
	return _CalcDesiredLayout( bHorz );
}

CSize CExtControlBar::CalcDynamicLayout(
	int nLength,
	DWORD nMode
	)
{
	// MSDN for LM_HORZDOCK and LM_VERTDOCK: ... return
	// horizontal/vertical docked dimensions ...

	if( ((nMode & (LM_HORZDOCK | LM_VERTDOCK)) == 0)
		|| IsFloating()
		)
	{ // if going to floated state or already floated
		
		if( nLength > 0 )
		{
			// MSDN for nLength: ... The requested dimension of the
			// control bar, either horizontal or vertical,
			// depending on dwMode ...

			// MSDN for LM_LENGTHY: ... Set if nLength indicates
			// height (Y-direction) instead of width.
			if( nMode & LM_LENGTHY ) 
				m_sizeFloated.cy = nLength;
			else
				m_sizeFloated.cx = nLength;
		} // if( nLength > 0 )

		// MSDN for LM_MRUWIDTH: ... Most Recently Used Dynamic Width.
		// Ignores nLength parameter and uses the remembered most
		// recently used width.

		// MSDN for LM_COMMIT: ... Resets LM_MRUWIDTH to current
		// width of floating control bar.
		if( nMode & ( LM_MRUWIDTH | LM_COMMIT ) )
			return m_sizeFloated;

		return _CalcDesiredLayout( nMode & LM_HORZ );

	} // if going to floated state or already floated

	if( nLength >= 0 )
		return _CalcDesiredLayout( nMode & LM_HORZ );

    ASSERT_VALID( m_pDockBar );
INT nCalcExtentMax = 
		((FriendlyDockBarHack*)m_pDockBar)->
			GetCalcExtentMax( nMode & LM_HORZ );
INT nCalcExtentMin =
		(nMode & LM_HORZ) ? afxData.cyBorder2 : afxData.cxBorder2;
INT nCalcExtentReal = 0, nMaxWidth = 0;
INT nCountOfBarsAtSide = m_pDockBar->m_arrBars.GetSize();
	ASSERT( nCountOfBarsAtSide > 0 );
INT nThisIdx = -1, nReviewStart = -1, nReviewStop = -1;
INT nCountInRowReal = 0, nCountExtBarsInRowReal = 0;
INT nCountOfMinSizedBars = 0;
	for( INT nBarAtSide = 0; nBarAtSide < nCountOfBarsAtSide ; nBarAtSide++ )
	{
		CControlBar * pBar =  (CControlBar *)
			m_pDockBar->m_arrBars[ nBarAtSide ];
		if( pBar == NULL )
		{
			if( nThisIdx >= 0 )
			{
				nReviewStop = nBarAtSide - 1;
				break;
			}
			nCountInRowReal = 0;
			nCountExtBarsInRowReal = 0;
			nMaxWidth = 0;
			nReviewStop = -1;
			nReviewStart = nBarAtSide + 1;
			continue;
		} // if( pBar == NULL )
		if( __PLACEHODLER_BAR_PTR(pBar) )
			continue;
		ASSERT_VALID( pBar );
		ASSERT(
			pBar->GetSafeHwnd() != NULL
			&& ::IsWindow(pBar->GetSafeHwnd())
			);
        if( !pBar->IsVisible() )
			continue;
		ASSERT(
			pBar->GetSafeHwnd() != NULL
			&& ::IsWindow( pBar->GetSafeHwnd() )
			);
		if( pBar == this )
		{
			if( nCountExtBarsInRowReal > 0 )
				goto __Align_Mutually_Row__;
			nThisIdx = nBarAtSide;
		}
		CRect rcBarWnd;
		pBar->GetWindowRect( &rcBarWnd );
		CExtControlBar * pExtBar =
			DYNAMIC_DOWNCAST( CExtControlBar, pBar );
		if( pExtBar!=NULL && pExtBar->m_bFixedMode )
			pExtBar = NULL;
		INT nCalcExtentStep =
			(nMode & LM_HORZ) ? rcBarWnd.Width() : rcBarWnd.Height();
		if( pExtBar != NULL )
		{
			nCountExtBarsInRowReal++;
			nMaxWidth =
				max(
					nMaxWidth,
					(nMode & LM_HORZ)
						? pExtBar->m_sizeDockedH.cy
						: pExtBar->m_sizeDockedV.cx
					);
		} // if( pExtBar != NULL )
		nCountInRowReal++;
	} // for( int nBarAtSide = 0; nBarAtSide < nCountOfBarsAtSide ; nBarAtSide++ )

	if( nCountInRowReal == 1 )
	{
        if( nMode & LM_HORZ )
            m_sizeDockedH.cx =
				max( _CalcDesiredMinHW(), nCalcExtentMax );
        else
            m_sizeDockedV.cy =
				max( _CalcDesiredMinVH(), nCalcExtentMax );
		goto __Align_Mutually_Row__;
	}

    for( nBarAtSide = nReviewStart; nBarAtSide <= nReviewStop; nBarAtSide++ )
    {
        CControlBar * pBar = (CControlBar*)
			m_pDockBar->m_arrBars[ nBarAtSide ];
        if( __PLACEHODLER_BAR_PTR(pBar) )
			continue;
		ASSERT_VALID( pBar );
		ASSERT(
			pBar->GetSafeHwnd() != NULL
			&& ::IsWindow(pBar->GetSafeHwnd())
			);
        if( !pBar->IsVisible() )
			continue;
		CExtControlBar * pExtBar =
			DYNAMIC_DOWNCAST( CExtControlBar, pBar );
		if( pExtBar!=NULL && pExtBar->m_bFixedMode )
			pExtBar = NULL;

        INT nCalcExtentBar = 0;
        if( pExtBar == NULL )
        {
            CRect rcBar;
            pBar->GetWindowRect( &rcBar );
            nCalcExtentBar =
				(nMode & LM_HORZ) ?
					rcBar.Width() : rcBar.Height();
        } // if( pExtBar == NULL )
        else
            nCalcExtentBar =
				(nMode & LM_HORZ)
					? pExtBar->_CalcDesiredMinHW()
					: pExtBar->_CalcDesiredMinVH();
        nCalcExtentMin += nCalcExtentBar;
		if( nCalcExtentMin <= nCalcExtentMax )
		{
			if( pExtBar == NULL )
			{
				nCalcExtentMax -= nCalcExtentBar;
				continue;
			}
			nCalcExtentReal +=
				(nMode & LM_HORZ)
					? pExtBar->m_sizeDockedH.cx
					: pExtBar->m_sizeDockedV.cy ;
			continue;
		} // if( nCalcExtentMin <= nCalcExtentMax )
        
        if( nBarAtSide < nThisIdx )
        {
			ASSERT( pBar != this );
			((FriendlyDockBarHack*)m_pDockBar)->
				_InsertRowSplitter( nBarAtSide + 1 );
			return _CalcDesiredLayout( nMode & LM_HORZ );
        }
        if( nBarAtSide == nThisIdx )
        {
			ASSERT( pBar == this );
			if( nMode & LM_HORZ )
				m_sizeDockedH.cx =
					max( _CalcDesiredMinHW(), nCalcExtentMax );
			else
				m_sizeDockedV.cy =
					max( _CalcDesiredMinVH(), nCalcExtentMax );
			break;
        }
		ASSERT( pBar != this );
		((FriendlyDockBarHack*)m_pDockBar)->
			_InsertRowSplitter( nBarAtSide );
        nReviewStop = nBarAtSide - 1;
        break;
    } // for( nBarAtSide = nReviewStart; nBarAtSide <= nReviewStop; nBarAtSide++ )

	nCountOfMinSizedBars = 0;
	for( nBarAtSide = nReviewStart; nBarAtSide <= nReviewStop; nBarAtSide++ )
	{
        CControlBar * pBar = (CControlBar*)
			m_pDockBar->m_arrBars[ nBarAtSide ];
        if( __PLACEHODLER_BAR_PTR(pBar) )
			continue;
		ASSERT_VALID( pBar );
		ASSERT(
			pBar->GetSafeHwnd() != NULL
			&& ::IsWindow(pBar->GetSafeHwnd())
			);
        if( !pBar->IsVisible() )
			continue;
		CExtControlBar * pExtBar =
			DYNAMIC_DOWNCAST( CExtControlBar, pBar );
		if( pExtBar!=NULL && pExtBar->m_bFixedMode )
			pExtBar = NULL;
		if( pExtBar == NULL )
			continue;

        LONG nMinMetric =
			(nMode & LM_HORZ)
				? pExtBar->_CalcDesiredMinHW()
				: pExtBar->_CalcDesiredMinVH();
        LONG & nCalcExtentBar =
			(nMode & LM_HORZ) ?
				pExtBar->m_sizeDockedH.cx : pExtBar->m_sizeDockedV.cy;
		if( nMinMetric == nCalcExtentBar )
		{
			nCountOfMinSizedBars++;
			continue;
		}
	} // for( nBarAtSide = nReviewStart; nBarAtSide <= nReviewStop; nBarAtSide++ )
	ASSERT( nCountOfMinSizedBars <= nCountExtBarsInRowReal );

	if( nCountOfMinSizedBars != nCountExtBarsInRowReal )
	{
		INT nCalcExtentDiff = nCalcExtentMax - nCalcExtentReal;
		INT nCalcStep =
			nCalcExtentDiff /
				(nCountExtBarsInRowReal - nCountOfMinSizedBars);
		for(	nBarAtSide = nReviewStart;
				nBarAtSide <= nReviewStop && nCalcExtentDiff != 0;
				)
		{
			CControlBar * pBar = (CControlBar*)
				m_pDockBar->m_arrBars[ nBarAtSide ];
			if( __PLACEHODLER_BAR_PTR(pBar) )
			{
				nBarAtSide++;
				continue;
			}
			ASSERT_VALID( pBar );
			ASSERT(
				pBar->GetSafeHwnd() != NULL
				&& ::IsWindow(pBar->GetSafeHwnd())
				);
			if( !pBar->IsVisible() )
			{
				nBarAtSide++;
				continue;
			}
			CExtControlBar * pExtBar =
				DYNAMIC_DOWNCAST( CExtControlBar, pBar );
			if( pExtBar!=NULL && pExtBar->m_bFixedMode )
				pExtBar = NULL;
			if( pExtBar == NULL )
			{
				nBarAtSide++;
				continue;
			}

			if( nMode & LM_HORZ )
				pExtBar->m_sizeDockedH.cy = nMaxWidth;
			else
				pExtBar->m_sizeDockedV.cx = nMaxWidth;
			
			ASSERT( nCalcExtentDiff != 0 );

			LONG nMinMetric =
				(nMode & LM_HORZ)
					? pExtBar->_CalcDesiredMinHW()
					: pExtBar->_CalcDesiredMinVH();
			LONG & ref_nCalcExtentBar =
				(nMode & LM_HORZ) ?
					pExtBar->m_sizeDockedH.cx : pExtBar->m_sizeDockedV.cy;
			if( ref_nCalcExtentBar == nMinMetric )
			{
				nBarAtSide++;
				continue;
			}

			// TO FIX: ASSERT( ref_nCalcExtentBar > nMinMetric );
			if( ref_nCalcExtentBar + nCalcStep < nMinMetric )
				nCalcStep = (nCalcStep > 0) ? 1 : -1;
			ref_nCalcExtentBar += nCalcStep;
			nCalcExtentDiff -= nCalcStep;
			// TO FIX: ASSERT( ref_nCalcExtentBar >= nMinMetric );
			if( nMinMetric == ref_nCalcExtentBar )
				nCalcStep = (nCalcStep > 0) ? 1 : -1;

			if( nBarAtSide == nReviewStop
				&& nCalcExtentDiff != 0
				)
			{
				nBarAtSide = nReviewStart;
				nCalcStep = (nCalcStep > 0) ? 1 : -1;
				continue;
			}
			nBarAtSide++;
		} // for( nBarAtSide = nReviewStart; ...
	} // if( nCountOfMinSizedBars != nCountExtBarsInRowReal )

__Align_Mutually_Row__:

	ASSERT_VALID( m_pDockBar );
bool bAtTop = IsDockedAtTop();
CRect rcWndDockBar;
	m_pDockBar->GetWindowRect( &rcWndDockBar );
CPoint ptDevOffset = -rcWndDockBar.TopLeft();
INT nCalcBarOffset = 0;	
bool bNeedRecalc = false;
	for( nBarAtSide = nReviewStart; nBarAtSide <= nReviewStop; nBarAtSide++ )
	{
		CControlBar * pBar = (CControlBar*)
			m_pDockBar->m_arrBars[ nBarAtSide ];
		if( __PLACEHODLER_BAR_PTR(pBar) )
			continue;
		ASSERT_VALID( pBar );
		ASSERT(
			pBar->GetSafeHwnd() != NULL
			&& ::IsWindow(pBar->GetSafeHwnd())
			);
        if( !pBar->IsVisible() )
			continue;
		CExtControlBar * pExtBar =
			DYNAMIC_DOWNCAST( CExtControlBar, pBar );
		if( pExtBar != NULL && pExtBar->m_bFixedMode )
			pExtBar = NULL;
		CRect rcWndBar;
        pBar->GetWindowRect( &rcWndBar );
        rcWndBar.OffsetRect( ptDevOffset );
		if( pExtBar != NULL )
            rcWndBar =
				CRect(
					rcWndBar.TopLeft(),
					(nMode & LM_HORZ)
						? pExtBar->m_sizeDockedH
						: pExtBar->m_sizeDockedV
					);
		INT nBarRealOffset =
			(nMode & LM_HORZ) ? rcWndBar.left : rcWndBar.top;
        if( nBarRealOffset != nCalcBarOffset )
        {
            if( !(nMode & LM_HORZ) )
                rcWndBar.OffsetRect(
					0,
					nCalcBarOffset - rcWndBar.top
					);
            else if( bAtTop )
                rcWndBar.OffsetRect(
					nCalcBarOffset - rcWndBar.left,
					-2
					);
            else
            	rcWndBar.OffsetRect(
					nCalcBarOffset - rcWndBar.left,
					0
					);
			pBar->MoveWindow( &rcWndBar );
			bNeedRecalc = true;
        } // if( nBarRealOffset != nCalcBarOffset )
        nCalcBarOffset += (nMode & LM_HORZ)
			? rcWndBar.Width() : rcWndBar.Height();
	} // for( nBarAtSide = nReviewStart; nBarAtSide <= nReviewStop; nBarAtSide++ )

	if( bNeedRecalc )
	{
		m_pDockSite->DelayRecalcLayout();
		m_pDockSite->RedrawWindow(
			NULL,
			NULL,
			RDW_ERASE|RDW_INVALIDATE
				|RDW_INTERNALPAINT
				|RDW_UPDATENOW
				|RDW_ALLCHILDREN
				|RDW_NOFRAME
			);
	} // if( bNeedRecalc )
	
	return _CalcDesiredLayout( nMode & LM_HORZ );
}

INT CExtControlBar::_CalcDesiredMinHW() const
{
INT nCalcVal = m_nMinHW + m_rcRowRecalcUp.Width();
	if( m_bGripperStaticallyAtTop )
	{
		int nResizers =
			m_rcRowRecalc.Width()
			+ m_rcRowRecalcUp.Width();
		int nMinMetric =
			1
			+ ::GetSystemMetrics(SM_CYSMCAPTION)+2
			+ nResizers;
		if( IsBarWithGripper() && !m_rcGrip.IsRectEmpty() )
		{
			int nMinFriendly = m_rcGrip.Height() + nResizers;
			if( nMinMetric < nMinFriendly )
				nMinMetric = nMinFriendly;
		}
		if( nCalcVal < nMinMetric  )
			nCalcVal = nMinMetric;
	}
	return nCalcVal;
}

INT CExtControlBar::_CalcDesiredMinVH() const
{
INT nCalcVal = m_nMinVH + m_rcRowRecalcUp.Height();
	return nCalcVal;
}

CSize CExtControlBar::_CalcDesiredLayout(
	BOOL bHorz,
	BOOL bForceFullRow // = FALSE
	)
{
	if( IsFloating() )
		return m_sizeFloated;
	
	if( bHorz )
	{
		if( bForceFullRow )
			return CSize( MAXSHORT, m_sizeDockedH.cy );
		if( m_sizeDockedH.cy < _CalcDesiredMinHW() )
			m_sizeDockedH.cy = _CalcDesiredMinHW();
		return m_sizeDockedH;
	} // if( bHorz )
	else
	{
		if( bForceFullRow )
			return CSize( m_sizeDockedH.cx, MAXSHORT );
		if( m_sizeDockedV.cx < _CalcDesiredMinVH() )
			m_sizeDockedV.cx = _CalcDesiredMinVH();
		return m_sizeDockedV;
	} // else from if( bHorz )
}

void CExtControlBar::DoPaint( CDC* pDC )
{
#if (defined __DEBUG_PAINTING_AREAS__)

//	CControlBar::DoPaint( pDC );
	ASSERT( pDC->GetSafeHdc() != NULL );
CRect rcBarClient;
	GetClientRect( &rcBarClient );
	pDC->FillSolidRect( &rcBarClient, ::GetSysColor(COLOR_INFOBK) );

CRect rcDummy( rcBarClient );
	rcDummy.DeflateRect( 4, 4 );
	pDC->DrawFocusRect( &rcDummy );

#else // __DEBUG_PAINTING_AREAS__

	ASSERT( pDC->GetSafeHdc() != NULL );
CRect rcClient;
	GetClientRect( &rcClient );
	if( rcClient.IsRectEmpty()
		|| !pDC->RectVisible( &rcClient )
		)
		return;
	CExtPaintManager::stat_ExcludeChildAreas(
		*pDC,
		*this
		);
COLORREF clrBack =
		g_PaintManager->GetColor(
			CExtPaintManager::CLR_3DFACE_OUT
			);
	pDC->FillSolidRect(
		&rcClient,
		clrBack
		);

#endif // ! __DEBUG_PAINTING_AREAS__
}

void CExtControlBar::OnUpdateCmdUI(
	CFrameWnd * pTarget,
	BOOL bDisableIfNoHndler
	)
{
	pTarget;
	bDisableIfNoHndler;

bool bNeedRepaint = false;

CPoint ptScreen;
	VERIFY( ::GetCursorPos( &ptScreen ) );
UINT nHT = NcButtons_HitTest( ptScreen );
	if( nHT == HTCLIENT )
	{
		if( m_bNcAreaBtnTracking )
			bNeedRepaint = true;
		m_bNcAreaBtnTracking = false;
	} // if( nHT == HTCLIENT )
	else
	{
		if( !m_bNcAreaBtnTracking )
			bNeedRepaint = true;
		m_bNcAreaBtnTracking = true;
	} // else from if( nHT == HTCLIENT )

HWND hTmpWndFocus = ::GetFocus();
bool bWindowActiveOldVal = m_bWindowActive;
	m_bWindowActive =
		( hTmpWndFocus != NULL
		&& (  GetSafeHwnd() == hTmpWndFocus
			  || ::IsChild( GetSafeHwnd(), hTmpWndFocus )
			)
		);
	if( bWindowActiveOldVal != m_bWindowActive )
		bNeedRepaint = true;

	if( bNeedRepaint )
	{
		//_RecalcNcArea();
		SendMessage( WM_NCPAINT );
		GetParent()->SendMessage( WM_NCPAINT );
	}
}

void CExtControlBar::_RecalcNcArea()
{
	if( !IsVisible() )
		return;
	SetWindowPos(
		NULL, 0, 0, 0, 0,
		SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER
		|SWP_FRAMECHANGED
		);
}

void CExtControlBar::_RecalcLayoutImpl()
{
CFrameWnd * pParent = GetParentFrame();
	if( pParent != NULL && pParent->GetSafeHwnd() != NULL )
		pParent->RecalcLayout();
}

LRESULT CExtControlBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch( message )
	{
	case WM_NCDESTROY:
		NcButtons_RemoveAll();
		break;
	case WM_SETFOCUS:
		{
			LRESULT lResult =
				CControlBar::WindowProc(message, wParam, lParam);
			if( IsMinimizedOnRow() )
				MaximizeOnRow();
			if( !m_bFixedMode )
			{
				CWnd* pWnd = GetWindow(GW_CHILD);
				if(pWnd != NULL)
				{
					pWnd->SetFocus();
					ASSERT(pWnd->GetWindow(GW_HWNDNEXT) == NULL);
				}
			} // if( !m_bFixedMode )
			return lResult;
		}
	case WM_ERASEBKGND:
		if( !m_bDoNotEraseClientBackground )
			return !0;
		CExtPaintManager::stat_ExcludeChildAreas(
			(HDC)wParam,
			*this
			);
		return CControlBar::WindowProc(message, wParam, lParam);;
	case WM_CLOSE:
		return 0;
	case WM_WINDOWPOSCHANGED:
		{
			if( IsDocked() && m_bFixedMode )
			{
				ExtControlBarVector_t vBars;
				_GetRowExtBars( vBars );
				for( INT i = 0; i < vBars.GetSize(); i++ )
					vBars[i]->_RecalcNcArea();
			}
			break;
		}
	case WM_WINDOWPOSCHANGING:
		{
			LPWINDOWPOS lpWindowPos = 
				reinterpret_cast < LPWINDOWPOS > (lParam);
			ASSERT( lpWindowPos != NULL );
			lpWindowPos->flags |= SWP_FRAMECHANGED;
			break;
		}
	} // switch( message )
	
	return CControlBar::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CExtControlBar message handlers

void CExtControlBar::NcButtons_Paint( CDC & dc )
{
	ASSERT( dc.GetSafeHdc() != NULL );
INT nCountOfNcButtons = NcButtons_GetCount();
	for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
	{
		CExtBarNcAreaButton * pBtn =
			NcButtons_GetAt( nBtn );
		if( !pBtn->OnQueryVisibility() )
			continue;
		pBtn->OnNcAreaDraw( dc );
	} // for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
}

UINT CExtControlBar::NcButtons_HitTest( CPoint point )
{
bool bNeedRepaint = false;

CWnd * pWndNcRepaint = this;
CRect rcBarWnd;
	if( IsFloating() )
	{
		CExtMiniDockFrameWnd * pFrame =
			DYNAMIC_DOWNCAST(
				CExtMiniDockFrameWnd,
				GetDockingFrame()
				);
		if( pFrame == NULL )
			return HTCLIENT;
		pFrame->GetWindowRect( &rcBarWnd );
		pWndNcRepaint = pFrame;
	} // if( IsFloating() )
	else
	{
		GetWindowRect( &rcBarWnd );
	} // else from if( IsFloating() )
CPoint ptDevOffset = -rcBarWnd.TopLeft();
CPoint ptWnd( point + ptDevOffset );

UINT nFirstHT = HTCLIENT;
INT nCountOfNcButtons = NcButtons_GetCount();
	for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
	{
		CExtBarNcAreaButton * pBtn =
			NcButtons_GetAt( nBtn );
		if( !pBtn->OnQueryVisibility() )
			continue;
		if( pBtn->m_bHover || pBtn->m_bPushed )
			bNeedRepaint = true;
		UINT nHT =
			pBtn->OnNcAreaHitTest( ptWnd );
		if( nHT != HTCLIENT )
		{
			// TO FIX: ASSERT( nFirstHT == HTCLIENT );
			nFirstHT = nHT;
			bNeedRepaint = true;
		}
	} // for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )

	if( bNeedRepaint )
	{
		// //_RecalcNcArea();
		// SendMessage( WM_NCPAINT );
		// GetParent()->SendMessage( WM_NCPAINT );
		pWndNcRepaint->SendMessage( WM_NCPAINT );
	}
	
	return nFirstHT;
}

UINT CExtControlBar::OnNcHitTest(CPoint point) 
{
UINT nHT = NcButtons_HitTest( point );
	if( nHT != HTCLIENT )
	{
		m_bNcAreaBtnTracking = true;
		return nHT;
	}

	if( m_bFixedMode || IsFloating() )
		return HTCLIENT;

CRect rcBarWnd;
	GetWindowRect( &rcBarWnd );
CPoint ptDevOffset = -rcBarWnd.TopLeft();
CRect rcBarClient;
	GetClientRect( &rcBarClient );
	ClientToScreen( &rcBarClient );
	if( rcBarWnd == rcBarClient )
		return HTCLIENT;
//	rcBarWnd.OffsetRect( ptDevOffset );
//	rcBarClient.OffsetRect( ptDevOffset );

	point += ptDevOffset;

bool bHorz = IsDockedHorizontally();
	if( !m_rcRowResize.IsRectEmpty()
		&& m_rcRowResize.PtInRect( point )
		)
	{
		return bHorz ? HTBOTTOM : HTRIGHT;
	}	
	if( !m_rcRowRecalc.IsRectEmpty()
		&& m_rcRowRecalc.PtInRect( point )
		)
	{
		if( _IsLastBarInRow( true ) )
			return HTCLIENT;
		if( !_RowRecalcing_IsBottomEnabled() )
			return HTCLIENT;
		return bHorz ? HTRIGHT : HTBOTTOM;
	}	
	if( !m_rcRowRecalcUp.IsRectEmpty()
		&& m_rcRowRecalcUp.PtInRect( point )
		)
	{
		if( _IsFirstBarInRow( true ) )
			return HTCLIENT;
		if( !_RowRecalcing_IsTopEnabled() )
			return HTCLIENT;
		return bHorz ? HTLEFT : HTTOP;
	}	
	return HTCLIENT;
}

void CExtControlBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if( IsFloating() )
		return;

CRect & rcClientLoc =
		reinterpret_cast < CRect & > ( lpncsp->rgrc[0] );
CRect rcDevOrg( rcClientLoc );

	m_rcGrip.SetRectEmpty();
	m_rcRowResize.SetRectEmpty();
	m_rcRowRecalc.SetRectEmpty();
	m_rcRowRecalcUp.SetRectEmpty();

	m_nMinHW = m_nSeparatorWidth;
	m_nMinVH = m_nSeparatorHeight;

bool bGripperAtTop = false;
bool bGripperExists = IsBarWithGripper( &bGripperAtTop );
	
CPoint ptGripOffset( 0, 0 );
UINT nDockBarDlgCtrlID = GetSafeDockBarDlgCtrlID();

	if( !m_bFixedMode )
	{
		MfcControlBarVector_t vBars;
		_GetRowBars( vBars );
		int nCountOfnBars = vBars.GetSize();
		INT nLastFixedIdx = -1;
		CExtControlBar * pLastExtBar = NULL;
		bool bAddTopRecalc = false;
		for( INT nBar = 0; nBar < nCountOfnBars ; nBar++ )
		{
			CControlBar * pBar = vBars[ nBar ];
			ASSERT_VALID( pBar );
			if( !pBar->IsVisible() )
				continue;
			CExtControlBar * pExtBar =
				DYNAMIC_DOWNCAST( CExtControlBar, pBar );
			if( pExtBar != NULL && pExtBar->m_bFixedMode )
				pExtBar = NULL;
			if( pExtBar != NULL )
			{
				if( pExtBar == this
					&& pLastExtBar != NULL
					&& nLastFixedIdx == (nBar-1)
					)
				{
					bAddTopRecalc = true;
					break;
				}
				pLastExtBar = pExtBar;
			}
			else
				nLastFixedIdx = nBar;
		} // for( INT nBar = 0; nBar < nCountOfnBars ; nBar++ )
		switch( nDockBarDlgCtrlID )
		{
		case AFX_IDW_DOCKBAR_TOP:
			m_rcRowRecalc = rcClientLoc;
			m_rcRowRecalc.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
			m_rcRowRecalc.left = m_rcRowRecalc.right - m_nSeparatorWidth;
			rcClientLoc.right -= m_nSeparatorWidth;

			rcClientLoc.DeflateRect( 0, 0, 0, m_nSeparatorHeight );
			
			m_rcRowResize = rcClientLoc;
			m_rcRowResize.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
			m_rcRowResize.top = m_rcRowResize.bottom;
			m_rcRowResize.bottom += m_nSeparatorHeight;
			m_rcRowResize.right = m_rcRowResize.left + rcDevOrg.Width();

			if( bAddTopRecalc )
			{
				m_rcRowRecalcUp = rcClientLoc;
				m_rcRowRecalcUp.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowRecalcUp.right = m_rcRowRecalcUp.left + m_nSeparatorWidth;
				if( bGripperExists )
					ptGripOffset.x += m_nSeparatorWidth;
				rcClientLoc.DeflateRect( m_nSeparatorWidth, 0, 0, 0 );
			} // if( bAddTopRecalc )
		break;
		case AFX_IDW_DOCKBAR_BOTTOM:
			m_rcRowRecalc = rcClientLoc;
			m_rcRowRecalc.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
			m_rcRowRecalc.left = m_rcRowRecalc.right - m_nSeparatorWidth;
			rcClientLoc.right -= m_nSeparatorWidth;

			rcClientLoc.DeflateRect( 0, m_nSeparatorHeight, 0, 0 );
			
			m_rcRowResize = rcClientLoc;
			m_rcRowResize.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
			m_rcRowResize.bottom = m_rcRowResize.top + m_nSeparatorHeight;
			m_rcRowResize.right = m_rcRowResize.left + rcDevOrg.Width();
			if( bGripperExists )
				ptGripOffset.y += m_nSeparatorHeight;

			if( bAddTopRecalc )
			{
				m_rcRowRecalcUp = rcClientLoc;
				m_rcRowRecalcUp.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowRecalcUp.right = m_rcRowRecalcUp.left + m_nSeparatorWidth;
				if( bGripperExists )
					ptGripOffset.x += m_nSeparatorWidth;
				rcClientLoc.DeflateRect( m_nSeparatorWidth, 0, 0, 0 );
			} // if( bAddTopRecalc )
		break;
		case AFX_IDW_DOCKBAR_LEFT:
			m_rcRowRecalc = rcClientLoc;
			m_rcRowRecalc.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
			m_rcRowRecalc.top = m_rcRowRecalc.bottom - m_nSeparatorHeight;

			rcClientLoc.DeflateRect( 0, 0, m_nSeparatorWidth, m_nSeparatorHeight );
			
			m_rcRowResize = rcClientLoc;
			m_rcRowResize.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
			m_rcRowResize.left = m_rcRowResize.right;
			m_rcRowResize.right += m_nSeparatorWidth;
			m_rcRowResize.bottom = m_rcRowResize.top + rcDevOrg.Height();

			if( bAddTopRecalc )
			{
				m_rcRowRecalcUp = rcClientLoc;
				m_rcRowRecalcUp.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowRecalcUp.bottom = m_rcRowRecalcUp.top + m_nSeparatorWidth;
				if( bGripperExists )
					ptGripOffset.y += m_nSeparatorHeight;
				rcClientLoc.DeflateRect( 0, m_nSeparatorHeight, 0, 0 );
			} // if( bAddTopRecalc )
		break;
		case AFX_IDW_DOCKBAR_RIGHT:
			m_rcRowRecalc = rcClientLoc;
			m_rcRowRecalc.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
			m_rcRowRecalc.top = m_rcRowRecalc.bottom - m_nSeparatorHeight;

			rcClientLoc.DeflateRect( m_nSeparatorWidth, 0, 0, m_nSeparatorHeight );
			
			m_rcRowResize = rcClientLoc;
			m_rcRowResize.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
			m_rcRowResize.right = m_rcRowResize.left + m_nSeparatorWidth;
			m_rcRowResize.bottom = m_rcRowResize.top + rcDevOrg.Height();
			if( bGripperExists )
				ptGripOffset.x += m_nSeparatorWidth;

			if( bAddTopRecalc )
			{
				m_rcRowRecalcUp = rcClientLoc;
				m_rcRowRecalcUp.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
				m_rcRowRecalcUp.bottom = m_rcRowRecalcUp.top + m_nSeparatorWidth;
				if( bGripperExists )
					ptGripOffset.y += m_nSeparatorHeight;
				rcClientLoc.DeflateRect( 0, m_nSeparatorHeight, 0, 0 );
			} // if( bAddTopRecalc )
		break;
		} // switch( nDockBarDlgCtrlID )
	} // if( !m_bFixedMode )
	else
	{
		if( IsDockedVertically() )
			rcClientLoc.DeflateRect(
				__TB_LINE_OFFSET/4,
				__TB_LINE_OFFSET/2
				);
		else
			rcClientLoc.DeflateRect(
				__TB_LINE_OFFSET/2,
				__TB_LINE_OFFSET/4
				);
	} // else from if( !m_bFixedMode )

	if( bGripperExists )
	{
		m_rcGrip = rcClientLoc;
		m_rcGrip.OffsetRect( -rcClientLoc.left, -rcClientLoc.top );
		if( bGripperAtTop )
		{
			m_rcGrip.bottom = m_rcGrip.top + m_nGripHeightAtTop;
			m_nMinVH += m_rcGrip.Height();
			m_rcGrip.DeflateRect( __BAR_NC_GAP_X, 0 );
		} // if( bGripperAtTop )
		else
		{
			m_rcGrip.right = m_rcGrip.left + m_nGripHeightAtTop;
			m_nMinHW += m_rcGrip.Width();
			m_rcGrip.DeflateRect( 0, __BAR_NC_GAP_Y );
		} // else from if( bGripperAtTop )
		m_rcGrip.OffsetRect( ptGripOffset );

		m_rcGrip.DeflateRect(
			(nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_LEFT)   ? __BAR_NC_GAP_X : 0,
			(nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_TOP)    ? __BAR_NC_GAP_X : 0,
			(nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_RIGHT)  ? __BAR_NC_GAP_X : 0,
			(nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_BOTTOM) ? __BAR_NC_GAP_X : 0
			);
	} // if( bGripperExists )

	OnNcAreaButtonsReinitialize();
	OnNcAreaButtonsReposition();

	rcClientLoc.DeflateRect(
		bGripperAtTop ? 0: m_nGripWidthAtLeft+__BAR_NC_GAP_X,
		bGripperAtTop ? m_nGripHeightAtTop+__BAR_NC_GAP_Y : 0,
		0,
		0
		);

	rcClientLoc.DeflateRect( __BAR_NC_GAP_X, __BAR_NC_GAP_Y );

	if( rcClientLoc.bottom < rcClientLoc.top )
		rcClientLoc.bottom = rcClientLoc.top;
	if( rcClientLoc.right < rcClientLoc.left )
		rcClientLoc.right = rcClientLoc.left;
}

void CExtControlBar::OnNcPaint() 
{
#if (defined __DEBUG_PAINTING_AREAS__)

CRect rcBarWnd, rcBarClient;
	GetWindowRect( &rcBarWnd );
	GetClientRect( &rcBarClient );
	ClientToScreen( &rcBarClient );
	if( rcBarWnd == rcBarClient )
		return;
CPoint ptDevOffset = -rcBarWnd.TopLeft();
	rcBarWnd.OffsetRect( ptDevOffset );
	rcBarClient.OffsetRect( ptDevOffset );

CWindowDC dcBarWnd( this );
	ASSERT( dcBarWnd.GetSafeHdc() != NULL );
	dcBarWnd.ExcludeClipRect( &rcBarClient );

CDC & dc = dcBarWnd;
	dc.FillSolidRect( &rcBarWnd, ::GetSysColor(COLOR_3DFACE) );

bool bHorz = IsDockedHorizontally();

bool bGripperAtTop = false;
bool bGripperExists = IsBarWithGripper( &bGripperAtTop );

	if( bGripperExists )
	{
		ASSERT( !m_rcGrip.IsRectEmpty() );
		CString sWndText;
		GetWindowText( sWndText );
		dc.FillSolidRect( &m_rcGrip, ::GetSysColor(COLOR_HIGHLIGHT) );
		if( !sWndText.IsEmpty() )
		{
			CRect rcGripText( m_rcGrip );
			rcGripText.DeflateRect( 2, 2 );
			HFONT hOldFont = (HFONT)
				::SelectObject(
					dc.GetSafeHdc(),
					::GetStockObject(DEFAULT_GUI_FONT)
					);
			COLORREF clrTextOld =
				dc.SetTextColor( ::GetSysColor(COLOR_HIGHLIGHTTEXT) );
			int nOldBkMode = dc.SetBkMode( TRANSPARENT );
			dc.DrawText(
				sWndText,
				&rcGripText,
				DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS
				);
			dc.SetBkMode( nOldBkMode );
			dc.SetTextColor( clrTextOld );
			::SelectObject(
				dc.GetSafeHdc(),
				hOldFont
				);
		}
	} // if( bGripperExists )

	if( !m_rcRowResize.IsRectEmpty() )
		dc.FillSolidRect( &m_rcRowResize, RGB(255,0,0) );
	if( !m_rcRowRecalc.IsRectEmpty() )
		dc.FillSolidRect( &m_rcRowRecalc, RGB(0,255,0) );
	if( !m_rcRowRecalcUp.IsRectEmpty() )
		dc.FillSolidRect( &m_rcRowRecalcUp, RGB(0,0,255) );

	if( IsBarWithGripper() && !IsFloating() )
		NcButtons_Paint( dc );

#else // __DEBUG_PAINTING_AREAS__

CWindowDC dc( this );

CRect rcClient, rcBar;
	GetClientRect( rcClient );
	ClientToScreen( rcClient );
	GetWindowRect( rcBar );
	rcClient.OffsetRect( -rcBar.TopLeft() );
	rcBar.OffsetRect( -rcBar.TopLeft() );
	dc.ExcludeClipRect(rcClient);
CRect rcDraw = rcBar;

CExtMemoryDC mdc( &dc, &rcBar );
    
	g_PaintManager->PaintDockingFrame(
		mdc,
		rcBar,
		rcClient,
		IsFloating() ? true : false,
		!m_bFixedMode
		);

bool bGripperAtTop = false;
	if( IsBarWithGripper(&bGripperAtTop) && !IsFloating() )
	{
		CRect rcGrip( m_rcGrip );
		bool bHorz = IsDockedHorizontally();

		CRect rcText( rcGrip );
		if( bHorz && !bGripperAtTop )
		{
			rcText.DeflateRect( 0, 2 );
			rcText.OffsetRect( -2, 0 );
		}
		else
		{
			rcText.DeflateRect( 2, 0 );
		}

		INT nCountOfNcButtons = NcButtons_GetCount();
		if( nCountOfNcButtons > 0 )
		{
			CExtBarNcAreaButton * pBtn =
				NcButtons_GetAt( nCountOfNcButtons - 1 );
			const CRect & rcBtn = *pBtn;
			if( bHorz && !bGripperAtTop )
			{
				rcText.top = rcBtn.bottom + 2;
				if( rcText.top > rcText.bottom )
					rcText.top = rcText.bottom;
			}
			else
			{
				rcText.right = rcBtn.left - 2;
				if( rcText.left > rcText.right )
					rcText.left = rcText.right;
			}
		} // if( nCountOfNcButtons > 0 )

		CString sCaption;
		GetWindowText( sCaption );
		
		CRect rcExclude;
		// setup borders clipping for buttons
		rcExclude.SetRect(
			rcBar.left,
			rcBar.top,
			rcGrip.left,
			rcBar.bottom
			);
		mdc.ExcludeClipRect( &rcExclude );
		rcExclude.SetRect(
			rcBar.left,
			rcGrip.bottom,
			rcBar.right,
			rcBar.bottom
			);
		mdc.ExcludeClipRect( &rcExclude );

		g_PaintManager->PaintGripper(
			mdc,
			rcGrip,
			rcText,
			m_bWindowActive,
			false,
			bHorz && !bGripperAtTop,
			!m_bFixedMode,
			sCaption.IsEmpty() ? NULL : (LPCTSTR)sCaption
			);

		mdc.SelectClipRgn( NULL );
		mdc.ExcludeClipRect( &rcClient );

		// setup borders clipping for buttons
		rcExclude.SetRect(
			rcBar.left,
			rcBar.top,
			rcGrip.left,
			rcBar.bottom
			);
		mdc.ExcludeClipRect( &rcExclude );
		rcExclude.SetRect(
			rcBar.left,
			rcGrip.bottom,
			rcBar.right,
			rcBar.bottom
			);
		mdc.ExcludeClipRect( &rcExclude );
		mdc.ExcludeClipRect( &rcClient );

		NcButtons_Paint( mdc );

		mdc.SelectClipRgn( NULL );
	} // if( IsBarWithGripper(&bGripperAtTop) && !IsFloating() )

	mdc.__Flush();
	ReleaseDC( &dc );

#endif // ! __DEBUG_PAINTING_AREAS__
}

void CExtControlBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
    CControlBar::OnSettingChange(uFlags, lpszSection);
	g_PaintManager->SyncSysColors();
	g_PaintManager->InitTranslatedColors();
}

void CExtControlBar::OnSysColorChange() 
{
	CControlBar::OnSysColorChange();
	g_PaintManager->SyncSysColors();
	g_PaintManager->InitTranslatedColors();
}

void CExtControlBar::OnCancelMode() 
{
	if( m_bRowResizing )
		_RowResizingStop();
	if( m_bRowRecalcing )
		_RowRecalcingStop();
	if( m_bDragging )
		_DraggingStop();
	CExtPopupMenuWnd::CancelMenuTracking();
	ASSERT( !CExtPopupMenuWnd::IsMenuTracking() );
	CControlBar::OnCancelMode();
}

void CExtControlBar::OnCaptureChanged(CWnd *pWnd) 
{
	if( m_bRowResizing )
		_RowResizingStop();
	if( m_bRowRecalcing )
		_RowRecalcingStop();
//	if( m_bDragging )
//		_DraggingStop();
	CControlBar::OnCaptureChanged(pWnd);
}

void CExtControlBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	_SetCursor( point );

	if( m_bNcAreaBtnTracking )
		return;

	if( m_bRowResizing )
	{
		_RowResizingUpdateState();
		return;
	}
	if( m_bRowRecalcing )
	{
		_RowRecalcingUpdateState();
		return;
	}
	if( m_bDragging )
	{
		CPoint ptScreen( point );
		ClientToScreen( &ptScreen );
		_DraggingUpdateState( ptScreen );
		return;
	}
	
	if( IsFloating()
		&&
		CExtMiniDockFrameWnd::g_bAutoMakeTopmostOnMouseEvent
		&& (!CExtPopupMenuWnd::IsMenuTracking())
		)
		GetParentFrame()->BringWindowToTop();

	CControlBar::OnMouseMove(nFlags, point);
}

bool CExtControlBar::_RowRecalcing_IsBottomEnabled()
{
	// prev or next extbar should not be minimized
ExtControlBarVector_t vBars;
	_GetRowExtBars( vBars );
INT nCountOfBars = vBars.GetSize();
bool bThisPassed = false;
CExtControlBar * pBarPrev = NULL, * pBarNext = NULL;
	for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	{
		CExtControlBar * pBar = vBars[ nBar ];
		ASSERT_VALID( pBar );
		if( !pBar->IsVisible() )
			continue;
		if( !bThisPassed )
			pBarPrev = pBar;
		else
		{
			pBarNext = pBar;
			break;
		}
		if( pBar == this )
		{
			bThisPassed = true;
			continue;
		}
	} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
bool bMinimizedPrev = true, bMinimizedNext = true;
	if( pBarPrev != NULL )
		bMinimizedPrev = pBarPrev->IsMinimizedOnRow();
	if( pBarNext != NULL )
		bMinimizedNext = pBarNext->IsMinimizedOnRow();
	if( bMinimizedPrev && bMinimizedNext )
		return false;
	return true;
}

bool CExtControlBar::_RowRecalcing_IsTopEnabled()
{
	// this or next extbar should not be minimized
ExtControlBarVector_t vBars;
	_GetRowExtBars( vBars );
INT nCountOfBars = vBars.GetSize();
CExtControlBar * pBarNext = NULL;
	for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	{
		CExtControlBar * pBar = vBars[ nCountOfBars - nBar - 1 ];
		ASSERT_VALID( pBar );
		if( !pBar->IsVisible() )
			continue;
		if( pBar = this )
			break;
		pBarNext = pBar;
	} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
bool bMinimizedNext = true;
	if( pBarNext != NULL )
		bMinimizedNext = pBarNext->IsMinimizedOnRow();
	if( bMinimizedNext && IsMinimizedOnRow() )
		return false;
	return true;
}

void CExtControlBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	if( m_bFixedMode || IsFloating() )
		return;

	switch( nHitTest )
	{
	case HTLEFT:
	case HTBOTTOM:
	break; // case HTLEFT & HTBOTTOM
	return;
	} // switch( nHitTest )

	ASSERT( !m_bRowRecalcing );
	ASSERT( !m_bRowResizing );

CRect rcBarWnd, rcBarClient;
	GetWindowRect( &rcBarWnd );
	GetClientRect( &rcBarClient );
	ClientToScreen( &rcBarClient );
	if( rcBarWnd == rcBarClient )
		return;
CPoint ptDevOffset = -rcBarWnd.TopLeft();
//	rcBarWnd.OffsetRect( ptDevOffset );
//	rcBarClient.OffsetRect( ptDevOffset );

	point += ptDevOffset;

bool bHorz = IsDockedHorizontally();
	if( !m_rcRowResize.IsRectEmpty()
		&& m_rcRowResize.PtInRect( point )
		)
	{
		ASSERT( (INT)nHitTest == (bHorz ? HTBOTTOM : HTRIGHT) );
		_RowResizingStart();
		return;
	}	
	if( !m_rcRowRecalc.IsRectEmpty()
		&& m_rcRowRecalc.PtInRect( point )
		)
	{
		ASSERT( !_IsLastBarInRow( true ) );
		ASSERT( (INT)nHitTest == (bHorz ? HTRIGHT : HTBOTTOM) );
		m_bTopRecalcing = false;
		
		if( !_RowRecalcing_IsBottomEnabled() )
			return;
		
		_RowRecalcingStart();
		return;
	}	
	if( !m_rcRowRecalcUp.IsRectEmpty()
		&& m_rcRowRecalcUp.PtInRect( point )
		)
	{
		ASSERT( !_IsFirstBarInRow( true ) );
		ASSERT( (INT)nHitTest == (bHorz ? HTLEFT : HTTOP) );

		if( !_RowRecalcing_IsTopEnabled() )
			return;
		
		m_bTopRecalcing = true;
		_RowRecalcingStart();
		return;
	}	
}

void CExtControlBar::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	if( m_bNcAreaBtnTracking )
	{
		CRect rcBarWnd;
		GetWindowRect( &rcBarWnd );
		CPoint ptDevOffset = -rcBarWnd.TopLeft();
		CPoint ptWnd( point + ptDevOffset );
		INT nCountOfNcButtons = NcButtons_GetCount();
		for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
		{
			CExtBarNcAreaButton * pBtn =
				NcButtons_GetAt( nBtn );
			if( !pBtn->OnQueryVisibility() )
				continue;
			if( pBtn->OnNcAreaClicked( ptWnd ) )
				return;
		} // for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
		return;
	}

	if( m_bRowResizing )
	{
		_RowResizingStop();
		return;
	}
	
	if( m_bRowRecalcing )
	{
		_RowRecalcingStop();
		return;
	}
	
	if( m_bDragging )
		return;

	CControlBar::OnNcLButtonUp(nHitTest, point);
}

void CExtControlBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if( m_pDockBar == NULL || CExtPopupMenuWnd::IsMenuTracking() )
	{
        CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	// start dragging
	ASSERT_VALID( m_pDockBar );
    ASSERT( m_pDockContext != NULL );

    ClientToScreen( &point );
	
bool bExtendedDragging = IsShowContentWhenDragging();
	if( !bExtendedDragging )
	{
		m_pDockContext->StartDrag( point );
		return;
	}

	if( m_pDockBar != NULL && (!IsFloating()) )
	{
		if( !m_pDockBar->IsKindOf(
				RUNTIME_CLASS(CExtDockBar)
				)
			)
			bExtendedDragging = FALSE;
	}
	_DraggingStart( point );
}

void CExtControlBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_bRowResizing )
		_RowResizingStop();
	if( m_bRowRecalcing )
		_RowRecalcingStop();
	if( m_bDragging )
		_DraggingStop();
	CControlBar::OnLButtonUp(nFlags, point);
}

void CExtControlBar::OnRButtonUp(UINT nFlags, CPoint point) 
{
//	if( m_bRowResizing )
//		_RowResizingStop();
//	if( m_bRowRecalcing )
//		_RowRecalcingStop();
//	if( m_bDragging )
//		_DraggingStop();
//	CControlBar::OnRButtonUp(nFlags, point);

	_ContextMenuTrack();
}

void CExtControlBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if( m_pDockBar == NULL )
	{
        CWnd::OnLButtonDblClk(nFlags, point);
		return;
	}

	ASSERT_VALID( m_pDockBar );
	ASSERT( m_pDockContext != NULL );
	m_pDockContext->ToggleDocking();
}

void CExtControlBar::_ContextMenuTrack()
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

bool CExtControlBar::IsShowContentWhenDragging() const
{
//BOOL bDragShowContent = FALSE;
//    ::SystemParametersInfo(
//		SPI_GETDRAGFULLWINDOWS,
//		0,
//        &bDragShowContent,
//		0
//		);
//	return bDragShowContent ? true : false;

//	if( IsKindOf(RUNTIME_CLASS(CExtToolControlBar)) )
//		return true;
//	return false;

//	if( CExtPopupMenuWnd::IsKeyPressed(VK_CONTROL) )
//		return false;

	return true;
}

void CExtControlBar::_RowResizingStart()
{
	ASSERT( !m_bRowRecalcing );
	ASSERT( !m_bRowResizing );
	m_bRowResizing = true;
	SetCapture();
	ASSERT_VALID( m_pDockSite );
	ASSERT( !IsFloating() );
	m_nMetricOffset = IsDockedHorizontally() ?
		m_sizeDockedH.cy : m_sizeDockedV.cx;
	if( !IsShowContentWhenDragging() )
	{
		CPoint ptCursorScreen;
		VERIFY( ::GetCursorPos(&ptCursorScreen) );
		m_nTrackerOffset =
			IsDockedHorizontally()
				? ptCursorScreen.y : ptCursorScreen.x;
		_DrawResizingTracker();
	} // if( !IsShowContentWhenDragging() )
}

void CExtControlBar::_RowResizingStop()
{
	m_bRowResizing = false;
	if( GetCapture() == this )
		ReleaseCapture();
	ASSERT_VALID( m_pDockSite );
	ASSERT( !IsFloating() );

	if( !IsShowContentWhenDragging() )
	{
		_DrawResizingTracker();

		bool bHorz = IsDockedHorizontally();
		if(	m_nMetricOffset !=
				(bHorz ? m_sizeDockedH.cy : m_sizeDockedV.cx)
			)
		{
			ExtControlBarVector_t vBars;
			_GetRowExtBars( vBars );
			INT nCountOfBars = vBars.GetSize();
			ASSERT( nCountOfBars > 0 );
			for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
			{
				CExtControlBar * pBar = vBars[ nBar ];
				ASSERT_VALID( pBar );
				if( !pBar->IsVisible() )
					continue;
				if( bHorz )
					pBar->m_sizeDockedH.cy = m_nMetricOffset;
				else
					pBar->m_sizeDockedV.cx = m_nMetricOffset;
			} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
		}
		
		m_pDockSite->DelayRecalcLayout();
	} // if( !IsShowContentWhenDragging() )
}

void CExtControlBar::_RowResizingUpdateState()
{
	ASSERT( m_bRowResizing );
	ASSERT_VALID( m_pDockSite );

CRect rcDockSite;
	m_pDockSite->GetWindowRect( &rcDockSite );
CSize sizeDockMax =
		rcDockSite.Size()
		- CSize(
			_CalcDesiredMinHW(),
			_CalcDesiredMinVH()
			);
	if( sizeDockMax.cx <= 0 || sizeDockMax.cy <= 0 )
		return;

CPoint ptCursorScreen;
	VERIFY( ::GetCursorPos(&ptCursorScreen) );
CPoint ptCursor( ptCursorScreen );

	//ScreenToClient( &ptCursor );
CRect rcBarWnd;
	GetWindowRect( &rcBarWnd );
CPoint ptDevOffset = -rcBarWnd.TopLeft();
	ptCursor += ptDevOffset;

bool bHorz = IsDockedHorizontally();
	if( bHorz )
	{
		bool bTop = IsDockedAtTop();
		m_nMetricOffset =
			max(
				_CalcDesiredMinVH()*2,
				bTop
					? (ptCursor.y + m_rcRowResize.Height()/2)
					: (m_sizeDockedH.cy - ptCursor.y + m_rcRowResize.Height()/2)
				);
		if( m_nMetricOffset > sizeDockMax.cy )
			m_nMetricOffset = sizeDockMax.cy;
		if( m_nMetricOffset == m_sizeDockedH.cy )
			return;
	} // if( bHorz )
	else
	{
		bool bLeft = IsDockedAtLeft();
		m_nMetricOffset =
			max(
				_CalcDesiredMinHW()*2,
				bLeft
					? (ptCursor.x + m_rcRowResize.Width()/2)
					: (m_sizeDockedV.cx - ptCursor.x + m_rcRowResize.Width()/2 )
				);
		if( m_nMetricOffset > sizeDockMax.cx )
			m_nMetricOffset = sizeDockMax.cx;
		if( m_nMetricOffset == m_sizeDockedV.cx )
			return;
	} // else from if( bHorz )

bool bShowContent = IsShowContentWhenDragging();
	if( bShowContent )
	{

		ExtControlBarVector_t vBars;
		_GetRowExtBars( vBars );
		INT nCountOfBars = vBars.GetSize();
		ASSERT( nCountOfBars > 0 );
		for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
		{
			CExtControlBar * pBar = vBars[ nBar ];
			ASSERT_VALID( pBar );
			if( bHorz )
				pBar->m_sizeDockedH.cy = m_nMetricOffset;
			else
				pBar->m_sizeDockedV.cx = m_nMetricOffset;
		} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
		
		m_pDockSite->DelayRecalcLayout();
		return;
	} // if( bShowContent )

	_DrawResizingTracker();

	m_nTrackerOffset =
		bHorz ? ptCursorScreen.y : ptCursorScreen.x;
	_DrawResizingTracker();
}

void CExtControlBar::_RowRecalcingStart()
{
	ASSERT( !m_bRowRecalcing );
	ASSERT( !m_bRowResizing );
	m_bRowRecalcing = true;
    SetCapture();
	ASSERT_VALID( m_pDockSite );
	if( !IsShowContentWhenDragging() )
	{
		CPoint ptCursorScreen;
		VERIFY( ::GetCursorPos(&ptCursorScreen) );
		m_nTrackerOffset =
			IsDockedHorizontally()
				? ptCursorScreen.x
				: ptCursorScreen.y;
		m_nMetricOffset = 0;
		_DrawRecalcingTracker();
	} // if( !IsShowContentWhenDragging() )
}

void CExtControlBar::_RowRecalcingStop()
{
	m_bRowRecalcing = false;
	if( GetCapture() == this )
		ReleaseCapture();
	ASSERT_VALID( m_pDockSite );
	if( !IsShowContentWhenDragging() )
	{
		_DrawRecalcingTracker();

		if( m_nMetricOffset != 0 )
		{
			bool bHorz = IsDockedHorizontally();
			ExtControlBarVector_t vBars;
			_GetRowExtBars( vBars );
			INT nCountOfBars = vBars.GetSize();
			ASSERT( nCountOfBars > 1 );
			bool bThisPassed = false;
			for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
			{
				CExtControlBar * pBar =
					vBars[
						m_bTopRecalcing
							? (nCountOfBars - nBar - 1)
							: nBar
						];
				ASSERT_VALID( pBar );
				if( pBar == this )
				{
					bThisPassed = true;
					continue;
				}
				if( !bThisPassed )
					continue;
				if( bHorz )
				{
					pBar->m_sizeDockedH.cx += m_nMetricOffset;
					if( pBar->m_sizeDockedH.cx < pBar->_CalcDesiredMinHW() )
					{
						m_sizeDockedH.cx -=
							pBar->_CalcDesiredMinHW() - pBar->m_sizeDockedH.cx;
						pBar->m_sizeDockedH.cx = pBar->_CalcDesiredMinHW();
					}
				} // if( bHorz )
				else
				{
					pBar->m_sizeDockedV.cy += m_nMetricOffset;
					if( pBar->m_sizeDockedV.cy < pBar->_CalcDesiredMinVH() )
					{
						m_sizeDockedV.cy -=
							pBar->_CalcDesiredMinVH() - pBar->m_sizeDockedV.cy;
						pBar->m_sizeDockedV.cy = pBar->_CalcDesiredMinVH();
					}
				} // else from if( bHorz )
			} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )

			if( bHorz )
			{
				m_sizeDockedH.cx -= m_nMetricOffset;
				if( m_sizeDockedH.cx < _CalcDesiredMinHW() )
				{
					m_sizeDockedH.cx +=
						_CalcDesiredMinHW() - m_sizeDockedH.cx;
					m_sizeDockedH.cx = _CalcDesiredMinHW();
				}
			} // if( bHorz )
			else
			{
				m_sizeDockedV.cy -= m_nMetricOffset;
				if( m_sizeDockedV.cy < _CalcDesiredMinVH() )
				{
					m_sizeDockedV.cy +=
						_CalcDesiredMinVH() - m_sizeDockedV.cy;
					m_sizeDockedV.cy = _CalcDesiredMinVH();
				}
			} // else from if( bHorz )

			m_pDockSite->DelayRecalcLayout();
		} // if( m_nMetricOffset != 0 )
	} // if( !IsShowContentWhenDragging() )
}

void CExtControlBar::_RowRecalcingUpdateState()
{
	ASSERT( m_bRowRecalcing );
	ASSERT_VALID( m_pDockBar );
	ASSERT_VALID( m_pDockSite );

CPoint ptCursorScreen;
	VERIFY( ::GetCursorPos(&ptCursorScreen) );
CPoint ptCursor( ptCursorScreen );

	//ScreenToClient( &ptCursor );
CRect rcBarWnd;
	GetWindowRect( &rcBarWnd );
CPoint ptDevOffset = -rcBarWnd.TopLeft();
	ptCursor += ptDevOffset;

bool bSysDragShowContent = IsShowContentWhenDragging();
bool bHorz = IsDockedHorizontally();
	if( bHorz )
	{
		if( m_bTopRecalcing )
		{
			INT nMetric =
				max(
					_CalcDesiredMinHW(),
					(m_sizeDockedH.cx - ptCursor.x + m_rcRowRecalcUp.Width()/2)
					);
			m_nMetricOffset = m_sizeDockedH.cx - nMetric;
			if( m_nMetricOffset == 0 )
				return;
			if( bSysDragShowContent )
				m_sizeDockedH.cx = nMetric;
		} // if( m_bTopRecalcing )
		else
		{
			INT nMetric =
				max(
					_CalcDesiredMinHW(),
					(ptCursor.x + m_rcRowRecalc.Width()/2)
					);
			m_nMetricOffset = m_sizeDockedH.cx - nMetric;
			if( m_nMetricOffset == 0 )
				return;
			if( bSysDragShowContent )
				m_sizeDockedH.cx = nMetric;
		} // else from if( bTopRecalcing )
	} // if( bHorz )
	else
	{
		if( m_bTopRecalcing )
		{
			INT nMetric =
				max(
					_CalcDesiredMinVH(),
					(m_sizeDockedV.cy - ptCursor.y + m_rcRowRecalcUp.Height()/2)
					);
			m_nMetricOffset = m_sizeDockedV.cy - nMetric;
			if( m_nMetricOffset == 0 )
				return;
			if( bSysDragShowContent )
				m_sizeDockedV.cy = nMetric;
		} // if( m_bTopRecalcing )
		else
		{
			INT nMetric =
				max(
					_CalcDesiredMinVH(),
					(ptCursor.y + m_rcRowRecalc.Height()/2)
					);
			m_nMetricOffset = m_sizeDockedV.cy - nMetric;
			if( m_nMetricOffset == 0 )
				return;
			if( bSysDragShowContent )
				m_sizeDockedV.cy = nMetric;
		} // else from if( m_bTopRecalcing )
	} // else from if( bHorz )
	ASSERT( m_nMetricOffset != 0 );

	if( bSysDragShowContent )
	{
		ExtControlBarVector_t vBars;
		_GetRowExtBars( vBars );
		INT nCountOfBars = vBars.GetSize();
		ASSERT( nCountOfBars > 1 );
		bool bThisPassed = false;
		for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
		{
			CExtControlBar * pBar =
				vBars[
					m_bTopRecalcing
						? (nCountOfBars - nBar - 1)
						: nBar
					];
			ASSERT_VALID( pBar );
			if( pBar == this )
			{
				bThisPassed = true;
				continue;
			}
			if( !bThisPassed )
				continue;
			if( bHorz )
			{
				pBar->m_sizeDockedH.cx += m_nMetricOffset;
				if( pBar->m_sizeDockedH.cx < pBar->_CalcDesiredMinHW() )
				{
					m_sizeDockedH.cx -=
						pBar->_CalcDesiredMinHW() - pBar->m_sizeDockedH.cx;
					pBar->m_sizeDockedH.cx = pBar->_CalcDesiredMinHW();
				}
			} // if( bHorz )
			else
			{
				pBar->m_sizeDockedV.cy += m_nMetricOffset;
				if( pBar->m_sizeDockedV.cy < pBar->_CalcDesiredMinVH() )
				{
					m_sizeDockedV.cy -=
						pBar->_CalcDesiredMinVH() - pBar->m_sizeDockedV.cy;
					pBar->m_sizeDockedV.cy = pBar->_CalcDesiredMinVH();
				}
			} // else from if( bHorz )
			break;
		} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )

		m_pDockSite->DelayRecalcLayout();
		return;
	} // if( bSysDragShowContent )

	ASSERT_VALID( m_pDockBar );
CRect rcDockBarWnd;
	m_pDockBar->GetWindowRect( &rcDockBarWnd );

INT nMinTrackerOffset = bHorz ?
		rcDockBarWnd.left : rcDockBarWnd.top;

	if( m_bTopRecalcing )
	{
		MfcControlBarVector_t vBars;
		_GetRowBars( vBars );
		INT nCountOfBars = vBars.GetSize();
		for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
		{
			CControlBar * pBar = vBars[ nBar ];
			ASSERT_VALID( pBar );
			if( !pBar->IsVisible() )
				continue;
			CExtControlBar * pExtBar =
				DYNAMIC_DOWNCAST( CExtControlBar, pBar );
			if( pExtBar != NULL && pExtBar->m_bFixedMode )
				pExtBar = NULL;
			if( pExtBar == NULL )
			{
				CRect rcBarWnd;
				pBar->GetWindowRect( &rcBarWnd );
				nMinTrackerOffset += bHorz ?
					rcBarWnd.Width() : rcBarWnd.Height();
			} // if( pExtBar == NULL )
			else
			{
				nMinTrackerOffset += bHorz
					? pExtBar->_CalcDesiredMinHW()
					: pExtBar->_CalcDesiredMinVH();
			} // else from if( pExtBar == NULL )
			if( pBar == this )
				break;
		} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	} // if( m_bTopRecalcing )
	else
	{
		nMinTrackerOffset = bHorz
			? rcBarWnd.left + _CalcDesiredMinHW()
			: rcBarWnd.top  + _CalcDesiredMinVH()
			;
	} // else from if( m_bTopRecalcing )

INT nMaxTrackerOffset = 0;

	if( m_bTopRecalcing )
	{
		nMaxTrackerOffset = bHorz
			? rcBarWnd.right  - _CalcDesiredMinHW()
			: rcBarWnd.bottom - _CalcDesiredMinVH()
			;
	} // if( m_bTopRecalcing )
	else
	{
		nMaxTrackerOffset = bHorz ?
			rcDockBarWnd.right : rcDockBarWnd.bottom;
		MfcControlBarVector_t vBars;
			_GetRowBars( vBars );
		INT nCountOfBars = vBars.GetSize();
		for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
		{
			CControlBar * pBar = vBars[ nCountOfBars - nBar - 1 ];
			ASSERT_VALID( pBar );
			if( !pBar->IsVisible() )
				continue;
			if( pBar == this )
				break;
			CExtControlBar * pExtBar =
				DYNAMIC_DOWNCAST( CExtControlBar, pBar );
			if( pExtBar != NULL && pExtBar->m_bFixedMode )
				pExtBar = NULL;
			if( pExtBar == NULL )
			{
				CRect rcBarWnd;
				pBar->GetWindowRect( &rcBarWnd );
				nMaxTrackerOffset -= bHorz ?
					rcBarWnd.Width() : rcBarWnd.Height();
			} // if( pExtBar == NULL )
			else
			{
				nMaxTrackerOffset -= bHorz
					? pExtBar->_CalcDesiredMinHW()
					: pExtBar->_CalcDesiredMinVH();
			} // else from if( pExtBar == NULL )
		} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	} // else from if( m_bTopRecalcing )

	_DrawRecalcingTracker();
	m_nTrackerOffset = bHorz
		? ptCursorScreen.x
		: ptCursorScreen.y;
	ASSERT( nMinTrackerOffset <= nMaxTrackerOffset );
	if( m_nTrackerOffset < nMinTrackerOffset )
		m_nTrackerOffset = nMinTrackerOffset;
	if( m_nTrackerOffset > nMaxTrackerOffset )
		m_nTrackerOffset = nMaxTrackerOffset;
	_DrawRecalcingTracker();
}

void CExtControlBar::_DrawResizingTracker()
{
	ASSERT( !IsFloating() );
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );

CRect rcDockBar;
	m_pDockBar->GetWindowRect( &rcDockBar );
	if( rcDockBar.IsRectEmpty() )
		return;

CRect rcTracker( rcDockBar );
	if( IsDockedHorizontally() )
	{
		rcTracker.top =
			m_nTrackerOffset - m_rcRowResize.Height()/2;
		rcTracker.bottom =
			m_nTrackerOffset + m_rcRowResize.Height()/2;
	} // if( IsDockedHorizontally() )
	else
	{
		rcTracker.left =
			m_nTrackerOffset - m_rcRowResize.Width()/2;
		rcTracker.right =
			m_nTrackerOffset + m_rcRowResize.Width()/2;
	} // else from if( IsDockedHorizontally() )

CRect rcDockingFrame;
	m_pDockSite->GetWindowRect( &rcDockingFrame );
	rcTracker.OffsetRect( -rcDockingFrame.TopLeft() );

CDC * pDC = m_pDockSite->GetDCEx(
		NULL, DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE );
CBrush * pBrushOld =
		pDC->SelectObject( CDC::GetHalftoneBrush() );
    pDC->PatBlt(
		rcTracker.left, rcTracker.top,
		rcTracker.Width(), rcTracker.Height(),
		PATINVERT
		);
    pDC->SelectObject(pBrushOld);
    m_pDockSite->ReleaseDC(pDC);
}

void CExtControlBar::_DrawRecalcingTracker()
{
	ASSERT( !IsFloating() );
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );

CRect rcTracker;
	GetWindowRect( &rcTracker );

	if( IsDockedHorizontally() )
	{
		INT nMetric =
			m_bTopRecalcing
				? m_rcRowRecalcUp.Width()/2
				: m_rcRowRecalc.Width()/2;
		rcTracker.left =
			m_nTrackerOffset - nMetric;
		rcTracker.right =
			m_nTrackerOffset + nMetric;
	} // if( IsDockedHorizontally() )
	else
	{
		INT nMetric =
			m_bTopRecalcing
				? m_rcRowRecalcUp.Height()/2
				: m_rcRowRecalc.Height()/2;
		rcTracker.top =
			m_nTrackerOffset - nMetric;
		rcTracker.bottom =
			m_nTrackerOffset + nMetric;
	} // else from if( IsDockedHorizontally() )

CRect rcDockingFrame;
	m_pDockSite->GetWindowRect( &rcDockingFrame );
	rcTracker.OffsetRect( -rcDockingFrame.TopLeft() );

CDC * pDC = m_pDockSite->GetDCEx(
		NULL, DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE );
CBrush * pBrushOld =
		pDC->SelectObject( CDC::GetHalftoneBrush() );
    pDC->PatBlt(
		rcTracker.left, rcTracker.top,
		rcTracker.Width(), rcTracker.Height(),
		PATINVERT
		);
    pDC->SelectObject(pBrushOld);
    m_pDockSite->ReleaseDC(pDC);
}

bool CExtControlBar::IsOleIpObjActive(
	CFrameWnd * pFrameSearch // = NULL
	)
{
	if( pFrameSearch == NULL )
	{
		pFrameSearch =
			DYNAMIC_DOWNCAST( CFrameWnd, AfxGetMainWnd() );
		if( pFrameSearch == NULL )
			return false;
	}
	ASSERT_VALID( pFrameSearch );
bool bOleInplaceItemActivated = false;
CView * pActiveView = pFrameSearch->GetActiveView();
	if( pActiveView != NULL )
	{
		ASSERT_VALID( pActiveView );
		CDocument * pActiveDoc = pActiveView->GetDocument();
		if( pActiveDoc != NULL )
		{
			ASSERT_VALID( pActiveDoc );
			COleDocument * pActiveOleDoc = 
				DYNAMIC_DOWNCAST(COleDocument,pActiveDoc);
			if( pActiveOleDoc != NULL )
			{
				ASSERT_VALID( pActiveOleDoc );
				COleClientItem * pItem =
					pActiveOleDoc->GetInPlaceActiveItem(pActiveView);
				if( pItem != NULL )
					bOleInplaceItemActivated = true;
			} // if( pActiveOleDoc != NULL )
		} // if( pActiveDoc != NULL )
	} // if( pActiveView != NULL )
	return bOleInplaceItemActivated;
}

bool CExtControlBar::SafeDisplayBar()
{
	if( IsVisible() )
		return true;
	if( m_nStateFlags &
		(CControlBar::tempHide | CControlBar::delayHide)
		)
		return false;
CFrameWnd * pFrame =
		_GetDockingFrameImpl();
	ASSERT_VALID( pFrame );
bool bOleInplaceItemActivated =
		CExtControlBar::IsOleIpObjActive( pFrame );
	if( bOleInplaceItemActivated &&
			(m_dwStyle & CBRS_HIDE_INPLACE)
		)
		return false;
	pFrame->ShowControlBar( this, TRUE, FALSE );
	if( !IsVisible() )
	{
		ASSERT( FALSE );
		return false;
	}
	return true;
}

bool CExtControlBar::IsSingleOnRow() const
{
ExtControlBarVector_t vBars;
	(const_cast<CExtControlBar *>(this))->
		_GetRowExtBars(
			vBars
			);
int nCountInRow = vBars.GetSize();
//	ASSERT( nCountInRow > 0 );
	return (nCountInRow == 1) ? TRUE : FALSE;
}

void CExtControlBar::MakeSingleOnRow()
{
CFrameWnd * pFrame = GetDockingFrame();
	ASSERT_VALID( pFrame );
	pFrame->RecalcLayout();
	if( IsDockedHorizontally() )
		m_sizeDockedH = CSize( MAXSHORT, m_sizeDockedH.cy );
	else
		m_sizeDockedV = CSize( m_sizeDockedV.cx, MAXSHORT );
	pFrame->RecalcLayout();
}

bool CExtControlBar::IsMinimizedOnRow() const
{
	if(	IsDockedVertically()
		&& m_sizeDockedV.cy == _CalcDesiredMinVH()
		)
		return true;
	if(	IsDockedHorizontally()
		&& m_sizeDockedH.cx == _CalcDesiredMinHW()
		)
		return true;
	return false;
}

bool CExtControlBar::IsMaximizedOnRow() const
{
	if( IsFloating() )
		return false;
	if(IsMinimizedOnRow())
		return false;
ExtControlBarVector_t vBars;
	(const_cast<CExtControlBar *>(this))->
		_GetRowExtBars(
			vBars
			);
INT nCountOfBars = vBars.GetSize();
	for( int nBar = 0; nBar < nCountOfBars; nBar++ )
	{
		CExtControlBar * pBar = vBars[ nBar ];
		ASSERT( pBar != NULL );
		if( pBar == this )
			continue;
		if( !(pBar->IsMinimizedOnRow()) )
			return false;
	}
	return true;
}

void CExtControlBar::MinimizeOnRow()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );
	ASSERT( GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow( GetSafeHwnd() ) != NULL );

	if( IsFloating () || IsSingleOnRow() )
		return;

ExtControlBarVector_t vBars;
    _GetRowExtBars(vBars);
INT nCountOfBars = vBars.GetSize();
	if( nCountOfBars <= 1 )
		return;
CExtDockBar * pDockBar =
		(CExtDockBar*) m_pDockBar;
	ASSERT( pDockBar != NULL );
BOOL bHorz = IsDockedHorizontally();

INT nCalcExtentMax = 0;
	if( bHorz )
	{
		INT nMin = _CalcDesiredMinHW();
		nCalcExtentMax = m_sizeDockedH.cx - nMin;
		m_sizeDockedH.cx = _CalcDesiredMinHW();
	}
	else
	{
		INT nMin = _CalcDesiredMinVH();
		nCalcExtentMax = m_sizeDockedV.cy - nMin;
		m_sizeDockedV.cy = _CalcDesiredMinVH();
	}

INT nMetric = nCalcExtentMax/(nCountOfBars-1);
	for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	{
		CExtControlBar * pBar = vBars[ nBar ];
		ASSERT_VALID( pBar );
		if( pBar == this )
			continue;
		if( bHorz )
			pBar->m_sizeDockedH.cx -= nMetric;
		else
			pBar->m_sizeDockedV.cy -= nMetric;
		nCalcExtentMax -= nMetric;
	} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	ASSERT( nCalcExtentMax >= 0 && nCalcExtentMax < (nCountOfBars-1) );
	for( nBar = 0; nCalcExtentMax > 0; nBar++, nCalcExtentMax-- )
	{
		CExtControlBar * pBar = vBars[ nBar ];
		ASSERT_VALID( pBar );
		if( pBar == this )
			continue;
		if( bHorz )
			pBar->m_sizeDockedH.cx++;
		else
			pBar->m_sizeDockedV.cy++;
	} // for( nBar = 0; nCalcExtentMax > 0; nBar++, nCalcExtentMax-- )

	m_pDockSite->RecalcLayout();
	
	_GetDockingFrameImpl()->SetFocus();
}

void CExtControlBar::MaximizeOnRow()
{
	ASSERT_VALID( this );
	ASSERT_VALID( m_pDockSite );
	ASSERT_VALID( m_pDockBar );
	ASSERT( GetSafeHwnd() != NULL );
	ASSERT( ::IsWindow( GetSafeHwnd() ) != NULL );

	if( IsFloating () || IsSingleOnRow() )
		return;

ExtControlBarVector_t vBars;
    _GetRowExtBars(vBars);
INT nCountOfBars = vBars.GetSize();
	if( nCountOfBars <= 1 )
		return;
CExtDockBar * pDockBar =
		(CExtDockBar*) m_pDockBar;
	ASSERT( pDockBar != NULL );
BOOL bHorz = IsDockedHorizontally();
INT nCalcExtentMax =
		((FriendlyDockBarHack *)pDockBar)->
			GetCalcExtentMax( bHorz );
INT nMetric = 0;
	for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	{
		CExtControlBar * pBar = vBars[ nBar ];
		ASSERT_VALID( pBar );
		if( pBar == this )
			continue;
		if( bHorz )
		{
			pBar->m_sizeDockedH.cx = pBar->_CalcDesiredMinHW();
			nMetric += pBar->m_sizeDockedH.cx;
		}
		else
		{
			pBar->m_sizeDockedV.cy = pBar->_CalcDesiredMinVH();
			nMetric += pBar->m_sizeDockedV.cy;
		}
	} // for( INT nBar = 0; nBar < nCountOfBars; nBar++ )
	if( bHorz )
		m_sizeDockedH.cx = nCalcExtentMax - nMetric;
	else
		m_sizeDockedV.cy = nCalcExtentMax - nMetric;
	m_pDockSite->RecalcLayout();
}

void CExtControlBar::OnRepositionSingleChild(
	int cx, // = -1, // if negative - get from client area
	int cy, // = -1
	bool bRedraw // = TRUE
	)
{
	if( !m_bReposSingleChildMode )
		return;

    // automatic child resizing - only one child is allowed
CWnd * pWnd = GetWindow( GW_CHILD );
    if( pWnd != NULL
		&& ::IsWindow( pWnd->GetSafeHwnd() )
		)
	{
		CRect rcClient;
		if( cx < 0 || cy < 0 )
			GetClientRect( &rcClient );
		else
			rcClient.SetRect( 0, 0, cx, cy );
		if( IsFloating() )
		{
			CSize sizeFrame(
				::GetSystemMetrics(SM_CXSIZEFRAME), // SM_CXFIXEDFRAME
				::GetSystemMetrics(SM_CYSIZEFRAME)  // SM_CYFIXEDFRAME
				);
			rcClient.DeflateRect( sizeFrame );
		} // if( IsFloating() )
		pWnd->MoveWindow( &rcClient, FALSE);
		ASSERT( pWnd->GetWindow(GW_HWNDNEXT) == NULL );
		//pWnd->ShowWindow( IsMinimizedOnRow() ? SW_HIDE : SW_SHOW );
	}

	if( !bRedraw )
		return;

	//pWnd->
	RedrawWindow(
		NULL,
		NULL,
		RDW_ERASE|RDW_INVALIDATE
			|RDW_INTERNALPAINT
			|RDW_UPDATENOW
			|RDW_ALLCHILDREN
			|RDW_NOFRAME
		);
}

void CExtControlBar::OnSize(UINT nType, int cx, int cy) 
{
	//CControlBar::OnSize(nType, cx, cy);
	nType;
    OnRepositionSingleChild( cx, cy, true );
}

void CExtControlBar::_OnStartDragging()
{
	CExtPopupMenuWnd::CancelMenuTracking();
}

void CExtControlBar::_SetCursor(
	const CPoint & point
	)
{
HCURSOR hCursor = NULL;
	if( m_bDragging || m_bRowResizing || m_bRowRecalcing )
	{
		if( m_bDragging )
		{
			ASSERT( !(m_bRowResizing || m_bRowRecalcing) );
			hCursor = g_hCursorDrag;
			ASSERT( hCursor != NULL );
		} // if( m_bDragging )
		else
		{
			ASSERT( !m_bDragging );
			ASSERT( !IsFloating() );
			if( IsDockedVertically() )
			{
				if( m_bRowRecalcing )
				{
					ASSERT( !m_bRowResizing );
					hCursor = g_hCursorResizeV;
					ASSERT( hCursor != NULL );
				} // if( m_bRowRecalcing )
				else
				{
					ASSERT( !m_bRowRecalcing );
					hCursor = g_hCursorResizeH;
					ASSERT( hCursor != NULL );
				} // else from if( m_bRowRecalcing )
			} // if( IsDockedVertically() )
			else
			{
				if( m_bRowRecalcing )
				{
					ASSERT( !m_bRowResizing );
					hCursor = g_hCursorResizeH;
					ASSERT( hCursor != NULL );
				} // if( m_bRowRecalcing )
				else
				{
					ASSERT( !m_bRowRecalcing );
					hCursor = g_hCursorResizeV;
					ASSERT( hCursor != NULL );
				} // else if( m_bRowRecalcing )
			} // else from if( IsDockedVertically() )
		} // else from if( m_bDragging )
	} // if( m_bDragging || m_bRowResizing || m_bRowRecalcing )
	else
	{

		CRect rcBarWnd;
		GetWindowRect( &rcBarWnd );
		CPoint ptDevOffset = -rcBarWnd.TopLeft();
		CPoint ptWnd( point );
		ClientToScreen( &ptWnd );
		ptWnd += ptDevOffset;
		if( m_rcGrip.PtInRect(ptWnd) )
		{
			INT nCountOfNcButtons = NcButtons_GetCount();
			for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
			{
				CExtBarNcAreaButton * pBtn =
					NcButtons_GetAt( nBtn );
				hCursor =
					pBtn->OnNcAreaQueryCursor( ptWnd );
				if( hCursor != NULL )
					break;
			} // for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
			if( hCursor == NULL )
			{
				hCursor = g_hCursorDrag;
				ASSERT( hCursor != NULL );
			} // if( hCursor == NULL )
		} // if( m_rcGrip.PtInRect(ptWnd) )
	
	} // else if( m_bDragging || m_bRowResizing || m_bRowRecalcing )
	if( hCursor == NULL )
	{
		hCursor = (HCURSOR)
			::GetClassLong( m_hWnd, GCL_HCURSOR );
		ASSERT( hCursor != NULL );
	} // if( hCursor == NULL )
	if( GetCursor() != hCursor )
		::SetCursor( hCursor );
}


CExtControlBar * CExtControlBar::_DraggingGetBar() // return currently dagging controlbar
{
	for( INT iBar=0; iBar<g_AllBars.GetSize(); iBar++ )
	{
		CExtControlBar * pBar = g_AllBars[iBar];
		ASSERT( pBar != NULL );
		if( pBar->m_bDragging )
		{
			ASSERT_VALID( pBar );
			return pBar;
		}
	} // for( INT iBar=0; iBar<g_AllBars.GetSize(); iBar++ )
	return NULL;
}

bool CExtControlBar::_DraggingCancel() // returns true if dragging was really canceled
{
CExtControlBar * pBar = _DraggingGetBar();
	if( pBar == NULL )
		return false;
	pBar->_DraggingStop();
	return true;
}

void CExtControlBar::_DraggingStart(
	const CPoint & point,
	const CPoint & pointOffset // = CPoint( 0, 0 );
	)
{
#ifdef _USRDLL
	// If this is a DLL, need to set up MFC state
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

	ASSERT( !m_bRowRecalcing );
	ASSERT( !m_bRowResizing );
	ASSERT( !m_bDragging );

    ASSERT( m_pDockBar != NULL );
    ASSERT( m_pDockContext != NULL );
    
	_OnStartDragging();
	SetCapture();
	m_bDragging = TRUE;
	m_ptHelperDraggingOffset = pointOffset;
	_DraggingUpdateState( point );

	while( m_bDragging )
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
			case WM_CANCELMODE:
//			case WM_COMMAND: // iC text editor dont want this!
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_NCLBUTTONUP:
			case WM_NCLBUTTONDOWN:
			case WM_NCRBUTTONUP:
			case WM_NCRBUTTONDOWN:
			case WM_CONTEXTMENU:
				bStop = true;
			break;
			default:
				if(	WM_KEYFIRST <= msg.message
					&& msg.message <= WM_KEYLAST
					)
					bStop = true;
				else
				{
					if( GetCapture() != this )
						bStop = true;
					else
					{
						if(	(!CExtPopupMenuWnd::
								IsKeyPressed(VK_LBUTTON))
							|| CExtPopupMenuWnd::
								IsKeyPressed(VK_RBUTTON)
							|| CExtPopupMenuWnd::
								IsKeyPressed(VK_MBUTTON)
							)
							bStop = true;
					}
				}
			break;
			} // switch( msg.message )
			if( bStop )
			{
				_DraggingStop();
				break;
			}
			if( !AfxGetApp()->PumpMessage() )
			{
				PostQuitMessage(0);
				break; // Signal WM_QUIT received
			} // if( !AfxGetApp()->PumpMessage() )
		} // while( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
		if( m_bDragging )
		{
			LONG lIdle = 0;
			while( AfxGetApp()->OnIdle(lIdle++) );
		}
	} // while( m_bDragging )
}

void CExtControlBar::_DraggingStop()
{
	ASSERT( !m_bRowRecalcing );
	ASSERT( !m_bRowResizing );
	//ASSERT( m_bDragging );
//	if( !m_bDragging )
//		return;
	m_bDragging = FALSE;
	if( GetCapture() == this )
		ReleaseCapture();
}

void CExtControlBar::_DraggingUpdateState(
	const CPoint & point
	)
{
	ASSERT( !m_bRowRecalcing );
	ASSERT( !m_bRowResizing );
	ASSERT( m_bDragging );
	ASSERT( CWnd::GetCapture() == this );

int nCyGripper = min( m_rcGrip.Width(), m_rcGrip.Height() );
CSize sizeOffset( nCyGripper, nCyGripper/2 );

CRect wrFloatingBar;
CFrameWnd * pFrame = GetParentFrame();
	ASSERT_VALID( pFrame );
CMiniFrameWnd * pDockingFrame = NULL;
	if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
	{
		ASSERT( IsFloating() );
		pDockingFrame = DYNAMIC_DOWNCAST(CMiniFrameWnd, pFrame);
		ASSERT( pDockingFrame == pFrame );
		ASSERT_VALID( pDockingFrame );
		pDockingFrame->GetWindowRect( &wrFloatingBar );
		pFrame = pDockingFrame->GetParentFrame();

	}
	else
	{
		ASSERT( !IsFloating() );
		GetWindowRect( &wrFloatingBar );
	}
	ASSERT_VALID( pFrame );
	ASSERT( !pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) );

	//ASSERT( !wrFloatingBar.IsRectEmpty() );
	if( wrFloatingBar.IsRectEmpty() )
	{
		SetCapture();
		return;
	}

CDockBar * pDockBar = NULL;
BOOL bMovingInsideDockBar = FALSE;
	if( (!IsFloating())
		&&
		m_bFixedMode
		)
	{
		if( IsDockedHorizontally() )
		{
			if(	wrFloatingBar.top <= point.y
				&& point.y <= wrFloatingBar.bottom
				)
			{
				wrFloatingBar.OffsetRect(
					point.x - (wrFloatingBar.TopLeft().x + sizeOffset.cx),
					0
					);
				bMovingInsideDockBar = TRUE;
			}
		} // if( IsDockedHorizontally() )
		else
		{
			ASSERT( IsDockedVertically() );
			if(	wrFloatingBar.left <= point.x
				&& point.x <= wrFloatingBar.right
				)
			{
				wrFloatingBar.OffsetRect(
					0,
					point.y - (wrFloatingBar.TopLeft().y + sizeOffset.cy)
					);
				bMovingInsideDockBar = TRUE;
			}
		} // else from if( IsDockedHorizontally() )
	} // if( (!IsFloating()) && m_bFixedMode )

	if( bMovingInsideDockBar )
	{
		ASSERT( !IsFloating() );
		ASSERT( m_pDockBar != NULL );
		ASSERT_VALID( m_pDockBar );
		pDockBar = m_pDockBar;

		CRect wrDockBar;
		//pDockBar->GetWindowRect( &wrDockBar );
		pDockBar->GetClientRect( &wrDockBar );
		pDockBar->ClientToScreen( &wrDockBar );

		if( IsDockedHorizontally() )
		{
			if( wrFloatingBar.right > wrDockBar.right )
				wrFloatingBar.OffsetRect(
					wrDockBar.right - wrFloatingBar.right,
					0
					);
			if( wrFloatingBar.left < wrDockBar.left )
				wrFloatingBar.OffsetRect(
					wrDockBar.left - wrFloatingBar.left,
					0
					);
		} // if( IsDockedHorizontally() )
		else
		{
			ASSERT( IsDockedVertically() );
			if( wrFloatingBar.bottom > wrDockBar.bottom )
				wrFloatingBar.OffsetRect(
					0,
					wrDockBar.bottom - wrFloatingBar.bottom - 1
					);
			if( wrFloatingBar.top < wrDockBar.top )
				wrFloatingBar.OffsetRect(
					0,
					wrDockBar.top - wrFloatingBar.top
					);
		} // else from if( IsDockedHorizontally() )
		
		// ok, find previouse/next controlbars
		MfcControlBarVector_t arrPrevBars;
		CControlBar * pPrevBar = NULL, * pNextBar = NULL;
		bool bThisPassed = false, bRowMaxSized = false;

		int nCbCount = pDockBar->m_arrBars.GetSize();
		for( int nBar=0; nBar<nCbCount; nBar++ )
		{
			CControlBar * pBar =
				((CExtDockBar *)pDockBar)->
					GetDockedControlBar( nBar );
			//void * pVoid = pDockBar->m_arrBars[nPos];
			if( pBar == NULL )
			{
				if( !bThisPassed )
				{
					arrPrevBars.RemoveAll();
					pPrevBar = NULL;
					continue;
				}
				else
					break;
			}
			ASSERT_VALID( pBar );
			if( (!pBar->IsWindowVisible())
				||
				(pBar->m_dwStyle & CBRS_FLOATING)
				)
			{
				if( !bThisPassed )
				{
					arrPrevBars.RemoveAll();
					pPrevBar = NULL;
					continue;
				}
				else
					break;
			}
			if(	(	pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
					&& (! (((CExtControlBar *)pBar)->m_bFixedMode))
				)
				//||	pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar))
				)
				bRowMaxSized = true;
			if( pBar == this )
			{
				bThisPassed = true;
				continue;
			}
			if( !bThisPassed )
			{
				arrPrevBars.Add( pBar );
				pPrevBar = pBar;
				continue;
			}
			if( pNextBar == NULL )
				pNextBar = pBar;
		} // for( int nBar=0; nBar<nCbCount; nBar++ )

		// analyze intersection with next control bar
		if( pNextBar != NULL
			&& bRowMaxSized
			)
		{
			CRect wrNextBar;
			pNextBar->GetWindowRect( & wrNextBar );
			if( IsDockedHorizontally() )
			{
				wrFloatingBar.OffsetRect(
					wrNextBar.left - wrFloatingBar.right,
					0
					);
			} // if( IsDockedHorizontally() )
			else
			{
				if( wrFloatingBar.bottom > wrNextBar.top )
					wrFloatingBar.OffsetRect(
						0,
						wrNextBar.top - wrFloatingBar.bottom
						);
			} // else from if( IsDockedHorizontally() )
		} // if( pNextBar != NULL )

		// analyze intersection with previouse control bar
		if( pPrevBar != NULL )
		{
			CRect wrPrevBar;
			pPrevBar->GetWindowRect( & wrPrevBar );
			if( IsDockedHorizontally() )
			{
				if( wrFloatingBar.left < wrPrevBar.right )
				{
					if( !bRowMaxSized )
					{
						_CompressRowSpace(
							wrPrevBar.right - wrFloatingBar.left,
							arrPrevBars
							);
						pPrevBar->GetWindowRect( & wrPrevBar );
					}
					if( wrFloatingBar.left < wrPrevBar.right )
						wrFloatingBar.OffsetRect(
							wrPrevBar.right - wrFloatingBar.left,
							0
							);
				} // if( wrFloatingBar.left < wrPrevBar.right )
			} // if( IsDockedHorizontally() )
			else
			{
				if( wrFloatingBar.top < wrPrevBar.bottom )
				{
					if( !bRowMaxSized )
					{
						_CompressRowSpace(
							wrPrevBar.bottom - wrFloatingBar.top,
							arrPrevBars
							);
						pPrevBar->GetWindowRect( & wrPrevBar );
					}
					if( wrFloatingBar.top < wrPrevBar.bottom )
						wrFloatingBar.OffsetRect(
							0,
							wrPrevBar.bottom - wrFloatingBar.top
							);
				}
			} // else from if( IsDockedHorizontally() )
		} // if( pPrevBar != NULL )

	} // if( bMovingInsideDockBar )
	else
	{
		wrFloatingBar.OffsetRect(
			point - (wrFloatingBar.TopLeft() + sizeOffset)
			);
		ASSERT( pDockBar == NULL );
		CRect rcDockBar;
		CDockBar * pPossibleDockBar = NULL;
		POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
		while( pos != NULL )
		{
			CDockBar * pTempDockBar = (CDockBar*)
				pFrame->m_listControlBars.GetNext( pos );
			if( pTempDockBar->IsDockBar()
				&&
				pTempDockBar->IsWindowVisible()
				&&
				(pTempDockBar->m_dwStyle & m_dwDockStyle & CBRS_ALIGN_ANY)
				&&
				(!pTempDockBar->m_bFloating ||
					(m_dwDockStyle & pTempDockBar->m_dwStyle & CBRS_FLOAT_MULTI))
				)
			{
				pTempDockBar->GetWindowRect( &rcDockBar );
				rcDockBar.InflateRect(__DOCKING_SIDE_GAP,0);
				rcDockBar.InflateRect(0,__DOCKING_SIDE_GAP);
				BOOL /*bInersected = FALSE;
				if( IsFloating() )
				{
				*/
					bInersected =
						rcDockBar.PtInRect(point);
				/*
				}
				else
				{
					bInersected =
						rcDockBar.IntersectRect(
							rcDockBar,
							wrFloatingBar
							);
				}
				*/
				if( bInersected )
				{
					if( !m_bFixedMode )
					{
						int nDockDlgCtrlID =
							pTempDockBar->GetDlgCtrlID();
						if( nDockDlgCtrlID == AFX_IDW_DOCKBAR_TOP
							||
							nDockDlgCtrlID == AFX_IDW_DOCKBAR_BOTTOM
							)
						{
							pPossibleDockBar = pTempDockBar;
							continue;
						}
					}
					pDockBar = pTempDockBar;
					break;
				}
			}
		}
		if( pDockBar == NULL )
			pDockBar = pPossibleDockBar;
		if( pDockBar == NULL )
		{
			if( !IsFloating() )
			{
				m_ptFloatHelper =
					point - sizeOffset
					- m_ptHelperDraggingOffset;
				pFrame->FloatControlBar(
					this,
					m_ptFloatHelper
					);
				pFrame->RecalcLayout(); // fix for OLE inplace frames
				SetCapture();
				return;
			}
			ASSERT( pDockingFrame != NULL );
			ASSERT( pDockingFrame == GetParentFrame() );
			CRect wrFrame;
			pDockingFrame->GetWindowRect( &wrFrame );
			wrFrame.OffsetRect(
				-wrFrame.TopLeft() + (point-sizeOffset)
				);
			CRect rcDesktop;
			VERIFY(
				::SystemParametersInfo(
					SPI_GETWORKAREA,
					0,
					PVOID(&rcDesktop),
					0
					)
				);
			if( wrFrame.top < rcDesktop.top )
				wrFrame.OffsetRect(0,rcDesktop.top-wrFrame.top);
			if( wrFrame.left < rcDesktop.left )
				wrFrame.OffsetRect(rcDesktop.left-wrFrame.left,0);
			if( wrFrame.top + afxData.cyBorder2*2 + nCyGripper > rcDesktop.bottom )
				wrFrame.OffsetRect(0,rcDesktop.bottom -(wrFrame.top + afxData.cyBorder2*2 + nCyGripper) );
			if( point.x > rcDesktop.right )
				wrFrame.OffsetRect(rcDesktop.right-point.x,0);
			wrFrame.OffsetRect( - m_ptHelperDraggingOffset );
			pDockingFrame->MoveWindow( &wrFrame );
			pDockingFrame->BringWindowToTop();
			pDockingFrame->DelayRecalcLayout();
			SetCapture();
			return;
		} // if( pDockBar == NULL )

		wrFloatingBar.OffsetRect(
			- sizeOffset
			- CSize(
				__DOCKING_SIDE_GAP*2/3,
				__DOCKING_SIDE_GAP*2/3
				)
			//- wrFloatingBar.Size()
			);
	} // else from if( bMovingInsideDockBar )

	ASSERT_VALID( pDockBar );
	ASSERT(pFrame->m_listControlBars.Find(this) != NULL);

	((CExtDockBar *)pDockBar)->
		_SlideDockControlBar(
			this,
			&wrFloatingBar,
			bMovingInsideDockBar
			);
/*
	pFrame->
		DockControlBar(
			this,
			pDockBar,
			&wrFloatingBar
			);
*/
    ASSERT( m_pDockBar != NULL );
    ASSERT( m_pDockBar == pDockBar );
    ASSERT( m_pDockContext != NULL );
    ASSERT( m_pDockSite != NULL );

	pFrame->RecalcLayout();
	m_pDockBar->RedrawWindow(
		NULL,
		NULL,
		RDW_ERASE|RDW_INVALIDATE
			//|RDW_INTERNALPAINT
			|RDW_UPDATENOW
			|RDW_ALLCHILDREN
			|RDW_FRAME
		);
	SetCapture();
}

void CExtControlBar::_CompressRowSpace(
	int nSpaceNeeded,
	MfcControlBarVector_t & arrPrevBars
	)
{
	ASSERT( nSpaceNeeded >= 0 );
	ASSERT_VALID( this );
	ASSERT( !IsFloating() );
	ASSERT_VALID( m_pDockBar );

CRect wrDockBar;
	//pDockBar->GetWindowRect( &wrDockBar );
	m_pDockBar->GetClientRect( &wrDockBar );
	m_pDockBar->ClientToScreen( &wrDockBar );

int nSpaceAvail = 0;
/* int nSpaceForCompress = 0; */ // <-- size compression currently not supported
int nPrevBarsCount = arrPrevBars.GetSize();
CRect rcBarPrev;
typedef CArray < CRect, CRect& > vBarRects_t;
vBarRects_t vBarRects;
typedef CArray < int, int & > vInt_t;
vInt_t vOffsets /*,vCompress*/ ;
	// first calc space available
	for( int nBar=0; nBar<nPrevBarsCount; nBar++ )
	{
		CControlBar * pBar = arrPrevBars[nBar];
		ASSERT_VALID( pBar );
		CRect rcBar;
		pBar->GetWindowRect( & rcBar );
/*
		ASSERT(
			wrDockBar.left <= rcBar.left
			&&
			wrDockBar.top <= rcBar.top
			&&
			rcBar.right <= wrDockBar.right
			&&
			rcBar.bottom <= wrDockBar.bottom
			);
*/
		ASSERT( pBar != this );
/*
		if( pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
		{
			ASSERT( ((CExtControlBar*)pBar)->m_bFixedMode );
			int nMinSize =
				IsDockedHorizontally() ?
					((CExtControlBar*)pBar)->m_sizeMinHorz.cx
					:
					((CExtControlBar*)pBar)->m_sizeMinHorz.cy
					;
			int nCurSize =
				IsDockedHorizontally() ?
					rcBar.Width() : rcBar.Height();
			ASSERT( nCurSize >= nMinSize );
			int nBarCompressMax = nCurSize >= nMinSize;
			nSpaceForCompress += nBarCompressMax;
			vCompress.Add( nBarCompressMax );
		} // if( pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
		else
			vCompress.Add( 0 );
*/
		if( nBar == 0 )
		{
			int nSpaceBeforeFirst =
				IsDockedHorizontally() ?
					rcBar.left - wrDockBar.left
					:
					rcBar.top - wrDockBar.top
					;
			nSpaceAvail = nSpaceBeforeFirst;
			vOffsets.Add( nSpaceBeforeFirst );
			ASSERT( nSpaceAvail >= 0 );
		} // if( nBar == 0 )
		else
		{
			int nSpaceBetweenBars =
				IsDockedHorizontally() ?
					rcBar.left - rcBarPrev.right
					:
					rcBar.top - rcBarPrev.top
					;
			
			//ASSERT( nSpaceBetweenBars >= 0 );
			if( nSpaceBetweenBars < 0 )
				nSpaceBetweenBars = 0;

			nSpaceAvail += nSpaceBetweenBars;
			vOffsets.Add( nSpaceBetweenBars );
		} // else from if( nBar == 0 )
		vBarRects.Add( rcBar );
		rcBarPrev = rcBar;
	} // for( int nBar=0; nBar<nPrevBarsCount; nBar++ )
	ASSERT( vBarRects.GetSize() == nPrevBarsCount );
	ASSERT( vOffsets.GetSize() == nPrevBarsCount );
/*	ASSERT( vCompress.GetSize() == nPrevBarsCount ); */
	if( nSpaceAvail == 0 /*&& nSpaceForCompress == 0*/ )
		return;
	if( nSpaceAvail > nSpaceNeeded )
	{
		nSpaceAvail = nSpaceNeeded;
/*
		nSpaceForCompress = 0;
*/
	}
/*
	else
	{
		int nTotalAvail = nSpaceAvail + nSpaceForCompress;
		if( nTotalAvail > nSpaceNeeded )
			nSpaceForCompress = nTotalAvail - nSpaceNeeded;
		ASSERT( nSpaceForCompress >= 0 );
	} // else from if( nSpaceAvail > nSpaceNeeded )
*/
	for( nBar=nPrevBarsCount; nBar>0; nBar--)
	{
		CControlBar * pBar = arrPrevBars[ nBar-1 ];
		ASSERT_VALID( pBar );
		CRect & rcBar = vBarRects[ nBar-1 ];
		int nShift = vOffsets[ nBar-1 ];
		if( nShift > nSpaceAvail )
			nShift = nSpaceAvail;
		if( IsDockedHorizontally() )
			rcBar.OffsetRect( -nSpaceAvail, 0 );
		else
			rcBar.OffsetRect( 0, -nSpaceAvail );
		nSpaceAvail -= nShift;
		ASSERT( nSpaceAvail >= 0 );
		if( nSpaceAvail == 0 )
			break;
	} // for( nBar=nPrevBarsCount; nBar>0; nBar--)
/*
	for( nBar=nPrevBarsCount; nBar>0; nBar--)
	{
		CControlBar * pBar = arrPrevBars[ nBar-1 ];
		ASSERT_VALID( pBar );
		CRect & rcBar = vBarRects[ nBar-1 ];
		if( nSpaceForCompress > 0 )
		{
			int nCanCompress = vCompress[ nBar-1 ];
			if( nCanCompress > nSpaceForCompress )
				nCanCompress = nSpaceForCompress;
			nSpaceForCompress -= nCanCompress;
			rcBar.right -= nCanCompress+1;
			ASSERT( rcBar.right >= rcBar.left );
		}
	} // for( nBar=nPrevBarsCount; nBar>0; nBar--)
*/
	for( nBar=0; nBar<nPrevBarsCount; nBar++ )
	{
		CControlBar * pBar = arrPrevBars[nBar];
		ASSERT_VALID( pBar );
		CRect & rcBar = vBarRects[nBar];
		((CExtDockBar *)m_pDockBar)->
			_SlideDockControlBar(
				pBar,
				&rcBar,
				TRUE
				);
	} // for( nBar=0; nBar<nPrevBarsCount; nBar++ )
}

void CExtControlBar::OnNcAreaButtonsReinitialize()
{
INT nCountOfNcButtons = NcButtons_GetCount();
	if( nCountOfNcButtons > 0 )
		return;
	NcButtons_Add( new CExtBarNcAreaButtonClose(this) );
	NcButtons_Add( new CExtBarNcAreaButtonExpand(this) );
	NcButtons_Add( new CExtBarNcAreaButtonMenu(this) );
}

void CExtControlBar::OnNcAreaButtonsReposition()
{

INT nCountOfNcButtons = NcButtons_GetCount();
CExtBarNcAreaButton * pPrevBtn = NULL;
	for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
	{
		CExtBarNcAreaButton * pBtn =
			NcButtons_GetAt( nBtn );
		pBtn->OnNcAreaReposition( pPrevBtn );
		if( !pBtn->OnQueryVisibility() )
			continue;
		pPrevBtn = pBtn;
	} // for( INT nBtn = 0; nBtn < nCountOfNcButtons; nBtn++ )
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButton

IMPLEMENT_DYNAMIC( CExtBarNcAreaButton, CObject )

CExtBarNcAreaButton::CExtBarNcAreaButton(
	CExtControlBar * pBar
	)
	: m_pBar( pBar )
	, m_rc( 0, 0, 0, 0 )
	, m_bHover( false )
	, m_bPushed( false )
	, m_bDisabled( false )
	, m_bPaintingCombinedArea( false )
{
	ASSERT_VALID( this );
}

CExtBarNcAreaButton::~CExtBarNcAreaButton()
{
	ASSERT_VALID( this );
}

void CExtBarNcAreaButton::OnNcDestroy()
{
	ASSERT_VALID( this );
	delete this;
}

bool CExtBarNcAreaButton::IsBarFixedMode() const
{
	ASSERT_VALID( this );
	return m_pBar->m_bFixedMode;
}

bool CExtBarNcAreaButton::IsBarVisible() const
{
	ASSERT_VALID( this );
	return m_pBar->IsVisible() ? true : false;
}

bool CExtBarNcAreaButton::IsBarFloated() const
{
	ASSERT_VALID( this );
	return m_pBar->IsFloating();
}

bool CExtBarNcAreaButton::IsBarDocked() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDocked();
}

bool CExtBarNcAreaButton::IsBarDockedHorizontally() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDockedHorizontally();
}

bool CExtBarNcAreaButton::IsBarDockedVertically() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDockedVertically();
}

bool CExtBarNcAreaButton::IsBarDockedAtLeft() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDockedAtLeft();
}

bool CExtBarNcAreaButton::IsBarDockedAtRight() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDockedAtRight();
}

bool CExtBarNcAreaButton::IsBarDockedAtTop() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDockedAtTop();
}

bool CExtBarNcAreaButton::IsBarDockedAtBottom() const
{
	ASSERT_VALID( this );
	return m_pBar->IsDockedAtBottom();
}

bool CExtBarNcAreaButton::GetBarNcAreaInfo( // returns false if no nc area on bar
	LPRECT pRectGripper, //  = NULL // in window coords
	LPRECT pRectWnd, // = NULL // in screen coords
	bool * pbGripperAtTop, //  = NULL
	bool * pbTextOnGripper //  = NULL
	) const
{
	ASSERT_VALID( this );
	if( pRectGripper != NULL )
		::SetRectEmpty( pRectGripper );
	if( pRectWnd != NULL )
		::SetRectEmpty( pRectWnd );
	
	if( IsBarFloated() )
	{
		if( pbGripperAtTop != NULL )
			*pbGripperAtTop = false;
		if( pbTextOnGripper != NULL )
			*pbTextOnGripper = false;
		
		CFrameWnd * pMiniFrame =
			m_pBar->GetDockingFrame();
		if( pRectWnd != NULL )
			pMiniFrame->GetWindowRect( pRectWnd );
		CExtMiniDockFrameWnd * pExtMiniFrame =
			DYNAMIC_DOWNCAST( CExtMiniDockFrameWnd, pMiniFrame );
		if( pExtMiniFrame == NULL )
			return false;
		
		if( pRectGripper != NULL )
		{
			CRect rcGrip( pExtMiniFrame->m_rcFrameCaption );
			rcGrip.InflateRect( 2, 2, 1, 2 );
			*pRectGripper = rcGrip;
		}
		if( pbGripperAtTop != NULL )
			*pbGripperAtTop = true;
		if( pbTextOnGripper != NULL )
			*pbTextOnGripper = true;
		return true;
	} // if( IsBarFloated() )

	if( pRectWnd != NULL )
		m_pBar->GetWindowRect( pRectWnd );
bool bGrip =
		m_pBar->IsBarWithGripper(
			pbGripperAtTop,
			pbTextOnGripper
			);
	if( !bGrip )
		return false;
	if( pRectGripper != NULL )
		*pRectGripper = m_pBar->m_rcGrip;
	return true;
}

bool CExtBarNcAreaButton::IsBarWndActive() const
{
	ASSERT_VALID( this );
	
	if( IsBarFloated() )
	{
		CFrameWnd * pMiniFrame =
			m_pBar->GetDockingFrame();
		CExtMiniDockFrameWnd * pExtMiniFrame =
			DYNAMIC_DOWNCAST( CExtMiniDockFrameWnd, pMiniFrame );
		if( pExtMiniFrame == NULL )
			return false;

		HWND hTmpWndFocus = ::GetFocus();
		if( hTmpWndFocus != NULL
			&& (  pExtMiniFrame->GetSafeHwnd() == hTmpWndFocus
				  || ::IsChild( pExtMiniFrame->GetSafeHwnd(), hTmpWndFocus )
				)
			)
			return true;
		return  false;
	} // if( IsBarFloated() )

	return m_pBar->m_bWindowActive;
}

bool CExtBarNcAreaButton::IsBarSingleOnRow() const
{
	ASSERT_VALID( this );
	return m_pBar->IsSingleOnRow();
}

bool CExtBarNcAreaButton::IsBarMinimizedOnRow() const
{
	ASSERT_VALID( this );
	return m_pBar->IsMinimizedOnRow();
}

bool CExtBarNcAreaButton::IsBarMaximizedOnRow() const
{
	ASSERT_VALID( this );
	return m_pBar->IsMaximizedOnRow();
}

bool CExtBarNcAreaButton::IsBarFirstOnRow( bool bExtBar ) const
{
	ASSERT_VALID( this );
	return m_pBar->_IsFirstBarInRow( bExtBar );
}

bool CExtBarNcAreaButton::IsBarLastOnRow( bool bExtBar ) const
{
	ASSERT_VALID( this );
	return m_pBar->_IsLastBarInRow( bExtBar );
}

CFrameWnd * CExtBarNcAreaButton::GetDockingFrame()
{
	ASSERT_VALID( this );
	return m_pBar->_GetDockingFrameImpl();
}

void CExtBarNcAreaButton::OnNcAreaReposition(
	CExtBarNcAreaButton * pPrevBtn
	)
{
	ASSERT_VALID( this );
bool bGripperAtTop = false;
	VERIFY(
		GetBarNcAreaInfo( &m_rc, NULL, &bGripperAtTop )
		);
const int _nGapToBordersH = 2;
const int _nGapToBordersV = 3;
const int _nGapBetweenButtons = 3;
	if( bGripperAtTop )
	{
		m_rc.DeflateRect(
			_nGapToBordersH,
			_nGapToBordersV
			);
		m_rc.left = m_rc.right - m_rc.Height();
		m_rc.OffsetRect( -1, 0 );
		if( pPrevBtn != NULL )
		{
			const CRect & rcPrev = *pPrevBtn;
			m_rc.OffsetRect(
				rcPrev.left - m_rc.right -
					_nGapBetweenButtons,
				0
				);
		} // if( pPrevBtn != NULL )
	} // if( bGripperAtTop )
	else
	{
		m_rc.DeflateRect(
			_nGapToBordersV,
			_nGapToBordersH
			);
		m_rc.bottom = m_rc.top + m_rc.Width();
		m_rc.OffsetRect( 0, 1 );
		if( pPrevBtn != NULL )
		{
			const CRect & rcPrev = *pPrevBtn;
			m_rc.OffsetRect(
				0,
				- rcPrev.top + m_rc.bottom +
					_nGapBetweenButtons
				);
		} // if( pPrevBtn != NULL )
	} // else from if( bGripperAtTop )
}

bool CExtBarNcAreaButton::OnQueryVisibility() const
{
	ASSERT_VALID( this );
	if( !IsBarVisible() )
		return false;
CRect rcGrip;
	if(	!GetBarNcAreaInfo( &rcGrip ) )
		return false;
	if( rcGrip.IsRectEmpty() )
		return false;
	return true;
}

void CExtBarNcAreaButton::NcDrawDefault(
	CDC & dc,
	int nDockBtnGlyptT
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( OnQueryVisibility() );
	if( m_rc.IsRectEmpty() )
		return;
bool bMenuTracking =
		CExtPopupMenuWnd::IsMenuTracking() ? true : false;
	g_PaintManager->PaintDockingCaptionButton(
		dc,
		m_rc,
		(CExtPaintManager::e_docking_caption_button_t)nDockBtnGlyptT,
		m_bHover && !bMenuTracking && !m_bPaintingCombinedArea,
		m_bPushed && !bMenuTracking && !m_bPaintingCombinedArea,
		!m_bDisabled,
		!IsBarFixedMode() || m_bPaintingCombinedArea,
		IsBarWndActive()
		);
}

void CExtBarNcAreaButton::OnNcAreaDraw( CDC & dc )
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( OnQueryVisibility() );
	if( m_rc.IsRectEmpty() )
		return;

#if (defined __DEBUG_PAINTING_AREAS__)
	dc.FillSolidRect( &m_rc, RGB(255,128,128) );
#else // __DEBUG_PAINTING_AREAS__
	dc;
#endif // !__DEBUG_PAINTING_AREAS__
}

UINT CExtBarNcAreaButton::OnNcAreaHitTest( CPoint point )
{
	ASSERT_VALID( this );
	if( m_rc.PtInRect(point) )
	{
		m_bPushed =
			(
				CExtPopupMenuWnd::IsKeyPressed( VK_LBUTTON )
				&& !CExtPopupMenuWnd::IsKeyPressed( VK_MBUTTON )
				&& !CExtPopupMenuWnd::IsKeyPressed( VK_RBUTTON )
			)
			? true : false;
		m_bHover = true;
		return HTNOWHERE;
	}
	m_bHover = m_bPushed = false;
	return HTCLIENT; // not inside nc-button
}

HCURSOR CExtBarNcAreaButton::OnNcAreaQueryCursor( CPoint point )
{
	ASSERT_VALID( this );
	if( m_rc.PtInRect(point) )
		return g_hCursorArrow;
	return NULL; // not inside nc-button
}

bool CExtBarNcAreaButton::OnNcAreaClicked( CPoint point )
{
	ASSERT_VALID( this );
	if( m_rc.PtInRect(point) )
		return true;
	return false; // continue asking nc-buttins
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonExpand

IMPLEMENT_DYNAMIC( CExtBarNcAreaButtonExpand, CExtBarNcAreaButton )

CExtBarNcAreaButtonExpand::CExtBarNcAreaButtonExpand(
	CExtControlBar * pBar
	)
	: CExtBarNcAreaButton( pBar )
{
}

bool CExtBarNcAreaButtonExpand::OnQueryVisibility() const
{
	ASSERT_VALID( this );
	if( !CExtBarNcAreaButton::OnQueryVisibility() )
		return false;
	if( IsBarFixedMode() || IsBarFloated() )
		return false;
	return true;
}

void CExtBarNcAreaButtonExpand::OnNcAreaDraw( CDC & dc )
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( OnQueryVisibility() );
	if( m_rc.IsRectEmpty() )
		return;

#if (defined __DEBUG_PAINTING_AREAS__)

	dc.FillSolidRect( &m_rc, RGB(128,255,128) );

#else // __DEBUG_PAINTING_AREAS__

	m_bDisabled =
		IsBarSingleOnRow()
		;
bool bMax = IsBarMaximizedOnRow();
bool bHorz = IsBarDockedHorizontally();
	NcDrawDefault(
		dc,
		bHorz
			? ( bMax
					? CExtPaintManager::__DCBT_ARROW_RIGHT
					: CExtPaintManager::__DCBT_ARROW_LEFT
				)
			: ( bMax
					? CExtPaintManager::__DCBT_ARROW_DOWN
					: CExtPaintManager::__DCBT_ARROW_UP
					)
		);

#endif // !__DEBUG_PAINTING_AREAS__
}

UINT CExtBarNcAreaButtonExpand::OnNcAreaHitTest( CPoint point )
{
	ASSERT_VALID( this );
UINT nHT = CExtBarNcAreaButton::OnNcAreaHitTest( point );
	if( nHT == HTCLIENT )
		return HTCLIENT; // not inside nc-button

	if( IsBarSingleOnRow() )
		return HTCLIENT;
	if( IsBarMaximizedOnRow() )
		return HTMINBUTTON;
	return HTMAXBUTTON;
}

bool CExtBarNcAreaButtonExpand::OnNcAreaClicked( CPoint point )
{
	ASSERT_VALID( this );
	if( !m_rc.PtInRect(point) )
		return false; // continue asking nc-buttins

	if( IsBarSingleOnRow() )
	{
		if( !IsBarWndActive() )
			m_pBar->SetFocus();
		return true;
	}

	if( IsBarMaximizedOnRow() )
		m_pBar->MinimizeOnRow();
	else
	{
		m_pBar->MaximizeOnRow();
		m_pBar->SetFocus();
	}
	
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonClose

IMPLEMENT_DYNAMIC( CExtBarNcAreaButtonClose, CExtBarNcAreaButton )

CExtBarNcAreaButtonClose::CExtBarNcAreaButtonClose(
	CExtControlBar * pBar
	)
	: CExtBarNcAreaButton( pBar )
{
}

bool CExtBarNcAreaButtonClose::OnQueryVisibility() const
{
	ASSERT_VALID( this );
	if( !CExtBarNcAreaButton::OnQueryVisibility() )
		return false;
	if( !IsBarFixedMode()  )
		return true;
	return IsBarFloated();
}

void CExtBarNcAreaButtonClose::OnNcAreaDraw( CDC & dc )
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( OnQueryVisibility() );
	if( m_rc.IsRectEmpty() )
		return;

#if (defined __DEBUG_PAINTING_AREAS__)

	dc.FillSolidRect( &m_rc, RGB(128,128,255) );

#else // __DEBUG_PAINTING_AREAS__

	NcDrawDefault(
		dc,
		CExtPaintManager::__DCBT_CLOSE
		);

#endif // !__DEBUG_PAINTING_AREAS__
}

UINT CExtBarNcAreaButtonClose::OnNcAreaHitTest( CPoint point )
{
	ASSERT_VALID( this );
UINT nHT = CExtBarNcAreaButton::OnNcAreaHitTest( point );
	if( nHT == HTCLIENT )
		return HTCLIENT; // not inside nc-button
	return HTCLOSE;
}

bool CExtBarNcAreaButtonClose::OnNcAreaClicked( CPoint point )
{
	ASSERT_VALID( this );
	if( m_rc.PtInRect(point) )
	{
		CFrameWnd * pFrame = GetDockingFrame();
		pFrame->ShowControlBar(
			m_pBar,
			!IsBarVisible(),
			FALSE
			);
		return true;
	}
	return false; // continue asking nc-buttins
}

/////////////////////////////////////////////////////////////////////////////
// CExtBarNcAreaButtonMenu

IMPLEMENT_DYNAMIC( CExtBarNcAreaButtonMenu, CExtBarNcAreaButton )

CExtBarNcAreaButtonMenu::CExtBarNcAreaButtonMenu(
	CExtControlBar * pBar
	)
	: CExtBarNcAreaButton( pBar )
{
}

bool CExtBarNcAreaButtonMenu::OnQueryVisibility() const
{
	ASSERT_VALID( this );
	if( !CExtBarNcAreaButton::OnQueryVisibility() )
		return false;
	if( !IsBarFixedMode() || !IsBarFloated() )
		return false;
	return true;
}

void CExtBarNcAreaButtonMenu::OnNcAreaDraw( CDC & dc )
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( OnQueryVisibility() );
	if( m_rc.IsRectEmpty() )
		return;

#if (defined __DEBUG_PAINTING_AREAS__)

	dc.FillSolidRect( &m_rc, RGB(255,255,128) );

#else // __DEBUG_PAINTING_AREAS__

	NcDrawDefault(
		dc,
		CExtPaintManager::__DCBT_ARROW_DOWN
		);

#endif // !__DEBUG_PAINTING_AREAS__
}

UINT CExtBarNcAreaButtonMenu::OnNcAreaHitTest( CPoint point )
{
	ASSERT_VALID( this );
UINT nHT = CExtBarNcAreaButton::OnNcAreaHitTest( point );
	if( nHT == HTCLIENT )
		return HTCLIENT; // not inside nc-button

	return HTMINBUTTON;
}

bool CExtBarNcAreaButtonMenu::OnNcAreaClicked( CPoint point )
{
	ASSERT_VALID( this );
	if( !m_rc.PtInRect(point) )
		return false; // continue asking nc-buttins

CFrameWnd * pFrame = GetDockingFrame();
	ASSERT_VALID( pFrame );
	ASSERT( pFrame->GetSafeHwnd() != NULL );

CExtPopupMenuWnd * pPopup =
		new CExtPopupMenuWnd;
	if( !pPopup->CreatePopupMenu( pFrame->GetSafeHwnd() ) )
	{
		ASSERT( FALSE );
		return false;
	}

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

	CExtDockBar::_ContextMenuBuild(
		pFrame,
		pPopupBars
		);

CRect rcBarWnd;
	VERIFY(
		GetBarNcAreaInfo( NULL, &rcBarWnd )
		);
CRect rcBtnScreen( m_rc );
	rcBtnScreen.OffsetRect( rcBarWnd.TopLeft() );
	rcBtnScreen.InflateRect( 1, 1 );

CPoint ptCursor = rcBtnScreen.CenterPoint();
	VERIFY(
		pPopup->TrackPopupMenu(
			TPMX_TOPALIGN,
			ptCursor.x,  ptCursor.y,
			pFrame->GetSafeHwnd(),
			&rcBtnScreen,
			false, false, false,
			CExtPopupMenuWnd::__COMBINE_DEFAULT,
			this,
			_CbPaintCombinedContent
			)
		);
	
	return true;
}

void CExtBarNcAreaButtonMenu::_CbPaintCombinedContent(
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

CExtBarNcAreaButtonMenu * pBtn = (CExtBarNcAreaButtonMenu *)pCookie;
	ASSERT( pBtn != NULL );
	ASSERT_VALID( pBtn );
	ASSERT( pBtn->IsKindOf(RUNTIME_CLASS(CExtBarNcAreaButtonMenu)) );

	ASSERT( eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE );

	if( rcExcludeArea.IsRectEmpty() )
		return;

CRect rcBarWnd;
	VERIFY(
		pBtn->GetBarNcAreaInfo( NULL, &rcBarWnd )
		);
CRect rcExcludeAreaX;
	refWndMenu.GetClientRect( &rcExcludeAreaX );
	refWndMenu.ClientToScreen( &rcExcludeAreaX );
CPoint ptOffset =
		rcBarWnd.TopLeft()
		- rcExcludeAreaX.TopLeft()
		;
CPoint ptViewportOrg = dc.GetViewportOrg();
	dc.SetViewportOrg( ptOffset );
	pBtn->m_bPaintingCombinedArea = true;
	pBtn->OnNcAreaDraw( dc );
	pBtn->m_bPaintingCombinedArea = false;
	dc.SetViewportOrg( ptViewportOrg );
}

