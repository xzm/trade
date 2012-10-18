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
#include "guitabbed.h"
#include "GuiBaseTab.h"
#include "..\header\guitabbed.h"
#define SCROLLCLOSE 0x886
#define SCROLLLEFT  0x887
#define SCROLLRIGHT 0x888


BEGIN_MESSAGE_MAP(GuiTabbedButton, CGuiNormalButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


GuiTabbedButton::GuiTabbedButton()
{
	bPress=FALSE;
}

GuiTabbedButton::~GuiTabbedButton()
{

}

void GuiTabbedButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (!bPress) return;
	bPress=FALSE;
	KillTimer(1);
	//CGuiNormalButton::OnLButtonUp(nFlags, point);
}
void GuiTabbedButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (bPress) return;
	bPress=TRUE;
	SetTimer(1,100,NULL);
	//CGuiNormalButton::OnLButtonDown(nFlags, point);
}

void GuiTabbedButton::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (!bPress) return;
	CRect rc;
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);
	GetClientRect(rc);
	if (rc.PtInRect(pt))
	{
		CWnd* pParent=GetParent();
		pParent->SendMessage(WM_COMMAND,GetDlgCtrlID());
		bPress=FALSE;
		//KillTimer(1);
	}
	else
	{
		bPress=FALSE;
		KillTimer(1);
		//CGuiNormalButton::OnTimer(nIDEvent);
	}
	
}

CGuiTabbed::CGuiTabbed(void)
{
	m_nDif=0;
}

CGuiTabbed::~CGuiTabbed(void)
{
}



