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
#include "GuiDockToolBar.h"
#include "GuiDrawlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuiDockToolBar

CGuiDockToolBar::CGuiDockToolBar()
{
	m_clrface=GuiDrawLayer::GetRGBColorXP();
	m_Size=16;
}

CGuiDockToolBar::~CGuiDockToolBar()
{

}


BEGIN_MESSAGE_MAP(CGuiDockToolBar, CControlBar)
	//{{AFX_MSG_MAP(CGuiDockToolBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGuiDockToolBar message handlers

BOOL CGuiDockToolBar::Create(CWnd* pParentWnd,DWORD dwAling) 
{
	// TODO: Add your specialized code here and/or call the base class
/*		ASSERT (  (dwAling & CBRS_ALIGN_TOP) &&
				  (dwAling & CBRS_ALIGN_RIGHT) &&
				  (dwAling & CBRS_ALIGN_BOTTOM) &&
				  (dwAling & CBRS_ALIGN_LEFT) );
*/	ASSERT ( dwAling & CBRS_ALIGN_ANY);
	SetBarStyle (dwAling);
	DWORD dwStyle = CCS_NOPARENTALIGN|CCS_NOMOVEY|CCS_NODIVIDER|CCS_NORESIZE;
	if (pParentWnd->GetStyle() & WS_THICKFRAME)
		dwStyle |= SBARS_SIZEGRIP;
	dwStyle |= WS_CHILD|WS_VISIBLE;
	return CControlBar::Create(0,0,dwStyle,
						CRect(0,0,0,0),pParentWnd,AFX_IDW_STATUS_BAR);
}

int CGuiDockToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CGuiDockToolBar::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBrush cbr;
	cbr.CreateSolidBrush(m_clrface);
	CRect m_rect;
	GetClientRect(m_rect);
	pDC->FillRect(m_rect,&cbr);
	return TRUE;

}


void CGuiDockToolBar::OnSysColorChange()
{
	m_clrface=GuiDrawLayer::GetRGBColorXP();
	CControlBar::OnSysColorChange();
}

void CGuiDockToolBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CControlBar::OnPaint() for painting messages
}


void CGuiDockToolBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// TODO: Add your message handler code here and/or call default
	CControlBar::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CGuiDockToolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
}

CSize CGuiDockToolBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	if (bHorz)
		return CSize(32767,m_Size);
	else
		return CSize(m_Size,32767);
}


