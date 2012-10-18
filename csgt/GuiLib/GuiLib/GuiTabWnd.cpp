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

#include "GuiTabWnd.h"
#include "GuiBasetab.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuiTabWnd

#define  SPACE_TAB		4
#define  DEFAULT_FLAT	18

CGuiTabWnd::CGuiTabWnd()
{
	m_rectCliente.SetRectEmpty();
	m_rectTab.SetRectEmpty();
	m_ActualVar=0;
	m_Numtabs=0;
	m_iSelectTab=0;
	m_sizeImag=CSize(15,15);
	m_pArray.RemoveAll();
	m_iMinValRec=0;  //minimo valor requerido para mostrar toda las carpetas
	
	// Modified Bu SunZhenyu
	if( !m_cfont.CreateStockObject( DEFAULT_GUI_FONT ) )
		m_cfont.CreateStockObject( ANSI_VAR_FONT );
	// m_cfont.CreateFont(-11,0,0,0,400,0,0,0,0,1,2,1,34,"MS Sans Serif");
	m_style=S3D;
	m_ActualTitle=TRUE;
	m_alnTab=ALN_BOTTOM;
	m_TypeTab=TYPE_NORMAL;

	/*Modified By SunZhenyu 2003/11/11, add the next 2 line */
	m_clrSelectTabBK	=	::GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrSelectTabText	=	::GetSysColor(COLOR_BTNTEXT);
}

CGuiTabWnd::~CGuiTabWnd()
{
	/* Modified By SunZhenyu 2003/8/22, Add next 7 lines. */
	for(int i=0; i<m_pArray.GetSize(); i++)
	{
		CGuiTab * p = (CGuiTab*)m_pArray[i];
		if( p )
			delete	p;
	}
	m_pArray.RemoveAll();
}


BEGIN_MESSAGE_MAP(CGuiTabWnd, CWnd)
	//{{AFX_MSG_MAP(CGuiTabWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
	// Modified By SunZhenyu, 2003/11/11, add the next 1 line
	ON_MESSAGE(WM_GUITABWND_COLORCHANGE, OnColorChange)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGuiTabWnd message handlers

BOOL CGuiTabWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_MOUSEMOVE)
		CToolTip.RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}

//****************************************************************************
void CGuiTabWnd::OnSysColorChange( )
{
	CWnd::OnSysColorChange( );
	
}

void CGuiTabWnd::SetTypeTab(TypeTab Type)
{
	m_TypeTab=Type;
}
//****************************************************************************
BOOL CGuiTabWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_Numtabs == 0)
	{
		CBrush cbr;
		cbr.CreateSysColorBrush(COLOR_BTNFACE);
		CRect m_rect;
		GetClientRect(m_rect);
		pDC->FillRect(m_rect,&cbr);
		
	}
	return TRUE;
}

void CGuiTabWnd::SetActualTitle(BOOL bActual)
{
	m_ActualTitle=bActual;
}

void CGuiTabWnd::SetALingTabs(AlingTab alnTab)
{
	m_alnTab=alnTab;
	Invalidate();
	UpdateWindow();
}

void CGuiTabWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CBrush cbr;
	CRect m_rectDraw;
	if (m_alnTab!=ALN_TOP)
		cbr.CreateSolidBrush(GuiDrawLayer::GetRGBColorXP());
	else
		cbr.CreateSolidBrush(GuiDrawLayer::GetRGBColorFace());
	GetClientRect(m_rectCliente);
	m_rectTab=m_rectCliente;
	m_rectDraw=m_rectCliente;
	if (m_alnTab==ALN_BOTTOM)
		m_rectDraw.bottom=m_rectCliente.bottom-(m_sizeImag.cy+SPACE_TAB+2);	
	else
		m_rectDraw.top=m_rectCliente.top+(m_sizeImag.cy+SPACE_TAB+2);
	
	if (m_alnTab!=ALN_TOP)
	{
		/* Modified By SunZhenyu 2003/10/26, Remove next 2 lines. */
		// dc.FillRect(m_rectTab,&cbr);
		// dc.Draw3dRect(m_rectDraw,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNSHADOW));
	}
	else
	{
		CRect rc=m_rectDraw;
		for (int i=0; i< 2; i++)
		{
			dc.Draw3dRect(rc,GuiDrawLayer::GetRGBColorFace(),GuiDrawLayer::GetRGBColorFace());
			rc.DeflateRect(1,1);
		}
		
		rc.InflateRect(3,3);
		dc.Draw3dRect(rc,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNSHADOW));
		rc=m_rectDraw;
		rc.bottom=rc.top+3;
		dc.FillRect(rc,&cbr);
		
	}
	
	
	if (m_style != S3D) //si son carpetas estilo excel o tabbed como devstudio 7
	{
		if (m_alnTab==ALN_BOTTOM)
		{
			m_rectCliente.bottom=m_rectCliente.bottom-(DEFAULT_FLAT);
			m_rectTab.top=m_rectCliente.bottom+2;
		}
		else
		{
			m_rectTab.bottom=m_rectCliente.top+(DEFAULT_FLAT)+3;
		}
	}

	Drawtabs(&dc);
	
}



