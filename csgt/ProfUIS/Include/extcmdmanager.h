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

#if (!defined __ExtCmdManager_H)
#define __ExtCmdManager_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __AFXMT_H__)
	#include <AfxMt.h>
#endif

#if (!defined __AFXTEMPL_H__)
	#include <AfxTempl.h>
#endif

// max text size
#define __MAX_UI_ITEM_TEXT (1024)

// mdi menu list commands
#define __ID_MDIWNDLIST_COUNT 9
#define __ID_MDIWNDLIST_FIRST AFX_IDM_FIRST_MDICHILD
#define __ID_MDIWNDLIST_LAST (__ID_MDIWNDLIST_FIRST+__ID_MDIWNDLIST_COUNT-1)
#define __ID_MDIWND_DLGWINDOWS (__ID_MDIWNDLIST_LAST+1)

class __PROF_UIS_API CExtCmdManager
{
public:
	static inline bool IsCommand(UINT nCmdID)
	{
		//if( ((INT)nCmdID) <= 0 )
		if( nCmdID == 0
			|| ((INT)nCmdID) <= ((INT)(-1))
			)
			return false;
		return true;
	};
	static inline bool IsSystemCommand( UINT nCmd )
	{
		bool bSystemCommand =
			(nCmd >= 0xF000 && nCmd < 0xF1F0) ?
				true : false;
		return bSystemCommand;
	};
	static inline bool IsAfxOleCommand( UINT nCmd )
	{
		if( ID_OLE_VERB_FIRST <= nCmd
			&& nCmd <= ID_OLE_VERB_LAST
			)
			return true;
		return false;
	};
	static inline bool IsAfxMdiWindowsCommand( UINT nCmd )
	{
		if( (	__ID_MDIWNDLIST_FIRST <= nCmd
				&&	nCmd <= __ID_MDIWNDLIST_LAST )
			|| nCmd == __ID_MDIWND_DLGWINDOWS
			)
			return true;
		return false;
	};
	static inline bool IsAfxFileMruCommand( UINT nCmd )
	{
		if( ID_FILE_MRU_FIRST <= nCmd
			&&	nCmd <= ID_FILE_MRU_LAST
			)
			return true;
		return false;
	};
	static inline bool IsForceBasicCommand( UINT nCmd )
	{
		if(		IsAfxMdiWindowsCommand( nCmd )
			||	IsAfxFileMruCommand( nCmd )
			||	IsSystemCommand( nCmd )
			||	IsAfxOleCommand( nCmd )
			)
			return true;
		return false;
	}
	static inline bool IsForceRarelyCommand( UINT nCmd )
	{
		return false;
	};
	static inline bool IsCommandNeedsSpecUpdate( UINT nCmd )
	{
		if(	IsSystemCommand( nCmd )
			|| IsAfxMdiWindowsCommand( nCmd )
			)
			return true;
		return false;
	};

	class cmd_profile_t;

/*
	// accelerator item class
	class accel_t
	{
		void AssignFromOther(const accel_t & other)
		{
		};
	public:
		// construction/destruction/assignment
		accel_t()
		{
		};
		accel_t(const accel_t & other)
		{
			AssignFromOther(other);
		};
		accel_t()
		{
		};
		accel_t & operator=(const accel_t & other)
		{
			AssignFromOther(other);
			return *this;
		};
	}; // class accel_t
*/

	// icon item class
	class __PROF_UIS_API icon_t
	{
		void InitClassInstance()
		{
			m_pProfile = NULL;
			m_hIcon = NULL;
			m_sizeIconOrginal.cx = m_sizeIconOrginal.cy = 0;
		};
		void AssignFromHICON(
			const HICON & hIcon,
			bool bCreateCopy
			)
		{
			DestroyIcon();
			ASSERT( IsEmpty() );
			if( hIcon == NULL )
				return;
			if( bCreateCopy )
			{
				ICONINFO ii;
				// Get icon dimension
				::memset( &ii, 0, sizeof(ICONINFO) );
				VERIFY( ::GetIconInfo( hIcon, &ii ) );
				m_sizeIconOrginal.cx = (BYTE)(ii.xHotspot * 2);
				m_sizeIconOrginal.cy = (BYTE)(ii.yHotspot * 2);
				//::DeleteObject(ii.hbmMask);
				//::DeleteObject(ii.hbmColor);
				m_hIcon = ::CreateIconIndirect(&ii);
				ASSERT( m_hIcon != NULL );
			}
			else
			{
				m_hIcon = hIcon;
				RecalcIconOriginalSize();
			}
		};
		void AssignFromOther(const icon_t & other)
		{
			m_pProfile = other.m_pProfile;
			AssignFromHICON(other.m_hIcon,true);
		};
		void RecalcIconOriginalSize()
		{
			m_sizeIconOrginal.cx = m_sizeIconOrginal.cy = 0;
			if( m_hIcon == NULL )
				return;
			ICONINFO ii;
			// Get icon dimension
			::memset( &ii, 0, sizeof(ICONINFO) );
			VERIFY( ::GetIconInfo( m_hIcon, &ii ) );
			m_sizeIconOrginal.cx = (BYTE)(ii.xHotspot * 2);
			m_sizeIconOrginal.cy = (BYTE)(ii.yHotspot * 2);
			::DeleteObject(ii.hbmMask);
			::DeleteObject(ii.hbmColor);
		};

