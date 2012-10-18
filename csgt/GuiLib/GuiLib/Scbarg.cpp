/////////////////////////////////////////////////////////////////////////
//
// CSizingControlBarG           Version 2.43
// 
// Created: Jan 24, 1998        Last Modified: August 03, 2000
//
// See the official site at www.datamekanix.com for documentation and
// the latest news.
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-2000 by Cristi Posea. All rights reserved.
//
// This code is free for personal and commercial use, providing this 
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// cristi@datamekanix.com or post them at the message board at the site.
/////////////////////////////////////////////////////////////////////////
//
// Class	:  CSizingControlBarG
//
// Version	: 1.0
//
// Created	: Jan 4, 2001        
//
// Last Modified: 
//
// Modify by Francisco Campos  	
//
// Copyright (C) 2001 by Francisco Campos. All rights reserved.
//
// fcampos@tutopia.com.
//
//
// sizecbar.cpp : implementation file
//

#include "stdafx.h"
#include "scbarg.h"
#include "resource.h"
#include "GuiDrawLayer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////
// CSizingControlBarG

IMPLEMENT_DYNAMIC(CSizingControlBarG, baseCSizingControlBarG);

CSizingControlBarG::CSizingControlBarG()
{
    m_cyGripper = 17;
	m_style=Office97;  //style office 97 by default
}

CSizingControlBarG::~CSizingControlBarG()
{
}

BEGIN_MESSAGE_MAP(CSizingControlBarG, baseCSizingControlBarG)
    //{{AFX_MSG_MAP(CSizingControlBarG)
    ON_WM_NCLBUTTONUP()
    ON_WM_NCHITTEST()
	ON_COMMAND(WM_SHOWTITLE,ShowCaption)
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
// CSizingControlBarG message handlers

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//

void CSizingControlBarG::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
    if (nHitTest == HTCLOSE)
        m_pDockSite->ShowControlBar(this, FALSE, FALSE); // hide

    baseCSizingControlBarG::OnNcLButtonUp(nHitTest, point);
}

void CSizingControlBarG::ShowCaption()
{
	baseCSizingControlBarG::OnNcpaint();

}
void CSizingControlBarG::NcPaintGripper(CDC* pDC, CRect rcClient)
{

    if (!HasGripper())
        return;
	CRect gripper = rcClient;
	gripper.top =5;
	gripper.bottom =gripper.top +m_cyGripper-1;
		
	if(!m_bActive)
	{
		CPen cp(PS_SOLID,1,::GetSysColor(COLOR_BTNSHADOW));
		
		CPen* cpold=pDC->SelectObject(&cp);
		//linea superior
		pDC->MoveTo(gripper.left+1,gripper.top);
		pDC->LineTo(gripper.right,gripper.top);
		//linea izquierda
		pDC->MoveTo(gripper.left,gripper.top+1);
		pDC->LineTo(gripper.left,gripper.bottom);
		//linea inferior
		pDC->MoveTo(gripper.left+1,gripper.bottom);
		pDC->LineTo(gripper.right,gripper.bottom);
		//linea derecha
		pDC->MoveTo(gripper.right,gripper.top+1);
		pDC->LineTo(gripper.right,gripper.bottom);
		pDC->SelectObject(cpold);
	}
	else
	{
		CBrush cb;
		cb.CreateSolidBrush(::GetSysColor(COLOR_ACTIVECAPTION));//GuiDrawLayer::GetRGBCaptionXP());
		pDC->FillRect(gripper,&cb);
	}
	gripper.DeflateRect(1, 1);
	CString m_caption;
	GetWindowText(m_caption);
	CFont m_cfont;

	m_cfont.CreateFont(-11,0,0,0,400,0,0,0,0,1,2,1,34,"Verdana");
		
	CFont* m_fontOld=pDC->SelectObject(&m_cfont);
	int nMode = pDC->SetBkMode(TRANSPARENT);
	CSize SizeCad=pDC->GetTextExtent(m_caption);
	CRect rCText=gripper;
	rCText.top=6;
	rCText.bottom =rCText.top+14;
	
	int cont=SizeCad.cx;
	while(cont > 1 && gripper.Width() > 0)
		{
			CSize coor=pDC->GetTextExtent(m_caption,m_caption.GetLength());
			if(coor.cx > gripper.Width()-10)
			{
				m_caption=m_caption.Left(m_caption.GetLength()-1);
			}
			else
				break;
			cont--;
				
		}
	if (gripper.Width() > 0	)
	if (!m_bActive)
		pDC->TextOut(rCText.left+3,rCText.top,m_caption);
	else
	{
		pDC->SetTextColor(RGB(255,255,255));
		pDC->TextOut(rCText.left+3,rCText.top,m_caption);
	}

 	pDC->SetBkMode(nMode);
	pDC->SelectObject(m_fontOld);


	if (m_bActive)
		m_biHide.m_bActive=TRUE;
	else
		m_biHide.m_bActive=FALSE;
	m_biHide.m_Stylo=m_style;	
    m_biHide.Paint(pDC);
	
	
}

