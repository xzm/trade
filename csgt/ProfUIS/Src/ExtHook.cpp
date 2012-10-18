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

#include "StdAfx.h"

#if (!defined __EXT_HOOK_H)
	#include "ExtHook.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef
	CList< CExtHookSink *, CExtHookSink * >
	HookSinkList_t;

struct __PROF_UIS_API CExtHookSink::HookChains_t
{
	HookSinkList_t m_HookSinkList;
	HWND m_hWndHooked;
	WNDPROC m_pWNDPROC;

	static LRESULT CALLBACK g_HookWndProc(
		HWND hWnd,
		UINT nMessage,
		WPARAM wParam,
		LPARAM lParam
		);

	HookChains_t(
		HWND hWndHooked
		)
		: m_hWndHooked( hWndHooked )
	{
		ASSERT( m_hWndHooked != NULL );
		ASSERT( ::IsWindow(m_hWndHooked) );

		m_pWNDPROC = (WNDPROC)
			::SetWindowLong(
				m_hWndHooked,
				GWL_WNDPROC,
				(DWORD)g_HookWndProc
				);
		ASSERT( m_pWNDPROC != NULL );
	};

	~HookChains_t()
	{
		DestroyChains( false );
	};

	void DestroyChains( bool bDelete )
	{
		POSITION pos = m_HookSinkList.GetHeadPosition();
		for( int nSinkIdx=0; nSinkIdx < m_HookSinkList.GetCount(); nSinkIdx++ )
		{
			CExtHookSink * pHookSink = m_HookSinkList.GetNext( pos );
			ASSERT( pHookSink != NULL );
			if( pHookSink->IsAutoDeleteHookWndSink() )
				delete pHookSink;
		} // for( int nSinkIdx=0; nSinkIdx < m_HookSinkList.GetCount(); nSinkIdx++ )
		m_HookSinkList.RemoveAll();

		ASSERT( m_hWndHooked != NULL );
		ASSERT( ::IsWindow(m_hWndHooked) );
		ASSERT( m_pWNDPROC != NULL );

		::SetWindowLong(
			m_hWndHooked,
			GWL_WNDPROC,
			(DWORD)m_pWNDPROC
			);
		if( bDelete )
			delete this;
	};

	LRESULT HookChainsWindowProc(
		UINT nMessage,
		WPARAM wParam,
		LPARAM lParam
		)
	{
		ASSERT( m_hWndHooked != NULL );
		ASSERT( ::IsWindow(m_hWndHooked) );
		ASSERT( m_pWNDPROC != NULL );
		POSITION pos = m_HookSinkList.GetHeadPosition();
		for( int nSinkIdx=0; nSinkIdx < m_HookSinkList.GetCount(); nSinkIdx++ )
		{
			CExtHookSink * pHookSink = m_HookSinkList.GetNext( pos );
			ASSERT( pHookSink != NULL );
			if( nMessage == WM_NCDESTROY ) 
			{
				pHookSink->OnHookWndNcDestroy();
				continue;
			}
			LRESULT lResult = 0;
			if(	pHookSink->OnHookWndMsg(
					lResult,
					m_hWndHooked,
					nMessage,
					wParam,
					lParam
					)
				)
				return lResult;
		} // for( int nSinkIdx=0; nSinkIdx < m_HookSinkList.GetCount(); nSinkIdx++ )
		WNDPROC pWNDPROC = m_pWNDPROC;
		HWND hWndHooked = m_hWndHooked;
		if( nMessage == WM_NCDESTROY ) 
			DestroyChains( true );
		LRESULT lResult =
			::CallWindowProc(
				pWNDPROC,
				hWndHooked,
				nMessage,
				wParam,
				lParam
				);
		return lResult;
	};

}; // struct CExtHookSink::HookChains_t

typedef
	CMap < HWND, HWND,
		CExtHookSink::HookChains_t *, CExtHookSink::HookChains_t * >
	HookChainsMap_t;

static HookChainsMap_t g_HookChainsMap;