BOOL CGuiTabWnd::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	// TODO: Add your specialized code here and/or call the base class
	pParent= pParentWnd;
	return CWnd::Create(NULL, _T(""), dwStyle, rect, pParentWnd, nID);
}

int CGuiTabWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CToolTip.Create(this,TTS_ALWAYSTIP);
	CToolTip.Activate(TRUE);
	CToolTip.BringWindowToTop();
	// TODO: Add your specialized creation code here
	
	return 0;
}


void CGuiTabWnd::RecalLayout()
{
	GetClientRect(m_rectCliente);

	if (m_style == S3D)
	{
		m_rectCliente.bottom=m_rectCliente.bottom-(m_sizeImag.cy+SPACE_TAB+1);
	}
	else
	{
		if (m_alnTab==ALN_BOTTOM)
			m_rectCliente.bottom=m_rectCliente.bottom-(DEFAULT_FLAT);
		else
		{
			m_rectCliente.top+=DEFAULT_FLAT;
			m_rectCliente.bottom=m_rectCliente.bottom-(DEFAULT_FLAT+4);

		}
	}

	for (int iCont=0; iCont< m_Numtabs;iCont++)
	{
		CGuiTab* ct=(CGuiTab*) m_pArray[iCont];
		CWnd* m_tempWin= ct->pParent;
		ASSERT_VALID(m_tempWin);
		if (m_style == S3D)
		{
			   m_tempWin->SetWindowPos(NULL,m_rectCliente.left+(SPACE_TAB-2),
			   m_rectCliente.top+(SPACE_TAB/2),m_rectCliente.right-(SPACE_TAB+1),
			   m_rectCliente.bottom-(SPACE_TAB+2),SWP_NOACTIVATE|
			   SWP_NOZORDER);
		}
		else
		{
			if (m_alnTab==ALN_BOTTOM)	
			{
			   m_tempWin->SetWindowPos(NULL,m_rectCliente.left+(SPACE_TAB-3),
			   m_rectCliente.top+3,m_rectCliente.right-2,
			   m_rectCliente.bottom-1,SWP_NOACTIVATE|SWP_NOZORDER);
			}
			else
			{ 
				if (m_TypeTab == TYPE_NORMAL)
				{
				  m_tempWin->SetWindowPos(NULL,m_rectCliente.left+(SPACE_TAB-3),
				  m_rectCliente.top+3,m_rectCliente.right-2,
				  m_rectCliente.bottom,SWP_NOACTIVATE|SWP_NOZORDER);
				}
			}
		}
	}
	AjustTabs();
	Invalidate();
	UpdateWindow();
}


