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

//#define __DEFAULT_PAINT_MANAGER_CLASS CExtPaintManager
//#define __DEFAULT_PAINT_MANAGER_CLASS CExtPaintManagerXP

#if _MSC_VER > 1200
#include <../Src/mfc/AfxImpl.h>
#else
#include <../Src/AfxImpl.h>
#endif // _MSC_VER > 1200

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC( CExtPaintManager, CObject )
IMPLEMENT_DYNAMIC( CExtPaintManagerXP, CExtPaintManager )

#define CXTEXTMARGIN 1

CExtPaintManager::CExtPaintManagerAutoPtr g_PaintManager;

CExtPaintManager::CExtPaintManagerAutoPtr::
	CExtPaintManagerAutoPtr()
{
	m_pPaintMenager = NULL;
	VERIFY(
		InitPaintManagerInstance()
		);
}

CExtPaintManager::CExtPaintManagerAutoPtr::
	~CExtPaintManagerAutoPtr()
{
	if( m_pPaintMenager )
		delete m_pPaintMenager;
	m_pPaintMenager = NULL;
}

bool CExtPaintManager::CExtPaintManagerAutoPtr::
	InitPaintManagerInstance()
{
	if( m_pPaintMenager != NULL )
		return true;
	return
		InstallPaintManager(
			(CExtPaintManager *)NULL
			);
}

CExtPaintManager *
	CExtPaintManager::CExtPaintManagerAutoPtr::
		operator->()
{
//	InitPaintManagerInstance();
	ASSERT( m_pPaintMenager != NULL );
	return m_pPaintMenager;
}

bool CExtPaintManager::CExtPaintManagerAutoPtr::
	InstallPaintManager(
		CExtPaintManager * pPaintMenager
		)
{
	if( m_pPaintMenager != NULL )
	{
		if( pPaintMenager == m_pPaintMenager )
			return true;
		if( m_pPaintMenager != NULL )
		{
			delete m_pPaintMenager;
			m_pPaintMenager = NULL;
		}
	} // if( m_pPaintMenager != NULL )

	if( pPaintMenager != NULL )
		m_pPaintMenager = pPaintMenager;
	else
	{
		m_pPaintMenager = new __DEFAULT_PAINT_MANAGER_CLASS;
		if(	!m_pPaintMenager->
				IsKindOf(
					RUNTIME_CLASS( CExtPaintManager )
					)
			)
		{
			ASSERT( FALSE );
			return false;
		}
	}

	m_pPaintMenager->SyncSysColors();
	m_pPaintMenager->InitTranslatedColors();
	return true;
}

bool CExtPaintManager::CExtPaintManagerAutoPtr::
	InstallPaintManager(
		CRuntimeClass * pRtcPaintMenager
		)
{
	if( pRtcPaintMenager == NULL )
		return InitPaintManagerInstance();
CObject* pObj = pRtcPaintMenager->CreateObject();
	if( pObj == NULL )
	{
		ASSERT( FALSE );
		return false;
	}
	ASSERT_VALID( pObj );
CExtPaintManager * pPaintManager =
		DYNAMIC_DOWNCAST(CExtPaintManager,pObj);
	if( pPaintManager == NULL )
	{
		delete pObj;
		ASSERT( FALSE );
		return false;
	}
	return InstallPaintManager( pPaintManager );
}

CExtPaintManager::CExtPaintManager()
{
	ASSERT( this != NULL );

	m_DSS_DISABLED_style = DSS_DISABLED;

/**/
#define __3D_COLOR_DEPTH_R 7
#define __3D_COLOR_DEPTH_G 6
#define __3D_COLOR_DEPTH_B 6

struct
{
    LOGPALETTE    LogPalette;
    PALETTEENTRY  PalEntry[ (__3D_COLOR_DEPTH_R*__3D_COLOR_DEPTH_G*__3D_COLOR_DEPTH_B) ];
} pal;
LOGPALETTE * pLogPalette = (LOGPALETTE*) &pal;
	pLogPalette->palVersion    = 0x300;
	pLogPalette->palNumEntries = (WORD)(__3D_COLOR_DEPTH_R*__3D_COLOR_DEPTH_G*__3D_COLOR_DEPTH_B); 
int i = 0;
    for( int nX = 0; nX < __3D_COLOR_DEPTH_R; nX++ )
    {
		for( int nY = 0; nY < __3D_COLOR_DEPTH_G; nY++ )
		{
			for( int nZ = 0; nZ < __3D_COLOR_DEPTH_B; nZ++ )
			{
				pLogPalette->palPalEntry[i].peRed   = (nX*255)/(__3D_COLOR_DEPTH_R-1);
				pLogPalette->palPalEntry[i].peGreen = (nY*255)/(__3D_COLOR_DEPTH_G-1);
				pLogPalette->palPalEntry[i].peBlue  = (nZ*255)/(__3D_COLOR_DEPTH_B-1);
				pLogPalette->palPalEntry[i].peFlags = 0;
				i++;
			}
		}
    }
    VERIFY( m_PaletteWide.CreatePalette(pLogPalette) );
/**/
}

CExtPaintManager::~CExtPaintManager()
{
	ASSERT( this != NULL );

	m_colors.RemoveAll();
	m_brushes.RemoveAll();
}

COLORREF CExtPaintManager::stat_RGBtoCMY( COLORREF clr )
{
int nR = GetRValue( clr );
int nG = GetGValue( clr );
int nB = GetBValue( clr );
int nC = 255-nR;
int nM = 255-nG;
int nY = 255-nB;
COLORREF clrCMY = RGB( nC, nM, nY );
	return clrCMY;
}

COLORREF CExtPaintManager::stat_CMYtoRGB( COLORREF clr )
{
int nC = GetRValue( clr );
int nM = GetGValue( clr );
int nY = GetBValue( clr );
int nR = 255 - nC;
int nG = 255 - nM;
int nB = 255 - nY;
COLORREF clrRGB = RGB( nR, nG, nB );
	return clrRGB;
}

//----------------------------------------------------------------------
// Conversion between the HSL (Hue, Saturation, and Luminosity) 
// and RBG color model.
//----------------------------------------------------------------------
// The conversion algorithms presented here come from the book by 
// Fundamentals of Interactive Computer Graphics by Foley and van Dam. 
// In the example code, HSL values are represented as floating point 
// number in the range 0 to 1. RGB tridrants use the Windows convention 
// of 0 to 255 of each element. 
//----------------------------------------------------------------------
double CExtPaintManager::stat_HuetoRGB(double m1, double m2, double h )
{
   if( h < 0 ) h += 1.0;
   if( h > 1 ) h -= 1.0;
   if( 6.0*h < 1 )
      return (m1+(m2-m1)*h*6.0);
   if( 2.0*h < 1 )
      return m2;
   if( 3.0*h < 2.0 )
      return (m1+(m2-m1)*((2.0/3.0)-h)*6.0);
   return m1;
}

COLORREF CExtPaintManager::stat_HLStoRGB( double H, double L, double S )
{
double r,g,b;
double m1, m2;
	if( S==0 )
	{
		r = g = b = L;
	}
	else
	{
		if( L <= 0.5 )
			m2 = L*(1.0+S);
		else
			m2 = L+S-L*S;
		m1 = 2.0*L-m2;
		r = stat_HuetoRGB(m1,m2,H+1.0/3.0);
		g = stat_HuetoRGB(m1,m2,H);
		b = stat_HuetoRGB(m1,m2,H-1.0/3.0);
	}
	return RGB((BYTE)(r*255),(BYTE)(g*255),(BYTE)(b*255));
}

void CExtPaintManager::stat_RGBtoHSL( COLORREF rgb, double *H, double *S, double *L )
{   
double delta;
double r = (double)GetRValue(rgb)/255;
double g = (double)GetGValue(rgb)/255;
double b = (double)GetBValue(rgb)/255;   
double cmax = max(r,max(g,b));
double cmin = min(r,min(g,b));   
	*L = (cmax + cmin) / 2.0;   
	
	if(cmax==cmin) 
	{
		*S = 0;      
		*H = 0; // it's really undefined   
	}
	else 
	{
		if( *L < 0.5 ) 
			*S = (cmax-cmin)/(cmax+cmin);      
		else
			*S = (cmax-cmin)/(2.0-cmax-cmin);      
		delta = cmax - cmin;
		if( r == cmax ) 
			*H = (g-b)/delta;      
		else if( g == cmax )
			*H = 2.0 +(b-r)/delta;
		else          
			*H = 4.0 + (r-g)/delta;
		*H /= 6.0; 
		if( *H < 0.0 )
			*H += 1;  
	}
}

BYTE CExtPaintManager::stat_GetRGBFromHue(float rm1, float rm2, float rh)
{
	if (rh > 360.0f)
		rh -= 360.0f;
	else if (rh < 0.0f)
		rh += 360.0f;
	if (rh <  60.0f)
		rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;   
	else if (rh < 180.0f)
		rm1 = rm2;
	else if (rh < 240.0f)
		rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;      
	return static_cast<BYTE>(rm1 * 255);
}

COLORREF CExtPaintManager::stat_GetRGBFromHLSExtend( double H, double L, double S )
{
WORD R, G, B;
	if( S == 0.0 )
	{
		R = G = B = unsigned char(L * 255.0);
	}
	else
	{
		float rm1, rm2;
		
		if( L <= 0.5f )
			rm2 = (float)(L + L * S);
		else
			rm2 = (float)(L + S - L * S);
		
		rm1 = (float)(2.0f * L - rm2);
		R = stat_GetRGBFromHue(rm1, rm2, (float)(H + 120.0f));
		G = stat_GetRGBFromHue(rm1, rm2, (float)(H));
		B = stat_GetRGBFromHue(rm1, rm2, (float)(H - 120.0f));
	}
	return RGB(R, G, B);
}

COLORREF CExtPaintManager::stat_HLS_Adjust(
	COLORREF clr,
	double percentH, // = 0.0
	double percentL, // = 0.0
	double percentS  // = 0.0
	)
{
double H = 0.0, L = 0.0, S = 0.0;
	stat_RGBtoHSL( clr, &H, &S, &L );

    if( percentH > 0.0 )
        H =
			H +
			(1.0 - H) * percentH
			;
    else if ( percentH < 0.0 )
        H =
			H * (1.0 + percentH);
	if( H < 0.0 )
		H = 0.0;
	else if( H > 1.0 )
		H = 1.0;

    if( percentL > 0.0 )
        L =
			L +
			(1.0 - L) * percentL
			;
    else if ( percentL < 0.0 )
        L =
			L * (1.0 + percentL);
	if( L < 0.0 )
		L = 0.0;
	else if( L > 1.0 )
		L = 1.0;

    if ( percentS > 0.0 )
        S =
			S +
			(1.0 - S) * percentS
			;
    else if ( percentS < 0.0 )
        S =
			S * (1.0 + percentS);
	if( S < 0.0 )
		S = 0.0;
	else if( S > 1.0 )
		S = 1.0;

    return stat_HLStoRGB( H, L, S );
}

BOOL CExtPaintManager::stat_PaintParentBk(
	HWND hWnd,
	HDC hDC,
	LPCRECT rectClip // = NULL
	)
{
	ASSERT( hDC != NULL );
	ASSERT( ::IsWindow(hWnd) );
	if( rectClip != NULL )
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect( rectClip );
		::SelectClipRgn( hDC, (HRGN)rgn.GetSafeHandle() );
	}
HWND hWndParent = ::GetParent( hWnd );
	ASSERT( ::IsWindow(hWndParent) );
CPoint pt(0,0);
	::MapWindowPoints(hWnd,hWndParent,&pt,1);
	VERIFY(
		::OffsetWindowOrgEx(
			hDC,
			pt.x,
			pt.y,
			&pt
			)
		);
LRESULT lRes =
		::SendMessage(
			hWndParent,
			WM_ERASEBKGND,
			(WPARAM)hDC,
			0
			);
	VERIFY(
		::SetWindowOrgEx(
			hDC,
			pt.x,
			pt.y,
			&pt
			)
		);
	::SelectClipRgn( hDC, NULL );
	return (BOOL)lRes;
}

int CExtPaintManager::stat_GetBPP()
{
//	return 4; // <-- test
//	return 8; // <-- test
CWindowDC dc_desktop(NULL);
int nBitsPerPixel =
		dc_desktop.GetDeviceCaps(BITSPIXEL);
	return nBitsPerPixel;
}

CSize CExtPaintManager::GetTextSizes(bool bBold /*= false*/)
{
CWindowDC dc (NULL);
CFont * pOldFont =
		dc.SelectObject(
			bBold ? &m_FontBold : &m_FontNormal
			);
	ASSERT( pOldFont != NULL );
TEXTMETRIC tm;
	dc.GetTextMetrics( &tm );
int nTextHeightHorz = tm.tmHeight + 2;
	dc.SelectObject( &m_FontNormal );
	dc.GetTextMetrics( &tm );
int nTextHeightVert = tm.tmHeight + 2;
	dc.SelectObject( pOldFont );
	return CSize(nTextHeightHorz,nTextHeightVert);
}

int CExtPaintManager::GetTextHeight(bool bHorz)
{
CSize _size = GetTextSizes();
	return bHorz ? _size.cx : _size.cy;
}

static const UINT stat_pixels_glyph_btn_expand_bottom[] =
{
	1,1,1,1,1,
	0,1,1,1,0,
	0,0,1,0,0,
};
const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_bottom(
		5,3,2,
		stat_pixels_glyph_btn_expand_bottom
		);

static const UINT stat_pixels_glyph_btn_expand_right2[] =
{
	1,1,0,0,1,1,0,0,
	0,1,1,0,0,1,1,0,
	0,0,1,1,0,0,1,1,
	0,1,1,0,0,1,1,0,
	1,1,0,0,1,1,0,0,
};
const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_right2(
		8,5,2,
		stat_pixels_glyph_btn_expand_right2
		);

const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_left(
		CExtPaintManager::g_glyph_btn_expand_bottom,
		90
		);

const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_right(
		CExtPaintManager::g_glyph_btn_expand_bottom,
		270
		);

const CExtPaintManager::glyph_t
	CExtPaintManager::g_glyph_btn_expand_bottom2(
		CExtPaintManager::g_glyph_btn_expand_right2,
		90
		);

void CExtPaintManager::PaintGlyph(
	CDC & dc,
	POINT ptDest,
	const glyph_t & glyph,
	COLORREF * pColorValues
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( pColorValues != NULL );
	ASSERT( glyph.Size().cx > 0 );
	ASSERT( glyph.Size().cy > 0 );
CRect rcTestVisiblity(ptDest,glyph.Size());
	if( !dc.RectVisible(&rcTestVisiblity) )
		return;
int x_dest = ptDest.x + glyph.Size().cx;
int y_dest = ptDest.y + glyph.Size().cy;
const UINT * pColorIndex = glyph.GetSurface();
	ASSERT( pColorIndex != NULL );
	for( int y = ptDest.y; y < y_dest; y++ )
	{
		for( int x = ptDest.x; x < x_dest; x++, pColorIndex++ )
		{
			UINT nColorIndex = *pColorIndex;
			ASSERT( nColorIndex < glyph.GetColorsCount() );
			if( nColorIndex == 0 )
				continue;
			COLORREF clr = pColorValues[nColorIndex];
			dc.SetPixel( x, y, clr );
		}
	}
}

void CExtPaintManager::PaintGlyphCentered(
	CDC & dc,
	const RECT & rect,
	const glyph_t & glyph,
	COLORREF * pColorValues
	)
{
CRect _rect(rect);
	_rect.NormalizeRect();
	if( _rect.IsRectEmpty() )
		return;
	ASSERT( glyph.Size().cx > 0 );
	ASSERT( glyph.Size().cy > 0 );
CPoint ptDest = _rect.TopLeft();
	ptDest.x +=
		(_rect.Width() - glyph.Size().cx) / 2;
	ptDest.y +=
		(_rect.Height() - glyph.Size().cy) / 2;
	PaintGlyph(
		dc,
		ptDest,
		glyph,
		pColorValues
		);
}

void CExtPaintManager::PaintMenuExpandButton(
	CDC & dc,
	const CRect & rectButton,
	bool bPushed,
	bool bHover
	)
{
CRect rcGlyph(rectButton);
	if( bPushed )
	{
		dc.FillRect(
			&rcGlyph,
			&m_brushLight
			);
		dc.Draw3dRect(
			&rcGlyph,
			GetColor(CLR_3DHILIGHT_OUT),
			GetColor(CLR_3DSHADOW_OUT)
			);
	}
	else
	{
		PaintMenuItem(
			dc, rectButton, _T(""),
			_T(""), NULL,
			false, false, false,
			false, false, true, false, false, false,
			0
			);
	}
COLORREF ColorValues[] =
{
	RGB(0,0,0),
	GetColor(CLR_TEXT_OUT)
};
	PaintGlyphCentered(
		dc,
		rcGlyph,
		g_glyph_btn_expand_bottom2,
		ColorValues
		);
}

void CExtPaintManagerXP::PaintMenuExpandButton(
	CDC & dc,
	const CRect & rectButton,
	bool bPushed,
	bool bHover
	)
{
	if( !bPushed )
	{
		CRect rc(rectButton);
		rc.top--;
		PaintMenuItem(
			dc, rc, _T(""),
			_T(""), NULL,
			false, false, false,
			false, false, true, false, false, false,
			0
			);
	}
CRect rcGlyph(rectButton);
	if( bPushed )
	{
		PaintPushButton(
			dc,true,rectButton,_T(""),NULL,
			true,false/*bHover*/,bPushed,false,
			false,true,false,false,
			__ALIGN_HORIZ_CENTER|__ALIGN_VERT,
			NULL,
			false
			);
		rcGlyph.OffsetRect(
			GetPushedOffset()
			);
	}
	else
	{
		PaintMenuItem(
			dc, rectButton, _T(""),
			_T(""), NULL,
			false, false, false,
			false, false, true, false, false, false,
			0
			);
	}
COLORREF ColorValues[] =
{
	RGB(0,0,0),
	GetColor(CLR_TEXT_OUT)
};
	PaintGlyphCentered(
		dc,
		rcGlyph,
		g_glyph_btn_expand_bottom2,
		ColorValues
		);
}