		HICON m_hIcon;
		CSize m_sizeIconOrginal;
		cmd_profile_t * m_pProfile;

	public:
		bool IsEmpty() const
		{
			return (m_hIcon == NULL) ? true : false;
		};
		void DestroyIcon()
		{
			if( m_hIcon != NULL )
			{
				VERIFY( ::DestroyIcon(m_hIcon) );
			}
			m_sizeIconOrginal.cx = m_sizeIconOrginal.cy = 0;
		};
		HICON Detach()
		{
			//ASSERT( !IsEmpty() );
			HICON hIcon = m_hIcon;
			m_hIcon = NULL;
			m_sizeIconOrginal.cx = m_sizeIconOrginal.cy = 0;
			return hIcon;
		};
		void Attach( HICON hIcon )
		{
			//ASSERT( IsEmpty() );
			DestroyIcon();

			AssignFromHICON(hIcon,false);
		};

		// construction/destruction/assignment
		icon_t()
		{
			InitClassInstance();
		};
		icon_t(const icon_t & other)
		{
			InitClassInstance();
			AssignFromOther(other);
		};
		icon_t(
			const HICON & hIcon,
			bool bCreateCopy //= false
			)
		{
			InitClassInstance();
			AssignFromHICON(hIcon,bCreateCopy);
		};
		~icon_t()
		{
			DestroyIcon();
		};

		icon_t & operator=(const icon_t & other)
		{
			AssignFromOther(other);
			return *this;
		};
		icon_t & operator=(const HICON & hIcon)
		{
			AssignFromHICON(hIcon,true);
			return *this;
		};

		// use as HICON 
		HICON GetIcon()
		{
			//ASSERT( !IsEmpty() );
			return m_hIcon;
		};
		const HICON GetIcon() const
		{
			//ASSERT( !IsEmpty() );
			return m_hIcon;
		};
		operator HICON()
		{
			return GetIcon();
		};
		operator HICON() const
		{
			return GetIcon();
		};

		// use as CSize
		CSize GetSize()
		{
			return m_sizeIconOrginal;
		};
		const CSize GetSize() const
		{
			return m_sizeIconOrginal;
		};
		operator CSize()
		{
			return GetSize();
		};
		operator CSize() const
		{
			return GetSize();
		};

	}; // class icon_t

	// command ui update class
	class cmd_t;
	class __PROF_UIS_API cmd_ui_t : public CCmdUI
	{
		cmd_t * m_pCmd;

	public:
		cmd_ui_t(
			cmd_t * pCmd,
			int nIndexMax = 0
			)
		{
			ASSERT( pCmd != NULL );
			m_pCmd = pCmd;
			m_nIndexMax = nIndexMax;
			m_nID = pCmd->m_nCmdID;
			m_pOther = NULL;
		};

		// Operations to do in ON_UPDATE_COMMAND_UI
		virtual void Enable(BOOL bOn = TRUE)
		{
			ASSERT( m_pCmd != NULL );
			m_pCmd->StateEnable( bOn ? true : false );
		}
		
		virtual void SetCheck(int nCheck = 1)   // 0, 1 or 2 (indeterminate)
		{
			ASSERT( m_pCmd != NULL );
			if( nCheck == 0 || nCheck == 1 )
				m_pCmd->StateSetCheck(
					(nCheck == 0) ? false : true
					);
			else
			{
				ASSERT( nCheck == 2 );
				m_pCmd->StateSetIndeterminate(
					true
					);
			}
		};
		
		virtual void SetRadio(BOOL bOn = TRUE)
		{
			ASSERT( m_pCmd != NULL );
			m_pCmd->StateSetRadio( bOn ? true : false );
		};
		
