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
#include "GuiLib.h"
#include "GuiPowerPointView.h"
#include "GuiDrawLayer.h"

// CGuiPowerPointView

IMPLEMENT_DYNCREATE(CGuiPowerPointView, CView)

CGuiPowerPointView::CGuiPowerPointView()
{
}

CGuiPowerPointView::~CGuiPowerPointView()
{
}

BEGIN_MESSAGE_MAP(CGuiPowerPointView, CView)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CGuiPowerPointView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	m_tbpp.MoveWindow(0,0,cx,cy);

}

int CGuiPowerPointView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_tbpp.Create(WS_VISIBLE|WS_CHILD,
			CRect(0,0,0,0), this, 0))
		return -1;
	m_tbpp.SetColorTabs(GuiDrawLayer::GetRGBColorFace());
	m_tbpp.SetAutoAjust(TRUE);
	m_tbpp.ShowButtonClose(TRUE);
	return 0;
}

// CGuiPowerPointView drawing

void CGuiPowerPointView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CGuiPowerPointView diagnostics

#ifdef _DEBUG
void CGuiPowerPointView::AssertValid() const
{
	CView::AssertValid();
}

void CGuiPowerPointView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CGuiPowerPointView message handlers
