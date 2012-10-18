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

// ExtPopupMenuWnd.cpp : implementation file
//

#include "stdafx.h"

#define _AFX_NO_OLE_SUPPORT

#ifndef __AFXPRIV_H__
	#include <AfxPriv.h>
#endif

#include <math.h>

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#if (!defined __EXT_TOOLCONTROLBAR_H)
	#include <ExtToolControlBar.h>
#endif

#if (!defined __ExtCmdManager_H)
	#include <ExtCmdManager.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
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

#define ID_TIMER_ANIMATION 666
#define ID_PERIOD_ANIMATION 20	// 30

#define ID_TIMER_ITEM_FOCUS_DELAY 667
#define ID_PERIOD_ITEM_FOCUS_DELAY 500

#define ID_TIMER_SCROLLING 668
#define ID_PERIOD_SCROLLING 10

#define ID_TIMER_DELAY_SHOW 669
#define ID_PERIOD_DELAY_SHOW 500	// 1000

#define ID_TIMER_DELAY_EXPAND 670
#define ID_FREQ_DELAY_EXPAND 50
#define ID_TOTAL_DELAY_EXPAND 1500

#define __SCROLLING_PIXEL_STEP 3

static const int g_nAnimStepMetric = 10;
static clock_t g_nLastAnimTime = 0;

#define __ANIM_CY 15

#define __EXCLUDE_AREA_GAP_DX 3
#define __EXCLUDE_AREA_GAP_DY __EXCLUDE_AREA_GAP_DX

#define __MI_HORZ_GAP_TO_BORDER 1

#define __BOX_ANIM_METRIC 32 // for __AT_BOXES,__AT_CIRCLES animation

static CExtPopupMenuWnd::CExtPopupMenuWndAutoPtr g_CurrentPopup;

CExtPopupMenuWnd * CExtPopupMenuWnd::g_pWndCapture = NULL;

UINT CExtPopupMenuWnd::g_nMsgPrepareMenu =
	::RegisterWindowMessage(
		_T("CExtPopupMenuWnd::g_nMsgPrepareMenu")
		);

bool CExtPopupMenuWnd::g_bMenuWithShadows = true; // allow shadows
bool CExtPopupMenuWnd::g_bMenuExpanding = true; // allow hide rarely used
bool CExtPopupMenuWnd::g_bMenuHighlightRarely = true; // display rarely used in different style
bool CExtPopupMenuWnd::g_bMenuShowCoolTips = true; // allow display cool tooltips
bool CExtPopupMenuWnd::g_bMenuExpandAnimation = true; // allow animation when show rarely used

bool CExtPopupBaseWnd::g_bUseDesktopWorkArea = true; // align to desktop work area (false - to screen area)

CExtPopupBaseWnd::e_animation_type_t
	CExtPopupBaseWnd::g_DefAnimationType =
		CExtPopupBaseWnd::__AT_FADE; // __AT_RANDOM

static inline int _GetMenuShadowSize()
{
	if( !CExtPopupMenuWnd::g_bMenuWithShadows )
		return 0;
//int nBitsPerPixel = CExtPaintManager::stat_GetBPP();
//	if( nBitsPerPixel <= 8 )
//		return 0;
	return g_PaintManager->GetMenuShadowSize();
};

HHOOK CExtPopupMenuWnd::g_hMouseHook = NULL;
HHOOK CExtPopupMenuWnd::g_hKeyboardHook = NULL;

HWND CExtPopupMenuWnd::g_hWndCmdReciever = NULL;

UINT CExtPopupBaseWnd::nMsgPopupNext =
	::RegisterWindowMessage(
		_T("CExtPopupBaseWnd::nMsgPopupNext")
		);
UINT CExtPopupBaseWnd::nMsgPopupPrev =
	::RegisterWindowMessage(
		_T("CExtPopupBaseWnd::nMsgPopupPrev")
		);
UINT CExtPopupBaseWnd::nMsgNotifyMenuExpanded =
	::RegisterWindowMessage(
		_T("CExtPopupBaseWnd::nMsgNotifyMenuExpanded")
		);
//UINT CExtPopupBaseWnd::nMsgNotifyMenuClosed =
//	::RegisterWindowMessage(
//		_T("CExtPopupMenuWnd::nMsgNotifyMenuClosed")
//		);

#define __POPUP_WNDCLASS_STYLES__ \
	( CS_SAVEBITS )
//	( CS_SAVEBITS \
//		|CS_HREDRAW \
//		|CS_VREDRAW \
//		|CS_NOCLOSE \
//	)

/////////////////////////////////////////////////////////////////////////////
// CExtWndShadow

CExtWndShadow::CExtWndShadow()
{
	Destroy();
}

CExtWndShadow::~CExtWndShadow()
{
	Destroy();
}

void CExtWndShadow::_DoPixelOvershadow(
	int nMakeSpec,
	int nPosX,
	int nPosY
	)
{
	ASSERT( nMakeSpec >= 0 && nMakeSpec <= 100 );
	ASSERT( !m_rcWndArea.IsRectEmpty() );
	ASSERT( nPosX < m_rcWndArea.Width() + INT(m_nShadowSize) );
	ASSERT( nPosY < m_rcWndArea.Height() + INT(m_nShadowSize) );
	ASSERT( m_pHelperDibSurface != NULL );
int nTotalWidth = m_rcWndArea.Width()+m_nShadowSize;
int nTotalHeight = m_rcWndArea.Height()+m_nShadowSize;
COLORREF * ptr =
		m_pHelperDibSurface
		+nPosX+(nTotalHeight-nPosY)*nTotalWidth;
	*ptr =
		RGB(
			(( nMakeSpec * int(GetRValue(*ptr)) ) / 100),
			(( nMakeSpec * int(GetGValue(*ptr)) ) / 100),
			(( nMakeSpec * int(GetBValue(*ptr)) ) / 100)
			);
}

bool CExtWndShadow::Restore( CDC & dc )
{
	ASSERT( m_nShadowSize >= 0 );
	if( m_nShadowSize == 0 || m_rcWndArea.IsRectEmpty () )
		return true;
	if(		m_bmp0.GetSafeHandle() == NULL
		||	m_bmp1.GetSafeHandle() == NULL
		)
		return false;

INT nWndAreaDX = m_rcWndArea.Width ();
INT nWndAreaDY = m_rcWndArea.Height ();
	ASSERT( nWndAreaDX > 0 && nWndAreaDY > 0 );
CDC dcmm;
	if( !dcmm.CreateCompatibleDC(&dc) )
	{
		ASSERT( FALSE );
		return false;
	}
CBitmap * pbmpold = dcmm.SelectObject( &m_bmp0 );
	ASSERT( pbmpold != NULL );
	dc.BitBlt(
		m_rcWndArea.right, m_rcWndArea.top,
		m_nShadowSize, nWndAreaDY+m_nShadowSize,
		&dcmm,
		0, 0,
		SRCCOPY
		);
	dcmm.SelectObject( &m_bmp1 );
	dc.BitBlt(
		m_rcWndArea.left, m_rcWndArea.bottom,
		nWndAreaDX+m_nShadowSize, m_nShadowSize,
		&dcmm,
		0, 0,
		SRCCOPY
		);
	dcmm.SelectObject( pbmpold );
	return true;
}

bool CExtWndShadow::Paint(
	CDC & dc,
	const CRect & rcWndArea,
	const CRect & rc1stArea, // = CRect(0,0,0,0)
	const CRect & rc2ndArea, // = CRect(0,0,0,0)
	UINT nShadowSize, // = DEF_SHADOW_SIZE
	UINT nBr0, // = DEF_BRIGHTNESS_MIN
	UINT nBr1, // = DEF_BRIGHTNESS_MAX
	bool bEnablePhotos // = true
	)
{
	m_rcWndArea = rcWndArea;
	m_rc1stArea = rc1stArea;
	m_rc2ndArea = rc2ndArea;
	m_nShadowSize = nShadowSize;
	m_nBr0 = nBr0;
	m_nBr1 = nBr1;
	m_bEnablePhotos = bEnablePhotos;
	return Paint( dc );
}

bool CExtWndShadow::Paint( CDC & dc )
{
	ASSERT( m_nShadowSize >= 0 );
	ASSERT( !m_rcWndArea.IsRectEmpty() );
	if( m_nShadowSize == 0 || m_rcWndArea.IsRectEmpty() )
		return true;

	if( Restore( dc ) )
		return true;

	if( CExtPaintManager::stat_GetBPP() > 8 )
		return _PaintHi( dc );
	return _PaintLo( dc );
}

bool CExtWndShadow::_PaintLo( CDC & dc )
{
INT nWndAreaDX = m_rcWndArea.Width();
INT nWndAreaDY = m_rcWndArea.Height();
	ASSERT(
			m_bmp0.GetSafeHandle() == NULL
		&&	m_bmp1.GetSafeHandle() == NULL
		);
static int _Pattern[] =
{
	~0xAA,
	~0x55,
	~0xAA,
	~0x55,
	~0xAA,
	~0x55,
	~0xAA,
	~0x55
};
CBitmap bmpsh;
CBrush brsh;
	if(		!bmpsh.CreateBitmap(8, 8, 1, 1, _Pattern)
		||	!brsh.CreatePatternBrush( &bmpsh )
		)
		return false;
CExtMemoryDC dcmm(
		&dc,
		NULL,
		CExtMemoryDC::MDCOPT_TO_MEMORY
		);
	ASSERT( dcmm.GetSafeHdc() != NULL );
	if( dcmm.GetSafeHdc() == NULL )
		return false;
	dcmm.BitBlt(
		m_rcWndArea.left, m_rcWndArea.top,
		nWndAreaDX+m_nShadowSize, nWndAreaDY+m_nShadowSize,
		&dc,
		m_rcWndArea.left, m_rcWndArea.top,
		SRCCOPY
		);
UINT nPaintShadowSize =
		( m_nShadowSize > 4 )
			? 4
			: m_nShadowSize;
CRect rV(
		 m_rcWndArea.right,
		 m_rcWndArea.top+nPaintShadowSize,
		 m_rcWndArea.right+nPaintShadowSize,
		 m_rcWndArea.bottom
		 );
CRect rH(
		 m_rcWndArea.left+nPaintShadowSize,
		 m_rcWndArea.bottom,
		 m_rcWndArea.right+nPaintShadowSize,
		 m_rcWndArea.bottom+nPaintShadowSize
		 );
CBrush * pbrold = dcmm.SelectObject( &brsh );
	dcmm.PatBlt( rH.left, rH.top, rH.Width(), rH.Height(), 0xA000C9 );
	dcmm.PatBlt( rV.left, rV.top, rV.Width(), rV.Height(), 0xA000C9 );
	dcmm.SelectObject( pbrold );
	dc.BitBlt(
		m_rcWndArea.left, m_rcWndArea.top,
		nWndAreaDX+m_nShadowSize, nWndAreaDY+m_nShadowSize, 
		&dcmm,
		m_rcWndArea.left, m_rcWndArea.top,
		SRCCOPY
		);
	if( m_bEnablePhotos )
	{
		if( !_MakePhotos(dc,dcmm) )
		{
			ASSERT( FALSE );
			return false;
		}
	}
	dcmm.__Flush( FALSE );
	return true;
}

bool CExtWndShadow::_PaintHi( CDC & dc )
{
INT nWndAreaDX = m_rcWndArea.Width();
INT nWndAreaDY = m_rcWndArea.Height();
	ASSERT(
			m_bmp0.GetSafeHandle() == NULL
		&&	m_bmp1.GetSafeHandle() == NULL
		);
CDC dcmm;
	if( !dcmm.CreateCompatibleDC( &dc ) )
	{
		ASSERT( FALSE );
		return false;
	}

BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = nWndAreaDX+m_nShadowSize;
	bih.biHeight = nWndAreaDY+m_nShadowSize;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = (nWndAreaDX+m_nShadowSize) * (nWndAreaDY+m_nShadowSize);
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	ASSERT( m_pHelperDibSurface == NULL );
HBITMAP hDIB =
		::CreateDIBSection(
			dcmm.GetSafeHdc(),
			(LPBITMAPINFO)&bih,
			DIB_RGB_COLORS,
			(void **)&m_pHelperDibSurface,
			NULL,
			NULL
			);
	if( hDIB == NULL || m_pHelperDibSurface == NULL )
	{
		ASSERT( FALSE );
		return false;
	}

	// MODIFIED BY SUNZHENYU, modified next 1 line
	HBITMAP hOldBitmap = (HBITMAP)dcmm.SelectObject( hDIB );
	dcmm.BitBlt(
		0, 0,
		nWndAreaDX+m_nShadowSize, nWndAreaDY+m_nShadowSize,
		&dc,
		m_rcWndArea.left, m_rcWndArea.top,
		SRCCOPY
		);

UINT nBrDiff = m_nBr1-m_nBr0;
LONG n2nd, nStep, nDist, nDist1;
LONG nDist2 = m_nShadowSize*m_nShadowSize;
INT nMakeSpec;
bool	bCmbaV = false, bCmbaH = false,
		bCmbaVrt = false, bCmbaVrb = false,
		bCmbaHrb = false, bCmbaHlb = false,
		bCmbaLA = false;
INT		nX0 = 0, nX1 = nWndAreaDX,
		nY0 = 0, nY1 = nWndAreaDY,
		nLa0 = 0, nLa1 = 0;
	if( !( m_rc1stArea.IsRectEmpty() || m_rc2ndArea.IsRectEmpty() ) )
	{
		bool bCmbaDetected = false;
		if( m_rc1stArea.right == m_rc2ndArea.right )
		{ // vertical-right combining
			bCmbaDetected = true;
			if( m_rc1stArea.top < m_rc2ndArea.top )
				bCmbaVrb = true;
			else
				bCmbaVrt = true;
		} // vertical-right combining
		if( (!bCmbaDetected) &&
			m_rc1stArea.left == m_rc2ndArea.left )
		{ // vertical-left combining
			if( m_rc2ndArea.bottom >= m_rc1stArea.top-1
				&& m_rc2ndArea.top < m_rc1stArea.top
				)
			{
				if( m_rc2ndArea.right >= m_rc1stArea.right )
				{
					bCmbaDetected = true;
					bCmbaV = true;
				}
			}
			if(	(!bCmbaDetected)
				&& m_rc1stArea.bottom >= m_rc2ndArea.top-1 
				&& m_rc1stArea.top < m_rc2ndArea.top
				)
			{
				if( m_rc1stArea.right >= m_rc2ndArea.right )
				{
					bCmbaDetected = true;
					bCmbaH = true;
					nX0 =
						m_rcWndArea.right  -
						(m_rc1stArea.right - m_rc2ndArea.right);
				}
			}
		} // vertical-left combining
		if( (!bCmbaDetected) &&
			m_rc1stArea.bottom == m_rc2ndArea.bottom
			)
		{ // horizontal-bottom combining
			bCmbaDetected = true;
			if( m_rc1stArea.left < m_rc2ndArea.left )
				bCmbaHrb = true;
			else
				bCmbaHlb = true;
		} // horizontal-bottom combining
		if( (!bCmbaDetected) &&
			m_rc1stArea.right == m_rc2ndArea.left+1
			)
		{
			bCmbaDetected = true;
			bCmbaLA = true;
			nLa0 = m_rcWndArea.top
				+ m_rc2ndArea.bottom - m_rc1stArea.top;
			nLa1 = nLa0 + m_nShadowSize;
		}
		if( (!bCmbaDetected) &&
			m_rc1stArea.left == m_rc2ndArea.right-1
			)
		{
			bCmbaDetected = true;
			bCmbaH = true;
		}
		bCmbaDetected;
	} // if( !( m_rc1stArea.IsRectEmpty() || m_rc2ndArea.IsRectEmpty() ) )
	for( nStep = 0; ULONG(nStep) < m_nShadowSize; nStep++ )
	{
		nMakeSpec = m_nBr0+(nStep*nBrDiff)/m_nShadowSize;
		for(	n2nd = nX0
					+ m_nShadowSize*2+1
					- ( bCmbaH ? (m_nShadowSize*2-nStep) : 0 )
					- ( bCmbaHlb ? m_nShadowSize : 0 )
					;
				n2nd < LONG(nX1) + (bCmbaHrb ? LONG(m_nShadowSize) : 0);
				n2nd++
				)
			_DoPixelOvershadow(
				nMakeSpec,
				n2nd,
				nY1+nStep
				);
		for(	n2nd = nY0
					+ m_nShadowSize*2+1
					- ( bCmbaV ? (m_nShadowSize*2-nStep) : 0 )
					- ( bCmbaVrt ? m_nShadowSize : 0 )
					;
				n2nd < LONG(nY1) + (bCmbaVrb ? LONG(m_nShadowSize) : 0);
				n2nd++
				)
		{
			if( bCmbaLA
				&& n2nd >= nLa0
				&& n2nd < LONG(nLa1) - (LONG(m_nShadowSize) - nStep)
				)
			{
				continue;
			}
			_DoPixelOvershadow(
				nMakeSpec,
				nX1+nStep,
				n2nd
				);
		}
		nDist1 = nStep*nStep;
		for( n2nd = 0; ULONG(n2nd) < m_nShadowSize; n2nd++ )
		{
			nDist = nDist1 + n2nd*n2nd;
			if( nDist > nDist2 )
				continue;
			nDist = (LONG)::sqrt( double(nDist) );
			nMakeSpec = m_nBr0+(nDist*nBrDiff)/m_nShadowSize;
			if( !(bCmbaVrb || bCmbaHrb) )
				_DoPixelOvershadow(
					nMakeSpec,
					nX1+nStep,
					nY1+n2nd
					);
			if( !(bCmbaH || bCmbaHlb) )
				_DoPixelOvershadow(
					nMakeSpec,
					m_nShadowSize+(m_nShadowSize-nStep),
					nY1+n2nd
					);
			if( !(bCmbaV || bCmbaVrt) )
				_DoPixelOvershadow(
					nMakeSpec,
					nX1+nStep,
					m_nShadowSize+(m_nShadowSize-n2nd)
					);
		} // for( n2nd = 0; ULONG(n2nd) < m_nShadowSize; n2nd++ )
	} // for( nStep = 0; ULONG(nStep) < m_nShadowSize; nStep++ )
	dc.BitBlt(
		m_rcWndArea.left, m_rcWndArea.top,
		nWndAreaDX+m_nShadowSize, nWndAreaDY+m_nShadowSize, 
		&dcmm,
		0, 0,
		SRCCOPY
		);

	if( m_bEnablePhotos )
	{
		if( !_MakePhotos(dc,dcmm) )
		{
			ASSERT( FALSE );
			return false;
		}
	}

	// MODIFIED BY SUNZHENYU, add next 1 line
	dcmm.SelectObject( hOldBitmap );
	::DeleteObject( hDIB );
	m_pHelperDibSurface = NULL;

	return true;
}

