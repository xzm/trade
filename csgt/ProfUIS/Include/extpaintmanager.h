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

#if (!defined __EXT_PAINT_MANAGER_H)
#define __EXT_PAINT_MANAGER_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if (!defined __AFXTEMPL_H__)
	#include <AfxTempl.h>
#endif

#if (!defined __DEFAULT_PAINT_MANAGER_CLASS)
	#define __DEFAULT_PAINT_MANAGER_CLASS CExtPaintManagerXP
#endif

#define __ExtMfc_MIN_SYS_COLOR_VAL	0L
#define __ExtMfc_MAX_SYS_COLOR_VAL	49L
#define __ExtMfc_COLOR_MAP_BASE		1000L
#define __ExtMfc_DISABLED_COLOR_SHIFT 1000L
#define __ExtMfc_XP_COLOR_SHIFT (__ExtMfc_DISABLED_COLOR_SHIFT+1000L)

//#define __ExtMfc_MENU_GAP__ 4

#define __DEF_MENU_ICON_CX 16
#define __DEF_MENU_ICON_CY __DEF_MENU_ICON_CX
#define __DEF_MENU_GAP 3 // 4
#define __DEF_MENU_HEIGHT (__DEF_MENU_ICON_CY+__DEF_MENU_GAP*2)
#define __DEF_MENU_SEPARATOR_HEIGHT 4
#define __DEF_MENU_ICON_AREA_DX (__DEF_MENU_ICON_CX+__DEF_MENU_GAP*2)
#define __DEF_MENU_POPUP_ARROW_AREA_DX 16
#define __DEF_MENU_IMG_RECT_OUT_GAP 2

#define __VERT_FONT_ESCAPEMENT__ (-900)

#define __DROPDOWN_ARROW_GAP 3

class __PROF_UIS_API CExtPaintManager : public CObject
{
	DECLARE_DYNAMIC( CExtPaintManager )
protected:
	// color values container
	typedef CArray < COLORREF, COLORREF & >
		sys_colors_container;
	sys_colors_container m_colors;

	// system brushes container
	typedef CArray < HBRUSH, HBRUSH & >
		sys_brashes_container;

	sys_brashes_container m_brushes;

	// translated colors map
	typedef
		CMap < int, int, int, int >
		translate_color_map;

	translate_color_map m_mapColorTranslate;

public:
	CPalette m_PaletteWide;

	CBrush m_brushLight;

	CFont m_FontNormal,m_FontNormalVert,m_FontNormalVertX;
	CFont m_FontBold,m_FontBoldVert,m_FontBoldVertX;
	CFont m_FontMarlett;

	int m_DSS_DISABLED_style;

	// global auto-pointer class for CExtPaintManager
	class __PROF_UIS_API CExtPaintManagerAutoPtr
	{
		CExtPaintManager * m_pPaintMenager;
		bool InitPaintManagerInstance();
	public:
		CExtPaintManagerAutoPtr();
		~CExtPaintManagerAutoPtr();
		CExtPaintManager * operator->();
		bool InstallPaintManager(
			CExtPaintManager * pPaintMenager
			);
		bool InstallPaintManager(
			CRuntimeClass * pRtcPaintMenager
			);
	}; // class CExtPaintManagerAutoPtr

	// construction/destruction
	CExtPaintManager();
	~CExtPaintManager();

	// init part of color table and system brushes table
	void SyncSysColors();

	// get any system color based brush
	HBRUSH GetBrush(
		int nColorIndex
		);

	// get any system or translated color value
	COLORREF GetColor(
		int nColorIndex
		);
	virtual COLORREF GetMenuFrameFillColor()
	{
		return GetColor(CLR_3DFACE_OUT);
	};

	// install new color
	int InstallColor(
		COLORREF clr,
		int nColorIndex = -1
		);
	
	// install required translated color as
	// described in e_translated_colors_t enumeration
	virtual void InitTranslatedColors();


	// used always by default used translated colors
	enum e_translated_colors_t
	{
		CLR_3DFACE_OUT = __ExtMfc_COLOR_MAP_BASE+0,
		CLR_3DFACE_IN = __ExtMfc_COLOR_MAP_BASE+1,
		CLR_3DFACE_DISABLED = CLR_3DFACE_OUT+__ExtMfc_DISABLED_COLOR_SHIFT,