void CExtPaintManager::PaintToolbarExpandButton(
	CDC & dc,
	const CRect & rectButton,
	bool bHorz, // if false - down
	bool bBarIsCompletelyVisible,
	bool bEnabled,
	bool bPushed,
	bool bHover,
	bool bTransparentBackground // = false
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
CRect rect(rectButton);
	rect.NormalizeRect();
	if( !dc.RectVisible( &rect ) )
		return;

//	ASSERT( bLeft ); // temporary
const glyph_t * pGlyph = NULL, * pGlyph2 = NULL;
	if( bHorz )
	{
		pGlyph = &g_glyph_btn_expand_bottom;
		if( !bBarIsCompletelyVisible )
			pGlyph2 = &g_glyph_btn_expand_right2;
	}
	else
	{
		pGlyph = &g_glyph_btn_expand_left;
		if( !bBarIsCompletelyVisible )
			pGlyph2 = &g_glyph_btn_expand_bottom2;
	}
	ASSERT( pGlyph != NULL );

	PaintPushButton(
		dc,true,rect,_T(""),NULL,true,
		bHover,bPushed,false,bEnabled,
		true,false,false,
		__ALIGN_HORIZ_CENTER|__ALIGN_VERT,
		NULL,false,0,bTransparentBackground
		);
COLORREF clr =
	GetColor(
		bEnabled? CLR_TEXT_OUT : COLOR_3DHILIGHT
		);
COLORREF ColorValues[] =
{
	0,
	clr
};
	if( bPushed )
		rect.OffsetRect(
			GetPushedOffset()
			);

CRect rectGlyph(rectButton.TopLeft(),pGlyph->Size());
CRect rectGlyph2(rectGlyph);
CSize sizePushedOffset = GetPushedOffset();
	if( bPushed )
	{
		rectGlyph.OffsetRect( sizePushedOffset );
		rectGlyph2.OffsetRect( sizePushedOffset );
	}

	if( bHorz )
	{
		int nGap = (max(sizePushedOffset.cy,1)) * 3;
		rectGlyph.OffsetRect(
			(rectButton.Size().cx - pGlyph->Size().cx) / 2,
			rectButton.Size().cy - pGlyph->Size().cy - nGap
			);
		if( !bBarIsCompletelyVisible )
		{
			ASSERT( pGlyph2 != NULL );
			rectGlyph2.OffsetRect(
				(rectButton.Size().cx - pGlyph2->Size().cx) / 2,
				nGap
				);
		}
	}
	else
	{
		int nGap = (max(sizePushedOffset.cx,1)) * 3;
		rectGlyph.OffsetRect(
			nGap,
			(rectButton.Size().cy - pGlyph->Size().cy)/2
			);
		if( !bBarIsCompletelyVisible )
		{
			ASSERT( pGlyph2 != NULL );
			rectGlyph2.OffsetRect(
				rectButton.Size().cx - pGlyph2->Size().cx - nGap,
				(rectButton.Size().cy - pGlyph2->Size().cy) / 2
				);
		}
	}

	if( bEnabled )
	{
		PaintGlyph(
			dc,rectGlyph.TopLeft(),*pGlyph,ColorValues
			);
		if( !bBarIsCompletelyVisible )
		{
			ASSERT( pGlyph2 != NULL );
			PaintGlyph(
				dc,rectGlyph2.TopLeft(),*pGlyph2,ColorValues
				);
		}
	} // if( bEnabled )
	else
	{
		rect.OffsetRect(1,1);
		PaintGlyphCentered(
			dc,rect,*pGlyph,ColorValues
			);
		ColorValues[1] = GetColor(COLOR_3DSHADOW);
		rect.OffsetRect(-1,-1);
		PaintGlyphCentered(
			dc,rect,*pGlyph,ColorValues
			);
	} // else from if( bEnabled )
}

void CExtPaintManagerXP::PaintToolbarExpandButton(
	CDC & dc,
	const CRect & rectButton,
	bool bHorz, // if false - down
	bool bBarIsCompletelyVisible,
	bool bEnabled,
	bool bPushed,
	bool bHover,
	bool bTransparentBackground // = false
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
CRect rect(rectButton);
	rect.NormalizeRect();
	if( !dc.RectVisible( &rect ) )
		return;

//	ASSERT( bLeft ); // temporary
const glyph_t * pGlyph = NULL, * pGlyph2 = NULL;
	if( bHorz )
	{
		pGlyph = &g_glyph_btn_expand_bottom;
		if( !bBarIsCompletelyVisible )
			pGlyph2 = &g_glyph_btn_expand_right2;
	}
	else
	{
		pGlyph = &g_glyph_btn_expand_left;
		if( !bBarIsCompletelyVisible )
			pGlyph2 = &g_glyph_btn_expand_bottom2;
	}
	ASSERT( pGlyph != NULL );

	PaintPushButton(
		dc,true,rect,_T(""),NULL,true,
		bHover,bPushed,false,bEnabled,
		true,false,false,
		__ALIGN_HORIZ_CENTER|__ALIGN_VERT,
		NULL,false,0,bTransparentBackground
		);
COLORREF clr =
	GetColor(
		bEnabled? CLR_TEXT_OUT : COLOR_3DHILIGHT
		);
COLORREF ColorValues[] =
{
	0,
	clr
};
	if( bPushed )
		rect.OffsetRect(
			GetPushedOffset()
			);

CRect rectGlyph(rectButton.TopLeft(),pGlyph->Size());
CRect rectGlyph2(rectGlyph);
CSize sizePushedOffset = GetPushedOffset();
	if( bPushed )
	{
		rectGlyph.OffsetRect( sizePushedOffset );
		rectGlyph2.OffsetRect( sizePushedOffset );
	}

	if( bHorz )
	{
		int nGap = (max(sizePushedOffset.cy,1)) * 3;
		rectGlyph.OffsetRect(
			(rectButton.Size().cx - pGlyph->Size().cx) / 2 + 1,
			rectButton.Size().cy - pGlyph->Size().cy - nGap
			);
		if( !bBarIsCompletelyVisible )
		{
			ASSERT( pGlyph2 != NULL );
			rectGlyph2.OffsetRect(
				(rectButton.Size().cx - pGlyph2->Size().cx) / 2 + 1,
				nGap
				);
		}
	}
	else
	{
		int nGap = (max(sizePushedOffset.cx,1)) * 3;
		rectGlyph.OffsetRect(
			nGap,
			(rectButton.Size().cy - pGlyph->Size().cy)/2 + 1
			);
		if( !bBarIsCompletelyVisible )
		{
			ASSERT( pGlyph2 != NULL );
			rectGlyph2.OffsetRect(
				rectButton.Size().cx - pGlyph2->Size().cx - nGap,
				(rectButton.Size().cy - pGlyph2->Size().cy) / 2 + 1
				);
		}
	}

	if( bEnabled )
	{
		if( bHover && (!bPushed) )
		{
			COLORREF ColorValuesHover[] =
			{
				0,
				GetColor(COLOR_3DDKSHADOW)
			};
			rectGlyph.OffsetRect(-1,-1);
			rectGlyph2.OffsetRect(-1,-1);
			PaintGlyph(
				dc,rectGlyph.TopLeft(),*pGlyph,ColorValuesHover
				);
			if( !bBarIsCompletelyVisible )
			{
				ASSERT( pGlyph2 != NULL );
				PaintGlyph(
					dc,rectGlyph2.TopLeft(),*pGlyph2,ColorValuesHover
					);
			}
			rectGlyph.OffsetRect(1,1);
			rectGlyph2.OffsetRect(1,1);
		}
		PaintGlyph(
			dc,rectGlyph.TopLeft(),*pGlyph,ColorValues
			);
		if( !bBarIsCompletelyVisible )
		{
			ASSERT( pGlyph2 != NULL );
			PaintGlyph(
				dc,rectGlyph2.TopLeft(),*pGlyph2,ColorValues
				);
		}
	} // if( bEnabled )
	else
	{
		rect.OffsetRect(1,1);
		PaintGlyphCentered(
			dc,rect,*pGlyph,ColorValues
			);
		ColorValues[1] = GetColor(COLOR_3DSHADOW);
		rect.OffsetRect(-1,-1);
		PaintGlyphCentered(
			dc,rect,*pGlyph,ColorValues
			);
	} // else from if( bEnabled )
}

void CExtPaintManager::stat_ExcludeChildAreas(
	HDC hDC,
	HWND hWnd
	)
{
	if( hDC == NULL || !IsWindow(hWnd) )
		return;
HWND hWndChild = GetWindow(hWnd,GW_CHILD);
	for(; hWndChild != NULL; hWndChild = GetWindow(hWndChild,GW_HWNDNEXT))
	{
		if( !IsWindowVisible(hWndChild) )
			continue;
		RECT rc;
		if( !GetWindowRect(hWndChild,&rc) )
			continue;
		if( !ScreenToClient(hWnd, (LPPOINT)(&rc)) )
			continue;
		if( !ScreenToClient(hWnd, ((LPPOINT)(&rc))+1) )
			continue;
		ExcludeClipRect(
			hDC,
			rc.left,rc.top,
			rc.right,rc.bottom
			);
	} // for(; hWndChild != NULL; hWndChild = GetWindow(hWndChild,GW_HWNDNEXT))
}

//static volatile bool stat_bTrySysTransparentBlt = true;
//static volatile HMODULE stat_hModuleMsimg32 = NULL;
//typedef BOOL (*pMsimg32_TransparentBlt_t)(
//		HDC hdcDest,        // handle to destination DC
//		int nXOriginDest,   // x-coord of destination upper-left corner
//		int nYOriginDest,   // y-coord of destination upper-left corner
//		int nWidthDest,     // width of destination rectangle
//		int hHeightDest,    // height of destination rectangle
//		HDC hdcSrc,         // handle to source DC
//		int nXOriginSrc,    // x-coord of source upper-left corner
//		int nYOriginSrc,    // y-coord of source upper-left corner
//		int nWidthSrc,      // width of source rectangle
//		int nHeightSrc,     // height of source rectangle
//		UINT crTransparent  // color to make transparent
//	);
//static volatile pMsimg32_TransparentBlt_t pMsimg32_TransparentBlt = NULL;

void CExtPaintManager::stat_TransparentBlt(
	HDC hdcDest,        // handle to destination DC
	int nXOriginDest,   // x-coord of destination upper-left corner
	int nYOriginDest,   // y-coord of destination upper-left corner
	int nWidthDest,     // width of destination rectangle
	int hHeightDest,    // height of destination rectangle
	HDC hdcSrc,         // handle to source DC
	int nXOriginSrc,    // x-coord of source upper-left corner
	int nYOriginSrc,    // y-coord of source upper-left corner
	int nWidthSrc,      // width of source rectangle
	int nHeightSrc,     // height of source rectangle
	UINT crTransparent  // color to make transparent
	)
{
	ASSERT( hdcDest != NULL && hdcSrc != NULL );
//	if( stat_bTrySysTransparentBlt )
//	{
//		stat_bTrySysTransparentBlt = false;
//		static CCriticalSection statCsQueryMsimg32;
//		statCsQueryMsimg32.Lock();
//		stat_hModuleMsimg32 = ::GetModuleHandle( _T("Msimg32.dll") );
//		if( stat_hModuleMsimg32 != NULL )
//		{
//			ASSERT( pMsimg32_TransparentBlt == NULL );
//			pMsimg32_TransparentBlt = (pMsimg32_TransparentBlt_t)
//				::GetProcAddress( stat_hModuleMsimg32, _T("TransparentBlt") );
//		}
//		statCsQueryMsimg32.Unlock();
//	}
//	if( pMsimg32_TransparentBlt != NULL )
//	{
//		VERIFY(
//			pMsimg32_TransparentBlt(
//				hdcDest,        // handle to destination DC
//				nXOriginDest,   // x-coord of destination upper-left corner
//				nYOriginDest,   // y-coord of destination upper-left corner
//				nWidthDest,     // width of destination rectangle
//				hHeightDest,    // height of destination rectangle
//				hdcSrc,         // handle to source DC
//				nXOriginSrc,    // x-coord of source upper-left corner
//				nYOriginSrc,    // y-coord of source upper-left corner
//				nWidthSrc,      // width of source rectangle
//				nHeightSrc,     // height of source rectangle
//				crTransparent  // color to make transparent
//				)
//			);
//		return;
//	}

CDC dcdst, dcsrc;
	dcdst.Attach( hdcDest );
	dcsrc.Attach( hdcSrc );
CExtMemoryDC dcmm( &dcdst ), dcmsk( &dcdst, NULL, CExtMemoryDC::MDCOPT_TO_MEMORY|CExtMemoryDC::MDCOPT_FORCE_BLACK );
	if(	( nWidthDest != nWidthSrc || hHeightDest != nHeightSrc ) && nWidthDest >= 0 && hHeightDest >= 0 )
			dcmm.StretchBlt( 0, 0, nWidthDest, hHeightDest, &dcsrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY );
	else	dcmm.BitBlt( 0, 0, nWidthSrc, nHeightSrc, &dcsrc, nXOriginSrc, nYOriginSrc, SRCCOPY );
int nRealWidthDest  = (nWidthDest < 0) ? nWidthSrc : nWidthDest;
int nRealHeightDest = (hHeightDest < 0) ? nHeightSrc : hHeightDest;
	dcmm.SetBkColor( crTransparent );
	dcmsk.BitBlt( 0, 0, nRealWidthDest, nRealHeightDest, &dcmm, 0, 0, SRCCOPY );
	dcmm.SetBkColor( RGB(0,0,0) );
	dcmm.SetTextColor( RGB(255,255,255) );
	dcmm.BitBlt( 0, 0, nRealWidthDest, nRealHeightDest, &dcmsk, 0, 0, SRCAND );
	dcdst.SetBkColor( RGB(255,255,255) );
	dcdst.SetTextColor( RGB(0,0,0) );
	dcdst.BitBlt( nXOriginDest, nYOriginDest, nRealWidthDest, nRealHeightDest, &dcmsk, 0, 0, SRCAND );
	dcdst.BitBlt( nXOriginDest, nYOriginDest, nRealWidthDest, nRealHeightDest, &dcmm, 0, 0, SRCPAINT );
	dcmm.__Flush( FALSE );
	dcmsk.__Flush( FALSE );
	dcdst.Detach();
	dcsrc.Detach();
}

// generate DIB for UI light brush (when BPP <=8)
HBITMAP CExtPaintManager::stat_GenLBDIB( HDC hDC )
{
BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = 16;
	bih.biHeight = 16;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = bih.biWidth * bih.biHeight;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;
COLORREF * pClrs = NULL;
HBITMAP hDIB =
		::CreateDIBSection(
			hDC,
			(LPBITMAPINFO)&bih,
			DIB_RGB_COLORS,
			(void **)&pClrs,
			NULL,
			NULL
			);
COLORREF clr0 = ::GetSysColor( COLOR_BTNFACE );
COLORREF clr1 = ::GetSysColor( COLOR_BTNHIGHLIGHT );
	if( hDIB == NULL || pClrs == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}
	for( int nY = 0; nY < 16; nY++ )
	{
		for( int nX = 0; nX < 16; nX++, pClrs++ )
		{
			COLORREF clr = (nY&0x01)
				? ( (nX&0x01) ? clr0 : clr1 )
				: ( (nX&0x01) ? clr1 : clr0 )
				;
			*pClrs = clr;
		}
	}
	return hDIB;
}

void CExtPaintManager::stat_PaintGradientRect(
	CDC & dc,
	const CRect & rcPaintGradient,
	COLORREF clrLeft,
	COLORREF clrRight,
	bool bHorz, // = false
	UINT nCountOfSteps // = 256
	)
{
	ASSERT( nCountOfSteps > 0 && nCountOfSteps <= 256 );
UINT nWidth = rcPaintGradient.Width();
UINT nHeight = rcPaintGradient.Height();
	if( bHorz )
	{
		if( nCountOfSteps > nHeight )
			nCountOfSteps = nHeight;
	}
	else
	{
		if( nCountOfSteps > nWidth )
			nCountOfSteps = nWidth;
	}
CRect rcItem( rcPaintGradient );
	for(	UINT nStep = 1, nBackStep = nCountOfSteps-1;
			nStep < nCountOfSteps;
			nStep++, nBackStep--
			)
	{
		COLORREF clrItem =
			RGB(
				(	GetRValue(clrLeft)*nBackStep
					+ GetRValue(clrRight)*nStep
					) / nCountOfSteps,
				(	GetGValue(clrLeft)*nBackStep
					+ GetGValue(clrRight)*nStep
					) / nCountOfSteps,
				(	GetBValue(clrLeft)*nBackStep
					+ GetBValue(clrRight)*nStep
					) / nCountOfSteps
				);
		if( bHorz )
		{
			rcItem.top =
				rcPaintGradient.bottom
				- nStep * nHeight / nCountOfSteps;
			rcItem.top--;
			ASSERT( rcItem.Height() >= 1 );
			dc.FillSolidRect( rcItem, clrItem );
			rcItem.bottom = rcItem.top;
		}
		else
		{
			rcItem.right =
				rcPaintGradient.left
				+ nStep * nWidth / nCountOfSteps;
			rcItem.right++;
			ASSERT( rcItem.Width() >= 1 );
			dc.FillSolidRect( rcItem, clrItem );
			rcItem.left = rcItem.right;
		}
	}
}

