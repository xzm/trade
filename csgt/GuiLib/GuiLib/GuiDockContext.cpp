//-----------------------------------------------------------------------//
// This is a part of the GuiLib MFC Extention.							 //	
// Autor  :  Francisco Campos											 //
// (C) 2002 Francisco Campos <www.beyondata.com> All rights reserved     //
// This code is provided "as is", with absolutely no warranty expressed  //
// or implied. Any use is at your own risk.								 //		
// You must obtain the author's consent before you can include this code //
// in a software library.												 //
// If the source code in  this file is used in any application			 //
// then acknowledgement must be made to the author of this program		 //	
// fcampos@tutopia.com													 //
//-----------------------------------------------------------------------//

#include "stdafx.h"
#include "GuiDockContext.h"
#include "GuiToolBarWnd.h"
#include "MenuBar.h"
#include "GuiControlBar.h"
#define _AfxGetDlgCtrlID(hWnd)          ((UINT)(WORD)::GetDlgCtrlID(hWnd))
#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

#define HORZF(dw) (dw & CBRS_ORIENT_HORZ)
#define VERTF(dw) (dw & CBRS_ORIENT_VERT)
#define CX_BORDER   1
#define CY_BORDER   1


AFX_STATIC void AFXAPI _AfxAdjustRectangle(CRect& rect, CPoint pt)
{
	int nXOffset = (pt.x < rect.left) ? (pt.x - rect.left) :
					(pt.x > rect.right) ? (pt.x - rect.right) : 0;
	int nYOffset = (pt.y < rect.top) ? (pt.y - rect.top) :
					(pt.y > rect.bottom) ? (pt.y - rect.bottom) : 0;
	rect.OffsetRect(nXOffset, nYOffset);
}

/////////////////////////////////////////////////////////////////////////////
// CGuiDockContext

CGuiDockContext::CGuiDockContext(CControlBar* pBar):CDockContext(pBar)
{
	bFirstClic=TRUE;
	bSecondClic=TRUE;
}

CGuiDockContext::~CGuiDockContext()
{
}

/////////////////////////////////////////////////////////////////////////////
// CGuiDockContext Drag Operations

void CGuiDockContext::StartDrag(CPoint pt)
{
	ASSERT_VALID(m_pBar);
	m_bDragging = TRUE;

	InitLoop();

	// GetWindowRect returns screen coordinates(not mirrored),
	// so if the desktop is mirrored then turn off mirroring
	// for the desktop dc so that we get correct focus rect drawn.
	// This layout change should be remembered, just in case ...

	if (m_pDC->GetLayout() & LAYOUT_RTL)
		m_pDC->SetLayout(LAYOUT_LTR);

	if (m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC)
	{
		// get true bar size (including borders)
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptLast = pt;
		AdjustWindowForFloat(rect);
		CSize sizeHorz = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK);
		CSize sizeVert = m_pBar->CalcDynamicLayout(0, LM_VERTDOCK);
		CSize sizeFloat = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);

		m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
		m_rectDragVert = CRect(rect.TopLeft(), sizeVert);

		// calculate frame dragging rectangle
		m_rectFrameDragHorz = CRect(rect.TopLeft(), sizeFloat);
		m_rectFrameDragVert = CRect(rect.TopLeft(), sizeFloat);

		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);

