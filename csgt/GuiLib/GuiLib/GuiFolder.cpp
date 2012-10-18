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
#include "GuiFolder.h"
#include "GuiDrawLayer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuiFolder
IMPLEMENT_DYNAMIC(CGuiFolder,CWnd)

CFolderBar::CFolderBar(CWnd* m_wnd,LPCSTR lp_msg,UINT uImg)
{
	m_Cwnd=m_wnd;
	m_lpMsg=lp_msg;
	m_img=uImg;
	m_rect=CRect(0,0,0,0);
	m_posUp=TRUE;
	
}


CGuiFolder::CGuiFolder()
{
	m_ArrayFolder.RemoveAll();
	m_NumFolders=0;
	m_numFolderDown=0;
	m_numFolderUp=0;
	m_iSelected=0;	
	m_ultimoFolderResal =-1;
	m_itemp=0;
	m_typel=mOutlook;//by default
	m_iWhatFolderIsDrawNow=0;
}

CGuiFolder::~CGuiFolder()
{
	/* Modified By SunZhenyu 2003/8/22, Add next 7 lines. */
	for(int i=0; i<m_ArrayFolder.GetSize(); i++)
	{
		CFolderBar * p = (CFolderBar*)m_ArrayFolder[i];
		if( p )
			delete	p;
	}
	m_ArrayFolder.RemoveAll();
}


BEGIN_MESSAGE_MAP(CGuiFolder, CWnd)
	//{{AFX_MSG_MAP(CGuiFolder)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGuiFolder message handlers

BOOL CGuiFolder::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CWnd::Create(NULL,_T(""), dwStyle|WS_CHILD, rect, pParentWnd, nID);
}

void CGuiFolder::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int m_iNumBotton=m_ArrayFolder.GetSize();
	for( int iCont=0; iCont< m_iNumBotton;iCont++)
	{
		CFolderBar* cit=(CFolderBar*)m_ArrayFolder[iCont];
		if (cit->m_rect.PtInRect(point))
		{
			DrawFolder(cit,mPush);
		}
	}		

	CWnd::OnLButtonDown(nFlags, point);
}

void CGuiFolder::PreSubclassWindow() 
{
	ModifyStyle(0,BS_OWNERDRAW);
	CWnd::PreSubclassWindow();
}

void CGuiFolder::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int m_iNumBotton=m_ArrayFolder.GetSize();
	for( int iCont=0; iCont< m_iNumBotton;iCont++)
	{
		CFolderBar* cit=(CFolderBar*)m_ArrayFolder[iCont];
		if (cit->m_rect.PtInRect(point))
		{
			DrawFolder(cit,mFlat);
			m_itemp=m_iSelected;
			m_iSelected=iCont;
			EfectoScroll();
		}
	}		

	CWnd::OnLButtonUp(nFlags, point);
}

void CGuiFolder::TypeLook(TypeFolder tol)
{
	m_typel=tol;
}