void CGuiTabWnd::AjustTabs()
{
	int m_iSizeAct=0;
	m_iMinValRec=4;
	CClientDC dc(this);
	GetClientRect(m_rectCliente);
	if (m_alnTab==ALN_BOTTOM)
		m_rectCliente.bottom=m_rectCliente.bottom-(m_sizeImag.cy+SPACE_TAB+1);
	else
		m_rectCliente.bottom=m_rectCliente.top+(m_sizeImag.cy+SPACE_TAB+1);

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
	//si el espacio requerido es mayor que el disponible
	//se debe proporcionar cada tab solo si es 3d
	if (m_style== S3D)
	{
		int m_NewResize=4;
		if (m_iMinValRec > m_rectCliente.Width()-4)
		{
			m_iSizeAct=(m_rectCliente.Width()-12)/m_Numtabs;
			for (int iCont=0; iCont< m_Numtabs;iCont++)
			{
				CGuiTab* ct=(CGuiTab*) m_pArray[iCont];
				CRect m_Arect;
				m_Arect.left=m_NewResize;
				m_Arect.right=m_NewResize+m_iSizeAct;
				m_Arect.top=m_rectCliente.bottom;
				m_Arect.bottom=m_rectCliente.bottom+4+m_sizeImag.cy;
				m_NewResize+=m_iSizeAct+1;
				ct->rect= m_Arect;
			}	
		}
	}

}
void CGuiTabWnd::Drawtabs(CDC* dc)
{
	CPen light(PS_SOLID,1,GetSysColor(COLOR_BTNHIGHLIGHT));
	CPen Dark(PS_SOLID,1,GetSysColor(COLOR_BTNSHADOW));
	CPen Black(PS_SOLID,1,GetSysColor(BLACK_PEN));	
	CRect rectText;
	CFont* m_fontOld=dc->SelectObject(&m_cfont);
	for (int iCont=0; iCont< m_Numtabs;iCont++)
	{
		CGuiTab* ct=(CGuiTab*) m_pArray[iCont];
		CPen* oldPen= dc->SelectObject(&light);
		//si es la carpeta seleccionada, se debe 
		//eliminar las lineas oscuras de encima
		if (iCont ==m_iSelectTab)
		{
			CBrush cbr;
			CRect  m_rectTabAux=ct->rect;
			m_rectTabAux.DeflateRect(1,1);
			m_rectTabAux.top-=4;	
			m_rectTabAux.bottom=m_rectTabAux.top+4;
			cbr.CreateSysColorBrush(COLOR_BTNFACE);
			
			dc->FillRect(ct->rect,&cbr);
			dc->MoveTo(ct->rect.left,ct->rect.top-3);
			dc->LineTo(ct->rect.left,ct->rect.bottom-1);
			dc->FillRect(m_rectTabAux,&cbr);

			dc->SelectStockObject(BLACK_PEN);
			dc->MoveTo(ct->rect.left,ct->rect.bottom-1);
			dc->LineTo(ct->rect.right,ct->rect.bottom-1);
			
			//linea derecha observe que se pinta dos veces para
			//dar el efecto de redondeada en la puntas

			dc->SelectStockObject(BLACK_PEN);
			dc->MoveTo(ct->rect.right-1,ct->rect.top-1);
			dc->LineTo(ct->rect.right-1,ct->rect.bottom-1);

		}
		else
		{
			if ((iCont+1) != m_iSelectTab)
			{
				dc->SelectObject(&Dark);
				dc->MoveTo(ct->rect.right-1,ct->rect.top+2);
				dc->LineTo(ct->rect.right-1,ct->rect.bottom-2);
			}
				
			
		}
		//se dibuja el icono
		int nMode = dc->SetBkMode(TRANSPARENT);
		CSize m_sChar=dc->GetTextExtent(ct->lpMsg,strlen(ct->lpMsg));
		if (m_sizeImag.cx+m_sChar.cx < ct->rect.Width()-8)  
		{
			Image.Draw(dc,ct->uIcon,CPoint(ct->rect.left+2,ct->rect.top+2),ILD_TRANSPARENT);
			rectText=ct->rect;
			rectText.left+=m_sizeImag.cx+8;
			rectText.right-=2;
			dc->DrawText(ct->lpMsg,rectText,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
			CToolTip.SetToolRect(this,iCont+1,CRect(0,0,0,0));
		}
		else
		{
			//verificamos si se puede poner algun texto o
			//por lo memos el icono
			if (m_sizeImag.cx < ct->rect.Width())
			{
				Image.Draw(dc,ct->uIcon,CPoint(ct->rect.left+2,ct->rect.top+2),ILD_TRANSPARENT);
				rectText=ct->rect;
				rectText.left+=m_sizeImag.cx+8;
				rectText.right-=5;
				CString m_cadBreak=ct->lpMsg;
				int c=0;
				int cont=m_cadBreak.GetLength();
				while(cont > 1 )
				{
					CString m_scadtemp=m_cadBreak+"...";
					CSize coor=dc->GetTextExtent(m_scadtemp,m_scadtemp.GetLength());
					if(coor.cx > rectText.Width())
						m_cadBreak=m_cadBreak.Left(m_cadBreak.GetLength()-1);
					else
						break;
					cont--;

				}
				m_cadBreak+=_T("...");
				rectText.right+=3;
				dc->DrawText(m_cadBreak,rectText,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
				CToolTip.SetToolRect(this,iCont+1,&ct->rect);
			}
			
		}
		dc->SetBkMode(nMode);
		
		// Modified By SunZhenyu
		dc->SelectObject(oldPen);
	}	
	
	dc->SelectObject(m_fontOld);
}

void CGuiTabWnd::SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask)
{
	CBitmap cbmp;
	BITMAP bmp;
	Image.Create(nBitmapID,cx,nGrow,crMask);
	cbmp.LoadBitmap(nBitmapID);
	cbmp.GetBitmap(&bmp);
	Image.Add(&cbmp,crMask);
	m_sizeImag=CSize(cx,bmp.bmHeight);	
	
}

CWnd* CGuiTabWnd::GetActiveWnd () const
{
		return m_Numtabs == -1 ? NULL : 
		((CGuiTab*) m_pArray [m_Numtabs])->pParent;
}

BOOL CGuiTabWnd::FindItem(HWND hWnd)
{
	for (int iCont=0; iCont< m_Numtabs;iCont++)
	{
		CGuiTab* ct=(CGuiTab*) m_pArray[iCont];
		if (ct->pParent->GetSafeHwnd() == hWnd)
		    return TRUE;
	}
	return FALSE;

}

CWnd*  CGuiTabWnd::GetNumWnd(int m_numtab) const
{
        if (m_numtab > m_Numtabs)
			return NULL;
		else
		{
			if (m_pArray.GetSize() < 1) return NULL;
			CWnd* pw=((CGuiTab*) m_pArray [m_numtab])->pParent;
			if (HIWORD(pw)== NULL) return NULL;
			else
			  return ((CGuiTab*) m_pArray [m_numtab])->pParent;
		}
}

void CGuiTabWnd::UpdateCaption(CWnd* pWnd,CString m_Title)
{
	for (int iCont=0; iCont< m_Numtabs;iCont++)
	{
		CGuiTab* ct=(CGuiTab*) m_pArray[iCont];
		if (ct->pParent->GetSafeHwnd() == pWnd->GetSafeHwnd())
		{
			CString x=ct->lpMsg;
			if(x!=m_Title)
			{
				ct->lpMsg=m_Title;
				GetParentFrame()->SendMessage(WM_SIZE);
				RecalLayout();
				Invalidate();
				UpdateWindow();
			}
			break;
		}
	}
	
}
void CGuiTabWnd::ShowTitle(CString m_Caption)
{
	if (m_ActualTitle==TRUE)
	{
		CWnd* pParent=GetParent();
		pParent->SetWindowText(m_Caption);
		pParent->SendMessage(WM_COMMAND,WM_SHOWTITLE);
	}
	
}
void CGuiTabWnd::Addtab(CWnd *pParent, CString lpMsg, UINT uIcon)
{
	ASSERT_VALID(pParent);
	m_pArray.SetAtGrow(m_Numtabs,new CGuiTab(pParent,lpMsg,uIcon));
	if (m_Numtabs==0)
	{
	//	ShowTitle(lpMsg);
		pParent->ShowWindow(SW_SHOW);
		m_iSelectTab=m_Numtabs;
	}
	else
		pParent->ShowWindow(SW_HIDE);
	m_Numtabs++;
	if (m_alnTab==ALN_TOP)	
		pParent->ShowWindow(SW_SHOWMAXIMIZED);
	if (m_style == S3D)
		CToolTip.AddTool(this,lpMsg,CRect(0,0,0,0),m_Numtabs);
	RecalLayout();
}


void CGuiTabWnd::DeleteTab(int m_numtab)
{
	if (m_numtab < m_Numtabs)
	{
		CGuiTab* ctn=(CGuiTab*) m_pArray[m_numtab]; 	
		CWnd* pParen=ctn->pParent;
		if (m_alnTab!=ALN_TOP)
			ASSERT_VALID(pParen);
		m_Numtabs--;
		/* Modified By SunZhenyu 2003/8/22, Add next 1 lines. */
		delete	ctn;
		m_pArray.RemoveAt(m_numtab,1);
		if (m_Numtabs >0)
		{
			if (m_iSelectTab == m_numtab)
			{
				m_iSelectTab=0;
				CGuiTab* ctn =(CGuiTab*) m_pArray[m_iSelectTab];
				CWnd* pParentNew=ctn->pParent;
				ASSERT_VALID(pParentNew);
				pParentNew->ShowWindow(SW_SHOW);
			}
			RecalLayout();
		}
		
	}
}

void CGuiTabWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if (m_Numtabs > 0)
		RecalLayout();
	
}


