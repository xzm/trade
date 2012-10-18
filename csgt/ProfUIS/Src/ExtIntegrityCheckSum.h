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

#if (!defined __EXT_EXTINTEGRITYCHECKSUM_H)
#define __EXT_EXTINTEGRITYCHECKSUM_H


#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#ifndef __AFXCONV_H__
	#include <AfxConv.h>
#endif

#include <io.h>

class __PROF_UIS_API CExtIntegrityCheckSum
{
public:
	static bool statIsFileExists(
		LPCTSTR sFilePath
		)
	{
		ASSERT(sFilePath != NULL);
		if(sFilePath == NULL)
			return false;
#if (defined _UNICODE)
			struct _wfinddata_t fd;
#else
			struct _finddata_t fd;
#endif
		long hNextFile =
#if (defined _UNICODE)
				_wfindfirst(
#else
				_findfirst(
#endif
					sFilePath,
					&fd
					);
		bool bExists = true;
		if(hNextFile < 0)
			bExists = false;
		else
		{
			if((fd.attrib&_A_SUBDIR)!=0)
				bExists = false;
		} // else from if(hNextFile < 0)
		_findclose(hNextFile);
		return bExists;
	};


public:
	//constructor/destructor
	CExtIntegrityCheckSum()
	{
		Init();
	};
	~CExtIntegrityCheckSum()
	{
	};

protected:

	int m_nCurrent;
	DWORD m_dwHashStarts[4];

	void Init()
	{
		m_nCurrent = 0L;
		::memset( m_dwHashStarts, 0, sizeof(m_dwHashStarts) );
	}
	void JumpNextItem()
	{
		m_nCurrent++;
		if( m_nCurrent < sizeof(m_dwHashStarts) / sizeof(m_dwHashStarts[0]) )
			return;
		m_nCurrent = 0L;
	}

public:
	void Update(
		const BYTE * pInput,
		ULONG nInputLen
		)
	{
		for( ULONG nIdx = 0; nIdx < nInputLen; nIdx++, pInput++ )
		{
			DWORD & dwHashItem = m_dwHashStarts[ m_nCurrent ];
			dwHashItem = ( dwHashItem << 4 ) + (*pInput);
			DWORD dwRecalc;
			if( (dwRecalc = dwHashItem & 0xF0000000L ) != 0L ) 
			{
				dwHashItem ^= dwRecalc >> 24;
				dwHashItem ^= dwRecalc;
			}
			JumpNextItem();
		}
	}

	CString Final()
	{
		CString sHash( _T("") );
		for(	int nIdx = 0;
				nIdx < sizeof(m_dwHashStarts) / sizeof(m_dwHashStarts[0]);
				nIdx++
				)
		{
			CString sTmp;
			sTmp.Format(
				_T("%s%02X %02X %02X %02X"),
				(nIdx > 0) ? _T(" ") :_T(""), 
				( int( m_dwHashStarts[ nIdx ] ) >> 24 ) & 0x0FF,
				( int( m_dwHashStarts[ nIdx ] ) >> 16 ) & 0x0FF,
				( int( m_dwHashStarts[ nIdx ] ) >>  8 ) & 0x0FF,
				  int( m_dwHashStarts[ nIdx ] )         & 0x0FF
				);
			sHash += sTmp;
		}
		return sHash;
	}

}; // class CExtIntegrityCheckSum

#endif // __EXT_EXTINTEGRITYCHECKSUM_H

