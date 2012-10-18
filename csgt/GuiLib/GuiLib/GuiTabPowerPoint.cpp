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
#include "guitabpowerpoint.h"
#include "GuiDrawLayer.h"
#include "GuiBaseTab.h"
#include "resource.h"

#define ID_CLOSE 6500

CGuiTabPowerPoint::CGuiTabPowerPoint(void)
{
	bAutoAjust=TRUE;
	m_clrTabs=GuiDrawLayer::GetRGBColorFace();
	m_bShowButton=TRUE;
}

CGuiTabPowerPoint::~CGuiTabPowerPoint(void)
{
}
BEGIN_MESSAGE_MAP(CGuiTabPowerPoint, CGuiTabWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_CLOSE,OnClose)
END_MESSAGE_MAP()

void CGuiTabPowerPoint::Close()
{

}

BOOL CGuiTabPowerPoint::OnCommand(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND) lParam;	
	if (m_ToolButton.GetSafeHwnd() == hwnd)
		SendMessage (WM_COMMAND,ID_CLOSE);
	return TRUE;

}


void CGuiTabPowerPoint::AjustTabs()
{
	
	int m_iSizeAct=0;
	m_iMinValRec=4;
	CClientDC dc(this);
	GetClientRect(m_rectCliente);
	m_rectCliente.bottom=m_rectCliente.top+(m_sizeImag.cy+4+1);

	CFont* m_fontOld=dc.SelectObject(&m_cfont);
	for (int iCont=0; iCont< m_Numtabs;iCont++)
	{
		CGuiTab* ct=(CGuiTab*) m_pArray[iCont];
		CRect m_Arect;
		CSize m_sChar=dc.GetTextExtent(ct->lpMsg);
		m_Arect.left=m_iMinValRec+1;
		m_iMinValRec+=m_alnTab!=ALN_TOP?m_sizeImag.cx+2+m_sChar.cx+14:m_sChar.cx+14;
		if(m_alnTab==ALN_BOTTOM)
		{
			m_Arect.top=m_rectCliente.bottom;
			m_Arect.right=m_iMinValRec;
			m_Arect.bottom=m_rectCliente.bottom+4+m_sizeImag.cy;
		}
		else
		{
			m_Arect.top=m_rectCliente.top;
			m_Arect.right=m_iMinValRec;
			m_Arect.bottom=m_rectCliente.top+4+m_sizeImag.cy;
		}
		ct->rect= m_Arect;
		
	}		
	dc.SelectObject(m_fontOld);
	
	if (bAutoAjust)
	{
		int m_NewResize=4;
		if (m_iMinValRec > m_rectCliente.Width()-4)
		{
			m_iSizeAct=(m_rectCliente.Width()-12)/m_Numtabs;
			for (int iCont=0; iCont< m_Numtabs;iCont++)
			{

				CGuiTab* ct=(CGuiTab*) m_pArray[iCont];
				if (ct->uIcon != -1)
				{
					if (m_sizeImag.cx+22 > m_NewResize+m_iSizeAct)
						continue;
				}
				CRect m_Arect;
				m_Arect.left=m_NewResize;
				m_Arect.right=m_NewResize+m_iSizeAct;
				m_Arect.top=m_alnTab==ALN_TOP? m_rectCliente.top:m_rectCliente.bottom;
				m_Arect.bottom=m_alnTab==ALN_TOP? m_rectCliente.top+4+m_sizeImag.cy:m_rectCliente.bottom+4+m_sizeImag.cy;
				m_NewResize+=m_iSizeAct+1;
				ct->rect= m_Arect;
			}	
		}
	}


}