		CLR_3DLIGHT_OUT = __ExtMfc_COLOR_MAP_BASE+2,
		CLR_3DLIGHT_IN = __ExtMfc_COLOR_MAP_BASE+3,
		CLR_3DLIGHT_DISABLED = CLR_3DLIGHT_OUT+__ExtMfc_DISABLED_COLOR_SHIFT,

		CLR_3DHILIGHT_OUT = __ExtMfc_COLOR_MAP_BASE+4,
		CLR_3DHILIGHT_IN = __ExtMfc_COLOR_MAP_BASE+5,
		CLR_3DHILIGHT_DISABLED = CLR_3DHILIGHT_OUT+__ExtMfc_DISABLED_COLOR_SHIFT,

		CLR_3DSHADOW_OUT = __ExtMfc_COLOR_MAP_BASE+6,
		CLR_3DSHADOW_IN = __ExtMfc_COLOR_MAP_BASE+7,
		CLR_3DSHADOW_DISABLED = CLR_3DSHADOW_OUT+__ExtMfc_DISABLED_COLOR_SHIFT,

		CLR_3DDKSHADOW_OUT = __ExtMfc_COLOR_MAP_BASE+8,
		CLR_3DDKSHADOW_IN = __ExtMfc_COLOR_MAP_BASE+9,
		CLR_3DDKSHADOW_DISABLED = CLR_3DDKSHADOW_OUT+__ExtMfc_DISABLED_COLOR_SHIFT,
		
		CLR_TEXT_OUT = __ExtMfc_COLOR_MAP_BASE+10,
		CLR_TEXT_IN = __ExtMfc_COLOR_MAP_BASE+11,
		CLR_TEXT_DISABLED = CLR_TEXT_OUT+__ExtMfc_DISABLED_COLOR_SHIFT,
	};

	// align types
	enum e_align_t
	{
		__ALIGN_HORIZ_CENTER = 1,
		__ALIGN_HORIZ_RIGHT = 2,
		__ALIGN_VERT = 4
	};

	// gets the pushed metic offset
	virtual CSize GetPushedOffset();

	class __PROF_UIS_API glyph_t
	{
		CSize m_size; // size in pixels
		UINT m_nColors; // count of used indexes (including 0)
			// index 0 - transparent
		const UINT * m_pColorIndexes;
		bool m_bDynamicData;
	public:
		glyph_t()
		{
			_Init();
		};
		glyph_t(
			const SIZE & _size,
			UINT nColors,
			const UINT * pColorIndexes
			)
		{
			m_bDynamicData = false;

			ASSERT( _size.cx > 0 );
			ASSERT( _size.cy > 0 );
			m_size.cx = _size.cx;
			m_size.cy = _size.cy;
			
			ASSERT( nColors > 0 );
			m_nColors = nColors;
			
			ASSERT( pColorIndexes != NULL );
			m_pColorIndexes = pColorIndexes;
		};
		glyph_t(
			int cx,
			int cy,
			UINT nColors,
			const UINT * pColorIndexes
			)
		{
			m_bDynamicData = false;
			
			ASSERT( cx > 0 );
			ASSERT( cy > 0 );
			m_size.cx = cx;
			m_size.cy = cy;
			
			ASSERT( nColors > 0 );
			m_nColors = nColors;
		
			ASSERT( pColorIndexes != NULL );
			m_pColorIndexes = pColorIndexes;
		};
		glyph_t(
			const glyph_t & other,
			UINT nRotateAngle = 0
			)
		{
			_Init();
			_CopyFromOther(other);
#ifdef _DEBUG
			if( other.m_pColorIndexes != NULL )
			{
				ASSERT( m_pColorIndexes != NULL );
				ASSERT( m_pColorIndexes != other.m_pColorIndexes );
				ASSERT( m_bDynamicData );
			}
#endif // _DEBUG
			ASSERT(
				nRotateAngle == 0
				|| nRotateAngle == 90
				|| nRotateAngle == 180
				|| nRotateAngle == 270
				);
			Rotate(nRotateAngle);
		};
		~glyph_t()
		{
			_Done();
		};
		const glyph_t & operator=( const glyph_t & other )
		{
			_CopyFromOther(other);
#ifdef _DEBUG
			if( other.m_pColorIndexes != NULL )
			{
				ASSERT( m_pColorIndexes != NULL );
				ASSERT( m_pColorIndexes != other.m_pColorIndexes );
				ASSERT( m_bDynamicData );
			}
#endif // _DEBUG
			return *this;
		};