		virtual void SetText(LPCTSTR lpszText)
		{
			ASSERT( m_pCmd != NULL );
			m_pCmd->m_sMenuText = lpszText;
			//if( !(m_pCmd->m_sToolbarText.IsEmpty()) )
			//	m_pCmd->m_sToolbarText = lpszText;
		};
	}; // class cmd_ui_t

	// command item class
	class __PROF_UIS_API cmd_t
	{
		void AssignFromOther(const cmd_t & other)
		{
			m_pProfile = other.m_pProfile;
			m_nCmdID = other.m_nCmdID;
			m_nStateFlags = other.m_nStateFlags;
			m_nIconIdx = other.m_nIconIdx;
			m_sMenuText = other.m_sMenuText,
			m_sToolbarText = other.m_sToolbarText,
			m_sTipTool = other.m_sTipTool;
			m_sTipStatus = other.m_sTipStatus;
			m_sAccelText = other.m_sAccelText;
			m_nUsageTickCount = other.m_nUsageTickCount;
		};
	public:

		enum e_state_t // state flags
		{
			STATE_ENABLED		= 0x00000000,
			STATE_DISABLED		= 0x00000001,
			STATE_CHECK			= 0x00000002,
			STATE_RADIO			= 0x00000004,
			STATE_INDETERMINATE	= 0x00000008,
			STATE_BASICCMDPROP	= 0x00000010,
			STATE_FORCE_RARELY	= 0x00000020,
		};

		cmd_profile_t * m_pProfile;
		UINT m_nCmdID;
		int m_nIconIdx;
		BYTE m_nStateFlags;
		CString
			m_sMenuText,m_sToolbarText,
			m_sTipTool,m_sTipStatus,
			m_sAccelText
			;
	protected:
		UINT m_nUsageTickCount;

	public:

		// construction/destruction/assignment
		cmd_t(UINT nCmdID = IDC_STATIC) :
			m_pProfile( NULL ),
			m_nCmdID(nCmdID),
			m_nStateFlags(STATE_ENABLED),
			m_nIconIdx(-1),
			m_sMenuText(_T("")),m_sToolbarText(_T("")),
			m_sTipTool(_T("")),m_sTipStatus(_T("")),
			m_sAccelText(_T("")),
			m_nUsageTickCount(0)
		{
		};
		cmd_t(const cmd_t & other)
			: m_pProfile( NULL )
		{
			AssignFromOther(other);
		};
		~cmd_t()
		{
			//m_icon.DestroyIcon();
		};
		cmd_t & operator=(const cmd_t & other)
		{
			AssignFromOther(other);
			return *this;
		};

		//clean tips
		void TipsClean()
		{
			m_sTipTool = _T("");
			m_sTipStatus = _T("");
		};

		// load tips
		bool TipsLoad()
		{
			TipsClean();
			if( !CExtCmdManager::IsCommand(m_nCmdID) )
			{
				ASSERT( FALSE );
				return false;
			}
			CString sText;

			if( !sText.LoadString(m_nCmdID) )
				return false;
			sText.Replace("\t"," ");
			sText.Replace("\r","");
			sText.TrimLeft();
			sText.TrimRight();
			if( sText.IsEmpty() )
				return false;
			int nSep =
				sText.ReverseFind('\n');
			if( nSep < 0 )
			{
				m_sTipTool = sText;
				return true;
			}
			int nLen = sText.GetLength();
			m_sTipTool = sText.Right(nLen-nSep);
			m_sTipTool.TrimLeft();
			m_sTipTool.TrimRight();
			m_sTipStatus = sText.Left(nSep);
			m_sTipStatus.TrimLeft();
			m_sTipStatus.TrimRight();
			return true;
		};

		// upload more full parameters from other
		void UpdateMoreExact( const cmd_t & other )
		{
			ASSERT( m_pProfile != NULL );
			ASSERT( m_nCmdID == other.m_nCmdID );
			if( m_nIconIdx < 0 )
				m_nIconIdx = other.m_nIconIdx;
			//if( m_sMenuText.IsEmpty() )
			if( !other.m_sMenuText.IsEmpty() )
				m_sMenuText = other.m_sMenuText;
			//if( m_sTipTool.IsEmpty() )
			if( !other.m_sTipTool.IsEmpty() )
				m_sTipTool = other.m_sTipTool;
			//if( m_sTipStatus.IsEmpty() )
			if( !other.m_sTipStatus.IsEmpty() )
				m_sTipStatus = other.m_sTipStatus;
			//if( m_sAccelText.IsEmpty() )
			if( !other.m_sAccelText.IsEmpty() )
				m_sAccelText = other.m_sAccelText;
		};

