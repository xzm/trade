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

#if (!defined __ExtCmdManager_H)
	#include <ExtCmdManager.h>
#endif

#if (!defined __EXT_REGISTRY_H)
	#include <ExtRegistry.h>
#endif

#if (!defined __EXT_EXTINTEGRITYCHECKSUM_H)
	#include "ExtIntegrityCheckSum.h"
#endif

#include <limits.h>

#pragma message("   Automatically linking with version.lib")
#pragma message("      (Version info support)")
#pragma comment(lib,"version.lib") 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExtCmdManager::CExtCmdManagerAutoPtr g_CmdManager;

volatile DWORD CExtCmdManager::CExtCmdManagerAutoPtr::g_dwVersion = 0;

#define __MAX_TOTAL_TICK_COUNT (UINT_MAX-1)
#define __DECREACE_TICK_COUNT_BY_REF(_REF_) (_REF_) /= 2

DWORD CExtCmdManager::CExtCmdManagerAutoPtr::GetVersionDWORD(
	bool bForSerialization // = false
	)
{
DWORD dwVersion = 0;
static CCriticalSection scs;
CSingleLock sl( &scs );
BYTE * pByteVerInfo = NULL;
	sl.Lock();
	try
	{
		if( g_dwVersion == 0 )
		{
			HANDLE hModule =
				::GetModuleHandle( __PROF_UIS_MODULE_NAME );
			ASSERT( hModule != NULL );
			TCHAR szFileName[MAX_PATH];
			VERIFY(
				::GetModuleFileName(
					HINSTANCE(hModule),
					szFileName,
					MAX_PATH
					)
				);
			DWORD dwFVIS =
				::GetFileVersionInfoSize(
					szFileName,
					&dwFVIS
					);
			pByteVerInfo = new BYTE[4096];
			::memset( pByteVerInfo, 0, 4096 );
			VERIFY(
				::GetFileVersionInfo(
					szFileName,
					NULL,
					dwFVIS,
					pByteVerInfo
					)
				);
			UINT uNum = 0;
			VS_FIXEDFILEINFO * lpv = NULL;
			VERIFY(
				::VerQueryValue(
					pByteVerInfo,
					_T("\\"),
					((LPVOID*)&lpv),
					&uNum
					)
				);
			ASSERT( lpv != NULL );
			g_dwVersion = 
				 (DWORD(HIWORD(lpv->dwFileVersionMS)&0x0FF)<<24)
				|(DWORD(LOWORD(lpv->dwFileVersionMS)&0x0FF)<<16)
				|(DWORD(HIWORD(lpv->dwFileVersionLS)&0x0FF)<<8)
				|(DWORD(LOWORD(lpv->dwFileVersionLS)&0x0FF))
				;
			if( pByteVerInfo != NULL )
			{
				delete pByteVerInfo;
				pByteVerInfo = NULL;
			}
		} // if( g_dwVersion == 0 )
		dwVersion = g_dwVersion;
		if( bForSerialization )
			dwVersion >>= 16;
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
	sl.Unlock();
	if( pByteVerInfo != NULL )
		delete pByteVerInfo;
	return dwVersion;
}

LPCTSTR CExtCmdManager::CExtCmdManagerAutoPtr::GetVersionString(
	CString & strBuff, // = CString()
	TCHAR tchrSeparator // = _T('.')
	)
{
DWORD dwVersion = GetVersionDWORD( false );
	ASSERT( dwVersion != 0 );
	strBuff.Format(
		_T("%d%c%d%c%d%c%d"),
		INT( (dwVersion>>24)&0x0FF ),
		tchrSeparator,
		INT( (dwVersion>>16)&0x0FF ),
		tchrSeparator,
		INT( (dwVersion>>8)&0x0FF ),
		tchrSeparator,
		INT( dwVersion&0x0FF )
		);
	return LPCTSTR( strBuff );
}

void CExtCmdManager::cmd_t::AnalyzeGlobalUsageOverflow()
{
	ASSERT( m_pProfile != NULL );
	if( m_pProfile->m_nTotalTickCount < __MAX_TOTAL_TICK_COUNT )
		return;
	__DECREACE_TICK_COUNT_BY_REF( m_pProfile->m_nTotalTickCount );
POSITION pos = m_pProfile->m_cmds.GetStartPosition();
	for( ; pos != NULL; )
	{
		UINT nCmdID;
		cmd_t * p_cmd = NULL;
		m_pProfile->m_cmds.GetNextAssoc( pos, nCmdID, p_cmd);
		ASSERT( p_cmd != NULL );
		__DECREACE_TICK_COUNT_BY_REF( p_cmd->m_nUsageTickCount );
	}
}

CExtCmdManager::CExtCmdManagerAutoPtr::CExtCmdManagerAutoPtr()
{
	m_pCmdMenager = new CExtCmdManager;
}

CExtCmdManager::CExtCmdManagerAutoPtr::~CExtCmdManagerAutoPtr()
{
	ASSERT( m_pCmdMenager != NULL );
	if( m_pCmdMenager != NULL )
	{
		delete m_pCmdMenager;
		m_pCmdMenager = NULL;
	}
}

CExtCmdManager * CExtCmdManager::CExtCmdManagerAutoPtr::operator->()
{
	ASSERT( m_pCmdMenager != NULL );
	return m_pCmdMenager;
}

bool CExtCmdManager::g_bDisableCmdIfNoHandler = true;

CExtCmdManager::CExtCmdManager()
{
	ProfileSetup(
		__EXTMFC_DEF_PROFILE_NAME
		);
}

CExtCmdManager::~CExtCmdManager()
{
	_RemoveAllProfilesImpl();
}

// setup single profile
bool CExtCmdManager::ProfileSetup(
	LPCTSTR sProfileName, //  = NULL
	HWND hProfileWnd // = NULL
	)
{
	if( sProfileName == NULL )
		sProfileName = __EXTMFC_DEF_PROFILE_NAME;
	m_cs.Lock();
cmd_profile_t * p_profile = NULL;
BOOL bExist = m_profiles.Lookup( sProfileName, (void *&)p_profile );
	if( !bExist )
	{
		p_profile = new cmd_profile_t( sProfileName );
		m_profiles.SetAt( sProfileName, p_profile );
	}
	ASSERT( p_profile != NULL );
	m_cs.Unlock();
	if( hProfileWnd != NULL )
	{
		VERIFY( ProfileWndAdd( sProfileName, hProfileWnd ) );
	}
	return true;
}

// setup profile window
bool CExtCmdManager::ProfileWndAdd(
	LPCTSTR sProfileName,
	HWND hProfileWnd
	)
{
	ASSERT( hProfileWnd != NULL );
	if( hProfileWnd == NULL )
		return false;
	ASSERT( ::IsWindow(hProfileWnd) );
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	m_cs.Lock();
cmd_profile_t * p_profile2 = NULL;
BOOL bExist = m_profile_wnds.Lookup( hProfileWnd, p_profile2 );
	if( !bExist )
		m_profile_wnds.SetAt( hProfileWnd, p_profile );
#ifdef _DEBUG
	else
	{
		ASSERT( p_profile2 != NULL );
		ASSERT( p_profile2 == p_profile );
	}
#endif // _DEBUG
	m_cs.Unlock();
	return true;
}

// remove profile window
bool CExtCmdManager::ProfileWndRemove(
	HWND hProfileWnd
	)
{
	ASSERT( hProfileWnd != NULL );
	if( hProfileWnd == NULL )
		return false;
	ASSERT( ::IsWindow(hProfileWnd) );
	m_cs.Lock();

cmd_profile_t * p_profile = NULL;
BOOL bExists = m_profile_wnds.Lookup( hProfileWnd, p_profile );
	if( bExists )
	{
		ASSERT( p_profile != NULL );
		m_profile_wnds.RemoveKey( hProfileWnd );
	}
	m_cs.Unlock();
	return true;
}

// get profile
CExtCmdManager::cmd_profile_t * CExtCmdManager::ProfileGetPtr(
	LPCTSTR sProfileName //  = NULL
	)
{
	if( sProfileName == NULL )
		sProfileName = __EXTMFC_DEF_PROFILE_NAME;
	m_cs.Lock();
cmd_profile_t * p_profile = NULL;
BOOL bExists = m_profiles.Lookup( sProfileName, (void *&)p_profile );
	if( bExists )
	{
		ASSERT( p_profile != NULL );
	}
	else
		p_profile = NULL;
	m_cs.Unlock();
	return p_profile;
}

// get profile name for window
LPCTSTR CExtCmdManager::ProfileNameFromWnd(
	HWND hWnd
	)
{
	ASSERT( hWnd != NULL );
	if( hWnd == NULL )
		return NULL;
	ASSERT( ::IsWindow(hWnd) );
LPCTSTR sProfileName = NULL;
	m_cs.Lock();
	for(	;
			sProfileName == NULL && hWnd != NULL;
			hWnd = GetParent(hWnd) //::GetWindow(hWnd,GW_OWNER)
			)
	{
		cmd_profile_t * p_profile = NULL;
		BOOL bExists = m_profile_wnds.Lookup( hWnd, p_profile );
		if( !bExists )
			continue;
		ASSERT( p_profile != NULL );
		ASSERT( !p_profile->m_sName.IsEmpty() );
		sProfileName = p_profile->m_sName;
		break;
	}
	m_cs.Unlock();
	return sProfileName;
}

// setup single command
bool CExtCmdManager::CmdSetup(
	LPCTSTR sProfileName,
	const CExtCmdManager::cmd_t & cmd,
	bool bReplaceOld, // = false // but force set images anywhere if was empty
	bool * pbWasAddedNew // = NULL
	)
{
	if( pbWasAddedNew != NULL )
		*pbWasAddedNew = false;
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	m_cs.Lock();
bool bRetVal = p_profile->CmdSetup(cmd,bReplaceOld,pbWasAddedNew);
	m_cs.Unlock();
	return bRetVal;
}

// remove single command
bool CExtCmdManager::CmdRemove(
	LPCTSTR sProfileName,
	UINT nCmdID,
	bool * pbWasRemoved // = NULL
	)
{
	if( pbWasRemoved != NULL )
		*pbWasRemoved = false;
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	m_cs.Lock();
bool bRetVal = p_profile->CmdRemove(nCmdID,pbWasRemoved);
	m_cs.Unlock();
	return bRetVal;
}

// alloc command
CExtCmdManager::cmd_t * CExtCmdManager::CmdAllocPtr(
	LPCTSTR sProfileName,
	UINT nCmdID // = 0 // 0 means any free in avail range
	)
{
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}
	m_cs.Lock();
CExtCmdManager::cmd_t * p_cmd = p_profile->CmdAllocPtr(nCmdID);
	m_cs.Unlock();
	return p_cmd;
}