		UINT GetColorPixel(
			int x,
			int y
			)
		{
			ASSERT( x >= 0 && x < m_size.cx );
			ASSERT( y >= 0 && y < m_size.cy );
			ASSERT( m_size.cx > 0 );
			ASSERT( m_size.cy > 0 );
			ASSERT( m_nColors > 0 );
			ASSERT( m_pColorIndexes != NULL );
			int nOffset =
				y * m_size.cx + x;
			UINT nValue =
				*( m_pColorIndexes + nOffset );
			return nValue;
		};

		void Rotate(UINT nRotateAngle)
		{
			ASSERT(
				nRotateAngle == 0
				|| nRotateAngle == 90
				|| nRotateAngle == 180
				|| nRotateAngle == 270
				);
			ASSERT( m_bDynamicData );
			ASSERT( m_size.cx > 0 );
			ASSERT( m_size.cy > 0 );
			ASSERT( m_nColors > 0 );
			ASSERT( m_pColorIndexes != NULL );
			if( nRotateAngle == 0 )
				return;
			
			int nBufferSize =
				m_size.cx * m_size.cy;
			ASSERT( nBufferSize > 0 );
			UINT * pNew =
				new/*(std::nothrow)*/ UINT[nBufferSize];
			/*
				(UINT *)::calloc(
					nBufferSize,
					sizeof(UINT)
					);
			*/
			ASSERT( pNew != NULL );
			if( pNew == NULL )
				return;

			int x, y;
			UINT * pOld = (UINT *)m_pColorIndexes;
			switch( nRotateAngle )
			{
			case 90:
			{
				for( y = 0; y<m_size.cy; y++ )
				{
					for( x = 0; x<m_size.cx; x++, pOld++ )
					{
						int nNewOffset =
							x*m_size.cy + (m_size.cy - y - 1);
						ASSERT(
							nNewOffset >= 0
							&&
							nNewOffset < nBufferSize
							);
						UINT * p = pNew + nNewOffset;
						*p = *pOld;
					} // for( x = 0; x<m_size.cx; x++, pOld++ )
				} // for( y = 0; y<m_size.cy; y++ )
				m_size = CSize(m_size.cy,m_size.cx);
			}
			break; // case 90
			case 180:
			{
				for( y = 0; y<m_size.cy; y++ )
				{
					for( x = 0; x<m_size.cx; x++, pOld++ )
					{
						int nNewOffset =
							(m_size.cy-y-1)*m_size.cx
							+ (m_size.cx-x-1);
						ASSERT(
							nNewOffset >= 0
							&&
							nNewOffset < nBufferSize
							);
						UINT * p = pNew + nNewOffset;
						*p = *pOld;
					} // for( x = 0; x<m_size.cx; x++, pOld++ )
				} // for( y = 0; y<m_size.cy; y++ )
			}
			break; // case 180
			case 270:
			{
				for( y = 0; y<m_size.cy; y++ )
				{
					for( x = 0; x<m_size.cx; x++, pOld++ )
					{
						int nNewOffset =
							(m_size.cx-x-1)*m_size.cy + y;
						ASSERT(
							nNewOffset >= 0
							&&
							nNewOffset < nBufferSize
							);
						UINT * p = pNew + nNewOffset;
						*p = *pOld;
					} // for( x = 0; x<m_size.cx; x++, pOld++ )
				} // for( y = 0; y<m_size.cy; y++ )
				m_size = CSize(m_size.cy,m_size.cx);
			}
			break; // case 270
			} // switch( nRotateAngle )
			::memcpy(
				(UINT *)m_pColorIndexes,
				pNew,
				nBufferSize*sizeof(UINT)
				);
			delete [] pNew; // ::free(pNew);
		};
		
		CSize Size() const
		{
			return m_size;
		};
		operator CSize() const
		{
			return Size();
		};
		
		const UINT * GetSurface() const
		{
			ASSERT( m_size.cx > 0 );
			ASSERT( m_size.cy > 0 );
			ASSERT( m_nColors > 0 );
			ASSERT( m_pColorIndexes != NULL );
			return m_pColorIndexes;
		};
		
		UINT GetColorsCount() const
		{
			ASSERT( m_size.cx > 0 );
			ASSERT( m_size.cy > 0 );
			ASSERT( m_nColors > 0 );
			ASSERT( m_pColorIndexes != NULL );
			return m_nColors;
		};

