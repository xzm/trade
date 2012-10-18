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


// GuiControlBar.cpp : implementation file


#include "stdafx.h"
#include "GuiControlBar.h"
#include "GuiDockContext.h"
#include "guicontrolbar.h"
#include "GuiDrawLayer.h"
#include "resource.h"
#include "..\header\guicontrolbar.h"
// CGuiControlBar

IMPLEMENT_DYNAMIC(CGuiControlBar, CControlBar)
CGuiControlBar::CGuiControlBar()
{
	nGapGripper=20;
	m_bActive=FALSE;
	m_bOldActive=FALSE;
	/*Modified By SunZhenyu 2003/6/21
	m_sizeMinFloating=m_sizeVert=m_sizeHorz=CSize(200,100);
	m_sizeHorzt=CSize(200,100);
	m_sizeVertt=CSize(200,100);
	*/
	m_sizeMinFloating=m_sizeVert=m_sizeHorz=CSize(210,400);
	m_sizeHorzt=CSize(210,400);
	m_sizeVertt=CSize(210,400);
	m_pos=0;
	m_Last=0;
	m_bTracking=FALSE;
	m_rcBorder=CRect(0,0,0,0);
	m_rcOldBorder=CRect(0,0,0,0);
	m_ptOld=CPoint(0,0);
	m_sizeMinV=CSize(28,28);
	m_sizeMinH=CSize(28,28);
	m_Initialize=FALSE;
	m_First=-1;
	m_bForcepaint=FALSE;
	m_stateBtn=NORMAL;
	m_clrFondo=GuiDrawLayer::GetRGBColorFace();
	m_bSupportMultiView=FALSE;
}

CGuiControlBar::~CGuiControlBar()
{
}


BEGIN_MESSAGE_MAP(CGuiControlBar, CControlBar)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_TIMER()
	ON_COMMAND(WM_SHOWTITLE, OnShowTitle)
	ON_WM_SYSCOLORCHANGE()
	
END_MESSAGE_MAP()



// CGuiControlBar message handlers

void CGuiControlBar::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/)
{
	
	CWnd* pFocus = GetFocus();
	m_bOldActive=(pFocus->GetSafeHwnd() && IsChild(pFocus));
	m_bForcepaint=TRUE;
	if (!m_bActive && m_bOldActive)
		OnActiveWindow();
	m_bForcepaint=FALSE;
	

}

void CGuiControlBar::OnSysColorChange( )
{
	m_clrFondo=GuiDrawLayer::GetRGBColorFace();
	CControlBar::OnSysColorChange( );
	
}

BOOL CGuiControlBar::Create(LPCTSTR lpszWindowName, DWORD dwStyle,CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class
	//gran parte del codigo se tomo como guia de clases MFC
	ASSERT_VALID(pParentWnd);   // must have a parent
	//en esta linea se verifica que la ventana debe disponer de un estilo fijo o dinamico
	//pero no los dos.el estilo Dynamic permite cambiar el tamaño dela ventana mientras flota
	//pero no cuando esta docking, el estilo fijo determina las columnas en que se disponen los
	//componentes y permance asi.
	ASSERT(!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));
	// save the style
	//en dwStyle debe asignarse un tipo de alineación por ejemplo CBRS_TOP,etc de lo contrario
	//se generase un ASSERT al acambiar el Style
	dwStyle|=CBRS_TOP;
	m_dwStyle = (dwStyle & CBRS_ALL);//save the original style
	dwStyle &= ~CBRS_ALL;
	//en la siguiente instruccion el proposito que se busca es evitar el parpadeo 
	//cuando se refresca la ventana.
	//WS_CLIPCHILDREN : recorta el area de las ventanas hijas cuando se dibuja sobre 
	//				 la ventana que la contiene.
	//WS_CLIPSIBLING : cuando se recibe el mensaje paint se recorta el area de las otras ventanas
	//				   hijas superpuestas, que estan fuera de la region.	
	dwStyle |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	//con el estilo CS_DBLCLKS, lo que se busca es que al recibir un doble clic
	//la ventana reaccione,ojo el problema es que esto lo hace solo con el area cliente. 
	LPCTSTR lpszClassName=::AfxRegisterWndClass(CS_DBLCLKS,
												::LoadCursor(NULL,IDC_ARROW),
												::GetSysColorBrush(COLOR_BTNFACE),
												NULL);
	//poque no se llama a CControlBar::Create, bueno, da igual llamar a cualquiera, CWnd o CControlBar
	//esto debido a que CControlbar se deriva de CWnd y porque ademas CControlBar no sobrecarga el 
	//metodo Create, nosotros si porque tenemos que particularizar, cosas.
	BOOL bResp= CWnd::Create(lpszClassName, lpszWindowName, dwStyle, CRect(0,0,0,0), pParentWnd, nID);
	if (!bResp) return FALSE;
	
	return TRUE;
}

int CGuiControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO:  Add your specialized creation code here
	//aqui es cuando llamamos a nuestra clase CGuiDockContext, de esta manera
	//sobrecargamos el clase original que para nuestros propositos no nos sirve.
	//porque ?, bueno porque me interesa que no se pegen las toolbar en el interior
	//de las ventanas.
	if (m_pDockContext==NULL)
		m_pDockContext=new CGuiDockContext(this);
	ASSERT(m_pDockContext);
	m_CloseBtn.SetData(6,"Close");
	m_CloseBtn.SetImageList(IDB_DOCKBAR,9,10,RGB(255,0,255));
	return 0;
}


void CGuiControlBar::OnShowTitle()
{
	ActiveCaption();
	SendMessage(WM_NCPAINT);
}