bool CExtWndShadow::_MakePhotos( CDC & dc, CDC & dcmm )
{
	ASSERT( m_nShadowSize >= 0 );
	ASSERT( !m_rcWndArea.IsRectEmpty() );
INT nWndAreaDX = m_rcWndArea.Width();
INT nWndAreaDY = m_rcWndArea.Height();
	ASSERT(
			m_bmp0.GetSafeHandle() == NULL
		&&	m_bmp1.GetSafeHandle() == NULL
		);
	if(	!m_bmp0.CreateCompatibleBitmap(
			&dc, m_nShadowSize, nWndAreaDY+m_nShadowSize
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	// MODIFIED BY SUNZHENYU, modified next 1 line
	CBitmap * pOldBitmap = dcmm.SelectObject( &m_bmp0 );
	if(	!dcmm.BitBlt(
			0, 0, m_nShadowSize, nWndAreaDY+m_nShadowSize,
			&dc,
			m_rcWndArea.right, m_rcWndArea.top,
			SRCCOPY
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	if(	!m_bmp1.CreateCompatibleBitmap(
			&dc, nWndAreaDX+m_nShadowSize, m_nShadowSize
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	dcmm.SelectObject( &m_bmp1 );
	if(	!dcmm.BitBlt(
			0, 0, nWndAreaDX+m_nShadowSize, m_nShadowSize,
			&dc,
			m_rcWndArea.left, m_rcWndArea.bottom,
			SRCCOPY
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	// MODIFIED BY SUNZHENYU, add next 1 line
	dcmm.SelectObject( pOldBitmap );
	return true;
}

void CExtWndShadow::_FreeWinObjects()
{
	if( m_bmp0.GetSafeHandle() != NULL )
		m_bmp0.DeleteObject();
	if( m_bmp1.GetSafeHandle() != NULL )
		m_bmp1.DeleteObject();
	m_pHelperDibSurface = NULL;
}

void CExtWndShadow::Destroy()
{
	_FreeWinObjects();
	m_rcWndArea.SetRectEmpty();
	m_bEnablePhotos = false;
}

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuWnd::menu_item_info_t

CExtPopupMenuWnd::menu_item_info_t::menu_item_info_t()
{
	// self constant type assertion
	ASSERT( TYPE_SEPARATOR==ID_SEPARATOR );
	m_pWndChild = NULL;
	m_nItemIndex = -100;
	m_nCmdID = TYPE_SEPARATOR;
	m_sPopupText = m_sPopupAccelText = _T("");
	m_hIconPopup = NULL;
	m_size.cx = m_size.cy = 0;
	m_bSelected
		= m_bDisplayed
		= false;
	m_cAccelChar = 0;
	m_hWndSpecCmdReciever = NULL;
}

CExtPopupMenuWnd::menu_item_info_t::~menu_item_info_t()
{
	// DestroyPopup();
}

void CExtPopupMenuWnd::menu_item_info_t::AccelCharInit()
{
	m_cAccelChar = 0;
	CString sDisplayText( GetText() );
	if( sDisplayText.IsEmpty() )
		return;
	int nIndex = sDisplayText.Find('&');
	if( nIndex + 1 == sDisplayText.GetLength() )
	{
		TRACE(_T("warning : & is bad position, access key is invalid.\n"));
		return;
	}
	if( nIndex < 0 )
	{
		if( !CExtPopupMenuWnd::g_bAllowNonAccelPositioning )
			return;
		m_cAccelChar = sDisplayText[0];
	}
	else
		m_cAccelChar = sDisplayText[nIndex + 1]; // -1 + 1 = 0; it's ok
	TCHAR szChar[2] = { m_cAccelChar, '\0'};
	::CharUpper( szChar );
	m_cAccelChar = szChar[0];
}

TCHAR CExtPopupMenuWnd::menu_item_info_t::AccelCharGet() const
{
	return m_cAccelChar;
}

bool CExtPopupMenuWnd::menu_item_info_t::AccelCharIsSet() const
{
	return (m_cAccelChar == 0) ? false : true;
}

bool CExtPopupMenuWnd::menu_item_info_t::UpdateCmdManagerCommand(
	CExtCmdManager::cmd_t * p_cmd,
	int nItemIndex
	)
{
	ASSERT( p_cmd != NULL );
	ASSERT( !IsPopup() );
	ASSERT( m_pWndChild == NULL );
	m_sPopupText.Empty();
	m_nCmdID = p_cmd->m_nCmdID;
	if( !CExtCmdManager::IsCommand(m_nCmdID) )
	{
		m_nCmdID = TYPE_SEPARATOR;
		m_nItemIndex = -100;
		ASSERT( FALSE );
		return false;
	}
	if( nItemIndex >= 0 )
		m_nItemIndex = nItemIndex;
	MeasureItem();
	return true;
}

CExtCmdManager::cmd_t * CExtPopupMenuWnd::menu_item_info_t::GetCmd() const
{
	if( IsPopup() || IsSeparator() )
		return NULL;
	CExtCmdManager::cmd_t * p_cmd =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( GetCmdReciever() ),
			m_nCmdID
			);
	ASSERT( p_cmd != NULL );
	return p_cmd;
}

void CExtPopupMenuWnd::menu_item_info_t::MeasureItem()
{
	AccelCharInit();
	if( IsSeparator() )
	{
		m_size.cx = 0;
		m_size.cy = __DEF_MENU_SEPARATOR_HEIGHT;
		return;
	}
	CString sMeasureText( GetText() );
	sMeasureText += GetAccelText();
	sMeasureText.Replace( _T("&"), _T("") );
	CWindowDC dc(NULL);
	CFont * pOldFont =
		dc.SelectObject( &(g_PaintManager->m_FontNormal) );
	ASSERT( pOldFont != NULL );
	CRect rect(0,0,0,0);
	m_size.cy = DrawText(
		dc.GetSafeHdc(),
		(LPCTSTR)sMeasureText,
		sMeasureText.GetLength(),
		&rect,
		DT_CALCRECT|DT_SINGLELINE
			|DT_LEFT|DT_VCENTER
		);
	m_size.cx = rect.Width();
	dc.SelectObject( pOldFont );
	m_size.cx +=
		__DEF_MENU_ICON_CX
		+ __DEF_MENU_GAP*6 // + __DEF_MENU_GAP*5
		+ __DEF_MENU_POPUP_ARROW_AREA_DX
		;
	if( m_size.cy < __DEF_MENU_HEIGHT )
		m_size.cy = __DEF_MENU_HEIGHT;
}

bool CExtPopupMenuWnd::menu_item_info_t::UpdateFromMenu(
	HWND hWndCmdProfileInfo,
	CMenu * pTrackMenu,
	int nItemIndex
	)
{
	ASSERT( pTrackMenu != NULL );
	ASSERT( pTrackMenu->GetSafeHmenu() != NULL );
	
#ifdef _DEBUG
	int	nMenuItemCount = pTrackMenu->GetMenuItemCount();
	ASSERT( nMenuItemCount > 0 );
	ASSERT( nItemIndex >= 0 && nItemIndex<nMenuItemCount );
#endif // _DEBUG

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
	mii.dwTypeData =
		m_sPopupText.GetBuffer(__MAX_UI_ITEM_TEXT);
	ASSERT( mii.dwTypeData != NULL );
	if( mii.dwTypeData == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	if( !pTrackMenu->GetMenuItemInfo(
			nItemIndex,
			&mii,
			TRUE
			)
		)
	{
		m_sPopupText.ReleaseBuffer();
		ASSERT( FALSE );
		return false;
	}
	m_sPopupText.ReleaseBuffer();

	m_nItemIndex = nItemIndex;
	if( (mii.fType&MFT_SEPARATOR) != 0 )
	{
		m_sPopupText.Empty();
		m_nCmdID = TYPE_SEPARATOR;
		MeasureItem();
		return true;
	}
	if( mii.hSubMenu != NULL )
	{
		if( !ConstructPopup() )
		{
			ASSERT( FALSE );
			return false;
		}
		m_nCmdID = TYPE_POPUP;
		MeasureItem();
		return true;
	}

	m_sPopupText.Empty(); // we no need loger this text
	if( !CExtCmdManager::IsCommand(mii.wID) )
	{
		ASSERT( FALSE );
		return false;
	}
	m_nCmdID = mii.wID;
	CExtCmdManager::cmd_t * p_cmd =
		g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( hWndCmdProfileInfo ),
			m_nCmdID
			);
	ASSERT( p_cmd != NULL );
	if( p_cmd == NULL )
		return false;
	MeasureItem();
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CExtPopupBaseWnd

IMPLEMENT_DYNCREATE(CExtPopupBaseWnd, __BaseClassOfCExtPopupBaseWnd)

CExtPopupBaseWnd::CExtPopupBaseWnd()
{
	m_clrFadeBitsSrc
		= m_clrFadeBitsDst
		= m_clrFadeBitsTmp
		= NULL;
	m_AnimationType = g_DefAnimationType;
	_InitAnimation();

	m_sizeFullItems.cx = m_sizeFullItems.cy = 1;

	m_bExcludeAreaSpec
		= m_bCombineWithEA
		= false;
	m_rcExcludeArea.SetRectEmpty();
	m_ptTrack.x = m_ptTrack.y;
}

CExtPopupBaseWnd::~CExtPopupBaseWnd()
{
}

void CExtPopupBaseWnd::_InitAnimation()
{
	m_nAnimPercent = m_nAnimPercentOld = 0;
	//m_AnimationType = g_DefAnimationType;

	if( (m_AnimationType < 0
			|| m_AnimationType > __AT_MAX_VALID_VALUE)
		&&
		m_AnimationType != __AT_CONTENT_DISPLAY
		&&
		m_AnimationType != __AT_CONTENT_EXPAND
		)
		m_AnimationType = __AT_NONE;

	if( m_AnimationType == __AT_RANDOM )
	{
		int nRand = rand() % (__AT_MAX_VALID_VALUE+1);
		ASSERT( nRand >= 0 && nRand <= __AT_MAX_VALID_VALUE );
		if( nRand == __AT_NONE || nRand == __AT_RANDOM )
			nRand = __AT_FADE; // most cool of all effects
		m_AnimationType = (e_animation_type_t)nRand;
	}

int nBitsPerPixel = CExtPaintManager::stat_GetBPP();
	if( nBitsPerPixel <= 8
		&&
		m_AnimationType != __AT_CONTENT_EXPAND
		&&
		m_AnimationType != __AT_CONTENT_DISPLAY
		)
	{
		m_AnimationType = __AT_NONE;
		m_bAnimFinished = true;
	}

	else
	{
		m_bAnimFinished =
			(m_AnimationType == __AT_NONE) ? true : false;
	}

	ASSERT(
			m_bmpScreenDst.GetSafeHandle() == NULL
		&&	m_bmpScreenSrc.GetSafeHandle() == NULL
		&&	m_bmpScreenTmp.GetSafeHandle() == NULL
		&&	m_clrFadeBitsSrc == NULL
		&&	m_clrFadeBitsDst == NULL
		&&	m_clrFadeBitsTmp == NULL
		);
//	_SurfacesDelete();

/*
	m_AnimationType = __AT_NONE;
	m_bAnimFinished = true;
*/
}

void CExtPopupBaseWnd::_StartAnimation()
{
	_InitAnimation();

	if( m_AnimationType != __AT_NONE )
	{
		SetTimer( ID_TIMER_ANIMATION, ID_PERIOD_ANIMATION, NULL );
		g_nLastAnimTime = clock();
	}
}

void CExtPopupMenuWnd::_StartAnimation()
{
	_ItemFocusDelay();
	_CoolTipHide( false );
	CExtPopupBaseWnd::_StartAnimation();
}

BEGIN_MESSAGE_MAP(CExtPopupBaseWnd, __BaseClassOfCExtPopupBaseWnd)
	//{{AFX_MSG_MAP(CExtPopupBaseWnd)
	ON_WM_MOUSEACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_SIZE()
	ON_WM_ACTIVATEAPP()
	ON_WM_CANCELMODE()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExtPopupBaseWnd::OnPaint() 
{
	if( !(::IsWindow(GetSafeHwnd())) )
		return;
	if( !(::IsWindowVisible(GetSafeHwnd())) )
		return;
CPaintDC dc(this); // device context for painting
//	_BitsSave( dc );

	if( !m_bAnimFinished )
		_DrawAnimatedState( dc );
	else
		_DoPaint( dc );
}

void CExtPopupBaseWnd::_DrawAnimatedState( CDC & dc )
{
	_DoPaint( dc );
}

void CExtPopupMenuWnd::_DrawAnimatedState( CDC & dc )
{
	if( !(::IsWindow(GetSafeHwnd())) )
		return;
CRect rcClient;
	GetClientRect( &rcClient );

int cx = rcClient.Width();
int cy = rcClient.Height();

CDC * pPaintDC = &dc;
CDC dcmm;
	if( !dcmm.CreateCompatibleDC(pPaintDC) )
	{
		return;
	}

	if( m_bmpScreenDst.GetSafeHandle() == NULL )
	{
		ASSERT( m_bmpScreenSrc.GetSafeHandle() == NULL );
		ASSERT( m_bmpScreenTmp.GetSafeHandle() == NULL );
		
		int nBitsPerPixel = CExtPaintManager::stat_GetBPP();
		CBitmap * pBmpOld = NULL;
		if(	(m_AnimationType == __AT_FADE
				&& nBitsPerPixel > 8 )
			|| m_AnimationType == __AT_CONTENT_EXPAND
			|| m_AnimationType == __AT_CONTENT_DISPLAY
			|| m_AnimationType == __AT_ROLL
			|| m_AnimationType == __AT_SLIDE
			|| m_AnimationType == __AT_ROLL_AND_SREETCH
			|| m_AnimationType == __AT_SLIDE_AND_SREETCH
			|| m_AnimationType == __AT_NOISE
			|| m_AnimationType == __AT_BOXES
			|| m_AnimationType == __AT_CIRCLES
			|| m_AnimationType == __AT_HOLES
			)
		{
			BITMAPINFOHEADER bih;
			bih.biSize = sizeof(BITMAPINFOHEADER);
			bih.biWidth = cx;
			bih.biHeight = cy;
			bih.biPlanes = 1;
			bih.biBitCount = 32;
			bih.biCompression = BI_RGB;
			bih.biSizeImage = cx * cy;
			bih.biXPelsPerMeter = 0;
			bih.biYPelsPerMeter = 0;
			bih.biClrUsed = 0;
			bih.biClrImportant = 0;

			HBITMAP hDIB =
				::CreateDIBSection(
					dcmm.GetSafeHdc(),
					(LPBITMAPINFO)&bih,
					DIB_RGB_COLORS,
					(void **)&m_clrFadeBitsSrc,
					NULL,
					NULL
					);
			if( hDIB == NULL || m_clrFadeBitsSrc == NULL )
				return;
			m_bmpScreenSrc.Attach( hDIB );

			hDIB =
				::CreateDIBSection(
					dcmm.GetSafeHdc(),
					(LPBITMAPINFO)&bih,
					DIB_RGB_COLORS,
					(void **)&m_clrFadeBitsDst,
					NULL,
					NULL
					);
			if( hDIB == NULL || m_clrFadeBitsDst == NULL )
			{
				m_bmpScreenSrc.DeleteObject();
				return;
			}
			m_bmpScreenDst.Attach( hDIB );

			hDIB =
				::CreateDIBSection(
					dcmm.GetSafeHdc(),
					(LPBITMAPINFO)&bih,
					DIB_RGB_COLORS,
					(void **)&m_clrFadeBitsTmp,
					NULL,
					NULL
					);
			if( hDIB == NULL || m_clrFadeBitsTmp == NULL )
			{
				m_bmpScreenSrc.DeleteObject();
				m_bmpScreenDst.DeleteObject();
				return;
			}
			m_bmpScreenTmp.Attach( hDIB );

			pBmpOld =
				dcmm.SelectObject( &m_bmpScreenSrc );
			dcmm.BitBlt(
				0, 0, cx, cy, pPaintDC,
				rcClient.left,
				rcClient.top,
				SRCCOPY
				);

			::memcpy(
				m_clrFadeBitsDst,
				m_clrFadeBitsSrc,
				sizeof(COLORREF)*cx*cy
				);
			dcmm.SelectObject( &m_bmpScreenDst );
		}
		else
		{
			m_bmpScreenDst.CreateCompatibleBitmap(
				pPaintDC,
				cx,
				cy
				);
			pBmpOld =
				dcmm.SelectObject( &m_bmpScreenDst );
		}

		_DoPaint( dcmm, false );
		dcmm.SetViewportOrg( CPoint(0,0) );
		dcmm.SelectObject( pBmpOld );
	} // if( m_bmpScreenDst.GetSafeHandle() == NULL )
	
COLORREF * src = m_clrFadeBitsSrc;
COLORREF * dst = m_clrFadeBitsDst;
COLORREF * tmp = m_clrFadeBitsTmp;

CBitmap * pBmpOld = NULL;

	switch( m_AnimationType )
	{
	case __AT_BOXES:
	case __AT_CIRCLES:
	case __AT_HOLES:
		if( tmp!= NULL && src!=NULL && dst != NULL )
		{
			ASSERT(
				m_nAnimPercent >= 0 && m_nAnimPercent <= 100
				&&
				m_nAnimPercentOld >= 0 && m_nAnimPercentOld <= 100
				&&
				m_nAnimPercentOld <= m_nAnimPercent
				);
			ASSERT( __BOX_ANIM_METRIC > 0 ); // self assert
			if( m_nAnimPercentOld != m_nAnimPercent )
			{
				pBmpOld = dcmm.SelectObject( &m_bmpScreenTmp );
				if( m_AnimationType == __AT_BOXES )
				{ // do boxes animation
					for( int y=0; y<cy; y++ )
					{
						int _y0 = y % __BOX_ANIM_METRIC;
						int _yPercent = 100 -
							(_y0 * 100) / __BOX_ANIM_METRIC;
						if( _yPercent < m_nAnimPercent )
						{
							::memcpy(
								tmp,
								src,
								cx*sizeof(COLORREF)
								);
							tmp += cx;
							src += cx;
							dst += cx;
							continue;
						}
						for( int x=0; x<cx; x++ )
						{
							int _x0 = x % __BOX_ANIM_METRIC;
							int _xPercent =
								(_x0 * 100) / __BOX_ANIM_METRIC;
							if( _xPercent < m_nAnimPercent )
								*tmp = *dst;
							else
								*tmp = *src;
							tmp++; src++; dst++;
						}
					} // for( int y=0; y<cy; y++ )
				} // do boxes animation
				else
				{ // do circles/holes animation
					double radius = 0;
					if( m_AnimationType == __AT_HOLES )
						radius = 
							((((double)(__BOX_ANIM_METRIC)) / 2) * (100-m_nAnimPercent))
							/ 100;
					else
						radius = 
							((((double)(__BOX_ANIM_METRIC)) / 2) * m_nAnimPercent)
							/ 100;
					if( radius > 0 )
					{
						for( int y=0; y<cy; y++ )
						{
							int _y0 = y % __BOX_ANIM_METRIC;
							int _y1 = y - _y0;
							//int _y2 = _y1 + __BOX_ANIM_METRIC;
							int _yCenter = _y1 + __BOX_ANIM_METRIC/2;
							int _yDist = abs( y - _yCenter );
							double _yDist2 = (double)_yDist*_yDist;
							for( int x=0; x<cx; x++ )
							{
								int _x0 = x % __BOX_ANIM_METRIC;
								int _x1 = x - _x0;
								//int _x2 = _x1 + __BOX_ANIM_METRIC;
								int _xCenter = _x1 + __BOX_ANIM_METRIC/2;
								int _xDist = abs( x - _xCenter );
								double _xDist2 = (double)_xDist*_xDist;

								double nDispance =
									::sqrt(_yDist2+_xDist2);
								if( m_AnimationType == __AT_HOLES )
								{ // holes
									if( nDispance >= radius )
										*tmp = *dst;
									else
										*tmp = *src;
								} // holes
								else
								{ // circles
									if( nDispance < radius )
										*tmp = *dst;
									else
										*tmp = *src;
								} // circles
								tmp++; src++; dst++;
							}
						} // for( int y=0; y<cy; y++ )
					} // if( radius > 0 )
				} // do circles/holes animation

				int nMenuShadowSize =
					_GetMenuShadowSize();
				if( m_bCombineWithEA )
				{ // paint combined area AS IS
					ASSERT( !m_rcExcludeArea.IsRectEmpty() );
					CRect rcExcludeArea( m_rcExcludeArea );
					ScreenToClient( &rcExcludeArea );
					// MODIFIED BY SUNZHENYU, modified next 1 line
					CBitmap * pBmpOldLocal = dcmm.SelectObject( &m_bmpScreenDst );
					pPaintDC->BitBlt(
						rcExcludeArea.left, rcExcludeArea.top,
						rcExcludeArea.Width(), rcExcludeArea.Height(), 
						&dcmm,
						rcExcludeArea.left, rcExcludeArea.top,
						SRCCOPY
						);
					// MODIFIED BY SUNZHENYU, modified next 2 line
					dcmm.SelectObject( pBmpOldLocal );
					// pBmpOld	=	NULL;
					if( m_eCombineAlign != __CMBA_NONE )
					{
//						int nMenuShadowSize =
//							_GetMenuShadowSize();
						rcExcludeArea.InflateRect(
							(m_eCombineAlign == __CMBA_RIGHT) ?
								0 : nMenuShadowSize,
							(m_eCombineAlign == __CMBA_BOTTOM) ?
								0 : nMenuShadowSize,
							(m_eCombineAlign == __CMBA_LEFT) ?
								0 : nMenuShadowSize,
							(m_eCombineAlign == __CMBA_TOP) ?
								0 : nMenuShadowSize
							);
					} // if( m_eCombineAlign != __CMBA_NONE )
					pPaintDC->ExcludeClipRect(
						&rcExcludeArea
						);
				} // paint combined area AS IS
				if( nMenuShadowSize > 0 )
				{
					CRect rcExcludeArea( rcClient );
					rcExcludeArea.left =
						rcExcludeArea.right
						- nMenuShadowSize;
					pPaintDC->ExcludeClipRect(
						&rcExcludeArea
						);
					rcExcludeArea = rcClient ;
					rcExcludeArea.top =
						rcExcludeArea.bottom
						- nMenuShadowSize;
					pPaintDC->ExcludeClipRect(
						&rcExcludeArea
						);
				}
				
				pPaintDC->BitBlt(
					rcClient.left, rcClient.top, cx, cy, 
					&dcmm,
					0,
					0,
					SRCCOPY
					);

				if( nMenuShadowSize > 0 || m_bCombineWithEA )
					pPaintDC->SelectClipRgn( NULL );
			} // if( m_nAnimPercentOld != m_nAnimPercent )
		}
	break; // cases __AT_BOXES,__AT_CIRCLES,__AT_HOLES
	case __AT_NOISE:
		if( tmp!= NULL && src!=NULL && dst != NULL )
		{
			ASSERT(
				m_nAnimPercent >= 0 && m_nAnimPercent <= 100
				&&
				m_nAnimPercentOld >= 0 && m_nAnimPercentOld <= 100
				&&
				m_nAnimPercentOld <= m_nAnimPercent
				);
			if( m_nAnimPercentOld != m_nAnimPercent )
			{
				int nDiff =
					m_nAnimPercent - m_nAnimPercentOld;
				ASSERT( nDiff > 0 );
				int nRandHalf = nDiff / 2 + 1;
				ASSERT( nRandHalf > 0 );
				pBmpOld = dcmm.SelectObject( &m_bmpScreenTmp );
				for( int pixel = 0; pixel < cx * cy; pixel++ )
				{
					int nRandValue = 
						rand() % nDiff;
					if( nRandValue > nRandHalf )
						*tmp = *dst;
//					else
//						*tmp = *src;
					tmp++; src++; dst++;
				} // for( int pixel = 0; pixel < cx * cy; pixel++ )

				int nMenuShadowSize =
					_GetMenuShadowSize();
				if( m_bCombineWithEA )
				{ // paint combined area AS IS
					ASSERT( !m_rcExcludeArea.IsRectEmpty() );
					CRect rcExcludeArea( m_rcExcludeArea );
					ScreenToClient( &rcExcludeArea );
					// MODIFIED BY SUNZHENYU, modified next 1 line
					CBitmap * pBmpOldLocal = dcmm.SelectObject( &m_bmpScreenDst );
					pPaintDC->BitBlt(
						rcExcludeArea.left, rcExcludeArea.top,
						rcExcludeArea.Width(), rcExcludeArea.Height(), 
						&dcmm,
						rcExcludeArea.left, rcExcludeArea.top,
						SRCCOPY
						);
					// MODIFIED BY SUNZHENYU, modified next 2 line
					dcmm.SelectObject( pBmpOldLocal );
					// pBmpOld = NULL;
					if( m_eCombineAlign != __CMBA_NONE )
					{
//						int nMenuShadowSize =
//							_GetMenuShadowSize();
						rcExcludeArea.InflateRect(
							(m_eCombineAlign == __CMBA_RIGHT) ?
								0 : nMenuShadowSize,
							(m_eCombineAlign == __CMBA_BOTTOM) ?
								0 : nMenuShadowSize,
							(m_eCombineAlign == __CMBA_LEFT) ?
								0 : nMenuShadowSize,
							(m_eCombineAlign == __CMBA_TOP) ?
								0 : nMenuShadowSize
							);
					} // if( m_eCombineAlign != __CMBA_NONE )
					pPaintDC->ExcludeClipRect(
						&rcExcludeArea
						);
				} // paint combined area AS IS
				if( nMenuShadowSize > 0 )
				{
					CRect rcExcludeArea( rcClient );
					rcExcludeArea.left =
						rcExcludeArea.right
						- nMenuShadowSize;
					pPaintDC->ExcludeClipRect(
						&rcExcludeArea
						);
					rcExcludeArea = rcClient ;
					rcExcludeArea.top =
						rcExcludeArea.bottom
						- nMenuShadowSize;
					pPaintDC->ExcludeClipRect(
						&rcExcludeArea
						);
				}
				
				pPaintDC->BitBlt(
					rcClient.left, rcClient.top, cx, cy, 
					&dcmm,
					0,
					0,
					SRCCOPY
					);

				if( nMenuShadowSize > 0 || m_bCombineWithEA )
					pPaintDC->SelectClipRgn( NULL );
			} // if( m_nAnimPercentOld != m_nAnimPercent )
		}
	break; // case __AT_NOISE
	case __AT_SLIDE:
	case __AT_ROLL:
	case __AT_ROLL_AND_SREETCH:
	case __AT_SLIDE_AND_SREETCH:
		if( tmp!= NULL && src!=NULL && dst != NULL )
		{
			ASSERT(
				m_nAnimPercent >= 0 && m_nAnimPercent <= 100
				&&
				m_nAnimPercentOld >= 0 && m_nAnimPercentOld <= 100
				&&
				m_nAnimPercentOld <= m_nAnimPercent
				);

			CRect _rcClient;
			_GetClientRect( &_rcClient );

			if( m_bCombineWithEA )
			{ // paint combined area AS IS
				ASSERT( !m_rcExcludeArea.IsRectEmpty() );
				CRect rcExcludeArea( m_rcExcludeArea );
				ScreenToClient( &rcExcludeArea );
				// MODIFIED BY SUNZHENYU, modified next 1 line
				CBitmap * pBmpOldLocal = dcmm.SelectObject( &m_bmpScreenDst );
				pPaintDC->BitBlt(
					rcExcludeArea.left, rcExcludeArea.top,
					rcExcludeArea.Width(), rcExcludeArea.Height(), 
					&dcmm,
					rcExcludeArea.left, rcExcludeArea.top,
					SRCCOPY
					);
				// MODIFIED BY SUNZHENYU, modified next 2 line
				dcmm.SelectObject( pBmpOldLocal );
				// pBmpOld = NULL;
				if( m_eCombineAlign != __CMBA_NONE )
				{
					int nMenuShadowSize =
						_GetMenuShadowSize();
					rcExcludeArea.InflateRect(
						(m_eCombineAlign == __CMBA_RIGHT) ?
							0 : nMenuShadowSize,
						(m_eCombineAlign == __CMBA_BOTTOM) ?
							0 : nMenuShadowSize,
						(m_eCombineAlign == __CMBA_LEFT) ?
							0 : nMenuShadowSize,
						(m_eCombineAlign == __CMBA_TOP) ?
							0 : nMenuShadowSize
						);
				} // if( m_eCombineAlign != __CMBA_NONE )
				pPaintDC->ExcludeClipRect(
					&rcExcludeArea
					);
			} // paint combined area AS IS

			pBmpOld = dcmm.SelectObject( &m_bmpScreenDst );
			int cx =
				_rcClient.Width();
			int cy =
				_rcClient.Height();
			if( m_AnimationType == __AT_ROLL
				||
				m_AnimationType == __AT_SLIDE
				)
			{ // non-stratchable variants
				int _cx = 0;
				if( m_AnimationType == __AT_ROLL )
					_cx = (cx * (100-m_nAnimPercent)) / 100;
				int _cy =
					(cy * (100-m_nAnimPercent)) / 100;
				pPaintDC->BitBlt(
					_rcClient.left-_cx, _rcClient.top-_cy, cx, cy, 
					&dcmm,
					_rcClient.left, _rcClient.top,
					SRCCOPY
					);
			} // non-stratchable variants
			else
			{ // stratchable variants
				int _cx = cx;
				if( m_AnimationType == __AT_ROLL_AND_SREETCH )
					_cx = (cx * m_nAnimPercent) / 100;
				int _cy =
					(cy * m_nAnimPercent) / 100;
				pPaintDC->StretchBlt(
					_rcClient.left, _rcClient.top, _cx, _cy, 
					&dcmm,
					_rcClient.left, _rcClient.top, cx, cy, 
					SRCCOPY
					);
			} // stratchable variants

			if( m_bCombineWithEA )
				pPaintDC->SelectClipRgn( NULL );
		}
	break; // cases __AT_ROLL, __AT_SLIDE, __AT_ROLL_AND_SREETCH, __AT_ROLL_AND_SREETCH

	case __AT_CONTENT_EXPAND:
		if( tmp!= NULL && src!=NULL && dst != NULL )
		{
			ASSERT(
				m_nAnimPercent >= 0 && m_nAnimPercent <= 100
				&&
				m_nAnimPercentOld >= 0 && m_nAnimPercentOld <= 100
				&&
				m_nAnimPercentOld <= m_nAnimPercent
				);

			CRect _rcClient;
			_GetClientRect( &_rcClient );

			if( m_bCombineWithEA )
			{ // paint combined area AS IS
				ASSERT( !m_rcExcludeArea.IsRectEmpty() );
				CRect rcExcludeArea( m_rcExcludeArea );
				ScreenToClient( &rcExcludeArea );
				// MODIFIED BY SUNZHENYU, modified next 1 line
				CBitmap * pBmpOldLocal = dcmm.SelectObject( &m_bmpScreenDst );
				pPaintDC->BitBlt(
					rcExcludeArea.left, rcExcludeArea.top,
					rcExcludeArea.Width(), rcExcludeArea.Height(), 
					&dcmm,
					rcExcludeArea.left, rcExcludeArea.top,
					SRCCOPY
					);
				// MODIFIED BY SUNZHENYU, modified next 2 line
				dcmm.SelectObject( pBmpOldLocal );
				// pBmpOld = NULL;
			} // paint combined area AS IS

			pBmpOld = dcmm.SelectObject( &m_bmpScreenDst );
			int cx =
				_rcClient.Width();
			int cy =
				_rcClient.Height();
			int _cx = cx;
			int _cy =
				(cy * m_nAnimPercent) / 100;

			visible_items_t v;
				_GetVisibleItems( dcmm /*dc*/, v );
			if( v.GetSize() == 0 )
			{ // if menu is empty
				pPaintDC->StretchBlt(
					_rcClient.left, _rcClient.top, _cx, _cy, 
					&dcmm,
					_rcClient.left, _rcClient.top, cx, cy,
					SRCCOPY
					);
			} // if menu is empty
			else
			{ // if menu is NOT empty
				int y_pos_src = _rcClient.top;
				int y_pos_dst = _rcClient.top;
				INT vis_iter = 0;
				int nItemIndex = 0;
				int nItemCount = v.GetSize();
				ASSERT( nItemCount > 0 );
				expand_effect_rects_container_t vRects;
				
				for( ; vis_iter < v.GetSize(); ++vis_iter, ++nItemIndex )
				{ // calc rects for visible items
					visible_item_def_t & vi = v[vis_iter];
#ifdef _DEBUG
					ASSERT(
						vi.m_nIndex >= 0
						&&
						vi.m_nIndex < m_items_all.GetSize()
						);
//					ASSERT( !vi.m_rcItem.IsRectEmpty() );
//					ASSERT( dc.RectVisible(&vi.m_rcItem) );
					menu_item_info_t & mi =
						m_items_all[vi.m_nIndex];
					ASSERT( mi.IsDisplayed() );
					ASSERT( mi.GetCmdID() == vi.m_nHelperCmdID );
#endif // _DEBUG

					CRect rcItemSrc( vi.m_rcItem );
					rcItemSrc.left = _rcClient.left;
					rcItemSrc.right = _rcClient.right;
					if( nItemIndex == 0 )
						rcItemSrc.top = _rcClient.top;
					else if( nItemIndex == (nItemCount-1) )
						rcItemSrc.bottom = _rcClient.bottom;

					int y_pos_src_next = 
						y_pos_src + rcItemSrc.Height();
					CRect rcItemDst( rcItemSrc );
					rcItemDst.OffsetRect(
						0,
						y_pos_dst - rcItemDst.top
						);
					if( vi.m_bRarelyUsed )
						rcItemDst.bottom =
							rcItemDst.top
							+
							(rcItemDst.Height() * m_nAnimPercent)
								/ 100;
					int y_pos_dst_next = 
						y_pos_dst + rcItemDst.Height();
					
					vRects.Add(
						expand_effect_rects_t(
							rcItemSrc,
							rcItemDst
							)
						);
					y_pos_dst = y_pos_dst_next;
					y_pos_src = y_pos_src_next;
				} // calc rects for visible items

				// y_pos_dst is now equal to size of
				// all compressed itsms
				ASSERT( y_pos_dst <= y_pos_src );
				if( m_bCombineWithEA
					&&
					m_eCombineAlign == __CMBA_BOTTOM
					&&
					y_pos_dst != y_pos_src
					)
				{ // expand from bottom to top
					INT iter_rects = 0;
					int nDstOffset =
						y_pos_src - y_pos_dst;
					for( ; iter_rects < vRects.GetSize(); ++iter_rects )
					{ // adjust destination rects
						expand_effect_rects_t & eerc =
							vRects[ iter_rects ];
						eerc.m_rcDst.OffsetRect(
							0,
							nDstOffset
							);
					} // adjust destination rects
				} // expand from bottom to top

				INT iter_rects = 0;
				for( ; iter_rects < vRects.GetSize(); ++iter_rects )
				{ // paint visible items
					expand_effect_rects_t & eerc =
						vRects[ iter_rects ];
					if( eerc.m_rcDst.IsRectEmpty() )
						continue;
					pPaintDC->StretchBlt(
						eerc.m_rcDst.left, eerc.m_rcDst.top,
						eerc.m_rcDst.Width(), eerc.m_rcDst.Height(), 
						&dcmm,
						eerc.m_rcSrc.left, eerc.m_rcSrc.top,
						eerc.m_rcSrc.Width(), eerc.m_rcSrc.Height(), 
						SRCCOPY
						);
				} // paint visible items
			} // if menu is NOT empty

		}
	break; // case __AT_CONTENT_EXPAND

	case __AT_CONTENT_DISPLAY:
		if( tmp!= NULL && src!=NULL && dst != NULL )
		{
			pBmpOld = dcmm.SelectObject( &m_bmpScreenDst );
			pPaintDC->BitBlt(
				rcClient.left, rcClient.top, cx, cy, 
				&dcmm,
				0,
				0,
				SRCCOPY
				);
		}
	break; // case __AT_CONTENT_DISPLAY

	case __AT_FADE:
		if( tmp!= NULL && src!=NULL && dst != NULL )
		{
			ASSERT(
				m_nAnimPercent >= 0 && m_nAnimPercent <= 100
				&&
				m_nAnimPercentOld >= 0 && m_nAnimPercentOld <= 100
				&&
				m_nAnimPercentOld <= m_nAnimPercent
				);
			pBmpOld = dcmm.SelectObject( &m_bmpScreenTmp );
			for( int pixel = 0; pixel < cx * cy; pixel++ )
			{
				COLORREF c0 = *dst++;
				COLORREF c1 = *src++;
				*tmp++ =
					RGB(
						(m_nAnimPercent*long(GetRValue(c0)) + (100L-m_nAnimPercent)*long(GetRValue(c1)) ) / 100L,
						(m_nAnimPercent*long(GetGValue(c0)) + (100L-m_nAnimPercent)*long(GetGValue(c1)) ) / 100L,
						(m_nAnimPercent*long(GetBValue(c0)) + (100L-m_nAnimPercent)*long(GetBValue(c1)) ) / 100L
						);
			} // for( int pixel = 0; pixel < cx * cy; pixel++ )
			pPaintDC->BitBlt(
				rcClient.left, rcClient.top, cx, cy, 
				&dcmm,
				0,
				0,
				SRCCOPY
				);
		}
	break; // case __AT_FADE

#ifdef _DEBUG
	default:
		ASSERT( FALSE );
	break; // default
#endif // _DEBUG
	} // switch( m_AnimationType )

	dcmm.SelectObject( pBmpOld );
}

void CExtPopupBaseWnd::_DoPaint( CDC & dcPaint, bool bUseBackBuffer /*= true*/ )
{
	dcPaint;
	bUseBackBuffer;
}

void CExtPopupBaseWnd::OnTimer(UINT nIDEvent) 
{
	switch( nIDEvent )
	{
	case ID_TIMER_DELAY_SHOW:
	{
		VERIFY( KillTimer(ID_TIMER_DELAY_SHOW) );
		ShowWindow( SW_SHOWNA );
		Invalidate( FALSE );
		UpdateWindow();
		return;
	}
	// case ID_TIMER_DELAY_SHOW

	case ID_TIMER_ANIMATION:
		if( !m_bAnimFinished )
		{
			clock_t nCurrAnimTime = clock();
			int nDuration =
				nCurrAnimTime - g_nLastAnimTime;
			int nSteps = (int)
				(.5 + (float) nDuration / ID_PERIOD_ANIMATION);
			switch( m_AnimationType )
			{
			case __AT_CONTENT_DISPLAY:
				m_nAnimPercent += g_nAnimStepMetric; // 2 times faster
//				m_bAnimFinished = true;
//			break;
			case __AT_FADE:
			case __AT_CONTENT_EXPAND:
			case __AT_ROLL:
			case __AT_SLIDE:
			case __AT_ROLL_AND_SREETCH:
			case __AT_SLIDE_AND_SREETCH:
			case __AT_NOISE:
			case __AT_BOXES:
			case __AT_CIRCLES:
			case __AT_HOLES:
				m_nAnimPercentOld = m_nAnimPercent;
				m_nAnimPercent += g_nAnimStepMetric;
				if( m_nAnimPercent >
						100 + nSteps * g_nAnimStepMetric
					)
					m_nAnimPercent = 101;
				if( m_nAnimPercent > 100 )
					m_bAnimFinished = true;
			break;
#ifdef _DEBUG
			default:
				ASSERT( FALSE );
			break;
#endif // _DEBUG
			} // switch( m_AnimationType )

			if( m_bAnimFinished )
			{
				KillTimer(ID_TIMER_ANIMATION);
				m_AnimationType = g_DefAnimationType;
			} // if( m_bAnimFinished )

			if( GetSafeHwnd() != NULL )
			{
				if( m_bAnimFinished )
					_EndAnimation();
				else
					Invalidate( FALSE );
				g_nLastAnimTime = nCurrAnimTime;
			}
		} // if( !m_bAnimFinished )
/*
		else
		{
			ASSERT( GetSafeHwnd() != NULL );
			KillTimer(ID_TIMER_ANIMATION);
			//ShowWindow( SW_SHOWNA );
			Invalidate( FALSE );
			//UpdateWindow();
		} // else from if( !m_bAnimFinished )
*/
		return;
	// case ID_TIMER_ANIMATION
	default:
		__BaseClassOfCExtPopupBaseWnd::OnTimer(nIDEvent);
	break; // default
	} // switch( nIDEvent )
}

int CExtPopupBaseWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return MA_NOACTIVATE;
	//return CExtPopupBaseWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CExtPopupBaseWnd::OnEraseBkgnd(CDC* pDC) 
{
	//return __BaseClassOfCExtPopupBaseWnd::OnEraseBkgnd(pDC);
	return TRUE;
}

void CExtPopupBaseWnd::OnNcPaint() 
{
	// TODO: Add your message handler code here
	// Do not call __BaseClassOfCExtPopupBaseWnd::OnNcPaint() for painting messages
}

void CExtPopupBaseWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
/*
CRect rectWindow;
	GetWindowRect(&rectWindow);
	ScreenToClient(&rectWindow);
	lpncsp->rgrc[0] = rectWindow;
*/
///	__BaseClassOfCExtPopupBaseWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CExtPopupBaseWnd::OnSize(UINT nType, int cx, int cy) 
{
	__BaseClassOfCExtPopupBaseWnd::OnSize(nType, cx, cy);
	Invalidate(FALSE);
}

BOOL CExtPopupBaseWnd::DestroyWindow()
{
	if( !(::IsWindow(GetSafeHwnd())) )
		return TRUE;
	return __BaseClassOfCExtPopupBaseWnd::DestroyWindow();
}

void CExtPopupBaseWnd::PostNcDestroy() 
{
///	__BaseClassOfCExtPopupBaseWnd::PostNcDestroy();
}

BOOL CExtPopupBaseWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return __BaseClassOfCExtPopupBaseWnd::OnSetCursor(pWnd, nHitTest, message);
/*
HCURSOR hCursor = (HCURSOR)
		::GetClassLong(GetSafeHwnd(),GCL_HCURSOR);
	ASSERT( hCursor != NULL );
HCURSOR hC2 = ::GetCursor();
	if(  hC2 != hCursor )
		::SetCursor( hCursor );
	return TRUE;
*/
}

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuTipWnd

IMPLEMENT_DYNCREATE(CExtPopupMenuTipWnd, CExtPopupBaseWnd)

CExtPopupMenuTipWnd::CExtPopupMenuTipWnd()
{
	m_hIcon = NULL;
	m_IconSize.cx = m_IconSize.cy = 0;
	m_sText = _T("");
	m_bFlipHorz = false;
	m_bFlipVert = false;

//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = ::LoadIcon(NULL,IDI_INFORMATION);
	m_IconSize.cx = m_IconSize.cy = 16;

	m_bAnimFinished = true;
	m_AnimationType = __AT_NONE;
}

CExtPopupMenuTipWnd::~CExtPopupMenuTipWnd()
{
}

BEGIN_MESSAGE_MAP(CExtPopupMenuTipWnd, CExtPopupBaseWnd)
	//{{AFX_MSG_MAP(CExtPopupMenuTipWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define CX_ROUNDED				12		// Tip horizontal roundness
#define CY_ROUNDED				10		// Tip vertical roundness
#define CX_LEADER				25		// Width of tip lead
#define CY_LEADER				25		// Height of tip lead
#define CX_ICON_MARGIN			5		// Width of margin between icon and tip text

#define __TIP_OVER_METRIC 12

bool CExtPopupMenuTipWnd::GetWindowRegion(
	CDC* pDC,
	HRGN* hRegion,
	CSize *Size /* = NULL */
	)
{
CRect	rcWnd(0,0,0,0);
POINT	ptLeader[3];
CRgn	LeaderRegion;
CRgn	CaptionRegion;
CFont	*pSysFont;
	
	ASSERT( pDC != NULL );
	ASSERT( hRegion != NULL );

	// Calculate the are for the tip text
	pSysFont =
		(CFont *)pDC->SelectObject(
			&(g_PaintManager->m_FontNormal)
			);
	pDC->DrawText(m_sText, &rcWnd, DT_CALCRECT);
	//if( pSysFont != NULL )
		pDC->SelectObject(pSysFont);

	// Adjust for the rounded corners
	rcWnd.InflateRect(CX_ROUNDED, CY_ROUNDED);

	// Adjust for icon
	if (m_hIcon != NULL)
		rcWnd.right = rcWnd.right + m_IconSize.cx + CX_ICON_MARGIN;
	if (rcWnd.Height() < m_IconSize.cy)
		rcWnd.bottom = rcWnd.top + m_IconSize.cy;

	// Calculate the leader triangle coordinates

	ptLeader[0].x = rcWnd.Width() - CX_ROUNDED;
	ptLeader[0].y = rcWnd.Height() - CY_ROUNDED;

	ptLeader[1].x = ptLeader[0].x;
	ptLeader[1].y = ptLeader[0].y + CY_LEADER;

	ptLeader[2].x = ptLeader[0].x - CX_LEADER;
	ptLeader[2].y = rcWnd.Height() - CY_ROUNDED;

CRect rcRoundRectRgn(CPoint(0,0),rcWnd.Size());
CRect rcRectRgn( 0, 0, rcWnd.Width(), rcWnd.Height() + CY_LEADER );
	m_bFlipHorz = false;
	m_bFlipVert = false;

CRect rcDesktop = _CalcDesktopRect();

	if( m_ptTrack.x - rcWnd.Width() < rcDesktop.left )
	{
		m_bFlipHorz = true;
		ptLeader[0].x = CX_ROUNDED;
		ptLeader[0].y = rcWnd.Height() - CY_ROUNDED;
		ptLeader[1].x = ptLeader[0].x;
		ptLeader[1].y = ptLeader[0].y + CY_LEADER;
		ptLeader[2].x = ptLeader[0].x + CX_LEADER;
		ptLeader[2].y = ptLeader[0].y;
	}

	if( m_ptTrack.y - rcWnd.Height() - CY_ROUNDED*2 < rcDesktop.top )
	{
		m_bFlipVert = true;
		ptLeader[0].y -= rcWnd.Height() - CY_ROUNDED*2;
		ptLeader[1].y = ptLeader[0].y - CY_LEADER;
		ptLeader[2].y -= ptLeader[0].y;
	}

	// Create the caption region
	CaptionRegion.CreateRoundRectRgn(
		//0, 0, rcWnd.Width(), rcWnd.Height(),
		rcRoundRectRgn.left,rcRoundRectRgn.top,rcRoundRectRgn.right,rcRoundRectRgn.bottom,
		CX_ROUNDED, CY_ROUNDED
		);
	// Create the leader region
	LeaderRegion.CreatePolygonRgn(ptLeader, 3, ALTERNATE);
	// Create window region
	*hRegion =  ::CreateRectRgn(
		//0, 0, rcWnd.Width(), rcWnd.Height() + CY_LEADER
		rcRectRgn.left,rcRectRgn.top,rcRectRgn.right,rcRectRgn.bottom
		);
	// Combine the regions
	CombineRgn(*hRegion, CaptionRegion.operator HRGN(), LeaderRegion.operator HRGN(), RGN_OR);

	if( m_bFlipVert )
		OffsetRgn(*hRegion,0,CY_LEADER);

	// Set the window size
	if (Size != NULL)
	{
		Size->cx	= rcWnd.Width();
		Size->cy	= rcWnd.Height() + CY_LEADER;
	}

	return true;
}

void CExtPopupMenuTipWnd::_DoPaint( CDC & dcPaint, bool bUseBackBuffer /*= true*/ )
{
//CPaintDC dc( this ); // device context for painting
CDC & dc = dcPaint;

CRect	rc;
CBrush	WindowBrush;
CBrush	FrameBrush;
CBrush	InnerFrameBrush;
HRGN	hRegion;
CRgn	*pRegion;
CFont	*pSysFont;

	// Get the client rectangle
	GetClientRect(rc);

	// Create the brushes
	InnerFrameBrush.CreateSolidBrush(
		//::GetSysColor(COLOR_SCROLLBAR)
		g_PaintManager->GetColor(COLOR_3DFACE)
		);
	FrameBrush.CreateSolidBrush(
		//::GetSysColor(COLOR_WINDOWTEXT)
		g_PaintManager->GetColor(COLOR_3DDKSHADOW)
		);
	WindowBrush.CreateSolidBrush(
		//::GetSysColor(COLOR_WINDOW)
		g_PaintManager->GetColor(COLOR_INFOBK)
		);

	// Get the window region
//	VERIFY( GetWindowRegion(&dc, &hRegion) );
//	pRegion = CRgn::FromHandle(hRegion);
	hRegion = CreateRectRgnIndirect(&rc);
	GetWindowRgn(hRegion);
	pRegion = CRgn::FromHandle(hRegion);

	// Draw the frame
	dc.FillRgn(pRegion, &WindowBrush);
	dc.FrameRgn(pRegion, &InnerFrameBrush, 3, 3);
	dc.FrameRgn(pRegion, &FrameBrush, 1, 1);

	// Adjust the area for the icon
	rc.DeflateRect(CX_ROUNDED, CY_ROUNDED, 0, 0);
	if( m_hIcon != NULL )
		rc.left += m_IconSize.cx + CX_ICON_MARGIN;
	
	// Set the font
	pSysFont = (CFont *)dc.SelectObject(
		&(g_PaintManager->m_FontNormal)
		);
	// Draw the tip text	
	dc.SetBkMode( TRANSPARENT );

int nIconVertPos = CY_ROUNDED;
	if( m_bFlipVert )
	{
		rc.OffsetRect(0,CY_LEADER);
		nIconVertPos += CY_LEADER;
	}

COLORREF clrOltTextColor =
		dc.SetTextColor(
			g_PaintManager->GetColor(COLOR_INFOTEXT)
			);
	dc.DrawText( m_sText, &rc, DT_TOP | DT_LEFT );
	dc.SetTextColor( clrOltTextColor );

	// Draw the icon
	if( m_hIcon != NULL )
		DrawIconEx(
			dc.GetSafeHdc(),
			CX_ROUNDED,
			nIconVertPos,
			m_hIcon,
			m_IconSize.cx,
			m_IconSize.cy,
			0,
			NULL,
			DI_NORMAL
			);

	// Clean up GDI
	::DeleteObject(hRegion);
	//if( pSysFont != NULL )
		dc.SelectObject(pSysFont);
}

void CExtPopupMenuTipWnd::SetText(LPCTSTR lpszText)
{
	m_sText = (lpszText==NULL) ? _T("") : lpszText;
	m_sText.TrimLeft();
	m_sText.TrimRight();
}

void CExtPopupMenuTipWnd::SetIcon(HICON hIcon) 
{
ICONINFO	IconInfo;
	m_hIcon = hIcon; 
	if (hIcon == NULL)
	{
		m_IconSize = CSize(0, 0);
		return;
	}
	
	// Get the icon sizes	
	ZeroMemory( &IconInfo, sizeof(ICONINFO) );
	::GetIconInfo(m_hIcon, &IconInfo);

	m_IconSize.cx = (BYTE)(IconInfo.xHotspot * 2);
	m_IconSize.cy = (BYTE)(IconInfo.yHotspot * 2);
    
	::DeleteObject(IconInfo.hbmMask);
	::DeleteObject(IconInfo.hbmColor);

	if( ::IsWindow(m_hWnd) )
		RedrawWindow();
}

bool CExtPopupMenuTipWnd::Show(
	CWnd * pWndParent,
	const RECT & rcExcludeArea // = CExtPopupMenuTipWnd::rcNoExcludeArea
	)
{
	ASSERT_VALID( pWndParent );
	ASSERT( pWndParent->GetSafeHwnd() != NULL );

HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW);
	ASSERT( hCursor != NULL );

	if( GetSafeHwnd() == NULL )
	{
		CString strClassName =
			::AfxRegisterWndClass (
				__POPUP_WNDCLASS_STYLES__,
				hCursor,
				(HBRUSH)(COLOR_BTNFACE + 1),
				NULL
				);
		BOOL bCreateResult =
			CWnd::CreateEx(
				WS_EX_TOPMOST, // 0,
				(LPCTSTR)strClassName,
				NULL,
				WS_POPUP,
				0, 0, 0, 0,
				pWndParent->GetSafeHwnd(),
				NULL,
				NULL
				);
		ASSERT( bCreateResult );
		if( !bCreateResult )
			return false;
	} // if( GetSafeHwnd() == NULL )
#ifdef _DEBUG
	else
	{
		ASSERT( pWndParent == GetParent() );
	} // else from if( GetSafeHwnd() == NULL )
#endif // _DEBUG

	ASSERT_VALID( this );

	m_rcExcludeArea = rcExcludeArea;
	m_rcExcludeArea.NormalizeRect();
CPoint ptTipTarget( m_rcExcludeArea.TopLeft() );
	ptTipTarget += CSize( __TIP_OVER_METRIC/2, __TIP_OVER_METRIC/2 );
	m_ptTrack = ptTipTarget;

CDC * pDC = GetDC();
	ASSERT_VALID( pDC );
HRGN	hRegion;
CSize WindowSize;
	VERIFY( GetWindowRegion(pDC, &hRegion, &WindowSize) );
	ReleaseDC(pDC);
	VERIFY(
		::SetWindowRgn(
			m_hWnd,
			hRegion,
			TRUE
			)
		);

	if( m_bFlipHorz )
		m_ptTrack.x +=
			m_rcExcludeArea.Width() - __TIP_OVER_METRIC
			+ WindowSize.cx - CX_ROUNDED*2
			;
	if( m_bFlipVert )
		m_ptTrack.y += 
			m_rcExcludeArea.Height() - __TIP_OVER_METRIC
			+ WindowSize.cy - CX_ROUNDED*2
			;

CPoint ptWndPos(
		m_ptTrack.x - WindowSize.cx + CX_ROUNDED,
		m_ptTrack.y - WindowSize.cy + CY_ROUNDED
		);
	SetWindowPos(
		&wndTop,
		ptWndPos.x,
		ptWndPos.y,
		WindowSize.cx,
		WindowSize.cy,
		SWP_NOACTIVATE //| SWP_SHOWWINDOW
		);
CRect rcWnd(ptWndPos,WindowSize);
	MoveWindow(&rcWnd);
//	ASSERT( IsWindowVisible() );
	::SetCursor(hCursor);

//	Invalidate( FALSE );
//	UpdateWindow();
	SetTimer( ID_TIMER_DELAY_SHOW, ID_PERIOD_DELAY_SHOW, NULL );

	return true;
}

void CExtPopupMenuTipWnd::Hide()
{
	if( GetSafeHwnd() != NULL )
	{
		KillTimer( ID_TIMER_DELAY_SHOW );
		ShowWindow( SW_HIDE );
//VERIFY( DestroyWindow() );
	}
	m_rcExcludeArea.SetRectEmpty();
}

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuWnd

// is allowed menu item positioning without
// using only & - marked text
bool CExtPopupMenuWnd::g_bAllowNonAccelPositioning = false;

IMPLEMENT_DYNCREATE(CExtPopupMenuWnd, CExtPopupBaseWnd)

CExtPopupMenuWnd::CExtPopupMenuWnd()
	: m_pWndParentMenu(NULL)
	//, m_pFocusHook(NULL)
{
	_Init();
}

CExtPopupMenuWnd::~CExtPopupMenuWnd()
{
	_OnCancelMode();
}

void CExtPopupMenuWnd::_Init()
{
	_InitAnimation();

	m_nDyScrollOffset = 0;
	m_bTopLevel
		= m_bScrollingAvailable
		= m_bExpandAvailable
		= m_bExpandWasPressed
		= m_bCombineWithEA 
		= false;
	m_nTrackFlags = 0;
	m_nWaitingExpandTickCount = 0;
	m_nCurIndex
		= m_nDelayedFocusItemIndex
		= IDX_NOTHING;

	m_nScrollingDirection = 0;
	m_rcScrollTop.SetRectEmpty();
	m_rcScrollBottom.SetRectEmpty();
	m_rcExpandBtn.SetRectEmpty();

	m_eCombineAlign = __CMBA_NONE;

	m_pCbPaintCombinedCookie = NULL;
	m_pCbPaintCombinedContent = NULL;
}

int CExtPopupMenuWnd::_GetCurIndex() const
{
	return m_nCurIndex;
}

int CExtPopupMenuWnd::_GetSpecBtnHeight()
{
int nHeight = ::GetSystemMetrics(SM_CYMENUCHECK);
	return nHeight;
}

void CExtPopupMenuWnd::_RecalcLayoutImpl()
{
CRect rcClient;
	_GetClientRect( &rcClient );
int nScrollBtnDy =
		_GetSpecBtnHeight();
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nMenuShadowSize = 0;
//		_GetMenuShadowSize();

int nScrollBottomOffsetDy = 0;
	if( m_bExpandAvailable )
	{
		m_rcExpandBtn.left =
			rcClient.left + nMenuBorderSize;
		m_rcExpandBtn.right =
			rcClient.right - (nMenuBorderSize + nMenuShadowSize);
		m_rcExpandBtn.bottom =
			rcClient.bottom - (nMenuBorderSize + nMenuShadowSize);
		m_rcExpandBtn.top =
			m_rcExpandBtn.bottom - nScrollBtnDy;
		nScrollBottomOffsetDy =
			- m_rcExpandBtn.Height();
		m_rcExpandBtn.DeflateRect(
			__MI_HORZ_GAP_TO_BORDER,
			0
			);
	} // if( m_bExpandAvailable )

	if( m_bScrollingAvailable )
	{
		m_rcScrollTop.left = m_rcScrollBottom.left =
			rcClient.left + nMenuBorderSize;
		m_rcScrollTop.right = m_rcScrollBottom.right =
			rcClient.right - (nMenuBorderSize + nMenuShadowSize);

		m_rcScrollTop.top =
			rcClient.top + nMenuBorderSize;
		m_rcScrollTop.bottom =
			m_rcScrollTop.top + nScrollBtnDy;
		
		m_rcScrollBottom.bottom =
			rcClient.bottom - (nMenuBorderSize + nMenuShadowSize);
		m_rcScrollBottom.top =
			m_rcScrollBottom.bottom - nScrollBtnDy;

		m_rcScrollBottom.OffsetRect(0,nScrollBottomOffsetDy);
		m_rcScrollTop.DeflateRect(
			__MI_HORZ_GAP_TO_BORDER,
			0
			);
		m_rcScrollBottom.DeflateRect(
			__MI_HORZ_GAP_TO_BORDER,
			0
			);
	} // if( m_bScrollingAvailable )

}

class CExtPopupMenuWnd::CExtPopupMenuCmdUI : public CCmdUI
{
public: // re-implementations only
	CExtPopupMenuWnd * m_pPopup;

	virtual void Enable(BOOL bOn)
	{
		m_bEnableChanged = TRUE;
		ASSERT( m_pPopup != NULL );
		ASSERT(
			::AfxIsValidAddress(
				m_pPopup,
				sizeof(CExtPopupMenuWnd)
				)
			);
		ASSERT_VALID( m_pPopup );
		ASSERT( m_nIndex >= 0 && INT(m_nIndex) < m_pPopup->ItemGetCount() );
		CExtPopupMenuWnd::menu_item_info_t & mi =
			m_pPopup->_GetItemRef(m_nIndex);
		if( mi.IsPopup() )
			return;
		ASSERT( m_pPopup != NULL );
		CExtCmdManager::cmd_t * p_cmd =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( mi.GetCmdReciever() ),
				mi.GetCmdID()
				);
		if( p_cmd == NULL )
			return;
		p_cmd->StateEnable( bOn ? true : false );
	};

	virtual void SetRadio(BOOL bOn)
	{
		ASSERT( m_pPopup != NULL );
		ASSERT(
			::AfxIsValidAddress(
				m_pPopup,
				sizeof(CExtPopupMenuWnd)
				)
			);
		ASSERT_VALID( m_pPopup );
		ASSERT( m_nIndex >= 0 && INT(m_nIndex) < m_pPopup->ItemGetCount() );
		CExtPopupMenuWnd::menu_item_info_t & mi =
			m_pPopup->_GetItemRef(m_nIndex);
		if( mi.IsPopup() )
			return;
		CExtCmdManager::cmd_t * p_cmd =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( mi.GetCmdReciever() ),
				mi.GetCmdID()
				);
		if( p_cmd == NULL )
			return;
		p_cmd->StateSetRadio( bOn ? true : false );
	};

	virtual void SetCheck(int nCheck)
	{
		ASSERT( m_pPopup != NULL );
		ASSERT(
			::AfxIsValidAddress(
				m_pPopup,
				sizeof(CExtPopupMenuWnd)
				)
			);
		ASSERT_VALID( m_pPopup );
		ASSERT( m_nIndex >= 0 && INT(m_nIndex) < m_pPopup->ItemGetCount() );
		CExtPopupMenuWnd::menu_item_info_t & mi =
			m_pPopup->_GetItemRef(m_nIndex);
		if( mi.IsPopup() )
			return;
		CExtCmdManager::cmd_t * p_cmd =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( mi.GetCmdReciever() ),
				mi.GetCmdID()
				);
		if( p_cmd == NULL )
			return;
		p_cmd->StateSetCheck( nCheck ? true : false );
	};

	virtual void SetText(LPCTSTR lpszText)
	{
		if( lpszText == NULL )
			return;
		ASSERT( m_pPopup != NULL );
		ASSERT(
			::AfxIsValidAddress(
				m_pPopup,
				sizeof(CExtPopupMenuWnd)
				)
			);
		ASSERT_VALID( m_pPopup );
		ASSERT( m_nIndex >= 0 && INT(m_nIndex) < m_pPopup->ItemGetCount() );
		CExtPopupMenuWnd::menu_item_info_t & mi =
			m_pPopup->_GetItemRef(m_nIndex);
		if( mi.IsPopup() )
			return;
		CExtCmdManager::cmd_t * p_cmd =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( mi.GetCmdReciever() ),
				mi.GetCmdID()
				);
		if( p_cmd == NULL )
			return;
		p_cmd->m_sMenuText = lpszText;
	};

}; // class CExtPopupMenuWnd::CExtPopupMenuCmdUI

void CExtPopupMenuWnd::_UpdateCmdUI()
{
INT iter = 0;

BOOL bDisableIfNoHndler = TRUE;
CExtPopupMenuCmdUI state;
	state.m_pPopup = this;
	state.m_pOther = NULL; // menu
	state.m_nIndexMax = (UINT)ItemGetCount();
	state.m_nIndex = 0;

	for( ; iter < m_items_all.GetSize(); iter++, state.m_nIndex++ )
	{
		menu_item_info_t & mi = m_items_all[ iter ];
		if( mi.IsSeparator() )
			continue;
		if( mi.IsPopup() )
		{
			mi.GetPopup()->_UpdateCmdUI();
			continue;
		}
		if(	CExtCmdManager::IsCommandNeedsSpecUpdate(
				mi.GetCmdID() )
			)
			continue;
		HWND hWndCmdReciever =
			mi.GetCmdReciever();
		ASSERT( hWndCmdReciever != NULL );
		ASSERT( ::IsWindow(hWndCmdReciever) );
		CWnd * pWndCmdReciever =
			CWnd::FromHandle( hWndCmdReciever );
		ASSERT_VALID( pWndCmdReciever );
		state.m_nID = mi.GetCmdID();
		state.DoUpdate(pWndCmdReciever, bDisableIfNoHndler);
	} // for( ; iter < m_items_all.GetSize(); iter++, state.m_nIndex++ )
}

bool CExtPopupMenuWnd::_BuildItems(
	HWND hWndCmdProfileInfo,
	CMenu * pBuildMenu,
	bool bTopLevel
	)
{
INT iter = 0;
	for( ; iter < m_items_all.GetSize(); iter++)
	{
		menu_item_info_t & mi = m_items_all[ iter ];
		if( mi.IsPopup() )
		{
			mi.GetPopup()->_OnCancelMode();
			VERIFY( mi.GetPopup()->_BuildItems(hWndCmdProfileInfo,NULL,false) );
			mi.DestroyPopup();
		}
	} // for( ; iter < m_items_all.GetSize(); iter++)
	m_items_all.RemoveAll();

	if( pBuildMenu == NULL )
		return true;
	ASSERT( pBuildMenu->GetSafeHmenu() != NULL );

	m_bTopLevel = bTopLevel;
	if( m_bTopLevel )
		m_pWndParentMenu = NULL;

	// build items loop
bool bPrevWasSeparator = false;
int nMruUpdateIndex = -1, nInsertedIndex = 0;
int	nMenuItemCount = pBuildMenu->GetMenuItemCount();
	ASSERT( nMenuItemCount > 0 );
	m_items_all.RemoveAll();
	ASSERT( hWndCmdProfileInfo != NULL );
	for( int nItemIndex=0; nItemIndex<nMenuItemCount; nItemIndex++ )
	{
		UINT nMenuItemID =
			pBuildMenu->GetMenuItemID( nItemIndex );
		if( ID_FILE_MRU_FIRST <= nMenuItemID
			&&
			nMenuItemID <= ID_FILE_MRU_LAST
			)
		{
			if( nMruUpdateIndex < 0 )
				nMruUpdateIndex = nInsertedIndex;
			continue;
		}
		menu_item_info_t mi;
		if( g_hWndCmdReciever == NULL )
			g_hWndCmdReciever = hWndCmdProfileInfo;
		VERIFY(
			mi.UpdateFromMenu(
				hWndCmdProfileInfo,
				pBuildMenu,
				nItemIndex
				)
			);
//		if( g_hWndCmdReciever == hWndCmdProfileInfo )
//			g_hWndCmdReciever = NULL;
		bool bIsSeparator = mi.IsSeparator();
		if( bPrevWasSeparator && bIsSeparator )
				continue;
		bPrevWasSeparator = bIsSeparator;
		
		if( mi.IsPopup() )
		{
			
			CMenu * pSubMenu =
					pBuildMenu->GetSubMenu(nItemIndex);
			ASSERT( pSubMenu != NULL );
			ASSERT( pSubMenu->GetSafeHmenu() != NULL );
			mi.GetPopup()->_BuildItems(hWndCmdProfileInfo,pSubMenu,false);
			mi.GetPopup()->m_pWndParentMenu = this;
		}
		_InsertItem( -1, mi );
		nInsertedIndex++;
	} // for( int nItemIndex=0; nItemIndex<nMenuItemCount; nItemIndex++ )
	ASSERT( m_items_all.GetSize() <= nMenuItemCount );

int nMruInsertCount = 0;
	if( nMruUpdateIndex >= 0 )
	{ // insert recently opened file items
		ASSERT( nMruUpdateIndex < nMenuItemCount );
		CRecentFileList * pRecentFileList =
			InternalFriendlyWinApp::_GetFriendlyApp()->
				_GetRecentFileList();
		if( pRecentFileList != NULL )
		{ // can be null !?
			int nRecentCount =
				pRecentFileList->GetSize();
			TCHAR sCurrDir[_MAX_PATH+1];
			::memset(sCurrDir,0,sizeof(sCurrDir));
			::GetCurrentDirectory(_MAX_PATH,sCurrDir);
			int nLenCurDir = _tcslen(sCurrDir);
			for( nItemIndex=0; nItemIndex<nRecentCount; nItemIndex++ )
			{
				CString sDisplayName( _T("") );
				if(	!pRecentFileList->GetDisplayName(
						sDisplayName,
						nItemIndex,
						sCurrDir,
						nLenCurDir,
						TRUE
						)
					)
					continue;
				ASSERT( !sDisplayName.IsEmpty() );
				UINT nCmdID = ID_FILE_MRU_FIRST + nItemIndex;
				ASSERT( nCmdID <= ID_FILE_MRU_LAST );
				CExtCmdManager::cmd_t * p_cmd =
					g_CmdManager->CmdGetPtr(
						g_CmdManager->ProfileNameFromWnd( hWndCmdProfileInfo ),
						nCmdID
						);
				if( p_cmd == NULL )
					p_cmd =
						g_CmdManager->CmdAllocPtr(
							g_CmdManager->ProfileNameFromWnd( hWndCmdProfileInfo ),
							nCmdID
							);
				ASSERT( p_cmd != NULL );
				if( p_cmd == NULL )
					return false;
				int nDisplayIndex = nItemIndex+1;
				if( nDisplayIndex < 10 )
					p_cmd->m_sMenuText.Format(
						_T("&%d %s"),
						nDisplayIndex,
						(LPCTSTR)sDisplayName
						);
				else
					p_cmd->m_sMenuText.Format(
						_T("%d&%d %s"),
						nDisplayIndex/10,
						nDisplayIndex%10,
						(LPCTSTR)sDisplayName
						);
				CExtLocalResourceHelper _LRH;
				CString sRecentFileFmt1,sRecentFileFmt2;
				if( !sRecentFileFmt1.LoadString(IDS_RECENT_FILE_FMT_1) )
				{
					ASSERT( FALSE );
					sRecentFileFmt1 = _T("Recent file %d");
				}
				if( !sRecentFileFmt2.LoadString(IDS_RECENT_FILE_FMT_2) )
				{
					ASSERT( FALSE );
					sRecentFileFmt2 = _T("Recent file %d (\"%s\")");
				}
				p_cmd->m_sToolbarText.Format(
					sRecentFileFmt1,
					nItemIndex + 1
					);
				p_cmd->m_sTipTool.Format(
					sRecentFileFmt2,
					nItemIndex + 1,
					(LPCTSTR)sDisplayName
					);
				p_cmd->m_sTipStatus = p_cmd->m_sTipTool;

				if( g_hWndCmdReciever == NULL )
					g_hWndCmdReciever = hWndCmdProfileInfo;
				BOOL bInsRetVal =
					ItemInsert(
						nCmdID,
						nMruUpdateIndex + nItemIndex
						);
	//			if( g_hWndCmdReciever == hWndCmdProfileInfo )
	//				g_hWndCmdReciever = NULL;
				
				if( !bInsRetVal )
				{
					ASSERT( FALSE );
					return false;
				}
				nMruInsertCount++;
			} // for( nItemIndex=0; nItemIndex<nRecentCount; nItemIndex++ )
		} // can be null !?
		if( nMruInsertCount > 0 )
		{
			if( !ItemInsert(
					ID_SEPARATOR,
					nMruUpdateIndex + nMruInsertCount
					)
				)
			{
				ASSERT( FALSE );
				return false;
			}
			nMruInsertCount++;
		} // if( nMruInsertCount > 0 )
	} // insert recently opened file items

	if( g_hWndCmdReciever == NULL )
		g_hWndCmdReciever = hWndCmdProfileInfo;
	_SyncItems();
//	if( g_hWndCmdReciever == hWndCmdProfileInfo )
//		g_hWndCmdReciever = NULL;

	return true;
}

void CExtPopupMenuWnd::_GetClientRect(RECT * pRectClient)
{
	ASSERT( pRectClient != NULL );
	ASSERT( GetSafeHwnd() != NULL );
/*
	GetClientRect( pRectClient );
int nMenuShadowSize =
		_GetMenuShadowSize();
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
	pRectClient->bottom -= nMenuShadowSize;
	pRectClient->right -= nMenuShadowSize;
	if( m_bCombineWithEA )
	{
		ASSERT( m_eCombineAlign != __CMBA_NONE );
		switch( m_eCombineAlign )
		{
		case __CMBA_TOP:
			pRectClient->top +=
				m_rcExcludeArea.Height();
		break; // case __CMBA_TOP
		case __CMBA_BOTTOM:
			pRectClient->bottom -=
				m_rcExcludeArea.Height()
				- nMenuShadowSize
				- nMenuBorderSize
				;
		break; // case __CMBA_BOTTOM
		case __CMBA_RIGHT:
			pRectClient->right -=
				m_rcExcludeArea.Width()
				- nMenuShadowSize
				- nMenuBorderSize
				;
		break; // case __CMBA_RIGHT
		case __CMBA_LEFT:
			pRectClient->left +=
				m_rcExcludeArea.Width();
		break; // case __CMBA_LEFT
#ifdef _DEBUG
		default:
			ASSERT( FALSE );
		break;
#endif // _DEBUG
		} // switch( m_eCombineAlign )
	} // if( m_bCombineWithEA )
*/

	*pRectClient = m_rcClient;

	ASSERT( pRectClient->top <= pRectClient->bottom );
	ASSERT( pRectClient->left <= pRectClient->right );
}

int CExtPopupMenuWnd::_HitTest(const CPoint & point )
{
	if( GetSafeHwnd() == NULL )
		return IDX_NOTHING;

CRect rcClient;
	_GetClientRect( &rcClient );
	if( !rcClient.PtInRect(point) )
		return IDX_NOTHING;

	if( m_bScrollingAvailable )
	{
		CRect rcItem;
		if( m_nDyScrollOffset != 0 )
		{
			_GetItemRect(IDX_SCROLL_TOP,rcItem);
			if( rcItem.PtInRect(point) )
				return IDX_SCROLL_TOP;
		}
		int nMaxScrollPos = _GetMaxScrollPos();
		if( m_nDyScrollOffset != nMaxScrollPos )
		{
			_GetItemRect(IDX_SCROLL_BOTTOM,rcItem);
			if( rcItem.PtInRect(point) )
				return IDX_SCROLL_BOTTOM;
		}
	}
	if( m_bExpandAvailable )
	{
		CRect rcExpand;
		_GetItemRect(IDX_EXPAND,rcExpand);
		if( rcExpand.PtInRect(point) )
			return IDX_EXPAND;
	}

int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();

int y = m_nDyScrollOffset + nMenuBorderSize;

INT iter = 0;
int nIndex = 0;
	for( ; iter < m_items_all.GetSize(); iter++,nIndex++ )
	{
		menu_item_info_t & mi = m_items_all[ iter ];
		if( !mi.IsDisplayed() )
			continue;

		CRect rcItem;
		_GetItemRect( nIndex, rcItem );
		if( rcItem.PtInRect(point) )
		{
			int nRetIndex = mi.GetIndex();
			ASSERT(
				nRetIndex >= 0
				&&
				nRetIndex < m_items_all.GetSize()
				);
			return nRetIndex;
		}
	} // for( ; iter < m_items_all.GetSize(); iter++,nIndex++ )
	return IDX_NOTHING;
};


BEGIN_MESSAGE_MAP(CExtPopupMenuWnd, CExtPopupBaseWnd)
	//{{AFX_MSG_MAP(CExtPopupMenuWnd)
	ON_WM_ACTIVATEAPP()
	ON_WM_CANCELMODE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	ON_WM_SYSDEADCHAR()
	ON_WM_CHAR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtPopupMenuWnd message handlers

BOOL CExtPopupMenuWnd::LoadMenu( 
	HWND hWndCmdProfileInfo,
	UINT nIDResource,
	bool bPopupMenu // = true
	)
{
CMenu menu;
	if( !menu.LoadMenu(nIDResource) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	return
		UpdateFromMenu(
			hWndCmdProfileInfo,
			&menu,
			bPopupMenu
			);
}

BOOL CExtPopupMenuWnd::UpdateMdiWindowsMenu(
	CWnd * pWndStartSearchMdiFrameWnd // = NULL
	)
{
CMDIFrameWnd * pFrame = NULL;
CWnd * pWnd =
		(pWndStartSearchMdiFrameWnd != NULL) ?
			pWndStartSearchMdiFrameWnd : ::AfxGetMainWnd();
	while( true )
	{
		ASSERT( pWnd != NULL );
		ASSERT_VALID( pWnd );
		pFrame =
			DYNAMIC_DOWNCAST(
				CMDIFrameWnd,
				pWnd
				);
		if( pFrame != NULL )
			break;
		pWnd = pWnd->GetParentFrame();
		if( pWnd == NULL )
			return FALSE;
	} // while( true )
	if( pFrame == NULL )
		return FALSE;
int nItemIndex = ItemGetCount();
	if( nItemIndex > 0 )
	{
		menu_item_info_t & mi = _GetItemRef(nItemIndex-1);
		if( !mi.IsSeparator() )
		{
			ItemInsert(ID_SEPARATOR);
			nItemIndex++;
			ASSERT( nItemIndex == ItemGetCount() );
		}
	}

HWND m_hWndMDIClient = pFrame->m_hWndMDIClient;
	ASSERT( m_hWndMDIClient != NULL );
	ASSERT( ::IsWindow(m_hWndMDIClient) );
HWND hWndActiveChildFrame =
		(HWND)::SendMessage(
			m_hWndMDIClient,
			WM_MDIGETACTIVE,
			0,
			NULL
			);
int nCmdID = __ID_MDIWNDLIST_FIRST;
	for( int iWin=1; true; iWin++,nCmdID++ )
	{
		HWND hWndChildFrame =
			::GetDlgItem(m_hWndMDIClient, nCmdID);
		if( hWndChildFrame == NULL )
			break;

		if( iWin <= __ID_MDIWNDLIST_COUNT )
		{ // if head of MDI windows list
			ASSERT( ::IsWindow(hWndChildFrame) );
			CString sWinName( _T("") );
			CWnd::FromHandle(hWndChildFrame)->
				GetWindowText(sWinName);
			CExtCmdManager::cmd_t * p_cmd =
				g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd( pWndStartSearchMdiFrameWnd->GetSafeHwnd() ),
					nCmdID
					);
			if( p_cmd == NULL )
				p_cmd =
				g_CmdManager->CmdAllocPtr(
					g_CmdManager->ProfileNameFromWnd( pWndStartSearchMdiFrameWnd->GetSafeHwnd() ),
					nCmdID
					);
			ASSERT( p_cmd != NULL );
			if( p_cmd == NULL )
				return FALSE;
			p_cmd->m_sMenuText.Format(
				_T("&%d %s"),
				iWin,
				(LPCTSTR)sWinName
				);
			p_cmd->m_sToolbarText = p_cmd->m_sMenuText;
			CExtLocalResourceHelper _LRH;
			CString sDocumentNameFmt;
			if( !sDocumentNameFmt.LoadString(IDS_DOCUMENT_NAME_FMT) )
			{
				ASSERT( FALSE );
				sDocumentNameFmt = _T("Document \"%s\"");
			}
			p_cmd->m_sTipTool.Format(
				sDocumentNameFmt,
				(LPCTSTR)sWinName
				);
			p_cmd->m_sTipStatus = p_cmd->m_sTipTool;
			p_cmd->StateSetRadio(
				(hWndChildFrame == hWndActiveChildFrame) ?
					true : false
				);
			if( !ItemInsert(nCmdID) )
			{
				ASSERT( FALSE );
				return FALSE;
			}
		} // if head of MDI windows list
		else
		{ // MDI windows list enough long, trim it
			if( !ItemInsert(TYPE_SEPARATOR) )
			{
				ASSERT( FALSE );
				return FALSE;
			}
			HWND hWndSearch = pWndStartSearchMdiFrameWnd->GetSafeHwnd();
			if( hWndSearch == NULL )
				hWndSearch = CExtPopupMenuWnd::GetCmdTargetWnd();
			ASSERT( hWndSearch != NULL );
			ASSERT( ::IsWindow(hWndSearch) );
			CExtCmdManager::cmd_t * p_cmd =
				g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd( hWndSearch ),
					__ID_MDIWND_DLGWINDOWS
					);
			if( p_cmd == NULL )
				p_cmd =
				g_CmdManager->CmdAllocPtr(
					g_CmdManager->ProfileNameFromWnd( hWndSearch ),
					__ID_MDIWND_DLGWINDOWS
					);
			ASSERT( p_cmd != NULL );
			if( p_cmd == NULL )
				return FALSE;
			CExtLocalResourceHelper _LRH;
			CString sMoreWindows,sManageWindows;
			if( !sMoreWindows.LoadString(IDS_MDI_MORE_WINDOWS) )
			{
				ASSERT( FALSE );
				sMoreWindows = _T("More Windows");
			}
			if( !sManageWindows.LoadString(IDS_MDI_MANAGE_OPENED_WINDOWS) )
			{
				ASSERT( FALSE );
				sManageWindows = _T("Manage opened document windows");
			}
			p_cmd->m_sMenuText = sMoreWindows;
			p_cmd->m_sToolbarText = p_cmd->m_sMenuText;
			p_cmd->m_sTipTool = sManageWindows;
			p_cmd->m_sTipStatus = p_cmd->m_sTipTool;
			if( !ItemInsert(nCmdID) )
			{
				ASSERT( FALSE );
				return FALSE;
			}
			break;
		} // MDI windows list enough long, trim it
	} // for( int iWin=1; true; iWin++,nCmdID++ )
	
	_SyncItems();
	return TRUE;
}

HMENU CExtPopupMenuWnd::ExportToMenu(
	BOOL bDeep // = TRUE
	) const
{
CMenu menu;
	if( !menu.CreatePopupMenu() )
	{
		ASSERT( FALSE );
		return NULL;
	}

OSVERSIONINFO ov;
    ov.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	VERIFY( ::GetVersionEx( &ov ) );
bool bStupidNT4XX =
		(
		( ov.dwPlatformId == VER_PLATFORM_WIN32_NT )
		&&
		( ov.dwMajorVersion < 5 )
		);

int nCount = ItemGetCount();
	for( int nItem = 0; nItem < nCount; nItem++ )
	{
		const menu_item_info_t & mi =
			_GetItemRef( nItem );

		if( mi.IsPopup() )
		{
			CString sMiText( mi.GetText() );
			CString sMiAccelText( mi.GetAccelText() );
			if( !sMiAccelText.IsEmpty() )
			{
				sMiText += _T("\t");
				sMiText += sMiAccelText;
			}
			const CExtPopupMenuWnd * pPopup = mi.GetPopup();
			ASSERT( pPopup != NULL );
			HMENU hPopupData;
			if( bDeep )
				hPopupData = pPopup->ExportToMenu();
			else
				hPopupData = ::CreatePopupMenu();
			if( ( ! (::IsMenu(hPopupData)) )
				||
				(!	menu.AppendMenu(
						MF_STRING|MF_POPUP,
						UINT(hPopupData),
						sMiText
						)
					)
				)
			{
				ASSERT( FALSE );
				//continue;
			}
			if( bStupidNT4XX )
			{
				VERIFY( ::DestroyMenu( hPopupData ) );
			}
			continue;
		} // if( mi.IsPopup() )

		if( mi.IsSeparator() )
		{
			VERIFY( menu.AppendMenu( MF_SEPARATOR ) );
			continue;
		} // if( mi.IsSeparator() )

		CString sMiText( mi.GetText() );
		CString sMiAccelText( mi.GetAccelText() );
		if( !sMiAccelText.IsEmpty() )
		{
			sMiText += _T("\t");
			sMiText += sMiAccelText;
		}
		VERIFY(
			menu.AppendMenu(
				MF_STRING,
				mi.GetCmdID(),
				sMiText
				)
			);

	} // for( int nItem = 0; nItem < nCount; nItem++ )

	return menu.Detach();
}

BOOL CExtPopupMenuWnd::UpdateFromMenu( 
	HWND hWndCmdProfileInfo,
	CMenu * pBuildMenu,
	bool bPopupMenu, // = true
	bool bTopLevel // = true
	)
{
///	g_CurrentPopup.DoneInstance();
//	while( IsMenuTracking() )
//		_PassMsgLoop();
	
	if( g_hWndCmdReciever == NULL )
		g_hWndCmdReciever = hWndCmdProfileInfo;
	ASSERT( g_hWndCmdReciever != NULL );

	if( pBuildMenu == NULL
		|| (!(::IsMenu(pBuildMenu->GetSafeHmenu())))
		)
	{
		ASSERT( FALSE );
		return false;
	}
	if( bPopupMenu )
	{
		pBuildMenu = pBuildMenu->GetSubMenu(0);
		if( pBuildMenu == NULL
			|| (!(::IsMenu(pBuildMenu->GetSafeHmenu())))
			)
		{
			ASSERT( FALSE );
			return false;
		}
	}

	if(	!g_CmdManager->UpdateFromMenu(
			g_CmdManager->ProfileNameFromWnd( hWndCmdProfileInfo ),
			pBuildMenu->GetSafeHmenu()
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}

	if( !_BuildItems( hWndCmdProfileInfo, pBuildMenu, bTopLevel ) )
		return FALSE;
	return TRUE;
}

CExtPopupMenuWnd * CExtPopupMenuWnd::GetTrackingMenu()
{
CExtPopupMenuWnd * pPopup = g_CurrentPopup.GetInstance();
	if( !(::IsWindow(pPopup->GetSafeHwnd())) )
		return NULL;
	return pPopup;
}

void CExtPopupMenuWnd::CancelMenuTracking()
{
CExtPopupMenuWnd * pPopup = GetTrackingMenu();
	if( pPopup != NULL )
	{
		ASSERT_VALID( pPopup );
		pPopup->PostMessage( WM_CANCELMODE );
	}
	g_CurrentPopup.DoneInstance();
}

BOOL CExtPopupMenuWnd::IsMenuTracking()
{
	if( g_CurrentPopup.IsEmpty()
		|| g_pWndCapture == NULL
		)
		return FALSE;
	return TRUE;
}

BOOL CExtPopupMenuWnd::TrackPopupMenu(
	UINT nFlags,
	int x,
	int y,
	HWND hWndCmdReciever, // = AfxGetMainWnd()->GetSafeHwnd()
	LPCRECT lpRect, // = 0
	bool bSelectAny, // = false
	bool bPerformMessageLoop, // = false
	bool bForceExpandRarelyUsed, // = false
	CExtPopupMenuWnd::e_combine_exclude_area_t eCombineExcludeAreaMode, // = __COMBINE_DEFAULT
	LPVOID pCbPaintCombinedCookie, // = NULL
	pCbPaintCombinedContent pCbPaintCombinedContent // = NULL
	)
{
	g_CurrentPopup.DoneInstance();

	if( hWndCmdReciever == NULL )
		g_hWndCmdReciever = ::AfxGetMainWnd()->GetSafeHwnd();
	else
		g_hWndCmdReciever = hWndCmdReciever;
	ASSERT( g_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(g_hWndCmdReciever) );

	ASSERT( m_bTopLevel );

	// MODIFIED BY SUNZHENYU, remove the next 1 line, add 7 lines, for leaks of fast moving mouse from menu to menu
	// ASSERT( g_CurrentPopup.IsEmpty() );
	if( !g_CurrentPopup.IsEmpty() )
	{
		CExtPopupMenuWnd::CExtPopupMenuWndAutoPtr menuwndAutoPtr;
		menuwndAutoPtr	=	this;
		menuwndAutoPtr.DoneInstance();
		return FALSE;
	}
	g_CurrentPopup = this;

	_UpdateCmdUI();

	if(
		! _TrackPopupMenu(
			nFlags,
			x,
			y,
			lpRect,
			bSelectAny,
			bForceExpandRarelyUsed,
			eCombineExcludeAreaMode,
			pCbPaintCombinedCookie,
			pCbPaintCombinedContent
			)
		)
	{
		ASSERT( FALSE );
		g_CurrentPopup.DoneInstance();
		return FALSE;
	}
	if( !bPerformMessageLoop )
		return TRUE;
	// do popup menu message loop
	while( IsMenuTracking() )
	{
		WaitMessage();
		_PassMsgLoop();
	} // while( IsMenuTracking() )
	g_CurrentPopup.DoneInstance();
	return TRUE;
}

void CExtPopupMenuWnd::_PassMsgLoop()
{
#ifdef _USRDLL
	// If this is a DLL, need to set up MFC state
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
MSG msg;
	// Process all the messages in the message queue
	while( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
	{
/*
		if( msg.message == WM_COMMAND )
		{
//				g_CurrentPopup.DoneInstance();
			return TRUE;
		}
*/
		if( !AfxGetApp()->PumpMessage() )
		{
//				g_CurrentPopup.DoneInstance();
			PostQuitMessage(0);
			return; ////return TRUE;	// Signal WM_QUIT received
		} // if( !AfxGetApp()->PumpMessage() )
	} // while( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
	LONG lIdle = 0;
	while( AfxGetApp()->OnIdle(lIdle++) );
}

CSize CExtPopupMenuWnd::_CalcTrackSize()
{
int nMenuShadowSize =
		_GetMenuShadowSize();
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nXtraSz =
		nMenuBorderSize*2 + nMenuShadowSize;
CSize _size(m_sizeFullItems);
	if( m_bExpandAvailable )
		_size.cy += _GetSpecBtnHeight(); // xpand btn
	_size += CSize(nXtraSz,nXtraSz);
	return _size;
}

CRect CExtPopupMenuWnd::_CalcTrackRect()
{
	m_eCombineAlign = __CMBA_NONE;

CRect rcDesktop = _CalcDesktopRect();

CSize _size = _CalcTrackSize();
int nMenuShadowSize =
		_GetMenuShadowSize();
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nGapShift = 0;
	if( !m_bCombineWithEA )
		nGapShift = nMenuBorderSize + nMenuShadowSize;

CRect wr(m_ptTrack,_size);
//	wr.NormalizeRect();
int nCombineOffset = 0;
	if(	m_bCombineWithEA )
	{
		if( (m_nTrackFlags&TPMX_ALIGN_MASK)
				== TPMX_TOPALIGN )
		{
			nCombineOffset = 1;
			wr.OffsetRect(0,-nCombineOffset);
		}
		if( (m_nTrackFlags&TPMX_ALIGN_MASK)
				== TPMX_LEFTALIGN )
		{
			nCombineOffset = 1;
			wr.OffsetRect(-nCombineOffset,0);
		}
		if( (m_nTrackFlags&TPMX_ALIGN_MASK)
				== TPMX_BOTTOMALIGN )
		{
			nCombineOffset =
				-nMenuShadowSize - 1;
			wr.OffsetRect(0,-nCombineOffset);
		}
		if( (m_nTrackFlags&TPMX_ALIGN_MASK)
				== TPMX_RIGHTALIGN )
		{
			nCombineOffset =
				-nMenuShadowSize - 1;
			wr.OffsetRect(-nCombineOffset,0);
		}
	} // if(	m_bCombineWithEA )

//CRect rcIntersection;
//	if(	rcIntersection.IntersectRect( &wr, &m_rcExcludeArea )
//		)
/*
	if( m_rcExcludeArea.PtInRect(m_ptTrack) )
	{
		CRect rcMargins(0,0,0,0);
		rcMargins.left = m_rcExcludeArea.left - rcDesktop.left;
		if( rcMargins.left < 0 )
			rcMargins.left = 0;
		rcMargins.right = rcDesktop.right - m_rcExcludeArea.right;
		if( rcMargins.right < 0 )
			rcMargins.right = 0;
		rcMargins.top = m_rcExcludeArea.top - rcDesktop.top;
		if( rcMargins.top < 0 )
			rcMargins.top = 0;
		rcMargins.bottom = rcDesktop.bottom - m_rcExcludeArea.bottom;
		if( rcMargins.bottom < 0 )
			rcMargins.bottom = 0;
		bool bRightShift = rcMargins.right > rcMargins.left;
		bool bBottomShift = rcMargins.bottom > rcMargins.top;
		CPoint ptOffset(
			m_rcExcludeArea.left - wr.left,
			m_rcExcludeArea.top - wr.top + m_rcExcludeArea.Height()
			);
		if( !bRightShift )
			ptOffset.x +=
				m_rcExcludeArea.Width() - wr.Width() + nMenuShadowSize;
		if( !bBottomShift )
			ptOffset.y +=
				-m_rcExcludeArea.Height() - wr.Height(); // + nMenuShadowSize;
		wr.OffsetRect(ptOffset);
	}
*/

	
	if(wr.bottom > rcDesktop.bottom)
	{
		if( m_bExcludeAreaSpec )
			wr.OffsetRect(
				0,
				-wr.Height()
					+ m_rcExcludeArea.Height()
					+ nGapShift
					+ (m_bCombineWithEA ?
						nMenuShadowSize : 0)
				);
		if(wr.bottom > rcDesktop.bottom)
			wr.OffsetRect(
				0,
				-(wr.bottom - rcDesktop.bottom)
				);
	}
	if(wr.top < rcDesktop.top)
		wr.OffsetRect(
			0,
			rcDesktop.top-wr.top
			);

	if(wr.right > rcDesktop.right)
	{
		if( (!m_bTopLevel) && m_bExcludeAreaSpec )
		{
			wr.OffsetRect(
				-wr.Width() - m_rcExcludeArea.Width(),
				0
				);
		}
		else
		{
			if( m_bExcludeAreaSpec )
				wr.OffsetRect(
					-wr.Width()
						+ m_rcExcludeArea.Width()
						+ nGapShift
						+ (m_bCombineWithEA ?
							nMenuShadowSize : 0)
						,
					0
					);
			if(wr.right > rcDesktop.right)
				wr.OffsetRect(
					-(wr.right - rcDesktop.right),
					0
					);
		}
	}
	if(wr.left < rcDesktop.left)
	{
		CRect rcDummy(wr);
		rcDummy.OffsetRect(
			wr.Width() + m_rcExcludeArea.Width(),
			0
			);
		if(rcDummy.right > rcDesktop.right)
			wr.OffsetRect(
				rcDesktop.left-wr.left,
				0
				);
		else
			wr = rcDummy;
	} // if(wr.left < rcDesktop.left)

	if( m_bExcludeAreaSpec || m_bCombineWithEA )
	{
		CRect rcIntersection;
		if(	rcIntersection.IntersectRect(
				&wr,
				&m_rcExcludeArea
				)
			|| m_bCombineWithEA
			)
		{ // if need adjust with exclude area
			CRect rcFree(0,0,0,0);
			if( m_rcExcludeArea.left > rcDesktop.left )
				rcFree.left =
					m_rcExcludeArea.left - rcDesktop.left;
			if( m_rcExcludeArea.right < rcDesktop.right )
				rcFree.right =
					rcDesktop.right - m_rcExcludeArea.right;
			if( m_rcExcludeArea.top > rcDesktop.top )
				rcFree.top =
					m_rcExcludeArea.top - rcDesktop.top;
			if( m_rcExcludeArea.bottom < rcDesktop.bottom )
				rcFree.bottom =
					rcDesktop.bottom - m_rcExcludeArea.bottom;
			if( (m_nTrackFlags&TPMX_ALIGN_MASK)
					== TPMX_LEFTALIGN
				&& rcFree.right > wr.Width()
				)
				wr.OffsetRect(
					wr.left - m_rcExcludeArea.right
						+ (m_bCombineWithEA ?
							nCombineOffset : 0)
						,
					0
					);
			if( (m_nTrackFlags&TPMX_ALIGN_MASK)
					== TPMX_RIGHTALIGN
				&& rcFree.left > wr.Width()
				)
				wr.OffsetRect(
					m_rcExcludeArea.left - wr.right
						- (m_bCombineWithEA ?
							nCombineOffset : 0)
						,
					0
					);

/*
			if( (m_nTrackFlags&TPMX_ALIGN_MASK)
					== TPMX_TOPALIGN
				&& rcFree.bottom > wr.Height()
				)
				wr.OffsetRect(
					0,
					wr.top - m_rcExcludeArea.bottom
						+ (m_bCombineWithEA ?
							nCombineOffset : 0)
					);
*/
			if( (m_nTrackFlags&TPMX_ALIGN_MASK)
					== TPMX_TOPALIGN
				)
			{
				if( rcFree.bottom > wr.Height() )
					wr.OffsetRect(
						0,
						wr.top - m_rcExcludeArea.bottom
							+ (m_bCombineWithEA ?
								nCombineOffset : 0)
						);
				else
				{
					if( ::IsWindow(GetSafeHwnd()) )
					{
						CRect rcWndOld;
						GetWindowRect( &rcWndOld );
						ShowWindow( SW_HIDE );
						_FreeWinObjects();
						_PassMsgLoop();
						if( !rcWndOld.IsRectEmpty() )
						{
							CWnd * pWndTLP = GetTopLevelParent();
							if( pWndTLP != NULL )
							{
								ASSERT_VALID( pWndTLP );
								if( pWndTLP->IsWindowVisible() )
								{
									CRect rcTLP;
									pWndTLP->GetWindowRect( &rcTLP );
									if( !rcTLP.IsRectEmpty() )
									{
										CRect rcIntersection;
										if(	rcIntersection.IntersectRect(
												&rcTLP,
												&rcWndOld
												)
											)
											pWndTLP->UpdateWindow();
									} // if( !rcTLP.IsRectEmpty() )
								} // if( pWndTLP->IsWindowVisible() )
							} // if( pWndTLP != NULL )
						} // if( !rcWndOld.IsRectEmpty() )
					}
					m_nTrackFlags &= ~TPMX_ALIGN_MASK;
					m_nTrackFlags |= TPMX_BOTTOMALIGN;
					return _CalcTrackRect();
				}

			}


			if( (m_nTrackFlags&TPMX_ALIGN_MASK)
					== TPMX_BOTTOMALIGN
					&& rcFree.top > wr.Height()
				)
				wr.OffsetRect(
					0,
					m_rcExcludeArea.top - wr.bottom
						- (m_bCombineWithEA ?
							nCombineOffset : 0)
					);
		} // if need adjust with exclude area
	} // if( m_bExcludeAreaSpec || m_bCombineWithEA )

	if( wr.bottom > rcDesktop.bottom )
	{
		wr.bottom = rcDesktop.bottom;
		m_bScrollingAvailable = true;
	}

	if( m_bCombineWithEA )
	{
		ASSERT( m_bTopLevel );
		ASSERT( m_bExcludeAreaSpec );
		if( wr.bottom == m_rcExcludeArea.top - nCombineOffset )
			m_eCombineAlign = __CMBA_BOTTOM;
		if( wr.top == m_rcExcludeArea.bottom  - nCombineOffset )
			m_eCombineAlign = __CMBA_TOP;
		if( wr.left == m_rcExcludeArea.right - nCombineOffset )
			m_eCombineAlign = __CMBA_LEFT;
		if( wr.right == m_rcExcludeArea.left - nCombineOffset  )
			m_eCombineAlign = __CMBA_RIGHT;
		if( m_eCombineAlign == __CMBA_NONE )
		{
			m_bCombineWithEA = false;
			return _CalcTrackRect();
		} // if( m_eCombineAlign == __CMBA_NONE )
		// init window region
		if( m_rgnWnd.GetSafeHandle() != NULL )
		{
			VERIFY(
				m_rgnWnd.DeleteObject()
				);
		}
		CRect rcExcludeArea(m_rcExcludeArea);
		rcExcludeArea.InflateRect(
			0,
			0,
			nMenuShadowSize,
			nMenuShadowSize
			);
		CRect rcFullWnd(
			min(wr.left,rcExcludeArea.left),
			min(wr.top,rcExcludeArea.top),
			max(wr.right,rcExcludeArea.right),
			max(wr.bottom,rcExcludeArea.bottom)
			);
		CRgn rgnAreaWnd,rgnAreaExclude;
		if(	(!m_rgnWnd.CreateRectRgnIndirect(
				&rcFullWnd ))
			||
			(!rgnAreaExclude.CreateRectRgnIndirect(
				&rcExcludeArea ))
			||
			(!rgnAreaWnd.CreateRectRgnIndirect(
				&wr ))
			||
			m_rgnWnd.CombineRgn(
				&rgnAreaWnd,
				&rgnAreaExclude,
				RGN_OR
				) == ERROR
			||
			m_rgnWnd.OffsetRgn(
				-rcFullWnd.TopLeft()
				) == ERROR
			)
		{
			if( m_rgnWnd.GetSafeHandle() != NULL )
			{
				VERIFY(
					m_rgnWnd.DeleteObject()
					);
			}
			m_bCombineWithEA = false;
			return _CalcTrackRect();
		}
		ASSERT( m_rgnWnd.GetSafeHandle() != NULL );
		
/*
		VERIFY( m_rgnWnd.DeleteObject() );
		VERIFY(
			m_rgnWnd.CreateRectRgnIndirect(
				&rcFullWnd
				)
			);
*/

		m_rcClient = wr;
		m_rcClient.OffsetRect( -rcFullWnd.TopLeft() );
		m_rcClient.DeflateRect(
			0,
			0,
			nMenuShadowSize,
			nMenuShadowSize
			);
		ASSERT( m_rcClient.top <= m_rcClient.bottom );
		ASSERT( m_rcClient.left <= m_rcClient.right );

		return rcFullWnd;
	} // if( m_bCombineWithEA )

	m_rcClient = wr;
	m_rcClient.OffsetRect( -wr.TopLeft() );
	m_rcClient.DeflateRect(
		0,
		0,
		nMenuShadowSize,
		nMenuShadowSize
		);
	ASSERT( m_rcClient.top <= m_rcClient.bottom );
	ASSERT( m_rcClient.left <= m_rcClient.right );

	return wr;
}

CRect CExtPopupBaseWnd::_CalcDesktopRect()
{
CRect rcDesktop;
	if( g_bUseDesktopWorkArea )
	{
		VERIFY(
			::SystemParametersInfo(
				SPI_GETWORKAREA,
				0,
				PVOID(&rcDesktop),
				0
				)
			);
		return rcDesktop;
	} // if( g_bMenuUsingDesktopWorkArea )
	rcDesktop.SetRect(0,0,800,600);
	return rcDesktop;
}

BOOL CExtPopupMenuWnd::_TrackPopupMenu(
	UINT nFlags,
	int x,
	int y,
	LPCRECT lpRect,
	bool bSelectAny, // = false
	bool bForceExpandRarelyUsed, // = false
	CExtPopupMenuWnd::e_combine_exclude_area_t eCombineExcludeAreaMode, // = __COMBINE_DEFAULT
	LPVOID pCbPaintCombinedCookie, // = NULL
	pCbPaintCombinedContent pCbPaintCombinedContent // = NULL
	)
{
	if( !g_bMenuExpanding )
		bForceExpandRarelyUsed = true;

	ASSERT( ::IsWindow(g_hWndCmdReciever) );
	ASSERT( GetSafeHwnd() == NULL );

	if( g_CurrentPopup.IsTopPupup(this) )
	{
		ASSERT( m_bTopLevel );
		MsgPrepareMenuData_t _mpmd( this );
		_mpmd.SendMessage( g_hWndCmdReciever );
		if( _mpmd.m_bMenuCanceled )
			return FALSE;
		if( _mpmd.m_bMenuChanged )
		{
			_UpdateCmdUI();
		}
		ASSERT( m_bTopLevel );
		ASSERT( g_CurrentPopup.IsTopPupup(this) );
	}

	{ // BLOCK: update system commands
		INT iter = 0;
		for(; iter < m_items_all.GetSize(); ++iter )
		{
			menu_item_info_t & mi = m_items_all[ iter ];
			if( mi.IsSeparator() )
				continue;
			UINT nCmdID = mi.GetCmdID();
			if( !CExtCmdManager::IsSystemCommand(nCmdID))
				continue;
			WINDOWPLACEMENT wndpl;
			::memset(&wndpl,0,sizeof(WINDOWPLACEMENT));
			wndpl.length = sizeof(WINDOWPLACEMENT);
			VERIFY(
				::GetWindowPlacement(
					mi.GetCmdReciever(),
					&wndpl
					)
				);
			bool bSysCmdEnabled = false;
			switch( nCmdID )
			{
			case SC_CLOSE:
				bSysCmdEnabled = true;
			break;
			case SC_SIZE:
			case SC_MOVE:
				if( wndpl.showCmd != SW_SHOWMINIMIZED
					&&
					wndpl.showCmd != SW_SHOWMAXIMIZED
					)
					bSysCmdEnabled = true;
			break;
			case SC_MINIMIZE:
				if( wndpl.showCmd != SW_SHOWMINIMIZED )
					bSysCmdEnabled = true;
			break;
			case SC_MAXIMIZE:
				if( wndpl.showCmd != SW_SHOWMAXIMIZED )
					bSysCmdEnabled = true;
			break;
			case SC_RESTORE:
				if( wndpl.showCmd == SW_SHOWMAXIMIZED
					||
					wndpl.showCmd == SW_SHOWMINIMIZED
					//||
					//wndpl.showCmd == SW_SHOWNORMAL
					)
					bSysCmdEnabled = true;
			break;
			case SC_CONTEXTHELP:
				if( GetWindowLong(
						mi.GetCmdReciever(),
						GWL_EXSTYLE
						) & WS_EX_CONTEXTHELP
					)
					bSysCmdEnabled = true;
			break;
//			case SC_NEXTWINDOW:
//			case SC_PREVWINDOW:
//			case SC_VSCROLL:
//			case SC_HSCROLL:
//			case SC_MOUSEMENU:
//			case SC_KEYMENU:
//			case SC_ARRANGE:
//			case SC_TASKLIST:
//			case SC_SCREENSAVE:
//#if(WINVER >= 0x0400)
//			case SC_DEFAULT:
//			case SC_MONITORPOWER:
//			case SC_SEPARATOR:
//#endif /* WINVER >= 0x0400 */
			case SC_HOTKEY:
			default:
				continue;
			} // switch( nCmdID )
			CExtCmdManager::cmd_t * p_cmd =
				g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd( g_hWndCmdReciever ),
					nCmdID
					);
			ASSERT( p_cmd != NULL );
			p_cmd->StateEnable( bSysCmdEnabled );
		} // for(; iter < m_items_all.end(); ++iter )
	} // BLOCK: update system commands

CWnd * pWndCmdReciever =
		CWnd::FromHandle( g_hWndCmdReciever );
	ASSERT_VALID( pWndCmdReciever );
	pWndCmdReciever->ActivateTopParent();
	pWndCmdReciever->BringWindowToTop();
	// MODIFIED BY SUNZHENYU, for error of CEditView
	// pWndCmdReciever->SetFocus();
	_Hook( true );

	// adjust own data
bool bOldTopLevel = m_bTopLevel;
bool bOldExpandAvailable = m_bExpandAvailable;
	_Init();
	m_bTopLevel = bOldTopLevel;
	m_bExpandAvailable = bOldExpandAvailable;
	m_nTrackFlags = nFlags;
	m_pCbPaintCombinedCookie = pCbPaintCombinedCookie;
	m_pCbPaintCombinedContent = pCbPaintCombinedContent;
	if( !m_bTopLevel )
	{
		ASSERT( m_pWndParentMenu != NULL );
		if( m_pWndParentMenu->m_bExpandWasPressed )
		{
			if( m_bExpandAvailable )
			{
				m_bExpandAvailable = false;
				m_bExpandWasPressed = true;
				_SyncItems();
			}
			else
				m_bExpandWasPressed = true;
		}
	} // if( !m_bTopLevel )
	else
	{
		if( bForceExpandRarelyUsed )
		{
			if( m_bExpandAvailable )
			{
				m_bExpandAvailable = false;
				m_bExpandWasPressed = true;
				_SyncItems();
			}
			else
				m_bExpandWasPressed = true;
		} // if( bForceExpandRarelyUsed )
	} // else from if( !m_bTopLevel )

	// adjust screen position
	m_ptTrack.x = x;
	m_ptTrack.y = y;
	if( m_ptTrack.x < 0 || m_ptTrack.y < 0 )
	{
		VERIFY( ::GetCursorPos(&m_ptTrack) );
	}

	if( lpRect != NULL )
	{
		m_rcExcludeArea = *lpRect;
		m_bExcludeAreaSpec = true;
	}
	else
	{
		m_bExcludeAreaSpec = false;
		m_rcExcludeArea.left = m_ptTrack.x - __EXCLUDE_AREA_GAP_DX;
		m_rcExcludeArea.right = m_ptTrack.x + __EXCLUDE_AREA_GAP_DX;
		m_rcExcludeArea.top = m_ptTrack.y - __EXCLUDE_AREA_GAP_DY;
		m_rcExcludeArea.bottom = m_ptTrack.y + __EXCLUDE_AREA_GAP_DY;
	}

	// adjust combine with exclude area mode
	m_bCombineWithEA = false;
	if( m_bTopLevel && m_bExcludeAreaSpec )
	{
		switch( eCombineExcludeAreaMode )
		{
		case __COMBINE_ANY_SUITABLE:
			m_bCombineWithEA = true;
		break;
		case __COMBINE_DEFAULT:
			m_bCombineWithEA =
				g_PaintManager->
					IsMenuMustCombineExcludeArea();
		break;
		} // switch( eCombineExcludeAreaMode )
	} // if( m_bTopLevel && m_bExcludeAreaSpec )

CSize _size = _CalcTrackSize();
bool bPointAdjusted = true;
	if( m_bExcludeAreaSpec )
	{
		switch( (m_nTrackFlags & TPMX_ALIGN_MASK) )
		{
		case TPMX_LEFTALIGN:
			m_ptTrack.x = m_rcExcludeArea.right;
			m_ptTrack.y = m_rcExcludeArea.top;
		break;
		case TPMX_RIGHTALIGN:
			m_ptTrack.x = m_rcExcludeArea.left - _size.cx;
			m_ptTrack.y = m_rcExcludeArea.top;
		break;
		case TPMX_TOPALIGN:
			m_ptTrack.x = m_rcExcludeArea.left;
			m_ptTrack.y = m_rcExcludeArea.bottom;
		break;
		case TPMX_BOTTOMALIGN:
			m_ptTrack.x = m_rcExcludeArea.left;
			m_ptTrack.y = m_rcExcludeArea.top - _size.cy;
		break;
		default:
			bPointAdjusted = false;
		break;
		} // switch( (m_nTrackFlags & TPMX_ALIGN_MASK) )
	} // if( m_bExcludeAreaSpec )
	if( !bPointAdjusted )
	{
		if( (m_nTrackFlags & TPMX_ALIGN_MASK)
				== TPMX_RIGHTALIGN )
			m_ptTrack.x -= _size.cx;
		else
		{
			if( (m_nTrackFlags & TPMX_ALIGN_MASK)
					== TPMX_CENTERALIGN )
				m_ptTrack.x -= _size.cx/2;
		}
		if( (m_nTrackFlags & TPMX_ALIGN_MASK)
				== TPMX_BOTTOMALIGN )
			m_ptTrack.y -= _size.cy;
		else
		{
			if( (m_nTrackFlags & TPMX_ALIGN_MASK)
					== TPMX_VCENTERALIGN )
				m_ptTrack.y -= _size.cy/2;
		}
	} // if( !bPointAdjusted )

bool bCreateResult =
	_CreateHelper( pWndCmdReciever );

	if( bSelectAny )
	{
		int nItem = _GetNextItem(__NI_ANY);
		if( nItem >= 0 )
			_ItemFocusSet(
				nItem,
				FALSE,
				FALSE
				);
	}

	m_AnimationType = g_DefAnimationType;
	if( CExtToolControlBar::g_bMenuTracking )
		m_AnimationType = __AT_NONE;

	_StartAnimation();
	if( m_AnimationType == __AT_NONE )
	{
		m_AnimationType = __AT_CONTENT_DISPLAY;
		_StartAnimation();
		ASSERT( m_AnimationType == __AT_CONTENT_DISPLAY );
	}

	if( m_rgnWnd.GetSafeHandle() != NULL )
	{
		ASSERT( m_bExcludeAreaSpec );
		ASSERT( m_bCombineWithEA );
		ASSERT( m_eCombineAlign != __CMBA_NONE );
		CRgn rgnTmp;
		VERIFY( rgnTmp.CreateRectRgn(0,0,0,0) );
		rgnTmp.CopyRgn( &m_rgnWnd );
		ASSERT( rgnTmp.GetSafeHandle() != NULL );
		VERIFY(
			SetWindowRgn(
				(HRGN)rgnTmp.Detach(),
				FALSE
				)
			);
	} // if( m_rgnWnd.GetSafeHandle() != NULL )
	SetWindowPos(
		&wndTop,
		-1, -1, -1, -1,
		SWP_NOACTIVATE
			|SWP_NOMOVE|SWP_NOSIZE
			|SWP_SHOWWINDOW
			//|SWP_NOREDRAW //|SWP_NOSENDCHANGING
			|SWP_NOZORDER //|SWP_NOOWNERZORDER
			//|SWP_NOCOPYBITS
		);
//	_BitsSave();

	ASSERT( IsWindowVisible() );

	_SetCapture();

	return TRUE;
}

bool CExtPopupMenuWnd::_CreateHelper(
	CWnd * pWndCmdReciever
	)
{
CRect wr = _CalcTrackRect();

HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW);
	ASSERT( hCursor != NULL );
CString strMenuClassName =
		::AfxRegisterWndClass(
			__POPUP_WNDCLASS_STYLES__,
			hCursor,
			(HBRUSH)(COLOR_BTNFACE + 1),
			NULL
			);
	ASSERT( !strMenuClassName.IsEmpty() );

BOOL bCreateResult =
		__BaseClassOfCExtPopupBaseWnd::CreateEx(
			WS_EX_TOPMOST, // 0,
			strMenuClassName,
			NULL,
			WS_POPUP,
			wr,
			pWndCmdReciever,
			0 // IDC_STATIC
			);
	if( !bCreateResult )
	{
		ASSERT( FALSE );
		return false;
	}

	_RecalcLayoutImpl();

	::SetCursor( hCursor );

	return true;
}

void CExtPopupMenuWnd::_GetItemRect(
	int nIndex,
	RECT & rectItem
	)
{
CRect rcClient;
	_GetClientRect( &rcClient );
	if( m_bScrollingAvailable )
	{
		ASSERT( !m_rcScrollTop.IsRectEmpty() );
		ASSERT( !m_rcScrollBottom.IsRectEmpty() );
		if( nIndex == IDX_SCROLL_TOP )
		{
			rectItem = m_rcScrollTop;
			return;
		}
		if( nIndex == IDX_SCROLL_BOTTOM )
		{
			rectItem = m_rcScrollBottom;
			return;
		}
	}
	if( m_bExpandAvailable
		&& nIndex == IDX_EXPAND
		)
	{
		rectItem = m_rcExpandBtn;
		return;
	}

int nCountOfItems = m_items_all.GetSize();
	if( nCountOfItems == 0 )
	{
		((CRect *)(&rectItem))->SetRectEmpty();
		return;
	}
	ASSERT(
		nIndex >= 0
		&&
		nIndex < nCountOfItems
		);
menu_item_info_t & mi =
		m_items_all[nIndex];
	if( !mi.IsDisplayed() )
	{
		((CRect *)(&rectItem))->SetRectEmpty();
		return;
	}

int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int y = m_nDyScrollOffset + nMenuBorderSize;

CRect rcItem;
	rcItem.SetRectEmpty();
	rcItem.left =
		nMenuBorderSize
			+ 1
		;
	rcItem.right =
		m_sizeFullItems.cx
			+ nMenuBorderSize
			- 1
		;

INT iter = 0;
	for(	int i = 0;
			i < nIndex
				&& iter != m_items_all.GetSize();
			i++, iter++
			)
	{
		menu_item_info_t & mi = m_items_all[ iter ];
		if( !mi.IsDisplayed() )
			continue;
		const RECT & rcBase =
			mi.GetBaseRect();
		y +=  rcBase.bottom - rcBase.top;
	}
	ASSERT( iter != m_items_all.GetSize() );
	rcItem.OffsetRect(0,y);
	mi = m_items_all[ iter ];
	ASSERT( mi.IsDisplayed() );
const RECT & rcBase = mi.GetBaseRect();
	rcItem.bottom =
		rcItem.top + rcBase.bottom - rcBase.top;
	
	// FIX for combined exclude area
	rcItem.OffsetRect( rcClient.TopLeft() );
	
	rectItem = rcItem;
}

void CExtPopupMenuWnd::_GetVisibleItems(
	CDC & dc,
	visible_items_t & v
	)
{
	v.RemoveAll();
INT iter = 0;
	for(	int nIndex = 0;
			iter != m_items_all.GetSize();
			iter++, nIndex++
			)
	{ // find visible items
		menu_item_info_t & mi = m_items_all[ iter ];
		if( !mi.IsDisplayed() )
			continue;
		CRect rcItem;
		_GetItemRect( nIndex, rcItem );
		if( rcItem.IsRectEmpty() )
			continue;
		if( !dc.RectVisible( &rcItem ) )
			continue;
		visible_item_def_t vi;
		vi.m_nIndex = nIndex;
		vi.m_nHelperCmdID = mi.GetCmdID();
		vi.m_rcItem = rcItem;
		if( mi.IsSeparator() )
		{
			vi.m_bHelperIsSeparator = true;
			v.Add( vi );
			continue;
		}
		if( mi.IsPopup() )
		{
			vi.m_bHelperIsPopup = true;
			vi.m_bRarelyUsed =
				mi.IsAllItemsRarelyUsed();
			v.Add( vi );
			continue;
		}
		CExtCmdManager::cmd_t * p_cmd =
			mi.GetCmd();
		ASSERT( p_cmd != NULL );
		vi.m_bRarelyUsed =
			p_cmd->StateIsRarelyUsed();
		v.Add( vi );
	} // find visible items
INT vis_iter = 0;
	for( ; vis_iter < v.GetSize(); ++vis_iter )
	{ // setup prev/next rarely used flags
		visible_item_def_t & vi = v[ vis_iter ];
		INT iter_prev = vis_iter;
		INT iter_next = vis_iter;
		if( vis_iter != 0 )
		{
			--iter_prev;
			if( v[iter_prev].m_bRarelyUsed )
				vi.m_bRarelyUsedPrev = true;
		} // if( vis_iter != v.begin() )
		++iter_next;
		if( iter_next != v.GetSize() )
		{
			if( v[iter_next].m_bRarelyUsed )
				vi.m_bRarelyUsedNext = true;
		} // if( iter_next != v.end() )

		if( vi.m_bHelperIsSeparator )
		{
			if(	vi.m_bRarelyUsedPrev
				&& vi.m_bRarelyUsedNext
				)
			{
				vi.m_bRarelyUsed = true;
				ASSERT( vis_iter != 0 );
				v[iter_prev].m_bRarelyUsedNext = true;
				ASSERT( iter_next != 0 );
				v[iter_next].m_bRarelyUsedPrev = true;
			}
		}
	} // setup prev/next rarely used flags
}

void CExtPopupMenuWnd::_DoPaint(
	CDC & dcPaint,
	bool bUseBackBuffer // = true
	)
{
	ASSERT_VALID( (&dcPaint) );
	ASSERT( dcPaint.GetSafeHdc() != NULL );

CRect rcRealClient;
	GetClientRect( &rcRealClient );
CRect rcClient;
	_GetClientRect( &rcClient );

//bUseBackBuffer = false;
//bUseBackBuffer = true;
	//dcPaint.SetViewportOrg( CPoint(0,0) );
CExtMemoryDC mdc;
	if( bUseBackBuffer )
	{
		mdc.__InitMemoryDC(
			&dcPaint,
			&rcClient // &rcRealClient
			);
		//mdc.SetViewportOrg( CPoint(0,0) );
	}
CDC & dcDummyRef = mdc;
CDC & dc = bUseBackBuffer ? dcDummyRef : dcPaint;

CFont * pOldFont = (CFont *)
		dc.SelectObject(
			&g_PaintManager->m_FontNormal
			);

	dc.FillSolidRect(
		&rcClient,
		g_PaintManager->GetMenuFrameFillColor()
		);

	g_PaintManager->PaintMenuBorder(
		dc,
		&rcClient
		);
CRect rcExpand;
	if( m_bExpandAvailable )
	{
		_GetItemRect(
			IDX_EXPAND,
			rcExpand
			);
		CRect rcExcludePart( rcExpand );
		rcExcludePart.InflateRect( 0, 1 );
		rcExcludePart.left = rcRealClient.left;
		rcExcludePart.right = rcRealClient.right;
		dc.ExcludeClipRect( rcExcludePart );
	}
CRect rcScrollTop,rcScrollBottom;
int nMaxScrollPos = 0;
	if( m_bScrollingAvailable )
	{
		if( m_nDyScrollOffset != 0 )
		{
			_GetItemRect(
				IDX_SCROLL_TOP,
				rcScrollTop
				);
			CRect rcExcludePart( rcScrollTop );
			rcExcludePart.InflateRect( 0, 1 );
			rcExcludePart.left = rcRealClient.left;
			rcExcludePart.right = rcRealClient.right;
			dc.ExcludeClipRect( rcExcludePart );
		}
		nMaxScrollPos = _GetMaxScrollPos();
		if( m_nDyScrollOffset != nMaxScrollPos )
		{
			_GetItemRect(
				IDX_SCROLL_BOTTOM,
				rcScrollBottom
				);
			CRect rcExcludePart( rcScrollBottom );
			rcExcludePart.InflateRect( 0, 1 );
			rcExcludePart.left = rcRealClient.left;
			rcExcludePart.right = rcRealClient.right;
			dc.ExcludeClipRect( rcExcludePart );
		}
	}
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nMenuShadowSize =
		_GetMenuShadowSize();
/*
	if( rcClient.left >= rcRealClient.left )
	{
		CRect rcExcludePart(rcRealClient);
		rcExcludePart.right =
			rcClient.left + nMenuBorderSize;
		dc.ExcludeClipRect( rcExcludePart );
	}
	if( rcClient.right <= rcRealClient.right )
	{
		CRect rcExcludePart(rcRealClient);
		rcExcludePart.left =
			rcClient.right - nMenuBorderSize;
		dc.ExcludeClipRect( rcExcludePart );
	}
*/
	if( rcClient.top >= rcRealClient.top )
	{
		CRect rcExcludePart(rcRealClient);
		rcExcludePart.bottom =
			rcClient.top + nMenuBorderSize;
		dc.ExcludeClipRect( rcExcludePart );
	}
	if( rcClient.bottom <= rcRealClient.bottom )
	{
		CRect rcExcludePart(rcRealClient);
		rcExcludePart.top =
			rcClient.bottom - nMenuBorderSize;
		dc.ExcludeClipRect( rcExcludePart );
	}

visible_items_t v;
	_GetVisibleItems( dc, v );
INT vis_iter = 0;
	for( ; vis_iter < v.GetSize(); ++vis_iter )
	{ // paint visible items
		visible_item_def_t & vi = v[ vis_iter ];
		ASSERT(
			vi.m_nIndex >= 0
			&&
			vi.m_nIndex < m_items_all.GetSize()
			);
		ASSERT( !vi.m_rcItem.IsRectEmpty() );
		ASSERT( dc.RectVisible(&vi.m_rcItem) );
		menu_item_info_t & mi =
			m_items_all[vi.m_nIndex];
		ASSERT( mi.IsDisplayed() );
		ASSERT( mi.GetCmdID() == vi.m_nHelperCmdID );
		if( vi.m_bHelperIsSeparator )
		{
			ASSERT( mi.IsSeparator() );
			g_PaintManager->PaintMenuSeparator(
				dc,
				vi.m_rcItem,
				g_bMenuHighlightRarely && vi.m_bRarelyUsed
				);
			continue;
		}
		ASSERT( !mi.IsSeparator() );
		ASSERT( mi.IsPopup() == vi.m_bHelperIsPopup );
		g_PaintManager->PaintMenuItem(
			dc,
			vi.m_rcItem,
			mi.GetText(),
			mi.GetAccelText(),
			mi.GetIcon(),
			vi.m_bHelperIsPopup,
			mi.IsSelected(),
			mi.GetCheck(),
			mi.GetRadio(),
			mi.GetIndeterminate(),
			mi.IsEnabled(),
			g_bMenuHighlightRarely && vi.m_bRarelyUsed,
			g_bMenuHighlightRarely && vi.m_bRarelyUsedPrev,
			g_bMenuHighlightRarely && vi.m_bRarelyUsedNext,
			mi.GetCmdID()
			);
	} // paint visible items

CPoint point;
	VERIFY( ::GetCursorPos(&point) );
	ScreenToClient( &point );
	dc.SelectClipRgn( NULL );

	if( m_bExpandAvailable )
	{
		bool bHover =
			(rcExpand.PtInRect(point))? true : false;
		bool bPushed = bHover;
		g_PaintManager->PaintMenuExpandButton(
			dc,
			rcExpand,
			bHover,
			bPushed
			);
	} // if( m_bExpandAvailable )

	if( m_bScrollingAvailable )
	{
		if( m_nDyScrollOffset != 0 )
		{
			bool bHover =
				(rcScrollTop.PtInRect(point))? true : false;
			bool bPushed =
				(m_nScrollingDirection>0)? true : false;
			bPushed = bPushed || bHover;
			g_PaintManager->PaintPushButton(
				dc,
				true,
				rcScrollTop,
				_T("t"), // up arrow
				NULL,
				true,false/*bHover*/,bPushed,false,
				true,true,false,false,
				CExtPaintManager::__ALIGN_HORIZ_CENTER
					| CExtPaintManager::__ALIGN_VERT,
				&(g_PaintManager->m_FontMarlett)
				);
		}
		if( m_nDyScrollOffset != nMaxScrollPos )
		{
			bool bHover =
				(rcScrollBottom.PtInRect(point))? true : false;
			bool bPushed =
				(m_nScrollingDirection<0)? true : false;
			bPushed = bPushed || bHover;
			g_PaintManager->PaintPushButton(
				dc,
				true,
				rcScrollBottom,
				_T("u"), // down arrow
				NULL,
				true,false/*bHover*/,bPushed,false,
				true,true,false,false,
				CExtPaintManager::__ALIGN_HORIZ_CENTER
					| CExtPaintManager::__ALIGN_VERT,
				&(g_PaintManager->m_FontMarlett)
				);
		}
	} // if( m_bScrollingAvailable )

	if( m_bCombineWithEA )
	{
		ASSERT( m_eCombineAlign != __CMBA_NONE );
		dc.SelectClipRgn(NULL);
		CRect rcExcludeClient( m_rcExcludeArea );
		ScreenToClient( &rcExcludeClient );
		g_PaintManager->PaintMenuCombinedArea(
			dc,
			rcExcludeClient,
			rcClient,
			m_eCombineAlign
			);
		if( m_pCbPaintCombinedContent != NULL )
			m_pCbPaintCombinedContent(
				m_pCbPaintCombinedCookie,
				dc,
				*this,
				m_rcExcludeArea,
				m_eCombineAlign
				);
	} // if( m_bCombineWithEA )

	dc.SelectObject( pOldFont );
	if( bUseBackBuffer )
	{
		ASSERT( mdc.GetSafeHdc() != NULL );
		mdc.__Flush();
	}

	if( CExtPopupMenuWnd::g_bMenuWithShadows
		&& m_bAnimFinished
		&& nMenuShadowSize > 0
		)
	{ // if we need to paint shadow for client area (and combined exclude area)
		dcPaint.SelectClipRgn(NULL);
		CRect
			rcExcludeClient( 0,0,0,0 ),
			rcExcludeScreen( 0,0,0,0 ),
			rcBaseScreen( 0,0,0,0 );
		if( m_bCombineWithEA )
		{
			if( CExtPopupMenuWnd::g_bMenuWithShadows
				&& m_bAnimFinished
				&& nMenuShadowSize > 0
				)
			{ // if we need to paint shadow for combined exclude area
				rcExcludeScreen = m_rcExcludeArea;
				rcExcludeClient = rcExcludeScreen;
				ScreenToClient( &rcExcludeClient );
				dcPaint.ExcludeClipRect( &rcClient );
				rcBaseScreen = rcClient;
				ClientToScreen( &rcBaseScreen );
				VERIFY(
					m_ShadowCMBA.Paint(
						dcPaint,
						rcExcludeClient,
						rcExcludeScreen,
						rcBaseScreen,
						nMenuShadowSize
						)
					);
				dcPaint.SelectClipRgn(NULL);
			} // if we need to paint shadow for combined exclude area

			CRect rcExcludePart( m_rcExcludeArea );
			ScreenToClient( &rcExcludePart );
			dcPaint.ExcludeClipRect( &rcExcludePart );
		} // if( m_bCombineWithEA )
		VERIFY(
			m_ShadowMain.Paint(
				dcPaint,
				rcClient,
				rcBaseScreen,
				rcExcludeScreen,
				nMenuShadowSize
				)
			);
	} // if we need to paint shadow for client area (and combined exclude area)
}

void CExtPopupMenuWnd::OnActivateApp(BOOL bActive, DWORD hTask) 
{
//	CExtPopupBaseWnd::OnActivateApp(bActive, hTask);
	hTask;
//	if( !bActive )
//		_OnCancelMode();
	PostMessage( WM_CANCELMODE );
}

bool CExtPopupMenuWnd::_StartScrolling(int nButtonIndex)
{
	if( GetSafeHwnd() == NULL )
		return FALSE;

	if( (nButtonIndex == IDX_SCROLL_TOP
			|| nButtonIndex == IDX_SCROLL_BOTTOM)
//		&& m_nCurIndex != nButtonIndex
		)
	{
		ASSERT( m_bScrollingAvailable );

		_ItemFocusDelay();
		_ItemFocusCancel( FALSE );
		m_nCurIndex = nButtonIndex;
		Invalidate( FALSE );

		SetTimer(
			ID_TIMER_SCROLLING,
			ID_PERIOD_SCROLLING,
			NULL
			);

		_SetCapture();

		return true;
	}
	return false;
}

bool CExtPopupMenuWnd::_OnMouseClick(UINT nFlags, CPoint point)
{
	if( GetSafeHwnd() == NULL )
		return false;

	if( !m_bAnimFinished )
		return true; // false;

CRect rectWindow;
	GetWindowRect( &rectWindow );
	ScreenToClient( &rectWindow );
	if( !rectWindow.PtInRect(point) )
	{
		if( m_pWndParentMenu != NULL
			&& m_pWndParentMenu->GetSafeHwnd() != NULL
			)
		{
			ASSERT_VALID( m_pWndParentMenu );
			ClientToScreen( &point );
			m_pWndParentMenu->ScreenToClient( &point );

			m_pWndParentMenu->_SetCapture();
			CExtPopupMenuWnd * pWndParentMenu = m_pWndParentMenu;
			if(	pWndParentMenu->_OnMouseClick(
					nFlags,
					point
					)
				)
			{
				PostMessage( WM_CANCELMODE );
				return true;
			}
			_SetCapture();
			return false;
		}
		PostMessage( WM_CANCELMODE );
		return false;
	}

int nHitTest = _HitTest( point );
	if( _StartScrolling(nHitTest) )
		return true;

	if( nHitTest < 0 )
	{
		if( (nHitTest == IDX_SCROLL_TOP
				|| nHitTest == IDX_SCROLL_BOTTOM)
			//&& m_nCurIndex != nHitTest
			)
		{
			return true;
		}

		if( nHitTest == IDX_EXPAND )
		{
			if(	(!m_bExpandWasPressed)
				&& m_bExpandAvailable
				)
				_DoExpand();
			return true;
		}

		if( nFlags != WM_RBUTTONUP ) // TEMPORARY
			PostMessage( WM_CANCELMODE );
		return false;
	}
	ASSERT( nHitTest < m_items_all.GetSize() );

menu_item_info_t & mi = _GetItemRef(nHitTest);
	if( (!mi.IsEnabled()) || mi.IsSeparator() )
		return false;
	if( mi.IsPopup() )
	{
		if( mi.GetPopup()->GetSafeHwnd() != NULL )
			return false;
		_ItemFocusSet(
			nHitTest,
			TRUE,
			TRUE
			);
		return true;
	}

	if( mi.IsExecutableCmdID() )
		_EndSequence(
			mi.GetCmdID(),
			mi.GetCmdReciever()
			);
	else
		PostMessage( WM_CANCELMODE );
	return false;
}

void CExtPopupMenuWnd::_EndSequence(
	UINT nCmdID, // = 0
	HWND hWndCmdReciever // = NULL
	)
{
	ASSERT_VALID( this );
	if( hWndCmdReciever == NULL )
		hWndCmdReciever = g_hWndCmdReciever;
	ASSERT( ::IsWindow(hWndCmdReciever) );
	PostMessage( WM_CANCELMODE );
	for(	CExtPopupMenuWnd * pWndParentMenu = m_pWndParentMenu;
			pWndParentMenu != NULL;
		)
	{
		CExtPopupMenuWnd * pWndParentMenu2 = pWndParentMenu;
		pWndParentMenu = pWndParentMenu->m_pWndParentMenu;
		pWndParentMenu2->PostMessage( WM_CANCELMODE );
	}

	if( nCmdID > 0 )
	{
		CExtCmdManager::cmd_t * p_cmd =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( CExtPopupMenuWnd::GetCmdTargetWnd() ),
				nCmdID
				);
		ASSERT( p_cmd != NULL );
		if( p_cmd != NULL )
		{
			VERIFY(
				p_cmd->Deliver( hWndCmdReciever )
				);
		} // if( p_cmd != NULL )
	}

	_Hook( false );

//	ASSERT( g_hWndCmdReciever != NULL );
//	ASSERT( ::IsWindow(g_hWndCmdReciever) );
//	::SendMessage(
//		g_hWndCmdReciever,
//		nMsgNotifyMenuClosed,
//		CExtCmdManager::IsCommand(nCmdID) ?
//			nCmdID : 0,
//		0
//		);
}