	private:
		void _Init()
		{
			m_size.cx = m_size.cy = 0;
			m_nColors = 1;
			m_pColorIndexes = NULL;
			m_bDynamicData = false;
		};
		void _Done()
		{
			if( m_bDynamicData )
			{
				ASSERT( m_pColorIndexes != NULL );
				delete [] ((UINT *)m_pColorIndexes); // ::free( (void *)m_pColorIndexes );
				m_pColorIndexes = NULL;
			}
			_Init();
		};
		bool _CopyFromOther( const glyph_t & other )
		{
			_Done();

			if( other.m_pColorIndexes != NULL )
			{
				int nBufferSize =
					other.m_size.cx * other.m_size.cy;
				ASSERT( nBufferSize > 0 );
				m_pColorIndexes =
					new/*(std::nothrow)*/ UINT[nBufferSize];
					/*
					(UINT *)::calloc(
						nBufferSize,
						sizeof(UINT)
						);
					*/
				ASSERT( m_pColorIndexes != NULL );
				if( m_pColorIndexes == NULL )
					return false;
				::memcpy(
					(void *)m_pColorIndexes,
					other.m_pColorIndexes,
					nBufferSize*sizeof(UINT)
					);
				m_bDynamicData = true;
			}
			m_size.cx = other.m_size.cx;
			m_size.cy = other.m_size.cy;
			
			m_nColors = other.m_nColors;

			return true;
		};
		void _SetColorPixel(
			int x,
			int y,
			UINT nValue
			)
		{
			ASSERT( x >= 0 && x < m_size.cx );
			ASSERT( y >= 0 && y < m_size.cy );
			ASSERT( m_bDynamicData );
			ASSERT( m_size.cx > 0 );
			ASSERT( m_size.cy > 0 );
			ASSERT( m_nColors > 0 );
			ASSERT( m_pColorIndexes != NULL );
			int nOffset =
				y * m_size.cx + x;
			UINT * ptr =
				(UINT *)m_pColorIndexes + nOffset;
			*ptr = nValue;
		};
	}; // class glyph_t

	static const glyph_t g_glyph_btn_expand_left;
	static const glyph_t g_glyph_btn_expand_right;
	static const glyph_t g_glyph_btn_expand_right2;
	static const glyph_t g_glyph_btn_expand_bottom;
	static const glyph_t g_glyph_btn_expand_bottom2;

	virtual void PaintGlyph(
		CDC & dc,
		POINT ptDest,
		const glyph_t & glyph,
		COLORREF * pColorValues
		);
	virtual void PaintGlyphCentered(
		CDC & dc,
		const RECT & rect,
		const glyph_t & glyph,
		COLORREF * pColorValues
		);

	virtual void PaintToolbarExpandButton(
		CDC & dc,
		const CRect & rectButton,
		bool bHorz, // if false - down
		bool bBarIsCompletelyVisible,
		bool bEnabled,
		bool bPushed,
		bool bHover,
		bool bTransparentBackground = false
		);
	virtual void PaintMenuExpandButton(
		CDC & dc,
		const CRect & rectButton,
		bool bPushed,
		bool bHover
		);

	// statically implemented paint/helper algorithms

	static COLORREF stat_RGBtoCMY( COLORREF clr );
	static COLORREF stat_CMYtoRGB( COLORREF clr );

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
	static double stat_HuetoRGB(double m1, double m2, double h );
	static COLORREF stat_HLStoRGB( double H, double L, double S );
	static void stat_RGBtoHSL( COLORREF rgb, double *H, double *S, double *L );

	static BYTE stat_GetRGBFromHue(float rm1, float rm2, float rh);
	static COLORREF stat_GetRGBFromHLSExtend( double H, double L, double S );

	static COLORREF stat_HLS_Adjust(
		COLORREF clr,
		double percentH = 0.0,
		double percentL = 0.0,
		double percentS = 0.0
		);

	static BOOL stat_PaintParentBk(
		HWND hWnd,
		HDC hDC,
		LPCRECT rectClip = NULL
		);

	static void stat_ExcludeChildAreas(
		HDC hDC,
		HWND hWnd
		);

	static void stat_TransparentBlt(
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
		);

	// generate DIB for UI light brush (when BPP <=8)
	static HBITMAP stat_GenLBDIB( HDC hDC );

	static void stat_PaintGradientRect(
		CDC & dc,
		const CRect & rcPaintGradient,
		COLORREF clrLeft,
		COLORREF clrRight,
		bool bHorz = false,
		UINT nCountOfSteps = 256
		);