		// state retrieve/update
		void StateSetBasic( bool bOn = true )
		{
			if(	bOn )
				m_nStateFlags |= STATE_BASICCMDPROP;
			else
				m_nStateFlags &= ~STATE_BASICCMDPROP;
		};
		bool StateIsBasic()
		{
			if( CExtCmdManager::IsForceBasicCommand(m_nCmdID) )
				return true;
			if( CExtCmdManager::IsForceRarelyCommand(m_nCmdID) )
				return false;
			bool bBasic =
				(m_nStateFlags&STATE_BASICCMDPROP) ? true : false;
			return bBasic;
		};
		bool StateIsRarelyUsed()
		{
			if( StateIsBasic() )
				return false;
			UINT nUsagePercent = GetUsagePercent();
			ASSERT( m_pProfile != NULL );
			return
				(nUsagePercent < m_pProfile->m_nRarelyPercent) ?
					true : false;
		}

		void StateForceRarely( bool bOn = true )
		{
			if(	bOn )
				m_nStateFlags |= STATE_FORCE_RARELY;
			else
				m_nStateFlags &= ~STATE_FORCE_RARELY;
		};
		bool StateIsForceRarely()
		{
			if( CExtCmdManager::IsForceRarelyCommand(m_nCmdID) )
				return true;
			if( CExtCmdManager::IsForceBasicCommand(m_nCmdID) )
				return false;
			bool bForceRarely =
				(m_nStateFlags&STATE_FORCE_RARELY) ? true : false;
			return bForceRarely;
		};

		void StateEnable( bool bOn = true )
		{
			if(	bOn )
				m_nStateFlags &= ~STATE_DISABLED;
			else
				m_nStateFlags |= STATE_DISABLED;
		};
		bool StateIsEnable()
		{
			bool bEnable =
				(m_nStateFlags&STATE_DISABLED) ? false : true;
			return bEnable;
		};
		void StateSetCheck( bool bOn = true )
		{
			m_nStateFlags &=
				~(STATE_RADIO|STATE_CHECK|STATE_INDETERMINATE);
			if( bOn )
				m_nStateFlags |= STATE_CHECK;
		};
		bool StateGetCheck()
		{
			bool bCheck =
				(m_nStateFlags&STATE_CHECK) ? true : false;
			return bCheck;
		};
		void StateSetRadio( bool bOn = true )
		{
			m_nStateFlags &=
				~(STATE_RADIO|STATE_CHECK|STATE_INDETERMINATE);
			if( bOn )
				m_nStateFlags |= STATE_RADIO;
		};
		bool StateGetRadio()
		{
			bool bRadio =
				(m_nStateFlags&STATE_RADIO) ? true : false;
			return bRadio;
		};
		void StateSetIndeterminate( bool bOn = true )
		{
			m_nStateFlags &=
				~(STATE_RADIO|STATE_CHECK|STATE_INDETERMINATE);
			if( bOn )
				m_nStateFlags |= STATE_INDETERMINATE;
		};
		bool StateGetIndeterminate()
		{
			bool bIndeterminate =
				(m_nStateFlags&STATE_INDETERMINATE) ? true : false;
			return bIndeterminate;
		};

		// update cmd ui
		bool DoUpdateCmdUI(
			CCmdTarget * pCmdTarget,
			UINT nIndex = 0
			)
		{
			ASSERT( pCmdTarget != NULL );
			cmd_ui_t cmd_ui(this);
			if( IsCommandNeedsSpecUpdate(m_nCmdID) )
			{
				cmd_ui.Enable(TRUE);
				return true;
			} // if( IsSystemCommand(m_nCmdID) )
			else
			{
				cmd_ui.m_nIndex = nIndex;
				BOOL bRetVal =
					cmd_ui.DoUpdate(
					pCmdTarget,
					CExtCmdManager::g_bDisableCmdIfNoHandler ?
						TRUE : FALSE
					);
				return bRetVal ? true : false;
			} // else from if( IsSystemCommand(m_nCmdID) )

		};