LRESULT CALLBACK CExtHookSink::HookChains_t::g_HookWndProc(
	HWND hWnd,
	UINT nMessage,
	WPARAM wParam,
	LPARAM lParam
	)
{
#ifdef _USRDLL
	// If this is a DLL, need to set up MFC state
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

LRESULT lResult = 0;

MSG & refMsgMfcCurr = AfxGetThreadState()->m_lastSentMsg;
MSG msgMfcSaved( refMsgMfcCurr );
	refMsgMfcCurr.hwnd    = hWnd;
	refMsgMfcCurr.message = nMessage;
	refMsgMfcCurr.wParam  = wParam;
	refMsgMfcCurr.lParam  = lParam;

CExtHookSink::HookChains_t * pHookChains = NULL;
	if( g_HookChainsMap.Lookup( hWnd, pHookChains ) )
	{
		ASSERT( pHookChains != NULL );
		ASSERT( pHookChains->m_hWndHooked == hWnd );
		lResult =
			pHookChains->HookChainsWindowProc(
				nMessage,
				wParam,
				lParam
				);  
		if( nMessage == WM_NCDESTROY ) 
			g_HookChainsMap.RemoveKey( hWnd );
	} // if( g_HookChainsMap.Lookup( hWnd, pHookChains ) )

	refMsgMfcCurr = msgMfcSaved;
	
	return lResult;
}

CExtHookSink::CExtHookSink(
	bool bEnableDetailedWndHooks // = true
	)
	: m_bEnableDetailedWndHooks( bEnableDetailedWndHooks )
{
}

CExtHookSink::~CExtHookSink()
{
}

LRESULT CExtHookSink::OnHookWndMsgDefault()
{
MSG & msgCurrMfc = AfxGetThreadState()->m_lastSentMsg;
	ASSERT( msgCurrMfc.hwnd != NULL );
	ASSERT( ::IsWindow( msgCurrMfc.hwnd ) );

CExtHookSink::HookChains_t * pHookChains = NULL;
	VERIFY(
		g_HookChainsMap.Lookup(
			msgCurrMfc.hwnd,
			pHookChains
			)
		);
	ASSERT( pHookChains != NULL );
	ASSERT( pHookChains->m_hWndHooked == msgCurrMfc.hwnd );
	ASSERT( pHookChains->m_pWNDPROC != NULL );

	return
		::CallWindowProc(
			pHookChains->m_pWNDPROC,
			msgCurrMfc.hwnd,
			msgCurrMfc.message,
			msgCurrMfc.wParam,
			msgCurrMfc.lParam
			);
}

bool CExtHookSink::OnHookWndMsg(
	LRESULT & lResult,
	HWND hWndHooked,
	UINT nMessage,
	WPARAM wParam,
	LPARAM lParam
	)
{
	lResult;
	hWndHooked;
	nMessage;
	wParam;
	lParam;

	if( !m_bEnableDetailedWndHooks )
		return false;

	switch( nMessage )
	{
	case WM_COMMAND:
		return
			OnHookCmdMsg(
				lResult,
				hWndHooked,
				HIWORD(wParam),
				LOWORD(wParam),
				(HWND)lParam
				);
	case WM_NOTIFY:
		return
			OnHookNotifyMsg(
				lResult,
				hWndHooked,
				(INT)wParam,
				(LPNMHDR)lParam
				);
	case WM_PAINT:
		return
			OnHookPaintMsg(
				lResult,
				hWndHooked,
				(HDC)wParam
				);
	case WM_ERASEBKGND:
		return
			OnHookEraseBackgroundMsg(
				lResult,
				hWndHooked,
				(HDC)wParam
				);
	case WM_PRINT:
		return
			OnHookPrintMsg(
				lResult,
				hWndHooked,
				(HDC)wParam
				);
	case WM_NCPAINT:
		return
			OnHookNcPaintMsg(
				lResult,
				hWndHooked,
				(HRGN)wParam
				);
	} // switch( nMessage )

	return false;
}

