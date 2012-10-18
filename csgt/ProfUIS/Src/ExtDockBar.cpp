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

#include "stdafx.h"

#if (!defined __EXTDOCKBAR_H)
	#include "ExtDockBar.h"
#endif

#if _MSC_VER > 1200
#include <../Src/mfc/AfxImpl.h>
#else
#include <../Src/AfxImpl.h>
#endif // _MSC_VER > 1200


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CExtDockBar, CDockBar);

BEGIN_MESSAGE_MAP(CExtDockBar, CDockBar)
	//{{AFX_MSG_MAP(CExtDockBar)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	ON_WM_CANCELMODE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

bool CExtDockBar::g_bControlBarFixSizePixel = true;
bool CExtDockBar::g_bExtendedRepositioning = false;

void CExtDockBar::OnCancelMode()
{
	CDockBar::OnCancelMode();

	CExtPopupMenuWnd::CancelMenuTracking();
	ASSERT( !CExtPopupMenuWnd::IsMenuTracking() );
}

CFrameWnd * CExtDockBar::_GetDockingFrameImpl()
{
	ASSERT_VALID( this );
CFrameWnd * pFrame = GetParentFrame();
	ASSERT_VALID( pFrame );
	if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) )
	{
		pFrame = pFrame->GetParentFrame();
		ASSERT_VALID( pFrame );
		ASSERT( !pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)) );
	}
	return pFrame;
}

void CExtDockBar::_ContextMenuBuild(
	CFrameWnd * pFrame,
	CExtPopupMenuWnd * pPopup
	)
{
	ASSERT_VALID( pFrame );
	ASSERT( pPopup != NULL );
ExtControlBarVector_t vBars;
	CExtControlBar::_GetFrameControlBars(
		pFrame,
		vBars
		);
bool bOleInplaceItemActivated =
	CExtControlBar::IsOleIpObjActive( pFrame );

int nBarsCount = vBars.GetSize();
CExtControlBar * pPrevBar = NULL;
	for( int i=0; i<nBarsCount; i++ )
	{
		CExtControlBar * pBar = vBars[i];
		ASSERT_VALID( pBar );
		if( pBar->GetSafeHwnd() == NULL )
			continue;
		// do not include temporary hidden bars
		if( pBar->m_nStateFlags &
				(CControlBar::delayHide | CControlBar::tempHide)
			)
			continue;
		// do not include some bars when OLE inplace object active
		if( bOleInplaceItemActivated &&
				(pBar->m_dwStyle & CBRS_HIDE_INPLACE)
			)
			continue;

		// do not include bars without caption
		CString sBarText;
		pBar->GetWindowText( sBarText );
		if( sBarText.IsEmpty() )
		{
			ASSERT( FALSE );
			continue;
		}

		if( pPrevBar != NULL
			&&
			pPrevBar->m_bFixedMode != pBar->m_bFixedMode
			)
		{
			VERIFY(
				pPopup->ItemInsert(
					CExtPopupMenuWnd::TYPE_SEPARATOR
					)
				);
		}
		pPrevBar = pBar;


		int nBarID = pBar->GetDlgCtrlID();
		ASSERT( CExtCmdManager::IsCommand(nBarID) );
		
		CExtCmdManager::cmd_t cmd;
		cmd.m_nCmdID = nBarID;
		cmd.m_sMenuText = sBarText;
		cmd.TipsLoad();
		g_CmdManager->CmdSetup(
			g_CmdManager->ProfileNameFromWnd( pFrame->GetSafeHwnd() ),
			cmd
			);

		VERIFY(
			pPopup->ItemInsert(
				nBarID
				)
			);
	} // for( int i=0; i<nBarsCount; i++ )
}

void CExtDockBar::_ContextMenuTrack()
{
	// find any control bar
CControlBar * pBar = NULL;
	for( int nPos = 0; nPos < m_arrBars.GetSize(); nPos++ )
	{
		pBar = GetDockedControlBar(nPos);
		if( pBar != NULL )
		{
			ASSERT_VALID( pBar );
			break;
		}
	}
	if( pBar == NULL )
		return;

CFrameWnd * pFrame = _GetDockingFrameImpl();
CPoint point;
	VERIFY( ::GetCursorPos(&point) );
CExtPopupMenuWnd * pPopup =
		new CExtPopupMenuWnd;
	VERIFY( pPopup->CreatePopupMenu( pBar->GetOwner()->GetSafeHwnd() ) );
	_ContextMenuBuild(
		pFrame,
		pPopup
		);
	pPopup->TrackPopupMenu(
		0,
		point.x,point.y,
		pBar->GetOwner()->GetSafeHwnd() // pFrame->GetSafeHwnd()
		);
}