void CExtPaintManager::SyncSysColors()
{
	ASSERT( this != NULL );

	// init fonts
HFONT hDefaultGuiFont =
		(HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	if( hDefaultGuiFont == NULL )
		hDefaultGuiFont =
			(HFONT)::GetStockObject(SYSTEM_FONT);
	ASSERT( hDefaultGuiFont != NULL );
	if( m_FontNormal.GetSafeHandle() )
		m_FontNormal.Detach();
	m_FontNormal.Attach( hDefaultGuiFont );

LOGFONT lf;
	VERIFY( m_FontNormal.GetLogFont(&lf) );
	lf.lfWeight = 900;
	if( m_FontBold.GetSafeHandle() )
		m_FontBold.DeleteObject();
	VERIFY( m_FontBold.CreateFontIndirect(&lf) );

	VERIFY( m_FontNormal.GetLogFont(&lf) );
	lf.lfCharSet = SYMBOL_CHARSET;
	lf.lfWeight = 0;
	lf.lfHeight = ::GetSystemMetrics(SM_CYMENUCHECK) - 1;
	_tcscpy( lf.lfFaceName, _T("Marlett") );
	if( m_FontMarlett.GetSafeHandle() )
		m_FontMarlett.DeleteObject();
	VERIFY( m_FontMarlett.CreateFontIndirect(&lf) );

	VERIFY( m_FontNormal.GetLogFont(&lf) );
	lf.lfEscapement = __VERT_FONT_ESCAPEMENT__;
	if( m_FontNormalVert.GetSafeHandle() )
		m_FontNormalVert.DeleteObject();
	VERIFY( m_FontNormalVert.CreateFontIndirect(&lf) );

	lf.lfEscapement = -(__VERT_FONT_ESCAPEMENT__);
	if( m_FontNormalVertX.GetSafeHandle() )
		m_FontNormalVertX.DeleteObject();
	VERIFY( m_FontNormalVertX.CreateFontIndirect(&lf) );

	VERIFY( m_FontBold.GetLogFont(&lf) );
	lf.lfEscapement = __VERT_FONT_ESCAPEMENT__;
	if( m_FontBoldVert.GetSafeHandle() )
		m_FontBoldVert.DeleteObject();
	VERIFY( m_FontBoldVert.CreateFontIndirect(&lf) );

	lf.lfEscapement = -(__VERT_FONT_ESCAPEMENT__);
	if( m_FontBoldVertX.GetSafeHandle() )
		m_FontBoldVertX.DeleteObject();
	VERIFY( m_FontBoldVertX.CreateFontIndirect(&lf) );

	/// init colors
int i;

COLORREF clrDefaultExtendColor =
		::GetSysColor(COLOR_3DFACE);
	for(	i = m_colors.GetSize();
			i <= __ExtMfc_MAX_SYS_COLOR_VAL;
			i++
			)
	{
		m_colors.Add( clrDefaultExtendColor );
	}

HBRUSH hDefaultExtendBrush =
		::GetSysColorBrush(COLOR_3DFACE);
	for(	i = m_brushes.GetSize();
			i <= __ExtMfc_MAX_SYS_COLOR_VAL;
			i++
			)
	{
		m_brushes.Add( hDefaultExtendBrush );
	}

	//m_colors.clear();
	//m_brushes.clear();
	for(	i = __ExtMfc_MIN_SYS_COLOR_VAL;
			i <= __ExtMfc_MAX_SYS_COLOR_VAL;
			i++
			)
	{
		m_colors[i] = ::GetSysColor(i);
		m_brushes[i] = ::GetSysColorBrush(i);
	}
	ASSERT( m_colors.GetSize() >= __ExtMfc_MAX_SYS_COLOR_VAL+1 );
	ASSERT( m_brushes.GetSize() >= __ExtMfc_MAX_SYS_COLOR_VAL+1 );

	if( m_brushLight.GetSafeHandle() != NULL )
		m_brushLight.DeleteObject();
int nBitsPerPixel = stat_GetBPP();
	if(nBitsPerPixel > 8)
	{
		COLORREF clrBtnFace = GetColor(COLOR_3DFACE);
		COLORREF clrBtnHilite = GetColor(COLOR_3DHILIGHT);
		COLORREF clrLight = RGB (
			GetRValue(clrBtnFace) + ((GetRValue(clrBtnHilite) -
				GetRValue(clrBtnFace)) / 2 ),
			GetGValue(clrBtnFace) + ((GetGValue(clrBtnHilite) -
				GetGValue(clrBtnFace)) / 2),
			GetBValue(clrBtnFace) + ((GetBValue(clrBtnHilite) -
				GetBValue(clrBtnFace)) / 2)
			);
		m_brushLight.CreateSolidBrush( clrLight );
	}
	else
	{
		CWindowDC dc(NULL);
		HBITMAP hbmGray = stat_GenLBDIB( dc.GetSafeHdc() );
		ASSERT( hbmGray != NULL) ;
		CBitmap bmp;
		bmp.Attach( hbmGray );
		m_brushLight.CreatePatternBrush( &bmp );
	}
}

// get any system color based brush
HBRUSH CExtPaintManager::GetBrush(
	int nColorIndex
	)
{
int nBrushesCount = m_brushes.GetSize();
	if(
			!(
				__ExtMfc_MIN_SYS_COLOR_VAL <= nColorIndex
				&&
				nColorIndex < nBrushesCount
			)
		)
	{
		ASSERT( FALSE );
		nColorIndex = COLOR_3DFACE;
	}
HBRUSH hBrush = m_brushes[nColorIndex];
	return hBrush;
}

COLORREF CExtPaintManager::GetColor(int nColorIndex)
{
	ASSERT( this != NULL );

	if( nColorIndex >= __ExtMfc_COLOR_MAP_BASE )
	{
		int nColorIndex2 = 0;
		if( m_mapColorTranslate.Lookup(
				nColorIndex,
				nColorIndex2
				)
			)
			nColorIndex = nColorIndex2;
		else
		{
			ASSERT( FALSE );
			nColorIndex = COLOR_3DFACE;
		}
	}
int nColorCount = m_colors.GetSize();
	if(
			!(
				__ExtMfc_MIN_SYS_COLOR_VAL <= nColorIndex
				&&
				nColorIndex < nColorCount // <= __ExtMfc_MAX_SYS_COLOR_VAL
			)
		)
	{
		ASSERT( FALSE );
		nColorIndex = COLOR_3DFACE;
	}
COLORREF clr = m_colors[nColorIndex];
	return clr;
}

int CExtPaintManager::InstallColor(
	COLORREF clr,
	int nColorIndex // = -1
	)
{
	ASSERT( this != NULL );

int nColorCount = m_colors.GetSize();
	if( nColorIndex < 0 || nColorIndex >= nColorCount )
	{
		nColorIndex = m_colors.Add( clr );
	}
	else
		m_colors[nColorIndex] = clr;
	return nColorIndex;
}


void CExtPaintManager::InitTranslatedColors()
{
	ASSERT( this != NULL );

	m_mapColorTranslate.RemoveAll();

	m_mapColorTranslate[CLR_3DFACE_OUT]				= COLOR_3DFACE;
	m_mapColorTranslate[CLR_3DFACE_IN]				= COLOR_3DFACE;
	m_mapColorTranslate[CLR_3DFACE_DISABLED]		= COLOR_3DFACE;

	m_mapColorTranslate[CLR_3DLIGHT_OUT]			= COLOR_3DLIGHT;
	m_mapColorTranslate[CLR_3DLIGHT_IN]				= COLOR_3DLIGHT;
	m_mapColorTranslate[CLR_3DLIGHT_DISABLED]		= COLOR_3DLIGHT;

	m_mapColorTranslate[CLR_3DHILIGHT_OUT]			= COLOR_3DHILIGHT;
	m_mapColorTranslate[CLR_3DHILIGHT_IN]			= COLOR_3DHILIGHT;
	m_mapColorTranslate[CLR_3DHILIGHT_DISABLED]		= COLOR_3DHILIGHT;

	m_mapColorTranslate[CLR_3DSHADOW_OUT]			= COLOR_3DSHADOW;
	m_mapColorTranslate[CLR_3DSHADOW_IN]			= COLOR_3DSHADOW;
	m_mapColorTranslate[CLR_3DSHADOW_DISABLED]		= COLOR_3DSHADOW;

	m_mapColorTranslate[CLR_3DDKSHADOW_OUT]			= COLOR_3DDKSHADOW;
	m_mapColorTranslate[CLR_3DDKSHADOW_IN]			= COLOR_3DDKSHADOW;
	m_mapColorTranslate[CLR_3DDKSHADOW_DISABLED]	= COLOR_3DDKSHADOW;
	
	m_mapColorTranslate[CLR_TEXT_OUT]				= COLOR_BTNTEXT;
	m_mapColorTranslate[CLR_TEXT_IN]				= COLOR_BTNTEXT;
	m_mapColorTranslate[CLR_TEXT_DISABLED]			= COLOR_3DSHADOW;
}

static const UINT stat_pixels_dock_btn_empty[] =
{
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_btn_close[] =
{
	0,0,0,0,0,0,0,0,0,
	0,1,1,0,0,0,0,1,1,
	0,0,1,1,0,0,1,1,0,
	0,0,0,1,1,1,1,0,0,
	0,0,0,0,1,1,0,0,0,
	0,0,0,1,1,1,1,0,0,
	0,0,1,1,0,0,1,1,0,
	0,1,1,0,0,0,0,1,1,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_btn_arrow_up[] =
{
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,1,1,1,0,0,0,
	0,0,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_wnd_minimize[] =
{
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_wnd_restore[] =
{
	0,0,1,1,1,1,1,1,0,
	0,0,1,0,0,0,0,1,0,
	1,1,1,1,1,1,0,1,0,
	1,1,1,1,1,1,0,1,0,
	1,0,0,0,0,1,1,1,0,
	1,0,0,0,0,1,0,0,0,
	1,0,0,0,0,1,0,0,0,
	1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_wnd_maximize[] =
{
	1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,
};

static const UINT stat_pixels_dock_wnd_keep[] =
{
	0,0,1,1,1,1,1,0,0,
	0,0,1,0,0,1,1,0,0,
	0,0,1,0,0,1,1,0,0,
	0,0,1,0,0,1,1,0,0,
	0,0,1,0,0,1,1,0,0,
	0,1,1,1,1,1,1,1,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};

static const UINT stat_pixels_dock_btn_contexthelp[] =
{
	0,0,0,1,1,1,1,0,0,
	0,0,1,1,0,0,1,1,0,
	0,0,1,1,0,0,1,1,0,
	0,0,0,0,0,1,1,0,0,
	0,0,0,0,1,1,0,0,0,
	0,0,0,0,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,1,0,0,0,
	0,0,0,0,1,1,0,0,0,
};

static CExtPaintManager::glyph_t stat_glyph_dock_empty(
	9,9,2,
	stat_pixels_dock_btn_empty
	);
static CExtPaintManager::glyph_t stat_glyph_dock_arrow_close(
	9,9,2,
	stat_pixels_dock_btn_close
	);
static CExtPaintManager::glyph_t stat_glyph_dock_arrow_up(
	9,9,2,
	stat_pixels_dock_btn_arrow_up
	);
static CExtPaintManager::glyph_t stat_glyph_dock_arrow_down(
	stat_glyph_dock_arrow_up,
	180
	);
static CExtPaintManager::glyph_t stat_glyph_dock_arrow_left(
	stat_glyph_dock_arrow_up,
	270
	);
static CExtPaintManager::glyph_t stat_glyph_dock_arrow_right(
	stat_glyph_dock_arrow_left,
	180
	);
static CExtPaintManager::glyph_t stat_glyph_dock_wnd_minimize(
	9,9,2,
	stat_pixels_dock_wnd_minimize
	);
static CExtPaintManager::glyph_t stat_glyph_dock_wnd_restore(
	9,9,2,
	stat_pixels_dock_wnd_restore
	);
static CExtPaintManager::glyph_t stat_glyph_dock_wnd_maximize(
	9,9,2,
	stat_pixels_dock_wnd_maximize
	);
static CExtPaintManager::glyph_t stat_glyph_dock_wnd_keep(
	9,9,2,
	stat_pixels_dock_wnd_keep
	);
static CExtPaintManager::glyph_t stat_glyph_dock_wnd_unkeep(
	stat_glyph_dock_wnd_keep,
	90
	);

static CExtPaintManager::glyph_t stat_glyph_dock_wnd_contexthelp(
	9,9,2,
	stat_pixels_dock_btn_contexthelp
	);


CExtPaintManager::glyph_t *
	CExtPaintManager::g_DockingCaptionGlyphs[__DCBT_VALUE_MAX+1] =
{
	& stat_glyph_dock_empty,
	& stat_glyph_dock_arrow_close,
	& stat_glyph_dock_arrow_up,
	& stat_glyph_dock_arrow_down,
	& stat_glyph_dock_arrow_left,
	& stat_glyph_dock_arrow_right,
	& stat_glyph_dock_wnd_minimize,
	& stat_glyph_dock_wnd_restore,
	& stat_glyph_dock_wnd_maximize,
	& stat_glyph_dock_wnd_contexthelp,
	& stat_glyph_dock_wnd_keep,
	& stat_glyph_dock_wnd_unkeep,
};


void CExtPaintManager::PaintDockingCaptionButton(
	CDC & dc,
	const CRect & rectClient,
	e_docking_caption_button_t eType,
	bool bHover,
	bool bPushed,
	bool bEnabled,
	bool bSideBar,
	bool bBarWndActive
	)
{
	bBarWndActive;
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT(
		__DCBT_VALUE_MIN <= eType
		&&
		eType <= __DCBT_VALUE_MAX
		);
	if( rectClient.IsRectEmpty() )
		return;
	if( !bEnabled )
		bHover = false;
COLORREF ColorValues[2] =
{
	RGB(0,0,0),
	RGB(0,0,0),
};
COLORREF clr3dHilight =
		GetColor( COLOR_3DHILIGHT );
	if( bEnabled )
	{
		if( ! (bHover || bPushed) )
			ColorValues[1] = clr3dHilight;
	} // if( bEnabled )
	else
	{
		ColorValues[1] = GetColor( COLOR_3DFACE );
	} // else from if( bEnabled )
CRect rcGlyph( rectClient );
	rcGlyph.NormalizeRect();
	if( bHover || bPushed )
		dc.FillSolidRect(
			&rcGlyph,
			clr3dHilight
			);
	if( rcGlyph.Width() & 1 )
		rcGlyph.left++;
	if( rcGlyph.Height() & 1 )
		rcGlyph.top++;
glyph_t * pGlyph = g_DockingCaptionGlyphs[eType];
	ASSERT( pGlyph != NULL );
	PaintGlyphCentered(
		dc,
		rcGlyph,
		*pGlyph,
		ColorValues
		);
}

void CExtPaintManagerXP::PaintDockingCaptionButton(
	CDC & dc,
	const CRect & rectClient,
	e_docking_caption_button_t eType,
	bool bHover,
	bool bPushed,
	bool bEnabled,
	bool bSideBar,
	bool bBarWndActive
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT(
		__DCBT_VALUE_MIN <= eType
		&&
		eType <= __DCBT_VALUE_MAX
		);
	if( rectClient.IsRectEmpty() )
		return;
	if( !bEnabled )
		bHover = false;
COLORREF ColorValues[2] =
{
	RGB(0,0,0),
	RGB(0,0,0),
};
	if( bEnabled )
	{
		if( ! (bHover || bPushed) )
			ColorValues[1] =
				GetColor(
					bSideBar ?
						(bBarWndActive ? COLOR_HIGHLIGHTTEXT : COLOR_BTNTEXT)
						:
						COLOR_3DHILIGHT
					);
	} // if( bEnabled )
	else
	{
		ColorValues[1] =
			GetColor(
				COLOR_3DSHADOW // COLOR_3DFACE
				);
	} // else from if( bEnabled )
CRect rcGlyph( rectClient );
	rcGlyph.NormalizeRect();
	if( bHover || bPushed )
	{
		dc.FillSolidRect(
			&rcGlyph,
			//clr3dHilight
			GetColor(
				bPushed ?
					XPCLR_HILIGHT
					:
					CLR_3DFACE_IN
				)
			);
		CRect rcBtnBorder(rcGlyph);
		rcBtnBorder.InflateRect(1,1);
		COLORREF clrBtnBorder =
			GetColor(
				(bBarWndActive ? XPCLR_HILIGHT : XPCLR_HILIGHT_BORDER)
				);
		dc.Draw3dRect(
			rcBtnBorder,
			clrBtnBorder,
			clrBtnBorder
			);
	}
	if( rcGlyph.Width() & 1 )
		rcGlyph.left++;
	if( rcGlyph.Height() & 1 )
		rcGlyph.top++;
glyph_t * pGlyph = g_DockingCaptionGlyphs[eType];
	ASSERT( pGlyph != NULL );
	PaintGlyphCentered(
		dc,
		rcGlyph,
		*pGlyph,
		ColorValues
		);
}

void CExtPaintManager::PaintPushButton(
	CDC & dc,
	bool bHorz,
	const CRect & rectClient,
	LPCTSTR sText,
	HICON hIcon,
	bool bFlat,
	bool bHover,
	bool bPushed,
	bool bIndeterminate,
	bool bEnabled,
	bool bDrawBorder,
	bool bDrawFocusRect,
	bool bDefaultPushButton,
	int eAlign,
	CFont * pFont, // = NULL
	bool bDropDown, // = false
	UINT nHelperSysCmdID, // = 0
	bool bTransparentBackground // = false
	)
{
	if( rectClient.IsRectEmpty() )
		return;

	if( !bEnabled )
	{
		bHover = false;
		//bDrawBorder = false;
		bDrawFocusRect = false;
		bIndeterminate = false;
	}

COLORREF clrOldText = dc.GetTextColor();
COLORREF clrOldBk = dc.GetBkColor();
int nOldBkMode = dc.SetBkMode( TRANSPARENT );

	// Prepare draw... paint button's area with background color
COLORREF bgColor;
	if(	bHover || bPushed )
		bgColor = GetColor(CLR_3DFACE_IN);
	else
		bgColor = GetColor(CLR_3DFACE_OUT);
	if( !bTransparentBackground )
	{
		//if( bIndeterminate )
		if( bEnabled && bPushed && bHover /*&& bFlat*/ )
			dc.FillRect( &rectClient, &m_brushLight );
		else
		{
			CBrush br( bgColor );
			dc.FillRect( &rectClient, &br );
		}
	} // if( !bTransparentBackground )

	// Draw pressed button
	if( bPushed )
	{
			if( bFlat )
			{
				if( bDrawBorder )
					dc.Draw3dRect(
						rectClient,
						GetColor(CLR_3DSHADOW_IN),
						GetColor(CLR_3DHILIGHT_IN)
						);
			} // if( bFlat )
			else    
			{
				dc.Draw3dRect(
					rectClient,
					GetColor(CLR_3DSHADOW_IN),
					GetColor(CLR_3DHILIGHT_IN)
					);
			} // else from if( bFlat )
	} // if( bPushed )
	else // ...else draw non pressed button
	{
		if( bFlat )
		{
			if( bHover && bDrawBorder )
			{
				dc.Draw3dRect(
					rectClient,
					GetColor(CLR_3DHILIGHT_OUT),
					GetColor(CLR_3DSHADOW_OUT)
					);
			}
		} // if( bFlat )
		else
		{
			if( bHover )
			{
				CRect cc(rectClient);
				dc.Draw3dRect(
					cc,
					GetColor(CLR_3DSHADOW_OUT),
					GetColor(CLR_3DDKSHADOW_OUT)
					);
				cc.DeflateRect(1,1);
				dc.Draw3dRect(
					cc,
					GetColor(CLR_3DHILIGHT_OUT),
					GetColor(CLR_3DSHADOW_OUT)
					);
			} // if( bHover )
			else
			{
				dc.Draw3dRect(
					rectClient,
					GetColor(CLR_3DHILIGHT_OUT),
					GetColor(CLR_3DSHADOW_OUT)
					);
			} // else from if( bHover )
		}  // else from if( bFlat )
	} // else from if( bPushed )

CString sTitle( _T("") );
	if( sText != NULL )
		sTitle = sText;

CRect rectClient2( rectClient );
	if( bDropDown )
	{
		CRect rcDopArea( rectClient2 );
		COLORREF ColorValues[2] =
		{
			RGB(0,0,0),
			GetColor(
				bEnabled ? CLR_TEXT_OUT : CLR_3DSHADOW_OUT
				)
		};
		if( bHorz )
		{
			rectClient2.right -=
				__DROPDOWN_ARROW_GAP*2
				+ g_glyph_btn_expand_bottom.Size().cx
				;
			rcDopArea.left = rectClient2.right;
			rcDopArea.DeflateRect(1,1);
			rcDopArea.OffsetRect(-1,0);
			if( bPushed )
				rcDopArea.OffsetRect(
					GetPushedOffset()
					);
			PaintGlyphCentered(
				dc,
				rcDopArea,
				g_glyph_btn_expand_bottom,
				ColorValues
				);
		} // if( bHorz )
		else
		{
			rectClient2.top +=
				__DROPDOWN_ARROW_GAP*2
				+ g_glyph_btn_expand_right.Size().cy
				;
			rcDopArea.bottom = rectClient2.top;
			rcDopArea.DeflateRect(1,1);
			rcDopArea.OffsetRect(0,1);
			if( bPushed )
				rcDopArea.OffsetRect(
					GetPushedOffset()
					);
			PaintGlyphCentered(
				dc,
				rcDopArea,
				g_glyph_btn_expand_right,
				ColorValues
				);
		} // else from if( bHorz )
	} // if( bDropDown )

CRect rectCaption = rectClient2; //lpDIS->rcItem;
	// Draw the icon
	if(	rectClient2.bottom > rectClient2.top
		&& rectClient2.right > rectClient2.left
		)
	{
		if( hIcon != NULL )
		{
			PaintIcon(
				dc,
				bHorz,
				sTitle,
				hIcon,
				rectClient2,
				rectCaption,
				bPushed,
				bEnabled,
				bHover,
				eAlign
				);
		} // if( hIcon != NULL )
		else
		{
			PaintSysCmdGlyph(
				dc,
				nHelperSysCmdID,
				rectClient2,
				false,
				bPushed,
				bEnabled
				);
		}
	}

	if( rectCaption.bottom > rectCaption.top
		&& rectCaption.right > rectCaption.left
		)
	{ // if we have valid area for text & focus rect
		if( !sTitle.IsEmpty() )
		{
			if( bPushed )
			{
				CSize sizePushedOffset = GetPushedOffset();
				rectCaption.OffsetRect(sizePushedOffset);
			}
			dc.SetBkColor( bgColor );

			if( bEnabled )
			{
				if( bHover || bPushed ) 
					dc.SetTextColor(
						GetColor(COLOR_3DDKSHADOW)
						);
				else 
					dc.SetTextColor(
						GetColor(
							COLOR_BTNTEXT /// !?!?!COLOR_HIGHLIGHTTEXT
							)
						);
			} // if( bEnabled )
			else
			{
				dc.SetTextColor(
					GetColor(COLOR_3DFACE)
					);
			} // else from if( bEnabled )

/*
			// center text
			CRect centerRect = rectCaption;
*/

			CFont * pOldBtnFont = NULL;
			CFont * pCurrFont = NULL;
			CFont fontDummy;

			if( pFont != NULL )
			{
				ASSERT( pFont->GetSafeHandle() != NULL );
				if( bHorz )
					pCurrFont = pFont;
				else
				{
					LOGFONT lf;
					::memset(&lf,0,sizeof(LOGFONT));
					pFont->GetLogFont(&lf);
					if( lf.lfEscapement == 0 )
					{
						lf.lfEscapement = __VERT_FONT_ESCAPEMENT__;
						VERIFY(
							fontDummy.CreateFontIndirect(&lf)
							);
						pCurrFont = &fontDummy;
					}
					else
					{
						// suppose font already prepared
						pCurrFont = pFont;
					}
				}
			} // if( pFont != NULL )
			else
			{
				if( bHorz )
				{
					if( bDefaultPushButton )
						pCurrFont = &m_FontBold;
					else
						pCurrFont = &m_FontNormal;
				}
				else
				{
					if( bDefaultPushButton )
						pCurrFont = &m_FontBoldVert;
					else
						pCurrFont = &m_FontNormalVert;
				}
			} // else from if( pFont != NULL )
			ASSERT( pCurrFont != NULL );
			pOldBtnFont = dc.SelectObject( pCurrFont );
			ASSERT( pOldBtnFont != NULL );

			int nTextLength = sTitle.GetLength();
			int nAmpIndex = sTitle.Find( _T('&') );
			CString sBtn;
			if( nAmpIndex < 0 )
				sBtn = sTitle;
			else
				sBtn =
					sTitle.Left(nAmpIndex)
					+ sTitle.Right(nTextLength - (nAmpIndex+1)
					);
			ASSERT( sBtn.Find( _T('&') ) < 0 );

			CRect rcText(0,0,0,0);
			dc.DrawText(
				sBtn,
				-1,
				rcText,
				DT_SINGLELINE|DT_CALCRECT
				);
			CRect rcCenteredCaption(rectCaption);
			rcCenteredCaption.OffsetRect(
				(rectCaption.Width() - rcText.Width())/2,
				(rectCaption.Height() - rcText.Height())/2
				);

			if( bHorz )
			{
/*
				dc.DrawText(
					sTitle,
					-1,
					rectCaption,
					DT_SINGLELINE|DT_CENTER|DT_VCENTER
					);
*/
				dc.DrawState(
					rcCenteredCaption.TopLeft(),
					rcCenteredCaption.Size(),
					(LPCTSTR)sTitle,
					(bEnabled) ? DSS_NORMAL : m_DSS_DISABLED_style, 
					TRUE,
					0,
					(CBrush*)NULL
					);
			}
			else
			{
				///int iOldAdvalncedGraphicsMode = // fix on NT
				///	::GetGraphicsMode(dc.GetSafeHdc());
				///::SetGraphicsMode(dc.GetSafeHdc(), GM_ADVANCED);

				LOGFONT lf;
				::memset(&lf,0,sizeof(LOGFONT));
				pCurrFont->GetLogFont(&lf);
				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin =
					(rectCaption.Width() - _cyHorzFont)  / 2
					;

				CPoint
					ptLineFrom(0,0),
					ptLineTo(0,0);
				if( nAmpIndex >= 0 )
				{
					ptLineFrom =
						CPoint(
							_cyTextMargin,
							CXTEXTMARGIN
								+ stat_CalcTextWidth(
									dc,
									bDefaultPushButton ?
										m_FontBold : m_FontNormal,
									sBtn.Left(nAmpIndex)
									)
							);
					ptLineTo =
						CPoint(
							_cyTextMargin,
							CXTEXTMARGIN
								+ stat_CalcTextWidth(
									dc,
									bDefaultPushButton ?
										m_FontBold : m_FontNormal,
									sBtn.Left(nAmpIndex+1)
									)
							);
				} // if( nAmpIndex >= 0 )

				CRect rcString = 
					CRect(
						CPoint(
							rectCaption.right - _cyTextMargin,
							rectCaption.top + CXTEXTMARGIN
							),
						rectCaption.Size() //m_sizeHorz
						);
				rcString.DeflateRect(2,2);

				CSize ptFix(-rcString.Width()-1,0);
				ptLineFrom += rcString.TopLeft() + ptFix;
				ptLineTo += rcString.TopLeft() + ptFix;
				if( bEnabled )
				{
					dc.DrawText(
						sBtn,
						rcString,
						DT_SINGLELINE //|DT_CENTER|DT_VCENTER
							|DT_NOCLIP|DT_NOPREFIX
						); // don't forget DT_NOCLIP
					if( nAmpIndex >= 0 )
					{
						CPen pen(
							PS_SOLID,
							0,
							dc.GetTextColor()
							);
						CPen * pOldPen = dc.SelectObject( &pen );
						dc.MoveTo( ptLineFrom );
						dc.LineTo( ptLineTo );
						dc.SelectObject( pOldPen );
					} // if( nAmpIndex >= 0 )
				} // if( bEnabled )
				else
				{
					CPoint ptOffsetPre(1,1);
					CPoint ptOffsetPost(-1,-1);
					COLORREF clrText = GetColor(COLOR_3DSHADOW);
					COLORREF clrShadow = GetColor(COLOR_3DHILIGHT);
					rcString.OffsetRect(ptOffsetPre.x,ptOffsetPre.y);
					dc.SetTextColor( clrShadow );
					dc.DrawText(
						sBtn,
						rcString,
						DT_SINGLELINE //|DT_CENTER|DT_VCENTER
							|DT_NOCLIP|DT_NOPREFIX
						); // don't forget DT_NOCLIP
					rcString.OffsetRect(ptOffsetPost.x,ptOffsetPost.y);
					dc.SetTextColor( clrText );
					dc.DrawText(
						sBtn,
						rcString,
						DT_SINGLELINE //|DT_CENTER|DT_VCENTER
							|DT_NOCLIP|DT_NOPREFIX
						); // don't forget DT_NOCLIP
					if( nAmpIndex >= 0 )
					{
						CPen pen(
							PS_SOLID,
							0,
							clrShadow
							);
						CPen * pOldPen = dc.SelectObject( &pen );
						ptLineFrom += ptOffsetPre;
						ptLineTo += ptOffsetPre;
						dc.MoveTo( ptLineFrom );
						dc.LineTo( ptLineTo );
						pen.DeleteObject();
						pen.CreatePen(
							PS_SOLID,
							0,
							clrText
							);
						dc.SelectObject( &pen );
						ptLineFrom += ptOffsetPost;
						ptLineTo += ptOffsetPost;
						dc.MoveTo( ptLineFrom );
						dc.LineTo( ptLineTo );
						dc.SelectObject( pOldPen );
					} // if( nAmpIndex >= 0 )
				} // else from if( bEnabled )

				///::SetGraphicsMode( dc.GetSafeHdc(), iOldAdvalncedGraphicsMode );
			}

			dc.SelectObject(pOldBtnFont);
		} // if( !sTitle.IsEmpty() )

		if( bDrawFocusRect )
		{
			CRect focusRect = rectCaption; // rectClient
			focusRect.DeflateRect( 3, 3 );
			dc.DrawFocusRect(&focusRect);
		} // if( bDrawFocusRect )
	} // if we have valid area for text & focus rect


	dc.SetBkMode( nOldBkMode );
	dc.SetBkColor( clrOldBk );
	dc.SetTextColor( clrOldText );
}

CRect CExtPaintManager::stat_CalcTextDimension(
	CDC & dc,
	CFont & font,
	const CString & strText
	)
{
	ASSERT( dc.GetSafeHdc() != NULL );
	ASSERT( font.GetSafeHandle() != NULL );
CRect rcText(0,0,0,0);
CFont * pOldFont = dc.SelectObject(
		&font
		);
	ASSERT( pOldFont != NULL );
	dc.DrawText(
		strText,
		&rcText,
		DT_SINGLELINE | DT_CALCRECT
		);
	dc.SelectObject(pOldFont);
	rcText.OffsetRect( -rcText.TopLeft() );
	return rcText;
}

int CExtPaintManager::stat_CalcTextWidth(
	CDC & dc,
	CFont & font,
	const CString & strText
	)
{
	return
		CExtPaintManager::stat_CalcTextDimension(
			dc,
			font,
			strText
			).Width();
}

void CExtPaintManager::PaintMenuSeparator(
	CDC & dc,
	CRect & rectItem,
	bool bRarelyUsed
	)
{
CRect rectItem2(rectItem);
	if( bRarelyUsed )
	{
		rectItem2.InflateRect(3,0,4,0);
		dc.FillRect(
			&rectItem2,
			&m_brushLight
			);
		rectItem2.DeflateRect(3,0,4,0);
	}
	else
		dc.FillSolidRect(
			&rectItem2,
			GetColor(CLR_3DFACE_OUT)
			);

int nFunnyDeflateMetric =
		::GetSystemMetrics(SM_CXSMICON) / 2;
	rectItem2.DeflateRect(
		nFunnyDeflateMetric,
		0
		);

COLORREF clrBtnShadow =
		GetColor(
			bRarelyUsed ?
				CLR_3DFACE_OUT : CLR_3DSHADOW_OUT
			);
COLORREF clrBtnHilight =
		GetColor(CLR_3DHILIGHT_OUT);
int y = (rectItem2.top+rectItem2.bottom)/2;
rectItem2.top = y-1; rectItem2.bottom = y+1;
	dc.Draw3dRect(
		rectItem2,
		clrBtnShadow,
		clrBtnHilight
		);
}

void CExtPaintManagerXP::PaintMenuSeparator(
	CDC & dc,
	CRect & rectItem,
	bool bRarelyUsed
	)
{
	if( rectItem.IsRectEmpty() )
		return;
COLORREF clr3dFace = // GetColor(CLR_3DFACE_OUT);
		GetColor(
			bRarelyUsed ?
				XPCLR_RARELY_BORDER : CLR_3DFACE_OUT
		);
CRect rc(rectItem);
	rc.right = rc.left+__DEF_MENU_ICON_AREA_DX;
	dc.FillSolidRect(
		rc,
		clr3dFace
		);
	rc = rectItem;
	rc.left += __DEF_MENU_ICON_AREA_DX;
	dc.FillSolidRect(
		rc,
		GetColor(XPCLR_3DFACE_NORMAL)
		);
	rc.left += __DEF_MENU_GAP;
	rc.top += rc.Height()/2;
	rc.bottom = rc.top+1;
	dc.FillSolidRect(
		rc,
		clr3dFace
		);
}

void CExtPaintManager::PaintSysCmdGlyph(
	CDC & dc,
	UINT nHelperSysCmdID,
	const CRect & rcItem,
	bool bHover,
	bool bPushed,
	bool bEnabled
	)
{
e_docking_caption_button_t eGlyph = __DCBT_EMPTY;
	switch( nHelperSysCmdID )
	{
	case SC_CLOSE:
		eGlyph = __DCBT_CLOSE;
	break;
	case SC_RESTORE:
		eGlyph = __DCBT_WND_RESTORE;
	break;
	case SC_MINIMIZE:
		eGlyph = __DCBT_WND_MINIMIZE;
	break;
	case SC_MAXIMIZE:
		eGlyph = __DCBT_WND_MAXIMIZE;
	break;
	case SC_CONTEXTHELP:
		eGlyph = __DCBT_WND_CONTEXTHELP;
	break;
	} // switch( nHelperSysCmdID )
	if( eGlyph == __DCBT_EMPTY )
		return;
COLORREF ColorValues[2] = {
	RGB(0,0,0),
	RGB(0,0,0),
};
CRect _rcItem( rcItem );
	if( bPushed )
		_rcItem.OffsetRect(
			GetPushedOffset()
			);
	if( bEnabled )
	{
		ColorValues[1] =
			GetColor(
				bHover ?
					COLOR_HIGHLIGHTTEXT : CLR_TEXT_OUT
				);
	} // if( bEnabled )
	else
	{
		ColorValues[1] =
			GetColor( CLR_3DHILIGHT_OUT );
		_rcItem.OffsetRect(1,1);
		PaintGlyphCentered(
			dc,
			_rcItem,
			*g_DockingCaptionGlyphs[eGlyph],
			ColorValues
			);
		_rcItem.OffsetRect(-1,-1);
		ColorValues[1] =
			GetColor( CLR_3DSHADOW_OUT );
	} // if( bEnabled )
	PaintGlyphCentered(
		dc,
		_rcItem,
		*g_DockingCaptionGlyphs[eGlyph],
		ColorValues
		);
}

void CExtPaintManagerXP::PaintSysCmdGlyph(
	CDC & dc,
	UINT nHelperSysCmdID,
	const CRect & rcItem,
	bool bHover,
	bool bPushed,
	bool bEnabled
	)
{
e_docking_caption_button_t eGlyph = __DCBT_EMPTY;
	switch( nHelperSysCmdID )
	{
	case SC_CLOSE:
		eGlyph = __DCBT_CLOSE;
	break;
	case SC_RESTORE:
		eGlyph = __DCBT_WND_RESTORE;
	break;
	case SC_MINIMIZE:
		eGlyph = __DCBT_WND_MINIMIZE;
	break;
	case SC_MAXIMIZE:
		eGlyph = __DCBT_WND_MAXIMIZE;
	break;
	case SC_CONTEXTHELP:
		eGlyph = __DCBT_WND_CONTEXTHELP;
	break;
	} // switch( nHelperSysCmdID )
	if( eGlyph == __DCBT_EMPTY )
		return;
COLORREF ColorValues[2] = {
	RGB(0,0,0),
	RGB(0,0,0),
};
CRect _rcItem( rcItem );
	if( bPushed )
		_rcItem.OffsetRect(
			GetPushedOffset()
			);
	if( bEnabled )
	{
//		if( bHover || bPushed )
//			ColorValues[1] =
//				GetColor( CLR_TEXT_IN );
		if( bHover & (!bPushed) )
		{
			ColorValues[1] =
				GetColor( COLOR_3DDKSHADOW );
			//_rcItem.OffsetRect(1,1);
			PaintGlyphCentered(
				dc,
				_rcItem,
				*g_DockingCaptionGlyphs[eGlyph],
				ColorValues
				);
			_rcItem.OffsetRect(-1,-1);
		}
		ColorValues[1] = GetColor( CLR_TEXT_OUT );
	} // if( bEnabled )
	else
	{
		ColorValues[1] = GetColor( COLOR_3DSHADOW );
	} // if( bEnabled )
	PaintGlyphCentered(
		dc,
		_rcItem,
		*g_DockingCaptionGlyphs[eGlyph],
		ColorValues
		);
}

void CExtPaintManager::PaintMenuItem(
	CDC & dc,
	const CRect & rectItem,
	LPCTSTR sText,
	LPCTSTR sAccelText,
	HICON hIcon,
	bool bPopup,
	bool bSelected,
	bool bPushedCheck,
	bool bPushedRadio,
	bool bIndeterminate,
	bool bEnabled,
	bool bRarelyUsed,
	bool bRarelyUsedPrev,
	bool bRarelyUsedNext,
	UINT nHelperSysCmdID // for painting system menu glyphs
	)
{
	if( rectItem.IsRectEmpty() )
		return;
CString _sText =
		(sText==NULL) ? _T("") : sText;
CString _sAccelText =
		(sAccelText==NULL) ? _T("") : sAccelText;

COLORREF clr3dFace = GetColor( CLR_3DFACE_OUT );
COLORREF clrBk = clr3dFace;
COLORREF clrText = GetColor(CLR_TEXT_OUT);
CRect rcBk(rectItem);
	dc.FillSolidRect(rcBk,clrBk);
	if( bSelected )
	{
		ASSERT( bEnabled );
		clrBk = GetColor(COLOR_HIGHLIGHT);
		clrText = GetColor(COLOR_HIGHLIGHTTEXT);
	}
	//else
	{
		if( bRarelyUsed )
		{
			rcBk.InflateRect(3,0,4,0);
			dc.FillRect(rcBk,&m_brushLight);
			if( !bRarelyUsedPrev )
			{
				CPen penShadow(
					PS_SOLID,
					1,
					GetColor(COLOR_3DSHADOW)
					);
				CPen * pOldPen =
					dc.SelectObject( &penShadow );
				ASSERT( pOldPen != NULL );
				dc.MoveTo(rcBk.left,rcBk.top);
				dc.LineTo(rcBk.right,rcBk.top);
				dc.SelectObject( pOldPen );
			}
			if( !bRarelyUsedNext )
			{
				CPen penLight(
					PS_SOLID,
					1,
					GetColor(COLOR_3DHIGHLIGHT)
					);
				CPen * pOldPen =
					dc.SelectObject( &penLight );
				ASSERT( pOldPen != NULL );
				dc.MoveTo(rcBk.left,rcBk.bottom-1);
				dc.LineTo(rcBk.right,rcBk.bottom-1);
				dc.SelectObject( pOldPen );
			}
			rcBk.DeflateRect(3,0,4,0);
		} // if( bRarelyUsed )
	}
bool bSmallerSelRect = 
		( hIcon != NULL || bPushedCheck || bPushedRadio ) ?
			true : false;
	if( bSmallerSelRect )
		rcBk.left += __DEF_MENU_ICON_AREA_DX;
	if( bSelected || (!bRarelyUsed) )
		dc.FillSolidRect(rcBk,clrBk);
	if( !bSmallerSelRect )
		rcBk.left += __DEF_MENU_ICON_AREA_DX;
	rcBk.DeflateRect(__DEF_MENU_GAP,__DEF_MENU_GAP);
COLORREF clrOldText = dc.SetTextColor(clrText);
int nOldBkMode = dc.SetBkMode(TRANSPARENT);
/*
	dc.DrawText(
		(LPCSTR)_sText,
		rcBk,
		DT_LEFT|DT_VCENTER|DT_SINGLELINE
		);
*/
CFont * pOldFont = dc.SelectObject(&m_FontNormal);
	ASSERT( pOldFont != NULL );
	dc.DrawState(
		rcBk.TopLeft(),
		rcBk.Size(),
		(LPCTSTR)(LPCSTR)_sText,
		(bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
		TRUE,
		0,
		(CBrush*)NULL
		);

	if( !_sAccelText.IsEmpty() )
	{
		CSize size;
		CRect rect(0,0,0,0);
		size.cy = dc.DrawText(
			(LPCTSTR)_sAccelText,
			_sAccelText.GetLength(),
			&rect,
			DT_CALCRECT|DT_SINGLELINE
				|DT_LEFT|DT_VCENTER
			);
		size.cx = rect.Width();
		rcBk.left =
			rcBk.right
			- size.cx
			- __DEF_MENU_POPUP_ARROW_AREA_DX
			;
/*
		dc.DrawText(
			(LPCSTR)_sAccelText,
			rcBk,
			DT_LEFT|DT_VCENTER|DT_SINGLELINE
			);
*/
		dc.DrawState(
			rcBk.TopLeft(),
			rcBk.Size(),
			(LPCTSTR)(LPCSTR)_sAccelText,
			(bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
			TRUE,
			0,
			(CBrush*)NULL
			);
	}

	if( bPopup )
	{
		CRect rcPopupArrow(rectItem);
		rcPopupArrow.DeflateRect(__DEF_MENU_GAP,__DEF_MENU_GAP);
		rcPopupArrow.left =
			rcPopupArrow.right - __DEF_MENU_POPUP_ARROW_AREA_DX;
		dc.SelectObject(&m_FontMarlett);
		dc.DrawText(
			_T("8"), // popup rightArrow
			rcPopupArrow,
			DT_CENTER|DT_VCENTER|DT_SINGLELINE
			);
	}
	
	// MODIFIED BY SUNZHENYU, modified the next 1 line
	dc.SelectObject(/*&*/pOldFont);
	dc.SetBkMode(nOldBkMode);
	dc.SetTextColor(clrOldText);

CRect rcIcon(rectItem);
	rcIcon.DeflateRect(__DEF_MENU_GAP,__DEF_MENU_GAP);
	rcIcon.right = rcIcon.left + __DEF_MENU_ICON_CY;
	if(rcIcon.Height() != __DEF_MENU_ICON_CY)
	{
		rcIcon.DeflateRect(0,rcIcon.Height()/2);
		if(rcIcon.Height() != __DEF_MENU_ICON_CY)
			rcIcon.bottom = rcIcon.top + __DEF_MENU_ICON_CY;
	}
CRect rcIconFrame(rcIcon);
	rcIconFrame.InflateRect(__DEF_MENU_IMG_RECT_OUT_GAP,__DEF_MENU_IMG_RECT_OUT_GAP);
	if( bPushedCheck || bPushedRadio )
		dc.FillRect(
			rcIconFrame,
			&m_brushLight
			);
	if( hIcon != NULL )
	{
		dc.DrawState(
			rcIcon.TopLeft(),
			rcIcon.Size(), 
			hIcon, 
			(bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
			(CBrush*)NULL
			);
		if( bSelected
			&& bEnabled
			&& (!(bPushedCheck || bPushedRadio))
			)
			dc.Draw3dRect(
				&rcIconFrame,
				GetColor(CLR_3DHILIGHT_OUT),
				GetColor(CLR_3DSHADOW_OUT)
				);
	} // if( hIcon != NULL )
	else
	{
		PaintSysCmdGlyph(
			dc,
			nHelperSysCmdID,
			rcIcon,
			bSelected,
			false,
			bEnabled
			);
	} // else from if( hIcon != NULL )

	if( bPushedCheck || bPushedRadio )
	{
		if( hIcon == NULL )
		{
			COLORREF clrCheck = GetColor(CLR_TEXT_OUT);
			int x = rcIcon.left + rcIcon.Width()/2 - 3;
			int y = rcIcon.top + rcIcon.Height()/2 - 3;
			if( bPushedCheck )
			{
				ASSERT( !bPushedRadio );
				stat_PaintCheckMark(
					dc,
					x,y,
					clrCheck
					);
			}
			else
			{
				ASSERT( !bPushedCheck );
				stat_PaintRadioDot(
					dc,
					x,y,
					clrCheck
					);
			}
		}
		dc.Draw3dRect(
			rcIconFrame,
			GetColor(CLR_3DSHADOW_OUT),
			GetColor(CLR_3DHILIGHT_OUT)
			);
	}
}

void CExtPaintManagerXP::PaintMenuItem(
	CDC & dc,
	const CRect & rectItem,
	LPCTSTR sText,
	LPCTSTR sAccelText,
	HICON hIcon,
	bool bPopup,
	bool bSelected,
	bool bPushedCheck,
	bool bPushedRadio,
	bool bIndeterminate,
	bool bEnabled,
	bool bRarelyUsed,
	bool bRarelyUsedPrev,
	bool bRarelyUsedNext,
	UINT nHelperSysCmdID // for painting system menu glyphs
	)
{
	if( rectItem.IsRectEmpty() )
		return;
CString _sText =
		(sText==NULL) ? _T("") : sText;
CString _sAccelText =
		(sAccelText==NULL) ? _T("") : sAccelText;

COLORREF clr3dFace = // GetColor( CLR_3DFACE_OUT );
		GetColor(
			bRarelyUsed ?
				XPCLR_RARELY_BORDER : CLR_3DFACE_OUT
		);
COLORREF clrDkBorder = GetColor( XPCLR_HILIGHT_BORDER );
COLORREF clrBk = clr3dFace;
COLORREF clrText = GetColor(CLR_TEXT_OUT);
COLORREF clrBkHilight = GetColor(XPCLR_HILIGHT);
COLORREF clrIconBkHilight =
			bSelected ?
				clrBkHilight
				:
				GetColor( CLR_3DFACE_IN );
COLORREF clrRight = GetColor(XPCLR_3DFACE_NORMAL);
CRect rcBk(rectItem);
	dc.FillSolidRect(rcBk,clrBk);
	if( bSelected )
	{
		ASSERT( bEnabled );
		clrBk = GetColor( CLR_3DFACE_IN ); // clrBkHilight;
	}
	
	dc.FillSolidRect(rcBk,clrBk);
	if( bSelected )
	{
		dc.Draw3dRect(
			rcBk,
			clrDkBorder,
			clrDkBorder
			);
		rcBk.left += __DEF_MENU_ICON_AREA_DX;
	}
	else
	{
		rcBk.left += __DEF_MENU_ICON_AREA_DX;
		dc.FillSolidRect(rcBk,clrRight);
	}

	rcBk.DeflateRect(__DEF_MENU_GAP,__DEF_MENU_GAP);
COLORREF clrOldText = dc.SetTextColor(clrText);
int nOldBkMode = dc.SetBkMode(TRANSPARENT);
/*
	dc.DrawText(
		(LPCSTR)_sText,
		rcBk,
		DT_LEFT|DT_VCENTER|DT_SINGLELINE
		);
*/
CFont * pOldFont = dc.SelectObject(&m_FontNormal);
	ASSERT( pOldFont != NULL );

	dc.DrawState(
		rcBk.TopLeft(),
		rcBk.Size(),
		(LPCTSTR)(LPCSTR)_sText,
		(bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
		TRUE,
		0,
		bEnabled ?
			(CBrush*)NULL
			:
			CBrush::FromHandle(
				GetBrush(COLOR_3DSHADOW)
				)
		);

	if( !_sAccelText.IsEmpty() )
	{
		CSize size;
		CRect rect(0,0,0,0);
		size.cy = dc.DrawText(
			(LPCTSTR)_sAccelText,
			_sAccelText.GetLength(),
			&rect,
			DT_CALCRECT|DT_SINGLELINE
				|DT_LEFT|DT_VCENTER
			);
		size.cx = rect.Width();
		rcBk.left =
			rcBk.right
			- size.cx
			- __DEF_MENU_POPUP_ARROW_AREA_DX
			;
/*
		dc.DrawText(
			(LPCSTR)_sAccelText,
			rcBk,
			DT_LEFT|DT_VCENTER|DT_SINGLELINE
			);
*/
		dc.DrawState(
			rcBk.TopLeft(),
			rcBk.Size(),
			(LPCTSTR)(LPCSTR)_sAccelText,
			(bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
			TRUE,
			0,
			bEnabled ?
				(CBrush*)NULL
				:
				CBrush::FromHandle(
					GetBrush(COLOR_3DSHADOW)
					)
			);
	}

	if( bPopup )
	{
		CRect rcPopupArrow(rectItem);
		rcPopupArrow.DeflateRect(__DEF_MENU_GAP,__DEF_MENU_GAP);
		rcPopupArrow.left =
			rcPopupArrow.right - __DEF_MENU_POPUP_ARROW_AREA_DX;
		dc.SelectObject(&m_FontMarlett);
		dc.DrawText(
			_T("8"), // popup rightArrow
			rcPopupArrow,
			DT_CENTER|DT_VCENTER|DT_SINGLELINE
			);
	}
	
	// MODIFIED BY SUNZHENYU, modified the next 1 line
	dc.SelectObject(/*&*/pOldFont);
	dc.SetBkMode(nOldBkMode);
	dc.SetTextColor(clrOldText);

CRect rcIcon(rectItem);
	rcIcon.DeflateRect(__DEF_MENU_GAP,__DEF_MENU_GAP);
	rcIcon.right = rcIcon.left + __DEF_MENU_ICON_CY;
	if(rcIcon.Height() != __DEF_MENU_ICON_CY)
	{
		rcIcon.DeflateRect(0,rcIcon.Height()/2);
		if(rcIcon.Height() != __DEF_MENU_ICON_CY)
			rcIcon.bottom = rcIcon.top + __DEF_MENU_ICON_CY;
	}
CRect rcIconFrame(rcIcon);
	rcIconFrame.InflateRect(__DEF_MENU_IMG_RECT_OUT_GAP,__DEF_MENU_IMG_RECT_OUT_GAP);
	if( bPushedCheck || bPushedRadio )
		dc.FillSolidRect(
			rcIconFrame,
			clrIconBkHilight
			);
	if( hIcon != NULL )
	{
		CRect rcDummy;
		PaintIcon(
			dc,
			true,
			CString(_T("")),
			hIcon,
			rcIcon,
			rcDummy,
			bPushedCheck || bPushedRadio,
			bEnabled,
			bSelected,
			0
			);
	}
	else
	{
		PaintSysCmdGlyph(
			dc,
			nHelperSysCmdID,
			rcIcon,
			bSelected,
			false,
			bEnabled
			);
	}

	if( bPushedCheck || bPushedRadio )
	{
		if( hIcon == NULL )
		{
			COLORREF clrCheck = GetColor(CLR_TEXT_OUT);
			int x = rcIcon.left + rcIcon.Width()/2 - 3;
			int y = rcIcon.top + rcIcon.Height()/2 - 3;
			if( bPushedCheck )
			{
				ASSERT( !bPushedRadio );
				stat_PaintCheckMark(
					dc,
					x,y,
					clrCheck
					);
			}
			else
			{
				ASSERT( !bPushedCheck );
				stat_PaintRadioDot(
					dc,
					x,y,
					clrCheck
					);
			}
		}
		dc.Draw3dRect(
			rcIconFrame,
			clrDkBorder,
			clrDkBorder
			);
	}
}

void CExtPaintManager::PaintIcon(
	CDC & dc,
	bool bHorz,
	CString & sTitle,
	HICON hIcon,
	const CRect & rectItem,
	CRect & rectCaption,
	bool bPushed,
	bool bEnabled,
	bool bHover,
	int eAlign
	)
{
	rectCaption = rectItem;
	if( hIcon == NULL )
		return;

ICONINFO ii;
	// Get icon dimension
	ZeroMemory(&ii, sizeof(ICONINFO));
	::GetIconInfo(hIcon, &ii);
BYTE cxIcon = (BYTE)(ii.xHotspot * 2);
BYTE cyIcon = (BYTE)(ii.yHotspot * 2);
    ::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);
/*
BYTE cxSysIconSmall = (BYTE)::GetSystemMetrics(SM_CXSMICON);
BYTE cySysIconSmall = (BYTE)::GetSystemMetrics(SM_CYSMICON);
	cxIcon = min( cxIcon, cxSysIconSmall );
	cyIcon = min( cyIcon, cxSysIconSmall );
	cxIcon = min( cxIcon, 16 );
	cyIcon = min( cyIcon, 16 );
*/

CRect iconRect = rectItem;
CRect btnRect;
	if( bHorz )
	{
		if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		{
			if( sTitle.IsEmpty() )
				// Center the icon horizontally
				iconRect.left += ((iconRect.Width() - cxIcon)/2);
			else
			{
				iconRect.left += 3;  
				rectCaption.left += cxIcon + 3;
			}
		} // if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		else
		{
			if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
			{
				btnRect = rectCaption;
				if( sTitle.IsEmpty() )
					// Center the icon horizontally
					iconRect.left += ((iconRect.Width() - cxIcon)/2);
				else
				{
					rectCaption.right = rectCaption.Width() - cxIcon - 3;
					rectCaption.left = 3;
					iconRect.left = btnRect.right - cxIcon - 3;
					// Center the icon vertically
					iconRect.top += ((iconRect.Height() - cyIcon)/2);
				}
			} // if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
		} // else from if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		if( (eAlign&__ALIGN_VERT) != 0 )
		{
			// Center the icon horizontally
			iconRect.top += ((iconRect.Height() - cyIcon)/2);
		} // if( (eAlign&__ALIGN_VERT) != 0 )
	} // if( bHorz )
	else
	{
		if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		{
			if( sTitle.IsEmpty() )
				// Center the icon horizontally
				iconRect.top += ((iconRect.Height() - cyIcon)/2);
			else
			{
				iconRect.top += 3;  
				rectCaption.top += cyIcon + 3;
			}
		} // if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		else
		{
			if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
			{
				btnRect = rectCaption;
				if( sTitle.IsEmpty() )
					// Center the icon horizontally
					iconRect.top += ((iconRect.Height() - cyIcon)/2);
				else
				{
					rectCaption.top = rectCaption.Height() - cyIcon - 3;
					rectCaption.top = 3;
					iconRect.top = btnRect.top - cyIcon - 3;
					// Center the icon vertically
					iconRect.left += ((iconRect.Width() - cxIcon)/2);
				}
			} // if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
		} // else from if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		if( (eAlign&__ALIGN_VERT) != 0 )
		{
			// Center the icon horizontally
			iconRect.left += ((iconRect.Width() - cxIcon)/2);
		} // if( (eAlign&__ALIGN_VERT) != 0 )
	} // else from if( bHorz )
    
	// If button is pressed then press the icon also
	if( bPushed )
	{
		CSize sizePushedOffset = GetPushedOffset();
		iconRect.OffsetRect(sizePushedOffset.cx, sizePushedOffset.cy);
	}
	dc.DrawState(
		iconRect.TopLeft(),
		iconRect.Size(), 
		hIcon, 
		(bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style), 
		(CBrush*)NULL
		);
}

CSize CExtPaintManager::GetPushedOffset()
{
	return CSize(1,1);
}

void CExtPaintManagerXP::PaintSeparator(
	CDC & dc,
	CRect & rectItem,
	bool bHorz
	)
{
CRect rectItem2(rectItem);
	dc.FillSolidRect(
		rectItem2,
		GetColor(CLR_3DFACE_OUT)
		);
	if( bHorz )
	{
		rectItem2.left =
			rectItem2.left + rectItem2.Width()/2 - 1;
		rectItem2.right =
			rectItem2.left + 1;
		rectItem2.DeflateRect( 0, 1 );
	}
	else
	{
		rectItem2.top =
			rectItem2.top + rectItem2.Height()/2 - 1;
		rectItem2.bottom =
			rectItem2.top + 1;
		rectItem2.DeflateRect( 1, 0 );
	}
	dc.FillSolidRect(
		&rectItem2,
		GetColor(XPCLR_SEPARATOR)
		);
}

void CExtPaintManager::PaintSeparator(
	CDC & dc,
	CRect & rectItem,
	bool bHorz
	)
{
CRect rectItem2(rectItem);
	dc.FillSolidRect(
		rectItem2,
		GetColor(CLR_3DFACE_OUT)
		);

COLORREF clrBtnShadow =  GetColor(CLR_3DSHADOW_OUT);
COLORREF clrBtnHilight = GetColor(CLR_3DHILIGHT_OUT);
	if( bHorz )
	{
		// draw the separator bar in the middle
		int x = (rectItem2.left + rectItem2.right) / 2;
		rectItem2.left = x-1; rectItem2.right = x+1;
		dc.Draw3dRect(
			rectItem2,
			clrBtnShadow,
			clrBtnHilight
		);
	} // if( bHorz )
	else
	{
		// draw middle separator
int y = (rectItem2.top+rectItem2.bottom)/2;
rectItem2.top = y-1; rectItem2.bottom = y+1;
		dc.Draw3dRect(
			rectItem2,
			clrBtnShadow,
			clrBtnHilight
			);
	} // else from if( bHorz )
}

void CExtPaintManager::PaintControlFrame(
	CDC & dc,
	CRect & rectClient,
	bool bFlat,
	bool bHover,
	bool bEnabled,
	bool bFocus
	)
{
	bFlat;
CRect rectItem( rectClient );
COLORREF clrInner = GetColor( COLOR_WINDOW );
COLORREF clrIn1, clrIn2, clrOut1, clrOut2;
	clrIn1 = clrIn2 = clrOut1 = clrOut2 = GetColor( CLR_3DFACE_OUT );
	if( bEnabled )
	{
		if( bHover && bFocus )
		{
				clrIn1 = GetColor( COLOR_3DFACE );
				clrIn2 = GetColor( COLOR_3DDKSHADOW );
				clrOut1 = GetColor( COLOR_3DHILIGHT );
				clrOut2 = GetColor( COLOR_3DSHADOW );
		} // if( bHover && bFocus )
		else
		{
			if( bHover || bFocus )
			{
				clrIn1 = GetColor( COLOR_3DHILIGHT );
				clrIn2 = GetColor( COLOR_3DSHADOW );
			} // if( bHover || bFocus )
		} // elsefrom if( bHover && bFocus )
	} // else from if( bEnabled )
	dc.Draw3dRect( rectItem, clrInner, clrInner );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrInner, clrInner );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrInner, clrInner );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrIn2, clrIn1 );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrOut2, clrOut1 );
}

void CExtPaintManagerXP::PaintControlFrame(
	CDC & dc,
	CRect & rectClient,
	bool bFlat,
	bool bHover,
	bool bEnabled,
	bool bFocus
	)
{
	bFlat;
CRect rectItem( rectClient );
COLORREF clrInner = GetColor( COLOR_WINDOW );
COLORREF clrIn, clrOut;
	clrIn = clrOut = GetColor( CLR_3DFACE_OUT );
	if( bEnabled )
	{
		if( bHover )
		{
			if( bFocus )
				clrIn = clrOut = GetColor( XPCLR_HILIGHT_BORDER );
			else
				clrIn = GetColor( XPCLR_HILIGHT_BORDER );
		} // if( bHover )
		else
		{
			if( bFocus )
				clrIn = GetColor( XPCLR_HILIGHT_BORDER );
		} // else from if( bHover )
	} // else from if( bEnabled )
	dc.Draw3dRect( rectItem, clrInner, clrInner );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrInner, clrInner );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrInner, clrInner );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrIn, clrIn );
	rectItem.InflateRect(1,1);
	dc.Draw3dRect( rectItem, clrOut, clrOut );
}

void CExtPaintManager::PaintComboFrame(
	CDC &dc,
	CRect & rectClient,
	bool bHover,
	bool bPushed,
	bool bEnabled
	)
{
COLORREF clrTopLeft, clrBottomRight;
	clrTopLeft = clrBottomRight =
		GetColor( CLR_3DFACE_OUT );
	if( bPushed || bHover )
	{
		clrTopLeft =
			GetColor( CLR_3DSHADOW_IN );
		clrBottomRight =
			GetColor( CLR_3DHILIGHT_IN );
	}
COLORREF clrHilightOut =	GetColor( CLR_3DHILIGHT_OUT );
COLORREF clrShadowIn =		GetColor( CLR_3DSHADOW_IN );
COLORREF clrHilightIn =		GetColor( CLR_3DHILIGHT_IN );
COLORREF clrFaceOut =		GetColor( CLR_3DFACE_OUT );

COLORREF clrWindow =
		GetColor(
			bEnabled ? COLOR_WINDOW : COLOR_3DFACE
			);

	// Cover up dark 3D shadow.
	dc.Draw3dRect(
		rectClient,
		clrTopLeft,
		clrBottomRight
		);
	rectClient.DeflateRect(1,1);
	dc.Draw3dRect(
		rectClient,
		bEnabled ? clrFaceOut : clrHilightOut,
		bEnabled ? clrFaceOut : clrHilightOut
		);

	// Cover up dark 3D shadow on drop arrow.
	rectClient.left =
		rectClient.right -
		::GetSystemMetrics(SM_CXHTHUMB);
	dc.Draw3dRect(
		rectClient,
		clrFaceOut,
		clrFaceOut
		);
	rectClient.DeflateRect(1,1);
	dc.Draw3dRect(
		rectClient,
		clrHilightOut,
		clrHilightOut
		);

	dc.FillSolidRect(
		rectClient.left-2,
		rectClient.top,
		3,
		rectClient.Height(),
		bEnabled ? clrWindow : clrFaceOut //clrHilightOut
		);

	if( bHover || bPushed )
		dc.FillSolidRect(
			rectClient,
			clrFaceOut
			);

	rectClient.left++;

CFont font;
int ppi = dc.GetDeviceCaps(LOGPIXELSX);
int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi
	font.CreatePointFont(pointsize, _T("Marlett"));

	PaintPushButton(
		dc,
		true,
		rectClient,
		_T("6"),
		NULL,
		true,
		(bHover&&(!bPushed)) ? true : false, //(bHover||bPushed) ? true : false,
		bPushed ? true : false,
		false,
		bEnabled,
		true,
		false,
		false,
		CExtPaintManager::__ALIGN_HORIZ_CENTER
			| CExtPaintManager::__ALIGN_VERT,
		&font // = NULL
		);

	if( !(bHover || bPushed) )
	{
		if( !bHover )
			dc.Draw3dRect(
				rectClient,
				clrHilightOut,
				clrHilightOut
				);
		else
			dc.Draw3dRect(
				rectClient,
				clrFaceOut,
				clrFaceOut
				);
	}
}

void CExtPaintManager::stat_PaintRadioDot(
	CDC & dc,
	int x,
	int y,
	COLORREF color
	)
{
CBrush brush,*pOldBrush;
CPen pen,*pOldPen;
CRect rcDot(x,y,x+6,y+6);

COLORREF color2 =
		dc.GetNearestColor(
			RGB(
				255-GetRValue(color),
				255-GetGValue(color),
				255-GetBValue(color),
			)
		);
	rcDot.OffsetRect(1,1);
	brush.CreateSolidBrush(color2);
	pen.CreatePen(PS_SOLID,0,color2);
	pOldBrush=dc.SelectObject(&brush);
	pOldPen=dc.SelectObject(&pen);
	dc.Ellipse(&rcDot);
	pen.DeleteObject();
	brush.DeleteObject();
	rcDot.OffsetRect(-1,-1);

	brush.CreateSolidBrush(color);
	pen.CreatePen(PS_SOLID,0,color);
	dc.SelectObject(&brush);
	dc.SelectObject(&pen);
	dc.Ellipse(&rcDot);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
	pen.DeleteObject();
	brush.DeleteObject();
}

void CExtPaintManager::stat_PaintCheckMark(
	CDC & dc,
	int x,
	int y,
	COLORREF color
	)
{
COLORREF color2 =
		dc.GetNearestColor(
			RGB(
				255-GetRValue(color),
				255-GetGValue(color),
				255-GetBValue(color),
			)
		);
 	dc.SetPixel(x,y+2,color);
	dc.SetPixel(x,y+3,color);
	dc.SetPixel(x,y+4,color2);

	dc.SetPixel(x+1,y+3,color);
	dc.SetPixel(x+1,y+4,color);
	dc.SetPixel(x+1,y+5,color2);

	dc.SetPixel(x+2,y+4,color);
	dc.SetPixel(x+2,y+5,color);
	dc.SetPixel(x+2,y+6,color2);

	dc.SetPixel(x+3,y+3,color);
	dc.SetPixel(x+3,y+4,color);
	dc.SetPixel(x+3,y+5,color2);

	dc.SetPixel(x+4,y+2,color);
	dc.SetPixel(x+4,y+3,color);
	dc.SetPixel(x+4,y+4,color2);

	dc.SetPixel(x+5,y+1,color);
	dc.SetPixel(x+5,y+2,color);
	dc.SetPixel(x+5,y+3,color2);

	dc.SetPixel(x+6,y,color);
	dc.SetPixel(x+6,y+1,color);
	dc.SetPixel(x+6,y+2,color2);
}

void CExtPaintManager::PaintDockingFrame(
	CDC & dc,
	const CRect & rectWindow,
	const CRect & rectClient,
	bool bFloating,
	bool bExtBar
	)
{
	if( rectWindow.IsRectEmpty() )
		return;
	dc.ExcludeClipRect(rectClient);
CRect rcWnd(rectWindow);
COLORREF clr3dFace = GetColor(CLR_3DFACE_OUT);
COLORREF clr3dHilight = GetColor(CLR_3DHILIGHT_OUT);
COLORREF clr3dShadow = GetColor(CLR_3DSHADOW_OUT);
COLORREF clr3dDkShadow = GetColor(CLR_3DDKSHADOW_OUT);
	dc.FillSolidRect(
		&rcWnd,
		clr3dFace
		);
	if( bFloating )
	{
		dc.Draw3dRect(
			&rcWnd, clr3dFace, clr3dDkShadow);
		rcWnd.DeflateRect(1,1);
		dc.Draw3dRect(
			&rcWnd, clr3dHilight, clr3dShadow);
	}
	else
	{
		if( !bExtBar )
		{
			dc.Draw3dRect(
				&rcWnd, clr3dHilight, clr3dShadow);
		}
	}
	dc.IntersectClipRect(rectWindow);
}

void CExtPaintManagerXP::PaintDockingFrame(
	CDC & dc,
	const CRect & rectWindow,
	const CRect & rectClient,
	bool bFloating,
	bool bExtBar
	)
{
	if( rectWindow.IsRectEmpty() )
		return;
	dc.ExcludeClipRect(rectClient);
CRect rcWnd(rectWindow);
COLORREF clr3dFace = GetColor(COLOR_3DFACE);
COLORREF clrDark = GetColor(COLOR_3DSHADOW);
COLORREF clrBkOld = dc.GetBkColor();
	dc.FillSolidRect(
		&rcWnd,
		GetColor(
			(bExtBar && bFloating) ?
				XPCLR_3DFACE_NORMAL : CLR_3DFACE_OUT
				)
		);
	if( bFloating )
	{
		dc.Draw3dRect(
			&rcWnd, clrDark, clrDark);
		rcWnd.DeflateRect(1,1);
		dc.Draw3dRect(
			&rcWnd, clrDark, clrDark);
//		rcWnd.DeflateRect(1,1);
//		dc.Draw3dRect(
//			&rcWnd, clrDark, clrDark);
	}
	else
	{
		if( !bExtBar )
		{
			dc.Draw3dRect(
				&rcWnd, clr3dFace, clr3dFace);
//			rcWnd.DeflateRect(1,1);
//			dc.Draw3dRect(
//				&rcWnd, clr3dFace, clr3dFace);
		}
	}
	dc.SetBkColor( clrBkOld );
	//dc.IntersectClipRect(rectWindow);
	dc.SelectClipRgn(NULL);
}

void CExtPaintManager::PaintGripper(
	CDC & dc,
	const CRect & rectGripper,
	const CRect & rectText,
	bool bActive,
	bool bFloating,
	bool bHorz,
	bool bSideBar, // = false
	LPCTSTR sCaption // = NULL
	)
{ 
CRect rectGripper2(rectGripper);
COLORREF clrBtnShadow =  GetColor(CLR_3DSHADOW_OUT);
COLORREF clrBtnHilight = GetColor(CLR_3DHILIGHT_OUT);
	if( bSideBar || bFloating )
	{
		COLORREF clrGrip =
			GetColor(
				bActive ? COLOR_HIGHLIGHT : COLOR_3DSHADOW
				);
		dc.FillSolidRect(
			rectGripper2,
			clrGrip
			);
		if( sCaption != NULL
			&& !rectText.IsRectEmpty()
			&& rectText.right > rectText.left
			&& rectText.bottom > rectText.top
			)
		{
			COLORREF clrText =
				GetColor(COLOR_HIGHLIGHTTEXT);
			COLORREF clrOldText =
				dc.SetTextColor(clrText);
			int nOldBkMode = dc.SetBkMode(TRANSPARENT);
			CFont * pCurrFont =
				(!bHorz) ?
					&m_FontBold : &m_FontBoldVertX
					;
			CFont * pOldFont =
				dc.SelectObject( pCurrFont );
			ASSERT( pOldFont != NULL );
			if( !bHorz )
			{ // if text is horizontal
				dc.DrawText(
					sCaption,
					(LPRECT)&rectText,
					DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS
					);
			} // if text is horizontal
			else
			{ // if text is vertical
				LOGFONT lf;
				::memset(&lf,0,sizeof(LOGFONT));
				pCurrFont->GetLogFont(&lf);
				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin =
					(rectText.Width() - _cyHorzFont)  / 2
					;

				CRect rcString = 
					CRect(
						CPoint(
							rectText.left + _cyTextMargin, // rectText.right - _cyTextMargin,
							rectText.bottom - CXTEXTMARGIN // rectText.top + CXTEXTMARGIN
							),
						rectText.Size() //m_sizeHorz
						);
				rcString.DeflateRect(2,2);

				CRect rcExclude;
				rcExclude.SetRect(
					rectGripper.left,
					rectGripper.top,
					rectGripper.right,
					rectText.top
					);
				dc.ExcludeClipRect( &rcExclude );

				dc.DrawText(
					sCaption,
					rcString,
					DT_SINGLELINE //|DT_CENTER|DT_VCENTER
						|DT_NOCLIP |DT_NOPREFIX
					); // don't forget DT_NOCLIP

				//dc.IntersectClipRect( &rectGripper );
				dc.SelectClipRgn(NULL);

/*
				CPoint ptOrg(
					rectText.left + 1,  // rectText.left - 1,
					rectText.bottom - 1 // rectText.bottom - 3
					);
				dc.ExtTextOut(
					ptOrg.x, ptOrg.y,
					ETO_CLIPPED,
					rectGripper2,
					(LPCTSTR)sCaption,
					NULL
					);
*/			
			} // if text is vertical
			dc.SelectObject( pOldFont );
			dc.SetBkMode(nOldBkMode);
			dc.SetTextColor(clrOldText);
		}
	} // if( bSideBar )
	else
	{
		if( bHorz )
		{
			rectGripper2.DeflateRect( 4, 3 );
			rectGripper2.right = rectGripper2.left + 3;
			dc.Draw3dRect(
				rectGripper2,
				clrBtnHilight,
				clrBtnShadow
			);
			rectGripper2.OffsetRect( 3, 0 );
			dc.Draw3dRect(
				rectGripper2,
				clrBtnHilight,
				clrBtnShadow
			);
		} // if( bHorz )
		else
		{
			rectGripper2.OffsetRect( 1, 0 );
			rectGripper2.DeflateRect( 3, 4 );
			rectGripper2.bottom = rectGripper2.top + 3;
			dc.Draw3dRect(
				rectGripper2,
				clrBtnHilight,
				clrBtnShadow
			);
			rectGripper2.OffsetRect( 0, 3 );
			dc.Draw3dRect(
				rectGripper2,
				clrBtnHilight,
				clrBtnShadow
			);
		} // else from if( bHorz )
	} // else from if( bSideBar )
}

void CExtPaintManagerXP::PaintGripper(
	CDC & dc,
	const CRect & rectGripper,
	const CRect & rectText,
	bool bActive,
	bool bFloating,
	bool bHorz,
	bool bSideBar, // = false
	LPCTSTR sCaption // = NULL
	)
{ 
CRect rectGripper2(rectGripper);
COLORREF clrBtnShadow =  GetColor(CLR_3DSHADOW_OUT);
COLORREF clrBtnHilight = GetColor(CLR_3DHILIGHT_OUT);
	if( bSideBar || bFloating )
	{
		COLORREF clrGrip =
			GetColor(
				//COLOR_HIGHLIGHT
				bSideBar ?
					(bActive ? COLOR_HIGHLIGHT : XPCLR_3DFACE_DARK )
					:
					COLOR_3DSHADOW
				);
		dc.FillSolidRect( rectGripper2, clrGrip );
		if( bSideBar && !bFloating && !bActive)
		{ // rect border of docked bar caption like VS-.NET-7.0
			COLORREF clrSideRect =
				GetColor( COLOR_3DDKSHADOW );
			dc.Draw3dRect(
				rectGripper2,
				clrSideRect,
				clrSideRect
				);
		} // rect border of docked bar caption like VS-.NET-7.0
		if( sCaption != NULL
			&& !rectText.IsRectEmpty()
			&& rectText.right > rectText.left
			&& rectText.bottom > rectText.top
			)
		{
			COLORREF clrText =
				GetColor(
					(bSideBar) ?
						(bActive ? COLOR_HIGHLIGHTTEXT : COLOR_BTNTEXT)
						:
						COLOR_HIGHLIGHTTEXT
					);
			COLORREF clrOldText =
				dc.SetTextColor(clrText);
			int nOldBkMode = dc.SetBkMode(TRANSPARENT);
			CFont * pCurrFont =
				(!bHorz) ?
					&m_FontBold : &m_FontBoldVertX
					;
			CFont * pOldFont =
				dc.SelectObject( pCurrFont );
			ASSERT( pOldFont != NULL );
			if( !bHorz )
			{ // if text is horizontal
				dc.DrawText(
					sCaption,
					(LPRECT)&rectText,
					DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS
					);
			} // if text is horizontal
			else
			{ // if text is vertical
				LOGFONT lf;
				::memset(&lf,0,sizeof(LOGFONT));
				pCurrFont->GetLogFont(&lf);
				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin =
					(rectText.Width() - _cyHorzFont)  / 2
					;

				CRect rcString = 
					CRect(
						CPoint(
							rectText.left + _cyTextMargin, // rectText.right - _cyTextMargin,
							rectText.bottom - CXTEXTMARGIN // rectText.top + CXTEXTMARGIN
							),
						rectText.Size() //m_sizeHorz
						);
				rcString.DeflateRect(2,2);

				CRect rcExclude;
				rcExclude.SetRect(
					rectGripper.left,
					rectGripper.top,
					rectGripper.right,
					rectText.top
					);
				dc.ExcludeClipRect( &rcExclude );

				dc.DrawText(
					sCaption,
					rcString,
					DT_SINGLELINE //|DT_CENTER|DT_VCENTER
						|DT_NOCLIP |DT_NOPREFIX
					); // don't forget DT_NOCLIP

				//dc.IntersectClipRect( &rectGripper );
				dc.SelectClipRgn(NULL);

/*
				CPoint ptOrg(
					rectText.left + 1,  // rectText.left - 1,
					rectText.bottom - 1 // rectText.bottom - 3
					);
				dc.ExtTextOut(
					ptOrg.x, ptOrg.y,
					ETO_CLIPPED,
					rectGripper2,
					(LPCTSTR)sCaption,
					NULL
					);
*/			
			} // if text is vertical
			dc.SelectObject( pOldFont );
			dc.SetBkMode(nOldBkMode);
			dc.SetTextColor(clrOldText);
		}
	} // if( bSideBar )
	else
	{
		COLORREF clrGrip =
			GetColor(CLR_3DFACE_OUT);
		dc.FillSolidRect(
			rectGripper,
			clrGrip
			);
		if( bHorz )
		{
			CBrush brGripperHorz;
			WORD horzHatchBits[8] =
			{
				0xFF, 0x00, 0xFF, 0x00,
				0xFF, 0x00, 0xFF, 0x00
			};
			CBitmap bmpGripperHorz;
			bmpGripperHorz.CreateBitmap( 8, 8, 1, 1, horzHatchBits );
			brGripperHorz.CreatePatternBrush( &bmpGripperHorz );

			rectGripper2.DeflateRect( 5, 4 );
			rectGripper2.right = rectGripper2.left + 5;
			rectGripper2.DeflateRect( 1, 0 );
			COLORREF clrOldBk = dc.SetBkColor( GetColor(XPCLR_3DFACE_NORMAL) );
			CBrush * pBrushOld = dc.SelectObject( &brGripperHorz );
			dc.PatBlt(
				rectGripper2.left, rectGripper2.top,
				rectGripper2.Width(), rectGripper2.Height(),
				0xA000C9
				);
			// MODIFIED BY SUNZHENYU, add next 1 line
			dc.SelectObject( pBrushOld );
			dc.SetBkColor( clrOldBk );
		} // if( bHorz )
		else
		{
			CBrush brGripperVert;
			WORD vertHatchBits[8] =
			{
				0xAA, 0xAA, 0xAA, 0xAA,
				0xAA, 0xAA, 0xAA, 0xAA
			};
			CBitmap bmpGripperVert;
			bmpGripperVert.CreateBitmap( 8, 8, 1, 1, vertHatchBits );
			brGripperVert.CreatePatternBrush( &bmpGripperVert );

			rectGripper2.OffsetRect( 1, 0 );
			rectGripper2.DeflateRect( 4, 5 );
			rectGripper2.bottom = rectGripper2.top + 5;
			rectGripper2.DeflateRect( 0, 1 );
			COLORREF clrOldBk = dc.SetBkColor( GetColor(XPCLR_3DFACE_NORMAL) );
			CBrush * pBrushOld = dc.SelectObject( &brGripperVert );
			dc.PatBlt(
				rectGripper2.left, rectGripper2.top,
				rectGripper2.Width(), rectGripper2.Height(),
				0xA000C9
				);
			dc.SelectObject( pBrushOld );
			dc.SetBkColor( clrOldBk );
		} // else from if( bHorz )
	} // else from if( bSideBar )
}

CSize CExtPaintManagerXP::GetPushedOffset()
{
	return CSize(0,0);
}

#define __XP_DSS_MONO DSS_MONO

void CExtPaintManagerXP::InitTranslatedColors()
{
	ASSERT( this != NULL );

	m_DSS_DISABLED_style = __XP_DSS_MONO;

	CExtPaintManager::InitTranslatedColors();

int nBitsPerPixel = CExtPaintManager::stat_GetBPP();

COLORREF clrSysWindow = ::GetSysColor(COLOR_WINDOW);
COLORREF clrSys3dFace = ::GetSysColor(COLOR_3DFACE);
COLORREF clrSys3dHilight = ::GetSysColor(COLOR_3DHILIGHT);
COLORREF clrSys3dShadow = ::GetSysColor(COLOR_3DSHADOW);
COLORREF clrSys3dDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);
COLORREF clrSysBtnText = ::GetSysColor(COLOR_BTNTEXT);
COLORREF clrSysInfoBk = ::GetSysColor(COLOR_INFOBK);
COLORREF clrSysItemHilight = ::GetSysColor(COLOR_HIGHLIGHT);

COLORREF xpclr_MenuLight = clrSysWindow;
COLORREF xpclr_ControlBarBk = clrSys3dFace;
COLORREF xpclr_Highlight = clrSys3dFace;
COLORREF xpclr_Separator = clrSys3dShadow;
COLORREF xpclr_PanelBorder = clrSys3dShadow;
COLORREF xpclr_RarelyUsedMenuLeft = clrSys3dFace;
COLORREF xpclr_HighlightDarked = clrSys3dShadow;
COLORREF xpclr_HighlightBorder = clrSysBtnText;

	if( nBitsPerPixel > 8 )
	{
		// xp - menu area light ver (like system COLOR_WINDOW but more brown)
		xpclr_MenuLight =
			RGB(
				( 750L*long(GetRValue(clrSysWindow)) + 250L*long(GetRValue(clrSysInfoBk)) ) / 1000L,
				( 750L*long(GetGValue(clrSysWindow)) + 250L*long(GetGValue(clrSysInfoBk)) ) / 1000L,
				( 750L*long(GetBValue(clrSysWindow)) + 250L*long(GetBValue(clrSysInfoBk)) ) / 1000L
				);
		// xp - funny window face
		COLORREF clrMixWindowFace =
			RGB(
				( 750L*long(GetRValue(clrSys3dFace)) + 250L*long(GetRValue(clrSysWindow)) ) / 1000L,
				( 750L*long(GetGValue(clrSys3dFace)) + 250L*long(GetGValue(clrSysWindow)) ) / 1000L,
				( 750L*long(GetBValue(clrSys3dFace)) + 250L*long(GetBValue(clrSysWindow)) ) / 1000L
				);
		// xp - rarely used item left side
		xpclr_RarelyUsedMenuLeft =
			RGB(
				( 999L*long(GetRValue(clrSys3dFace)) ) / 1000L,
				( 995L*long(GetGValue(clrSys3dFace)) ) / 1000L,
				( 995L*long(GetBValue(clrSys3dFace)) ) / 1000L
				);
		// xp - control bar funny background
		xpclr_ControlBarBk =
			RGB(
				( 970L*long(GetRValue(clrMixWindowFace)) ) / 1000L,
				( 970L*long(GetGValue(clrMixWindowFace)) ) / 1000L,
				( 970L*long(GetBValue(clrMixWindowFace)) ) / 1000L
				);
		// xp - blue like light ver
//		xpclr_Highlight =
//			RGB(
//				( 730L*long(GetRValue(xpclr_MenuLight)) ) / 1000L,
//				( 760L*long(GetGValue(xpclr_MenuLight)) ) / 1000L,
//				( 890L*long(GetBValue(xpclr_MenuLight)) ) / 1000L
//				);
		xpclr_Highlight =
			stat_HLS_Adjust(
				clrSysItemHilight,
				0.00, 0.75, -0.75
				);
		// xp - blue like dark ver
//		xpclr_HighlightDarked =
//			RGB(
//				( 730L*long(GetRValue(xpclr_Highlight)) ) / 1000L,
//				( 760L*long(GetGValue(xpclr_Highlight)) ) / 1000L,
//				( 890L*long(GetBValue(xpclr_Highlight)) ) / 1000L
//				);
		xpclr_HighlightDarked =
			stat_HLS_Adjust(
				xpclr_Highlight,
				0.00, -0.25, 0.10
				);
		// xp - blue dark selected border
//		xpclr_HighlightBorder =
//			RGB(
//				( 500L*long(GetRValue(xpclr_HighlightDarked)) ) / 1000L,
//				( 500L*long(GetGValue(xpclr_HighlightDarked)) ) / 1000L,
//				( 500L*long(GetBValue(xpclr_HighlightDarked)) ) / 1000L
//				);
		xpclr_HighlightBorder =
			stat_HLS_Adjust(
				xpclr_HighlightDarked,
				0.00, -0.50, 0.00
				);
		// xp - dark gray separator
		xpclr_Separator = 
			RGB(
				( 857L*long(GetRValue(clrSys3dFace)) ) / 1000L,
				( 857L*long(GetGValue(clrSys3dFace)) ) / 1000L,
				( 857L*long(GetBValue(clrSys3dFace)) ) / 1000L
				);
		// xp - dark panel border (for floating controlbars)
		xpclr_PanelBorder =
			RGB(
				( 750L*long(GetRValue(clrSys3dShadow)) + 250L*long(GetRValue(clrSys3dDkShadow)) ) / 1000L,
				( 750L*long(GetGValue(clrSys3dShadow)) + 250L*long(GetGValue(clrSys3dDkShadow)) ) / 1000L,
				( 750L*long(GetBValue(clrSys3dShadow)) + 250L*long(GetBValue(clrSys3dDkShadow)) ) / 1000L
				);
	} // if( nBitsPerPixel > 8 )

	m_mapColorTranslate[CLR_3DFACE_OUT]				= InstallColor( xpclr_ControlBarBk );
	m_mapColorTranslate[CLR_3DFACE_IN]				= InstallColor( xpclr_Highlight );
	m_mapColorTranslate[CLR_3DFACE_DISABLED]		= COLOR_3DFACE;

int idxClrDark = InstallColor( xpclr_PanelBorder );

	m_mapColorTranslate[CLR_3DLIGHT_OUT]			= idxClrDark;
	m_mapColorTranslate[CLR_3DLIGHT_IN]				= idxClrDark;
	m_mapColorTranslate[CLR_3DLIGHT_DISABLED]		= idxClrDark;

	m_mapColorTranslate[CLR_3DHILIGHT_OUT]			= idxClrDark;
	m_mapColorTranslate[CLR_3DHILIGHT_IN]			= idxClrDark;
	m_mapColorTranslate[CLR_3DHILIGHT_DISABLED]		= idxClrDark;

	m_mapColorTranslate[CLR_3DSHADOW_OUT]			= idxClrDark;
	m_mapColorTranslate[CLR_3DSHADOW_IN]			= idxClrDark;
	m_mapColorTranslate[CLR_3DSHADOW_DISABLED]		= idxClrDark;

	m_mapColorTranslate[CLR_3DDKSHADOW_OUT]			= idxClrDark;
	m_mapColorTranslate[CLR_3DDKSHADOW_IN]			= idxClrDark;
	m_mapColorTranslate[CLR_3DDKSHADOW_DISABLED]	= idxClrDark;
	
	m_mapColorTranslate[CLR_TEXT_OUT]				= COLOR_BTNTEXT;
	m_mapColorTranslate[CLR_TEXT_IN] =
		InstallColor(
			(nBitsPerPixel > 8)
				? RGB(0,0,0)
				: clrSys3dHilight
			);
	m_mapColorTranslate[CLR_TEXT_DISABLED]			= COLOR_3DSHADOW;

	m_mapColorTranslate[XPCLR_3DFACE_DARK]			= InstallColor( xpclr_ControlBarBk );
	m_mapColorTranslate[XPCLR_3DFACE_NORMAL]		= InstallColor( xpclr_MenuLight );
	m_mapColorTranslate[XPCLR_SEPARATOR]			= InstallColor( xpclr_Separator ); 

	m_mapColorTranslate[XPCLR_HILIGHT]				= InstallColor( xpclr_HighlightDarked ); 

	m_mapColorTranslate[XPCLR_HILIGHT_BORDER]		= InstallColor( xpclr_HighlightBorder ); 
	
	m_mapColorTranslate[XPCLR_RARELY_BORDER]		= InstallColor( xpclr_RarelyUsedMenuLeft );
}

void CExtPaintManagerXP::PaintPushButton(
	CDC & dc,
	bool bHorz,
	const CRect & rectClient,
	LPCTSTR sText,
	HICON hIcon,
	bool bFlat,
	bool bHover,
	bool bPushed,
	bool bIndeterminate,
	bool bEnabled,
	bool bDrawBorder,
	bool bDrawFocusRect,
	bool bDefaultPushButton,
	int eAlign,
	CFont * pFont, // = NULL
	bool bDropDown, // = false
	UINT nHelperSysCmdID, // = 0
	bool bTransparentBackground // = false
	)
{
	if( rectClient.IsRectEmpty() )
		return;

	if( !bEnabled )
	{
		bHover = false;
		//bDrawBorder = false;
		bDrawFocusRect = false;
		bIndeterminate = false;
	}

COLORREF clrOldText = dc.GetTextColor();
COLORREF clrOldBk = dc.GetBkColor();
int nOldBkMode = dc.SetBkMode( TRANSPARENT );

	// Prepare draw... paint button's area with background color
COLORREF bgColor;
	if(	bHover || bPushed )
	{
		if( bPushed )
			bgColor = GetColor( bHover ? XPCLR_HILIGHT : CLR_3DFACE_IN);
		else
			bgColor = GetColor( CLR_3DFACE_IN );
	}
	else
		bgColor = GetColor(CLR_3DFACE_OUT);

	if( !bTransparentBackground )
	{
		CBrush br(bgColor);
		dc.FillRect(&rectClient, &br);
	} // if( !bTransparentBackground )

COLORREF clrDkBorder =
		GetColor(XPCLR_HILIGHT_BORDER);
	if(bHover || bPushed)
	{
		dc.Draw3dRect(
			&rectClient,
			clrDkBorder,
			clrDkBorder
			);
		/*
		HBRUSH hOldBrush = (HBRUSH)
			::SelectObject(
				dc.GetSafeHdc(),
				::GetStockObject(NULL_BRUSH)
				);
		COLORREF clrOld = dc.SetTextColor(
			clrDkBorder
			);
		dc.Rectangle(
			&rectClient
			);
		dc.SetTextColor( clrOld );
		if( hOldBrush )
			::SelectObject(
				dc.GetSafeHdc(),
				hOldBrush
				);
		*/
	}

CString sTitle( _T("") );
	if( sText != NULL )
		sTitle = sText;

CRect rectClient2( rectClient );
	if( bDropDown )
	{
		CRect rcDopArea( rectClient2 );
		COLORREF ColorValues[2] =
		{
			RGB(0,0,0),
			GetColor(
				bEnabled
					? (bHover ? CLR_TEXT_IN : CLR_TEXT_OUT)
					: CLR_3DSHADOW_OUT
				)
		};
		if( bHorz )
		{
			rectClient2.right -=
				__DROPDOWN_ARROW_GAP*2
				+ g_glyph_btn_expand_bottom.Size().cx
				;
			rcDopArea.left = rectClient2.right;
			rcDopArea.DeflateRect(1,1);
			rcDopArea.OffsetRect(-1,0);
			if( bPushed )
				rcDopArea.OffsetRect(
					GetPushedOffset()
					);
			PaintGlyphCentered(
				dc,
				rcDopArea,
				g_glyph_btn_expand_bottom,
				ColorValues
				);
		} // if( bHorz )
		else
		{
			rectClient2.top +=
				__DROPDOWN_ARROW_GAP*2
				+ g_glyph_btn_expand_right.Size().cy
				;
			rcDopArea.bottom = rectClient2.top;
			rcDopArea.DeflateRect(1,1);
			rcDopArea.OffsetRect(0,1);
			if( bPushed )
				rcDopArea.OffsetRect(
					GetPushedOffset()
					);
			PaintGlyphCentered(
				dc,
				rcDopArea,
				g_glyph_btn_expand_right,
				ColorValues
				);
		} // else from if( bHorz )
	} // if( bDropDown )

CRect rectCaption = rectClient2; //lpDIS->rcItem;
	// Draw the icon
	if(	rectClient2.bottom > rectClient2.top
		&& rectClient2.right > rectClient2.left
		)
	{
		if( hIcon != NULL )
		{
			PaintIcon(
				dc,
				bHorz,
				sTitle,
				hIcon,
				rectClient2,
				rectCaption,
				bPushed,
				bEnabled,
				bHover,
				eAlign
				);
		} // if( hIcon != NULL )
		else
		{
			PaintSysCmdGlyph(
				dc,
				nHelperSysCmdID,
				rectClient2,
				false,
				bPushed,
				bEnabled
				);
		}
	}

	if( rectCaption.bottom > rectCaption.top
		&& rectCaption.right > rectCaption.left
		)
	{ // if we have valid area for text & focus rect
		if( !sTitle.IsEmpty() )
		{
			if( bPushed )
			{
				CSize sizePushedOffset = GetPushedOffset();
				rectCaption.OffsetRect(sizePushedOffset);
			}
			dc.SetBkColor( bgColor );

			if( bEnabled ) 
			{
				dc.SetTextColor(
					GetColor(
						(bHover || bPushed) ?
							CLR_TEXT_IN : CLR_TEXT_OUT
						)
					);
			} 
			else 
			{
				dc.SetTextColor(
					GetColor(COLOR_3DSHADOW)
					);
			}

/*
			// center text
			CRect centerRect = rectCaption;
*/

			CFont * pOldBtnFont = NULL;
			CFont * pCurrFont = NULL;
			CFont fontDummy;

			if( pFont != NULL )
			{
				ASSERT( pFont->GetSafeHandle() != NULL );
				if( bHorz )
					pCurrFont = pFont;
				else
				{
					LOGFONT lf;
					::memset(&lf,0,sizeof(LOGFONT));
					pFont->GetLogFont(&lf);
					if( lf.lfEscapement == 0 )
					{
						lf.lfEscapement = __VERT_FONT_ESCAPEMENT__;
						VERIFY(
							fontDummy.CreateFontIndirect(&lf)
							);
						pCurrFont = &fontDummy;
					}
					else
					{
						// suppose font already prepared
						pCurrFont = pFont;
					}
				}
			} // if( pFont != NULL )
			else
			{
				if( bHorz )
				{
					if( bDefaultPushButton )
						pCurrFont = &m_FontBold;
					else
						pCurrFont = &m_FontNormal;
				}
				else
				{
					if( bDefaultPushButton )
						pCurrFont = &m_FontBoldVert;
					else
						pCurrFont = &m_FontNormalVert;
				}
			} // else from if( pFont != NULL )
			ASSERT( pCurrFont != NULL );
			pOldBtnFont = dc.SelectObject( pCurrFont );
			ASSERT( pOldBtnFont != NULL );

			int nTextLength = sTitle.GetLength();
			int nAmpIndex = sTitle.Find( _T('&') );
			CString sBtn;
			if( nAmpIndex < 0 )
				sBtn = sTitle;
			else
				sBtn =
					sTitle.Left(nAmpIndex)
					+ sTitle.Right(nTextLength - (nAmpIndex+1)
					);
			ASSERT( sBtn.Find( _T('&') ) < 0 );

			CRect rcText(0,0,0,0);
			dc.DrawText(
				sBtn,
				-1,
				rcText,
				DT_SINGLELINE|DT_CALCRECT
				);
			CRect rcCenteredCaption(rectCaption);
			rcCenteredCaption.OffsetRect(
				(rectCaption.Width() - rcText.Width())/2,
				(rectCaption.Height() - rcText.Height())/2
				);

			if( bHorz )
			{
				dc.DrawText(
					sTitle,
					-1,
					rectCaption,
					DT_SINGLELINE|DT_CENTER|DT_VCENTER
					);
			}
			else
			{
				///int iOldAdvalncedGraphicsMode = // fix on NT
				///	::GetGraphicsMode(dc.GetSafeHdc());
				///::SetGraphicsMode(dc.GetSafeHdc(), GM_ADVANCED);

				LOGFONT lf;
				::memset(&lf,0,sizeof(LOGFONT));
				pCurrFont->GetLogFont(&lf);
				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin =
					(rectCaption.Width() - _cyHorzFont)  / 2
					;

				CPoint
					ptLineFrom(0,0),
					ptLineTo(0,0);
				if( nAmpIndex >= 0 )
				{
					ptLineFrom =
						CPoint(
							_cyTextMargin,
							CXTEXTMARGIN
								+ stat_CalcTextWidth(
									dc,
									bDefaultPushButton ?
										m_FontBold : m_FontNormal,
									sBtn.Left(nAmpIndex)
									)
							);
					ptLineTo =
						CPoint(
							_cyTextMargin,
							CXTEXTMARGIN
								+ stat_CalcTextWidth(
									dc,
									bDefaultPushButton ?
										m_FontBold : m_FontNormal,
									sBtn.Left(nAmpIndex+1)
									)
							);
				} // if( nAmpIndex >= 0 )

				CRect rcString = 
					CRect(
						CPoint(
							rectCaption.right - _cyTextMargin,
							rectCaption.top + CXTEXTMARGIN
							),
						rectCaption.Size() //m_sizeHorz
						);
				rcString.DeflateRect(2,2);

				CSize ptFix(-rcString.Width()-1,0);
				ptLineFrom += rcString.TopLeft() + ptFix;
				ptLineTo += rcString.TopLeft() + ptFix;
				if( bEnabled )
				{
					dc.DrawText(
						sBtn,
						rcString,
						DT_SINGLELINE //|DT_CENTER|DT_VCENTER
							|DT_NOCLIP|DT_NOPREFIX
						); // don't forget DT_NOCLIP
					if( nAmpIndex >= 0 )
					{
						CPen pen(
							PS_SOLID,
							0,
							dc.GetTextColor()
							);
						CPen * pOldPen = dc.SelectObject( &pen );
						dc.MoveTo( ptLineFrom );
						dc.LineTo( ptLineTo );
						dc.SelectObject( pOldPen );
					} // if( nAmpIndex >= 0 )
				} // if( bEnabled )
				else
				{
//					CPoint ptOffsetPre(1,1);
//					CPoint ptOffsetPost(-1,-1);
//					COLORREF clrText = GetColor(COLOR_3DSHADOW);
//					COLORREF clrShadow = GetColor(COLOR_3DHILIGHT);
//					rcString.OffsetRect(ptOffsetPre.x,ptOffsetPre.y);
//					dc.SetTextColor( clrShadow );
					dc.DrawText(
						sBtn,
						rcString,
						DT_SINGLELINE //|DT_CENTER|DT_VCENTER
							|DT_NOCLIP|DT_NOPREFIX
						); // don't forget DT_NOCLIP
//					rcString.OffsetRect(ptOffsetPost.x,ptOffsetPost.y);
//					dc.SetTextColor( clrText );
//					dc.DrawText(
//						sBtn,
//						rcString,
//						DT_SINGLELINE //|DT_CENTER|DT_VCENTER
//							|DT_NOCLIP|DT_NOPREFIX
//						); // don't forget DT_NOCLIP
					if( nAmpIndex >= 0 )
					{
						CPen pen(
							PS_SOLID,
							0,
							dc.GetTextColor() // clrShadow
							);
						CPen * pOldPen = dc.SelectObject( &pen );
//						ptLineFrom += ptOffsetPre;
//						ptLineTo += ptOffsetPre;
						dc.MoveTo( ptLineFrom );
						dc.LineTo( ptLineTo );
//						dc.SelectObject( pOldPen );
//						pen.DeleteObject();
//						pen.CreatePen(
//							PS_SOLID,
//							0,
//							clrText
//							);
//						dc.SelectObject( &pen );
//						ptLineFrom += ptOffsetPost;
//						ptLineTo += ptOffsetPost;
//						dc.MoveTo( ptLineFrom );
//						dc.LineTo( ptLineTo );
						dc.SelectObject( pOldPen );
					} // if( nAmpIndex >= 0 )
				} // else from if( bEnabled )

				///::SetGraphicsMode( dc.GetSafeHdc(), iOldAdvalncedGraphicsMode );
			}

			dc.SelectObject(pOldBtnFont);
		} // if( !sTitle.IsEmpty() )

		if( bDrawFocusRect )
		{
			CRect focusRect = rectCaption; // rectClient
			focusRect.DeflateRect( 3, 3 );
			dc.DrawFocusRect(&focusRect);
		} // if( bDrawFocusRect )
	} // if we have valid area for text & focus rect


	dc.SetBkMode( nOldBkMode );
	dc.SetBkColor( clrOldBk );
	dc.SetTextColor( clrOldText );
}

void CExtPaintManagerXP::PaintIcon(
	CDC & dc,
	bool bHorz,
	CString & sTitle,
	HICON hIcon,
	const CRect & rectItem,
	CRect & rectCaption,
	bool bPushed,
	bool bEnabled,
	bool bHover,
	int eAlign
	)
{
	rectCaption = rectItem;
	if( hIcon == NULL )
		return;
ICONINFO ii;
	// Get icon dimension
	ZeroMemory(&ii, sizeof(ICONINFO));
	::GetIconInfo(hIcon, &ii);
BYTE cxIcon = (BYTE)(ii.xHotspot * 2);
BYTE cyIcon = (BYTE)(ii.yHotspot * 2);
    ::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);
/*
BYTE cxSysIconSmall = (BYTE)::GetSystemMetrics(SM_CXSMICON);
BYTE cySysIconSmall = (BYTE)::GetSystemMetrics(SM_CYSMICON);
	cxIcon = min( cxIcon, cxSysIconSmall );
	cyIcon = min( cyIcon, cxSysIconSmall );
	cxIcon = min( cxIcon, 16 );
	cyIcon = min( cyIcon, 16 );
*/

CRect iconRect = rectItem;
CRect btnRect;
	if( bHorz )
	{
		if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		{
			if( sTitle.IsEmpty() )
				// Center the icon horizontally
				iconRect.left += ((iconRect.Width() - cxIcon)/2);
			else
			{
				iconRect.left += 3;  
				rectCaption.left += cxIcon + 3;
			}
		} // if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		else
		{
			if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
			{
				btnRect = rectCaption;
				if( sTitle.IsEmpty() )
					// Center the icon horizontally
					iconRect.left += ((iconRect.Width() - cxIcon)/2);
				else
				{
					rectCaption.right = rectCaption.Width() - cxIcon - 3;
					rectCaption.left = 3;
					iconRect.left = btnRect.right - cxIcon - 3;
					// Center the icon vertically
					iconRect.top += ((iconRect.Height() - cyIcon)/2);
				}
			} // if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
		} // else from if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		if( (eAlign&__ALIGN_VERT) != 0 )
		{
			// Center the icon horizontally
			iconRect.top += ((iconRect.Height() - cyIcon)/2);
		} // if( (eAlign&__ALIGN_VERT) != 0 )
	} // if( bHorz )
	else
	{
		if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		{
			if( sTitle.IsEmpty() )
				// Center the icon horizontally
				iconRect.top += ((iconRect.Height() - cyIcon)/2);
			else
			{
				iconRect.top += 3;  
				rectCaption.top += cyIcon + 3;
			}
		} // if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		else
		{
			if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
			{
				btnRect = rectCaption;
				if( sTitle.IsEmpty() )
					// Center the icon horizontally
					iconRect.top += ((iconRect.Height() - cyIcon)/2);
				else
				{
					rectCaption.top = rectCaption.Height() - cyIcon - 3;
					rectCaption.top = 3;
					iconRect.top = btnRect.top - cyIcon - 3;
					// Center the icon vertically
					iconRect.left += ((iconRect.Width() - cxIcon)/2);
				}
			} // if( (eAlign&__ALIGN_HORIZ_RIGHT) != 0 )
		} // else from if( (eAlign&__ALIGN_HORIZ_CENTER) != 0 )
		if( (eAlign&__ALIGN_VERT) != 0 )
		{
			// Center the icon horizontally
			iconRect.left += ((iconRect.Width() - cxIcon)/2);
		} // if( (eAlign&__ALIGN_VERT) != 0 )
	} // else from if( bHorz )
    
	// If button is pressed then press the icon also
	if( bPushed )
	{
		CSize sizePushedOffset = GetPushedOffset();
		iconRect.OffsetRect(sizePushedOffset.cx, sizePushedOffset.cy);
	}

CPoint ptTopLeft = iconRect.TopLeft()+CSize(1,1);
	if( bEnabled )
	{
		CPoint ptOffs(0,0);
		if( bHover && (!bPushed) )
		{
			CBrush brBack;
			if( !brBack.CreateSolidBrush(GetColor(COLOR_3DDKSHADOW)) )
			{
				ASSERT(FALSE);
				return;
			}
			dc.DrawState(
				ptTopLeft,
				iconRect.Size(), 
				hIcon, 
				__XP_DSS_MONO, 
				&brBack
				);
			ptOffs.x = ptOffs.y = -1;
		}
		dc.DrawState(
			ptTopLeft + ptOffs,
			iconRect.Size(), 
			hIcon, 
			bEnabled ?
				DSS_NORMAL : m_DSS_DISABLED_style, 
			(CBrush*)NULL
			);
	}
	else
	{
		CBrush brDisabled;
		if( !brDisabled.CreateSolidBrush(GetColor(XPCLR_SEPARATOR)) )
		{
			ASSERT(FALSE);
			return;
		}
		dc.DrawState(
			ptTopLeft,
			iconRect.Size(), 
			hIcon, 
			__XP_DSS_MONO, 
			&brDisabled
			);
	}
}

void CExtPaintManagerXP::PaintComboFrame(
	CDC &dc,
	CRect & rectClient,
	bool bHover,
	bool bPushed,
	bool bEnabled
	)
{
CRect rectClient2( rectClient );

/*
COLORREF clrTopLeft, clrBottomRight;
	clrTopLeft = clrBottomRight =
		GetColor( CLR_3DFACE_OUT );
	if( bPushed || bHover )
	{
		clrTopLeft =
			GetColor( CLR_3DSHADOW_IN );
		clrBottomRight =
			GetColor( CLR_3DHILIGHT_IN );
	}

COLORREF clrHilightOut =	GetColor( CLR_3DHILIGHT_OUT );
COLORREF clrShadowIn =		GetColor( CLR_3DSHADOW_IN );
COLORREF clrHilightIn =		GetColor( CLR_3DHILIGHT_IN );
COLORREF clrFaceOut =		GetColor( CLR_3DFACE_OUT );
*/

int nSavedDC = dc.SaveDC();

COLORREF clrWindow =
		GetColor(
			bEnabled ? COLOR_WINDOW : COLOR_3DFACE
			);
COLORREF clrFaceOut = GetColor( CLR_3DFACE_OUT );
COLORREF clrHilight = GetColor( XPCLR_3DFACE_NORMAL );
COLORREF clrShadow   = GetColor( CLR_3DFACE_OUT );

	// Cover up dark 3D shadow.
	dc.Draw3dRect(
		rectClient,
		clrFaceOut,
		clrFaceOut
		);
	rectClient.DeflateRect(1,1);
	dc.Draw3dRect(
		rectClient,
		clrHilight,
		clrHilight
		);

	// Cover up dark 3D shadow on drop arrow.
	rectClient.left =
		rectClient.right -
		::GetSystemMetrics(SM_CXHTHUMB);
	dc.Draw3dRect(
		rectClient,
		clrHilight,
		clrHilight
		);
	rectClient.DeflateRect(1,1);
	dc.Draw3dRect(
		rectClient,
		clrHilight,
		clrHilight
		);

	dc.FillSolidRect(
		rectClient.left-2,
		rectClient.top,
		3,
		rectClient.Height(),
		clrWindow // clrHilight
		);

	if( bHover || bPushed )
		dc.FillSolidRect(
			rectClient,
			clrHilight
			);

	rectClient.InflateRect(1,1);

CFont font;
int ppi = dc.GetDeviceCaps(LOGPIXELSX);
int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi
	font.CreatePointFont(pointsize, _T("Marlett"));

	PaintPushButton(
		dc,
		true,
		rectClient,
		_T("6"),
		NULL,
		true,
		(bHover||bPushed) ? true : false,
		bPushed ? true : false,
		false,
		bEnabled,
		true,
		false,
		false,
		CExtPaintManager::__ALIGN_HORIZ_CENTER
			| CExtPaintManager::__ALIGN_VERT,
		&font // = NULL
		);

	if( !(bHover || bPushed) )
	{
		dc.Draw3dRect(
			rectClient,
			clrHilight,
			clrHilight
			);
	}

	if(bHover || bPushed)
	{
		rectClient2.DeflateRect(1,1);
		HBRUSH hOldBrush = (HBRUSH)
			::SelectObject(
				dc.GetSafeHdc(),
				::GetStockObject(NULL_BRUSH)
				);
		COLORREF clrOld = dc.SetTextColor( clrShadow );
		dc.Rectangle(
			&rectClient2
			);
		dc.SetTextColor( clrOld );
		if( hOldBrush )
			::SelectObject(
				dc.GetSafeHdc(),
				hOldBrush
				);
	}

	dc.RestoreDC( nSavedDC );
}

int CExtPaintManager::GetMenuBorderSize()
{
	return 4;
}

void CExtPaintManager::PaintMenuBorder(
	CDC & dc,
	const CRect & rectClient
	)
{
	if( rectClient.IsRectEmpty() )
		return;
CRect rc(rectClient);
	dc.Draw3dRect(
		&rc,
		GetColor(CLR_3DFACE_OUT),
		GetColor(CLR_3DDKSHADOW_OUT)
		);
	rc.DeflateRect(1,1);
	dc.Draw3dRect(
		&rc,
		GetColor(CLR_3DHILIGHT_OUT),
		GetColor(CLR_3DSHADOW_OUT)
		);
COLORREF clrFace = GetColor(CLR_3DFACE_OUT);
	rc.DeflateRect(1,1);
	dc.Draw3dRect(&rc,clrFace,clrFace);
	rc.DeflateRect(1,1);
	dc.Draw3dRect(&rc,clrFace,clrFace);
}

int CExtPaintManager::GetMenuShadowSize()
{
	return 5;
}

int CExtPaintManagerXP::GetMenuShadowSize()
{
	return 5;
}

int CExtPaintManagerXP::GetMenuBorderSize()
{
	return 2;
}

void CExtPaintManagerXP::PaintMenuBorder(
	CDC & dc,
	const CRect & rectClient
	)
{
	if( rectClient.IsRectEmpty() )
		return;
CRect rc(rectClient);
COLORREF clr =
		GetColor(
			COLOR_3DDKSHADOW
			);
	dc.Draw3dRect(&rc,clr,clr);
COLORREF clrFace = GetColor(XPCLR_3DFACE_NORMAL);
	rc.DeflateRect(1,1);
	dc.Draw3dRect(&rc,clrFace,clrFace);
	rc.right--;
	rc.bottom--;
	dc.Draw3dRect(&rc,clrFace,clrFace);
}

bool CExtPaintManager::IsMenuMustCombineExcludeArea()
{
//
// some not funny implementadion of
// CExtPaintManager::PaintMenuCombinedArea() provided,
// but offices 98/2k don't combine menu area
// with exclude area
//
// interesting ? uncomment and play with it !
//
//	return true; 
//
	return false;
}

bool CExtPaintManagerXP::IsMenuMustCombineExcludeArea()
{
	return true;
}

void CExtPaintManager::PaintMenuCombinedArea(
	CDC & dc,
	const CRect & rcArea,
	const CRect & rcClient,
	int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
	)
{
	if( rcArea.IsRectEmpty() )
		return;
	dc.SelectClipRgn( NULL );
	ASSERT( eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE );
int nExpandSize =
		GetMenuBorderSize() - 1;
CRect _rcClient( rcClient );
CRect _rcArea( rcArea );
	_rcArea.InflateRect(
		(eCombineAlign==CExtPopupMenuWnd::__CMBA_RIGHT) ?
			nExpandSize : 0,
		(eCombineAlign==CExtPopupMenuWnd::__CMBA_BOTTOM) ?
			nExpandSize : 0,
		(eCombineAlign==CExtPopupMenuWnd::__CMBA_LEFT) ?
			nExpandSize : 0,
		(eCombineAlign==CExtPopupMenuWnd::__CMBA_TOP) ?
			nExpandSize : 0
		);
	dc.FillSolidRect(
		&_rcArea,
		GetColor( CLR_3DFACE_OUT )
		);
//	_rcArea = rcArea;

COLORREF clrMenuBorderLTo =
		GetColor( CLR_3DFACE_OUT );
COLORREF clrMenuBorderLTi =
		GetColor( CLR_3DHILIGHT_OUT );
COLORREF clrMenuBorderRBo =
		GetColor( CLR_3DSHADOW_OUT );
COLORREF clrMenuBorderRBi =
		GetColor( CLR_3DDKSHADOW_OUT );

	switch( eCombineAlign )
	{
	case CExtPopupMenuWnd::__CMBA_TOP:
		_rcArea.InflateRect( 0, 0, 0, nExpandSize );
		_rcClient.InflateRect( -nExpandSize, 0 );
	break;
	case CExtPopupMenuWnd::__CMBA_BOTTOM:
		_rcArea.InflateRect( 0, nExpandSize, 0, 0 );
		_rcClient.InflateRect( -nExpandSize, 0 );
	break;
	case CExtPopupMenuWnd::__CMBA_LEFT:
		_rcArea.InflateRect( 0, 0, nExpandSize, 0 );
		_rcClient.InflateRect( 0, -nExpandSize );
	break;
	case CExtPopupMenuWnd::__CMBA_RIGHT:
		_rcArea.InflateRect( nExpandSize, 0, 0, 0 );
		_rcClient.InflateRect( 0, -nExpandSize );
	break;
#ifdef _DEBUG
	default:
		ASSERT(FALSE);
	break;
#endif // _DEBUG
	} // switch( eCombineAlign )

	dc.ExcludeClipRect( &_rcClient );

int cx = _rcArea.Width();
int cy = _rcArea.Height();
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_BOTTOM )
		dc.FillSolidRect(_rcArea.left, _rcArea.top, cx - 1, 1, clrMenuBorderLTo);
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_RIGHT )
		dc.FillSolidRect(_rcArea.left, _rcArea.top, 1, cy - 1, clrMenuBorderLTo);
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_LEFT )
		dc.FillSolidRect(_rcArea.left + cx, _rcArea.top, -1, cy, clrMenuBorderRBo);
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_TOP )
		dc.FillSolidRect(_rcArea.left, _rcArea.top + cy, cx, -1, clrMenuBorderRBo);
	_rcArea.DeflateRect(1,1,1,1);
	cx = _rcArea.Width();
	cy = _rcArea.Height();
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_BOTTOM )
		dc.FillSolidRect(_rcArea.left, _rcArea.top, cx - 1, 1, clrMenuBorderLTi);
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_RIGHT )
		dc.FillSolidRect(_rcArea.left, _rcArea.top, 1, cy - 1, clrMenuBorderLTi);
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_LEFT )
		dc.FillSolidRect(_rcArea.left + cx, _rcArea.top, -1, cy, clrMenuBorderRBi);
	if( eCombineAlign != CExtPopupMenuWnd::__CMBA_TOP )
		dc.FillSolidRect(_rcArea.left, _rcArea.top + cy, cx, -1, clrMenuBorderRBi);
	