//esta funcion calcula el tamaño horizontal de la ventana,no importa si esta 
//docking a izquierda o derecha o arriba o abajo.Debemos disponer de un espacio equitativo entre todas
//ventanas que se encuentren docking ya sea en una fila o columna.
CSize CGuiControlBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	//la funcion original toma el ancho o alto dependiendo del sentido que nos 
	//indica bHorz.
	ASSERT_VALID(this);

	if (IsFloating())
		return m_sizeMinFloating;
	else
	{
		//si bStrerch es TRUE significa que esta ventana no se puede hacer
		//Docking
		if (bStretch) 
		{
			if (bHorz)
				return CSize(32767, m_sizeHorz.cy);
			else
				return CSize(m_sizeVert.cx, 32767);
		}
	}
	
	int Len=GetHiWid();

	int nWidth = GetWidthMax();
	int nMinSpace=0;//minimo espacio requerido con lo tamaños normales
	int nMinimo=0;  //minimo espacio de los tamaños minimos
	int nRealBars=0;
	int m_First=GetFirstPos();
	
	

	for (int nPos = m_First; nPos <= m_Last; nPos++)
	{
		CGuiControlBar* pBar = GetGuiControlBar(nPos,TRUE);
		if (pBar== NULL) continue;
		if (!pBar->IsVisible()) continue;
		if (!pBar->IsKindOf(RUNTIME_CLASS(CGuiControlBar)))
		{
			CPoint pt(GetMessagePos());
			m_pDockSite->FloatControlBar(pBar,pt);
			continue;
		}
		if(IsVert())
			pBar->m_sizeVert.cx=nWidth;
		else	
			pBar->m_sizeHorz.cy=nWidth; //todas se hacen con el mismo ancho
		nMinSpace+=IsVert() ? pBar->m_sizeVert.cy:pBar->m_sizeHorz.cx; //minimo espacio para alinear las barras
		nRealBars++;	//cuantas barras realmente existen
	}
	
	//si el tamaño de las barras en la fila es mayor que 
	//el espacio disponible, luego la solucion salomonica es 
	//repartir el espacio entre todas.
	if (nRealBars == 1 )
	{
		if (bHorz)
			return m_sizeHorz= CSize(Len,m_sizeHorz.cy);
		else
			return m_sizeVert=CSize(m_sizeVert.cx,Len);
		
	}
	
	

	int nDif=Len-nMinSpace;
	if (abs(nDif) !=0)
	{
		BOOL bGrow=FALSE;
		if (nDif > 0)
			bGrow=TRUE;
		nDif=abs(nDif);
		while(nDif > 0)
		{
			for (int nPos = m_First; nPos <= m_Last; nPos++)
			{
				CGuiControlBar* pBar = GetGuiControlBar(nPos);
				if (pBar== NULL) continue;
				if(IsVert())
				{
					if(bGrow)
						pBar->m_sizeVert.cy+=1;
					else
					{
						
						if (pBar->m_sizeVert.cy-1 < pBar->m_sizeMinV.cy)
							continue;
						pBar->m_sizeVert.cy-=1;
						
					}
				}
				else
				{
					if(bGrow)
						pBar->m_sizeHorz.cx+=1;
					else
					{
						
						if (pBar->m_sizeHorz.cx-1 < pBar->m_sizeMinH.cx)
							continue;
						pBar->m_sizeHorz.cx-=1;
						
					}
					
				}
				
				nDif--;
				if(nDif==0) break;
			}
	
		}

	}
	
   //--reubicar las ventanas, sin esta rutina nada funciona
  RecalWindowPos();
 
	if (IsHorz())
		return 	m_sizeHorz;
	else
		return 	m_sizeVert;
}


//esta rutina dispone de la posición en el Dockbar de la pila de  ventanas 
void CGuiControlBar::RecalWindowPos()
{
	int m_First=GetFirstPos();
	int m_Last=GetLastPos();
	int m_This=m_pDockBar->FindBar(this);
	
	CRect rcWin=GetDockRect();
	int m_VertPos=0;
	for(int i=m_First; i<= m_Last; i++)
	{
		CGuiControlBar* pBar = GetGuiControlBar(i);
		if (pBar == NULL) continue;
		CRect rcBar;
		pBar->GetWindowRect(rcBar);
		rcBar.OffsetRect(-rcWin.TopLeft());
		if (IsVert())
		{
	 	 if (i==m_First) 
			rcBar.top=-2;
		 else
		 	rcBar.top=m_VertPos;
		}
		else
		{
			if (i==m_First) 
				rcBar.left=-2;
			else
		 		rcBar.left=m_VertPos;
		}
		pBar->MoveWindow(rcBar);
		m_VertPos+=IsVert()? rcBar.Height():rcBar.Width();

	}
	m_pDockSite->RecalcLayout();

}

CRect CGuiControlBar::GetDockRect()
{
	CRect rcWin;
	if (IsVert())
		if (IsLeft())
			m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_LEFT)->GetWindowRect(rcWin);
		else
			m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_RIGHT)->GetWindowRect(rcWin);
	else
		if(IsBottom())
			m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_BOTTOM)->GetWindowRect(rcWin);
		else
			m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_TOP)->GetWindowRect(rcWin);
	return rcWin;
}



int CGuiControlBar::GetWidthMax()
{
	m_pos=m_pDockBar->FindBar(this);
	m_Last=GetLastPos();
	int nWidth=0;
	for (int nPos = GetFirstPos(); nPos <= m_Last; nPos++)
	{
		CGuiControlBar* pBar = GetGuiControlBar(nPos);
		if (pBar== NULL) continue;
		nWidth=max(nWidth,IsVert() ? pBar->m_sizeVert.cx:pBar->m_sizeHorz.cy);
	}
	return nWidth;
}



CGuiControlBar* CGuiControlBar::GetGuiControlBar(int nPos,BOOL bAll) const
{
	CGuiControlBar* pResult = (CGuiControlBar*)m_pDockBar->m_arrBars[nPos];
	if (bAll==FALSE)
	{
		if (HIWORD(pResult) == NULL) return NULL;
		else if (!pResult->IsVisible()) return NULL;
		else if (!pResult->IsKindOf(RUNTIME_CLASS(CGuiControlBar))) return NULL;
	}
	else
	{
		if (HIWORD(pResult) == NULL)
		return NULL;
	}
	return pResult;
}

//En esta función se calcula el tamaño de la ventana cuando esta flotando
//y gestionar cuando el mouse es presionado en las esquinas.
//#define HTTOPLEFT           13
//#define HTTOPRIGHT          14
//#define HTBOTTOMLEFT        16
//#define HTBOTTOMRIGHT       17


