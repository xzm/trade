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
#include "guiimagelinkbutton.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CGuiImageLinkButton,CGuiToolButton)
CGuiImageLinkButton::CGuiImageLinkButton(void)
{
	m_cfontSelect.CreateFont(14,0,0,0,400,0,1,0,0,1,2,1,34,"Arial");
	m_cfontNormal.CreateFont(14,0,0,0,400,0,0,0,0,1,2,1,34,"Arial");
	rectletra=CRect(0,0,0,0);
	m_iDent=0;
}

CGuiImageLinkButton::~CGuiImageLinkButton(void)
{
}

BEGIN_MESSAGE_MAP(CGuiImageLinkButton, CGuiToolButton)
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CGuiImageLinkButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC*  pdc= CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rc=lpDrawItemStruct->rcItem;
	UINT  uState=lpDrawItemStruct->itemState;
	CFont* m_OldFont;
	CBrush cb;
	int calculodify;

	cb.CreateSolidBrush(m_clColor); //deberia ser blanco
	
	if (m_bMouserOver)
		m_OldFont=pdc->SelectObject(&m_cfontSelect);
	else
		m_OldFont=pdc->SelectObject(&m_cfontNormal);

	pdc->FillRect(rc,&cb);
	
	
	calculodify=rc.Height()-(m_SizeImage.cy);
	calculodify/=2;
	CPoint m_point=CPoint(12,calculodify+1);
	rc.left+=8; //identar a la derecha
	if (m_SizeImage.cx > 2)
	{
		pdc->DrawState (m_point, m_SizeImage,m_Icon,
					(uState==ODS_DISABLED?DSS_DISABLED:DSS_NORMAL),(CBrush*)NULL);
	}
	
	if (m_SizeText.cx > 2)
	{
		int nMode = pdc->SetBkMode(TRANSPARENT);
		COLORREF clOldColor=pdc->SetTextColor(RGB(0,0,200));
		rectletra=rc;
		rectletra.left+=m_iDent+m_SizeImage.cx+8;
		CPoint pt=CSize(rectletra.top,rectletra.left);
		if (uState & ODS_DISABLED)
			pdc->DrawState(pt, m_SizeText, m_szText, DSS_DISABLED, TRUE, 0, (CBrush*)NULL);
		else
			pdc->DrawText(m_szText,rectletra,DT_SINGLELINE|DT_LEFT|DT_BOTTOM);
		pdc->SetBkMode(nMode);
		pdc->SetTextColor(clOldColor);
	}
	pdc->SelectObject(m_OldFont);	
	// TODO:  Add your code to draw the specified item
}

BOOL CGuiImageLinkButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
//	::SetCursor(::LoadCursor(NULL,IDC_HAND));	
	return TRUE;	
	//return CGuiToolButton::OnSetCursor(pWnd, nHitTest, message);
}



void CGuiImageLinkButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//la idea es que solo se muestre el cursor hand si se esta sobre la imagen
	//o sobre la cadena
	CRect m_rect;
	GetClientRect(&m_rect);
	rectletra.left=m_rect.left+4;
	if (m_bMouserOver) return;
	if (rectletra.PtInRect(point))
		::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));//IDC_HAND));
	else
		::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	CGuiToolButton::OnMouseMove(nFlags, point);
}

//********************************************************************************
BEGIN_MESSAGE_MAP(CGuiScrollButton, CGuiToolButton)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


CGuiScrollButton::CGuiScrollButton(void)
{
}

CGuiScrollButton::~CGuiScrollButton(void)
{
}
void CGuiScrollButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CGuiToolButton::OnMouseMove(nFlags, point);
	if (m_bMouserOvernew) 
	{
		
		SetTimer(2,20,0);
	}
}

void CGuiScrollButton::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CGuiToolButton::OnTimer(nIDEvent);
//	if (!m_bMouserOvernew) return;
	CRect rc;
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);
	GetClientRect(rc);
	if (rc.PtInRect(pt))
	{
		CWnd* pParent=GetParent();
		pParent->SendMessage(WM_COMMAND,GetDlgCtrlID());
	}
	CButton::OnTimer(nIDEvent);
}