		bool Deliver(
			HWND hWndCmdTarget,
			bool bSend = false
			)
		{
			if( hWndCmdTarget == NULL )
				return false;
			IncrementUsageCount();
			ASSERT( ::IsWindow( hWndCmdTarget ) );
			ASSERT( CExtCmdManager::IsCommand(m_nCmdID) );
			UINT nMsg = WM_COMMAND;
			LPARAM lParam = 0;
			if( CExtCmdManager::IsSystemCommand(m_nCmdID) )
			{ // if we need WM_SYSCOMMAND
				nMsg = WM_SYSCOMMAND;
				POINT point = { 0, 0 };
				VERIFY(
					::GetCursorPos( &point )
					);
				lParam =
					MAKELONG(
						point.x,
						point.y
						);
			} // if we need WM_SYSCOMMAND
			if( bSend )
			{
				if( !::SendMessage(
						hWndCmdTarget,
						nMsg,
						m_nCmdID,
						lParam
						)
					)
				{
					ASSERT( FALSE );
					return false;
				}
			} // if( bSend )
			else
			{
				if( !::PostMessage(
						hWndCmdTarget,
						nMsg,
						m_nCmdID,
						lParam
						)
					)
				{
					ASSERT( FALSE );
					return false;
				}
			} // else from  if( bSend )
			return true;
		};
		bool Deliver(
			CWnd * pWndCmdTarget,
			bool bSend = false
			)
		{
			ASSERT_VALID( pWndCmdTarget );
			return
				Deliver(
					pWndCmdTarget->GetSafeHwnd(),
					bSend
					);
		};
		bool Deliver(
			CControlBar * pWndCmdSource,
			bool bSend = false
			)
		{
			if( pWndCmdSource == NULL
				|| pWndCmdSource->GetSafeHwnd() == NULL
				)
			{
				ASSERT( FALSE );
				return FALSE;
			}
			ASSERT_VALID( pWndCmdSource );
			CWnd * pOwner = pWndCmdSource->GetOwner();
			if( pOwner == NULL )
			{
				ASSERT( FALSE );
				return FALSE;
			}
			ASSERT_VALID( pOwner );
			ASSERT( ! pOwner->IsKindOf(RUNTIME_CLASS(CControlBar)) );
			return
				Deliver(
					pOwner->GetSafeHwnd(),
					bSend
					);
		};

		UINT GetUsageTickCount() const
		{
			return m_nUsageTickCount;
		};
		UINT GetUsagePercent() const
		{
			ASSERT( m_pProfile != NULL );
			if( m_pProfile->m_nTotalTickCount == 0 )
				return 0;
			return
				(m_nUsageTickCount * 100)
				/
				m_pProfile->m_nTotalTickCount
				;
		};
		void IncrementUsageCount()
		{
			if( StateIsBasic()
				|| StateIsForceRarely()
				)
				return;
			ASSERT( m_pProfile != NULL );
			m_nUsageTickCount++;
			m_pProfile->m_nTotalTickCount++;
			AnalyzeGlobalUsageOverflow();
		};
		UINT GetProfileTickCount()
		{
			ASSERT( m_pProfile != NULL );
			return m_pProfile->m_nTotalTickCount;
		};
		UINT GetProfileRarelyPercent()
		{
			ASSERT( m_pProfile != NULL );
			return m_pProfile->m_nRarelyPercent;
		};
		void SetProfileRarelyPercent(
			UINT nRarelyPercent
			)
		{
			ASSERT( nRarelyPercent <= 100 );
			if( nRarelyPercent > 100 )
				nRarelyPercent = 100;
			ASSERT( m_pProfile != NULL );
			m_pProfile->m_nRarelyPercent = nRarelyPercent;
		};
	protected:
		void AnalyzeGlobalUsageOverflow();

		friend class CExtCmdManager;
		friend class cmd_profile_t;
	}; // class cmd_t

	typedef
		CArray < icon_t *, icon_t * >
		icon_container_t;
	typedef
		CMap < UINT, UINT, cmd_t *, cmd_t *& >
		cmd_container_t;

	class cmd_profile_t
	{
	public:
		CString m_sName;
		UINT m_nRarelyPercent; // values 0..100 has sence
		UINT m_nTotalTickCount;

		cmd_container_t m_cmds;
		icon_container_t m_icons;

#define __MFCEXT_DEF_RARELY_USED_PERCENT 10