CSize CGuiControlBar::CalcDynamicLayout(int nLength, DWORD nMode)
{

	m_pDockSite->RecalcLayout();

	if (IsFloating())
	{
		// Enable diagonal arrow cursor for resizing
		//m_sizeVert=m_sizeHorz=CSize(200,200);
		GetParent()->GetParent()->ModifyStyle(MFS_4THICKFRAME|WS_CAPTION,0);
	} 
	if (nMode & (LM_HORZDOCK | LM_VERTDOCK)) 
	{
		m_pDockSite->DelayRecalcLayout();
		//obligar a reposicionar  la ventana, de lo contrario cuando vuelva de un doble click
		//desde la ventana CMiniFrameWnd queda sin area cliente
		SetWindowPos(NULL, 0, 0, 0, 0,
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER |
			SWP_NOACTIVATE | SWP_FRAMECHANGED|SWP_NOREDRAW);
	 	return CControlBar::CalcDynamicLayout(nLength, nMode);
	}
	if (nMode & LM_MRUWIDTH)
        return  m_sizeMinFloating;

    if (nMode & LM_COMMIT)
        return  m_sizeMinFloating ;
    
	if (IsFloating())
	{

		CRect	rcWin;
		POINT	cpt;
		GetCursorPos(&cpt);
		GetParent()->GetParent()->GetWindowRect(&rcWin);
		int nXOffset=0;int nYOffset=0;
		switch (m_pDockContext->m_nHitTest)
		{

			//------------------------------------------------------------------
			case HTLEFT:
				m_pDockContext->m_rectFrameDragHorz= rcWin;
				m_pDockContext->m_rectFrameDragHorz.left = cpt.x;
				m_sizeMinFloating.cx = max(rcWin.right - cpt.x,32)-4 ;
				m_sizeMinFloating.cy = max((rcWin.bottom -rcWin.top)-nGapGripper-5,32)+2 ;
				return m_sizeMinFloating;		
			break;
			case HTTOP:
				m_pDockContext->m_rectFrameDragHorz=rcWin;
				m_pDockContext->m_rectFrameDragHorz.top = cpt.y;
				m_sizeMinFloating.cx = max(rcWin.right-rcWin.left-2,32)-4 ;
				m_sizeMinFloating.cy = max((rcWin.bottom -nGapGripper-cpt.y-3),32) ;
				return m_sizeMinFloating;		
			break;
			case HTRIGHT:
				m_pDockContext->m_rectFrameDragHorz=rcWin;
				m_pDockContext->m_rectFrameDragHorz.right = cpt.x;
				m_sizeMinFloating.cy = max(rcWin.bottom -rcWin.top-nGapGripper-3,32) ;
				m_sizeMinFloating.cx = max(cpt.x-rcWin.left-4,32);
				return m_sizeMinFloating;		
			break;
			case HTBOTTOM:
				m_pDockContext->m_rectFrameDragHorz=rcWin;
				m_sizeMinFloating.cy = max(cpt.y-rcWin.top -nGapGripper-3,32) ;
				m_sizeMinFloating.cx = max(rcWin.right-rcWin.left-2,32)-4 ;
				m_pDockContext->m_rectFrameDragHorz.bottom = cpt.y-4;
				return m_sizeMinFloating;
				break;
			case HTTOPLEFT:
				//---------------------------------------------------------
			//En este caso crece la ventana a izquierda y hacia arriba
			//izquierda incrementa cx y top incrementa cy
				m_sizeMinFloating.cx = max(rcWin.right - cpt.x,32)-3 ;
				m_sizeMinFloating.cy = max(rcWin.bottom -nGapGripper-cpt.y,32)-2 ;
				m_pDockContext->m_rectFrameDragHorz.top = cpt.y-1;
				m_pDockContext->m_rectFrameDragHorz.left = cpt.x-2;

				return m_sizeMinFloating;		
				break;
			case HTTOPRIGHT:
				m_sizeMinFloating.cx = max(cpt.x-rcWin.left,32)-4 ;
				m_sizeMinFloating.cy = max(rcWin.bottom -nGapGripper-cpt.y,32)-2 ;
				m_pDockContext->m_rectFrameDragHorz.top = cpt.y-1;
				m_pDockContext->m_rectFrameDragHorz.right = cpt.x-2;
				return m_sizeMinFloating;		
				break;
			case HTBOTTOMLEFT:
				m_sizeMinFloating.cx = max(rcWin.right - cpt.x,32)-4;
				m_sizeMinFloating.cy = max(cpt.y-rcWin.top -nGapGripper,32)-2 ;
				m_pDockContext->m_rectFrameDragHorz.top = rcWin.top;
				m_pDockContext->m_rectFrameDragHorz.bottom = cpt.y-1;
				m_pDockContext->m_rectFrameDragHorz.left = cpt.x-2;
				return m_sizeMinFloating;		
				break;
			case HTBOTTOMRIGHT:
				m_sizeMinFloating.cx = max(cpt.x-rcWin.left,32);
				m_sizeMinFloating.cy = max(cpt.y-rcWin.top -nGapGripper,32) ;
				m_pDockContext->m_rectFrameDragHorz.top = rcWin.top;
				m_pDockContext->m_rectFrameDragHorz.bottom = cpt.y+1;
				m_pDockContext->m_rectFrameDragHorz.right = cpt.x+2;
				return m_sizeMinFloating;		
				break;
			
		}
		
	}

	if(nMode & LM_LENGTHY)
		 m_sizeMinFloating.cy = max(nLength,32);
	else
	     m_sizeMinFloating.cx = max(nLength,32);
	return m_sizeMinFloating;
}

void CGuiControlBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_pDockBar != NULL)
		m_pDockContext->ToggleDocking();
	else
		CWnd::OnLButtonDblClk(nFlags, point);
}
void CGuiControlBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CControlBar::OnLButtonDown(nFlags, point);
}



void CGuiControlBar::OnMouseMove( UINT nHitTest, CPoint point)
{
	
	if(m_bTracking)
	{
		if (GetCapture() != this)
		{
			//StopTracking(FALSE);
			m_bTracking=FALSE;
		}
		
		OnInvertTracker(m_rcBorder);			
		//nuevos tamaños de la ventana
		if (IsVert())
		{
			if (m_SideMove==HTLEFT || m_SideMove==HTRIGHT)
			{
				m_rcBorder.left=point.x;
				m_rcBorder.right=m_rcBorder.left+4;
				
			}
			else
			{
				m_rcBorder.top=point.y+26;
				m_rcBorder.bottom=m_rcBorder.top+4;
				
			}
		}
		else
		{
			if (m_SideMove==HTBOTTOM || m_SideMove==HTTOP)
			{
				m_rcBorder.top=point.y+26;
				m_rcBorder.bottom=m_rcBorder.top+4;
				
			}
			else
			{
				m_rcBorder.left=point.x;
				m_rcBorder.right=m_rcBorder.left+4;
				
			}
		}
		
		//--------------------------------------------------
		//se hacen iguales todos los tamaños
		ClientToScreen(&point);
		GetParentFrame()->ScreenToClient(&point);
		m_ptActualPos=point;
				
		OnInvertTracker(m_rcBorder);
		//SetEqualWidth();
		//-----------------------------------------------
		
		
		
	}
	
}


