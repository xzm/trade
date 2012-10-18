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
#include "GuiWorkTab.h"
#include "GuiBaseTab.h"
#include "resource.h"
#include "GuiToolButton.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ID_WTABSPIN		501
#define ID_SCROLLWINDOW 502
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MIN_SCROLL ::GetSystemMetrics(SM_CXHSCROLL)*2

CGuiWorkTab::CGuiWorkTab()
{
	SetStyle(CGuiTabWnd::SHADOW);
	// Modified By SunZhenyu
	if( !m_cfont.CreateStockObject( DEFAULT_GUI_FONT ) )
		m_cfont.CreateStockObject( ANSI_VAR_FONT );
	// m_cfont.CreateFont(14,0,0,0,400,0,0,0,0,1,2,1,34,"Arial");
	m_deplaza=0;
	m_btndesplaza.SetRectEmpty();
	bIniciaArrastre=FALSE;
	m_EnableL=FALSE;
	m_EnableR=FALSE;
}

CGuiWorkTab::~CGuiWorkTab()
{

}


BEGIN_MESSAGE_MAP(CGuiWorkTab,CGuiTabWnd)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(ID_SPINIZ,OnSpiNiz)
	ON_COMMAND(ID_SPINDE,OnSpiNde)	
END_MESSAGE_MAP()


void CGuiWorkTab::PintaSeparador(CDC* dc)
{
	CRect m_rClient=m_rectTab;
		//se pinta el boton de desplazamiento
	if (m_btndesplaza.IsRectNull())
	{
		   /*Modified By SunZhenyu 2003/6/21
		   m_rClient.left=m_rClient.Width()/2;
		   */
		   m_rClient.left=5*m_rClient.Width()/6;
		   m_rClient.top-=3;
		   m_rClient.left-=5;
		   m_rClient.right=m_rClient.left+5;	
		   m_rClient.bottom-=1;
		   m_btndesplaza=m_rClient;
		   m_scroll.SetWindowPos(NULL,m_btndesplaza.right ,m_rectTab.top,m_rectTab.right-(m_rClient.left+5),m_rectTab.Height()-1,SWP_NOACTIVATE|SWP_NOCOPYBITS);	        
  		   m_scroll.Invalidate();
		   m_scroll.UpdateWindow();
		   return;
	}
	else
	{
		CRect rcCli;
		GetClientRect(&rcCli);
     	if (m_btndesplaza.left+40 > rcCli.right)
		{
			m_btndesplaza.left=	rcCli.right-40;
			m_btndesplaza.right=m_btndesplaza.left+5;
		}
		m_btndesplaza.top=m_rectTab.top;
		m_btndesplaza.bottom=m_rectTab.bottom;
		m_rClient=m_btndesplaza;
	}
	if (!bIniciaArrastre)
	{
		
		dc->Draw3dRect(m_rClient, ::GetSysColor(BLACK_PEN),
					  ::GetSysColor(COLOR_BTNHIGHLIGHT));
		m_rClient.DeflateRect(1,1);
		dc->Draw3dRect(m_rClient, ::GetSysColor(COLOR_BTNHIGHLIGHT),
								  ::GetSysColor(COLOR_BTNSHADOW));
	}
}

void CGuiWorkTab::OnSpiNiz()
{
	if(m_EnableL==TRUE)
	{
		 m_PresLeft=TRUE;
		 m_PresRight=FALSE;
		 m_deplaza+=20;
		 CClientDC dc(this);
		 Drawtabs(&dc);
	}
}
void CGuiWorkTab::OnSpiNde()
{
	if(m_EnableR==TRUE)
	{
		m_PresLeft=FALSE;
		m_PresRight=TRUE;
		m_deplaza-=20; 	
		CClientDC dc(this);
		Drawtabs(&dc);	
	}
}

// Modified By SunZhenyu, 2003/10/28, add the next function
void CGuiWorkTab::ResetDeplaza( )
{
	m_btndesplaza.SetRectEmpty();
}