/*
void CExtPopupMenuWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
//	CExtPopupBaseWnd::OnMouseMove(nFlags, point);

	if( _GetCapture() == this )
		_OnMouseMove(nFlags, point);
}
void CExtPopupMenuWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
//CExtPopupBaseWnd::OnLButtonDown(nFlags, point);

	if( _GetCapture() == this )
		_OnMouseClick(nFlags,point);
}
void CExtPopupMenuWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//CExtPopupBaseWnd::OnLButtonUp(nFlags, point);
}
void CExtPopupMenuWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	//CExtPopupBaseWnd::OnLButtonDblClk(nFlags, point);
}
void CExtPopupMenuWnd::OnMButtonDown(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags, point);
}
void CExtPopupMenuWnd::OnMButtonUp(UINT nFlags, CPoint point) 
{
	OnLButtonUp(nFlags, point);
}
void CExtPopupMenuWnd::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDblClk(nFlags, point);
}
void CExtPopupMenuWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags, point);
}
void CExtPopupMenuWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	OnLButtonUp(nFlags, point);
}
void CExtPopupMenuWnd::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDblClk(nFlags, point);
}
  
void CExtPopupMenuWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	CExtPopupBaseWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	if( _GetCapture() == this )
		_OnKeyDown(nChar, nRepCnt, nFlags);
}
void CExtPopupMenuWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	CExtPopupBaseWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}
void CExtPopupMenuWnd::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	CExtPopupBaseWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}
void CExtPopupMenuWnd::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	CExtPopupBaseWnd::OnSysKeyUp(nChar, nRepCnt, nFlags);
}
void CExtPopupMenuWnd::OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	CExtPopupBaseWnd::OnSysDeadChar(nChar, nRepCnt, nFlags);
}
void CExtPopupMenuWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	CExtPopupBaseWnd::OnChar(nChar, nRepCnt, nFlags);
}
*/