void CGuiTabbed::Drawtabs(CDC* dc)
{
	int m_StrPos=0;
	BOOL m_ViewAlltab=TRUE;
	CRect m_rClient;
	CBrush cbr;
	CPen light(PS_SOLID,1,GetSysColor(COLOR_BTNHIGHLIGHT));
	CPen Dark(PS_SOLID,1,GetSysColor(COLOR_BTNSHADOW));
	CPen Black(PS_SOLID,1,GetSysColor(BLACK_PEN));	
	CRect rectText;
	CFont* m_fontOld=dc->SelectObject(&m_cfont);
	// Modified By SunZhenyu
	CPen* pOldPen = dc->SelectObject(&light);
	CSize mszBtn=GetSizeButton();
	int m_iSizeButton=mszBtn.cx;
	cbr.CreateSolidBrush(GuiDrawLayer::GetRGBColorTabs());
	dc->FillRect(m_rectTab,&cbr);
	//dc->Draw3dRect(m_rectTab,GuiDrawLayer::GetRGBColorShadow(),GuiDrawLayer::GetRGBColorBTNHigh());
	m_rectTab.bottom+=1;
	CRect rectT=m_rectTab;
	rectT.right-=m_iSizeButton+2;
	int mSizeTotal=m_iMinValRec;
	m_toolBtn[0].EnableWindow(FALSE);
	m_toolBtn[1].EnableWindow(FALSE);
	
	if (mSizeTotal > m_rectTab.Width())
	{
		
		m_toolBtn[1].EnableWindow(TRUE);
		if (m_nDif < 0) 
			m_toolBtn[0].EnableWindow(TRUE);
		if((mSizeTotal-(abs(m_nDif)-20)) < m_rectTab.Width())
			m_toolBtn[1].EnableWindow(FALSE);
	}
	else
		m_nDif=0;
	
	CRect rectScroll; 
	CRgn reg;	
	CRect rectRecorte;
	rectRecorte=rectT;
	reg.CreateRectRgnIndirect(rectRecorte);
	dc->SelectClipRgn(&reg);
	int m_right=m_nDif;
	for ( int iCont=0; iCont< m_Numtabs;iCont++)
	{	

		CGuiTab* ctb=(CGuiTab*)m_pArray[iCont];
		CRect mrt=ctb->rect;		
		int ancho=mrt.Width();
		mrt.left=m_right;
		mrt.right=mrt.left+ancho;
		CPoint Coor[4];
		//si es la carpeta seleccionada, se debe 
		//eliminar las lineas oscuras de encima
		if (iCont ==m_iSelectTab)
		{
			CBrush cbr;
			CRect  m_rectTabAux=mrt;
			cbr.CreateSysColorBrush(COLOR_BTNFACE);
			m_rectTabAux.top+=3;
			m_rectTabAux.bottom+=2;
			dc->FillRect(m_rectTabAux,&cbr);
			dc->SelectObject(&light);
			dc->MoveTo(m_rectTabAux.left,m_rectTabAux.top);
			dc->LineTo(m_rectTabAux.left,m_rectTabAux.bottom);
			//linea de arriba
			dc->MoveTo(m_rectTabAux.left,m_rectTabAux.top);
			dc->LineTo(m_rectTabAux.right,m_rectTabAux.top);
			//linea derecha observe que se pinta dos veces para
			//dar el efecto de redondeada en la puntas

			dc->SelectObject(&Black);
			dc->MoveTo(m_rectTabAux.right-1,m_rectTabAux.top+1);
			dc->LineTo(m_rectTabAux.right-1,m_rectTabAux.bottom);
			dc->SetTextColor(RGB(0,0,0));
			//pDC->TextOut(rCText.left+3,rCText.top,m_caption);

		}
		else
		{
			if ((iCont+1) != m_iSelectTab)
			{
				dc->SelectObject(&Dark);
				dc->MoveTo(mrt.right-1,mrt.top+3);
				dc->LineTo(mrt.right-1,mrt.bottom+1);
				dc->SetTextColor(GuiDrawLayer::GetRGBColorShadow());
			}
				
			
		}
		CRect m_rectTabAux= mrt;
		m_rectTabAux.top+=4;
		m_rectTabAux.left+=4;
		int OldMode=dc->SetBkMode(TRANSPARENT);
		dc->DrawText(ctb->lpMsg,m_rectTabAux,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		m_right=mrt.right;
		ctb->rect=mrt;
		dc->SetBkMode(OldMode);
	}


	dc->SelectClipRgn(NULL);
	
	CRect rcCli;
	GetClientRect(rcCli);
    rcCli.top=m_rectTab.top;
	rcCli.bottom=m_rectTab.bottom;
	rcCli.left=rcCli.right-(m_iSizeButton);
	int nacum=0;
	for (int i=0; i<3;i++)
	{
		CSize szBtn=m_toolBtn[i].GetSizeButton();
		m_toolBtn[i].MoveWindow(rcCli.left+nacum-1,rcCli.top+2,szBtn.cx,rcCli.Height()-4);
		nacum+=szBtn.cx;
	}
	GetClientRect(rcCli);
	dc->Draw3dRect(rcCli,GuiDrawLayer::GetRGBColorShadow(),GuiDrawLayer::GetRGBColorShadow());
	
	// Modified By SunZhenyu
	dc->SelectObject(m_fontOld);
	dc->SelectObject(pOldPen);
}


BEGIN_MESSAGE_MAP(CGuiTabbed, CGuiTabWnd)
	ON_WM_CREATE()
	ON_COMMAND(SCROLLLEFT,OnScrollLeft)
	ON_COMMAND(SCROLLRIGHT,OnScrollRight)
	ON_COMMAND(SCROLLCLOSE,OnScrollClose)
END_MESSAGE_MAP()

void  CGuiTabbed::OnScrollLeft()
{
	m_nDif+=300;
	CClientDC dc(this);
	Drawtabs(&dc);	
	//RecalLayout();
	
}

void  CGuiTabbed::OnScrollRight()
{
	m_nDif-=300;
	CClientDC dc(this);
	Drawtabs(&dc);	
	//RecalLayout();
}

void  CGuiTabbed::OnScrollClose()
{
	//no es nuestro problema lo debe manejar la clase derivada
}


int CGuiTabbed::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiTabWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_img.Create(IDB_DOCKBAR,9,11,RGB(255,0,255)))
	{
		TRACE0("error imagelist");
	}

	m_toolBtn[0].Create(_T(""),WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,CRect(0,0,0,0),this,SCROLLLEFT);
	m_toolBtn[0].SetStyleButton(CGuiNormalButton::STL_FLAT);
	m_toolBtn[0].SetToolTip("Scroll Left");
	m_toolBtn[0].SethIcon(m_img.ExtractIcon(4));
	m_toolBtn[0].SetColor(GuiDrawLayer::GetRGBColorTabs());
	m_toolBtn[0].ShowDark(FALSE);
	m_toolBtn[1].Create(_T(""),WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,CRect(0,0,0,0),this,SCROLLRIGHT);
	m_toolBtn[1].SetStyleButton(CGuiNormalButton::STL_FLAT);
	m_toolBtn[1].SetToolTip("Scroll Right");
	m_toolBtn[1].SethIcon(m_img.ExtractIcon(2));
	m_toolBtn[1].SetColor(GuiDrawLayer::GetRGBColorTabs());
	m_toolBtn[1].ShowDark(FALSE);
	m_toolBtn[2].Create(_T(""),WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,CRect(0,0,0,0),this,SCROLLCLOSE);
	m_toolBtn[2].SetStyleButton(CGuiNormalButton::STL_FLAT);
	m_toolBtn[2].SetToolTip("Close");
	m_toolBtn[2].SethIcon(m_img.ExtractIcon(10));
	m_toolBtn[2].SetColor(GuiDrawLayer::GetRGBColorTabs());
	m_toolBtn[2].ShowDark(FALSE);
	m_toolBtn[0].EnableWindow(FALSE);
	m_toolBtn[1].EnableWindow(FALSE);
	SetALingTabs(CGuiTabWnd::ALN_TOP); //los tabs arriba
	SetStyle(CGuiTabWnd::SHADOW);	   //no debe acondicionar el tamaño de los tabs		return 0;
	SetTypeTab(CGuiTabWnd::TYPE_MDI);
	SetActualTitle(FALSE);
	return 0;
}

CSize CGuiTabbed::GetSizeButton()
{
	CSize szresult=CSize(0,0);
	for (int i=0; i < 3; i++)
	{
		CSize sztemp=m_toolBtn[i].GetSizeButton();
		szresult.cx+=sztemp.cx;
		szresult.cy+=sztemp.cy;
	}
	return szresult;
}