void CExtDockBar::OnContextMenu(CWnd* pWnd, CPoint point)
{
	pWnd;
	point;
	_ContextMenuTrack();
}

void CExtDockBar::DoPaint(CDC* pDC)
{
	ASSERT_VALID( pDC );
	CExtPaintManager::stat_ExcludeChildAreas(
		pDC->GetSafeHdc(),
		GetSafeHwnd()
		);
	CDockBar::DoPaint( pDC );
}

BOOL CExtDockBar::OnEraseBkgnd( CDC* pDC )
{
	CExtPaintManager::stat_ExcludeChildAreas(
		*pDC,
		*this
		);
	return CDockBar::OnEraseBkgnd(pDC);
}

CSize CExtDockBar::CalcFixedLayout(
	BOOL bStretch,
	BOOL bHorz
	)
{
	// based on MFC's source of
	// CDockBar::CalcFixedLayout()
	ASSERT_VALID(this);
CSize sizeFixed =
		CControlBar::CalcFixedLayout( bStretch, bHorz );
	// get max size
CSize sizeMax;
	if( !m_rectLayout.IsRectEmpty() )
		sizeMax = m_rectLayout.Size();
	else
	{
		CFrameWnd * pFrame = GetParentFrame();
		CRect rcFrameWindow;
		pFrame->GetClientRect( &rcFrameWindow );
		sizeMax = rcFrameWindow.Size();
	}
	// prepare for layout
AFX_SIZEPARENTPARAMS layout;
	layout.hDWP = m_bLayoutQuery ?
		NULL : ::BeginDeferWindowPos( m_arrBars.GetSize() );

CPoint pt( 0, 0 );
int nWidth = 0;

BOOL bWrapped = FALSE;

	for( int nPos = 0; nPos < m_arrBars.GetSize(); nPos++ )
	{ // layout all the control bars
		CControlBar * pBar = GetDockedControlBar(nPos);
		void * pVoid = m_arrBars[nPos];

		if( pBar != NULL )
		{
			CRect rcBarWin, rcBarClient;
			pBar->GetWindowRect( &rcBarWin );
			pBar->ScreenToClient( & rcBarWin );
			pBar->GetClientRect( &rcBarClient );
			CSize sizeBarMin =
				rcBarWin.Size() - rcBarClient.Size();

			if(	pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
			{
				if( ! ((CExtControlBar*)pBar)->m_bFixedMode )
				{
					sizeBarMin =
						CSize(
							((CExtControlBar *)pBar)->_CalcDesiredMinHW(),
							((CExtControlBar *)pBar)->_CalcDesiredMinVH()
							);
				} // if( ! ((CExtControlBar*)pBar)->m_bFixedMode )
			} // if(	pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) )
			if( pBar->IsVisible() )
			{
				// get ideal rect for bar
				DWORD dwMode = 0;
				if(	(pBar->m_dwStyle & CBRS_SIZE_DYNAMIC)
					&&
					(pBar->m_dwStyle & CBRS_FLOATING)
					)
					dwMode |= LM_HORZ | LM_MRUWIDTH;
				else if(pBar->m_dwStyle & CBRS_ORIENT_HORZ)
					dwMode |= LM_HORZ | LM_HORZDOCK;
				else
					dwMode |=  LM_VERTDOCK;

				CSize sizeBar =
					pBar->CalcDynamicLayout(-1, dwMode);
				BOOL bIsMenuBar = FALSE;

				if( pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) )
				{
					bIsMenuBar = TRUE;
					if(dwMode & LM_HORZDOCK)
						sizeBar.cx = sizeMax.cx;
					else if(dwMode & LM_VERTDOCK)
						sizeBar.cy = sizeMax.cy;
				}

				CRect rc(pt, sizeBar);

				// get current rect for bar
				CRect rcBar;
				pBar->GetWindowRect( &rcBar );
				ScreenToClient( &rcBar );

				BOOL bMenuIsCutted = FALSE;

				if( bHorz )
				{
					// Offset Calculated Rect out to Actual
					if(	rcBar.left > rc.left
						&& !m_bFloating
						)
						rc.OffsetRect(
							rcBar.left - rc.left,
							0
							);

					// If ControlBar goes off the right, then right justify
					if(	rc.right > sizeMax.cx
						&& !m_bFloating
						)
					{
						int x = rc.Width();
						x = max(sizeMax.cx - x, pt.x);
						rc.OffsetRect(
							x - rc.left,
							0
							);
						if( bIsMenuBar )
							bMenuIsCutted = TRUE;
						if(	rc.right  > sizeMax.cx )
							rc.right -= rc.right - sizeMax.cx;
					}

					// If ControlBar has been wrapped, then left justify
					if( bWrapped )
					{
						bWrapped = FALSE;
						rc.OffsetRect( -rc.left, 0 );

					}
					// If ControlBar is completely invisible, then wrap it
					else if(
						rc.Width() < sizeBarMin.cx
						||
						(rc.left >= sizeMax.cx || bMenuIsCutted )
						&&
						(nPos > 0) && (m_arrBars[nPos - 1] != NULL)
						)
					{
						if(	!pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
							||	(
								pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
								&&
								((CExtControlBar*)pBar)->m_bFixedMode
								)
							)
						{
							m_arrBars.InsertAt(
								nPos,
								(CObject*)NULL
								);
							pBar = NULL;
							pVoid = NULL;
							bWrapped = TRUE;
						}
					}
					if( !bWrapped )
					{
						if(rc != rcBar)
						{
							if(	!m_bLayoutQuery &&
								!(pBar->m_dwStyle & CBRS_FLOATING)
								)
								pBar->m_pDockContext->
									m_rectMRUDockPos = rc;
							AfxRepositionWindow(
								&layout,
								pBar->m_hWnd,
								&rc
								);
						}
						pt.x = rc.left + sizeBar.cx;
						nWidth = max(nWidth, sizeBar.cy);
					} // if( !bWrapped )
				} // if( bHorz )
				else
				{
					// Offset Calculated Rect out to Actual
					if(	rcBar.top > rc.top
						&& !m_bFloating
						)
						rc.OffsetRect(
							0,
							rcBar.top - rc.top
							);

					// If ControlBar goes off the bottom, then bottom justify
					if(	rc.bottom > sizeMax.cy
						&& !m_bFloating
						)
					{
						int y = rc.Height();
						y = max(sizeMax.cy - y, pt.y);
						rc.OffsetRect(
							0,
							y - rc.top
							);
						if(bIsMenuBar)
							bMenuIsCutted = TRUE;
						if(	rc.bottom  > sizeMax.cy )
							rc.bottom -= rc.bottom - sizeMax.cy;
					}

					// If ControlBar has been wrapped, then top justify
					if( bWrapped )
					{
						bWrapped = FALSE;
						rc.OffsetRect( 0, -rc.top );
					}
					// If ControlBar is completely invisible, then wrap it
					else if(
						rc.Height() < sizeBarMin.cy
						||
						(rc.top >= sizeMax.cy || bMenuIsCutted )
						&& (nPos > 0)
						&& (m_arrBars[nPos - 1] != NULL)
						)
					{
						if(	!pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar)) 
							||	(
								pBar->IsKindOf(RUNTIME_CLASS(CExtControlBar))
								&&
								((CExtControlBar*)pBar)->m_bFixedMode
								)
							)
						{
							m_arrBars.InsertAt(
								nPos,
								(CObject*)NULL
								);
							pBar = NULL;
							pVoid = NULL;
							bWrapped = TRUE;
						}
					}

					if( !bWrapped )
					{
						if(rc != rcBar)
						{
							if(	!m_bLayoutQuery
								&& !(pBar->m_dwStyle & CBRS_FLOATING)
								&& pBar->m_pDockContext != NULL
								)
								pBar->m_pDockContext->
									m_rectMRUDockPos = rc;
							AfxRepositionWindow(
								&layout,
								pBar->m_hWnd,
								&rc
								);
						}
						pt.y = rc.top + sizeBar.cy;
						nWidth = max(nWidth, sizeBar.cx);
					} // if( !bWrapped )
				} // else from if( bHorz )
			} // if( pBar->IsVisible() )

			if( !bWrapped )
				// handle any delay/show hide for the bar
				pBar->RecalcDelayShow( &layout );
		} // if( pBar != NULL )
		else
		{
			if( g_bExtendedRepositioning )
			{
				// try to remove wrapping:
				// are we have free space in the end of previouse row?
				if( nPos > 0 && nPos != m_arrBars.GetSize()-1 )
				{
					int nPosPrev = nPos-1;
					CControlBar * pPrevBar = NULL;
					do
					{
						pPrevBar = GetDockedControlBar(nPosPrev--);
						if( pPrevBar != NULL
							&& pPrevBar->IsVisible()
							&& (!pPrevBar->IsFloating())
							)
						{
							if( pPrevBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)) )
							{
								pPrevBar = NULL;
								break;
							}

							// get ideal rect for prev bar
							DWORD dwMode = 0;
							if(	(pPrevBar->m_dwStyle & CBRS_SIZE_DYNAMIC)
								&&
								(pPrevBar->m_dwStyle & CBRS_FLOATING)
								)
								dwMode |= LM_HORZ | LM_MRUWIDTH;
							else if(pPrevBar->m_dwStyle & CBRS_ORIENT_HORZ)
								dwMode |= LM_HORZ | LM_HORZDOCK;
							else
								dwMode |=  LM_VERTDOCK;

							CSize sizePrevBar =
								pPrevBar->CalcDynamicLayout(-1, dwMode);

							CRect rcPrev(pt, sizePrevBar);

							if( bHorz )
							{
								if(	rcPrev.right > sizeMax.cx
									&& !m_bFloating
									)
									pPrevBar = NULL;
							}
							else
							{
								if(	rcPrev.bottom > sizeMax.cy
									&& !m_bFloating
									)
									pPrevBar = NULL;
							}
							if( pPrevBar != NULL )
								break;
						}
						pPrevBar = NULL;
					} while( nPosPrev >= 0 );
					if( pPrevBar != NULL )
					{
						m_arrBars.RemoveAt(nPos);
						nPos--;
						continue;
					}
				} // if( nPos > 0 && nPos != m_arrBars.GetSize()-1 )
			} // if( g_bExtendedRepositioning )
		} // else from if( pBar != NULL )

		if( pBar == NULL && pVoid == NULL && nWidth != 0 )
		{
			// end of row because pBar == NULL
			if( bHorz )
			{
				pt.y += nWidth;
				sizeFixed.cx = max(sizeFixed.cx, pt.x);
				sizeFixed.cy = max(sizeFixed.cy, pt.y);
				pt.x = 0;
				sizeFixed.cy--;
			}
			else
			{
				pt.x += nWidth;
				sizeFixed.cx = max(sizeFixed.cx, pt.x);
				sizeFixed.cy = max(sizeFixed.cy, pt.y);
				pt.y = 0;
				sizeFixed.cx--;
			}
			nWidth = 0;
		}
	} // layout all the control bars
	if( !m_bLayoutQuery )
	{
		VERIFY( layout.hDWP != NULL );
		if( layout.hDWP != NULL )
		{
			VERIFY( ::EndDeferWindowPos(layout.hDWP) );
		}
	} // if( !m_bLayoutQuery )

	// adjust size for borders on the dock bar itself