void CGuiFolder::EfectoScroll()
{
	int m_iNumBotton=m_ArrayFolder.GetSize();
	int m_iSentidoScroll=0;	//1=hacia abajo, 2=hacia arriba
	if (m_iNumBotton > 0) //si existe un solo folder no haga nada
	{
		// si es el ultimo folder y no esta abajo no haga nada
		if (m_iSelected == m_NumFolders-1 &&
			((CFolderBar*)m_ArrayFolder[m_iSelected])->m_posUp ==TRUE)	
		{
			m_iSelected=m_itemp;
			return;
		}
		else if (((CFolderBar*)m_ArrayFolder[m_iSelected])->m_posUp ==TRUE )
		{
			if (((CFolderBar*)m_ArrayFolder[m_iSelected+1])->m_posUp ==FALSE)
				return;
			m_iSentidoScroll =1;
				//de esta posición todos los folders hacia abajo 
			for (int iCont=m_iSelected+1;iCont < m_iNumBotton; iCont++)
				((CFolderBar*)m_ArrayFolder[iCont])->m_posUp =FALSE;
				
		}
		else if (((CFolderBar*)m_ArrayFolder[m_iSelected])->m_posUp ==FALSE)
		{
				m_iSentidoScroll =2;
				//de esta posición todos los folders van arriba
				for (int iCont=m_iSelected;iCont >0; iCont--)
					((CFolderBar*)m_ArrayFolder[iCont])->m_posUp =TRUE;
		}
		
	}

	CRect m_rect=((CFolderBar*)m_ArrayFolder[m_iSelected])->m_rect;
	CClientDC dc(this);
	if (m_iSentidoScroll >0)
	{
		CWnd* pParent=((CFolderBar*)m_ArrayFolder[m_itemp])->m_Cwnd;
		ASSERT_VALID(pParent);
		CWnd* pParentn=((CFolderBar*)m_ArrayFolder[m_iSelected])->m_Cwnd;
		ASSERT_VALID(pParentn);
		pParent->ShowWindow(SW_HIDE);
		pParentn->ShowWindow(SW_SHOW);
		m_AntFolder=m_iSelected;
	}
	
	if (m_iSentidoScroll ==1)
	{
		/*CGuiFolderBar* cb=(CGuiFolderBar*)m_ArrayFolder[m_iSelected];
		CWnd* m_cwnd=((CGuiFolderBar*)m_ArrayFolder[m_iSelected])->m_Cwnd;
		CRect rctemp=cb->m_rect;
		while (m_rect.bottom < m_iPosDown)
		{
			m_cwnd->SetWindowPos(NULL,m_rect.left,cb->m_rect.bottom,m_rect.Width(),m_rect.bottom,
			SWP_NOACTIVATE|SWP_NOZORDER);
			m_rect.bottom+=10;
			//DrawFolder(cb,mFlat);
			//m_cwnd->Invalidate();
			//m_cwnd->SendMessage(WM_PAINT,0,0);
			
			
			
		}*/
		RecalLayout();	
	}
	else if (m_iSentidoScroll ==2)
	{
		/*CWnd* m_cwnd=((CGuiFolderBar*)m_ArrayFolder[m_iSelected])->m_Cwnd;
		CGuiFolderBar* cb=(CGuiFolderBar*)m_ArrayFolder[m_iSelected];
		while (m_rect.top > m_iposUp)
		{
			m_cwnd->SetWindowPos(NULL,m_rect.left,m_iposUp-1,m_rect.Width(),m_iPosDown-m_iposUp,
			SWP_NOACTIVATE|SWP_NOZORDER);
			cb->m_rect.top-=10;
			cb->m_rect.bottom-=10;
			Sleep(50);
			DrawFolder(cb,mFlat);
			
			
		}
		*/
		RecalLayout();	
	}
	else
		m_iSelected=m_itemp;
		



}

void CGuiFolder::OnMouseMove(UINT nFlags, CPoint point) 
{
	int m_iNumBotton= m_ArrayFolder.GetSize();
	static int m_AntTecla=-1;
	for( int iCont=0; iCont< m_iNumBotton;iCont++)
	{
		CFolderBar* cit=(CFolderBar*)m_ArrayFolder[iCont];
		m_iWhatFolderIsDrawNow=iCont;
		if (cit->m_rect.PtInRect(point))
		{
			if (m_ultimoFolderResal==m_AntTecla &&
				(m_ultimoFolderResal!=-1 && m_AntTecla !=-1)) return;
			else
			{
			  if (m_ultimoFolderResal != -1)
			  {
				CFolderBar* cit1=(CFolderBar*)m_ArrayFolder[m_ultimoFolderResal];	
				DrawFolder(cit1,mFlat);
			  }
			}
			DrawFolder(cit,mNormal);
			m_AntTecla=m_ultimoFolderResal;
			m_ultimoFolderResal=iCont;
			SetTimer(1,100,NULL);
			return;			
		}
	}	

	if (m_ultimoFolderResal != -1)
	 {
		CFolderBar* cit1=(CFolderBar*)m_ArrayFolder[m_ultimoFolderResal];	
		DrawFolder(cit1,mFlat);
		m_ultimoFolderResal=-1;
	 }
	m_AntTecla=-1;

	CWnd::OnMouseMove(nFlags, point);
}