//depende de la posicion se hace igual el tamanio del ancho o alto
void CGuiControlBar::SetEqualWidth()
{

	int nFirstPos=GetFirstPos();
	for (int nPos = nFirstPos; nPos <= m_Last; nPos++)
	{
		CGuiControlBar* pBar = GetGuiControlBar(nPos);
		if (pBar== NULL) continue;
		if (IsHorz())
			pBar->m_sizeHorz.cy=m_sizeHorz.cy;//rcDockBar.Height();
		else
			pBar->m_sizeVert.cx=m_sizeVert.cx;
	}
}

int CGuiControlBar::GetHiWid()
{
	CRect rcWin;
	rcWin=GetDockRect();
	if (IsVert())
		return rcWin.Height() ;
	else
		return rcWin.Width() ;
}


//espero que funcione el truco
//la idea es trabajar con coordenadas screen las dimensiones de los bordes del
//dockBar, mas no con los movimientos internos.
void CGuiControlBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptTemp=point;
	ClientToScreen(&ptTemp);
	if (m_bTracking)
	{
		ReleaseCapture();
		m_bTracking=FALSE;
		OnInvertTracker(m_rcBorder);
		m_pDockSite->UnlockWindowUpdate();
	
		if (ptTemp ==m_ptStartPos) return;
		
		if (IsVert())
		{
			if (m_SideMove==HTLEFT)
				m_sizeVert.cx-=point.x;
			else if(m_SideMove==HTRIGHT)
				m_sizeVert.cx=point.x;
			else if(m_SideMove==HTTOP)
				AjustReDinSize(point);
		}
		else
		{
		   if (m_SideMove==HTBOTTOM)
			   {
				if (point.y < 0)
					m_sizeHorz.cy+=abs(point.y);
				else
					m_sizeHorz.cy=point.y;
			}
			 
		   else if (m_SideMove==HTTOP)
		   {
			  if (point.y < 0)
				  m_sizeHorz.cy+=abs(point.y)-12;
			  else
				  m_sizeHorz.cy-=abs(point.y)+12;	
		   }
			else if (m_SideMove==HTRIGHT)
		   	   AjustReDinSize(point);
		    	
		}
		
		SetEqualWidth();
		
	}
	m_pDockSite->RecalcLayout();
	

}

//ajusta las ventanas a redimencionarlas verticalmente
//se decrementa las anteriores ventanas a la actual y se 
//incrementan las posteriores.
void CGuiControlBar::AjustReDinSize(CPoint point)
{
	
	int nFirstPos=GetFirstPos();
	int nLastPos=GetLastPos();
	int m_ThisPos=m_pDockBar->FindBar(this);
	

	ClientToScreen(&point);

	//si la diferencia es negativa esta barra crece la anterior a esta disminuye
	int nDif=0;
	BOOL bGrow=FALSE;
	
	if (IsVert())
	{
		 nDif=m_ptStartPos.y- point.y;
		 if (nDif > 0)
			bGrow=TRUE;
		 
		 if (bGrow)
                m_sizeVert.cy+=abs(nDif)+4;
		 else
				m_sizeVert.cy-=abs(nDif);
			
	 	 if (nFirstPos == m_ThisPos) 
			return;
	
	}
	else
	{
		 nDif=m_ptStartPos.x- point.x;
		 if (nDif < 0)
			bGrow=TRUE;

		 if (bGrow)
				m_sizeHorz.cx+=abs(nDif);
			else
				m_sizeHorz.cx-=abs(nDif);
			
         if (nLastPos == m_ThisPos) 
			return;
	}
	
	
	if (IsVert())
		AjustVert(bGrow,nDif);
	else
		AjustHorz(bGrow,nDif);
	
	RecalWindowPos();
	

	
}
	

void CGuiControlBar::AjustVert(BOOL bGrow,int nDif)
{
	int nFirstPos=GetFirstPos();
	int nLastPos=GetLastPos();
	int m_ThisPos=m_pDockBar->FindBar(this);

	if(m_SideMove==HTTOP)
	{
				
		//Esta ventana crece las anteriores reducen su tamaño
		
		if (bGrow)	
			{
				for (int i=m_ThisPos-1; i > 0; i--)
				{
					CGuiControlBar* pBar = GetGuiControlBar(i);
					if (pBar== NULL) return;
					if(IsVert())
					{
						if (pBar->m_sizeVert.cy-abs(nDif) < pBar->m_sizeMinV.cy)
						{
							pBar->m_sizeVert.cy=pBar->m_sizeMinV.cy;
							continue;
						}
						else
						{
							pBar->m_sizeVert.cy-=abs(nDif);
							break;
						}
					}
				}//for
					
			}//bGrow
			else	//este disminuye la anterior crece
			{
				if (m_ThisPos-1 > 0)
				{
					CGuiControlBar* pBar = GetGuiControlBar(m_ThisPos-1);
					if (pBar== NULL) return;
					pBar->m_sizeVert.cy+=abs(nDif); 
					if(m_sizeVert.cy > m_sizeMinV.cy)
						return;
					else
					  pBar->m_sizeVert.cy-=m_sizeMinV.cy; 
				}
				for (int i=m_ThisPos+1; i >= m_Last; i++)
				{
					CGuiControlBar* pBar = GetGuiControlBar(i);
					if (pBar== NULL) return;
					if(IsVert())
					{
						if (pBar->m_sizeVert.cy-abs(nDif) < pBar->m_sizeMinV.cy)
							continue;
						else
						{
							pBar->m_sizeVert.cy-=abs(nDif);
							return;
						}
					}
				}//for
			}
	}
}