bool CExtHookSink::OnHookCmdMsg(
	LRESULT & lResult,
	HWND hWndHooked,
	WORD wNotifyCode,
	WORD wID,
	HWND hWndCtrl
	)
{
	lResult;
	hWndHooked;
	wNotifyCode;
	wID;
	hWndCtrl;
	
	return false;
}

bool CExtHookSink::OnHookNotifyMsg(
	LRESULT & lResult,
	HWND hWndHooked,
	INT nIdCtrl,
	LPNMHDR lpnmhdr
	)
{
	lResult;
	hWndHooked;
	nIdCtrl;
	lpnmhdr;
	
	return false;
}

bool CExtHookSink::OnHookPaintMsg(
	LRESULT & lResult,
	HWND hWndHooked,
	HDC hDC
	)
{
	lResult;
	hWndHooked;
	hDC;
	
	return false;
}

bool CExtHookSink::OnHookEraseBackgroundMsg(
	LRESULT & lResult,
	HWND hWndHooked,
	HDC hDC
	)
{
	lResult;
	hWndHooked;
	hDC;
	
	return false;
}

bool CExtHookSink::OnHookPrintMsg(
	LRESULT & lResult,
	HWND hWndHooked,
	HDC hDC
	)
{
	lResult;
	hWndHooked;
	hDC;
	
	return false;
}

bool CExtHookSink::OnHookNcPaintMsg(
	LRESULT & lResult,
	HWND hWndHooked,
	HRGN hRgnUpdate
	)
{
	lResult;
	hWndHooked;
	hRgnUpdate;
	
	return false;
}

void CExtHookSink::OnHookWndNcDestroy()
{
}

void CExtHookSink::OnHookWndAttach( HWND hWnd )
{
	ASSERT( hWnd != NULL );
	hWnd;
}

void CExtHookSink::OnHookWndDetach( HWND hWnd )
{
	ASSERT( hWnd != NULL );
	hWnd;
}

bool CExtHookSink::IsAutoDeleteHookWndSink()
{
	return false;
}

bool CExtHookSink::SetupHookWndSink(
	HWND hWnd,
	bool bRemove, // = false
	bool bAddToHead // = true
	)
{
	ASSERT( hWnd != NULL );
	if( hWnd == NULL )
		return false;

	// MODIFIED BY SUNZHENYU, remove the next 1 lines, child window is closed.
	// ASSERT( ::IsWindow(hWnd) );
	if( !::IsWindow(hWnd) )
		return false;

CExtHookSink::HookChains_t * pHookChains = NULL;
	if( !g_HookChainsMap.Lookup( hWnd, pHookChains ) )
	{
		ASSERT( pHookChains == NULL );
	}
	else
	{
		ASSERT( pHookChains != NULL );
	}
	
	if( bRemove )
	{
		if( pHookChains == NULL )
			return true;
		POSITION pos =
			pHookChains->m_HookSinkList.Find( this );
		if( pos == NULL )
			return true;

		OnHookWndDetach( hWnd );

		pHookChains->m_HookSinkList.RemoveAt( pos );
		if( IsAutoDeleteHookWndSink() )
			delete this;
		
		if( pHookChains->m_HookSinkList.GetCount() == 0 )
		{
			// MODIFIED BY SUNZHENYU, REMOVE the next 1 line, when window close, it destroys and remove as same.
			// pHookChains->DestroyChains( true );
			// g_HookChainsMap.RemoveKey( hWnd );
		}
		return true;
	} // if( bRemove )
	
	if( pHookChains == NULL )
	{
		pHookChains =
			new CExtHookSink::HookChains_t( hWnd );
		g_HookChainsMap.SetAt( hWnd, pHookChains );
	} // if( pHookChains == NULL )
	else
	{
		POSITION pos =
			pHookChains->m_HookSinkList.Find( this );
		if( pos != NULL )
			return true;
	} // else from if( pHookChains == NULL )

	if( bAddToHead )
		pHookChains->m_HookSinkList.AddHead( this );
	else
		pHookChains->m_HookSinkList.AddTail( this );

	OnHookWndAttach( hWnd );

	return true;
}

