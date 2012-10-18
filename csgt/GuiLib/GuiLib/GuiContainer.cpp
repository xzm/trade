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
#include "GuiContainer.h"
#include "GuiVisioFolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuiContainer

CGuiContainer::CGuiContainer()
{
	m_arrContainer.RemoveAll();
	m_nItems=0;
	m_clrface=GuiDrawLayer::GetRGBColorFace();
	m_nItems = -1;
	m_bUseWin=FALSE;
	m_bDrawBorder=TRUE;
}

CGuiContainer::~CGuiContainer()
{
	/* Modified By SunZhenyu 2003/8/22, Add next 7 lines. */
	for(int i=0; i<m_arrContainer.GetSize(); i++)
	{
		CComponents * p = (CComponents*)m_arrContainer[i];
		if( p )
			delete	p;
	}
	m_arrContainer.RemoveAll();
}


BEGIN_MESSAGE_MAP(CGuiContainer, CWnd)
	//{{AFX_MSG_MAP(CGuiContainer)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuiContainer message handlers

int CGuiContainer::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

BOOL CGuiContainer::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CWnd* pParent= GetParent();
	pParent->SendMessage (WM_COMMAND,wParam);
	return TRUE;
	

}
//********************************************************************************
void CGuiContainer::OnPaint()  
{
	CPaintDC dc(this); // device context for painting
	CBrush cbr;
	CRect m_rectDraw;
	GetClientRect(&m_rectDraw);
	cbr.CreateSolidBrush(m_clrface);
	dc.FillRect(&m_rectDraw,&cbr);
	if (m_enBorder == STYLE3D)
		dc.Draw3dRect(m_rectDraw,GuiDrawLayer::GetRGBColorBTNHigh(),
			GuiDrawLayer::GetRGBColorShadow());
	if (m_enBorder == STYLEPRESS)
	{
		dc.Draw3dRect(m_rectDraw,GuiDrawLayer::GetRGBColorShadow(),
			GuiDrawLayer::GetRGBColorBTNHigh());
	}
	RecalLayout();

}


BOOL CGuiContainer::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	// TODO: Add your specialized code here and/or call the base class
	pParent= pParentWnd;
	return CWnd::Create(NULL, _T(""), dwStyle, rect, pParentWnd, nID);
}

//********************************************************************************

void CGuiContainer::OnSysColorChange() 
{
	CWnd::OnSysColorChange();
	m_clrface=GuiDrawLayer::GetRGBColorFace();
	for (int i=0; i < m_nItems+1;i++)
	{
		
		CComponents* m_cwnd=(CComponents*) m_arrContainer[i];
		if(m_cwnd->bMiniTool==TRUE)
			m_cwnd->m_guiMini->SetColor(m_clrface);
	}
	RecalLayout();
}

//********************************************************************************
void CGuiContainer::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	RecalLayout();
}

//********************************************************************************
BOOL CGuiContainer::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
void CGuiContainer::UsedAllWindow(BOOL bUseWindow)
{
	m_bUseWin=bUseWindow;
}
//********************************************************************************
/*void CGuiContainer::PreSubclassWindow() 
{
	CWnd::PreSubclassWindow();
	RecalLayout();
}
*/
//********************************************************************************
void  CGuiContainer::RecalLayout()
{
	if (!GetSafeHwnd()) return;
	CRect rcClient;
	CClientDC dc(this);
	GetClientRect(rcClient);
	rcClient.DeflateRect(1,1);
	CRect rctemp=rcClient;
	int nTipo=0;
	CalcAutoSize(rcClient);
	for (int i=0; i < m_nItems+1;i++)
	{
		
		CComponents* m_cwnd=(CComponents*) m_arrContainer[i];
		
		if(m_cwnd->bMiniTool==TRUE || m_cwnd->bTypeSmall == TRUE)
			nTipo=24;
		else
			nTipo=m_nResultCWnd;
		
		rctemp.bottom=rctemp.top+nTipo;
		
		if (m_cwnd->bMiniTool==TRUE)
			m_cwnd->m_guiMini->MoveWindow(rctemp);
		else if (nTipo != 0)
		{
			if (m_bDrawBorder)
				dc.Draw3dRect(rctemp,GuiDrawLayer::GetRGBColorShadow(),
				GuiDrawLayer::GetRGBColorShadow());
				
				if (m_bUseWin==FALSE)
					rctemp.DeflateRect(2,2);
				
				m_cwnd->m_cwnd->MoveWindow(rctemp);
				
				if (m_bUseWin==FALSE)
					rctemp.InflateRect(2,2);
		}
		rctemp.top+=nTipo;

	}
}

//********************************************************************************
void CGuiContainer::CalcAutoSize(CRect m_rc)
{

	if (m_nItems == -1) return;
	m_nResultCWnd=0;
    m_nResultTools=0; 	
	int m_numCWnd=0;
	BOOL m_bExisteCWnd=FALSE;
	for (int i=0; i < m_nItems+1;i++)
	{
		if (((CComponents*) m_arrContainer [i])->bMiniTool==TRUE ||
			((CComponents*) m_arrContainer [i])->bTypeSmall ==TRUE)
			m_nResultTools+=24;
		else
		{
				m_bExisteCWnd=TRUE;	
				m_numCWnd++;
		}
	}

	
	if (m_bExisteCWnd)
	{
		if (m_nResultTools > m_rc.Height())  //el espacio no alcanza para los botones
			m_nResultCWnd=0;
		else
		{
			m_nResultCWnd=m_rc.Height()-m_nResultTools;
			m_nResultCWnd/=m_numCWnd;
		}
	}

}
//********************************************************************************
BOOL CGuiContainer::AddComponen(CGuiMiniTool* m_miniTool)
{
	m_nItems++;
	if (!m_miniTool->Create(_T(""),WS_VISIBLE | WS_CHILD |SS_OWNERDRAW, 
						CRect(0,0,0,0), this))
		return FALSE;
	m_arrContainer.SetAtGrow(m_nItems, new CComponents(m_miniTool));
	RecalLayout();
	return TRUE;

}
//********************************************************************************
void CGuiContainer::SetBorder(Borders m_enborder)
{
	m_enBorder=m_enborder;
}