void CGuiControlBar::AjustHorz(BOOL bGrow,int nDif)
{
	int nFirstPos=GetFirstPos();
	int nLastPos=GetLastPos();
	int m_ThisPos=m_pDockBar->FindBar(this);

	if(m_SideMove==HTRIGHT)
	{
				
		//Esta ventana crece las anteriores reducen su tamaño
		
		if (bGrow)	
			{
				for (int i=m_ThisPos+1; i <= nLastPos; i++)
				{
					CGuiControlBar* pBar = GetGuiControlBar(i);
					if (pBar== NULL) return;
					if(IsHorz())
					{
						if (pBar->m_sizeHorz.cx-abs(nDif) < pBar->m_sizeMinH.cx)
						{
							pBar->m_sizeHorz.cx=pBar->m_sizeMinH.cx;
							continue;
						}
						else
						{
							pBar->m_sizeHorz.cx-=abs(nDif);
							break;
						}
					}
				}//for
					
			}//bGrow
			else	//este disminuye la anterior crece
			{
			   if (m_ThisPos+1 <= m_Last)
				{
					CGuiControlBar* pBar = GetGuiControlBar(m_ThisPos+1);
					if (pBar== NULL) return;
					pBar->m_sizeHorz.cx+=abs(nDif); 
					if(m_sizeHorz.cx > m_sizeMinH.cx)
						return;
					else
					  pBar->m_sizeHorz.cx+=abs(nDif); 
				}


				for (int i=m_ThisPos-1; i >0; i--)
				{
					CGuiControlBar* pBar = GetGuiControlBar(i);
					if (pBar== NULL) return;
					if(IsHorz())
					{
						if (pBar->m_sizeHorz.cx-abs(nDif) < pBar->m_sizeMinH.cx)
							continue;
						else
						{
							pBar->m_sizeHorz.cx-=abs(nDif);
							return;
						}
					}
				}//for
			}
	}

	
}


//----------------------------------------------------
//OnActiveWindow retira o asigna el foco a la ventana 

void CGuiControlBar::OnActiveWindow()
{
	POSITION pos = m_pDockSite->m_listControlBars.GetHeadPosition();
	while (pos != NULL)
	{
		CDockBar* pDockBar = (CDockBar*)m_pDockSite->m_listControlBars.GetNext(pos);
		if (pDockBar->IsDockBar() && pDockBar->IsWindowVisible() &&
			(!pDockBar->m_bFloating ))
			{
				int nNumBars=(int)pDockBar->m_arrBars.GetSize();
				for(int i=0; i< nNumBars;i++)
				{
					CGuiControlBar* pBar = (CGuiControlBar*) pDockBar->m_arrBars[i];
					if (HIWORD(pBar) == NULL) continue;
					if (!pBar->IsVisible()) continue;
					if (!pBar->IsKindOf(RUNTIME_CLASS(CGuiControlBar))) continue;
					if (pBar != this)
					{
						pBar->m_bOldActive=FALSE;
						pBar->m_bActive=FALSE;
						pBar->m_bForcepaint=TRUE;
						pBar->SendMessage(WM_NCPAINT);
						pBar->m_bForcepaint=FALSE;
					}
					else
					{
						m_bOldActive=m_bActive;
						m_bActive=TRUE;
						m_bForcepaint=TRUE;
						SendMessage(WM_NCPAINT);
						m_bForcepaint=FALSE;
						

					}
				}
			}
	}
}


void CGuiControlBar::ActiveCaption()
{
	CWnd* pFocus=SetFocus();	
	if(pFocus->GetSafeHwnd())
		IsChild(pFocus);
	m_bForcepaint=TRUE;
	OnActiveWindow();
	m_bForcepaint=FALSE;

}



void CGuiControlBar::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bTracking /*|| IsFloating()*/)
        return;
	
	
	 
	m_ptStartPos=point;
	if( nHitTest == HTCAPTION || nHitTest == HTCLOSE)											
		ActiveCaption();
	//---------para el boton----
	if( nHitTest == HTCLOSE)											
	{
		m_stateBtn=PRESS;
		SendMessage(WM_NCPAINT);
		SetTimer(1,100,0);
		
		return;
	}
	//--------------------------

	if (m_pDockBar != NULL)
	{
		
		if (HTCAPTION == nHitTest)
		{
			m_pDockContext->StartDrag(point);
			m_sizeHorzt=m_sizeHorz;
			m_sizeVertt=m_sizeVert;
		}
		if(!m_bTracking)
		{
			if(m_rcBorder.PtInRect(point))
			{
				
				m_pDockSite->LockWindowUpdate();
				OnInvertTracker(m_rcBorder);
				
				m_ptStartPos=point;
				SetCapture();
				SetFocus();
			
				m_bTracking=TRUE;
				m_sizeHorzt=m_sizeHorz;
				m_sizeVertt=m_sizeVert;
				
			}
		}
	}
	CWnd::OnNcLButtonDown(nHitTest, point);
	
}

void CGuiControlBar::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	//------------------ para el boton
	CRect rcT=m_rcCloseBtn;
	ClientToScreen(rcT);
	point.y+=23;
	point.x+=5;
	if (rcT.PtInRect(point))
	{
		
		if (m_stateBtn ==PRESS)
		{
			m_stateBtn=NORMAL;
			KillTimer(1);
			GetDockingFrame()->ShowControlBar(this, FALSE, FALSE);
		}
		//SendMessage(WM_NCPAINT);
		return;
			
	}
	//-------------------para el boton
	
	
}

void CGuiControlBar::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CControlBar::OnNcPaint() for painting messages
	// Tomo la misma rutina que se desarrolla para la clase 
	// CGuiToolBarWnd.
	CRect rcWindow;
	CRect rcClient;
	CWindowDC	dc(this);
	CDC			m_dc;		//contexto de dispositivo en memoria
	CBitmap		m_bitmap;
	//la idea es tomar el area de la ventana y area  cliente , luego debemos
	//igualar el area de coordenadas de ventana al cliente
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);
	ScreenToClient(rcWindow);
    rcClient.OffsetRect(-rcWindow.TopLeft());
    rcWindow.OffsetRect(-rcWindow.TopLeft());
  	
	m_dc.CreateCompatibleDC(&dc);
	m_bitmap.CreateCompatibleBitmap(&dc,rcWindow.Width(),rcWindow.Height());
	CBitmap *m_OldBitmap=m_dc.SelectObject(&m_bitmap);
	//aqui debe utilizarse la brocha que define GuiDrawLayer, si no hacemos la siguiente
	//linea usted vera un horrible color negro, a cambio del dibujo.
	CBrush cb;
	cb.CreateSolidBrush(m_clrFondo);
	m_dc.FillRect(rcWindow, &cb);

	DrawGripper(&m_dc,&rcWindow);
	dc.IntersectClipRect(rcWindow);
    dc.ExcludeClipRect(rcClient);//asi evitamos el parpadeo
	dc.BitBlt(rcWindow.left,rcWindow.top,rcWindow.Width(),rcWindow.Height(),&m_dc,0,0,SRCCOPY);
	ReleaseDC(&dc);
	m_dc.SelectObject(m_OldBitmap);
	m_bitmap.DeleteObject();
	m_dc.DeleteDC();
	
		

}