void CExtPopupMenuWnd::OnCancelMode() 
{
//	CExtPopupBaseWnd::OnCancelMode();
	_OnCancelMode();
}

void CExtPopupMenuWnd::_FreeWinObjects()
{
	if( m_rgnWnd.GetSafeHandle() != NULL )
	{
		VERIFY(
			m_rgnWnd.DeleteObject()
			);
	}

	m_ShadowMain.Destroy();
	m_ShadowCMBA.Destroy();

	_SurfacesDelete();

	if( !(::IsWindow(GetSafeHwnd())) )
		return;
	KillTimer(ID_TIMER_ANIMATION);
	m_bAnimFinished = true;
	KillTimer(ID_TIMER_DELAY_SHOW);
	KillTimer(ID_TIMER_ITEM_FOCUS_DELAY	);
	KillTimer(ID_TIMER_SCROLLING);
}

void CExtPopupMenuWnd::_OnCancelMode() 
{
	_FreeWinObjects();
	_ReleaseCapture();

INT iter = 0;
	for( ; iter < m_items_all.GetSize(); iter++ )
	{
		menu_item_info_t & mi = m_items_all[ iter ];
		mi.SetSelected( false );

		if( mi.IsPopup() )
		{
			if( mi.GetPopup()->GetSafeHwnd() != NULL )
				mi.GetPopup()->_OnCancelMode();
//			ASSERT( mi.GetPopup()->GetSafeHwnd() == NULL );
		}
	}

	m_nCurIndex = IDX_NOTHING;

	if( g_CurrentPopup.IsTopPupup(this) )
	{
		if( ::IsWindow(g_CurrentPopup->GetSafeHwnd()) )
			g_CurrentPopup->SendMessage( WM_CLOSE );
		//g_CurrentPopup.SetInstance();

		g_hWndCmdReciever = NULL;
	}

	if( !(::IsWindow(GetSafeHwnd())) )
		return;
	ShowWindow( SW_HIDE );
	PostMessage( WM_CLOSE );
}

