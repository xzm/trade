// FlatListBox.cpp : implementation file
//

#include "stdafx.h"
#include "FlatListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlatListBox

CFlatListBox::CFlatListBox()
{	
	m_clrHilite = GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrShadow = GetSysColor(COLOR_BTNSHADOW);
	m_clrDkShad = GetSysColor(COLOR_3DDKSHADOW);
	m_clrButton = GetSysColor(COLOR_BTNFACE);
}

CFlatListBox::~CFlatListBox()
{
}


BEGIN_MESSAGE_MAP(CFlatListBox, CListBox)
	//{{AFX_MSG_MAP(CFlatListBox)
	ON_CONTROL_REFLECT(LBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(LBN_SETFOCUS, OnSetfocus)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatListBox message handlers

void CFlatListBox::OnKillfocus() 
{
	DrawListBox(FE_MOUSEOVER, m_clrShadow, m_clrHilite);
	SetTimer(1, 500, NULL);
}

void CFlatListBox::OnSetfocus() 
{
	DrawListBox(FE_MOUSEOVER, m_clrShadow, m_clrHilite);
	SetTimer(1, 500, NULL);
}

void CFlatListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = true;
	CListBox::OnLButtonDown(nFlags, point);
}

void CFlatListBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = false;
	CListBox::OnLButtonUp(nFlags, point);
}

void CFlatListBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetTimer(1, 10, NULL);
	CListBox::OnMouseMove(nFlags, point);
}

void CFlatListBox::OnTimer(UINT nIDEvent) 
{
	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);

	if(!rcItem.PtInRect(pt)) {
		KillTimer(1);
		DrawListBox(FE_MOUSEOUT, m_clrButton, m_clrButton);
		return;
	}
	else {
		DrawListBox(FE_MOUSEOVER, m_clrShadow, m_clrHilite);
	}

	CListBox::OnTimer(nIDEvent);
}

void CFlatListBox::OnPaint() 
{
	Default();
	DrawListBox(FE_MOUSEOUT, m_clrButton, m_clrButton);
}

void CFlatListBox::OnSysColorChange() 
{
	CListBox::OnSysColorChange();
	m_clrHilite = GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrShadow = GetSysColor(COLOR_BTNSHADOW);
	m_clrDkShad = GetSysColor(COLOR_3DDKSHADOW);
	m_clrButton = GetSysColor(COLOR_BTNFACE);	
}

void CFlatListBox::DrawListBox(int style, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	CRect rcItem;
	GetClientRect(&rcItem);
	CDC *pDC = GetDC();

	pDC->Draw3dRect(rcItem, clrTopLeft, clrBottomRight);
	rcItem.DeflateRect(1, 1);

	if(!IsWindowEnabled()) {
		pDC->Draw3dRect(rcItem, m_clrHilite, m_clrHilite);
	}
	else {
		pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
	}

	if(!IsWindowEnabled())
		return;

	switch(style) {

	case FE_MOUSEOVER:

		rcItem.top -= 1;
		rcItem.bottom -= 1;
		pDC->Draw3dRect(rcItem, m_clrHilite, m_clrHilite); //was shadow
		break;

	case FE_MOUSEOUT:
		rcItem.top += 1;
		rcItem.bottom += 1;
		pDC->Draw3dRect(rcItem, m_clrHilite, m_clrShadow); //was shadow
		rcItem.left += 1;
		pDC->Draw3dRect(rcItem, m_clrHilite, m_clrShadow); //was shadow
		break;
	}
	

	ReleaseDC(pDC);
}