CRect rc( 0, 0, 0, 0 );
	CalcInsideRect(rc, bHorz);
	if( (!bStretch || !bHorz) && sizeFixed.cx != 0 )
		sizeFixed.cx +=
			-rc.right + rc.left
			+ g_bControlBarFixSizePixel ? 1 : 0;
	if(	(!bStretch || bHorz) && sizeFixed.cy != 0 )
		sizeFixed.cy +=
			-rc.bottom + rc.top
			+ g_bControlBarFixSizePixel ? 1 : 0;

	return sizeFixed;
}

void CExtDockBar::_SlideDockControlBar(
	CControlBar* pBar,
	LPCRECT lpRect,
	BOOL bMovingEnabled
	)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pBar);
	ASSERT_KINDOF(CControlBar, pBar);

CRect rcBar;
	pBar->GetWindowRect(&rcBar);
	if(	pBar->m_pDockBar == this
		&& (lpRect == NULL || rcBar == *lpRect)
		)
	{
		// already docked and no change in position
		return;
	}

	// set CBRS_FLOAT_MULTI style if docking bar has it
	if(	m_bFloating
		&& (pBar->m_dwDockStyle & CBRS_FLOAT_MULTI)
		)
		m_dwStyle |= CBRS_FLOAT_MULTI;

	m_dwStyle &= ~(CBRS_SIZE_FIXED | CBRS_SIZE_DYNAMIC);
	m_dwStyle |=
		pBar->m_dwStyle & (CBRS_SIZE_FIXED | CBRS_SIZE_DYNAMIC);

	if( !(m_dwStyle & CBRS_FLOAT_MULTI) )
	{
		TCHAR szTitle[_MAX_PATH];
		pBar->GetWindowText(szTitle, _countof(szTitle));
		AfxSetWindowText(m_hWnd, szTitle);
	}

	// align correctly and turn on all borders