void CExtPopupMenuWnd::_ItemFocusSet(
	int nCurIndex,
	BOOL bEnableDropChild,
	BOOL bRepaint
	)
{
	if( m_nCurIndex != nCurIndex )
	{
		_ItemFocusCancel( FALSE );
		ASSERT( m_nCurIndex < 0 );
		m_nCurIndex = nCurIndex;
		if( m_nCurIndex < 0 )
			return;
	}

bool bWasSet = false;
menu_item_info_t & mi = _GetItemRef(m_nCurIndex);
	if( mi.IsEnabled() && mi.IsDisplayed() )
	{
		mi.SetSelected( true );
		bWasSet = true;

		// adjust scrolling
		int nMenuBorderSize =
			g_PaintManager->GetMenuBorderSize();
		if( m_bScrollingAvailable )
		{
			int nMetric;
			int nMaxScrollPos = _GetMaxScrollPos();
			CRect rcItem,rcClient,
				rcScrollBtnUp,rcScrollBtnDown;
			_GetItemRect( IDX_SCROLL_TOP, rcScrollBtnUp );
			_GetItemRect( IDX_SCROLL_BOTTOM, rcScrollBtnDown );
			_GetClientRect( rcClient );
			_GetItemRect( m_nCurIndex, rcItem );

			nMetric = rcClient.bottom;
			if( m_nDyScrollOffset != nMaxScrollPos )
			{
				nMetric -= rcScrollBtnDown.Height();
				if( m_bExpandAvailable )
				{
					CRect rcExpand;
					_GetItemRect( IDX_EXPAND, rcExpand );
					nMetric -= rcExpand.Height();
				}
			}
			if( rcItem.bottom >= nMetric )
			{
				m_nDyScrollOffset -=
					rcItem.bottom - nMetric
					+ nMenuBorderSize*2;
			}

			nMetric = rcClient.top;
			if( m_nDyScrollOffset != 0 )
			{
				nMetric += rcScrollBtnUp.Height();
			}
			if( rcItem.top <= nMetric )
			{
				m_nDyScrollOffset +=
					nMetric - rcItem.top
					+ nMenuBorderSize*2;
			}

			if( m_nDyScrollOffset > 0 )
				m_nDyScrollOffset = 0;
			else
			{
				if( m_nDyScrollOffset < nMaxScrollPos )
					m_nDyScrollOffset = nMaxScrollPos;
			}
		} // if( m_bScrollingAvailable )

		_SetCapture();
	}
	if( bWasSet )
	{
		// update status bar message
		CWnd * pWndOwner = GetOwner();
		if( pWndOwner != NULL )
		{
			ASSERT_VALID( pWndOwner );
			WPARAM iStatusUpdateValue = (WPARAM)AFX_IDS_IDLEMESSAGE;
			UINT nCmdID = mi.GetCmdID();
			if( CExtCmdManager::IsCommand(nCmdID) )
				iStatusUpdateValue = (WPARAM)nCmdID;
			pWndOwner->SendMessage(
				WM_SETMESSAGESTRING,
				CommandIDToStringID(iStatusUpdateValue)
				);
		} // if( pWndOwner != NULL )
	} // if( bWasSet )
	
	if( bRepaint && ::IsWindowVisible(GetSafeHwnd()) )
	{
//		if( m_rgnWnd.GetSafeHandle() != NULL )
//			RedrawWindow(
//				NULL,
//				&m_rgnWnd,
//				RDW_INVALIDATE|RDW_UPDATENOW|RDW_NOERASE
//					|RDW_INTERNALPAINT
//				);
//		else
//		{
			CClientDC dc( this );
			_DoPaint( dc );
//		}
	}
	
	if( bWasSet
		&& bEnableDropChild
		&& mi.IsPopup()
		&& mi.GetPopup()->GetSafeHwnd() == NULL
		)
	{
		_ReleaseCapture();
		CRect rectItem;
		_GetItemRect(
			m_nCurIndex,
			rectItem
			);
		CPoint point;
		point.x = rectItem.right;
		point.y = rectItem.top,
		ClientToScreen(&point);
		ClientToScreen( &rectItem );
		ASSERT( mi.GetPopup()->m_pWndParentMenu == this );
		mi.GetPopup()->_TrackPopupMenu(
			0,
			point.x,point.y,
			&rectItem
			);
		int nCountOfPopupItems =
			mi.GetPopup()->m_items_all.GetSize();
		if( nCountOfPopupItems > 0 )
		{
			int nIndex = mi.GetPopup()->_GetNextItem(__NI_ANY);
			if( nIndex >= 0 )
				mi.GetPopup()->_ItemFocusSet(
					nIndex,
					FALSE,
					mi.GetPopup()->m_bAnimFinished ?
						TRUE : FALSE
					);
		}
	}
}