		cmd_profile_t(
			LPCTSTR sName = NULL
			)
			: m_sName( (sName == NULL) ? _T("") : sName )
			, m_nRarelyPercent( __MFCEXT_DEF_RARELY_USED_PERCENT )
			, m_nTotalTickCount( 0 )
		{
		};
		cmd_profile_t(
			const cmd_profile_t & other
			)
			: m_sName( _T("") )
			, m_nRarelyPercent( __MFCEXT_DEF_RARELY_USED_PERCENT )
			, m_nTotalTickCount( 0 )
		{
			AssignFromOther( other );
		};
		~cmd_profile_t()
		{
			_RemoveAllIconsImpl();
			_RemoveAllCmdsImpl();
		};
		cmd_profile_t & operator=(
			const cmd_profile_t & other
			)
		{
			AssignFromOther( other );
			return *this;
		};
		// setup single command
		bool CmdSetup(
			const cmd_t & cmd,
			bool bReplaceOld = false, // but force set images anywhere if was empty
			bool * pbWasAddedNew = NULL
			)
		{
			if( pbWasAddedNew != NULL )
				*pbWasAddedNew = false;
			if( !IsCommand(cmd.m_nCmdID) )
			{
				ASSERT(FALSE);
				return false;
			}

			cmd_t * p_cmd = NULL;
			BOOL bExist =
				m_cmds.Lookup( cmd.m_nCmdID, p_cmd );
			if( !bExist )
			{
				p_cmd = new cmd_t(cmd);
				p_cmd->m_pProfile = this;
				m_cmds.SetAt( cmd.m_nCmdID, p_cmd );
				if( pbWasAddedNew != NULL )
					*pbWasAddedNew = true;
				return true;
			} // if( !bExist )
			ASSERT( p_cmd != NULL );
			ASSERT( p_cmd->m_pProfile == this );
			p_cmd->UpdateMoreExact( cmd );
			
			return true;
		};
		// remove single command
		bool CmdRemove(
			UINT nCmdID,
			bool * pbWasRemoved = NULL
			)
		{
			if( pbWasRemoved != NULL )
				*pbWasRemoved = false;
			if( !IsCommand(nCmdID) )
			{
				ASSERT(FALSE);
				return false;
			}

			cmd_t * p_cmd = NULL;
			BOOL bExist =
				m_cmds.Lookup( nCmdID, p_cmd );
			if( !bExist )
				return true;
			ASSERT( p_cmd != NULL );
			delete p_cmd;
			m_cmds.RemoveKey( nCmdID );
			if( pbWasRemoved != NULL )
				*pbWasRemoved = true;
			return true;
		};
		// alloc command
		cmd_t * CmdAllocPtr(
			UINT nCmdID = 0 // 0 means any free in avail range
			)
		{
			if( nCmdID != 0 && CmdIsRegistered(nCmdID) )
				return NULL;
			if( nCmdID == 0 )
			{ // find free ID for new command
				nCmdID = 65534;
				if( !m_cmds.IsEmpty() )
				{
					while( true )
					{
						cmd_t * p_cmd = NULL;
						BOOL bExist =
							m_cmds.Lookup( nCmdID, p_cmd );
						if( bExist )
						{
							ASSERT( p_cmd != NULL );
							nCmdID --;
							if( nCmdID == 0 )
							{
								ASSERT( FALSE );
								return NULL;
							}
							continue;
						}
						ASSERT( IsCommand(nCmdID) );
						break;
					} // while( true )
				} // if( !m_cmds.IsEmpty() )
			} // find free ID for new command
			ASSERT( !CmdIsRegistered(nCmdID) );
			cmd_t cmd(nCmdID);
			if( !CmdSetup(cmd) )
				return NULL;
			cmd_t * p_cmd = CmdGetPtr(nCmdID);
			ASSERT( p_cmd != NULL );
			ASSERT( p_cmd->m_nCmdID == nCmdID );
			return p_cmd;
		};
		// get command
		cmd_t * CmdGetPtr(
			UINT nCmdID
			)
		{
			if( !IsCommand(nCmdID) )
			{
				ASSERT(FALSE);
				return NULL;
			}
			cmd_t * p_cmd = NULL;
			BOOL bExist =
				m_cmds.Lookup( nCmdID, p_cmd );
			if( !bExist )
				return NULL;
			ASSERT( p_cmd != NULL );
			return p_cmd;
		};
		// get command icon (if command and its icon exist)
		icon_t * CmdGetIconPtr(
			UINT nCmdID
			)
		{
			if( !IsCommand(nCmdID) )
			{
				ASSERT(FALSE);
				return NULL;
			}
			cmd_t * p_cmd = NULL;
			BOOL bExist =
				m_cmds.Lookup( nCmdID, p_cmd );
			if( !bExist )
				return NULL;
			ASSERT( p_cmd != NULL );
			if( p_cmd->m_nIconIdx < 0 )
				return NULL;
			ASSERT( p_cmd->m_nIconIdx < m_icons.GetSize() );
			icon_t * p_icon = m_icons[ p_cmd->m_nIconIdx ];
			ASSERT( p_icon != NULL );
			ASSERT( !(p_icon->IsEmpty()) );
			return p_icon;
		};
		// is command registered
		bool CmdIsRegistered(
			UINT nCmdID
			)
		{
			bool bRegistered =
				(CmdGetPtr(nCmdID) != NULL) ?
					true : false;
			return bRegistered;
		};
		// update commands coolection from menu handle
		bool UpdateFromMenu(
			HMENU hMenu,
			bool bReplaceOld = false,
			bool bRecursive = true,
			bool bLoadTips = true
			);
		// update commands coolection from menu resurce
		bool UpdateFromMenu(
			UINT nResourceID,
			bool bReplaceOld = false,
			bool bLoadTips = true
			);
		// update commands coolection from toolbar wnd
		bool UpdateFromToolBar(
			CToolBar & bar,
			bool bReplaceOld = false, // but force set images anywhere if was empty
			bool bLoadTips = true
			);
		// update commands coolection from toolbar resource
		bool UpdateFromToolBar(
			UINT nResourceID,
			bool bReplaceOld = false, // but force set images anywhere if was empty
			bool bLoadTips = true
			);
		// set list of commands (up to (UINT)0) as basic or non basic
		bool SetBasicCommands(
			UINT * pCommands,
			bool bOn = true
			);