/*
	dc.ExcludeClipRect( &_rcClient );
	dc.Draw3dRect(
		&_rcArea,
		clrMenuBorderLTo,
		clrMenuBorderRBo
		);
	_rcArea.DeflateRect(1,1,1,1);
	dc.Draw3dRect(
		&_rcArea,
		clrMenuBorderLTi,
		clrMenuBorderRBi
		);
*/
	dc.SelectClipRgn( NULL );
}

void CExtPaintManagerXP::PaintMenuCombinedArea(
	CDC & dc,
	const CRect & rcArea,
	const CRect & rcClient,
	int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
	)
{
	if( rcArea.IsRectEmpty() )
		return;
	ASSERT( eCombineAlign != CExtPopupMenuWnd::__CMBA_NONE );
CRect _rcArea( rcArea );
CRect _rcClient( rcClient );
	dc.FillSolidRect(
		&_rcArea,
		GetColor(
			XPCLR_3DFACE_DARK //XPCLR_3DFACE_NORMAL
			)
		);
COLORREF clrMenuBorder =
		GetColor( COLOR_3DDKSHADOW );
	switch( eCombineAlign )
	{
	case CExtPopupMenuWnd::__CMBA_TOP:
	case CExtPopupMenuWnd::__CMBA_BOTTOM:
		_rcClient.InflateRect( -1, 0 );
	break;
	case CExtPopupMenuWnd::__CMBA_LEFT:
	case CExtPopupMenuWnd::__CMBA_RIGHT:
		_rcClient.InflateRect( 0, -1 );
	break;
#ifdef _DEBUG
	default:
		ASSERT(FALSE);
	break;
#endif // _DEBUG
	} // switch( eCombineAlign )
	dc.ExcludeClipRect( &_rcClient );
	dc.Draw3dRect(
		&_rcArea,
		clrMenuBorder,
		clrMenuBorder
		);
	dc.SelectClipRgn( NULL );
}