void CGuiControlBar::DrawGripper(CDC* pDC,CRect* rc)
{
	CRect gripper = rc;
	gripper.top =3;
	gripper.left+=4;
	gripper.right-=IsVert()?6:4;
	gripper.bottom =gripper.top +nGapGripper-3;
	//si la ventana esta activa pintamos el caption o el area del titulo de color azul 	
	if (IsFloating())
		m_bActive=TRUE;
	
	/* Modified By SunZhenyu 2003/6/21, Add the following line*/
	m_bActive = FALSE;
	
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

	// Modified Bu SunZhenyu
	if( !m_cfont.CreateStockObject( DEFAULT_GUI_FONT ) )
		m_cfont.CreateStockObject( ANSI_VAR_FONT );
	// m_cfont.CreateFont(-11,0,0,0,400,0,0,0,0,1,2,1,34,"Verdana");
		
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
	//CRect gripper;
	//------------------------------------------------
	GetWindowRect( gripper );
	ScreenToClient( gripper );
	gripper.OffsetRect( -gripper.left, -gripper.top );
	gripper.left=gripper.right-20;
	gripper.right-=7;
	gripper.top+=6;
	gripper.bottom=gripper.top+13;
	m_rcCloseBtn=gripper;
	//m_rcCloseBtn.left-=4;
	//ClientToScreen(m_rcCloseBtn);
	if(!m_bActive)
	  m_CloseBtn.Paint(pDC,m_stateBtn,gripper,::GetSysColor(COLOR_BTNFACE));
	else
	  m_CloseBtn.Paint(pDC,m_stateBtn,gripper,::GetSysColor(COLOR_ACTIVECAPTION));
	//------------------------------------------------
 	pDC->SetBkMode(nMode);
	pDC->SelectObject(m_fontOld);
	
}

//en esta función se calcula el area cliente que podra ser utilizado
//por ventanas que deriven esta clase.
void CGuiControlBar::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp)
{
	// adjust non-client area for border space
	lpncsp->rgrc[0].left +=!IsFloating()?5:2;
	lpncsp->rgrc[0].top +=nGapGripper+3;
	lpncsp->rgrc[0].right -=!IsFloating()?IsVert()?7:4:2;
	lpncsp->rgrc[0].bottom -=!IsFloating()?3:2;
	
}


//Aqui la idea es verificar que si se da clic sobre otra ventana de este tipo
//automaticamente emita un mensaje eliminando el caption que la identifica como
//ventana default.

void CGuiControlBar::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CRect rc;
	GetClientRect(rc);
	nDockBarAling = GetParent()->GetDlgCtrlID();
	//envie un recalculo del area cliente solo si el tamaño ha sido 
	//cambiado, de lo contrario permanezca igual
	lpwndpos->flags |= SWP_FRAMECHANGED;
	CControlBar::OnWindowPosChanged(lpwndpos);

	CWnd* pWnd = GetWindow(GW_CHILD);
	if (!m_bSupportMultiView)
	{
        if (pWnd != NULL)
        {
            pWnd->MoveWindow(rc);
            ASSERT(pWnd->GetWindow(GW_HWNDNEXT) == NULL);
		}
	}
	else
	{
		while (pWnd != NULL)
        {
            if (pWnd->IsWindowVisible())
			{
				pWnd->MoveWindow(rc);
				break;
			}
            pWnd=pWnd->GetWindow(GW_HWNDNEXT);
		}
	}
}


//este conjunto de clases nos indican el estado de la ventana
//en un momento determinado

BOOL CGuiControlBar::IsLeft()
{
	if (nDockBarAling == AFX_IDW_DOCKBAR_LEFT)	return TRUE;
	return FALSE;
}

BOOL CGuiControlBar::IsRight()
{
	if (nDockBarAling == AFX_IDW_DOCKBAR_RIGHT)	return TRUE;
	return FALSE;
}

BOOL CGuiControlBar::IsTop()
{
	if (nDockBarAling == AFX_IDW_DOCKBAR_TOP) return TRUE;
	return FALSE;
}

BOOL CGuiControlBar::IsBottom()
{
	if (nDockBarAling == AFX_IDW_DOCKBAR_BOTTOM) return TRUE;
	return FALSE;
}

BOOL CGuiControlBar::IsVert()
{
	if (IsLeft() || IsRight())	return TRUE;
	return FALSE;
}

BOOL CGuiControlBar::IsHorz()
{
	if (IsTop() || IsBottom())	return TRUE;
	return FALSE;
}

BOOL CGuiControlBar::IsFloating()
{
	if (nDockBarAling == AFX_IDW_DOCKBAR_FLOAT)	return TRUE;
	return FALSE;

}
void CGuiControlBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	m_pDockSite->RecalcLayout();//si quita esto se tiene problemas    

	// TODO: Add your message handler code here
	// Do not call CControlBar::OnPaint() for painting messages
}


UINT CGuiControlBar::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rcWindow;
	//no se convierte las coordenadas de pantalla porque el punto
	//entregado por esta función esta dado en el mismo sentido.
	GetWindowRect(rcWindow);
	int it=0;