		// save/load command manager state
		bool SerializeState(
			LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
			LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
			bool bSave
			);
		bool SerializeState(
			CArchive & ar
			);
	private:
		void AssignFromOther(
			const cmd_profile_t & other
			)
		{
			m_sName = other.m_sName;
			m_nRarelyPercent = other.m_nRarelyPercent;
			m_nTotalTickCount = other.m_nTotalTickCount;

			_RemoveAllCmdsImpl();
			POSITION pos = other.m_cmds.GetStartPosition();
			for( ; pos != NULL; )
			{
				UINT nCmdID;
				cmd_t * p_cmd = NULL;
				other.m_cmds.GetNextAssoc( pos, nCmdID, p_cmd);
				ASSERT( p_cmd != NULL );
				m_cmds[ nCmdID  ] = new cmd_t( *p_cmd );
			}

			_RemoveAllIconsImpl();
			for(int iIcon=0; iIcon<other.m_icons.GetSize(); iIcon++ )
			{
				icon_t * p_icon = other.m_icons[iIcon];
				ASSERT( p_icon != NULL );
				m_icons.Add( new icon_t( *p_icon ) );
			}
		};
		void _RemoveAllCmdsImpl()
		{
			POSITION pos = m_cmds.GetStartPosition();
			for( ; pos != NULL; )
			{
				UINT nCmdID;
				cmd_t * p_cmd = NULL;
				m_cmds.GetNextAssoc( pos, nCmdID, p_cmd);
				ASSERT( p_cmd != NULL );
				delete p_cmd;
			}
			m_cmds.RemoveAll();
		};
		void _RemoveAllIconsImpl()
		{
			for( INT iIcon=0; iIcon<m_icons.GetSize(); iIcon++ )
			{
				icon_t * p_icon = m_icons[iIcon];
				ASSERT( p_icon != NULL );
				delete p_icon;
			}
			m_icons.RemoveAll();
		};
	}; // class cmd_profile_t

	static bool g_bDisableCmdIfNoHandler;

private:

	typedef
		CMapStringToPtr
		profile_container_t;
	profile_container_t m_profiles;

	typedef
		CMap< HWND, HWND, cmd_profile_t *, cmd_profile_t *& >
		profile_wnd_container_t;
	profile_wnd_container_t m_profile_wnds;

	void _RemoveAllProfilesImpl()
	{
		POSITION pos = m_profiles.GetStartPosition();
		for( ; pos != NULL; )
		{
			CString sProfileName;
			cmd_profile_t * p_profile = NULL;
			m_profiles.GetNextAssoc( pos, sProfileName, (void *&)p_profile );
			ASSERT( p_profile != NULL );
			delete p_profile;
		}
		m_profiles.RemoveAll();
		m_profile_wnds.RemoveAll();
	};

	CCriticalSection m_cs;

public:
	// global auto-pointer class for CExtCmdManager
	class __PROF_UIS_API CExtCmdManagerAutoPtr
	{
		CExtCmdManager * m_pCmdMenager;
		static volatile DWORD g_dwVersion;
	public:

		CExtCmdManagerAutoPtr();
		~CExtCmdManagerAutoPtr();

		// command maneger instance access
		CExtCmdManager * operator->();