void CGuiTabPowerPoint::Drawtabs(CDC* dc)
{
	CBrush cb;
	CRect rectText;
	CRect rc;
	int nHeight=0;
	CBrush cbr;
	CPen cp(PS_SOLID,1,::GetSysColor(COLOR_BTNSHADOW));
	CPen cpw(PS_SOLID,1,GuiDrawLayer::GetRGBColorWhite());
	
	cbr.CreateSolidBrush(m_clrTabs);
	dc->FillRect(m_rectTab,&cbr);
	CFont* m_fontOld=dc->SelectObject(&m_cfont);
	CPen penShadow(PS_SOLID, 1, GuiDrawLayer::GetRGBColorShadow());
	CPen* pOldPen = dc->SelectObject(&penShadow);
	/*Modified By SunZhenyu 2003/11/11, add the next 1 line */
	if( ALN_BOTTOM != m_alnTab )
	{
		dc->MoveTo(m_rectTab.left,m_rectTab.bottom-1);
		dc->LineTo(m_rectTab.right,m_rectTab.bottom-1);
	}
	int nMode = dc->SetBkMode(TRANSPARENT);
	int m_right=0;


	for ( int iCont=0; iCont< m_Numtabs;iCont++)
	{	

		dc->SelectObject(&penShadow);
		CGuiTab* ctb=(CGuiTab*)m_pArray[iCont];
		CRect mrt=ctb->rect;		
		int ancho=mrt.Width();
		mrt.left=m_right;
		mrt.right=mrt.left+ancho;
		CPoint Coor[4];
		CPoint CoorFive[6];
				
				if (iCont-1 == m_iSelectTab ) 
				{	
					CBrush brushRed(GuiDrawLayer::GetRGBColorFace());
					CBrush* pOldBrush = dc->SelectObject(&brushRed);
					CoorFive[0].x=mrt.left+(m_rectTab.Height()/4)+1;			
					CoorFive[0].y=(mrt.bottom-m_rectTab.Height()/2)+1;
					CoorFive[1].x=(mrt.left)+m_rectTab.Height()/2;			
					CoorFive[1].y=mrt.top+1;
					CoorFive[2].x=(mrt.right)-m_rectTab.Height()/2;			
					CoorFive[2].y=mrt.top+1;
					
					if (iCont == m_Numtabs-1)
					{
						CoorFive[3].x=mrt.right-2;			
						CoorFive[3].y=mrt.bottom+1;
					}
					else
					{
						CoorFive[3].x=(mrt.right)-5;			
						CoorFive[3].y=(mrt.bottom-(m_rectTab.Height()/2))+1;
					}

					CoorFive[4].x=(mrt.right)-8;			
					CoorFive[4].y=mrt.bottom+1;

					CoorFive[5].x=(mrt.left)+9;			
					CoorFive[5].y=mrt.bottom+1;

					/*Modified By SunZhenyu 2003/11/11, add the next 5 line */
					if( ALN_BOTTOM == m_alnTab )
					{
						for( int k=0; k<6; k++ )
							CoorFive[k].y	=	m_rectTab.bottom - (CoorFive[k].y-m_rectTab.top);
					}
					
					dc->Polygon(CoorFive,6);	
					dc->SelectObject(pOldBrush);
				}
				else
				{
					CBrush brushRed(GuiDrawLayer::GetRGBColorXP());
					CBrush* pOldBrush = dc->SelectObject(&brushRed);

					CoorFive[0].x=mrt.left;			
					CoorFive[0].y=mrt.bottom+1;
					CoorFive[1].x=(mrt.left)+m_rectTab.Height()/2;			
					CoorFive[1].y=mrt.top+1;
					CoorFive[2].x=(mrt.right)-m_rectTab.Height()/2;			
					CoorFive[2].y=mrt.top+1;
					
					if (iCont == m_iSelectTab || iCont == m_Numtabs-1 )
					{
						CoorFive[3].x=mrt.right-2;			
						CoorFive[3].y=mrt.bottom+1;
					}
					else
					{
						CoorFive[3].x=(mrt.right)-5;			
						CoorFive[3].y=(mrt.bottom-(m_rectTab.Height()/2))+1;
					}
					
					CoorFive[4].x=(mrt.right)-8;			
					CoorFive[4].y=mrt.bottom+1;

					CoorFive[5].x=(mrt.left)+9;			
					CoorFive[5].y=mrt.bottom+1;
					
					/*Modified By SunZhenyu 2003/11/11, add the next 5 line */
					if( ALN_BOTTOM == m_alnTab )
					{
						for( int k=0; k<6; k++ )
							CoorFive[k].y	=	m_rectTab.bottom - (CoorFive[k].y-m_rectTab.top);
					}
					
					dc->Polygon(CoorFive,6);	
					dc->SelectObject(pOldBrush);
					
				}
				m_right=mrt.right;
			if (iCont == m_iSelectTab )
			{
				
				/*Modified By SunZhenyu 2003/11/11, replace the next 1 line
				CBrush brushRed(GuiDrawLayer::GetRGBColorWhite());
				*/
				CBrush brushRed(m_clrSelectTabBK);
				CBrush* pOldBrush = dc->SelectObject(&brushRed);
				Coor[0].x=mrt.left;
				Coor[0].y=mrt.bottom;
				Coor[1].x=(mrt.left)+m_rectTab.Height()/2;
				Coor[1].y=mrt.top+1;
				Coor[2].x=(mrt.right)-m_rectTab.Height()/2; 
				Coor[2].y=mrt.top+1;
				Coor[3].x=(mrt.right-1);
				Coor[3].y=mrt.bottom;

				/*Modified By SunZhenyu 2003/11/11, add the next 5 line */
				if( ALN_BOTTOM == m_alnTab )
				{
					for( int k=0; k<4; k++ )
						Coor[k].y	=	m_rectTab.bottom - (Coor[k].y-m_rectTab.top);
				}
				
				dc->Polygon(Coor,4);	
				dc->SelectObject(pOldBrush);
				/*Modified By SunZhenyu 2003/11/11, add the next 5 line */
				if( ALN_BOTTOM == m_alnTab )
				{
					dc->FillRect( CRect(mrt.left+1,mrt.top+1,mrt.right-1,mrt.top+4), &brushRed );
				}
				else
				{
					dc->SelectObject(&cpw);
					dc->MoveTo(mrt.left+1,mrt.bottom);
					dc->LineTo(mrt.right-1,mrt.bottom);
					dc->MoveTo(mrt.left+1,mrt.bottom+1);
					dc->LineTo(mrt.right-1,mrt.bottom+1);
				}
			}
			
			// Modified By SunZhenyu
			// CFont* m_fontOld=dc->SelectObject(&m_cfont);
			dc->SetBkMode(TRANSPARENT);
			CRect mrtt=mrt; mrtt.top+=2;
			CSize m_sChar=dc->GetTextExtent(ctb->lpMsg,strlen(ctb->lpMsg));
			ctb->rect=mrt;
			m_right-=(m_rectTab.Height()/2);	
			if (m_sChar.cx < ctb->rect.Width()-8)  
				{
					rectText=ctb->rect;
					/*Modified By SunZhenyu 2003/11/11, add the next 1 line */
					dc->SetTextColor(iCont == m_iSelectTab ? m_clrSelectTabText : ::GetSysColor(COLOR_BTNTEXT));
					dc->DrawText(ctb->lpMsg,mrtt,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
				}
				else
				{
					int nDif= ctb->rect.Width()-m_sizeImag.cx;
					nDif/=2;
					if(ctb->uIcon == -1) continue;
					Image.Draw(dc,ctb->uIcon,CPoint(ctb->rect.left+nDif,ctb->rect.top+2),ILD_TRANSPARENT);
					CToolTip.SetToolRect(this,iCont+1,CRect(0,0,0,0));
				}
			
		}





	GetClientRect(rc);
	CSize m_SizeBtn=m_ToolButton.GetSizeButton();
	if (m_bShowButton==TRUE)
	{
		if (m_right+m_SizeBtn.cx > rc.Width())
			m_ToolButton.ShowWindow(SW_HIDE);
		else
		{
			rc.left=rc.right-(m_SizeBtn.cx+2);
			rc.bottom=m_rectTab.Height()-2;
			m_ToolButton.MoveWindow(rc.left,rc.top+1,m_SizeBtn.cx,rc.bottom-1);
			m_ToolButton.ShowWindow(SW_SHOW);
		}
	}
	dc->SetBkMode(nMode);
	dc->SelectObject(m_fontOld);
	// Modified By SunZhenyu
	dc->SelectObject(pOldPen);
}

void CGuiTabPowerPoint::ShowButtonClose(BOOL bShowButton)
{
	m_bShowButton=bShowButton;

};	
void CGuiTabPowerPoint::SetAutoAjust(BOOL bAjust)
{
	bAutoAjust=bAjust;
	m_ToolButton.ShowWindow(bAutoAjust==FALSE?SW_HIDE:SW_SHOW);
}

void CGuiTabPowerPoint::SetColorTabs(COLORREF clrTabs)
{
	m_clrTabs=clrTabs;
}

int CGuiTabPowerPoint::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiTabWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_img.Create(IDB_MDIICONS,9,3,RGB(255,0,255)))
	{
		TRACE0("error imagelist");
	}
	SetALingTabs(CGuiTabWnd::ALN_TOP); //los tabs arriba
	SetStyle(CGuiTabWnd::FLAT);
	m_ToolButton.ShowDark(FALSE);
	SetActualTitle(FALSE);
	m_ToolButton.Create(_T(""),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,CRect(0,0,0,0),this,ID_CLOSE);
	m_ToolButton.SetToolTip("Close");
	m_ToolButton.SethIcon(m_img.ExtractIcon(2));
	m_ToolButton.SetColor(GuiDrawLayer::GetRGBColorFace());
	
	
	// TODO:  Add your specialized creation code here

	return 0;
}