//	if (IsFloating())
//        return CControlBar::OnNcHitTest(point);
	CRect rcT=m_rcCloseBtn;
	ClientToScreen(rcT);
	CPoint pt=point;
	pt.y+=23;
	pt.x+=5;
	//pt.Offset(-rcT.left,-rcT.top);
	if (rcT.PtInRect(pt))
		return HTCLOSE;
	CRect rcTemp;
	for (int i=0; i< 4; i++)
	{
		rcTemp=rcWindow;
		if (i== 0)		//left
		{
			it=	rcTemp.left;
			it+=4;
			rcTemp.right=it;
			m_rcBorder=rcTemp;
			if (rcTemp.PtInRect(point))
				if(IsLegal(HTLEFT)) return m_SideMove=HTLEFT;
		}
		if (i==1)  //top
		{
			it=	rcTemp.top;
			it+=4;
			rcTemp.bottom=it;
			m_rcBorder=rcTemp;
			if (rcTemp.PtInRect(point))
				 if(IsLegal(HTTOP)) return m_SideMove=HTTOP ;			
		}
		if (i==2)  //right
		{
			it=	rcTemp.right;
			it-=4;
			rcTemp.left=it;
			m_rcBorder=rcTemp;
			if (rcTemp.PtInRect(point))
				if (IsLegal(HTRIGHT)) return m_SideMove=HTRIGHT;			
		}
		if (i==3)  //bottom
		{
			it=	rcTemp.bottom;
			it-=4;
			rcTemp.top=it;
			m_rcBorder=rcTemp;
			if (rcTemp.PtInRect(point))
				if (IsLegal(HTBOTTOM))return m_SideMove=HTBOTTOM;			
		}

	}
	it=0;
	rcTemp=rcWindow;
	it=	rcTemp.top+nGapGripper;
	rcTemp.bottom=it;
	if (rcTemp.PtInRect(point))
		return m_SideMove=HTCAPTION;
	
	return CControlBar::OnNcHitTest(point);
}

//-----------------------------------------------------------
//aqui se verifica que las coordenadas de cambio de tamaño
//sean las correctas

BOOL CGuiControlBar::IsLegal(UINT uAlin)
{
	m_First=GetFirstPos();
//	if (IsFloating()) return FALSE;
	switch(uAlin)
	{
		case HTLEFT:
			if (IsHorz() && m_pos >0 && (m_pos != m_Last && m_pos != m_First)) return TRUE;
			if (IsVert() && m_pos <= m_Last && IsRight() ) return TRUE;
			return FALSE;
			break;
		case HTTOP:
			if (m_pos != m_First && IsVert()) return TRUE;
			if (IsHorz() && m_pos <= m_Last && IsBottom() ) return TRUE;
			return FALSE;
			break;
		case HTRIGHT:
			if (m_pos <= m_Last && IsVert() && IsLeft() ) return TRUE;
			if (IsHorz() && m_pos >0 &&  m_pos != m_Last) return TRUE;
			return FALSE;
		case HTBOTTOM:
			if ((m_pos != m_Last && m_pos != m_First) && IsHorz() && IsBottom()) return TRUE;
			if (m_pos <= m_Last && IsHorz() && IsTop()) return TRUE;
			//if (IsVert() && m_pos >0 ) return TRUE;
			return FALSE;
			break;
	}
	return FALSE;
}



//----------------------------------------------
//debemos obtener cuantas barras existen en esta columnas
//porque si intentamos obtener el conteo con la funciones de dockbar
//siempre obtendremos nuestra actual barra mas otra de otra fila, por lo que
//el conteo es incorrecto, luego despues de nuestra barra la siguiente nula
//es el final de esta fila.

int CGuiControlBar::GetLastPos()
{
	
	int nNumBars=(int)m_pDockBar->m_arrBars.GetSize();
	int m_pos=m_pDockBar->FindBar(this);
	for(int i=m_pos+1; i< nNumBars;i++)
	{
		if (m_pDockBar->m_arrBars[i]== NULL)
		    return i-1;	
	}
	return -1;
}

//--------------------------------------------
//esta rutina funciona algo parecido a la anterior
//con la diferencia que ahora se parte desde la posicion
//que indetifica m_pos hacia atraz hasta encontrar el nulo
int CGuiControlBar::GetFirstPos()
{
	int m_pos=m_pDockBar->FindBar(this);
	for(int i=m_pos; i>=0;i--)
	{
		if (m_pDockBar->m_arrBars[i]== NULL)
		    return i+1;	
	}
	return -1;

}



//------------------------------------------------------------------------

BOOL CGuiControlBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	if (IsFloating()) return 0;
	CPoint ptCursor;
	::GetCursorPos (&ptCursor);
	if(nHitTest == HTLEFT ||nHitTest == HTRIGHT)
		SetCursor(AfxGetApp ()->LoadCursor (AFX_IDC_HSPLITBAR));
	else if(nHitTest == HTTOP ||nHitTest == HTBOTTOM)
		SetCursor(AfxGetApp ()->LoadCursor (AFX_IDC_VSPLITBAR));
	else
		return CControlBar::OnSetCursor(pWnd, nHitTest, message);
	return 1;
}



void CGuiControlBar::OnInvertTracker(const CRect& rect)
{
	ASSERT_VALID(this);
	ASSERT(!rect.IsRectEmpty());
		
	CRect rcWin=GetDockRect();
	
	CDC *pDC = m_pDockSite->GetDCEx(NULL,
        DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE);
	CRect rcBar;
	GetWindowRect(rcBar);
	if (IsVert())  //el sentido de las barras es vertical
	{

		
		
		if (m_SideMove==HTLEFT || m_SideMove==HTRIGHT) //el mouse esta en el borde izquierdo o derecho
		{
			rcWin.OffsetRect(-rect.left,-rect.top);
			rcWin.top+=10;
			rcWin.left=rect.left+2;
			rcWin.right=rect.right+2;
		}
		else  //el mouse esta el borde de arriba pero de una barra vertical
		{
			rcBar.OffsetRect(-rect.TopLeft());
			rcWin=rcBar;
			if (IsLeft() || IsRight())   //a la izquierda
			{
				
				rcWin.top=rect.top-2;
				rcWin.bottom=rect.bottom-2;
			}
			//
			
		}
	}
	else  //el sentido de las barras es horizontal
	{
		
		if (m_SideMove==HTTOP || m_SideMove==HTBOTTOM) //el mouse esta en el borde de arriba o abajo
		{
			rcWin.OffsetRect(-rect.left,-rect.top);
			rcWin.top=rect.top-2;
			rcWin.bottom=rect.bottom-2;
		}
		else		//el mouse esta en el borde derecho
		{
			rcBar.OffsetRect(-rect.TopLeft());
			rcWin=rcBar;
			if (IsBottom() || IsTop())	//abajo
			{		
				rcWin.left=rect.left+2;
				rcWin.right=rect.right+2;
			}
		}
	}
	ClientToScreen(&rcWin);
	GetParentFrame()->ScreenToClient(&rcWin);
	// invert the brush pattern (looks just like frame window sizing)
	CBrush* pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush = NULL;
	if (pBrush != NULL)
		hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);
	pDC->PatBlt(rcWin.left, rcWin.top, rcWin.Width(), rcWin.Height(), PATINVERT);
	if (hOldBrush != NULL)
		SelectObject(pDC->m_hDC, hOldBrush);
	m_pDockSite->ReleaseDC(pDC);
}


