//-----------------------------------------------------------------------//
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
#include "GuiOutLook.h"
#include "resource.h"
#include "GuiDrawlayer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuiOutLook

IMPLEMENT_DYNCREATE(CGuiOutLook, CWnd)

CGuiOutLook::CGuiOutLook()
{
	m_ArrayButtons.RemoveAll();
	m_sizeImage=CSize(20,20);
	m_ActualButtonPres=-1;
	m_cbkg=::GetSysColor(COLOR_3DSHADOW);
	m_NumBottoms=0;
	m_mouseMove=-1;
	m_ultimoBotonResal=-1;
	m_Avance=0;    //avance del scroll
	m_iDrawFrameUp=FALSE;		//dibujar o no los botones arriba 
	m_iDrawFrameDown=FALSE;	    //y abajo
	m_bIsXp=FALSE;
}

CItems::CItems(CGuiToolButton* button,HICON hIcon,CString szMessage,CString szToolTip)
{
	m_bt=button;
	m_bt->SethIcon(hIcon);
	m_bt->SetToolTip(szToolTip);
	m_szMessage=szMessage;
	m_btipo=TRUE;
}

CGuiOutLook::~CGuiOutLook()
{
	/* Modified By SunZhenyu 2003/8/22, Add next 7 lines. */
	for(int i=0; i<m_ArrayButtons.GetSize(); i++)
	{
		CItems * p = (CItems*)m_ArrayButtons[i];
		if( p )
			delete	p;
	}
	m_ArrayButtons.RemoveAll();
}


BEGIN_MESSAGE_MAP(CGuiOutLook, CWnd)
	//{{AFX_MSG_MAP(CGuiOutLook)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGuiOutLook message handlers

BOOL CGuiOutLook::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	mnID=nID;
	return CWnd::Create(NULL,_T(""), dwStyle|WS_CHILD, rect, pParentWnd, nID);
}

BOOL CGuiOutLook::PreTranslateMessage(MSG* pMsg) 
{
	return CWnd::PreTranslateMessage(pMsg);
}

int CGuiOutLook::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}


BOOL CGuiOutLook::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CBrush cbm;
	CRect rect;
	GetClientRect(&rect);
	rect.left+=1;
	rect.OffsetRect(-1,0);
	cbm.CreateSolidBrush(GuiDrawLayer::GetRGBColorXP());
	pDC->FillRect(rect,&cbm);
	return TRUE;
}

void CGuiOutLook::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int m_iNumBotton= m_ArrayButtons.GetSize();
	CClientDC  dc(this);
	m_bPressU=FALSE;
	m_bPressD=FALSE;
	
	if (m_iDrawFrameUp==TRUE)
	{
		if (m_rectArrowUp.PtInRect(point))
		{
			dc.DrawFrameControl(m_rectArrowUp,DFC_SCROLL,DFCS_SCROLLUP|DFCS_PUSHED);
			m_bPressU=TRUE;
			SetTimer(1,100,NULL);
			m_bPressU=FALSE;
		}
	}
	if (m_iDrawFrameDown==TRUE)
	{
		if (m_rectArrowDown.PtInRect(point))
		{
			dc.DrawFrameControl(m_rectArrowDown,DFC_SCROLL,DFCS_SCROLLDOWN|DFCS_PUSHED);
			m_bPressD=TRUE;
			SetTimer(1,100,NULL);
			m_bPressD=FALSE;
		}
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CGuiOutLook::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
	{
		CPoint pt(GetMessagePos());
		CRect m_rc;
		ScreenToClient(&pt);
		GetClientRect(m_rc);
		if (!m_rc.PtInRect(pt))
		{
			KillTimer(1);
			CClientDC dc(this);
			if (m_bPressU==TRUE)
				dc.DrawFrameControl(m_rectArrowUp,DFC_SCROLL,DFCS_SCROLLUP);
			if (m_bPressD==TRUE)
				dc.DrawFrameControl(m_rectArrowDown,DFC_SCROLL,DFCS_SCROLLDOWN);
		}
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CGuiOutLook::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	//------------------------------
	if (m_iDrawFrameUp==TRUE)
	{
		if (m_rectArrowUp.PtInRect(point))
		{
			m_Avance+=50;		
			RecalLayout();		
		}
		else
			dc.DrawFrameControl(m_rectArrowUp,DFC_SCROLL,DFCS_SCROLLUP);
	}
	//-----------------------------
	if (m_iDrawFrameDown==TRUE)
	{
		if (m_rectArrowDown.PtInRect(point))
		{
			m_Avance-=50;
			RecalLayout();
		}
		else
			dc.DrawFrameControl(m_rectArrowDown,DFC_SCROLL,DFCS_SCROLLDOWN);
	}
	//------------------------------
	CWnd::OnLButtonUp(nFlags, point);
}

void CGuiOutLook::RecalLayout()
{
	static int m_iLastHeigh=0; //se almacena el ultimo valor de altura del area cliente
	CRect rect;
	CRect m_rectC;
	GetClientRect(&m_rectC);
	CClientDC dc(this);
	int iMitad;
	int iMinRequerido=0;
	iMitad= m_rectC.Width()/2;
	iMitad-=(m_sizeImage.cx/2); //posición a partir del cual se pintan los botones
	iMitad--;
	m_iDrawFrameUp=FALSE;		//dibujar o no los botones arriba 
	m_iDrawFrameDown=FALSE;	    //y abajo
	
	int m_iNumBotton= m_ArrayButtons.GetSize();
	for( int iCont=1; iCont< m_iNumBotton;iCont++)
	{
		CItems* cit=(CItems*)m_ArrayButtons[iCont];
		CSize sz=cit->m_bt->GetSizeButton();
		iMinRequerido+=sz.cy+18;
	}

	int iy=5+m_Avance; //a partir de esta posición se comienza a escribir
	//calculo real del espacio que ocuparan los botones
	CRect rcMsg;
	for( iCont=1; iCont< m_iNumBotton;iCont++)
	{
		CItems* cit=(CItems*)m_ArrayButtons[iCont];
		rect.left=iMitad;
		rect.top=iy;
		rect.bottom=rect.top+m_sizeImage.cy+6;
		rect.right=rect.left+m_sizeImage.cx+6;
		cit->m_bt->MoveWindow(rect);
		iy+=rect.Height()+18;
	}	
	
	
	DeleteDC(dc);
	if (iy > m_rectC.Height())
		m_iDrawFrameDown=TRUE;

	if (m_Avance < 0)
		m_iDrawFrameUp=TRUE;
	m_iLastHeigh=m_rectC.Height();
	Invalidate();
	UpdateWindow();

}


void CGuiOutLook::OnSysColorChange() 
{
	CWnd::OnSysColorChange();
	int m_iNumBotton= m_ArrayButtons.GetSize();
	for( int iCont=1; iCont< m_iNumBotton;iCont++)
	{
		CItems* cit=(CItems*)m_ArrayButtons[iCont];
		cit->m_bt->SetColor(GuiDrawLayer::GetRGBColorXP());
	}
	Invalidate ();
	UpdateWindow ();
}

void CGuiOutLook::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if (m_NumBottoms > 0)
		RecalLayout();
}