bool CExtPopupMenuWnd::_CoolTipIsVisible()
{
	if( !(::IsWindow(GetSafeHwnd())) )
		return false;
	if( g_pWndCapture != this
		&& g_pWndCapture != NULL
		)
		return false;
HWND hWndToolTip =
		m_wndToolTip.GetSafeHwnd();
	if(	hWndToolTip != NULL
		&&
		::IsWindow( hWndToolTip )
		&&
		::IsWindowVisible( hWndToolTip )
		)
		return true;
	return false;
}

void CExtPopupMenuWnd::_CoolTipHide(
	bool bAdvOperation // = true
	)
{
	if( !(::IsWindow(GetSafeHwnd())) )
		return;

//	if( !_CoolTipIsVisible() )
//		return;
HWND hWndToolTip =
		m_wndToolTip.GetSafeHwnd();
	if(	hWndToolTip == NULL
		||
		(!(::IsWindow(hWndToolTip)))
//		||
//		(!(::IsWindowVisible(hWndToolTip))
		)
		return;
	
	if( bAdvOperation && m_bAnimFinished )
	{
		m_AnimationType = __AT_CONTENT_DISPLAY;
		_StartAnimation();
	}
	m_wndToolTip.Hide();
}

void CExtPopupMenuWnd::_ItemFocusCancel( BOOL bRepaint )
{
	_CoolTipHide();

	if( m_nCurIndex < 0 )
	{
		m_nCurIndex = IDX_NOTHING;
	} // if( m_nCurIndex < 0 )
	else
	{
		ASSERT( m_nCurIndex < m_items_all.GetSize() );
		menu_item_info_t & mi = _GetItemRef(m_nCurIndex);
		mi.SetSelected( false );
		m_nCurIndex = IDX_NOTHING;

		if( mi.IsPopup() )
			mi.GetPopup()->_OnCancelMode();
	} // else from if( m_nCurIndex < 0 )
	
	if( bRepaint && ::IsWindowVisible(GetSafeHwnd()) )
	{
		CClientDC dc( this );
		_DoPaint( dc );
	}
}

bool CExtPopupMenuWnd::_OnMouseMove(UINT nFlags, CPoint point)
{
	if( GetSafeHwnd() == NULL )
		return false;

	if( !m_bAnimFinished ) // ?!?! possible this not needed
		return false;

//	ASSERT( GetCapture() == this );

CRect rectWindow;
	GetWindowRect( &rectWindow );
	ScreenToClient( &rectWindow );
	if( !rectWindow.PtInRect(point) )
	{
//		_ItemFocusCancel( TRUE );
		if( m_pWndParentMenu != NULL
			&& m_pWndParentMenu->GetSafeHwnd() != NULL
			)
		{
			ASSERT_VALID( m_pWndParentMenu );
			ClientToScreen( &point );
			m_pWndParentMenu->ScreenToClient( &point );
			if( m_pWndParentMenu->_OnMouseMove(nFlags, point) )
			{
				_OnCancelMode();
				return true;
			}
			_SetCapture();
		}
		return false;
	}

int nCurIndex = _HitTest(point);
	if( nCurIndex == IDX_EXPAND )
	{
		_ItemFocusCancel( TRUE );
		_SetCapture();
		if( m_nWaitingExpandTickCount == 0 )
		{
			SetTimer(
				ID_TIMER_DELAY_EXPAND,
				ID_TOTAL_DELAY_EXPAND/ID_FREQ_DELAY_EXPAND,
				NULL
				);
			m_nWaitingExpandTickCount = 1;
		}
		return true;
	}
	if( _StartScrolling(nCurIndex) )
		return true;

//	if( m_nCurIndex == nCurIndex )
//		return false;
	if( nCurIndex >= 0 )
	{
#ifdef _DEBUG
		int nCountOfItems = m_items_all.GetSize();
		ASSERT( nCurIndex < nCountOfItems );
#endif // _DEBUG
		menu_item_info_t &mi = _GetItemRef(nCurIndex);
		if( (!mi.IsEnabled()) || mi.IsSeparator() )
			return false;
		_SetCapture();
		int nOldCurIndex = m_nCurIndex; 
		if( nOldCurIndex != nCurIndex )
			_ItemFocusSet(
				nCurIndex,
				FALSE,
				TRUE
				);

		if( _GetItemRef(nCurIndex).IsPopup() )
			_ItemFocusDelay( nCurIndex );
		else
		{
			if(	g_bMenuShowCoolTips
				&&	(  nOldCurIndex != nCurIndex
					|| (nCurIndex == 0
							&& nOldCurIndex == 0
							&& m_wndToolTip.GetSafeHwnd()==NULL)
					)
				)
			{
				CString sTipText = mi.GetTip();
				if( !sTipText.IsEmpty()
					&& g_pWndCapture != NULL
					&& g_pWndCapture == this
//					&& m_bAnimFinished
					)
				{
					CRect rcItem;
					_GetItemRect(nCurIndex,rcItem);
					ClientToScreen( &rcItem );
					m_wndToolTip.SetText( (LPCTSTR)sTipText );
					VERIFY(
						m_wndToolTip.Show(
							this,
							rcItem
							)
						);
				}
			} // if( nOldCurIndex != nCurIndex )
		}
		
		return (nOldCurIndex == nCurIndex) ? false : true;
	}

	return false;
}

void CExtPopupMenuWnd::_ItemFocusDelay(
	int nItemIndex // = IDX_NOTHING
	)
{
	if( !(::IsWindow(GetSafeHwnd())) )
		return;
	if( m_nDelayedFocusItemIndex != nItemIndex )
	{
		if( m_nDelayedFocusItemIndex >= 0 )
		{
			VERIFY( KillTimer( ID_TIMER_ITEM_FOCUS_DELAY ) );
		}
		//m_nDelayedFocusItemIndex = IDX_NOTHING;
	}
	if( nItemIndex < 0 )
	{
		m_nDelayedFocusItemIndex = IDX_NOTHING;
		return;
	}
	if( m_nDelayedFocusItemIndex == nItemIndex )
		return;
	ASSERT( nItemIndex>=0 && nItemIndex<m_items_all.GetSize() );
	m_nDelayedFocusItemIndex = nItemIndex;
	ASSERT( _GetItemRef(m_nDelayedFocusItemIndex).IsPopup() );
	SetTimer(
		ID_TIMER_ITEM_FOCUS_DELAY,
		ID_PERIOD_ITEM_FOCUS_DELAY,
		NULL
		);
}

int CExtPopupMenuWnd::_GetMaxScrollPos()
{
	ASSERT( m_bScrollingAvailable );
CRect rcClient;
	_GetClientRect( &rcClient );
int nClientHeight =
		rcClient.Height();
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nMenuShadowSize =
		_GetMenuShadowSize();
int nMaxScrollPos = m_sizeFullItems.cy;
	nMaxScrollPos += nMenuBorderSize*2;
	nMaxScrollPos += nMenuShadowSize;
	nMaxScrollPos -= nClientHeight;
	ASSERT( nMaxScrollPos > 0 );
	if( m_bExpandAvailable )
		nMaxScrollPos += _GetSpecBtnHeight(); // xpand btn
	return -nMaxScrollPos;
}

void CExtPopupMenuWnd::OnTimer(UINT nIDEvent) 
{
	switch( nIDEvent )
	{
	case ID_TIMER_DELAY_EXPAND:
	{
		if( m_nWaitingExpandTickCount <
				ID_TOTAL_DELAY_EXPAND
			)
		{
			if( m_nWaitingExpandTickCount <= 1 )
				m_nWaitingExpandTickCount = 0;
			m_nWaitingExpandTickCount +=
				ID_FREQ_DELAY_EXPAND;
			return;
		}
		VERIFY( KillTimer(ID_TIMER_DELAY_EXPAND) );
		m_nWaitingExpandTickCount = 0;
		if( m_bExpandAvailable
			&& (!m_bExpandWasPressed)
			&& m_bAnimFinished
			)
		{ // if expand can be performed
			CPoint point;
			VERIFY( ::GetCursorPos(&point) );
			ScreenToClient( &point );
			CRect rcExpand;
			_GetItemRect( IDX_EXPAND, rcExpand );
			if( rcExpand.PtInRect(point) )
				_DoExpand();
		} // if expand can be performed
		return;
	}
	//case ID_TIMER_DELAY_EXPAND

	case ID_TIMER_SCROLLING:
	{
		_ItemFocusDelay();
		if( m_nCurIndex >= 0 )
			_ItemFocusCancel(TRUE);

		CPoint point;
		VERIFY( ::GetCursorPos(&point) );
		ScreenToClient( &point );

		m_nScrollingDirection = 0;

		CRect rcItem;
		_GetItemRect(IDX_SCROLL_TOP,rcItem);
		if( rcItem.PtInRect(point) )
			m_nScrollingDirection = __SCROLLING_PIXEL_STEP;
		else
		{
			_GetItemRect(IDX_SCROLL_BOTTOM,rcItem);
			if( rcItem.PtInRect(point) )
				m_nScrollingDirection = (-__SCROLLING_PIXEL_STEP);
		}

		m_nDyScrollOffset += m_nScrollingDirection;

		bool bKillThisTimer =
			(m_nScrollingDirection == 0) ? true : false;
		if( m_nDyScrollOffset > 0 )
		{
			m_nDyScrollOffset = 0;
			bKillThisTimer = true;
		}
		else
		{
			int nMaxScrollPos = _GetMaxScrollPos();
			if( m_nDyScrollOffset < nMaxScrollPos )
			{
				m_nDyScrollOffset = nMaxScrollPos;
				bKillThisTimer = true;
			}
		}

		if( bKillThisTimer )
		{
			VERIFY( KillTimer(ID_TIMER_SCROLLING) );
		}
		Invalidate( FALSE );
		return;
	}
	// case ID_TIMER_SCROLLING

	case ID_TIMER_ITEM_FOCUS_DELAY:
	{
		if( m_nDelayedFocusItemIndex >= 0
			&& m_bAnimFinished
			)
		{
			ASSERT( m_nDelayedFocusItemIndex>=0 && m_nDelayedFocusItemIndex<m_items_all.GetSize() );
			ASSERT( _GetItemRef(m_nDelayedFocusItemIndex).IsPopup() );

			CRect rcItem;
			_GetItemRect(m_nDelayedFocusItemIndex,rcItem);
			CPoint point;
			VERIFY( ::GetCursorPos( &point ) );
			ScreenToClient( &point );
			if( rcItem.PtInRect(point) )
			{
				_SetCapture();
				_ItemFocusSet(
					m_nDelayedFocusItemIndex,
					TRUE,
					TRUE
					);
			}
		}
		VERIFY( KillTimer( ID_TIMER_ITEM_FOCUS_DELAY ) );
		m_nDelayedFocusItemIndex = IDX_NOTHING;
		return;
	}
	// case ID_TIMER_ITEM_FOCUS_DELAY
	
	default:
		CExtPopupBaseWnd::OnTimer(nIDEvent);
	break; // default
	} // switch( nIDEvent )
}

CExtPopupMenuWnd * CExtPopupMenuWnd::_GetCapture()
{
//	return GetCapture();
	return g_pWndCapture;
}

void CExtPopupMenuWnd::_SetCapture()
{
	if( GetSafeHwnd() != NULL )
	{
		g_pWndCapture = this;
	}
	else
	{
		g_pWndCapture = NULL;
	}
}

void CExtPopupMenuWnd::_ReleaseCapture()
{
	if( g_pWndCapture == this )
		g_pWndCapture = NULL;
}

int CExtPopupMenuWnd::_GetNextItem(
	next_item_t nit
	)
{
int nCountOfItems = m_items_all.GetSize();
bool bCanExpand = false;
	if( m_bExpandAvailable
		&& (!m_bExpandWasPressed)
		)
	{
		if( IsAllItemsRarelyUsed() )
			return IDX_EXPAND;
		bCanExpand = true;
	}
	if( nit == __NI_NOTHING ||  nCountOfItems == 0 )
	{
		if( bCanExpand )
			return IDX_EXPAND;
		return IDX_NOTHING;
	}

	if( nit == __NI_ANY && m_nCurIndex >= 0 )
		return m_nCurIndex;

	if( m_nCurIndex < 0 )
	{
		INT iter = 0;
		for( ; iter < m_items_all.GetSize(); ++iter )
		{
			menu_item_info_t & mi = m_items_all[ iter ];
			if( mi.IsEnabled() && mi.IsDisplayed() )
				return mi.GetIndex();
		}
		if( bCanExpand )
			return IDX_EXPAND;
		return IDX_NOTHING;
	}

	if( nit == __NI_NEXT )
	{
		if( nCountOfItems < 2 )
		{
			if( bCanExpand )
				return IDX_EXPAND;
			return IDX_NOTHING;
		}

		INT iter = m_nCurIndex + 1;
		for( ; iter < m_items_all.GetSize(); ++iter )
		{
			menu_item_info_t & mi = m_items_all[ iter ];
			if( mi.IsEnabled() && mi.IsDisplayed() )
				return mi.GetIndex();
		}
		if( bCanExpand )
			return IDX_EXPAND;
		if( m_nCurIndex == 0 )
			return IDX_NOTHING;
		iter = 0;
		INT iter2 = m_nCurIndex - 1;
		do
		{
			menu_item_info_t & mi = m_items_all[ iter ];
			if( mi.IsEnabled() && mi.IsDisplayed() )
				return mi.GetIndex();
			iter++;
		} while( iter != iter2 );
		return IDX_NOTHING;
	}
	if( nit == __NI_PREV )
	{
		if( nCountOfItems < 2 )
			return IDX_NOTHING;

		if( m_nCurIndex != 0 )
		{
			int nIndex = m_nCurIndex - 1;
			INT iter = nIndex;
			for( ; true;  )
			{
				menu_item_info_t & mi = m_items_all[ iter ];
				if( mi.IsEnabled() && mi.IsDisplayed() )
					return mi.GetIndex();
				if( nIndex == 0 )
					break;
				--iter;
				--nIndex;
			}
		}
		int nIndex = m_nCurIndex + 1;
		INT iter2 = nIndex;
		INT iter = m_items_all.GetSize() - 1;
		do
		{
			menu_item_info_t & mi = m_items_all[ iter ];
			if( mi.IsEnabled() && mi.IsDisplayed() )
				return mi.GetIndex();
			iter--;
		} while( iter != iter2 );

		return IDX_NOTHING;
	}

	return IDX_NOTHING;
}

bool CExtPopupMenuWnd::_OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( GetSafeHwnd() == NULL )
		return true; //false;

	if( !m_bAnimFinished )
		return true; //false;

next_item_t nit = __NI_NOTHING;
	switch( nChar )
	{
	case VK_RETURN:
	case VK_SPACE:
	{
		if( m_nCurIndex < 0 )
			return true; // break;
		menu_item_info_t & mi = _GetItemRef(m_nCurIndex);
		if( mi.IsExecutableCmdID() )
		{
			_EndSequence(
				mi.GetCmdID(),
				mi.GetCmdReciever()
				);
			return true; // break;
		}
	} // case VK_SPACE or VK_RETURN

	case VK_RIGHT:
	{
		bool bSendChangeMenu = false;
		if( m_nCurIndex < 0 )
		{
			if( m_items_all.GetSize() == 0 )
				return true; // false;
//			nit = __NI_ANY;
			bSendChangeMenu = true;
		}
		else
		{
			menu_item_info_t & mi = _GetItemRef(m_nCurIndex);
			if( !mi.IsPopup() )
			{
				bSendChangeMenu = true;
			}
			else
			{
				if( mi.GetPopup()->GetSafeHwnd() == NULL )
				{
					_ItemFocusSet(
						m_nCurIndex,
						TRUE,
						TRUE
						);
					_ItemFocusDelay();
				}
			}
		}
		if( bSendChangeMenu && nChar==VK_RIGHT )
		{
			ASSERT( ::IsWindow(g_hWndCmdReciever) );
			if( CExtToolControlBar::g_bMenuTracking )
				::PostMessage(
					g_hWndCmdReciever,
					CExtPopupMenuWnd::nMsgPopupNext,
					0,
					0
					);
			return true; // false;
		}
	}
	break; // case VK_RIGHT

	case VK_LEFT:
		if( m_pWndParentMenu == NULL )
		{
			ASSERT( ::IsWindow(g_hWndCmdReciever) );
			if( CExtToolControlBar::g_bMenuTracking )
				::PostMessage(
					g_hWndCmdReciever,
					CExtPopupMenuWnd::nMsgPopupPrev,
					0,
					0
					);
			return true; // false;
		}
		else
		{
			int nParentCurIndex =
				m_pWndParentMenu->m_nCurIndex;
			ASSERT(
				nParentCurIndex >= 0
				&&
				nParentCurIndex <=
					m_pWndParentMenu->m_items_all.GetSize()
				);
			m_pWndParentMenu->_ItemFocusCancel(
				FALSE
				);
			CExtPopupMenuWnd::_PassMsgLoop();
			m_pWndParentMenu->_ItemFocusSet(
				nParentCurIndex,
				FALSE,
				TRUE
				);
			return true;
		}

	case VK_ESCAPE:
		_EndSequence();
		return true;
	// case VK_ESCAPE
	
	case VK_UP:
		nit = __NI_PREV;
	break; // case VK_UP
	case VK_DOWN:
		if( IsKeyPressed( VK_CONTROL ) )
		{
			if( m_bExpandAvailable )
				_DoExpand();
			return true;
		}
		nit = __NI_NEXT;
	break; // case VK_DOWN
	case VK_NEXT:
		nit = __NI_PAGE_DOWN;
	break; // case VK_NEXT
	case VK_PRIOR:
		nit = __NI_PAGE_UP;
	break; // case VK_PRIOR

	default:
	//if( _istalnum(nChar) )
	{ // BLOCK: try analyze quick char access
		if( IsKeyPressed(VK_SHIFT)
			|| IsKeyPressed(VK_CONTROL)
			|| IsKeyPressed(VK_MENU)
			|| ( (nFlags & (1<<29)) != 0 ) // ALT pressed?
			)
			return false;
		WORD nMapped;
		BYTE lpKeyState[256];
		::GetKeyboardState( lpKeyState );

		::ToAsciiEx(
			nChar,
			::MapVirtualKey( nChar, 0 ),
			lpKeyState,
			&nMapped,
			1,
			::GetKeyboardLayout(
				(::AfxGetThread())->m_nThreadID
				)
			);

/*
		TCHAR szChar[2] = { (TCHAR)nMapped, '\0'};
		::CharUpper( szChar );

		int cAccelSearch = szChar[0]; //toupper(nChar);
		if( cAccelSearch != 0 )
		{
			int nStartIdx =
				(m_nCurIndex >= 0) ? (m_nCurIndex + 1) : 0;
			int nCount = m_items_all.GetSize(); //ItemGetCount();
			for( int nIdx = nStartIdx; nIdx < nCount; nIdx++ )
			{
				menu_item_info_t & mi = _GetItemRef(nIdx);
				if( !mi.AccelCharIsSet() )
					continue;
				if( !mi.IsDisplayed() )
					continue;
				TCHAR cAccel = mi.AccelCharGet();
				if( cAccelSearch ==
						cAccel // _totupper(cAccel)
					)
				{
					if( m_nCurIndex != nIdx )
					{
						_ItemFocusSet(
							nIdx,
							TRUE,
							TRUE
							);
						return true;
					}
				}
			} // for( int nIdx = nStartIdx; nIdx < nCount; nIdx++ )
			if( nStartIdx != 0 )
			{
				for( nIdx=0; nIdx<m_nCurIndex; nIdx++ )
				{
					menu_item_info_t & mi = _GetItemRef(nIdx);
					if( !mi.AccelCharIsSet() )
						continue;
					if( !mi.IsDisplayed() )
						continue;
					TCHAR cAccel = mi.AccelCharGet();
					if( cAccelSearch ==
							cAccel // _totupper(cAccel)
							)
					{
						if( m_nCurIndex != nIdx )
						{
							_ItemFocusSet(
								nIdx,
								TRUE,
								TRUE
								);
							return true;
						}
					}
				} // for( nIdx=0; nIdx<m_nCurIndex; nIdx++ )
			} // if( nStartIdx != 0 )
			return true; // false;
		} // if( cAccelSearch != 0 )
*/
		INT nNextIdx =
			ItemFindByAccessChar(
				(TCHAR)nMapped,
				m_nCurIndex // (m_nCurIndex >= 0) ? m_nCurIndex : -1
				);
		if(		nNextIdx < 0
			&&	m_nCurIndex >= 0
			&&	ItemFindByAccessChar( (TCHAR)nMapped, -1 )
					== m_nCurIndex
			)
		{ // if only 1 with this char
			menu_item_info_t & mi =
				_GetItemRef( m_nCurIndex );
			if( mi.IsExecutableCmdID() )
			{
				_EndSequence(
					mi.GetCmdID(),
					mi.GetCmdReciever()
					);
				return true;
			} // if( mi.IsExecutableCmdID() )
		} // if only 1 with this char
		if( m_nCurIndex != nNextIdx
			&& nNextIdx >= 0
			)
		{
			_ItemFocusSet( nNextIdx, TRUE, TRUE );
			if( ItemFindByAccessChar(
					(TCHAR)nMapped,
					nNextIdx
					) < 0
				)
			{ // if only 1 with this char
				menu_item_info_t & mi =
					_GetItemRef( nNextIdx );
				if( mi.IsExecutableCmdID() )
				{
					_EndSequence(
						mi.GetCmdID(),
						mi.GetCmdReciever()
						);
					return true;
				} // if( mi.IsExecutableCmdID() )
			} // if only 1 with this char
			return true;
		}

	} // BLOCK: try analyze quick char access

	} // switch( nChar )

	if( nit != __NI_NOTHING )
	{
		int nItemIndex = _GetNextItem(nit);
		if( m_items_all.GetSize() > 0
			&& nItemIndex >= 0
			&& nItemIndex != m_nCurIndex
			)
		{
			_ItemFocusSet(
				nItemIndex,
				FALSE,
				TRUE
				);
			return true; // false;
		}
		if( nItemIndex == IDX_EXPAND )
		{
			ASSERT( m_bExpandAvailable );
			ASSERT( !m_bExpandWasPressed );
			int nItemIndex = m_nCurIndex;
			_DoExpand();
			if( nItemIndex >= 0 )
			{
				_ItemFocusSet(
					nItemIndex,
					FALSE,
					m_bAnimFinished ? TRUE : FALSE // TRUE
					);
				int i = _GetNextItem(__NI_NEXT);
				_ItemFocusSet(
					(i>0) ? i : nItemIndex,
					FALSE,
					m_bAnimFinished ? TRUE : FALSE // TRUE
					);
			}
			return true;
		}
	} // if( nit != __NI_NOTHING )

	return false;
}

INT CExtPopupMenuWnd::ItemFindByAccessChar(
	TCHAR chrAccess,
	INT nStartIdx, // = -1
	BOOL bRestartAt0 // = TRUE
	) const
{
	if( chrAccess == 0 )
		return -1;
TCHAR szChar[2] = { chrAccess, '\0'};
	::CharUpper( szChar );

int cAccelSearch = szChar[0]; //toupper(nChar);
	if( cAccelSearch == 0 )
		return -1;
int nCount = m_items_all.GetSize(); //ItemGetCount();
int nIdx = (nStartIdx >= 0) ? (nStartIdx + 1) : 0;
	for( ; nIdx < nCount; nIdx++ )
	{
		const menu_item_info_t & mi = _GetItemRef(nIdx);
		if( !mi.AccelCharIsSet() )
			continue;
		if( !mi.IsDisplayed() )
			continue;
		if( !mi.IsEnabled() )
			continue;
		TCHAR cAccel = mi.AccelCharGet();
		if( cAccelSearch ==
				cAccel // _totupper(cAccel)
			)
		{
			if( nStartIdx != nIdx )
				return nIdx;
		}
	} // for( nIdx = nStartIdx; nIdx < nCount; nIdx++ )
	if( nStartIdx == 0 )
		return -1;
	for( nIdx = 0; nIdx<nStartIdx; nIdx++ )
	{
		const menu_item_info_t & mi = _GetItemRef(nIdx);
		if( !mi.AccelCharIsSet() )
			continue;
		if( !mi.IsDisplayed() )
			continue;
		if( !mi.IsEnabled() )
			continue;
		TCHAR cAccel = mi.AccelCharGet();
		if( cAccelSearch ==
				cAccel // _totupper(cAccel)
				)
		{
			if( nStartIdx != nIdx )
				return nIdx;
		}
	} // for( nIdx = 0; nIdx<nStartIdx; nIdx++ )
	return -1;
}