void CGuiControlBar::OnSize(UINT nType, int cx, int cy)
{
//	CControlBar::OnSize(nType, cx, cy);
	CWnd* pWnd = GetWindow(GW_CHILD);
	if (!m_bSupportMultiView)
	{
        if (pWnd != NULL)
        {
            pWnd->MoveWindow(0, 0, cx, cy);
            ASSERT(pWnd->GetWindow(GW_HWNDNEXT) == NULL);
		}
	}
	else
	{
		while (pWnd != NULL)
        {
            if (pWnd->IsWindowVisible())
			{
				pWnd->MoveWindow(0, 0, cx, cy);
				break;
			}
            pWnd=pWnd->GetWindow(GW_HWNDNEXT);
		}
	}
	//m_pDockSite->RecalcLayout();
	// TODO: Add your message handler code here
}
void CGuiControlBar::SetColorFondo(COLORREF clrFondo)
{
	m_clrFondo=clrFondo;
	
}

//enum State{NORMAL=0,OVER=1,PRESS=2};


CGuiControlBarButton::CGuiControlBarButton()
{
	bTypeButton=GUINORMAL;
	m_bEnabled=TRUE;
}


CGuiControlBarButton::~CGuiControlBarButton()
{

}

void CGuiControlBarButton::SetTypeButton(GuiType bTypeb)
{
	bTypeButton=bTypeb;
}


void CGuiControlBarButton::SetEnabled(BOOL bEnabled)
{
	m_bEnabled=bEnabled;
}

void CGuiControlBarButton::Paint(CDC* pDC,int st,CRect rc,COLORREF clrFondo)
{
	CBrush cb;
	if(bTypeButton==GUINORMAL) 
		cb.CreateSolidBrush(clrFondo);
	else
	{
		if (m_bEnabled)
			if (st == OVER)
				cb.CreateSolidBrush(GuiDrawLayer::GetRGBFondoXP());
			else if(st == PRESS)
				cb.CreateSolidBrush(GuiDrawLayer::GetRGBPressBXP());
			else
				cb.CreateSolidBrush(GuiDrawLayer::GetRGBColorFace());
	}

	if (m_bEnabled)
	{
		pDC->FillRect(rc,&cb);
		if (st == OVER)
			pDC->Draw3dRect(rc,bTypeButton==GUINORMAL?::GetSysColor(COLOR_BTNHIGHLIGHT):GuiDrawLayer::GetRGBCaptionXP(),
							   bTypeButton==GUINORMAL?::GetSysColor(COLOR_BTNSHADOW):GuiDrawLayer::GetRGBCaptionXP());
		if (st == PRESS)
			pDC->Draw3dRect(rc,bTypeButton==GUINORMAL?::GetSysColor(COLOR_BTNSHADOW):GuiDrawLayer::GetRGBCaptionXP(),
			bTypeButton==GUINORMAL?::GetSysColor(COLOR_BTNHIGHLIGHT):GuiDrawLayer::GetRGBCaptionXP());
	}
	if(bTypeButton==GUINORMAL) 
		m_imgList.Draw(pDC,clrFondo == ::GetSysColor(COLOR_ACTIVECAPTION)? m_nIcon+1:m_nIcon, 
						CPoint(rc.left+1,rc.top+2), ILD_TRANSPARENT);				
	else
	{
		if (m_bEnabled)
			m_imgList.Draw(pDC,m_nIcon, CPoint(rc.left+1,rc.top+1), ILD_TRANSPARENT);				
		else
		{
			HICON m_Icon=m_imgList.ExtractIcon(m_nIcon);
			ICONINFO bm;
			ZeroMemory(&bm, sizeof(ICONINFO));
			::GetIconInfo(m_Icon,&bm);
			CSize m_SizeImage =CSize((BYTE)(bm.xHotspot*2),(BYTE)(bm.yHotspot*2));
			pDC->DrawState(CPoint(rc.left+1,rc.top+1),m_SizeImage,m_Icon,DSS_MONO,CBrush (GuiDrawLayer::GetRGBColorShadow()));
		}
	}
}

void CGuiControlBarButton::SetData(int nIcon,LPCTSTR lpMsg)
{
	m_nIcon=nIcon;
	m_lpMsg=lpMsg;
}

void  CGuiControlBarButton::SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask)
{
	CBitmap cbmp;
	BITMAP bmp;
	m_imgList.Create(nBitmapID,cx,nGrow,crMask);
	cbmp.LoadBitmap(nBitmapID);
	cbmp.GetBitmap(&bmp);
	m_imgList.Add(&cbmp,crMask);

}


void CGuiControlBar::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_stateBtn != NORMAL) return;
	if (nHitTest == HTCLOSE)
	{
		m_stateBtn=OVER;
		SetTimer(1,100,0);
	}
	SendMessage(WM_NCPAINT);
	CControlBar::OnNcMouseMove(nHitTest, point);
}

void CGuiControlBar::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (m_stateBtn==NORMAL) return;
	CRect rc;
	CPoint pt(GetMessagePos());
	
	CRect rcT=m_rcCloseBtn;
	ClientToScreen(rcT);
	pt.y+=23;
	pt.x+=5;
	if (!rcT.PtInRect(pt))
	{
		m_stateBtn=NORMAL;
		KillTimer(1);
		SendMessage(WM_NCPAINT); 		
	}
	CControlBar::OnTimer(nIDEvent);
}

void CGuiControlBar::OnNcLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_pDockBar != NULL)
	{

		m_pDockContext->ToggleDocking();
		m_rcOldBorder=CRect(0,0,0,0);
				
	}	
}