//********************************************************************************
CWnd*  CGuiContainer::AddComponen(CRuntimeClass* m_rtcwnd,UINT nID,DWORD dwStyle)
{
	
	CWnd* m_cwnd=NULL;
	BOOL btype=FALSE; //only object small
	if (m_rtcwnd->IsDerivedFrom(RUNTIME_CLASS(CComboBox)))
	{
		 m_cwnd = new CGuiComboBoxExt();
		 if (!((CGuiComboBoxExt*)m_cwnd)->Create(dwStyle|WS_CHILD|WS_VISIBLE, CRect(1,1,100,100), this, nID))
			return NULL;
		 btype=TRUE;
		 goto crear;
	}

	if (m_rtcwnd->IsDerivedFrom(RUNTIME_CLASS(CTreeCtrl)))
	{
		 m_cwnd = new CTreeCtrl();
		 if (!((CTreeCtrl*)m_cwnd)->Create(dwStyle|WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, nID))
			return NULL;
		 goto crear;
	}

	else if ( m_rtcwnd->IsDerivedFrom(RUNTIME_CLASS(CListCtrl)))
	{
		m_cwnd = new CListCtrl();
	    if (!((CListCtrl*)m_cwnd)->Create(dwStyle|WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, nID))
			return NULL;
		goto crear;

	}
	else if ( m_rtcwnd->IsDerivedFrom(RUNTIME_CLASS(CListBox)))
	{
		 m_cwnd = new CListBox();
		 if (!((CListBox*)m_cwnd)->Create(dwStyle|WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, nID))
			return NULL;
		 goto crear;
	}
	else if ( m_rtcwnd->IsDerivedFrom(RUNTIME_CLASS(CEdit)))
	{
		 m_cwnd = new CEdit();
		 if (!((CEdit*)m_cwnd)->Create(dwStyle|WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, nID))
			return NULL;
		 btype=TRUE;
		 goto crear;
	}
	
	else if ( m_rtcwnd->IsDerivedFrom(RUNTIME_CLASS(CGuiImageLinkButton)))
	{
		 m_cwnd = new CGuiImageLinkButton();
		 if (!((CGuiImageLinkButton*)m_cwnd)->Create(_T(""),dwStyle|WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, nID))
			return NULL;
		 btype=TRUE;
		 goto crear;
	}
	else if ( m_rtcwnd->IsDerivedFrom(RUNTIME_CLASS(CGuiLabelButton)))
	{
		 m_cwnd = new CGuiLabelButton();
		 if (!((CGuiLabelButton*)m_cwnd)->Create(_T(""),dwStyle|WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, nID))
			return NULL;
		 btype=TRUE;
		 goto crear;
	}
	else if ( m_rtcwnd->IsDerivedFrom(RUNTIME_CLASS(CListCtrl)))
	{
		 m_cwnd = new CListCtrl();
		 if (!((CListCtrl*)m_cwnd)->Create(dwStyle|WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, nID))
			return NULL;
		 btype=TRUE;
		 goto crear;
	}
	else if ( m_rtcwnd->IsDerivedFrom(RUNTIME_CLASS(CGuiVisioFolder)))
	{
		 m_cwnd = new CGuiVisioFolder();
		 if (!((CGuiVisioFolder*)m_cwnd)->Create(dwStyle|WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, nID))
			return NULL;
		 goto crear;
	}
	else if ( m_rtcwnd->IsDerivedFrom(RUNTIME_CLASS(CGuiToolButton)))
	{
		 m_cwnd = new CGuiToolButton();
		 if (!((CGuiToolButton*)m_cwnd)->Create(_T(""),
			 dwStyle|WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, nID))
			 btype=TRUE;
			return NULL;
		 goto crear;
	}
	else
	{
		   m_cwnd=(CWnd*) m_rtcwnd->CreateObject();
		   if (!m_cwnd->Create(0,0,dwStyle|WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, nID))
			return NULL;
		   goto crear;
	}
crear:
	m_nItems++;
	m_arrContainer.SetAtGrow(m_nItems, new CComponents(m_cwnd,btype));
	RecalLayout();
	return m_cwnd;

}

//*************************************************************************
CComponents::CComponents(CGuiMiniTool* m_GuiMini)
{
	m_guiMini=m_GuiMini;
	bMiniTool=TRUE;
}

//*************************************************************************
CComponents::CComponents(CWnd* m_wnd,BOOL bType)
{
	ASSERT(::IsWindow(m_wnd->m_hWnd));
	m_cwnd=m_wnd;
	bMiniTool=FALSE;
	bTypeSmall=bType;
}


BOOL CGuiContainer::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd* pParent= GetParent();
	if (pParent->GetSafeHwnd())
		pParent->SendMessage(WM_NOTIFY, wParam, lParam);
	
	return TRUE;
}
