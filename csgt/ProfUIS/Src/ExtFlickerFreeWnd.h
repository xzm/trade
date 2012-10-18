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

#if (!defined __ExtFlickerFreeWnd_H)
#define __ExtFlickerFreeWnd_H

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

template <
	class _CExtFlickerFreeWndBase = CWnd,
	bool _bExludeChildAreas = true,
	bool _bEat_WM_ERASEBKGND = true
	>
class CExtFlickerFreeWnd : public _CExtFlickerFreeWndBase
{
public:
	virtual LRESULT WindowProc( 
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
		)
	{
		switch( message )
		{
		case WM_ERASEBKGND:
			if( _bEat_WM_ERASEBKGND )
				return FALSE;
		break;
		case WM_PAINT:
			{
				CPaintDC dcPaint( this );
				if( _bExludeChildAreas )
				{
					CExtPaintManager::stat_ExcludeChildAreas(
						dcPaint.GetSafeHdc(),
						GetSafeHwnd()
						);
				}
				CExtMemoryDC dc( &dcPaint );
				DefWindowProc(
					WM_PAINT,
					WPARAM( dc.GetSafeHdc() ),
					LPARAM(0)
					);
			}
			return TRUE;
		};
		return _CExtFlickerFreeWndBase::WindowProc( message, wParam, lParam );
	};
}; // class CExtFlickerFreeWnd

#endif // __ExtFlickerFreeWnd_H