// get command
CExtCmdManager::cmd_t * CExtCmdManager::CmdGetPtr(
	LPCTSTR sProfileName,
	UINT nCmdID
	)
{
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}
	m_cs.Lock();
CExtCmdManager::cmd_t * p_cmd = p_profile->CmdGetPtr(nCmdID);
	m_cs.Unlock();
	return p_cmd;
}

// get command icon (if command and its icon exist)
CExtCmdManager::icon_t * CExtCmdManager::CmdGetIconPtr(
	LPCTSTR sProfileName,
	UINT nCmdID
	)
{
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}
	m_cs.Lock();
CExtCmdManager::icon_t * p_icon = p_profile->CmdGetIconPtr(nCmdID);
	m_cs.Unlock();
	return p_icon;
}

// is command registered
bool CExtCmdManager::CmdIsRegistered(
	LPCTSTR sProfileName,
	UINT nCmdID
	)
{
	bool bRegistered =
		(CmdGetPtr(sProfileName,nCmdID) != NULL) ?
			true : false;
	return bRegistered;
}

// update commands coolection from menu handle
bool CExtCmdManager::UpdateFromMenu(
	LPCTSTR sProfileName,
	HMENU hMenu,
	bool bReplaceOld, // = false
	bool bRecursive, // = true
	bool bLoadTips // = true
	)
{
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	m_cs.Lock();
bool bRetVal =
		p_profile->UpdateFromMenu(
			hMenu, bReplaceOld, bRecursive, bLoadTips );
	m_cs.Unlock();
	return bRetVal;
}