void CGuiFolder::DrawFolder(CFolderBar* cfb,Style m_Style)
{
	CRect rc=cfb->m_rect;
	CBrush cb;
	CClientDC pDC(this);
	cb.CreateSysColorBrush(COLOR_BTNFACE);
	rc.top+=1;
	rc.right-=1;
	pDC.FillRect(&rc,&cb);
	if (m_typel!=mOutlook)
	{
		rc.right-=30;
	}
		
	if (m_Style == mNormal)
	{
				
		pDC.Draw3dRect(rc, GuiDrawLayer::GetRGBColorBTNHigh(),GuiDrawLayer::GetRGBColorShadow());
		rc.DeflateRect(1,1);
		pDC.Draw3dRect(rc, GuiDrawLayer::GetRGBColorFace(),GuiDrawLayer::GetRGBColorShadow());
	}
	else if (m_Style == mFlat)
		pDC.Draw3dRect(rc, GuiDrawLayer::GetRGBColorBTNHigh(),GuiDrawLayer::GetRGBColorShadow());
	else if (m_Style == mPush)
		pDC.Draw3dRect(rc, GuiDrawLayer::GetRGBColorShadow(),GuiDrawLayer::GetRGBColorBTNHigh());
	int OldMode=pDC.SetBkMode(TRANSPARENT);
	cfont = pDC.SelectObject(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
	CString m_cadBreak=cfb->m_lpMsg;
	int cont=m_cadBreak.GetLength();
	CSize coor=pDC.GetTextExtent(m_cadBreak,m_cadBreak.GetLength());
	if ( coor.cx > rc.Width())
	{
		rc.left+=1;
		while(cont > 1 )
		{
			CString m_scadtemp=m_cadBreak+"...";
			CSize coor=pDC.GetTextExtent(m_scadtemp,m_scadtemp.GetLength());
			if(coor.cx > rc.Width())
				m_cadBreak=m_cadBreak.Left(m_cadBreak.GetLength()-1);
			else
				break;
			cont--;
		}
		m_cadBreak+=_T("...");
		rc.right+=3;
		
			pDC.DrawText(m_cadBreak,rc,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	}
	else
	{
	  if (m_typel!=mOutlook)
			pDC.DrawText(cfb->m_lpMsg,rc,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	  else	
			pDC.DrawText(cfb->m_lpMsg,rc,DT_WORDBREAK|DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	}

   	pDC.SetBkColor(OldMode);
	// Modified By SunZhenyu
	pDC.SelectObject(cfont);
}


void CGuiFolder::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect mrect;
	int m_iNumFolder=m_ArrayFolder.GetSize();
	//mrect.InflateRect(1,1);
	//GetClientRect(mrect);
	//dc.Draw3dRect(mrect, ::GetSysColor(COLOR_BTNSHADOW),::GetSysColor(COLOR_BTNHIGHLIGHT));
	for ( int iCont=0; iCont< m_iNumFolder; iCont++)
	{
		m_iWhatFolderIsDrawNow=iCont;
		if (m_iSelected== iCont)
			DrawFolder((CFolderBar*) m_ArrayFolder[iCont],mFlat);
		else
			DrawFolder((CFolderBar*) m_ArrayFolder[iCont],mFlat);		

	}		
}

void CGuiFolder::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if (m_NumFolders > 0)
		RecalLayout();
	// TODO: Add your message handler code here
	
}

void CGuiFolder::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		CRect rc;
		CPoint pt(GetMessagePos());
		ScreenToClient (&pt);
		GetClientRect(&rc);
		if (m_ultimoFolderResal != -1)
		{
			CFolderBar* cfd=(CFolderBar*) m_ArrayFolder[m_ultimoFolderResal];
			rc=cfd->m_rect;
		}
		
		if (!rc.PtInRect(pt))
		{
			if (m_ultimoFolderResal != -1)
			{
			CFolderBar* cfd=(CFolderBar*) m_ArrayFolder[m_ultimoFolderResal];
			CClientDC dc(this);
			DrawFolder(cfd,mFlat);
			m_ultimoFolderResal=-1;
			}
			KillTimer(1);
		}
		
	}
	CWnd::OnTimer(nIDEvent);
}

int CGuiFolder::PointInFolder(CPoint pt)
{
	for (int i=0; i <	m_ArrayFolder.GetSize(); i++)
	{
		if ( ((CFolderBar*) m_ArrayFolder[i])->m_rect.PtInRect(pt) )
			return i;
	}
	return -1;
}

