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
#include "resource.h"
#include "guiworkpanel.h"
#include "guipanelAccess.h"


CGuiPanelAccess::CGuiPanelAccess(void)
{
	m_PressDown=FALSE;
	m_PressUp=FALSE;
	m_nDif=0;
}

CGuiPanelAccess::~CGuiPanelAccess(void)
{
}

void  CGuiPanelAccess::RecalLayout()
{
	if (!GetSafeHwnd()) return;
	CRect rcClient;
	CClientDC dc(this);
	GetClientRect(rcClient);
	rcClient.DeflateRect(1,1);
	CRect rctemp=rcClient;
	rctemp.top+=7;
	int nRealval=0;

	rctemp.top+=m_nDif;
		
	int nTipo=0;
	m_nResultTools=m_nDif;
	for (int i=0; i < m_nItems+1;i++)
	{
		
		CComponents* m_cwnd=(CComponents*) m_arrContainer[i];
		
		if(m_cwnd->bTypeSmall == TRUE)
			nTipo=18;
		m_nResultTools+=nTipo;
		rctemp.bottom=rctemp.top+nTipo;
		m_cwnd->m_cwnd->MoveWindow(rctemp);
		if((rctemp.top < rcClient.top+7) || (rctemp.top > rcClient.bottom-25))
			m_cwnd->m_cwnd->ShowWindow(SW_HIDE);
		else 
			m_cwnd->m_cwnd->ShowWindow(SW_SHOW);
		nRealval+=nTipo;

		rctemp.top+=nTipo;
		
	}
	
	m_ToolDown.ShowWindow(SW_HIDE);
	m_ToolUp.ShowWindow(SW_HIDE);
	m_PressDown=TRUE;
	m_PressUp=TRUE;

	if (rcClient.Height() > 1)
	{
		if(nRealval+8 > rcClient.Height())
		{
			if (m_nDif < 0)
			{
				m_ToolUp.ShowWindow(SW_SHOW);
				m_ToolUp.MoveWindow(0,0,rcClient.Width(),8);
			}
			if (m_nDif==0 || m_nResultTools > rcClient.bottom-25)
			{
				m_ToolDown.ShowWindow(SW_SHOW);
				m_ToolDown.MoveWindow(0,rcClient.bottom-8,rcClient.Width(),8);
			}
		}
		else
		{
			if (m_nDif < 0)
			{
				m_ToolUp.ShowWindow(SW_SHOW);
				m_ToolUp.MoveWindow(0,0,rcClient.Width(),8);
			}
		}
		
	}
	

	
}


void  CGuiPanelAccess::SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask)
{
	CBitmap cbmp;
	BITMAP bmp;
	m_imgList.Create(nBitmapID,cx,nGrow,crMask);
	cbmp.LoadBitmap(nBitmapID);
	cbmp.GetBitmap(&bmp);
	m_imgList.Add(&cbmp,crMask);

}


CGuiToolButton* CGuiPanelAccess::AddButton(UINT ID_BUTTON,int nNumImage)
{
	CGuiToolButton* m_Linkbtn;
	m_Linkbtn= (CGuiToolButton*)AddComponen(RUNTIME_CLASS(CGuiToolButton),ID_BUTTON,WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW);
//	m_Linkbtn->SethIcon(m_imgList.ExtractIcon(nNumImage));
	return m_Linkbtn;
}

BOOL CGuiPanelAccess::OnCommand(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND) lParam;	
	for (int i=0; i < m_nItems+1;i++)
	{
			if (((CComponents*) m_arrContainer[i])->m_cwnd->m_hWnd
				== hwnd)
			{
				int x=((CComponents*) m_arrContainer[i])->m_cwnd->GetDlgCtrlID();
				CWnd* pParent= GetParent();
				pParent->SendMessage (WM_COMMAND,x);
				AfxGetMainWnd()->SendMessage(WM_COMMAND,x);
				return TRUE;
			}
	
	}
	if (wParam == ID_TOOLUP)
		OnUp();
	
	if (wParam == ID_TOOLDOWN)
		OnDownd();
	
	return FALSE;

}


void  CGuiPanelAccess::OnDownd()
{
	m_nDif--;
	m_PressDown=TRUE;
	m_PressUp=FALSE;
	RecalLayout();
}
void  CGuiPanelAccess::OnUp()
{
	m_nDif++;
	m_PressDown=FALSE;
	m_PressUp=TRUE;
	RecalLayout();
}

BEGIN_MESSAGE_MAP(CGuiPanelAccess, CGuiContainer)
ON_WM_CREATE()
ON_WM_PAINT()
ON_COMMAND(ID_TOOLUP,OnUp)
ON_COMMAND(ID_TOOLDOWN,OnDownd)
END_MESSAGE_MAP()

int CGuiPanelAccess::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiContainer::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CBitmap cbmp;
	BITMAP bmp;
	m_imgListLocal.Create(IDB_DOCKBAR,9,11,RGB(255,0,255));
	cbmp.LoadBitmap(IDB_DOCKBAR);
	cbmp.GetBitmap(&bmp);
	m_imgListLocal.Add(&cbmp,RGB(255,0,255));
	SetColor(GuiDrawLayer::GetRGBColorFace());
	m_ToolUp.Create(_T(""),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,CRect(0,0,0,0),this,ID_TOOLUP);
	m_ToolUp.SetImage(m_imgListLocal.ExtractIcon(8));
	m_ToolUp.SetStyleBorder(CGuiButton::STYLE_FLAT);
	m_ToolDown.Create(_T(""),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,CRect(0,0,0,0),this,ID_TOOLDOWN);
	m_ToolDown.SetImage(m_imgListLocal.ExtractIcon(0));
	m_ToolDown.SetStyleBorder(CGuiButton::STYLE_FLAT);
	m_ToolUp.ShowWindow(SW_HIDE);
	m_ToolDown.ShowWindow(SW_HIDE);
	SetDrawBorder();
	return 0;
}

void CGuiPanelAccess::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CGuiContainer::OnPaint() for painting messages
	CRect rcClient;
	GetClientRect(rcClient);
	CBrush cb;
	cb.CreateSolidBrush(GuiDrawLayer::GetRGBColorFace());
	dc.FillRect(rcClient,&cb);
	

	if (m_ToolUp.IsWindowVisible())
		m_ToolUp.MoveWindow(0,0,rcClient.Width(),8);
	else
		m_ToolUp.MoveWindow(0,0,0,0);

	if (m_ToolDown.IsWindowVisible())
		m_ToolDown.MoveWindow(0,rcClient.bottom-8,rcClient.Width(),8);
	else
		m_ToolDown.MoveWindow(0,0,0,0);
	
}