LRESULT CALLBACK CExtPopupMenuWnd::_HookMouseProc(
	int nCode,      // hook code
	WPARAM wParam,  // message identifier
	LPARAM lParam   // mouse coordinates
	)
{
/*
	if( nCode < 0 )
	{
		return
			::CallNextHookEx(
				g_hMouseHook,
				nCode,
				wParam,
				lParam
				);
	}
*/

MOUSEHOOKSTRUCT* lpMS = (MOUSEHOOKSTRUCT*)lParam;
	ASSERT( lpMS != NULL );

	if( g_pWndCapture != NULL )
	{
		switch( wParam )
		{
//		case WM_MOUSEACTIVATE:
//			if( lpMS->hwnd != g_pWndCapture->GetSafeHwnd() )
//				return 1;
//		break;

		case WM_MOUSEMOVE:
		{
			if( ::IsWindow( g_pWndCapture->GetSafeHwnd() ) )
			{
				CPoint pt( lpMS->pt );
				g_pWndCapture->ScreenToClient( &pt );
				g_pWndCapture->
					_OnMouseMove(
						wParam,
						pt
						);
			} // if( ::IsWindow( g_pWndCapture->GetSafeHwnd() ) )
		}
		break;

		case WM_MOUSEWHEEL:
					return 1; // eat!

		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		{
			CPoint pt( lpMS->pt );
			//g_pWndCapture->ScreenToClient( &pt );
			CExtPopupMenuWnd * pPopup =
				CExtPopupMenuWnd::_GetCapture();
			for(; pPopup != NULL; pPopup = pPopup->m_pWndParentMenu )
			{
				//ASSERT( ::IsWindow(pPopup->GetSafeHwnd()) );
				if( !::IsWindow(pPopup->GetSafeHwnd()) )
					return 1; // eat!

				CRect wrMenuFrame;
				pPopup->GetWindowRect( &wrMenuFrame );
				if( wrMenuFrame.PtInRect(pt) )
					return 1; // eat!
			}
			pPopup = g_CurrentPopup.GetInstance();
			ASSERT_VALID( pPopup );
			ASSERT( ::IsWindow(pPopup->GetSafeHwnd()) );
			pPopup->PostMessage( WM_CANCELMODE	);

		if(	wParam != WM_LBUTTONDOWN
			&&
			wParam != WM_RBUTTONDOWN
			&&
			wParam != WM_MBUTTONDOWN
			)
			// currently NC areas are not safe on MDI apps
			return 1; // eat!

		}
		break;

		case WM_NCLBUTTONUP:
		case WM_NCRBUTTONUP:
		case WM_NCMBUTTONUP:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		{
			if( ::IsWindow( g_pWndCapture->GetSafeHwnd() ) )
			{
				CPoint pt( lpMS->pt );
				g_pWndCapture->ScreenToClient( &pt );
				if(	g_pWndCapture->
						_OnMouseClick(
							wParam,
							pt
							)
						)
					return 1; // eat!
			} // if( ::IsWindow( g_pWndCapture->GetSafeHwnd() ) )
		}
		break;

//		default:
//			return 1; // eat!
		} // switch( wParam )
	}
	return
		::CallNextHookEx(
			g_hMouseHook,
			nCode,
			wParam,
			lParam
			);
}

BOOL CExtPopupMenuWnd::IsCmdKeyActivation(
	LPARAM lParam // = 0
	)
{
	if( (lParam & (1<<29)) != 0 ) // ALT pressed?
		return TRUE;
	
static int nTestKeys[] = {
	VK_MENU,VK_RMENU,VK_LMENU,
	VK_CONTROL,VK_RCONTROL,VK_LCONTROL,
	VK_SHIFT,VK_RSHIFT,VK_LSHIFT
};
	for( int i=0; i<sizeof(nTestKeys)/sizeof(int); i++ )
	{
		if( IsKeyPressed(nTestKeys[i]) )
		{
//			if(		nTestKeys[i] == VK_CONTROL
//				||	nTestKeys[i] == VK_RCONTROL
//				||	nTestKeys[i] == VK_LCONTROL
//				)
//			{
//				if( IsKeyPressed(VK_DOWN) )
//					return FALSE;
//			}
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT CALLBACK CExtPopupMenuWnd::_HookKeyboardProc(
	int nCode,      // hook code
	WPARAM wParam,  // virtual-key code
	LPARAM lParam   // keystroke-message information
	)
{
/*
	if( nCode < 0 )
	{
		return
			::CallNextHookEx(
				g_hKeyboardHook,
				nCode,
				wParam,
				lParam
				);
	}
*/

//	if(		(!CExtToolControlBar::g_bMenuTracking)
//		&&	IsCmdKeyActivation( lParam )
//		&&	( lParam & (1<<31) ) != 0
//		)
//	{
//		CancelMenuTracking();
//		return
//			::CallNextHookEx(
//				g_hKeyboardHook,
//				nCode,
//				wParam,
//				lParam
//				);
//	}

	if( g_pWndCapture != NULL )
	{
		if( (lParam & (1<<31)) == 0 )
		{
			// clocks allows to fix problem with
			// duplicated fast key hook calls
			static clock_t _clock_step =
				CLOCKS_PER_SEC / 50; // 1/50 of second
			static clock_t _clock_last =
				0; //clock();
			clock_t _clock_curr =
				clock();
			//ASSERT( _clock_curr >= _clock_last );
			clock_t _clock_diff =
				abs(_clock_curr - _clock_last);
			if( _clock_diff >= _clock_step )
			{
				_clock_last = _clock_curr;
				if(	g_pWndCapture->
						_OnKeyDown(
							wParam,
							LOWORD(lParam),
							HIWORD(lParam)
							)
					)
					return 1; // eat!
				if( g_pWndCapture != NULL )
					return 1; // eat!
			}
		}
		if( !CExtToolControlBar::g_bMenuTracking )
			return 1; // eat!
	} // if( g_pWndCapture != NULL )

	if(		(!CExtToolControlBar::g_bMenuTracking)
		&&	IsCmdKeyActivation( lParam )
		&&	( lParam & (1<<31) ) != 0
		)
	{
		CancelMenuTracking();
		return
			::CallNextHookEx(
				g_hKeyboardHook,
				nCode,
				wParam,
				lParam
				);
	}

	return
		::CallNextHookEx(
			g_hKeyboardHook,
			nCode,
			wParam,
			lParam
			);
}

void CExtPopupMenuWnd::_Hook(
	bool bHook // = true
	)
{
	if( bHook )
	{
		if( CExtPopupMenuWnd::g_hMouseHook == NULL )
		{
			CExtPopupMenuWnd::g_hMouseHook =
				::SetWindowsHookEx(
					WH_MOUSE,
					CExtPopupMenuWnd::_HookMouseProc, 
					0,
					::GetCurrentThreadId()
					);
			ASSERT( CExtPopupMenuWnd::g_hMouseHook != NULL );
		}
		if( CExtPopupMenuWnd::g_hKeyboardHook == NULL )
		{
			CExtPopupMenuWnd::g_hKeyboardHook =
				::SetWindowsHookEx(
					WH_KEYBOARD,
					CExtPopupMenuWnd::_HookKeyboardProc, 
					0,
					::GetCurrentThreadId()
					);
			ASSERT( CExtPopupMenuWnd::g_hKeyboardHook != NULL );
		}
	} // if( bHook )
	else
	{
		if( CExtPopupMenuWnd::g_hMouseHook != NULL )
		{
			::UnhookWindowsHookEx(
				CExtPopupMenuWnd::g_hMouseHook
				);
			CExtPopupMenuWnd::g_hMouseHook = NULL;
		}
		if( CExtPopupMenuWnd::g_hKeyboardHook != NULL )
		{
			::UnhookWindowsHookEx(
				CExtPopupMenuWnd::g_hKeyboardHook
				);
			CExtPopupMenuWnd::g_hKeyboardHook = NULL;
		}
	} // else from if( bHook )
}

INT CExtPopupMenuWnd::ItemGetCount() const
{
//	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = m_items_all.GetSize();
	return nCountOfItems;
}

INT CExtPopupMenuWnd::ItemFindPosForCmdID(
	UINT nCmdID,
	INT nPosStart // = -1
	) const
{
INT nPos = (nPosStart < 0) ? 0 : nPosStart + 1;
INT nCount = ItemGetCount();
	for( ; nPos < nCount; nPos++ )
	{
		UINT nCmdID2 = ItemGetCmdID( nPos );
		if( nCmdID == nCmdID2 )
			return nPos;
	}
	return -1; // not found
}

UINT CExtPopupMenuWnd::ItemGetCmdID( // menu_item_type_t values can be returned
	INT nPos
	) const
{
	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return TYPE_SEPARATOR;
	}
const menu_item_info_t & mi = _GetItemRef(nPos);
UINT nCmdID = mi.GetCmdID();
	return nCmdID;
}

bool CExtPopupMenuWnd::ItemSetPopupIcon(
	INT nPos,
	HICON hIcon // = NULL // no icon by default
	)
{
	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return false;
	}
menu_item_info_t & mi = _GetItemRef(nPos);
	return mi.SetPopupIcon( hIcon );
}

bool CExtPopupMenuWnd::ItemSetPopupText(
	INT nPos,
	LPCTSTR sText // NULL if empty
	)
{
	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return false;
	}
menu_item_info_t & mi = _GetItemRef(nPos);
	return mi.SetPopupText( sText );
}

bool CExtPopupMenuWnd::ItemSetPopupAccelText(
	INT nPos,
	LPCTSTR sText // NULL if empty
	)
{
	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return false;
	}
menu_item_info_t & mi = _GetItemRef(nPos);
	return mi.SetPopupAccelText( sText );
}

HICON CExtPopupMenuWnd::ItemGetIcon( INT nPos ) const
{
	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return NULL;
	}
const menu_item_info_t & mi = _GetItemRef(nPos);
	return ((menu_item_info_t & )mi).GetIcon();
}

CString CExtPopupMenuWnd::ItemGetText( INT nPos ) const
{
	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return CString( _T("") );
	}
const menu_item_info_t & mi = _GetItemRef(nPos);
	return CString( mi.GetText() );
}

CString CExtPopupMenuWnd::ItemGetAccelText( INT nPos ) const
{
	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return CString( _T("") );
	}
const menu_item_info_t & mi = _GetItemRef(nPos);
	return CString( mi.GetAccelText() );
}

CExtPopupMenuWnd * CExtPopupMenuWnd::ItemGetPopup(
	INT nPos
	)
{
	ASSERT( GetTrackingMenu() != this );
const CExtPopupMenuWnd * pChild =
		((const CExtPopupMenuWnd *)this)->
			ItemGetPopup( nPos );
	if( pChild == NULL )
		return NULL;
	return
		const_cast
			<CExtPopupMenuWnd *>
				(pChild);
}

void CExtPopupMenuWnd::_DoExpand()
{
	_ItemFocusDelay();
	_CoolTipHide( false );
	if( (!m_bAnimFinished)
		|| _CoolTipIsVisible()
		)
		return;

	ASSERT( m_bExpandAvailable );
	ASSERT( !m_bExpandWasPressed );
	ASSERT( ::IsWindow(GetSafeHwnd()) );
	ASSERT( m_bAnimFinished );
	
//	_ItemFocusCancel( FALSE );
	
	m_bExpandWasPressed = true;
CSize m_sizeFullItems2 = m_sizeFullItems;
	_SyncItems();
	_RecalcLayoutImpl();

	if( m_sizeFullItems != m_sizeFullItems2 )
	{ // if size of all items was changed
		ASSERT(
			m_sizeFullItems.cx >= m_sizeFullItems2.cx
			&&
			m_sizeFullItems.cy >= m_sizeFullItems2.cy
			);

		CRect rcWndOld;
		GetWindowRect( &rcWndOld );
		
		/*
		int nMenuShadowSize =
			_GetMenuShadowSize();
		int nMenuBorderSize =
			g_PaintManager->GetMenuBorderSize();
		int nXtraSz =
			nMenuBorderSize*2 + nMenuShadowSize;
		/ *
			this conditional ShowWindow not work
			fine coz wndclass has CS_SAVEBITS
		if(	(m_sizeFullItems.cx - m_sizeFullItems2.cx)
				< nXtraSz
			||
			(m_sizeFullItems.cy - m_sizeFullItems2.cy)
				< nXtraSz
			)
		*/
		{

//			_BitsRestore();
			VERIFY(
				SetWindowRgn(
					NULL,
					FALSE
					)
				);
			ShowWindow( SW_HIDE	);
			ASSERT( !IsWindowVisible() );
			//_PassMsgLoop();
		}

		if( !rcWndOld.IsRectEmpty() )
		{
			CWnd * pWndTLP = GetTopLevelParent();
			if( pWndTLP != NULL )
			{
				ASSERT_VALID( pWndTLP );
				if( pWndTLP->IsWindowVisible() )
				{
					CRect rcTLP;
					pWndTLP->GetWindowRect( &rcTLP );
					if( !rcTLP.IsRectEmpty() )
					{
						CRect rcIntersection;
						if(	rcIntersection.IntersectRect(
								&rcTLP,
								&rcWndOld
								)
							)
							pWndTLP->UpdateWindow();
					} // if( !rcTLP.IsRectEmpty() )
				} // if( pWndTLP->IsWindowVisible() )
			} // if( pWndTLP != NULL )
		} // if( !rcWndOld.IsRectEmpty() )

		_FreeWinObjects();

		ASSERT( ::IsWindow(GetSafeHwnd()) );
		ASSERT( m_bAnimFinished );

		// adjust screen position
		CRect rcWnd = _CalcTrackRect();
		
		if( m_rgnWnd.GetSafeHandle() != NULL )
		{
			ASSERT( m_bExcludeAreaSpec );
			ASSERT( m_bCombineWithEA );
			ASSERT( m_eCombineAlign != __CMBA_NONE );
			CRgn rgnTmp;
			VERIFY( rgnTmp.CreateRectRgn(0,0,0,0) );
			rgnTmp.CopyRgn( &m_rgnWnd );
			ASSERT( rgnTmp.GetSafeHandle() != NULL );
			VERIFY(
				SetWindowRgn(
					(HRGN)rgnTmp.Detach(),
					FALSE
					)
				);
		} // if( m_rgnWnd.GetSafeHandle() != NULL )

		m_AnimationType =
			g_bMenuExpandAnimation ?
				__AT_CONTENT_EXPAND : __AT_CONTENT_DISPLAY
			;
		_StartAnimation();
		if( m_AnimationType == __AT_NONE )
		{
			m_AnimationType = __AT_CONTENT_DISPLAY;
			_StartAnimation();
			ASSERT( m_AnimationType == __AT_CONTENT_DISPLAY );
		}

		if( m_bScrollingAvailable )
			_RecalcLayoutImpl();
		SetWindowPos(
			NULL,
			rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(),
			SWP_NOACTIVATE
				|SWP_SHOWWINDOW
				//|SWP_NOREDRAW //|SWP_NOSENDCHANGING
				|SWP_NOZORDER //|SWP_NOOWNERZORDER
				//|SWP_NOCOPYBITS
			);

		ASSERT( IsWindowVisible() );

/*
UpdateWindow();
		::Sleep(1000);
		ShowWindow(SW_HIDE);
		_FreeWinObjects();
		::Sleep(1000);
		ShowWindow(SW_SHOWNOACTIVATE);
*/

	} // if size of all items was changed
	else
		UpdateWindow();

	ASSERT( g_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(g_hWndCmdReciever) );
	::SendMessage(
		g_hWndCmdReciever,
		nMsgNotifyMenuExpanded,
		0,
		0
		);
}

bool CExtPopupMenuWnd::IsAllItemsRarelyUsed() const
{
INT _iter = 0;
	for( ; _iter < m_items_all.GetSize(); ++_iter )
	{
		const menu_item_info_t & mi = m_items_all[ _iter ];
		if( mi.IsDisplayed() )
			return false;
	} // for( ; _iter != m_items_all.end(); ++_iter )
	return true;
}

void CExtPopupMenuWnd::_SyncItems()
{
	ASSERT( g_hWndCmdReciever != NULL );
//HWND hWndDummyCmdRecv = NULL;
//	if( g_hWndCmdReciever == NULL )
//		g_hWndCmdReciever = hWndDummyCmdRecv = ::AfxGetMainWnd()->GetSafeHwnd();
	m_bExpandAvailable = false;
	m_sizeFullItems.cx = m_sizeFullItems.cy = 1;
int nItemIndex = 0;
INT _iterLast = m_items_all.GetSize();
INT _iterLastVisSep = _iterLast;
bool bAtLeastOneDisplayed = false;
INT _iter = 0;
	for( ; _iter < m_items_all.GetSize(); ++_iter )
	{
		menu_item_info_t & mi = m_items_all[ _iter ];
		mi.SetIndex( nItemIndex++ );

		if( m_bExpandWasPressed )
		{
			mi.SetDisplayed( true );
		} // if( m_bExpandWasPressed )
		else
		{
			if( mi.IsPopup() )
			{ // if popup sub-menu
				ASSERT( mi.GetPopup() != NULL );
				mi.GetPopup()->_SyncItems();
				if( mi.IsAllItemsRarelyUsed() )
				{ // if all not displayed
					_iterLast = m_items_all.GetSize();
					mi.SetDisplayed( false );
				} // if all not displayed
				else
				{ // if at least one displayed
					_iterLast = _iter;
					_iterLastVisSep = m_items_all.GetSize();
					mi.SetDisplayed( true );
				} // if at least one displayed
			} // if popup sub-menu
			else
			{ // if separator or command item
				if( mi.IsSeparator() )
				{ // if separator
					if( _iterLast != m_items_all.GetSize()
						||
						(	 _iterLast == m_items_all.GetSize()
							&& (_iterLastVisSep == m_items_all.GetSize())
							&& bAtLeastOneDisplayed
							)
						)
					{
						_iterLastVisSep = _iter;
						mi.SetDisplayed( true );
					}
					else
					{
						_iterLast = m_items_all.GetSize();
						mi.SetDisplayed( false );
					}
				} // if separator
				else
				{ // if command item
					CExtCmdManager::cmd_t * p_cmd =
						mi.GetCmd();
					if( p_cmd->StateIsRarelyUsed() )
					{ // if command rarely used
						_iterLast = m_items_all.GetSize();
						mi.SetDisplayed( false );
					} // if command rarely used
					else
					{ // if basic command or frequently used
						_iterLast = _iter;
						_iterLastVisSep = m_items_all.GetSize();
						mi.SetDisplayed( true );
					} // if basic command or frequently used
				} // if command item
			} // if separator or command item
		} // else from if( m_bExpandWasPressed )

		if( mi.IsDisplayed() )
		{
			bAtLeastOneDisplayed = true;
			m_sizeFullItems.cy += mi.GetMeasuredHeight();
			if( m_sizeFullItems.cx < mi.GetMeasuredWidth() )
				m_sizeFullItems.cx = mi.GetMeasuredWidth();
		} // if( mi.IsDisplayed() )
		else
		{
			ASSERT( !m_bExpandWasPressed );
			m_bExpandAvailable = true;
		} // else from if( mi.IsDisplayed() )
	} // for( ; _iter < m_items_all.GetSize(); ++_iter )

	if( _iterLastVisSep != m_items_all.GetSize() )
	{ // remove last separator
		if( _iterLastVisSep >= _iterLast
			||
			_iterLast == m_items_all.GetSize()
			)
		{
			m_items_all[_iterLastVisSep].SetDisplayed( false );
			m_sizeFullItems.cy -=
				m_items_all[_iterLastVisSep].GetMeasuredHeight();
		}
	} // remove last separator

int nMixDx =
		//g_PaintManager->GetMenuBorderSize()*2
		//+ _GetMenuShadowSize()
		+ 80;
	if( m_sizeFullItems.cx < nMixDx )
		m_sizeFullItems.cx = nMixDx;
//	if( g_hWndCmdReciever == hWndDummyCmdRecv )
//		g_hWndCmdReciever = NULL;
}

const CExtPopupMenuWnd * CExtPopupMenuWnd::ItemGetPopup(
	INT nPos
	) const
{
	ASSERT( GetTrackingMenu() != this );
INT nCountOfItems = ItemGetCount();
	if( nPos < 0 || nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return NULL;
	}
const menu_item_info_t & mi = _GetItemRef(nPos);
UINT nCmdID = mi.GetCmdID();
	if( nCmdID != TYPE_POPUP )
		return NULL;
	ASSERT( mi.IsPopup() );
	return mi.GetPopup();
}

BOOL CExtPopupMenuWnd::ItemRemove(
	INT nPos // = -1 // remove all
	)
{
	ASSERT( GetTrackingMenu() != this );

	if( nPos < 0 )
	{
		if( !_BuildItems( NULL,NULL,false) )
		{
			ASSERT( FALSE );
			return FALSE;
		}
		_SyncItems();
		return TRUE;
	} // if( nPos < 0 )

INT nCountOfItems = ItemGetCount();
	if( nPos >= nCountOfItems )
	{
		ASSERT( FALSE );
		return FALSE;
	}

menu_item_info_t & mi = _GetItemRef(nPos);
	if( mi.IsPopup() )
	{
		mi.GetPopup()->_OnCancelMode();
		VERIFY( mi.GetPopup()->_BuildItems(NULL,NULL,false) );
		mi.DestroyPopup();
	}
	m_items_all.RemoveAt( nPos );
	ASSERT( nCountOfItems == m_items_all.GetSize() + 1 );

	_SyncItems();

	return TRUE;
}

BOOL CExtPopupMenuWnd::ItemInsertSpecPopup( // insert specific popup implementation
	CExtPopupMenuWnd * pSpecPopup,
	INT nPos, // = -1 // append
	LPCTSTR sText, // = NULL
	HICON hIcon // = NULL
	)
{
	ASSERT( pSpecPopup != NULL );

INT nCountCountOfItems = ItemGetCount();
	if( nPos < 0 )
		nPos = nCountCountOfItems;
	if( nPos > nCountCountOfItems )
	{
		ASSERT( FALSE );
		return FALSE;
	}

menu_item_info_t mi;
	mi.SetPopup( nPos, sText, hIcon, pSpecPopup );
	ASSERT( mi.GetPopup()->m_pWndParentMenu == NULL ); // still not initialized
	mi.GetPopup()->m_pWndParentMenu = this;

	_InsertItem( nPos, mi );

	if( m_bTopLevel )
		_SyncItems();
	else
	{
		ASSERT( m_pWndParentMenu != NULL );
		m_pWndParentMenu->_SyncItems();
	}

	return TRUE;
}

BOOL CExtPopupMenuWnd::ItemInsert(
	UINT nCmdID, // = TYPE_SEPARATOR,
	INT nPos, // = -1 // append
	LPCTSTR sText, // = NULL // for TYPE_POPUP only
	HICON hIcon, // = NULL // for TYPE_POPUP only
	HWND hWndSpecCmdReciever // = NULL // specific command reciever
	)
{
	ASSERT( GetTrackingMenu() != this );

INT nCountCountOfItems = ItemGetCount();
	if( nPos < 0 )
		nPos = nCountCountOfItems;
	if( nPos > nCountCountOfItems )
	{
		ASSERT( FALSE );
		return FALSE;
	}

menu_item_info_t mi;

	switch( nCmdID )
	{
	case TYPE_POPUP:
		mi.SetPopup( nPos, sText, hIcon );
		break; // case TYPE_POPUP
	case TYPE_SEPARATOR:
		mi.SetSeparator( nPos );
		break; // case TYPE_SEPARATOR
	default:
	{
		ASSERT( g_hWndCmdReciever != NULL );
		CExtCmdManager::cmd_t * p_cmd =
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( g_hWndCmdReciever ),
				nCmdID
				);
		ASSERT( p_cmd != NULL );
		if( p_cmd == NULL )
			return FALSE;
		mi.SetCmdReciever( hWndSpecCmdReciever );
		VERIFY(
			mi.UpdateCmdManagerCommand(
				p_cmd,
				nPos
				)
			);
	}
	break; // default
	} // switch( nCmdID )

	if( mi.IsPopup() )
	{
		ASSERT( mi.GetPopup()->m_pWndParentMenu == NULL ); // still not initialized
		mi.GetPopup()->m_pWndParentMenu = this;
	}
	
	_InsertItem( nPos, mi );

	if( m_bTopLevel )
		_SyncItems();
	else
	{
		ASSERT( m_pWndParentMenu != NULL );
		m_pWndParentMenu->_SyncItems();
	}

	return TRUE;
}

BOOL CExtPopupMenuWnd::CreatePopupMenu( HWND hWndCmdRecv )
{
///	g_CurrentPopup.DoneInstance();
//	while( IsMenuTracking() )
//		_PassMsgLoop();

	ASSERT( GetTrackingMenu() != this );
	if( m_bTopLevel
		|| m_pWndParentMenu != NULL
		|| ItemGetCount() != 0
		)
	{
		// already created
		ASSERT( FALSE );
		return FALSE;
	}
	m_sizeFullItems.cx = m_sizeFullItems.cy = 1;
	m_bTopLevel = true;
	m_pWndParentMenu = NULL;

	g_hWndCmdReciever = hWndCmdRecv;
	ASSERT( g_hWndCmdReciever != NULL );
	
	return TRUE;
}

HWND CExtPopupMenuWnd::GetCmdTargetWnd(
	BOOL bValidate //= TRUE
	)
{
	if( bValidate )
		if( !CExtPopupMenuWnd::IsMenuTracking() )
			return NULL;
	ASSERT( g_hWndCmdReciever != NULL );
	ASSERT( ::IsWindow(g_hWndCmdReciever) );
	return g_hWndCmdReciever;
}

void CExtPopupMenuWnd::SetCmdTargetToAllItems(
	HWND hWndSpecCmdReciever, // = NULL
	bool bOnlyThisLevel // = false
	)
{
	ASSERT( GetTrackingMenu() != this );

INT iter = 0;
	for( ; iter < m_items_all.GetSize(); iter++)
	{
		menu_item_info_t & mi = m_items_all[ iter ];
		if( mi.IsSeparator() )
			continue;
		if( mi.IsPopup() )
		{
			if( !bOnlyThisLevel )
				mi.GetPopup()->SetCmdTargetToAllItems(
					hWndSpecCmdReciever,
					true
					);
		}
		else
		{
			mi.SetCmdReciever( hWndSpecCmdReciever );
		}
	}
}

/*
CExtPopupMenuWnd::const_items_container_t &
	CExtPopupMenuWnd::_GetItemsContaner() const
{
	return m_items_all;
}

CExtPopupMenuWnd::items_container_t &
	CExtPopupMenuWnd::_GetItemsContaner()
{
	return m_items_all;
}
*/

CExtPopupMenuWnd::menu_item_info_t &
	CExtPopupMenuWnd::_GetItemRef(int nIndex)
{
	ASSERT( nIndex >= 0 && nIndex < m_items_all.GetSize() );
menu_item_info_t & mi = m_items_all[nIndex];
	return mi;
}

const CExtPopupMenuWnd::menu_item_info_t &
	CExtPopupMenuWnd::_GetItemRef(int nIndex) const
{
	return
		(const_cast< CExtPopupMenuWnd * > (this)) ->
			_GetItemRef(nIndex);
}

void CExtPopupMenuWnd::_InsertItem(
	int nInsertBefore,
	menu_item_info_t & mi
	)
{
	m_items_all.InsertAt(
		(nInsertBefore < 0)
			? m_items_all.GetSize()
			: nInsertBefore
			,
		mi
		);
}

/*
void CExtPopupBaseWnd::_BitsSave( CDC &dc )
{
	if( m_bmpSaveBits.GetSafeHandle() != NULL )
	{
//		VERIFY(
//			m_bmpSaveBits.DeleteObject()
//			);
		return;
	}

int nBitsPerPixel = CExtPaintManager::stat_GetBPP();
	if( nBitsPerPixel < 8 )
		return;

CRect rcClient;
	GetClientRect( &rcClient );
int cx = rcClient.Width();
int cy = rcClient.Height();
	if( cx<=0 || cy<=0 )
		return;

//CClientDC dc( this );
CDC dcmm;
	if( !dcmm.CreateCompatibleDC(&dc) )
	{
		ASSERT( FALSE );
		return;
	}

	// Fill in the BITMAPINFOHEADER
BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = cx;
	bih.biHeight = cy;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = cx * cy;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

COLORREF * p_clrValues = NULL;
HBITMAP hDIB =
		::CreateDIBSection(
			dcmm.GetSafeHdc(),
			(LPBITMAPINFO)&bih,
			DIB_RGB_COLORS,
			(void **)&p_clrValues,
			NULL,
			NULL
			);
	if( hDIB == NULL || p_clrValues == NULL )
	{
		ASSERT( FALSE );
		return;
	}
	m_bmpSaveBits.Attach( hDIB );
CBitmap * pBmpOld =
		dcmm.SelectObject( &m_bmpSaveBits );
	VERIFY(
		dcmm.BitBlt(
			0, 0, cx, cy, &dc,
			rcClient.left,
			rcClient.top,
			SRCCOPY
			)
		);
	dcmm.SelectObject( pBmpOld );
}

void CExtPopupBaseWnd::_BitsRestore()
{
	if( m_bmpSaveBits.GetSafeHandle() == NULL )
		return;
int nBitsPerPixel = CExtPaintManager::stat_GetBPP();
	if( nBitsPerPixel < 8 )
		return;

CRect rcClient;
	GetClientRect( &rcClient );
int cx = rcClient.Width();
int cy = rcClient.Height();
	if( cx<=0 || cy<=0 )
		return;

CClientDC dc( this );
CDC dcmm;
	if( !dcmm.CreateCompatibleDC(&dc) )
	{
		ASSERT( FALSE );
		return;
	}
CBitmap * pBmpOld =
		dcmm.SelectObject( &m_bmpSaveBits );
	VERIFY(
		dc.BitBlt(
			rcClient.left, rcClient.top, cx, cy, 
			&dcmm,
			0,
			0,
			SRCCOPY
			)
		);
	dcmm.SelectObject( pBmpOld );
}
*/

/////////////////////////////////////////////////////////////////////////////
// CExtPopupColorMenuWnd

IMPLEMENT_DYNCREATE(CExtPopupColorMenuWnd, CExtPopupMenuWnd)

BEGIN_MESSAGE_MAP(CExtPopupColorMenuWnd, CExtPopupMenuWnd)
	//{{AFX_MSG_MAP(CExtPopupColorMenuWnd)
	//}}AFX_MSG_MAP
    ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()
END_MESSAGE_MAP()

UINT CExtPopupColorMenuWnd::nMsgNotifyColorChanged =
	::RegisterWindowMessage(
		_T("CExtPopupColorMenuWnd::nMsgNotifyColorChanged")
		);