void CExtPaintManager::PaintControlBarBordersDefImpl(
	e_control_bar_border_type_t eCBBT,
	DWORD dwStyle,
	CDC & dc,
	CRect & rc,
	COLORREF clrHi,
	COLORREF clrLo
	)
{
	if( !(dwStyle & CBRS_BORDER_ANY) )
		return;

	// prepare for dark lines
//	ASSERT( rc.top == 0 && rc.left == 0 );
CRect rc1( rc ), rc2( rc );
COLORREF clr = clrLo;
//		afxData.bWin4
//			? afxData.clrBtnShadow
//			: afxData.clrWindowFrame;
		g_PaintManager->GetColor(
			CExtPaintManager::CLR_3DFACE_OUT
			);

	// draw dark line one pixel back/up
	if( dwStyle & CBRS_BORDER_3D )
	{
		rc1.right -= CX_BORDER;
		rc1.bottom -= CY_BORDER;
	}
	if( dwStyle & CBRS_BORDER_TOP )
		rc2.top += afxData.cyBorder2;
	if( dwStyle & CBRS_BORDER_BOTTOM )
		rc2.bottom -= afxData.cyBorder2;

	// draw left and top
	if( dwStyle & CBRS_BORDER_LEFT )
		dc.FillSolidRect(
			0, rc2.top, CX_BORDER, rc2.Height(),
			clr
			);
	if( dwStyle & CBRS_BORDER_TOP )
		dc.FillSolidRect(
			0, 0, rc.right, CY_BORDER,
			clr
			);

	// draw right and bottom
	if( dwStyle & CBRS_BORDER_RIGHT )
		dc.FillSolidRect(
			rc1.right, rc2.top, -CX_BORDER, rc2.Height(),
			clr
			);
	if( dwStyle & CBRS_BORDER_BOTTOM )
		dc.FillSolidRect(
			0, rc1.bottom, rc.right, -CY_BORDER,
			clr
			);

	if( dwStyle & CBRS_BORDER_3D )
	{
		// prepare for hilite lines
//		clr = afxData.clrBtnHilite;
		clr = clrHi;

		// draw left and top
		if( dwStyle & CBRS_BORDER_LEFT )
			dc.FillSolidRect(
				1, rc2.top, CX_BORDER, rc2.Height(),
				clr
				);
		if( dwStyle & CBRS_BORDER_TOP )
			dc.FillSolidRect(
				0, 1, rc.right, CY_BORDER,
				clr
				);

		// draw right and bottom
		if( dwStyle & CBRS_BORDER_RIGHT )
			dc.FillSolidRect(
				rc.right, rc2.top, -CX_BORDER, rc2.Height(),
				clr
				);
		if( dwStyle & CBRS_BORDER_BOTTOM )
			dc.FillSolidRect(
				0, rc.bottom, rc.right, -CY_BORDER,
				clr
				);
	}

	if( dwStyle & CBRS_BORDER_LEFT )
		rc.left += afxData.cxBorder2;
	if( dwStyle & CBRS_BORDER_TOP )
		rc.top += afxData.cyBorder2;
	if( dwStyle & CBRS_BORDER_RIGHT )
		rc.right -= afxData.cxBorder2;
	if( dwStyle & CBRS_BORDER_BOTTOM )
		rc.bottom -= afxData.cyBorder2;
}