BOOL CGuiOutLook::OnCommand(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND) lParam;	
	for (int i=1; i < m_NumBottoms+1;i++)
	{
		if (((CItems*) m_ArrayButtons [i])->m_bt->m_hWnd== hwnd)
			{
				int x=((CItems*) m_ArrayButtons [i])->m_bt->GetDlgCtrlID();
				CWnd* pParent= GetParent();
				pParent->SendMessage (WM_COMMAND,x);
				AfxGetMainWnd()->SendMessage(WM_COMMAND,x);
				return TRUE;
			}
	
	}
	return FALSE;

}
void CGuiOutLook::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);
	CRect rcMsg;
	int iy=0;
	int m_iNumBotton= m_ArrayButtons.GetSize();
	int iMitad= rect.Width()/2;
	iMitad-=(m_sizeImage.cx/2); //posición a partir del cual se pintan los botones
	iMitad--;
	int nMode=dc.SetBkMode(TRANSPARENT);
	CFont* cfont = dc.SelectObject(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
	for( int iCont=1; iCont< m_iNumBotton;iCont++)
	{
		CItems* cit=(CItems*)m_ArrayButtons[iCont];
		CRect rcWin;
		cit->m_bt->GetWindowRect(rcWin);
		ScreenToClient(rcWin);
		rcMsg=rcWin;
		rcMsg.top=rcWin.bottom+2;
		rcMsg.bottom=rcMsg.top+16;
		rcMsg.left=rect.left;
		rcMsg.right=rect.right;
		dc.DrawText(cit->m_szMessage,rcMsg,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	}	
	dc.SetBkMode(nMode);
	dc.SelectObject(cfont);
	if (m_iDrawFrameUp==TRUE)
	{
		m_rectArrowUp.top=5;
		m_rectArrowUp.bottom=m_rectArrowUp.top+16;
		m_rectArrowUp.right=rect.right-5;
		m_rectArrowUp.left=m_rectArrowUp.right-16;
		dc.DrawFrameControl(m_rectArrowUp,DFC_SCROLL,DFCS_SCROLLUP);
	}
	if (m_iDrawFrameDown==TRUE)
	{
		m_rectArrowDown.bottom=rect.bottom-5;
		m_rectArrowDown.top=m_rectArrowDown.bottom-16;
		m_rectArrowDown.right=rect.right-5;
		m_rectArrowDown.left=m_rectArrowDown.right-16;
		dc.DrawFrameControl(m_rectArrowDown,DFC_SCROLL,DFCS_SCROLLDOWN);

	}
}
void CGuiOutLook::SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask)
{
	CBitmap cbmp;
	BITMAP hbmp;
	cbmp.LoadBitmap(nBitmapID);
	cbmp.GetBitmap(&hbmp);
	m_imageList.Create(nBitmapID,cx,nGrow,crMask);
	m_imageList.Add(&cbmp,crMask);
	m_sizeImage=CSize(cx,hbmp.bmHeight);
}

void  CGuiOutLook::AddItem(UINT Cmd,CString m_szCaption,int nIndex,CString m_ToolTip)
{
	CGuiToolButton* m_guib=new CGuiToolButton();
	m_NumBottoms++;
	m_guib->Create(_T(""), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW, 
		CRect(0,0,0,0), this, Cmd);
	//m_guib->SetColor(GuiDrawLayer::GetRGBColorShadow());
	//m_guib->ShowDark(FALSE);
	m_ArrayButtons.SetAtGrow(m_NumBottoms, new CItems
			(m_guib,m_imageList.ExtractIcon(nIndex),m_szCaption,m_ToolTip));
	
	RecalLayout();

}