	static void stat_PaintRadioDot(
		CDC & dc,
		int x,
		int y,
		COLORREF color
		);

	static void stat_PaintCheckMark(
		CDC & dc,
		int x,
		int y,
		COLORREF color
		);

	static CRect stat_CalcTextDimension(
		CDC & dc,
		CFont & font,
		const CString & strText
		);
	static int stat_CalcTextWidth(
		CDC & dc,
		CFont & font,
		const CString & strText
		);

	// paint methods

	virtual void PaintIcon(
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
		);

	enum e_docking_caption_button_t
	{
		__DCBT_EMPTY = 0,
		__DCBT_CLOSE = 1,
		__DCBT_ARROW_UP = 2,
		__DCBT_ARROW_DOWN = 3,
		__DCBT_ARROW_LEFT = 4,
		__DCBT_ARROW_RIGHT = 5,
		__DCBT_WND_MINIMIZE = 6,
		__DCBT_WND_RESTORE = 7,
		__DCBT_WND_MAXIMIZE = 8,
		__DCBT_WND_CONTEXTHELP = 9,
		__DCBT_WND_KEEP = 10,
		__DCBT_WND_UNKEEP = 11,
		__DCBT_VALUE_MIN = 0,
		__DCBT_VALUE_MAX = 11,
	};
	
	static glyph_t * g_DockingCaptionGlyphs[__DCBT_VALUE_MAX+1];

	virtual void PaintDockingCaptionButton(
		CDC & dc,
		const CRect & rectClient,
		e_docking_caption_button_t eType,
		bool bHover,
		bool bPushed,
		bool bEnabled,
		bool bSideBar,
		bool bBarWndActive
		);

	virtual void PaintSysCmdGlyph(
		CDC & dc,
		UINT nHelperSysCmdID,
		const CRect & rcItem,
		bool bHover,
		bool bPushed,
		bool bEnabled
		);

	virtual void PaintPushButton(
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
		CFont * pFont = NULL,
		bool bDropDown = false,
		UINT nHelperSysCmdID = 0,
		bool bTransparentBackground = false
		);

	virtual void PaintMenuItem(
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
		);
	virtual void PaintMenuSeparator(
		CDC & dc,
		CRect & rectItem,
		bool bRarelyUsed
		);

	virtual void PaintSeparator(
		CDC & dc,
		CRect & rectItem,
		bool bHorz
		);

	virtual void PaintGripper(
		CDC & dc,
		const CRect & rectGripper,
		const CRect & rectText,
		bool bActive,
		bool bFloating,
		bool bHorz,
		bool bSideBar = false,
		LPCTSTR sCaption = NULL
		);

	virtual void PaintDockingFrame(
		CDC & dc,
		const CRect & rectWindow,
		const CRect & rectClient,
		bool bFloating,
		bool bExtBar
		);

	virtual void PaintControlFrame(
		CDC & dc,
		CRect & rectClient,
		bool bFlat,
		bool bHover,
		bool bEnabled,
		bool bFocus
		);

	virtual void PaintComboFrame(
		CDC &dc,
		CRect & rectClient,
		bool bHover,
		bool bPushed,
		bool bEnabled
		);

	static int stat_GetBPP();

	CSize GetTextSizes(bool bBold = false);
	virtual int GetTextHeight(bool bHorz);

	virtual int GetMenuShadowSize();
	virtual int GetMenuBorderSize();
	virtual void PaintMenuBorder(
		CDC & dc,
		const CRect & rectClient
		);

	virtual bool IsMenuMustCombineExcludeArea();
	virtual void PaintMenuCombinedArea(
		CDC & dc,
		const CRect & rcArea,
		const CRect & rcClient,
		int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
		);

	enum e_control_bar_border_type_t
	{
		__CB_OUTER_DOCKBAR = 0,
		__CB_OUTER_STATUSBAR = 1,
		__CB_INNER_STATUSBAR_ITEM = 2,
	};
	
	void PaintControlBarBordersDefImpl(
		e_control_bar_border_type_t eCBBT,
		DWORD dwStyle,
		CDC & dc,
		CRect & rc,
		COLORREF clrHi,
		COLORREF clrLo
		);

	virtual void PaintControlBarBorders(
		e_control_bar_border_type_t eCBBT,
		DWORD dwStyle,
		CDC & dc,
		CRect & rc
		);

}; // class CExtPaintManager