void CExtPaintManager::PaintControlBarBorders(
	e_control_bar_border_type_t eCBBT,
	DWORD dwStyle,
	CDC & dc,
	CRect & rc
	)
{
	if( eCBBT == __CB_INNER_STATUSBAR_ITEM )
	{
		dc.Draw3dRect(
			&rc,
			GetColor( CLR_3DSHADOW_OUT ),
			GetColor( CLR_3DHILIGHT_OUT )
			);
		return;
	}
COLORREF clr = GetColor( CLR_3DFACE_OUT );
	PaintControlBarBordersDefImpl(
		eCBBT, dwStyle, dc, rc, clr, clr
		);
}

void CExtPaintManagerXP::PaintControlBarBorders(
	e_control_bar_border_type_t eCBBT,
	DWORD dwStyle,
	CDC & dc,
	CRect & rc
	)
{
	if( eCBBT == __CB_INNER_STATUSBAR_ITEM )
	{
		COLORREF clr = GetColor( COLOR_3DHILIGHT );
		dc.Draw3dRect(
			&rc,
			clr,
			clr
			);
		return;
	}
COLORREF clr = GetColor( CLR_3DFACE_OUT );
	PaintControlBarBordersDefImpl(
		eCBBT, dwStyle, dc, rc, clr, clr
		);
}