		// Prof-UIS version info
		static DWORD GetVersionDWORD(
			bool bForSerialization = false
			);
		static LPCTSTR GetVersionString(
			CString & strBuff = CString(),
			TCHAR tchrSeparator = _T('.')
			);
	}; // class CExtCmdManagerAutoPtr

public:
	// construction/destruction
	CExtCmdManager();
	~CExtCmdManager();

public:
#define __EXTMFC_DEF_PROFILE_NAME _T("default")
	// setup single profile
	bool ProfileSetup(
		LPCTSTR sProfileName = NULL,
		HWND hProfileWnd = NULL
		);
	// get profile
	CExtCmdManager::cmd_profile_t * ProfileGetPtr(
		LPCTSTR sProfileName = NULL
		);
	// setup profile window
	bool ProfileWndAdd(
		LPCTSTR sProfileName,
		HWND hProfileWnd
		);
	// remove profile window
	bool ProfileWndRemove(
		HWND hProfileWnd
		);
	// get profile name for window
	LPCTSTR ProfileNameFromWnd(
		HWND hWnd
		);

	// setup single command
	bool CmdSetup(
		LPCTSTR sProfileName,
		const CExtCmdManager::cmd_t & cmd,
		bool bReplaceOld = false, // but force set images anywhere if was empty
		bool * pbWasAddedNew = NULL
		);
	// remove single command
	bool CmdRemove(
		LPCTSTR sProfileName,
		UINT nCmdID,
		bool * pbWasRemoved = NULL
		);
	// get command
	CExtCmdManager::cmd_t * CmdGetPtr(
		LPCTSTR sProfileName,
		UINT nCmdID
		);
	// alloc command
	CExtCmdManager::cmd_t * CmdAllocPtr(
		LPCTSTR sProfileName,
		UINT nCmdID = 0 // 0 means any free in avail range
		);
	// is command registered
	bool CmdIsRegistered(
		LPCTSTR sProfileName,
		UINT nCmdID
		);
	// get command icon (if command and its icon exist)
	CExtCmdManager::icon_t * CmdGetIconPtr(
		LPCTSTR sProfileName,
		UINT nCmdID
		);
	// get command hIcon
	HICON CmdGetHICON(
		LPCTSTR sProfileName,
		UINT nCmdID
		)
	{
		CExtCmdManager::icon_t * p_icon =
			CmdGetIconPtr(sProfileName,nCmdID);
		if( p_icon == NULL )
			return NULL;
		ASSERT( !(p_icon->IsEmpty()) );
		HICON hIcon = (*p_icon);
		ASSERT( hIcon != NULL );
		return hIcon;
	};

	// update commands coolection from menu handle
	bool UpdateFromMenu(
		LPCTSTR sProfileName,
		HMENU hMenu,
		bool bReplaceOld = false,
		bool bRecursive = true,
		bool bLoadTips = true
		);
	// update commands coolection from menu resurce
	bool UpdateFromMenu(
		LPCTSTR sProfileName,
		UINT nResourceID,
		bool bReplaceOld = false,
		bool bLoadTips = true
		);
	// update commands coolection from toolbar wnd
	bool UpdateFromToolBar(
		LPCTSTR sProfileName,
		CToolBar & bar,
		bool bReplaceOld = false, // but force set images anywhere if was empty
		bool bLoadTips = true
		);
	// update commands coolection from toolbar resource
	bool UpdateFromToolBar(
		LPCTSTR sProfileName,
		UINT nResourceID,
		bool bReplaceOld = false, // but force set images anywhere if was empty
		bool bLoadTips = true
		);
	// set list of commands (up to (UINT)0) as basic or non basic
	bool SetBasicCommands(
		LPCTSTR sProfileName,
		UINT * pCommands,
		bool bOn = true
		);

	// save/load command manager state
	bool SerializeState(
		LPCTSTR sProfileName,
		LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		LPCTSTR sSectionNameProduct, // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		bool bSave
		);
	bool SerializeState(
		LPCTSTR sProfileName,
		CArchive & ar
		);

	static bool FileObjToRegistry(
		CFile & _file,
		LPCTSTR sRegKeyPath
		);
	static bool FileObjFromRegistry(
		CFile & _file,
		LPCTSTR sRegKeyPath
		);

	static CString GetSubSystemRegKeyPath(
		LPCTSTR sSubSystemName,
		LPCTSTR sProfileName,
		LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
		LPCTSTR sSectionNameProduct // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
		);

	friend class cmd_t;
	friend class cmd_profile_t;
}; // class CExtCmdManager

extern __PROF_UIS_API CExtCmdManager::CExtCmdManagerAutoPtr g_CmdManager;

#endif // __ExtCmdManager_H