BOOL CGuiFolder::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CGuiFolder::RecalLayout()
{
	CRect m_rect;
	CRect m_rectFolder;
	m_iPosDown=0; //a partir de donde se ubica el anterior folder
	m_iposUp=0;   //a partir de donde se ubica el siguiente folder
	int m_iNumFolder=m_ArrayFolder.GetSize();
	GetClientRect(&m_rect);
	m_rectFolder=m_rect;
	m_iPosDown=m_rect.bottom;
	for ( int iCont=0; iCont< m_iNumFolder; iCont++)
	{
		CFolderBar* cfd=(CFolderBar*) m_ArrayFolder[iCont];
		if (iCont==0) //inevitablemente arriba
		{
			m_rectFolder.bottom=m_rect.top+21;
			m_iposUp+=m_rectFolder.Height()-1;
			cfd->m_rect=m_rectFolder;
		}
		else
		{
			if (cfd->m_posUp)
			{
				m_rectFolder.top=m_iposUp;
				m_rectFolder.bottom=m_rectFolder.top+20;
				m_iposUp+=m_rectFolder.Height();
				cfd->m_rect=m_rectFolder;
			}
			else
			{
				
				for(int iRev=m_iNumFolder-1; iRev>=iCont; iRev--)
				{
					CFolderBar* cfd=(CFolderBar*) m_ArrayFolder[iRev];
					m_rectFolder.bottom=m_iPosDown;
					m_rectFolder.top=m_rectFolder.bottom-20;
					m_iPosDown-=m_rectFolder.Height();
					cfd->m_rect=m_rectFolder;
				}
				break;
			}
		}
		
	}
	CFolderBar* cfd=(CFolderBar*) m_ArrayFolder[m_iSelected];
	CWnd*	m_cwnd=cfd->m_Cwnd;
	m_cwnd->SetWindowPos(NULL,m_rect.left+1,m_iposUp+1,m_rectFolder.Width()-1,(m_iPosDown-m_iposUp),
		SWP_NOACTIVATE|SWP_NOZORDER);
	Invalidate();
	UpdateWindow();

}

void CGuiFolder::AddFolder(CWnd* pParent,LPCSTR lpMsg, UINT uIMg)
{
	m_ArrayFolder.SetAtGrow(m_NumFolders, new CFolderBar(pParent,lpMsg,uIMg));
	if (m_NumFolders == 0)
	  m_numFolderUp=1;
	pParent->ShowWindow(SW_SHOW);
	m_iSelected=m_NumFolders;
	m_AntFolder=m_iSelected;
	m_NumFolders++;
	RecalLayout();
}


BOOL CGuiFolder::OnEraseBkgnd(CDC* pDC) 
{
	CBrush cbr;
	cbr.CreateSolidBrush(GuiDrawLayer::GetRGBColorFace());
	CRect m_rect;
	GetClientRect(m_rect);
	if (m_NumFolders<= 0)
		pDC->FillRect(m_rect,&cbr);
	else
	{
		for (int i=0;i<2; i++)
		{
			pDC->Draw3dRect(m_rect,GuiDrawLayer::GetRGBColorFace(),GuiDrawLayer::GetRGBColorFace());
			m_rect.DeflateRect(1,1);
		}
	}
	return TRUE;
}

void CGuiFolder::DeleteFolder(int m_iNumFolder)
{
	/* Modified By SunZhenyu 2003/8/22, replace all function lines. */
	if (m_iNumFolder >= 0 && m_iNumFolder < m_NumFolders)
	{
		m_NumFolders--;
		CFolderBar * p = (CFolderBar *)m_ArrayFolder.GetAt(m_iNumFolder);
		if( p )	delete	p;
		m_ArrayFolder.RemoveAt(m_iNumFolder);

		if (m_iNumFolder == 0 || m_iNumFolder < m_NumFolders)
			m_iSelected=m_iNumFolder;
		else if(m_iNumFolder > 0)
			m_iSelected=m_iNumFolder-1;
		RecalLayout();
	}
	/*
	if (m_iNumFolder < m_NumFolders)
	{
		m_iNumFolder--;
		m_ArrayFolder.RemoveAt(m_iNumFolder,m_NumFolders);
		if (m_iNumFolder == 0 || m_iNumFolder == m_iNumFolder)
			m_iSelected=m_iNumFolder;
		else if(m_iNumFolder > 0)
			m_iSelected=m_iNumFolder-1;
		RecalLayout();
	}
	*/
}

void CGuiFolder::RemoveAllFolder()
{
	/* Modified By SunZhenyu 2003/8/22, Add next 6 lines. */
	for(int i=0; i<m_ArrayFolder.GetSize(); i++)
	{
		CFolderBar * p = (CFolderBar*)m_ArrayFolder[i];
		if( p )
			delete	p;
	}

	m_ArrayFolder.RemoveAll();
	RecalLayout();
	m_NumFolders=-1;
	m_iSelected=-1;
}