DWORD dwStyle = pBar->GetBarStyle();
	dwStyle &= ~(CBRS_ALIGN_ANY);
	dwStyle |=  (m_dwStyle & CBRS_ALIGN_ANY) | CBRS_BORDER_ANY;

	if( m_bFloating )
		dwStyle |= CBRS_FLOATING;
	else
		dwStyle &= ~CBRS_FLOATING;

	pBar->SetBarStyle( dwStyle );

/*
	// hide first if changing to a new docking site to avoid flashing
BOOL bShow = FALSE;
	if( pBar->m_pDockBar != this && pBar->IsWindowVisible() )
	{
		pBar->SetWindowPos(NULL, 0, 0, 0, 0,
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_HIDEWINDOW);
		bShow = TRUE;
	}
*/

int nPos = -1;
	if( lpRect != NULL )
	{
		// insert into appropriate row
		CRect rc( lpRect );
		ScreenToClient( &rc );
		CPoint ptMid(
			rc.left + rc.Width()/2,
			rc.top + rc.Height()/2
			);
		nPos = Insert( pBar, rc, ptMid );

		// position at requested position
		if( bMovingEnabled )
			pBar->SetWindowPos(
				NULL,
				rc.left, rc.top, rc.Width(), rc.Height(),
				SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS
					|SWP_NOREDRAW|SWP_NOSENDCHANGING
				);
	}
	else
	{
		// always add on current row, then create new one
		m_arrBars.Add(pBar);
		m_arrBars.Add(NULL);

		// align off the edge initially
		if( bMovingEnabled )
			pBar->SetWindowPos(
				NULL,
				-afxData.cxBorder2, -afxData.cyBorder2,
				0, 0,
				SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS
					|SWP_NOREDRAW|SWP_NOSENDCHANGING
				);
	}

	// attach it to the docking site
	if( pBar->GetParent() != this )
		pBar->SetParent(this);
	if( pBar->m_pDockBar == this )
		pBar->m_pDockBar->
			RemoveControlBar(pBar, nPos);
	else if( pBar->m_pDockBar != NULL )
		pBar->m_pDockBar->
			RemoveControlBar(
				pBar,
				-1,
				m_bFloating && !pBar->m_pDockBar->m_bFloating
				);
	pBar->m_pDockBar = this;

/*
	if( bShow )
	{
		ASSERT(!pBar->IsWindowVisible());
		pBar->SetWindowPos(NULL, 0, 0, 0, 0,
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);
	}
*/
	// remove any place holder for pBar in this dockbar
	RemovePlaceHolder( pBar );

/*
	// get parent frame for recalc layout
CFrameWnd* pFrameWnd = GetDockingFrame();
	pFrameWnd->DelayRecalcLayout();
*/
}

void CExtDockBar::DrawBorders(CDC* pDC, CRect& rect)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pDC );

	g_PaintManager->PaintControlBarBorders(
		CExtPaintManager::__CB_OUTER_DOCKBAR,
		m_dwStyle, // |CBRS_BORDER_ANY|CBRS_BORDER_3D
		*pDC, 
		rect
		);
}

void CExtDockBar::DrawGripper(CDC* pDC, const CRect& rect)
{
	pDC,
	rect;
}



