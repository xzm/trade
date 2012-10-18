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
#include "GuiOutLookView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CGuiOutLookView,CView)

CGuiOutLookView::CGuiOutLookView()
{

}

CGuiOutLookView::~CGuiOutLookView()
{

}


BEGIN_MESSAGE_MAP(CGuiOutLookView, CView)
	//{{AFX_MSG_MAP(COutlookBarView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutlookBarView drawing

void CGuiOutLookView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// COutlookBarView diagnostics

#ifdef _DEBUG
void CGuiOutLookView::AssertValid() const
{
	CView::AssertValid();
}

void CGuiOutLookView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// COutlookBarView message handlers

void CGuiOutLookView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
}

void CGuiOutLookView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	cf.MoveWindow(0,0,cx,cy);

}

int CGuiOutLookView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!cf.Create(WS_VISIBLE|WS_CHILD,
			CRect(0,0,0,0), this, 0))
		return -1;
	return 0;
}