void CGuiTabWnd::SetCurtab(int m_numtab)
{
	if (m_numtab < m_Numtabs && m_numtab != m_iSelectTab )
	{
		CGuiTab* ctn=(CGuiTab*) m_pArray[m_numtab]; 
		CWnd* pParentNew= ctn->pParent;
		CGuiTab* ct =(CGuiTab*) m_pArray[m_iSelectTab];
		CWnd* pParentActual=ct->pParent;
		ASSERT_VALID(pParentNew);
		ASSERT_VALID(pParentActual);
		if (m_alnTab==ALN_TOP)	
		{
			pParentNew->BringWindowToTop();
		}
		else
		{
			pParentActual->ShowWindow(SW_HIDE);
			pParentNew->ShowWindow(SW_SHOW);
		}
		
		m_iSelectTab=m_numtab;
		Invalidate();
		UpdateWindow();
	}

	// Modified By SunZhenYu, 2003/10/28, add next 3 lines
	CWnd * pParent = GetParent();
	if( pParent )
	{
		WPARAM wParam = GetDlgCtrlID();
		NMHDR hdr;
		hdr.hwndFrom = GetSafeHwnd();
		hdr.idFrom = GetDlgCtrlID();
		hdr.code = TCN_SELCHANGE;
		pParent->SendMessage( WM_NOTIFY, wParam, (LPARAM)&hdr);
	}
}

