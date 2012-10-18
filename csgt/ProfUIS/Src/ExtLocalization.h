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

#if (!defined __EXT_LOCALIZATION_H)
#define __EXT_LOCALIZATION_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

class __PROF_UIS_API CExtLocalResourceHelper
{
	HINSTANCE m_hInstPrevResource;
	bool m_bFree;
	void Init(
		HINSTANCE hInstResourceModule
		)
	{
		ASSERT( m_bFree );
		m_hInstPrevResource =
			::AfxGetResourceHandle();
		::AfxSetResourceHandle(
			hInstResourceModule
			);
		m_bFree = false;
	};
	void Init(
		LPCTSTR sResourceModulePath
		)
	{
		HINSTANCE hInstResourceModule =
			::GetModuleHandle(
				sResourceModulePath
				);
		Init( hInstResourceModule );
	};
public:
	CExtLocalResourceHelper()
		: m_hInstPrevResource( NULL )
		, m_bFree( true )
	{
		Init( __PROF_UIS_MODULE_NAME );
	};
	CExtLocalResourceHelper(
		HINSTANCE hInstResourceModule
		)
		: m_hInstPrevResource( NULL )
		, m_bFree( true )
	{
		Init( hInstResourceModule );
	};
	CExtLocalResourceHelper(
		LPCTSTR sResourceModulePath
		)
		: m_hInstPrevResource( NULL )
		, m_bFree( true )
	{
		Init( sResourceModulePath );
	};
	~CExtLocalResourceHelper()
	{
		Free();
	};
	void Free()
	{
		if( m_bFree )
			return;
		::AfxSetResourceHandle(
			m_hInstPrevResource
			);
		m_hInstPrevResource = NULL;
		m_bFree = true;
	};
}; // class CExtLocalResourceHelper

#endif // __EXT_LOCALIZATION_H