//		m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
//		m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	}
	else if (m_pBar->m_dwStyle & CBRS_SIZE_FIXED)
	{
		// get true bar size (including borders)
		CRect rect;
		m_pBar->GetWindowRect(rect);
		AdjustWindowForFloat(rect);
		m_ptLast = pt;
		CSize sizeHorz = m_pBar->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
		CSize sizeVert = m_pBar->CalcDynamicLayout(-1, LM_VERTDOCK);

		// calculate frame dragging rectangle
		m_rectFrameDragHorz = m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
		m_rectFrameDragVert = m_rectDragVert = CRect(rect.TopLeft(), sizeVert);

		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
//		m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
//		m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	}
	else
	{
		// get true bar size (including borders)
		CRect rect;
		m_pBar->GetWindowRect(rect);
		AdjustWindowForFloat(rect);
		m_ptLast = pt;
		BOOL bHorz = HORZF(m_dwStyle);
		DWORD dwMode = !bHorz ? (LM_HORZ | LM_HORZDOCK) : LM_VERTDOCK;
		CSize size = m_pBar->CalcDynamicLayout(-1, dwMode);

		// calculate inverted dragging rect
		if (bHorz)
		{
			m_rectDragHorz = rect;
			m_rectDragVert = CRect(CPoint(pt.x - rect.Height()/2, rect.top), size);
		}
		else // vertical orientation
		{
			m_rectDragVert = rect;
			m_rectDragHorz = CRect(CPoint(rect.left, pt.y - rect.Width()/2), size);
		}

		// calculate frame dragging rectangle
		m_rectFrameDragHorz = m_rectDragHorz;
		m_rectFrameDragVert = m_rectDragVert;

		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
//		m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
//		m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	}

	// adjust rectangles so that point is inside
	_AfxAdjustRectangle(m_rectDragHorz, pt);
	_AfxAdjustRectangle(m_rectDragVert, pt);
	_AfxAdjustRectangle(m_rectFrameDragHorz, pt);
	_AfxAdjustRectangle(m_rectFrameDragVert, pt);

	// initialize tracking state and enter tracking loop
	m_dwOverDockStyle = CanDock();
	Move(pt);   // call it here to handle special keys
	Track();
}

void CGuiDockContext::AdjustWindowForFloat(CRect& rect)
{
	if( m_pBar->IsFloating())
	{
		rect.top+=(GetSystemMetrics(SM_CYCAPTION));
		rect.bottom-=(GetSystemMetrics(SM_CYFRAME)+GetSystemMetrics(SM_CYCAPTION))*2;
	}
}

void CGuiDockContext::Move(CPoint pt)
{
	CPoint ptOffset = pt - m_ptLast;

	// offset all drag rects to new position
	m_rectDragHorz.OffsetRect(ptOffset);
	m_rectFrameDragHorz.OffsetRect(ptOffset);
	m_rectDragVert.OffsetRect(ptOffset);
	m_rectFrameDragVert.OffsetRect(ptOffset);
	m_ptLast = pt;

	// if control key is down don't dock
	m_dwOverDockStyle = m_bForceFrame ? 0 : CanDock();

	// update feedback
	DrawFocusRect();
}

void CGuiDockContext::OnKey(int nChar, BOOL bDown)
{
	if (nChar == VK_CONTROL)
		UpdateState(&m_bForceFrame, bDown);
	if (nChar == VK_SHIFT)
		UpdateState(&m_bFlip, bDown);
}

void CGuiDockContext::EndDrag()
{
	CancelLoop();

	if (m_dwOverDockStyle != 0)
	{
		CDockBar* pDockBar = GetDockBar(m_dwOverDockStyle);
		ASSERT(pDockBar != NULL);

		CRect rect = (m_dwOverDockStyle & CBRS_ORIENT_VERT) ?
			m_rectDragVert : m_rectDragHorz;

		UINT uID = _AfxGetDlgCtrlID(pDockBar->m_hWnd);
		if (uID >= AFX_IDW_DOCKBAR_TOP &&
			uID <= AFX_IDW_DOCKBAR_BOTTOM)
		{
					
			m_uMRUDockID = uID;
			m_rectMRUDockPos = rect;
			pDockBar->ScreenToClient(&m_rectMRUDockPos);
		}
	
		// dock it at the specified position, RecalcLayout will snap
		m_pDockSite->DockControlBar(m_pBar, pDockBar, &rect);
		m_pDockSite->RecalcLayout();
	}
	else if ((m_dwStyle & CBRS_SIZE_DYNAMIC) || (HORZF(m_dwStyle) && !m_bFlip) ||
			(VERTF(m_dwStyle) && m_bFlip))
	{
		m_dwMRUFloatStyle = CBRS_ALIGN_TOP | (m_dwDockStyle & CBRS_FLOAT_MULTI);
		m_ptMRUFloatPos = m_rectFrameDragHorz.TopLeft();
		m_pDockSite->FloatControlBar(m_pBar, m_ptMRUFloatPos, m_dwMRUFloatStyle);
	}
	else // vertical float
	{
		m_dwMRUFloatStyle = CBRS_ALIGN_LEFT | (m_dwDockStyle & CBRS_FLOAT_MULTI);
		m_ptMRUFloatPos = m_rectFrameDragVert.TopLeft();
		m_pDockSite->FloatControlBar(m_pBar, m_ptMRUFloatPos, m_dwMRUFloatStyle);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGuiDockContext Resize Operations

#define m_rectRequestedSize     m_rectDragHorz
#define m_rectActualSize        m_rectDragVert
#define m_rectActualFrameSize   m_rectFrameDragHorz
#define m_rectFrameBorders      m_rectFrameDragVert

void CGuiDockContext::StartResize(int nHitTest, CPoint pt)
{
	ASSERT_VALID(m_pBar);
	ASSERT(m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC);
	m_bDragging = FALSE;

	InitLoop();

	// GetWindowRect returns screen coordinates(not mirrored)
	// So if the desktop is mirrored then turn off mirroring
	// for the desktop dc so that we draw correct focus rect

	if (m_pDC->GetLayout() & LAYOUT_RTL)
		m_pDC->SetLayout(LAYOUT_LTR);

	// get true bar size (including borders)
	CRect rect;
	m_pBar->GetWindowRect(rect);
	m_ptLast = pt;
	m_nHitTest = nHitTest;

	CSize size = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);
	m_rectRequestedSize = CRect(rect.TopLeft(), size);
	m_rectActualSize = CRect(rect.TopLeft(), size);
	m_rectActualFrameSize = CRect(rect.TopLeft(), size);

	// calculate frame rectangle
	CMiniFrameWnd::CalcBorders(&m_rectActualFrameSize);
//	m_rectActualFrameSize.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);

	m_rectFrameBorders = CRect(CPoint(0,0),
		m_rectActualFrameSize.Size() - m_rectActualSize.Size());

	// initialize tracking state and enter tracking loop
	m_dwOverDockStyle = 0;
	Stretch(pt);   // call it here to handle special keys
	Track();
}

void CGuiDockContext::Stretch(CPoint pt)
{
	CPoint ptOffset = pt - m_ptLast;

	// offset all drag rects to new position
	int nLength = 0;
	DWORD dwMode = LM_HORZ;
	if (m_nHitTest == HTLEFT || m_nHitTest == HTRIGHT)
	{
		if (m_nHitTest == HTLEFT)
			m_rectRequestedSize.left += ptOffset.x;
		else
			m_rectRequestedSize.right += ptOffset.x;
		nLength = m_rectRequestedSize.Width();
	}
	else
	{
		dwMode |= LM_LENGTHY;
		if (m_nHitTest == HTTOP)
			m_rectRequestedSize.top += ptOffset.y;
		else
			m_rectRequestedSize.bottom += ptOffset.y;
		nLength = m_rectRequestedSize.Height();
	}
	nLength = (nLength >= 0) ? nLength : 0;

	CSize size = m_pBar->CalcDynamicLayout(nLength, dwMode);

	CRect rectDesk;
	HWND hWndDesk = ::GetDesktopWindow();
	::GetWindowRect(hWndDesk, &rectDesk);
	CRect rectTemp = m_rectActualFrameSize;

	if (m_nHitTest == HTLEFT || m_nHitTest == HTTOP)
	{
		rectTemp.left = rectTemp.right -
			(size.cx + m_rectFrameBorders.Width());
		rectTemp.top = rectTemp.bottom -
			(size.cy + m_rectFrameBorders.Height());
		CRect rect;
		if (rect.IntersectRect(rectDesk, rectTemp))
		{
			m_rectActualSize.left = m_rectActualSize.right - size.cx;
			m_rectActualSize.top = m_rectActualSize.bottom - size.cy;
			m_rectActualFrameSize.left = rectTemp.left;
			m_rectActualFrameSize.top = rectTemp.top;
		}
	}
	else
	{
		rectTemp.right = rectTemp.left +
			(size.cx + m_rectFrameBorders.Width());
		rectTemp.bottom = rectTemp.top +
			(size.cy + m_rectFrameBorders.Height());
		CRect rect;
		if (rect.IntersectRect(rectDesk, rectTemp))
		{
			m_rectActualSize.right = m_rectActualSize.left + size.cx;
			m_rectActualSize.bottom = m_rectActualSize.top + size.cy;
			m_rectActualFrameSize.right = rectTemp.right;
			m_rectActualFrameSize.bottom = rectTemp.bottom;
		}
	}
	m_ptLast = pt;

	// update feedback
	DrawFocusRect();
}

void CGuiDockContext::EndResize()
{
	CancelLoop();

	m_pBar->CalcDynamicLayout(m_rectActualSize.Width(), LM_HORZ | LM_COMMIT);
	m_pDockSite->FloatControlBar(m_pBar, m_rectActualFrameSize.TopLeft(),
		CBRS_ALIGN_TOP | (m_dwDockStyle & CBRS_FLOAT_MULTI) | CBRS_SIZE_DYNAMIC);
}

/////////////////////////////////////////////////////////////////////////////
// CGuiDockContext Double Click Operations

void CGuiDockContext::ToggleDocking()
{
	if (m_pBar->IsFloating())
	{
		// Dock it only if is allowed to be docked
		if (m_pBar->m_dwDockStyle & CBRS_ALIGN_ANY)
		{
			ASSERT((m_uMRUDockID >= AFX_IDW_DOCKBAR_TOP &&
				m_uMRUDockID <= AFX_IDW_DOCKBAR_BOTTOM) ||
				m_uMRUDockID == 0);
					
			CRect rect = m_rectMRUDockPos;
			CDockBar* pDockBar = NULL;
			if (m_uMRUDockID != 0)
			{
				pDockBar = (CDockBar*)m_pDockSite->GetControlBar(m_uMRUDockID);
				pDockBar->ClientToScreen(&rect);
			}

			// dock it at the specified position, RecalcLayout will snap
			m_pDockSite->ReDockControlBar(m_pBar, pDockBar, &rect);
			m_pDockSite->RecalcLayout();
		}
	}
	else
	{
		CPoint ptFloat = m_ptMRUFloatPos;
		if (ptFloat.x < 0 || ptFloat.y < 0)
		{
			ptFloat = m_rectMRUDockPos.TopLeft();
			m_pBar->GetParent()->ClientToScreen(&ptFloat);
		}
		m_pDockSite->FloatControlBar(m_pBar, ptFloat, m_dwMRUFloatStyle);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGuiDockContext Operations

void CGuiDockContext::InitLoop()
{
	// handle pending WM_PAINT messages
	MSG msg;
	while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
	{
		if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
			return;
		DispatchMessage(&msg);
	}

	// get styles from bar
	m_dwDockStyle = m_pBar->m_dwDockStyle;
	m_dwStyle = m_pBar->m_dwStyle & CBRS_ALIGN_ANY;
	ASSERT(m_dwStyle != 0);

	// initialize state
	m_rectLast.SetRectEmpty();
	m_sizeLast.cx = m_sizeLast.cy = 0;
	m_bForceFrame = m_bFlip = m_bDitherLast = FALSE;

	// lock window update while dragging
	ASSERT(m_pDC == NULL);
	CWnd* pWnd = CWnd::GetDesktopWindow();
	if (pWnd->LockWindowUpdate())
		m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE);
	else
		m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW|DCX_CACHE);
	ASSERT(m_pDC != NULL);
}

void CGuiDockContext::CancelLoop()
{
	DrawFocusRect(TRUE);    // gets rid of focus rect
	ReleaseCapture();

	CWnd* pWnd = CWnd::GetDesktopWindow();
	pWnd->UnlockWindowUpdate();
	if (m_pDC != NULL)
	{
		pWnd->ReleaseDC(m_pDC);
		m_pDC = NULL;
	}
	bFirstClic=TRUE;
	bSecondClic=TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

void CGuiDockContext::DrawFocusRect(BOOL bRemoveRect)
{
	ASSERT(m_pDC != NULL);

	// default to thin frame
	CSize size(CX_BORDER, CY_BORDER);

	// determine new rect and size
	CRect rect;
	CBrush* pWhiteBrush = CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH));
	CBrush* pDitherBrush = CDC::GetHalftoneBrush();
	CBrush* pBrush = pWhiteBrush;

	if (HORZF(m_dwOverDockStyle))
		rect = m_rectDragHorz;
	else if (VERTF(m_dwOverDockStyle))
		rect = m_rectDragVert;
	else
	{
		// use thick frame instead
		size.cx = GetSystemMetrics(SM_CXFRAME) - CX_BORDER;
		size.cy = GetSystemMetrics(SM_CYFRAME) - CY_BORDER;
		if ((HORZF(m_dwStyle) && !m_bFlip) || (VERTF(m_dwStyle) && m_bFlip))
			rect = m_rectFrameDragHorz;
		else
			rect = m_rectFrameDragVert;
		pBrush = pDitherBrush;
	}
	if (bRemoveRect)
		size.cx = size.cy = 0;

/*	if (afxData.bWin4 &&
		(HORZF(m_dwOverDockStyle) || VERTF(m_dwOverDockStyle)))
	{
		// looks better one pixel in (makes the bar look pushed down)
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
	}
*/
	// draw it and remember last size
	
	if (bFirstClic==TRUE)
	{
		bFirstClic=FALSE;
	}
	else{
		if (bSecondClic==TRUE)
		{
		  m_pDC->DrawDragRect(&rect, size, &m_rectLast, m_sizeLast,
			pBrush, m_bDitherLast ? pDitherBrush : pWhiteBrush);
		  m_bDitherLast = (pBrush == pDitherBrush);
		   m_pDC->DrawDragRect(&rect, size, &m_rectLast, m_sizeLast,
			pBrush, m_bDitherLast ? pDitherBrush : pWhiteBrush);
		  bSecondClic=FALSE;
		}
		else
		{
			m_pDC->DrawDragRect(&rect, size, &m_rectLast, m_sizeLast,
			pBrush, m_bDitherLast ? pDitherBrush : pWhiteBrush);
			m_rectLast = rect;
			m_sizeLast = size;
			m_bDitherLast = (pBrush == pDitherBrush);
		}
	}
	
}

void CGuiDockContext::UpdateState(BOOL* pFlag, BOOL bNewValue)
{
	if (*pFlag != bNewValue)
	{
		*pFlag = bNewValue;
		m_bFlip = (HORZF(m_dwDockStyle) && VERTF(m_dwDockStyle) && m_bFlip); // shift key
		m_dwOverDockStyle = (m_bForceFrame) ? 0 : CanDock();
		DrawFocusRect();
	}
}

DWORD CGuiDockContext::CanDock()
{
	BOOL bStyleHorz;
	DWORD dwDock = 0; // Dock Canidate
	DWORD dwCurr = 0; // Current Orientation

	// let's check for something in our current orientation first
	// then if the shift key is not forcing our orientation then
	// check for horizontal or vertical orientations as long
	// as we are close enough
	ASSERT(m_dwStyle != 0);

	bStyleHorz = HORZF(m_dwStyle);
	bStyleHorz = m_bFlip ? !bStyleHorz : bStyleHorz;

	if (bStyleHorz && HORZF(m_dwDockStyle))
		dwDock = CanDock(m_rectDragHorz,
									  m_dwDockStyle & ~CBRS_ORIENT_VERT);
	else if (VERTF(m_dwDockStyle))
		dwDock = CanDock(m_rectDragVert,
									  m_dwDockStyle & ~CBRS_ORIENT_HORZ);

	if (!m_bFlip)
	{
		if (dwDock == 0 && HORZF(m_dwDockStyle))
		{
			dwCurr = CanDock(m_rectDragVert,
										  m_dwDockStyle & ~CBRS_ORIENT_VERT);
			dwDock = CanDock(m_rectDragHorz,
										  m_dwDockStyle & ~CBRS_ORIENT_VERT);
			dwDock = (dwDock == dwCurr) ? dwDock : 0;
		}
		if (dwDock == 0 && VERTF(m_dwDockStyle))
		{
			dwCurr = CanDock(m_rectDragHorz,
										  m_dwDockStyle & ~CBRS_ORIENT_HORZ);
			dwDock = CanDock(m_rectDragVert,
										  m_dwDockStyle & ~CBRS_ORIENT_HORZ);
			dwDock = (dwDock == dwCurr) ? dwDock : 0;
		}
	}

	return dwDock;
}


DWORD CGuiDockContext::CanDock(CRect rect, DWORD dwDockStyle, CDockBar** ppDockBar)
{
	// dwDockStyle -- allowable styles of bar
	// don't allow to dock to floating unless multi is specified
	BOOL bTipoToolBar=m_pBar->IsKindOf(RUNTIME_CLASS( CGuiToolBarWnd)) ||
		              m_pBar->IsKindOf(RUNTIME_CLASS( CMenuBar));
	dwDockStyle &= CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI;
	if (ppDockBar != NULL)
		*ppDockBar = NULL;
	POSITION pos = m_pDockSite->m_listControlBars.GetHeadPosition();
	while (pos != NULL)
	{
		CDockBar* pDockBar = (CDockBar*)m_pDockSite->m_listControlBars.GetNext(pos);
		if (pDockBar->IsDockBar() && pDockBar->IsWindowVisible() &&
			(pDockBar->m_dwStyle & dwDockStyle & CBRS_ALIGN_ANY) &&
			(!pDockBar->m_bFloating ||
				(dwDockStyle & pDockBar->m_dwStyle & CBRS_FLOAT_MULTI)))
		{
		
				int nSize=pDockBar->m_arrBars.GetSize();
				for (int i=0;i <nSize;  i++)
				{
					UINT m_nDockBarID = pDockBar->GetDlgCtrlID();
					CControlBar* pBar;
					 pBar = (CControlBar*) pDockBar->m_arrBars[i];
					 if (HIWORD(pBar) == 0) continue; // placeholder
					 if (!pBar->IsVisible()) continue;
					 CRect rectBar;
					 pBar->GetWindowRect(&rectBar);
					 if (rectBar.Width() == 0)
						rectBar.right++;
					 if (rectBar.Height() == 0)
							rectBar.bottom++;
					 CRect rectBar1;
					 pDockBar->GetWindowRect(&rectBar1);
					 if (dwDockStyle==CBRS_ORIENT_VERT)
					 {
						 if (m_nDockBarID==AFX_IDW_DOCKBAR_LEFT)
					 		rectBar1.right=rectBar1.left+10;
						 if(m_nDockBarID==AFX_IDW_DOCKBAR_RIGHT)
							 rectBar1.left=rectBar1.right-10;
					 }
					 else
					 {

						if (m_nDockBarID==AFX_IDW_DOCKBAR_BOTTOM)
							rectBar1.top=rectBar1.bottom-10;
						if (m_nDockBarID==AFX_IDW_DOCKBAR_TOP)
							rectBar1.bottom=rectBar1.top+10;
						
					 }
					 if (rectBar1.IntersectRect(rectBar1, rect))
					 {
						if (ppDockBar != NULL)
							
							*ppDockBar = pDockBar;
						
						return pDockBar->m_dwStyle & dwDockStyle;
					 }
			

					//***********************************************
					if (rectBar.IntersectRect(rectBar, rect))
					{
							if (bTipoToolBar)
							{
								if (pBar->IsKindOf(RUNTIME_CLASS( CGuiControlBar)))
									return 0;
							}
							if (ppDockBar != NULL)
								*ppDockBar = pDockBar;
							return pDockBar->m_dwStyle & dwDockStyle;
							
						 
							
					}
					 
				}//fin del for
							
		
			CRect rectBar;
			pDockBar->GetWindowRect(&rectBar);
			if (rectBar.Width() == 0)
				rectBar.right++;
			if (rectBar.Height() == 0)
				rectBar.bottom++;
			if (rectBar.IntersectRect(rectBar, rect))
			{
				if (ppDockBar != NULL)
					*ppDockBar = pDockBar;
				return pDockBar->m_dwStyle & dwDockStyle;
			}
		}
	}
	return 0;
}

//*********************************************
// return 0 no acepta el docking
//        1 lo acepta
//        2 continue
int  CGuiDockContext::VerLeft(int i,int nSize,CDockBar* pDockBar,CControlBar* pBar)
{
	BOOL bTipoToolBar=m_pBar->IsKindOf(RUNTIME_CLASS( CGuiToolBarWnd));
	//i--;
	if (i+1 < nSize)
	{
		CControlBar* pBartemp;
 		pBartemp = (CControlBar*) pDockBar->m_arrBars[i+1];
		if (HIWORD(pBartemp) == 0) return 2; // placeholder
		if (!pBartemp->IsVisible()) return 2;
		if (bTipoToolBar)
				if (!pBartemp->IsKindOf(RUNTIME_CLASS( CGuiToolBarWnd)))
					return 0;				
	}
	if (i > 0)
	{
		CControlBar* pBartemp;
		pBartemp = (CControlBar*) pDockBar->m_arrBars[i-1];
		if (HIWORD(pBartemp) == 0) return 2; // placeholder
		if (!pBartemp->IsVisible()) return 2;
		if (bTipoToolBar)
				if (!pBartemp->IsKindOf(RUNTIME_CLASS( CGuiToolBarWnd)))
					return 0;				
	}

	return 1;

}

//*********************************************
// return 0 no acepta el docking
//        1 lo acepta
//        2 continue

int  CGuiDockContext::VerRight(int i,int nSize,CDockBar* pDockBar,CControlBar* pBar)

{
	BOOL bTipoToolBar=m_pBar->IsKindOf(RUNTIME_CLASS( CGuiToolBarWnd));
	//i--;
	if (i==0)
	{
		CControlBar* pBartemp;
		pBartemp = (CControlBar*) pDockBar->m_arrBars[i+1];
		if (HIWORD(pBartemp) == 0) return 2; // placeholder
		if (!pBartemp->IsVisible()) return 2;
		if (bTipoToolBar)
				if (!pBartemp->IsKindOf(RUNTIME_CLASS( CGuiToolBarWnd)))
					return 0;				
	}
	if (i > 0)
	{
			CControlBar* pBartemp;
			pBartemp = (CControlBar*) pDockBar->m_arrBars[i-1];
			if (HIWORD(pBartemp) == 0) return 2; // placeholder
			if (!pBartemp->IsVisible()) return 2;
			if (bTipoToolBar)
				if (!pBartemp->IsKindOf(RUNTIME_CLASS( CGuiToolBarWnd)))
					return 0;				
	
	}
	return 1;
}

CDockBar* CGuiDockContext::GetDockBar(DWORD dwOverDockStyle)
{
	DWORD dw = 0;
	CDockBar* pBar;
	if (HORZF(dwOverDockStyle))
	{
		dw = CanDock(m_rectDragHorz,
			dwOverDockStyle & ~CBRS_ORIENT_VERT, &pBar);
		ASSERT(dw != 0);
		ASSERT(pBar != NULL);
		return pBar;
	}
	if (VERTF(dwOverDockStyle))
	{
		dw =CanDock(m_rectDragVert,
			dwOverDockStyle & ~CBRS_ORIENT_HORZ, &pBar);
		ASSERT(dw != 0);
		ASSERT(pBar != NULL);
		return pBar;
	}
	return NULL;
}

BOOL CGuiDockContext::Track()
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	// set capture to the window which received this message
	m_pBar->SetCapture();
	ASSERT(m_pBar == CWnd::GetCapture());

	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == m_pBar)
	{
		MSG msg;
		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage(msg.wParam);
			break;
		}

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			if (m_bDragging)
				EndDrag();
			else
				EndResize();
			return TRUE;
		case WM_MOUSEMOVE:
			if (m_bDragging)
				Move(msg.pt);
			else
				Stretch(msg.pt);
			break;
		case WM_KEYUP:
			if (m_bDragging)
				OnKey((int)msg.wParam, FALSE);
			break;
		case WM_KEYDOWN:
			if (m_bDragging)
				OnKey((int)msg.wParam, TRUE);
			if (msg.wParam == VK_ESCAPE)
			{
				CancelLoop();
				return FALSE;
			}
			break;
		case WM_RBUTTONDOWN:
			CancelLoop();
			return FALSE;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

	CancelLoop();

	return FALSE;
}