ULONG CExtHookSink::SetupHookWndSinkToChilds(
	HWND hWnd,
	UINT * pDlgCtrlIDs, // = NULL
	ULONG nCountOfDlgCtrlIDs, // = 0
	bool bDeep // = false
	)
{
	ASSERT( hWnd != NULL );
	if( hWnd == NULL )
		return 0;

	ASSERT( ::IsWindow(hWnd) );
	if( !::IsWindow(hWnd) )
		return 0;
ULONG nCountOfHooks = 0;
	hWnd = ::GetWindow( hWnd, GW_CHILD );
	for( ; hWnd != NULL; hWnd = ::GetWindow( hWnd, GW_HWNDNEXT ) )
	{
		ASSERT(
			(nCountOfDlgCtrlIDs == 0 && pDlgCtrlIDs == NULL)
			|| (nCountOfDlgCtrlIDs > 0 && pDlgCtrlIDs != NULL)
			);
		bool bSetupHook = true;
		if( nCountOfDlgCtrlIDs > 0 && pDlgCtrlIDs != NULL )
		{
			bSetupHook = false;
			UINT nDlgCtrlID = ::GetDlgCtrlID( hWnd );
			for( ULONG i=0; i<nCountOfDlgCtrlIDs; i++ )
			{
				if( pDlgCtrlIDs[i] == nDlgCtrlID )
				{
					bSetupHook = true;
					break;
				}
			} // for( ULONG i=0; i<nCountOfDlgCtrlIDs; i++ )
		} // if( nCountOfDlgCtrlIDs > 0 && pDlgCtrlIDs != NULL )
		if( bSetupHook )
		{
			if(	SetupHookWndSink( hWnd ) )
				nCountOfHooks++;
			else
			{
				ASSERT( FALSE );
			}
		} // if( bSetupHook )
		if( bDeep )
			nCountOfHooks +=
				SetupHookWndSinkToChilds(
					hWnd,
					pDlgCtrlIDs,
					nCountOfDlgCtrlIDs,
					bDeep
					);
	} // for( ; hWnd != NULL; hWnd = ::GetWindow( hWnd, GW_HWNDNEXT ) )
	return nCountOfHooks;
}

void CExtHookSink::RemoveAllWndHooks()
{
HookedWndList_t _list;
	GetHookedWindows( _list );
POSITION pos = _list.GetHeadPosition();
	for( int nHwndIdx = 0; nHwndIdx < _list.GetCount(); nHwndIdx++ )
	{
		HWND hWndHooked = _list.GetNext( pos );
		VERIFY( SetupHookWndSink( hWndHooked, false ) );
	} // for( int nHwndIdx = 0; nHwndIdx < _list.GetCount(); nHwndIdx++ )
}

void CExtHookSink::GetHookedWindows( HookedWndList_t & _list )
{
	_list.RemoveAll();
POSITION posChains = g_HookChainsMap.GetStartPosition();
	for( ; posChains != NULL; )
	{
		CExtHookSink::HookChains_t * pHookChains = NULL;
		HWND hWndHooked = NULL;
		g_HookChainsMap.GetNextAssoc(
			posChains, hWndHooked, pHookChains );
		ASSERT( hWndHooked != NULL );
		ASSERT( pHookChains != NULL );
		ASSERT( pHookChains->m_hWndHooked == hWndHooked );
		if( pHookChains->m_HookSinkList.Find(this) == NULL )
			continue;
		ASSERT( _list.Find(pHookChains->m_hWndHooked) == NULL );
			_list.AddTail( pHookChains->m_hWndHooked );
	} // for( ; pos != NULL; )
}

bool CExtHookSink::IsHookedWindow( HWND hWnd )
{
CExtHookSink::HookChains_t * pHookChains = NULL;
	if( !g_HookChainsMap.Lookup( hWnd, pHookChains ) )
		return false;
	ASSERT( pHookChains != NULL );
	ASSERT( pHookChains->m_hWndHooked == hWnd );
	if( pHookChains->m_HookSinkList.Find(this) != NULL )
		return true;
	return false;
}