UINT CExtPopupColorMenuWnd::nMsgNotifyColorChangedFinally =
	::RegisterWindowMessage(
		_T("CExtPopupColorMenuWnd::nMsgNotifyColorChangedFinally")
		);

UINT CExtPopupColorMenuWnd::nMsgNotifyCustColor =
	::RegisterWindowMessage(
		_T("CExtPopupColorMenuWnd::nMsgNotifyCustColor")
		);

#define __NCLR_DX 8
#define __NCLR_DY 5
#define __NCLR_COUNT (__NCLR_DX*__NCLR_DY)
#define __NCLR_BOX_DX_SIZE 16
#define __NCLR_BOX_DY_SIZE 16
#define __NCLR_BOX_DX_SPACE 2
#define __NCLR_BOX_DY_SPACE 2

// table captured from color picker control source by
// Chris Maunder
CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY
	CExtPopupColorMenuWnd::g_colors[] =
{
    { RGB(0x00, 0x00, 0x00),    _T("Black")             },
    { RGB(0xA5, 0x2A, 0x00),    _T("Brown")             },
    { RGB(0x00, 0x40, 0x40),    _T("Dark Olive Green")  },
    { RGB(0x00, 0x55, 0x00),    _T("Dark Green")        },
    { RGB(0x00, 0x00, 0x5E),    _T("Dark Teal")         },
    { RGB(0x00, 0x00, 0x8B),    _T("Dark blue")         },
    { RGB(0x4B, 0x00, 0x82),    _T("Indigo")            },
    { RGB(0x28, 0x28, 0x28),    _T("Dark grey")         },

    { RGB(0x8B, 0x00, 0x00),    _T("Dark red")          },
    { RGB(0xFF, 0x68, 0x20),    _T("Orange")            },
    { RGB(0x8B, 0x8B, 0x00),    _T("Dark yellow")       },
    { RGB(0x00, 0x93, 0x00),    _T("Green")             },
    { RGB(0x38, 0x8E, 0x8E),    _T("Teal")              },
    { RGB(0x00, 0x00, 0xFF),    _T("Blue")              },
    { RGB(0x7B, 0x7B, 0xC0),    _T("Blue-grey")         },
    { RGB(0x66, 0x66, 0x66),    _T("Grey - 40")         },

    { RGB(0xFF, 0x00, 0x00),    _T("Red")               },
    { RGB(0xFF, 0xAD, 0x5B),    _T("Light orange")      },
    { RGB(0x32, 0xCD, 0x32),    _T("Lime")              }, 
    { RGB(0x3C, 0xB3, 0x71),    _T("Sea green")         },
    { RGB(0x7F, 0xFF, 0xD4),    _T("Aqua")              },
    { RGB(0x7D, 0x9E, 0xC0),    _T("Light blue")        },
    { RGB(0x80, 0x00, 0x80),    _T("Violet")            },
    { RGB(0x7F, 0x7F, 0x7F),    _T("Grey - 50")         },

    { RGB(0xFF, 0xC0, 0xCB),    _T("Pink")              },
    { RGB(0xFF, 0xD7, 0x00),    _T("Gold")              },
    { RGB(0xFF, 0xFF, 0x00),    _T("Yellow")            },    
    { RGB(0x00, 0xFF, 0x00),    _T("Bright green")      },
    { RGB(0x40, 0xE0, 0xD0),    _T("Turquoise")         },
    { RGB(0xC0, 0xFF, 0xFF),    _T("Skyblue")           },
    { RGB(0x48, 0x00, 0x48),    _T("Plum")              },
    { RGB(0xC0, 0xC0, 0xC0),    _T("Light grey")        },

    { RGB(0xFF, 0xE4, 0xE1),    _T("Rose")              },
    { RGB(0xD2, 0xB4, 0x8C),    _T("Tan")               },
    { RGB(0xFF, 0xFF, 0xE0),    _T("Light yellow")      },
    { RGB(0x98, 0xFB, 0x98),    _T("Pale green ")       },
    { RGB(0xAF, 0xEE, 0xEE),    _T("Pale turquoise")    },
    { RGB(0x68, 0x83, 0x8B),    _T("Pale blue")         },
    { RGB(0xE6, 0xE6, 0xFA),    _T("Lavender")          },
    { RGB(0xFF, 0xFF, 0xFF),    _T("White")             },
};

CExtPopupColorMenuWnd::CExtPopupColorMenuWnd()
{
	ASSERT( __NCLR_COUNT == sizeof(g_colors)/sizeof(COLORREF_TABLE_ENTRY) );

	m_nColorIdxCurr
		= m_nInitialColorIdx
		= -1;

	m_hWndNotifyColorChanged = NULL; // - use command targed

	m_clrInitial = (COLORREF)(-1); // unexisting
	m_clrDefault = RGB(0,0,0);
	m_bEnableBtnColorDefault = true;
	m_bEnableBtnColorCustom = true;

	m_rcDefColorText.SetRectEmpty();
	m_rcCustColorText.SetRectEmpty();

	m_lParamCockie = 0;

CExtLocalResourceHelper _LRH;
	if( !m_sBtnTextColorDefault.LoadString(IDS_COLOR_DEFAULT) )
	{
		ASSERT( FALSE );
		m_sBtnTextColorDefault = _T("Default Color");
	}
	if( !m_sBtnTextColorCustom.LoadString(IDS_COLOR_CUSTOM) )
	{
		ASSERT( FALSE );
		m_sBtnTextColorCustom = _T("Custom Color ...");
	}
}

BOOL CExtPopupColorMenuWnd::OnQueryNewPalette() 
{
    Invalidate();    
    return CWnd::OnQueryNewPalette();
}

void CExtPopupColorMenuWnd::OnPaletteChanged(CWnd* pFocusWnd) 
{
    CWnd::OnPaletteChanged(pFocusWnd);

    if (pFocusWnd->GetSafeHwnd() != GetSafeHwnd())
        Invalidate();
}

CSize CExtPopupColorMenuWnd::_CalcTrackSize()
{
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nMenuShadowSize =
		_GetMenuShadowSize();
CSize _size(
		(__NCLR_BOX_DX_SIZE + __NCLR_BOX_DX_SPACE*2) * __NCLR_DX
			+ nMenuBorderSize*2 + nMenuShadowSize
			,
		(__NCLR_BOX_DY_SIZE + __NCLR_BOX_DY_SPACE*2) * __NCLR_DY
			+ nMenuBorderSize*2 + nMenuShadowSize
		);
	if( m_bEnableBtnColorDefault )
	{
		_size.cy +=
			__DEF_MENU_SEPARATOR_HEIGHT
			+ __DEF_MENU_GAP*2;
		CString sMeasureText( m_sBtnTextColorDefault );
		sMeasureText.Replace( _T("&"), _T("") );

		CWindowDC dc(NULL);
		CFont * pOldFont =
			dc.SelectObject( &(g_PaintManager->m_FontNormal) );
		ASSERT( pOldFont != NULL );
		CRect rect(0,0,0,0);
		CSize _sizeDefColorText;
		_sizeDefColorText.cy = DrawText(
			dc.GetSafeHdc(),
			(LPCTSTR)sMeasureText,
			sMeasureText.GetLength(),
			&rect,
			DT_CALCRECT|DT_SINGLELINE
				|DT_LEFT|DT_VCENTER
			);
		_sizeDefColorText.cx = rect.Width();
		dc.SelectObject( pOldFont );
		_sizeDefColorText.cx +=
			//__DEF_MENU_ICON_CX
			+ __DEF_MENU_GAP*2 // + __DEF_MENU_GAP*6
			//+ __DEF_MENU_POPUP_ARROW_AREA_DX
			;
		if( _sizeDefColorText.cy < __DEF_MENU_HEIGHT )
			_sizeDefColorText.cy = __DEF_MENU_HEIGHT;
		_size.cy += _sizeDefColorText.cy;
		if( _size.cx < _sizeDefColorText.cx )
			_size.cx = _sizeDefColorText.cx;
		CPoint pt(
			nMenuBorderSize + __DEF_MENU_GAP
				,
			nMenuBorderSize + __DEF_MENU_GAP
			);
		m_rcDefColorText.SetRect(
			pt,
			pt
			+
			CSize(
				max( _size.cx , _sizeDefColorText.cx )
					- __DEF_MENU_GAP*2
					- nMenuBorderSize*2
					- nMenuShadowSize
					,
				_sizeDefColorText.cy
				)
			);
	} // if( m_bEnableBtnColorDefault )
	if( m_bEnableBtnColorCustom )
	{
		_size.cy +=
			__DEF_MENU_SEPARATOR_HEIGHT
			+ __DEF_MENU_GAP*2;
		CString sMeasureText( m_sBtnTextColorCustom );
		sMeasureText.Replace( _T("&"), _T("") );

		CWindowDC dc(NULL);
		CFont * pOldFont =
			dc.SelectObject( &(g_PaintManager->m_FontNormal) );
		ASSERT( pOldFont != NULL );
		CRect rect(0,0,0,0);
		CSize _sizeCustColorText;
		_sizeCustColorText.cy = DrawText(
			dc.GetSafeHdc(),
			(LPCTSTR)sMeasureText,
			sMeasureText.GetLength(),
			&rect,
			DT_CALCRECT|DT_SINGLELINE
				|DT_LEFT|DT_VCENTER
			);
		_sizeCustColorText.cx = rect.Width();
		dc.SelectObject( pOldFont );
		_sizeCustColorText.cx +=
			//__DEF_MENU_ICON_CX
			+ __DEF_MENU_GAP*2 // + __DEF_MENU_GAP*6
			//+ __DEF_MENU_POPUP_ARROW_AREA_DX
			;
		if( _sizeCustColorText.cy < __DEF_MENU_HEIGHT )
			_sizeCustColorText.cy = __DEF_MENU_HEIGHT;
		_size.cy += _sizeCustColorText.cy;
		if( _size.cx < _sizeCustColorText.cx )
			_size.cx = _sizeCustColorText.cx;
		CPoint pt(
			nMenuBorderSize + __DEF_MENU_GAP
				,
			nMenuBorderSize + __DEF_MENU_GAP
			);
		m_rcCustColorText.SetRect(
			pt,
			pt
			+
			CSize(
				max( _size.cx , _sizeCustColorText.cx )
					- __DEF_MENU_GAP*2
					- nMenuBorderSize*2
					- nMenuShadowSize
					,
				_sizeCustColorText.cy
				)
			);
		if( m_bEnableBtnColorDefault )
			m_rcCustColorText.OffsetRect(
				0,
				m_rcCustColorText.Height()
				+ __DEF_MENU_GAP*2
				+ __DEF_MENU_SEPARATOR_HEIGHT
				);
		m_rcCustColorText.OffsetRect(
			0,
			nMenuBorderSize +
				(__NCLR_BOX_DY_SIZE + __NCLR_BOX_DY_SPACE*2) * __NCLR_DY
				+ __NCLR_BOX_DY_SPACE
			);
	} // if( m_bEnableBtnColorCustom )

	return _size;
}

CPoint CExtPopupColorMenuWnd::_GetColorItemCoord(int nIdx)
{
	ASSERT( nIdx >= 0 && nIdx < __NCLR_COUNT );
int nX = nIdx % __NCLR_DX;
	ASSERT( nX < __NCLR_DX );
int nY = nIdx / __NCLR_DX;
	ASSERT( nY < __NCLR_DY );
	return CPoint(nX,nY);
}

CRect CExtPopupColorMenuWnd::_GetColorItemRect(int nIdx)
{
	ASSERT( nIdx >= 0 && nIdx < __NCLR_COUNT );
CPoint ptCoord =  _GetColorItemCoord(nIdx);
int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
CRect rcItem(
		CPoint(
			nMenuBorderSize +
				(__NCLR_BOX_DX_SIZE + __NCLR_BOX_DX_SPACE*2) * ptCoord.x
				+ __NCLR_BOX_DX_SPACE
				,
			nMenuBorderSize +
				(__NCLR_BOX_DY_SIZE + __NCLR_BOX_DY_SPACE*2) * ptCoord.y
				+ __NCLR_BOX_DY_SPACE
			),
		CSize(__NCLR_BOX_DX_SIZE,__NCLR_BOX_DY_SIZE)
		);
	if( m_bEnableBtnColorDefault )
	{
		rcItem.OffsetRect(
			0,
			m_rcDefColorText.Height()
			+ __DEF_MENU_SEPARATOR_HEIGHT
			+ __DEF_MENU_GAP*2
			);
	}
CRect rcClient;
	_GetClientRect( &rcClient );
	rcItem.OffsetRect( rcClient.TopLeft() ) ;
	return rcItem;
}

CRect CExtPopupColorMenuWnd::_CalcTrackRect()
{
	return CExtPopupMenuWnd::_CalcTrackRect();
}

bool CExtPopupColorMenuWnd::_CreateHelper(
	CWnd * pWndCmdReciever
	)
{
	if( !CExtPopupMenuWnd::_CreateHelper(
			pWndCmdReciever
			)
		)
		return false;
	int nIdx =
		_FindCellByColorRef(m_clrInitial);
	if( nIdx >= 0 )
	{
		m_nInitialColorIdx = nIdx;
		Invalidate();
	}
CRect rcClient;
	_GetClientRect( &rcClient );
	if( m_bEnableBtnColorDefault )
		m_rcDefColorText.OffsetRect(
			rcClient.TopLeft()
			);
	if( m_bEnableBtnColorCustom )
		m_rcCustColorText.OffsetRect(
			rcClient.TopLeft()
			);
	return true;
}

void CExtPopupColorMenuWnd::_DoPaint( CDC & dcPaint, bool bUseBackBuffer /*= true*/ )
{
	ASSERT_VALID( (&dcPaint) );
	ASSERT( dcPaint.GetSafeHdc() != NULL );

CRect rcRealClient;
	GetClientRect( &rcRealClient );
CRect rcClient;
	_GetClientRect( &rcClient );

    // Select and realize the palette
CPalette * pOldPalette = NULL;
    if( dcPaint.GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
    {
        pOldPalette =
			dcPaint.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE );
        dcPaint.RealizePalette();
    }

CExtMemoryDC mdc;
	if( bUseBackBuffer )
	{
		mdc.__InitMemoryDC(
			&dcPaint,
			&rcClient, // &rcRealClient
			CExtMemoryDC::MDCOPT_TO_MEMORY
				|CExtMemoryDC::MDCOPT_FILL_SURFACE
				|CExtMemoryDC::MDCOPT_FORCE_DIB 
			);
	}
CDC & dcDummyRef = mdc;
CDC & dc = bUseBackBuffer ? dcDummyRef : dcPaint;

CFont * pOldFont = (CFont *)
		dc.SelectObject(
			&g_PaintManager->m_FontNormal
			);

	dc.FillSolidRect(
		&rcClient,
		g_PaintManager->GetMenuFrameFillColor()
		);

	g_PaintManager->PaintMenuBorder(
		dc,
		&rcClient
		);

	ASSERT( !m_bExpandAvailable );
	ASSERT( !m_bScrollingAvailable );

int nMenuBorderSize =
		g_PaintManager->GetMenuBorderSize();
int nMenuShadowSize =
		_GetMenuShadowSize();
/*
	if( rcClient.left >= rcRealClient.left )
	{
		CRect rcExcludePart(rcRealClient);
		rcExcludePart.right =
			rcClient.left + nMenuBorderSize;
		dc.ExcludeClipRect( rcExcludePart );
	}
	if( rcClient.right <= rcRealClient.right )
	{
		CRect rcExcludePart(rcRealClient);
		rcExcludePart.left =
			rcClient.right - nMenuBorderSize;
		dc.ExcludeClipRect( rcExcludePart );
	}
*/
	if( rcClient.top >= rcRealClient.top )
	{
		CRect rcExcludePart(rcRealClient);
		rcExcludePart.bottom =
			rcClient.top + nMenuBorderSize;
		dc.ExcludeClipRect( rcExcludePart );
	}
	if( rcClient.bottom <= rcRealClient.bottom )
	{
		CRect rcExcludePart(rcRealClient);
		rcExcludePart.top =
			rcClient.bottom - nMenuBorderSize;
		dc.ExcludeClipRect( rcExcludePart );
	}

	// paint default color btn
	if( m_bEnableBtnColorDefault )
	{
		bool bSelected =
			(m_nColorIdxCurr == IDX_DEFAULT_COLOR_BTN) ?
			true : false;
		g_PaintManager->PaintPushButton(
			dc,
			true,
			m_rcDefColorText,
			(LPCTSTR)m_sBtnTextColorDefault,
			NULL,
			true,
			bSelected,
			false, // bSelected,
			false,
			true,
			bSelected,
			false,
			false,
			CExtPaintManager::__ALIGN_HORIZ_CENTER
				|CExtPaintManager::__ALIGN_VERT,
			NULL,
			false,
			0,
			!bSelected
			);
		CRect rcItem( m_rcDefColorText );
		rcItem.OffsetRect(
			0,
			rcItem.Height() + __DEF_MENU_GAP
			);
		rcItem.bottom =
			rcItem.top + __DEF_MENU_SEPARATOR_HEIGHT;
//		g_PaintManager->PaintMenuSeparator(
//			dc,
//			rcItem,
//			false
//			);
		rcItem.top += rcItem.Height()/2;
		rcItem.bottom = rcItem.top+1;
		g_PaintManager->PaintSeparator(
			dc,
			rcItem,
			false
			);
	}
	if( m_bEnableBtnColorCustom )
	{
		bool bSelected =
			(m_nColorIdxCurr == IDX_CUSTOM_COLOR_BTN) ?
			true : false;
		g_PaintManager->PaintPushButton(
			dc,
			true,
			m_rcCustColorText,
			(LPCTSTR)m_sBtnTextColorCustom,
			NULL,
			true,
			bSelected,
			false, // bSelected,
			false,
			true,
			bSelected,
			false,
			false,
			CExtPaintManager::__ALIGN_HORIZ_CENTER
				|CExtPaintManager::__ALIGN_VERT,
			NULL,
			false,
			0,
			!bSelected
			);
		CRect rcItem( m_rcCustColorText );
		rcItem.OffsetRect(
			0,
			- __DEF_MENU_GAP
			);
		rcItem.bottom = rcItem.top;
		rcItem.top -= __DEF_MENU_SEPARATOR_HEIGHT;
//		g_PaintManager->PaintMenuSeparator(
//			dc,
//			rcItem,
//			false
//			);
		rcItem.top += rcItem.Height()/2;
		rcItem.bottom = rcItem.top+1;
		g_PaintManager->PaintSeparator(
			dc,
			rcItem,
			false
			);
	}

	// paint color buttons
COLORREF clrColorBorder =
		g_PaintManager->GetColor(COLOR_3DDKSHADOW);
	for( int nIdx = 0; nIdx < __NCLR_COUNT; nIdx++ )
	{
		bool bSelected =
			(m_nColorIdxCurr == nIdx
			|| m_nInitialColorIdx == nIdx
			) ?
			true : false;
		CRect rcItem = _GetColorItemRect( nIdx );
		g_PaintManager->PaintPushButton(
			dc,
			true,
			rcItem,
			_T(""),
			NULL,
			true,
			bSelected,
			bSelected,
			false,
			true,
			bSelected,
			false,
			false,
			CExtPaintManager::__ALIGN_HORIZ_CENTER
				|CExtPaintManager::__ALIGN_VERT,
			NULL,
			false,
			0,
			!bSelected
			);
		rcItem.DeflateRect(
			__NCLR_BOX_DX_SPACE,
			__NCLR_BOX_DY_SPACE
			);
		dc.FillSolidRect(
			&rcItem,
			//dc.GetNearestColor(
				g_colors[nIdx].m_clr
				//) 
			);
		dc.Draw3dRect(
			&rcItem,
			clrColorBorder,clrColorBorder
			);
	}

CPoint point;
	VERIFY( ::GetCursorPos(&point) );
	ScreenToClient( &point );
	dc.SelectClipRgn( NULL );

	if( m_bCombineWithEA )
	{
		ASSERT( m_eCombineAlign != __CMBA_NONE );
		dc.SelectClipRgn(NULL);
		CRect rcExcludeClient( m_rcExcludeArea );
		ScreenToClient( &rcExcludeClient );
		g_PaintManager->PaintMenuCombinedArea(
			dc,
			rcExcludeClient,
			rcClient,
			m_eCombineAlign
			);
		if( m_pCbPaintCombinedContent != NULL )
			m_pCbPaintCombinedContent(
				m_pCbPaintCombinedCookie,
				dc,
				*this,
				m_rcExcludeArea,
				m_eCombineAlign
				);
	} // if( m_bCombineWithEA )

	dc.SelectObject( pOldFont );

	if( bUseBackBuffer )
	{
		ASSERT( mdc.GetSafeHdc() != NULL );
		mdc.__Flush();
	}

	if( pOldPalette != NULL )
		dcPaint.SelectPalette( pOldPalette, FALSE );

	if( CExtPopupMenuWnd::g_bMenuWithShadows
		&& m_bAnimFinished
		&& nMenuShadowSize > 0
		)
	{ // if we need to paint shadow for client area (and combined exclude area)
		dcPaint.SelectClipRgn(NULL);
		CRect
			rcExcludeClient( 0,0,0,0 ),
			rcExcludeScreen( 0,0,0,0 ),
			rcBaseScreen( 0,0,0,0 );
		if( m_bCombineWithEA )
		{
			if( CExtPopupMenuWnd::g_bMenuWithShadows
				&& m_bAnimFinished
				&& nMenuShadowSize > 0
				)
			{ // if we need to paint shadow for combined exclude area
				rcExcludeScreen = m_rcExcludeArea;
				rcExcludeClient = rcExcludeScreen;
				ScreenToClient( &rcExcludeClient );
				dcPaint.ExcludeClipRect( &rcClient );
				rcBaseScreen = rcClient;
				ClientToScreen( &rcBaseScreen );
				VERIFY(
					m_ShadowCMBA.Paint(
						dcPaint,
						rcExcludeClient,
						rcExcludeScreen,
						rcBaseScreen,
						nMenuShadowSize
						)
					);
				dcPaint.SelectClipRgn(NULL);
			} // if we need to paint shadow for combined exclude area

			CRect rcExcludePart( m_rcExcludeArea );
			ScreenToClient( &rcExcludePart );
			dcPaint.ExcludeClipRect( &rcExcludePart );
		} // if( m_bCombineWithEA )
		VERIFY(
			m_ShadowMain.Paint(
				dcPaint,
				rcClient,
				rcBaseScreen,
				rcExcludeScreen,
				nMenuShadowSize
				)
			);
	} // if we need to paint shadow for client area (and combined exclude area)

}

int CExtPopupColorMenuWnd::_ColorItemHitTest(
	const CPoint & point
	)
{
	for( int nIdx = 0; nIdx < __NCLR_COUNT; nIdx++ )
	{
		CRect rcItem = _GetColorItemRect( nIdx );
		if( rcItem.PtInRect(point) )
			return nIdx;
	}
	if( m_rcDefColorText.PtInRect(point) )
		return IDX_DEFAULT_COLOR_BTN;
	if( m_rcCustColorText.PtInRect(point) )
		return IDX_CUSTOM_COLOR_BTN;
	return -1;
}

bool CExtPopupColorMenuWnd::_OnMouseMove(UINT nFlags, CPoint point)
{
	if( GetSafeHwnd() == NULL )
		return false;

	if( !m_bAnimFinished ) // ?!?! possible this not needed
		return false;

//	ASSERT( GetCapture() == this );

CRect rectWindow;
	GetWindowRect( &rectWindow );
	ScreenToClient( &rectWindow );
	if( !rectWindow.PtInRect(point) )
	{
//		_ItemFocusCancel( TRUE );
		if( m_pWndParentMenu != NULL
			&& m_pWndParentMenu->GetSafeHwnd() != NULL
			)
		{
			ASSERT_VALID( m_pWndParentMenu );
			ClientToScreen( &point );
			m_pWndParentMenu->ScreenToClient( &point );
			if( m_pWndParentMenu->_OnMouseMove(nFlags, point) )
			{
				_OnCancelMode();
				return true;
			}
			_SetCapture();
		}
		return false;
	}

bool bHoverChanged = false;
int nColorIdxCurr = _ColorItemHitTest(point);
	if( m_nColorIdxCurr != nColorIdxCurr )
	{
		m_nColorIdxCurr = nColorIdxCurr;
		bHoverChanged = true;
		Invalidate();
		if( m_nColorIdxCurr != IDX_CUSTOM_COLOR_BTN )
			_NotifyColorChanged();
	}

	if( m_nColorIdxCurr >= 0 )
	{
		_SetCapture();
		if(	g_bMenuShowCoolTips
			&& bHoverChanged
			)
		{
			CString sTipText =
				g_colors[m_nColorIdxCurr].m_sName;
			if( !sTipText.IsEmpty()
				&& g_pWndCapture != NULL
				&& g_pWndCapture == this
				&& m_bAnimFinished
				)
			{
				CRect rcItem =
					_GetColorItemRect(m_nColorIdxCurr);
				ClientToScreen( &rcItem );
				m_wndToolTip.SetText( (LPCTSTR)sTipText );
				VERIFY(
					m_wndToolTip.Show(
						this,
						rcItem
						)
					);
			}
		} // if( nOldCurIndex != nCurIndex )
	}
	else
		_CoolTipHide();

	return true;
}

bool CExtPopupColorMenuWnd::_OnMouseClick(UINT nFlags, CPoint point)
{
int m_nColorIdxCurr = _ColorItemHitTest(point);
	if( m_nColorIdxCurr >= 0
		|| m_nColorIdxCurr == IDX_DEFAULT_COLOR_BTN
		|| m_nColorIdxCurr == IDX_CUSTOM_COLOR_BTN
		)
		_NotifyColorChanged( true );
	_EndSequence();
	return true;
}

bool CExtPopupColorMenuWnd::_OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( GetSafeHwnd() == NULL )
		return true; //false;

	if( !m_bAnimFinished )
		return true; //false;

bool bEat = false;
int nColorIdxCurr = m_nColorIdxCurr;
	switch( nChar )
	{
	case VK_RETURN:
	{
/*
		if( m_pWndParentMenu == NULL )
			return true; // false;
		int nParentCurIndex =
			m_pWndParentMenu->_GetCurIndex();
		ASSERT(
			nParentCurIndex >= 0
			&&
			nParentCurIndex <=
				m_pWndParentMenu->ItemGetCount()
			);
		m_pWndParentMenu->_ItemFocusCancel(
			FALSE
			);
		CExtPopupMenuWnd::_PassMsgLoop();
		m_pWndParentMenu->_ItemFocusSet(
			nParentCurIndex,
			FALSE,
			TRUE
			);
*/
		if( m_nColorIdxCurr >= 0
			|| m_nColorIdxCurr == IDX_DEFAULT_COLOR_BTN
			|| m_nColorIdxCurr == IDX_CUSTOM_COLOR_BTN
			)
			_NotifyColorChanged( true );
		_EndSequence();
		return true;
	} // VK_RETURN
	
	case VK_ESCAPE:
		_EndSequence();
		return true;

	case VK_RIGHT:
		bEat = true;
		if( m_nColorIdxCurr < 0 )
			m_nColorIdxCurr = 0;
		else
		{
			CPoint ptCoord =
				_GetColorItemCoord(m_nColorIdxCurr);
			ptCoord.x++;
			if( ptCoord.x >= __NCLR_DX )
				ptCoord.x = 0;
			m_nColorIdxCurr = 
				ptCoord.x + ptCoord.y * __NCLR_DX;
		}
		break;
	case VK_LEFT:
		bEat = true;
		if( m_nColorIdxCurr < 0 )
			m_nColorIdxCurr = 0;
		else
		{
			CPoint ptCoord =
				_GetColorItemCoord(m_nColorIdxCurr);
			ptCoord.x--;
			if( ptCoord.x < 0 )
				ptCoord.x = __NCLR_DX - 1;
			m_nColorIdxCurr = 
				ptCoord.x + ptCoord.y * __NCLR_DX;
		}
		break;
	case VK_DOWN:
		bEat = true;
		if( m_nColorIdxCurr < 0 )
			m_nColorIdxCurr = 0;
		else
		{
			CPoint ptCoord =
				_GetColorItemCoord(m_nColorIdxCurr);
			ptCoord.y++;
			if( ptCoord.y >= __NCLR_DY )
				ptCoord.y = 0;
			m_nColorIdxCurr = 
				ptCoord.x + ptCoord.y * __NCLR_DX;
		}
		break;
	case VK_UP:
		bEat = true;
		if( m_nColorIdxCurr < 0 )
			m_nColorIdxCurr = 0;
		else
		{
			CPoint ptCoord =
				_GetColorItemCoord(m_nColorIdxCurr);
			ptCoord.y--;
			if( ptCoord.y < 0 )
				ptCoord.y = __NCLR_DY - 1;
			m_nColorIdxCurr = 
				ptCoord.x + ptCoord.y * __NCLR_DX;
		}
		break;

	} // switch( nChar )

	if( bEat )
	{
		_CoolTipHide();
		Invalidate();
		_NotifyColorChanged();
	}

	return bEat;
}

HWND CExtPopupColorMenuWnd::_GetWndNotifyColorChanged()
{
	if( m_hWndNotifyColorChanged != NULL )
	{
		ASSERT( ::IsWindow(m_hWndNotifyColorChanged) );
		return m_hWndNotifyColorChanged;
	}
	ASSERT( ::IsWindow(g_hWndCmdReciever) );
	return g_hWndCmdReciever;
}

void CExtPopupColorMenuWnd::_NotifyColorChanged(
	bool bFinal // = false
	)
{
HWND hWndNotify = _GetWndNotifyColorChanged();
	ASSERT( hWndNotify != NULL );
	ASSERT( ::IsWindow(hWndNotify) );

	if( m_nColorIdxCurr == IDX_CUSTOM_COLOR_BTN )
	{
		::PostMessage(
			hWndNotify,
			nMsgNotifyCustColor,
			(WPARAM)0,
			m_lParamCockie
			);
		return;
	}

COLORREF clr = RGB(0,0,0);
	if( m_nColorIdxCurr == IDX_DEFAULT_COLOR_BTN )
		clr = m_clrDefault;
	else
	{
		if( m_nColorIdxCurr < 0 )
			return;
		clr = g_colors[m_nColorIdxCurr].m_clr;
	}
	::PostMessage(
		hWndNotify,
		bFinal ?
			nMsgNotifyColorChangedFinally
			:
			nMsgNotifyColorChanged
			,
		(WPARAM)clr,
		m_lParamCockie
		);
}

int CExtPopupColorMenuWnd::_FindCellByColorRef(COLORREF clr)
{
	for( int nIdx = 0; nIdx < __NCLR_COUNT; nIdx++ )
	{
		if( g_colors[nIdx].m_clr == clr )
			return nIdx;
	}
	return -1;
}