int CGuiTabWnd::GetCurtab()
{
	return m_iSelectTab;
}


void CGuiTabWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnLButtonDown(nFlags, point);
	for (int iCont=0; iCont< m_Numtabs;iCont++)
	{
		CGuiTab* ct=(CGuiTab*) m_pArray[iCont];
		if (ct->rect.PtInRect(point) != 0)
		{
            SetCurtab(iCont);
			ShowTitle(ct->lpMsg);
			return;
		}
		
	}

}

void CGuiTabWnd::SetStyle(Style estyle)
{
	m_style=estyle;
}

// Modified By SunZhenyu, 2003/10/28, add the next function
CRect CGuiTabWnd::GetTabRect( int m_numtab )
{
	if( m_numtab >= 0 && m_numtab < m_Numtabs )
	{
		CGuiTab* ct=(CGuiTab*) m_pArray[m_numtab];
		CRect	rect = ct->rect;
		ClientToScreen( &rect );
		return rect;
	}
	return CRect(0,0,0,0);
}

// Modified By SunZhenyu, 2003/11/09, add the next function
void CGuiTabWnd::SetSelectTabColor( COLORREF clrBK, COLORREF clrText )
{
	m_clrSelectTabBK	=	clrBK;
	m_clrSelectTabText	=	clrText;
}

// Modified By SunZhenyu, 2003/11/11, add the next 1 function
LRESULT CGuiTabWnd::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	SetSelectTabColor( wParam, lParam );
	Invalidate( );
	return 0L;
}