// update commands coolection from menu handle
bool CExtCmdManager::cmd_profile_t::UpdateFromMenu(
	HMENU hMenu,
	bool bReplaceOld, // = false
	bool bRecursive, // = true
	bool bLoadTips // = true
	)
{
	if( hMenu == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	if( !(::IsMenu(hMenu)) )
	{
		ASSERT( FALSE );
		return false;
	}

int nMenuItemCount = ::GetMenuItemCount(hMenu);
	for(	int nMenuItemIdx=0;
			nMenuItemIdx < nMenuItemCount;
			nMenuItemIdx++
			)
	{ // for all menu level items
		// get the menu item info
		CString sMenuText;
		MENUITEMINFO mii;
		::memset( &mii, 0, sizeof(MENUITEMINFO) );
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
		mii.dwTypeData =
			sMenuText.GetBuffer(__MAX_UI_ITEM_TEXT);
		ASSERT( mii.dwTypeData != NULL );
		if( mii.dwTypeData == NULL )
		{
			ASSERT( FALSE );
			continue;
		}
		if( !::GetMenuItemInfo(
				hMenu,
				nMenuItemIdx,
				TRUE,
				&mii
				)
			)
		{
			sMenuText.ReleaseBuffer();
			ASSERT( FALSE );
			continue;
		}
		sMenuText.ReleaseBuffer();
		if( (mii.fType & MFT_SEPARATOR) != 0 )
			continue;
		
		// if sub-menu process it
		if( mii.hSubMenu != NULL )
		{
			if( !bRecursive )
				continue;
			VERIFY(
				UpdateFromMenu(
					mii.hSubMenu,
					bReplaceOld,
					true
					)
				);
			continue;
		} // if( mii.hSubMenu != NULL )
		
		// register command
		if( !IsCommand(mii.wID) )
			continue;

		cmd_t cmd;
		cmd.m_nCmdID = mii.wID;

		sMenuText.Replace( _T("\n"), _T("") );
		sMenuText.Replace( _T("\r"), _T("") );
		sMenuText.TrimLeft();
		sMenuText.TrimRight();
		if( !sMenuText.IsEmpty() )
		{
			int nSep =
				sMenuText.ReverseFind( _T('\t') );
			if( nSep >= 0 )
			{
				int nLen = sMenuText.GetLength();
				cmd.m_sAccelText = sMenuText.Right(nLen-nSep);
				cmd.m_sAccelText.TrimLeft();
				cmd.m_sAccelText.TrimRight();
				cmd.m_sMenuText = sMenuText.Left(nSep);
				cmd.m_sMenuText.TrimLeft();
				cmd.m_sMenuText.TrimRight();
			}
			else
				cmd.m_sMenuText = sMenuText;
		} // if( !sMenuText.IsEmpty() )

		if( bLoadTips )
			cmd.TipsLoad();

		VERIFY( CmdSetup(cmd,bReplaceOld) );
	} // for all menu level items

	return true;
}

// update commands coolection from menu resurce
bool CExtCmdManager::UpdateFromMenu(
	LPCTSTR sProfileName,
	UINT nResourceID,
	bool bReplaceOld, // = false
	bool bLoadTips // = true
	)
{
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	m_cs.Lock();
bool bRetVal =
		p_profile->UpdateFromMenu(
			nResourceID, bReplaceOld, bLoadTips );
	m_cs.Unlock();
	return bRetVal;
}

// update commands coolection from menu resurce
bool CExtCmdManager::cmd_profile_t::UpdateFromMenu(
	UINT nResourceID,
	bool bReplaceOld, // = false
	bool bLoadTips // = true
	)
{
CMenu menu;
	if( !menu.LoadMenu(nResourceID) )
	{
		ASSERT(FALSE);
		return false;
	}
	return
		UpdateFromMenu(
			menu.GetSafeHmenu(),
			bReplaceOld,
			true, // bRecursive
			bLoadTips
			);
}

// update commands coolection from toolbar wnd
bool CExtCmdManager::UpdateFromToolBar(
	LPCTSTR sProfileName,
	CToolBar & bar,
	bool bReplaceOld, // = false // but force set images anywhere if was empty
	bool bLoadTips // = true
	)
{
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	m_cs.Lock();
bool bRetVal =
		p_profile->UpdateFromToolBar(
			bar, bReplaceOld, bLoadTips );
	m_cs.Unlock();
	return bRetVal;
}

// update commands coolection from toolbar wnd
bool CExtCmdManager::cmd_profile_t::UpdateFromToolBar(
	CToolBar & bar,
	bool bReplaceOld, // = false // but force set images anywhere if was empty
	bool bLoadTips // = true
	)
{
	if( !(::IsWindow(bar.GetSafeHwnd())) )
	{
		ASSERT( FALSE );
		return false;
	}
CImageList * pImageList =
		bar.GetToolBarCtrl().GetImageList();
	if( pImageList == NULL
		|| pImageList->GetSafeHandle() == NULL
		)
	{
		ASSERT( FALSE );
		return false;
	}
int nBtnCount = bar.GetCount();
    for( int nBtnIdx=0; nBtnIdx < nBtnCount; nBtnIdx++ )
	{
		// get button info
		UINT nCmdID = bar.GetItemID(nBtnIdx); 
		if( nCmdID == ID_SEPARATOR )
			continue;
		ASSERT( IsCommand(nCmdID) );

		TBBUTTONINFO tbi;
		::memset( &tbi, 0, sizeof(TBBUTTONINFO) );
		tbi.cbSize = sizeof(TBBUTTONINFO);
		tbi.idCommand = nCmdID;
		tbi.dwMask = TBIF_IMAGE|TBIF_TEXT;
		tbi.cchText = __MAX_UI_ITEM_TEXT;
		CString sToolbarText;
		tbi.pszText =
			sToolbarText.GetBuffer(__MAX_UI_ITEM_TEXT);
		ASSERT( tbi.pszText != NULL );
		if( tbi.pszText == NULL )
		{
			ASSERT( FALSE );
			continue;
		}
		if( !bar.GetToolBarCtrl().GetButtonInfo(nCmdID,&tbi) )
		{
			UINT nDummyID,nDummyStyle;
			bar.GetButtonInfo(
				nBtnIdx,
				nDummyID,
				nDummyStyle,
				tbi.iImage
				);
			/*
			sToolbarText.ReleaseBuffer();
			ASSERT(FALSE);
			continue;
			*/
		}
		sToolbarText.ReleaseBuffer();

		// register command
		cmd_t cmd;
		cmd.m_nCmdID = nCmdID;
		cmd.m_sToolbarText = sToolbarText;
		if( bLoadTips )
			cmd.TipsLoad();
		if( !CmdSetup(cmd,bReplaceOld) )
		{
			ASSERT(FALSE);
			continue;
		}
		if( tbi.iImage < 0 )
			continue;
		ASSERT( tbi.iImage < pImageList->GetImageCount() );
		cmd_t * p_cmd = CmdGetPtr(nCmdID);
		ASSERT( p_cmd != NULL );
		if( p_cmd->m_nIconIdx >= 0 )
		{
			ASSERT( p_cmd->m_nIconIdx < m_icons.GetSize() );
			continue;
		}
		HICON hIcon = pImageList->ExtractIcon( tbi.iImage );
		if( hIcon == NULL )
		{
			ASSERT(FALSE);
			continue;
		}
		icon_t * p_icon = new icon_t;
		p_icon->Attach(hIcon);
		int nIconIndex = m_icons.Add( p_icon );
		ASSERT( nIconIndex >= 0 );
		p_cmd->m_nIconIdx = nIconIndex;
	} // for( int nBtnIdx=0; nBtnIdx < nBtnCount; nBtnIdx++ )
	return true;
}

// update commands coolection from toolbar resource
bool CExtCmdManager::UpdateFromToolBar(
	LPCTSTR sProfileName,
	UINT nResourceID,
	bool bReplaceOld, // = false // but force set images anywhere if was empty
	bool bLoadTips // = true
	)
{
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	m_cs.Lock();
bool bRetVal =
		p_profile->UpdateFromToolBar(
			nResourceID, bReplaceOld, bLoadTips );
	m_cs.Unlock();
	return bRetVal;
}

// update commands coolection from toolbar resource
bool CExtCmdManager::cmd_profile_t::UpdateFromToolBar(
	UINT nResourceID,
	bool bReplaceOld, // = false // but force set images anywhere if was empty
	bool bLoadTips // = true
	)
{
CToolBar bar;
	if( !bar.Create(::AfxGetMainWnd()) )
	{
		ASSERT( FALSE );
		return false;
	}
	if( !bar.LoadToolBar(nResourceID) )
	{
		ASSERT( FALSE );
		return false;
	}
	return
		UpdateFromToolBar(
			bar,
			bReplaceOld,
			bLoadTips
			);
}

// set list of commands (up to (UINT)0) as basic or non basic
bool CExtCmdManager::SetBasicCommands(
	LPCTSTR sProfileName,
	UINT * pCommands,
	bool bOn // = true
	)
{
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	m_cs.Lock();
bool bRetVal =
		p_profile->SetBasicCommands(
			pCommands, bOn );
	m_cs.Unlock();
	return bRetVal;
}

// set list of commands (up to (UINT)0) as basic or non basic
bool CExtCmdManager::cmd_profile_t::SetBasicCommands(
	UINT * pCommands,
	bool bOn // = true
	)
{
	if( pCommands == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
bool bRetVal = true;
	for(; *pCommands != 0; pCommands++ )
	{
		if( IsSystemCommand( *pCommands ) )
		{
			ASSERT( FALSE );
			bRetVal = false; // we find some error
			continue;
		}
		cmd_t * p_cmd = CmdGetPtr( *pCommands );
		if( p_cmd == NULL )
		{
			ASSERT( FALSE );
			bRetVal = false; // we find some error
			continue;
		}
		p_cmd->StateSetBasic( bOn );
	} // for(; *pCommands != 0; pCommands++ )
	return bRetVal;
}

#define __REG_LINES_IN_BLOCK	128 // 1024
#define __REG_LINE_SIZE			16
#define __REG_LINE_FMT			_T("data_0x%08lX")
#define __REG_BLOCK_FMT			_T("block_0x%08lX")
#define __REG_VAR_DATA_SIZE		_T("data_size")
#define __REG_VAR_DATA_CHECK	_T("data_integrity")
#define __REG_VAR_GENERATOR		_T("data_generator")
#define __REG_FMT_GENERATOR		_T("Prof-UIS v. %s registry archiver")

CString CExtCmdManager::GetSubSystemRegKeyPath(
	LPCTSTR sSubSystemName,
	LPCTSTR sProfileName,
	LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	LPCTSTR sSectionNameProduct // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	)
{
	ASSERT( sSubSystemName != NULL );
	ASSERT( sProfileName != NULL );
	ASSERT( sSectionNameCompany != NULL );
	ASSERT( sSectionNameProduct != NULL );
CString s;
	s.Format(
		_T("Software\\%s\\%s\\%s\\%s\\%s\\%s"),
		sSectionNameCompany,
		sSectionNameProduct,
		__PROF_UIS_REG_SECTION,
		__PROF_UIS_REG_PROFILES,
		sProfileName,
		sSubSystemName
		);
//	s.Replace(' ','_');
	s.Replace('\r','_');
	s.Replace('\t','_');
	s.Replace('\n','_');
	s.Replace('?','_');
	s.Replace('*','_');
	s.Replace('.','_');
	s.Replace(',','_');
	return s;
}

static CString productsection2regkeypath(
	LPCTSTR sProfileName,
	LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	LPCTSTR sSectionNameProduct // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	)
{
	return CExtCmdManager::GetSubSystemRegKeyPath(
		__PROF_UIS_REG_COMMAND_MANAGER,
		sProfileName,
		sSectionNameCompany,
		sSectionNameProduct
		);
}

bool CExtCmdManager::FileObjToRegistry(
	CFile & _file,
	LPCTSTR sRegKeyPath
	)
{
	ASSERT( sRegKeyPath != NULL );
	if( sRegKeyPath == NULL )
		return false;
	CExtRegistry::RegDeleteKey(
		HKEY_CURRENT_USER,
		(LPCTSTR)sRegKeyPath,
		NULL
		);
	_file.Seek( 0, CFile::begin );
DWORD dwLen = (DWORD)_file.GetLength();
CExtRegistry reg;
	if( !reg.Create(
			HKEY_CURRENT_USER,
			(LPCTSTR)sRegKeyPath,
			KEY_ALL_ACCESS
			)
		)
		return false;
	if(	!reg.SaveNumber(
			__REG_VAR_DATA_SIZE,
			dwLen
			)
		)
		return false;
BYTE buffer[__REG_LINE_SIZE];
ULONG nCount, nPortion=0;
CExtIntegrityCheckSum _ExtIntegrityCheckSum;
	for( ; (nCount=_file.Read(&buffer,__REG_LINE_SIZE)) > 0;  )
	{
		CString sBlockSubKey;
		ULONG nBlockNo = nPortion/__REG_LINES_IN_BLOCK;
		sBlockSubKey.Format( __REG_BLOCK_FMT, nBlockNo );
		CString sRegKeyPath2( sRegKeyPath );
		sRegKeyPath2 += _T('\\');
		sRegKeyPath2 += sBlockSubKey;
		CExtRegistry reg;
		if( !reg.Create(
				HKEY_CURRENT_USER,
				(LPCTSTR)sRegKeyPath2,
				KEY_ALL_ACCESS
				)
			)
			return false;

		CString sVarName;
		sVarName.Format(
			__REG_LINE_FMT,
			nPortion++
			);
		if(	!reg.SaveBinary(
				(LPCTSTR)sVarName,
				buffer,
				nCount
				)
			)
			return false;
		_ExtIntegrityCheckSum.Update(
			buffer,
			nCount
			);
	}
USES_CONVERSION;
CString sExtIntegrityCheckSum =
		LPCTSTR( _ExtIntegrityCheckSum.Final() );
	ASSERT( !sExtIntegrityCheckSum.IsEmpty() );
	if(	!reg.SaveString(
			__REG_VAR_DATA_CHECK,
			(LPCTSTR)sExtIntegrityCheckSum
			)
		)
		return false;
	
CString sGenerator;
	sGenerator.Format(
		__REG_FMT_GENERATOR,
		g_CmdManager.GetVersionString()
		);
	VERIFY(
		reg.SaveString(
			__REG_VAR_GENERATOR,
			(LPCTSTR)sGenerator
			)
		);

	return true;
}

bool CExtCmdManager::FileObjFromRegistry(
	CFile & _file,
	LPCTSTR sRegKeyPath
	)
{
	ASSERT( sRegKeyPath != NULL );
	if( sRegKeyPath == NULL )
		return false;
CExtRegistry reg;
	if( !reg.Open(
			HKEY_CURRENT_USER,
			(LPCTSTR)sRegKeyPath,
			KEY_READ
			)
		)
		return false;
DWORD dwLen = 0;
	if(	!reg.LoadNumber(
			__REG_VAR_DATA_SIZE,
			&dwLen
			)
		)
		return false;
	if( dwLen == 0 )
		return true;
BYTE buffer[__REG_LINE_SIZE];
ULONG nCount = __REG_LINE_SIZE, nPortion = 0;
DWORD dwLoaded = 0;
CExtIntegrityCheckSum _ExtIntegrityCheckSum;
	for( ; true;  )
	{
		CString sBlockSubKey;
		ULONG nBlockNo = nPortion/__REG_LINES_IN_BLOCK;
		sBlockSubKey.Format( __REG_BLOCK_FMT, nBlockNo );
		CString sRegKeyPath2( sRegKeyPath );
		sRegKeyPath2 += _T('\\');
		sRegKeyPath2 += sBlockSubKey;
		CExtRegistry reg;
		if( !reg.Open(
				HKEY_CURRENT_USER,
				(LPCTSTR)sRegKeyPath2,
				KEY_READ
				)
			)
			return false;
	
		CString sVarName;
		sVarName.Format(
			__REG_LINE_FMT,
			nPortion++
			);
		if( dwLen-dwLoaded < __REG_LINE_SIZE )
			nCount = dwLen-dwLoaded;
		ASSERT( nCount > 0 && nCount <= __REG_LINE_SIZE );
		if(	!reg.LoadBinary(
				(LPCTSTR)sVarName,
				buffer,
				nCount
				)
			)
			return false;
		dwLoaded += nCount;
		ASSERT( dwLoaded <= dwLen );

		_ExtIntegrityCheckSum.Update(
			buffer,
			nCount
			);
		_file.Write(
			buffer,
			nCount
			);

		if( dwLoaded == dwLen )
			break;
	}
USES_CONVERSION;
CString sExtIntegrityCheckSum = 
		LPCTSTR( _ExtIntegrityCheckSum.Final() );
	ASSERT( !sExtIntegrityCheckSum.IsEmpty() );

CString sExtIntegrityCheckSumA;
int nSz = sExtIntegrityCheckSum.GetLength() + 1;
	if(	!reg.LoadString(
			__REG_VAR_DATA_CHECK,
			sExtIntegrityCheckSumA.GetBuffer( nSz ),
			nSz
			)
		)
	{
		sExtIntegrityCheckSumA.ReleaseBuffer();
		return false;
	}
	sExtIntegrityCheckSumA.ReleaseBuffer();

	if( sExtIntegrityCheckSumA != sExtIntegrityCheckSum )
	{
		ASSERT( FALSE );
		return false;
	}

	_file.Seek( 0, CFile::begin );

	return true;
}

static bool fileobj_to_registry(
	CFile & _file,
	LPCTSTR sProfileName,
	LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	LPCTSTR sSectionNameProduct // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	)
{
	ASSERT( sProfileName != NULL );
	ASSERT( sSectionNameCompany != NULL );
	ASSERT( sSectionNameProduct != NULL );
CString sRegKeyPath =
		productsection2regkeypath(
			sProfileName,
			sSectionNameCompany,
			sSectionNameProduct
			);

	return CExtCmdManager::FileObjToRegistry( _file, sRegKeyPath );
}

static bool fileobj_from_registry(
	CFile & _file,
	LPCTSTR sProfileName,
	LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	LPCTSTR sSectionNameProduct // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	)
{
	ASSERT( sProfileName != NULL );
	ASSERT( sSectionNameCompany != NULL );
	ASSERT( sSectionNameProduct != NULL );
	ASSERT( _file.GetLength() == 0 );
CString sRegKeyPath =
		productsection2regkeypath(
				sProfileName,
			sSectionNameCompany,
			sSectionNameProduct
			);
	return CExtCmdManager::FileObjFromRegistry( _file, sRegKeyPath );
}

// save/load command manager state
bool CExtCmdManager::SerializeState(
	LPCTSTR sProfileName,
	LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	bool bSave
	)
{
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	m_cs.Lock();
bool bRetVal =
		p_profile->SerializeState(
			sSectionNameCompany,
			sSectionNameProduct,
			bSave
			);
	m_cs.Unlock();
	return bRetVal;
}

// save/load command manager state
bool CExtCmdManager::cmd_profile_t::SerializeState(
	LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	bool bSave
	)
{
	ASSERT( sSectionNameCompany != NULL );
	ASSERT( sSectionNameProduct != NULL );
bool bRetVal = false;
	try
	{
		CMemFile _file;
		if( bSave )
		{
			{ // BLOCK: CArchive usage
				CArchive ar(
					&_file,
					CArchive::store
					);
				if( !SerializeState(ar) )
					return false;
				ar.Flush();
			} // BLOCK: CArchive usage

			// ... write _file to registty
			_file.Seek(0,CFile::begin);
			if( !fileobj_to_registry(
					_file,
					LPCTSTR(m_sName),
					sSectionNameCompany,
					sSectionNameProduct
					)
				)
				return false;

		} // if( bSave )
		else
		{
			// ... read _file from registty
			if( !fileobj_from_registry(
					_file,
					LPCTSTR(m_sName),
					sSectionNameCompany,
					sSectionNameProduct
					)
				)
				return false;
			_file.Seek(0,CFile::begin);

			CArchive ar(
				&_file,
				CArchive::load
				);
			if( !SerializeState(ar) )
				return false;
		} // else from if( bSave )

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

bool CExtCmdManager::SerializeState(
	LPCTSTR sProfileName,
	CArchive & ar
	)
{
cmd_profile_t * p_profile = ProfileGetPtr( sProfileName );
	if( p_profile == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	m_cs.Lock();
bool bRetVal =
		p_profile->SerializeState( ar );
	m_cs.Unlock();
	return bRetVal;
}

bool CExtCmdManager::cmd_profile_t::SerializeState(
	CArchive & ar
	)
{
bool bRetVal = false;
	try
	{
		CString sFriendlyVer;
		DWORD dwApiVer0 = g_CmdManager.GetVersionDWORD( false );
		DWORD dwApiVer1 = g_CmdManager.GetVersionDWORD( true );
		DWORD dwReserved = 0;

		if( ar.IsStoring() )
		{ // store state
			// store version info
			sFriendlyVer.Format(
				_T("Prof-UIS (v. %s) command profile"),
				g_CmdManager.GetVersionString()
				);
			ar << sFriendlyVer;
			ar << dwApiVer0;
			ar << dwApiVer1;
			ar << dwReserved;
			ar << dwReserved;
			ar << dwReserved;
			ar << dwReserved;

			// store command usage information
			ar << m_nTotalTickCount;

			POSITION pos = m_cmds.GetStartPosition();
			for( ; pos != NULL; )
			{
				UINT nCmdID;
				cmd_t * p_cmd = NULL;
				m_cmds.GetNextAssoc( pos, nCmdID, p_cmd);
				ASSERT( p_cmd != NULL );
				if( p_cmd->StateIsBasic()
					|| p_cmd->StateIsForceRarely()
					|| IsSystemCommand( p_cmd->m_nCmdID )
					|| p_cmd->m_nUsageTickCount == 0
					)
					continue;
				ar << p_cmd->m_nCmdID;
				ar << p_cmd->m_nUsageTickCount;
			}

			ar << ((UINT)(0));
		} // store state
		else
		{ // load state
			// load version info
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

			// load command usage information
			ar >> m_nTotalTickCount;
			for( ; true; )
			{
				UINT nCmdId,nUsageTickCount;
				ar >> nCmdId;
				if( nCmdId == 0 )
					break;
				ar >> nUsageTickCount;
				cmd_t * p_cmd = CmdGetPtr(nCmdId);
				ASSERT( p_cmd != NULL );
				ASSERT( !(p_cmd->StateIsBasic()) );
				ASSERT( !(p_cmd->StateIsForceRarely()) );
				ASSERT( !(IsSystemCommand(p_cmd->m_nCmdID)) );
				p_cmd->m_nUsageTickCount = nUsageTickCount;
			}
		} // load state

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