class __PROF_UIS_API CExtPaintManagerXP : public CExtPaintManager
{
public:
	DECLARE_DYNAMIC( CExtPaintManagerXP )
private:
	// install required translated color as
	// described in e_translated_colors_t enumeration
	virtual void InitTranslatedColors();

	// gets the pushed metic offset
	virtual CSize GetPushedOffset();

	// paint methods

	virtual void PaintPushButton(
		CDC & dc,
		bool bHorz,
		const CRect & rectClient,
		LPCTSTR sText,
		HICON hIcon,
		bool bFlat,
		bool bHover,
		bool bPushed,
		bool bEnabled,
		bool bIndeterminate,
		bool bDrawBorder,
		bool bDrawFocusRect,
		bool bDefaultPushButton,
		int eAlign,
		CFont * pFont = NULL,
		bool bDropDown = false,
		UINT nHelperSysCmdID = 0,
		bool bTransparentBackground = false
		);

	virtual void PaintGripper(
		CDC & dc,
		const CRect & rectGripper,
		const CRect & rectText,
		bool bActive,
		bool bFloating,
		bool bHorz,
		bool bSideBar = false,
		LPCTSTR sCaption = NULL
		);

	virtual void PaintSeparator(
		CDC & dc,
		CRect & rectItem,
		bool bHorz
		);

	virtual void PaintIcon(
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
		);

	virtual void PaintMenuItem(
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
		);
	virtual void PaintMenuSeparator(
		CDC & dc,
		CRect & rectItem,
		bool bRarelyUsed
		);

	virtual void PaintControlFrame(
		CDC & dc,
		CRect & rectClient,
		bool bFlat,
		bool bHover,
		bool bEnabled,
		bool bFocus
		);

	virtual void PaintComboFrame(
		CDC &dc,
		CRect & rectClient,
		bool bHover,
		bool bPushed,
		bool bEnabled
		);

	virtual void PaintSysCmdGlyph(
		CDC & dc,
		UINT nHelperSysCmdID,
		const CRect & rcItem,
		bool bHover,
		bool bPushed,
		bool bEnabled
		);

	virtual void PaintToolbarExpandButton(
		CDC & dc,
		const CRect & rectButton,
		bool bHorz, // if false - down
		bool bBarIsCompletelyVisible,
		bool bEnabled,
		bool bPushed,
		bool bHover,
		bool bTransparentBackground = false
		);
	virtual void PaintMenuExpandButton(
		CDC & dc,
		const CRect & rectButton,
		bool bPushed,
		bool bHover
		);

	virtual void PaintDockingFrame(
		CDC & dc,
		const CRect & rectWindow,
		const CRect & rectClient,
		bool bFloating,
		bool bExtBar
		);

	virtual int GetMenuShadowSize();
	virtual int GetMenuBorderSize();

	virtual void PaintMenuBorder(
		CDC & dc,
		const CRect & rectClient
		);

public:
	enum e_xp_colors_t
	{
		XPCLR_3DFACE_DARK		= __ExtMfc_XP_COLOR_SHIFT+0,
		XPCLR_3DFACE_NORMAL		= __ExtMfc_XP_COLOR_SHIFT+1,
		XPCLR_SEPARATOR			= __ExtMfc_XP_COLOR_SHIFT+2,
		XPCLR_HILIGHT			= __ExtMfc_XP_COLOR_SHIFT+3,
		XPCLR_HILIGHT_BORDER	= __ExtMfc_XP_COLOR_SHIFT+4,
		XPCLR_RARELY_BORDER		= __ExtMfc_XP_COLOR_SHIFT+5,
	};

	virtual bool IsMenuMustCombineExcludeArea();
	virtual void PaintMenuCombinedArea(
		CDC & dc,
		const CRect & rcArea,
		const CRect & rcClient,
		int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
		);

	virtual void PaintDockingCaptionButton(
		CDC & dc,
		const CRect & rectClient,
		e_docking_caption_button_t eType,
		bool bHover,
		bool bPushed,
		bool bEnabled,
		bool bSideBar,
		bool bBarWndActive
		);
	virtual COLORREF GetMenuFrameFillColor()
	{
		return GetColor(XPCLR_3DFACE_NORMAL);
	};

	virtual void PaintControlBarBorders(
		e_control_bar_border_type_t eCBBT,
		DWORD dwStyle,
		CDC & dc,
		CRect & rc
		);

};

extern __PROF_UIS_API CExtPaintManager::CExtPaintManagerAutoPtr g_PaintManager;

#endif // __EXT_PAINT_MANAGER_H