void CGuiWorkTab::Drawtabs(CDC* dc)
{
	int m_StrPos=0;
	BOOL m_ViewAlltab=TRUE;
	CRect m_rClient;
	CBrush cbr;
	CPen cp(PS_SOLID,1,::GetSysColor(COLOR_BTNSHADOW));
	int m_ianchoScroll=::GetSystemMetrics (SM_CXHSCROLL) * 2;
	m_EnableR=TRUE;
	m_EnableL=TRUE;
	cbr.CreateSolidBrush(m_wd.GetRGBColorXP());
	GetClientRect(m_rClient);
	CRect rectT=m_rectTab;
	rectT.right=m_btndesplaza.left;
	rectT.right+=4;
	dc->FillRect(rectT,&cbr);
	CPen *pOldPen = dc->SelectObject(&cp);
	dc->MoveTo(m_rectTab.left,m_rectTab.top);
	dc->LineTo(m_btndesplaza.right+1,m_rectTab.top);
	dc->Draw3dRect(m_rClient, ::GetSysColor(COLOR_BTNSHADOW),
				                  ::GetSysColor(COLOR_BTNHIGHLIGHT));
	if (!::IsWindow(m_mSpin.GetSafeHwnd()))
	{
		CRect r=m_rectTab;
		r.right=r.left+35;
		r.top+=1;
		r.bottom-=1;
		if (!m_mSpin.Create(_T(""),WS_VISIBLE | WS_CHILD |SS_OWNERDRAW, 
						r, this))
		m_mSpin.AlingButtons(CGuiMiniTool::ALIGN_LEFT);
		m_mSpin.SetColor(GuiDrawLayer::GetRGBColorFace());
		m_mSpin.SetImageList(IDB_DOCKBAR, 9,10, RGB(255,0,255));
		m_mSpin.AddButton(4,ID_SPINIZ,NULL,_T(""),"Left");
		m_mSpin.AddButton(2,ID_SPINDE,NULL,_T(""),"Right");
		m_mSpin.AutoSize(TRUE);
		m_mSpin.SetBorder(CGuiMiniTool::STYLEFRAME);
		CGuiToolButton* btn=m_mSpin.GetButton(ID_SPINIZ);
		btn->ShowDark(FALSE);
		btn=m_mSpin.GetButton(ID_SPINDE);
		btn->ShowDark(FALSE);

	}
	
	if (!::IsWindow(m_scroll.GetSafeHwnd()))
	{
		m_scroll.Create(WS_VISIBLE|WS_CHILD|UDS_HORZ,m_rectTab,this,ID_SCROLLWINDOW);
		
	}
	
	CRect rcSpin;
	m_mSpin.GetClientRect(rcSpin);m_StrPos=rcSpin.Width();
	if ((m_btndesplaza.left < rcSpin.right) && ! m_btndesplaza.IsRectEmpty())
	{
		m_btndesplaza.left=rcSpin.right+5;
		m_btndesplaza.right=m_btndesplaza.left+5;
	}
	PintaSeparador(dc);
	int m_Space=8;
	int m_Pos=10;
	int m_right=m_StrPos;
	if (m_iMinValRec-26 > m_btndesplaza.left) //No existe suficioente espacio para los tabs
		{
			m_ViewAlltab=FALSE;
			//m_spin.EnableScrollBar();
			if (m_PresLeft==TRUE)
				m_right+=m_deplaza;
			if (m_PresRight==TRUE)
				m_right+=m_deplaza;
			if (m_deplaza == 0)
			{
				m_EnableL=FALSE;
				m_EnableR=TRUE;
				;//m_spin.EnableScrollBar(ESB_DISABLE_LTUP);
			}
			if ((m_iMinValRec+m_deplaza)-26 <= m_btndesplaza.left)
			{
				;//m_spin.EnableScrollBar(ESB_DISABLE_RTDN);
				m_EnableR=FALSE;
				m_EnableL=TRUE;
			}
		
		}
		else
		{
			m_EnableL=FALSE;
			m_EnableR=FALSE;
			//m_spin.EnableScrollBar(ESB_DISABLE_BOTH);
			m_ViewAlltab=TRUE;
			m_deplaza=0;
		}
	CRect rectScroll; 
	CRgn reg;	
	CRect rectRecorte;
	rectRecorte.left=m_rectTab.left+m_ianchoScroll;
	rectRecorte.top=m_rectTab.top;
	rectRecorte.bottom=m_rectTab.bottom;
	rectRecorte.right=m_btndesplaza.left;
	reg.CreateRectRgnIndirect(rectRecorte);
	dc->SelectClipRgn(&reg);
	for ( int iCont=0; iCont< m_Numtabs;iCont++)
	{	

		//;
		CGuiTab* ctb=(CGuiTab*)m_pArray[iCont];
		CRect mrt=ctb->rect;		
		int ancho=mrt.Width();
		mrt.left=m_right;
		mrt.right=mrt.left+ancho;
		CPoint Coor[4];
			//dibujar la hoja seleccionada
			if (iCont == 0 )
			{

				
				dc->MoveTo(mrt.left,mrt.top);
				dc->LineTo((mrt.left)+m_rectTab.Height()/2,mrt.bottom-1);
				dc->MoveTo((mrt.left)+m_rectTab.Height()/2,mrt.bottom-1);
				dc->LineTo((mrt.right)-m_rectTab.Height()/2,mrt.bottom-1);
				dc->MoveTo((mrt.right)-m_rectTab.Height()/2,mrt.bottom-1);
				if (m_iSelectTab ==iCont)
					dc->LineTo(mrt.right-2,mrt.top+2);
				else
					dc->LineTo((mrt.right)-3,mrt.top+(m_rectTab.Height()/2));
				
				m_right=mrt.right;
				
			}
			else
			{
				//Se dibuja la hoja siguiente a la seleccionada
				//no se dibuja toda la diagonal y no es la ultima hoja
				
				if (iCont-1 == m_iSelectTab ) 
				{	

					
					dc->MoveTo(mrt.left+(m_rectTab.Height()/4)+1,mrt.top+m_rectTab.Height()/2);
					dc->LineTo((mrt.left)+m_rectTab.Height()/2,mrt.bottom-1);
					dc->MoveTo((mrt.left)+m_rectTab.Height()/2,mrt.bottom-1);
					dc->LineTo((mrt.right)-m_rectTab.Height()/2,mrt.bottom-1);
					dc->MoveTo((mrt.right)-m_rectTab.Height()/2,mrt.bottom-1);
					if (iCont == m_Numtabs-1)
						dc->LineTo((mrt.right-2),mrt.top+2);
					else
						dc->LineTo((mrt.right)-3,mrt.top+(m_rectTab.Height()/2));

				}
				else
				{
					
					dc->MoveTo(mrt.left,mrt.top);
					dc->LineTo((mrt.left)+m_rectTab.Height()/2,mrt.bottom-1);
					dc->MoveTo((mrt.left)+m_rectTab.Height()/2,mrt.bottom-1);
					dc->LineTo((mrt.right)-m_rectTab.Height()/2,mrt.bottom-1);
					dc->MoveTo((mrt.right)-m_rectTab.Height()/2,mrt.bottom-1);
					if (iCont == m_iSelectTab || iCont == m_Numtabs-1 )
						dc->LineTo((mrt.right-2),mrt.top+2);
					else
						dc->LineTo((mrt.right)-3,mrt.top+3+(m_rectTab.Height()/2));
					
				}
				m_right=mrt.right;
			}
			if (iCont == m_iSelectTab )
			{
				
				Coor[0].x=mrt.left;
				Coor[0].y=mrt.top;
				Coor[1].x=(mrt.left)+m_rectTab.Height()/2;
				Coor[1].y=mrt.bottom-1;
				Coor[2].x=(mrt.right)-m_rectTab.Height()/2; 
				Coor[2].y=mrt.bottom-1;
				Coor[3].x=(mrt.right-1);
				Coor[3].y=mrt.top+3;
				
				/* Modified By SunZhenyu, 2003/11/7, add the next 2 line */
				CBrush	brush( m_clrSelectTabBK );
				CBrush * pOldBrush = dc->SelectObject( &brush );
				dc->Polygon(Coor,4);
				/* Modified By SunZhenyu, 2003/11/7, add the next 1 line */
				dc->SelectObject( pOldBrush );
				dc->SelectStockObject(WHITE_PEN);
				dc->MoveTo(mrt.left+3,mrt.top+3);
				dc->LineTo(mrt.right-1,mrt.top+3);
				dc->MoveTo(mrt.left+2,mrt.top+2);
				dc->LineTo(mrt.right,mrt.top+2);
				dc->SelectObject(&cp);
				/* Modified By SunZhenyu, 2003/11/9, add the next 1 line */
				dc->SetTextColor(m_clrSelectTabText);
			}
			else
				/* Modified By SunZhenyu, 2003/11/9, add the next 1 line */
				dc->SetTextColor(::GetSysColor(COLOR_BTNTEXT));

			CFont* m_fontOld=dc->SelectObject(&m_cfont);
			dc->SetBkMode(TRANSPARENT);
			CRect mrtt=mrt; mrtt.top+=2;
			dc->DrawText(ctb->lpMsg,mrtt,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			// Modified By SunZhenyu
			dc->SelectObject(m_fontOld);
			ctb->rect=mrt;
			m_right-=(m_rectTab.Height()/2);	
			if (!bIniciaArrastre)
			{
				CRect m_rectCli;
				GetClientRect(m_rectCli);
				if (m_btndesplaza.left <m_rectCli.left+rcSpin.left+1)
				{
					m_btndesplaza.left=m_rectCli.left+rcSpin.left+5;
					m_btndesplaza.right=m_btndesplaza.left+5;
					PintaSeparador(dc);
				}
				m_scroll.SetWindowPos(NULL,m_btndesplaza.right ,m_rectTab.top,m_rectCli.right-(m_btndesplaza.left+5),m_rectTab.Height()-1,SWP_NOACTIVATE|SWP_NOCOPYBITS);	        
				m_scroll.Invalidate();
				m_scroll.UpdateWindow();	
				m_mSpin.SetWindowPos(NULL,m_rectTab.left,m_rectTab.top,m_ianchoScroll,m_rectTab.Height()-2,SWP_NOACTIVATE|SWP_NOZORDER);			
			}
			
		}
		
		dc->SelectClipRgn(NULL);
		// Modified By SunZhenyu
		dc->SelectObject(pOldPen);

		m_mSpin.Invalidate();
		m_mSpin.UpdateWindow();	
}


void CGuiWorkTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);

	/*Modified By SunZhenyu 2003/10/22, Remove the next 1 line
	if (pScrollBar == (CScrollBar*)&m_scroll )
	*/
	{
		CGuiTab* ct=(CGuiTab*) m_pArray[m_iSelectTab];
		CWnd* ViewAc=ct->pParent;
		int rangmin,rangMax;
		ViewAc->SendMessage(WM_HSCROLL,MAKEWPARAM(nSBCode,nPos));
		ViewAc->GetScrollRange(SB_HORZ,&rangmin,&rangMax);
		m_scroll.SetScrollRange(rangmin,rangMax);
		m_scroll.SetScrollPos(ViewAc->GetScrollPos(SB_HORZ));
		
		return;
	}
    CClientDC dc(this);
	Drawtabs(&dc);
	
	
		
	
}