void CSizingControlBarG::NcCalcClient(LPRECT pRc, UINT nDockBarID)
{
    CRect rcBar(pRc); // save the bar rect


    // subtract edges
    baseCSizingControlBarG::NcCalcClient(pRc, nDockBarID);

    if (!HasGripper())
        return;

    CRect rc(pRc); // the client rect as calculated by the base class

   // rc.DeflateRect(0, m_cyGripper, 0, 0);

    // set position for the "x" (hide bar) button
    CPoint ptOrgBtn;
    ptOrgBtn = CPoint(rc.right - 12, rc.top - 15);

    m_biHide.Move(ptOrgBtn - rcBar.TopLeft());
	
    *pRc = rc;
}

UINT CSizingControlBarG::OnNcHitTest(CPoint point)
{
    CRect rcBar;
    GetWindowRect(rcBar);

    UINT nRet = baseCSizingControlBarG::OnNcHitTest(point);
    if (nRet != HTCLIENT)
        return nRet;

    CRect rc = m_biHide.GetRect();
    rc.OffsetRect(rcBar.TopLeft());
    if (rc.PtInRect(point))
        return HTCLOSE;

    return HTCLIENT;
}

/////////////////////////////////////////////////////////////////////////
// CSizingControlBarG implementation helpers

void CSizingControlBarG::OnUpdateCmdUI(CFrameWnd* pTarget,
                                      BOOL bDisableIfNoHndler)
{
    CPoint pt;
    ::GetCursorPos(&pt);
	BOOL bNeedPaint = FALSE;

	UNUSED_ALWAYS(bDisableIfNoHndler);
    UNUSED_ALWAYS(pTarget);

    if (!HasGripper())
        return;


    CWnd* pFocus = GetFocus();
    BOOL bActiveOld = m_bActive;

    m_bActive = (pFocus->GetSafeHwnd() && IsChild(pFocus));

	BOOL bHit = (OnNcHitTest(pt) == HTCLOSE);
    BOOL bLButtonDown = (::GetKeyState(VK_LBUTTON) < 0);

    BOOL bWasPushed = m_biHide.bPushed;
    m_biHide.bPushed = bHit && bLButtonDown;

    BOOL bWasRaised = m_biHide.bRaised;
    m_biHide.bRaised = bHit && !bLButtonDown;
	
	bNeedPaint |= (m_biHide.bPushed ^ bWasPushed) ||
                  (m_biHide.bRaised ^ bWasRaised)|| (bActiveOld != m_bActive);

    if (bNeedPaint)
        SendMessage(WM_NCPAINT);
}

/////////////////////////////////////////////////////////////////////////
// CSCBButton

CSCBButton::CSCBButton()
{
    bRaised = TRUE;
    bPushed = FALSE;
	m_bActive=FALSE;
}

void CSCBButton::Paint(CDC* pDC)
{
    CRect rc = GetRect();
	rc.InflateRect(1,1);
		if (bPushed)
	        pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW),
		        ::GetSysColor(COLOR_BTNHIGHLIGHT));
		else
		{
			
			if (m_Stylo == 1   )
				{  //== office 97
					rc.InflateRect(1,1);
					pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNHIGHLIGHT),
						::GetSysColor(BLACK_PEN));
					CPen Dark(PS_SOLID,1,GetSysColor(COLOR_BTNSHADOW));
					
					CPen* pOld=pDC->SelectObject(&Dark);
					pDC->MoveTo(rc.left+1,rc.bottom-2);
					pDC->LineTo(rc.right-2,rc.bottom-2);
					pDC->MoveTo(rc.right-2,rc.top+1);
					pDC->LineTo(rc.right-2,rc.bottom-1);
					pDC->SelectObject(pOld);					
			}
			else if (m_Stylo == 1 && !bRaised) // == office 2000
			{
				CBrush cbn;
				cbn.CreateSysColorBrush(COLOR_BTNFACE);
					pDC->FillRect(rc,&cbn);	
			}
			else if (bRaised)
			{
				pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNHIGHLIGHT),
					::GetSysColor(BLACK_PEN));
				rc.DeflateRect(1,1);
				pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNFACE),
					::GetSysColor(COLOR_BTNSHADOW));
			}	
		}
	
    COLORREF clrOldTextColor = pDC->GetTextColor();
    if(!m_bActive)
		pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
	else
		pDC->SetTextColor(pDC->SetTextColor(RGB(255,255,255)));
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);
    CFont font;
    int ppi = pDC->GetDeviceCaps(LOGPIXELSX);
    int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi
    font.CreatePointFont(pointsize, _T("Marlett"));
    CFont* oldfont = pDC->SelectObject(&font);

    pDC->TextOut(ptOrg.x + 2, ptOrg.y + 2, CString(_T("r"))); // x-like

    pDC->SelectObject(oldfont);
    pDC->SetBkMode(nPrevBkMode);
    pDC->SetTextColor(clrOldTextColor);
}
 
void CSizingControlBarG::setStyleG(StyleG style) 
{
		m_style=style;
		SendMessage(WM_NCPAINT);

}

BOOL CSizingControlBarG::HasGripper() const
{
#if defined(_SCB_MINIFRAME_CAPTION) || !defined(_SCB_REPLACE_MINIFRAME)
    // if the miniframe has a caption, don't display the gripper
    if (IsFloating())
        return FALSE;
#endif //_SCB_MINIFRAME_CAPTION

    return TRUE;
}