BOOL CGuiWorkTab::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
		CPoint ptCurPos;
		GetCursorPos (&ptCurPos);
		ScreenToClient (&ptCurPos);

		if (m_btndesplaza.PtInRect (ptCurPos))
		{
			SetCursor (LoadCursor(NULL, IDC_SIZEWE));
			return TRUE;
		}
	
	return CGuiTabWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CGuiWorkTab::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_btndesplaza.PtInRect(point) && !bIniciaArrastre)
		{
			bIniciaArrastre=TRUE;		
			SetCapture ();
			InvalidateRect(m_btndesplaza,TRUE);
		}
	
	else	CGuiTabWnd::OnLButtonDown(nFlags, point);
	
}


void CGuiWorkTab::OnLButtonUp(UINT nFlags, CPoint point) 
{
		if (bIniciaArrastre==TRUE)
		{
			bIniciaArrastre=FALSE;
			int iDiferencia=m_rectTab.right-31;
			if (point.x < m_rectTab.left+31) point.x=m_rectTab.left+31;
			if (point.x > iDiferencia ) point.x= iDiferencia;
			m_btndesplaza.left=point.x;
			m_btndesplaza.right=m_btndesplaza.left+5;
			ReleaseCapture ();	
			Invalidate();
			UpdateWindow();
				
		}
		
	
		CGuiTabWnd::OnLButtonUp(nFlags, point);

}

void CGuiWorkTab::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect m_rectCli;
	GetClientRect(m_rectCli);
    if (bIniciaArrastre==TRUE)
    {
		int iDiferencia=m_rectCli.right-31;
		if (point.x < m_rectTab.left+31) point.x=m_rectTab.left+31;
		if (point.x > iDiferencia ) point.x= iDiferencia;
		m_scroll.SetWindowPos(NULL,point.x ,m_rectTab.top,m_rectCli.right-point.x,m_rectTab.Height()-1,	SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOCOPYBITS);	        
		m_scroll.Invalidate();
		m_scroll.UpdateWindow();
		m_btndesplaza.left=point.x;
		m_rectCli=m_btndesplaza;
		m_rectCli.left-=5;
		InvalidateRect(m_btndesplaza,TRUE);
		UpdateWindow();
		
	}
	else
		CGuiTabWnd::OnMouseMove(nFlags, point);
}
void CGuiWorkTab::OnSysColorChange() 
{
	CGuiTabWnd::OnSysColorChange();

		Invalidate ();
		UpdateWindow ();
}

