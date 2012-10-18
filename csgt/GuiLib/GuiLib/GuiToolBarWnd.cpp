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
#include "GuiToolBarWnd.h"
#include "resource.h"
#include "GuiDrawlayer.h"
#include "GuiDockContext.h"
//#include "GuiColors.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define BTNS_SHOWTEXT           0x0040

extern int		gbintHorz;			//Horz=0, Vert=1
extern CRect	rcMenu;				//CRect of button

class CDropmenu
{
public:
	UINT nIDboton;
	UINT nMenu;
	CDropmenu(UINT nID,UINT nMENU)
	{
		nIDboton=nID;
		nMenu=nMENU;
	}
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CGuiToolBarWnd,CToolBar)

BEGIN_MESSAGE_MAP(CGuiToolBarWnd,CToolBar)
	//{{AFX_MSG_MAP(CGuiToolBarWnd)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT_EX(TBN_DROPDOWN,OnDropDown)
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()

	//}}AFX_MSG_MAP
 END_MESSAGE_MAP()


int _afxDropDownWidth = -1;

int AFXAPI _AfxGetDropDownWidth()
{
	// return cached version if already determined...
	if (_afxDropDownWidth != -1)
		return _afxDropDownWidth;

	// otherwise calculate it...
	HDC hDC = GetDC(NULL);
	ASSERT(hDC != NULL);
	HFONT hFont;
	if ((hFont = CreateFont(GetSystemMetrics(SM_CYMENUCHECK), 0, 0, 0,
		FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0, _T("Marlett"))) != NULL)
		hFont = (HFONT)SelectObject(hDC, hFont);
	VERIFY(GetCharWidth(hDC, '6', '6', &_afxDropDownWidth));
	if (hFont != NULL)
	{
		SelectObject(hDC, hFont);
		DeleteObject(hFont);
	}
	ReleaseDC(NULL, hDC);
	ASSERT(_afxDropDownWidth != -1);
	return _afxDropDownWidth;
}

int CGuiToolBarWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->dwExStyle |= WS_CLIPCHILDREN; // prevents flashing
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (m_pDockContext==NULL)
		m_pDockContext=new CGuiDockContext(this);
	ASSERT(m_pDockContext);
	//ModifyStyle(WS_CAPTION,0);
	
	return 0;
}


//***********************************************************************************
void CGuiToolBarWnd::OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler ) 
{
	 CToolBar::OnUpdateCmdUI( pTarget, bDisableIfNoHndler ); 
}

//***********************************************************************************
void CGuiToolBarWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CToolBar::OnWindowPosChanging(lpwndpos);
	if(!(m_dwStyle & CBRS_FLOATING))  
		SetSizes(m_sizeButton, m_sizeImage);
	
	if( !(lpwndpos->flags & SWP_NOMOVE) )
		Invalidate(FALSE);
}

//***********************************************************************************
void CGuiToolBarWnd::OnSysColorChange( )
{
	CToolBar::OnSysColorChange( );
	if(!(m_dwStyle & CBRS_FLOATING))  
		SetSizes(m_sizeButton, m_sizeImage);
	Invalidate(TRUE);
	GetParentFrame()->RecalcLayout();
}

//***********************************************************************************

CGuiToolBarWnd::CGuiToolBarWnd()
{
	m_style = Office2000; //Office97;  //by default
	m_pArray.RemoveAll();
	m_iElements=0;
	m_bIsXp=TRUE;
	bPressDown=FALSE;
	m_imgArrow.Create(IDB_DOCKBAR,9,10,RGB(255,0,255));
	_AfxGetDropDownWidth();
	//m_iWidthDrowDown=GetSystemMetrics(SM_CXHTHUMB);
	CBitmap cbmp;
	BITMAP bmp;
	cbmp.LoadBitmap(IDB_DOCKBAR);
	cbmp.GetBitmap(&bmp);
	mHeight=CPoint(bmp.bmWidth,bmp.bmHeight);	
	// Modified Bu SunZhenyu
	if( !m_cfont.CreateStockObject( DEFAULT_GUI_FONT ) )
		m_cfont.CreateStockObject( ANSI_VAR_FONT );
	// m_cfont.CreateFont(14,0,0,0,400,0,0,0,0,1,2,1,34,_T("Marlett"));
}

CGuiToolBarWnd::~CGuiToolBarWnd()
{
	/* Modified By SunZhenyu 2003/8/22, Add next 7 lines. */
	for(int i=0; i<m_pArray.GetSize(); i++)
	{
		CDropmenu * p = (CDropmenu*)m_pArray[i];
		if( p )
			delete	p;
	}
	m_pArray.RemoveAll();
}

void CGuiToolBarWnd::SetXp(BOOL m_bXp)
{
	m_bIsXp=m_bXp;
}

void CGuiToolBarWnd::SetTextButton(int IdButton,CString sztexto)
{
//	CToolBarCtrl &tbc=GetToolBarCtrl();
//	tbc.SetStyle(tbc.GetStyle()| TBSTYLE_LIST | CCS_ADJUSTABLE);
	
	sztexto+=sztexto.Find( '&' ) == -1 ?"   ":"    ";
	CClientDC dc(this);
	SetButtonText(IdButton,sztexto);
	SetButtonStyle(IdButton,TBSTYLE_AUTOSIZE);
	SetSizes(m_sizeButton, m_sizeImage);
            
}


void CGuiToolBarWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CToolBar::OnNcCalcSize(bCalcValidRects, lpncsp);
}


BOOL CGuiToolBarWnd::CreateCombo(CComboBox* pControl,UINT nID,int iSize,DWORD dwStyle)
{
	CRect rect;
	CFont m_Font;
	m_Font.CreateStockObject (DEFAULT_GUI_FONT);
	int nIndex = this->GetToolBarCtrl().CommandToIndex(nID);
	this->SetButtonInfo(nIndex, nID, TBBS_SEPARATOR, iSize);
	this->GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.bottom-=1;
	rect.top = 1;
	rect.bottom = rect.top +iSize/*drop height*/;
	if(!pControl->Create(dwStyle, rect, this, nID))
	{
		TRACE(_T("Failed to create combo-box\n"));
		return FALSE;
	}
	
	return TRUE;
}


BOOL CGuiToolBarWnd::CreateCombo(CComboBoxEx* pControl,UINT nID,int iSize,CImageList* m_imgList,DWORD dwStyle)
{
	CRect rect;
	int nIndex = this->GetToolBarCtrl().CommandToIndex(nID);
	this->SetButtonInfo(nIndex, nID, TBBS_SEPARATOR, iSize);
	this->GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.top = 1;
	rect.bottom = rect.top + 250 /*drop height*/;
	if(!pControl->Create(dwStyle, rect, this, nID))
	{
		TRACE(_T("Failed to create combo-box\n"));
		return FALSE;
	}
	pControl->SetImageList(m_imgList);
	return TRUE;
	
}

void CGuiToolBarWnd::AddRowComBoboxEx(CComboBoxEx* pControl,CString szMsg,int iItem,int iImage)
{
	COMBOBOXEXITEM cb;
	cb.mask=CBEIF_TEXT | CBEIF_IMAGE |CBEIF_SELECTEDIMAGE;       

	cb.pszText=(LPTSTR)(LPCTSTR)szMsg;
	cb.iItem=iItem;
	cb.iImage=cb.iSelectedImage=iImage;
	cb.cchTextMax=20; 
	pControl->InsertItem(&cb);

}

void CGuiToolBarWnd::SetButtonDropDown(UINT nID,int iImage,UINT nMENU)
{
	CToolBarCtrl &m_tbc= GetToolBarCtrl();
	m_tbc.SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	m_tbc.SetStyle(m_tbc.GetStyle() | TBSTYLE_LIST | CCS_ADJUSTABLE);
	int nIndex = this->GetToolBarCtrl().CommandToIndex(nID);
	this->SetButtonInfo(nIndex, nID, TBBS_DROPDOWN, iImage);
	m_pArray.SetAtGrow(m_iElements,new CDropmenu(nID,nMENU));
	m_iElements++;
}

BOOL CGuiToolBarWnd::OnDropDown(NMHDR* pNMHDR, LRESULT* pRes)
{
	
	const NMTOOLBAR& m_nmTb = *(NMTOOLBAR*)pNMHDR;
	CRect rc;
	GetToolBarCtrl().GetRect(m_nmTb.iItem, rc);
	ClientToScreen(&rc);
	for(int i=0; i < m_iElements; i++)
	{
		if (((CDropmenu*)m_pArray[i])->nIDboton == (UINT)m_nmTb.iItem)
			break;
	}
	bPressDown=TRUE;
	CMenu m_menu;
	m_menu.LoadMenu(((CDropmenu*)m_pArray[i])->nMenu);
	CMenu* m_SubMenu = m_menu.GetSubMenu(0);
	gbintHorz=0;			//Horz=0, Vert=1
	rcMenu=rc;					//CRect of button
	m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,
			rc.left, rc.bottom-2, AfxGetMainWnd(), &rc);
	bPressDown=FALSE;
	return TRUE;
}

void CGuiToolBarWnd::OnNcPaint()
{

	CRect rcWindow;
	CRect rcClient;
	CWindowDC	dc(this);
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);

    rcClient.OffsetRect(-rcWindow.TopLeft());
    rcWindow.OffsetRect(-rcWindow.TopLeft());
    ScreenToClient(rcWindow);
	rcClient.OffsetRect(-rcWindow.left,-rcWindow.top);
	//if (!m_bIsXp)
	if (m_dwStyle & CBRS_ORIENT_HORZ && !(m_dwStyle & CBRS_FLOATING))
	{rcClient.top+=2;rcClient.left+=1;}
		
	dc.ExcludeClipRect(rcClient);   
	rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);
	rcWindow.DeflateRect(1,1);
	//pintamos la izquierda
	dc.FillSolidRect(0,0,rcWindow.right+1,rcWindow.bottom+2,::GetSysColor(COLOR_BTNFACE)); 
	dc.FillSolidRect(0,rcWindow.top+1,rcWindow.right+1,1,::GetSysColor(COLOR_BTNFACE)); 
	//pintamos el borde inferior
	dc.FillSolidRect(1,rcWindow.bottom-1,rcWindow.right+1,rcWindow.bottom-1,::GetSysColor(COLOR_BTNFACE)); 
	dc.FillSolidRect(0,rcWindow.bottom+1,rcWindow.right+1,rcWindow.bottom+1,::GetSysColor(COLOR_BTNFACE )); 
	//pintamos la derecha
	dc.FillSolidRect(rcWindow.right,0,rcWindow.right-1,rcWindow.bottom,::GetSysColor(COLOR_BTNFACE)); 
	dc.FillSolidRect(rcWindow.right+1,-1,rcWindow.right+4,rcWindow.bottom+4,::GetSysColor(COLOR_BTNFACE )); 
	

	dc.IntersectClipRect(rcWindow);
	CBrush cb;
	cb.CreateSolidBrush(GuiDrawLayer::GetRGBColorXP());
	dc.FillRect(rcWindow,&cb);

	DrawGripper(&dc,&rcWindow);	
		
}

void CGuiToolBarWnd::OnPaint()
{

	CPaintDC dc(this);  	
	CRect rcBtn;
	
	int nBtn=GetToolBarCtrl().GetButtonCount();
	int iBitmap;
	UINT nID;
	UINT nStyle;
	WORD wStyle, wState;
	DWORD dwBarStyle = GetToolBarCtrl().GetStyle();
	BOOL mbIsPress;
	BOOL mbIsEnabled;
	CBrush cb;
	CImageList m_imgList;
	CRect rcUpdate;
	int iUltbot;
	CRect rcClient;
	GetClientRect(&rcClient);
	cb.CreateSolidBrush(GuiDrawLayer::GetRGBColorXP());
	
    // draw borders in non-client area
    CRect rcDraw = rcClient;
 
	dc.FillRect(rcDraw, &cb);
	
	
	HIMAGELIST hiImg = 0;
	iBitmap=0;
	hiImg = HIMAGELIST(SendMessage(TB_GETIMAGELIST));
	m_imgList.Attach(hiImg);
	int j=0;
	for(int i=0; i < nBtn; i++)
	{
		
		bOver=FALSE;
		GetItemRect(i,&rcBtn);
		GetButtonInfo(i,nID,nStyle,iBitmap);
		wStyle= LOWORD(nStyle);
		wState= HIWORD(nStyle);
		if (wState & TBSTATE_HIDDEN) continue;
		if (wStyle == TBSTYLE_SEP)
		{
			if (!(wState & TBSTATE_WRAP) || ! IsFloating() )
			{
				CRect m_Separ;
				m_Separ=rcBtn;
				
				//si existe un separador vertical
				if (m_dwStyle & CBRS_ORIENT_HORZ)
				{
					dc.FillRect(&rcBtn,&cb);
					int nhalf=(m_Separ.Width()/2)-1;
					m_Separ.top-=1;
					m_Separ.left+=nhalf;
					m_Separ.right=m_Separ.left+1;
					m_Separ.bottom+=1;
					
				}
				else
				{
					int nhalf=(m_Separ.Width()/2)-1;
					int iHeigh=m_Separ.Height();
					
					m_Separ.left=0;
					m_Separ.right=m_Separ.left+m_Separ.Height()+1;
					m_Separ.top=iUltbot;
					m_Separ.bottom=m_Separ.top+m_Separ.Width();			
					dc.FillRect(&m_Separ,&cb);
					
					m_Separ.left=0;
					m_Separ.right=m_Separ.left+iHeigh+1;
					m_Separ.top=iUltbot+nhalf;
					m_Separ.bottom=m_Separ.top+1;
					
					
				}
				CBrush cbt;
				cbt.CreateSolidBrush(RGB(167, 167, 167));
				dc.FillRect(m_Separ,&cbt);
				continue;
				
			}
			
				 
		}
		else {
			
			mbIsPress=(wState & TBSTATE_CHECKED) || (wState & TBSTATE_PRESSED);
			mbIsEnabled= GetToolBarCtrl().IsButtonEnabled((int)nID);
			CPoint pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			CBrush cblu;
			if(!rcBtn.PtInRect(pt))
			{
				if (wState & TBSTATE_CHECKED)
				{
					cblu.CreateSolidBrush(GuiDrawLayer::GetRGBPressBXP());
					dc.FillRect(rcBtn,&cblu);					
				}
				else
				{
					dc.FillRect(rcBtn,&cb);
				}
			}
			else
			{
				
				if (bPressDown)
					dc.Draw3dRect(rcBtn,GuiDrawLayer::GetRGBColorShadow(),GuiDrawLayer::GetRGBColorShadow());
				else
					dc.Draw3dRect(rcBtn,GuiDrawLayer::GetRGBCaptionXP(),GuiDrawLayer::GetRGBCaptionXP());
				rcBtn.DeflateRect(1,1);
				if (wState & TBSTATE_CHECKED)
				{
					cblu.CreateSolidBrush(GuiDrawLayer::GetRGBFondoXP());
					dc.FillRect(rcBtn,&cblu);					
				}
				else
				{
					
					if (bPressDown)
					{
						cblu.CreateSolidBrush(GuiDrawLayer::GetRGBColorXP());
						dc.FillRect(rcBtn,&cblu);
					}
					else
					{
						cblu.CreateSolidBrush(mbIsPress?GuiDrawLayer::GetRGBPressBXP():GuiDrawLayer::GetRGBFondoXP());
						dc.FillRect(rcBtn,&cblu);					
					}
					if (wStyle == 8) 
					{
						
						CRect rcCli=rcBtn;
						int dif=rcBtn.right-_afxDropDownWidth;
						rcCli.left=dif+1;
						
						if (!bPressDown)
						{
							CPen Cp( PS_SOLID, 1, GuiDrawLayer::GetRGBCaptionXP());
							CPen* oldPen= dc.SelectObject(&Cp);
							dc.MoveTo(dif,rcBtn.top);
							dc.LineTo(dif,rcBtn.bottom);
							dc.SelectObject(oldPen);
						}
						
					}
					if (!mbIsPress && !bPressDown)
						bOver=TRUE;
					else
						bOver=FALSE;
					if (!bPressDown) 
						SetTimer(1,10,NULL);
					else
						KillTimer(1);		

				}
				rcBtn.InflateRect(1,1);
				
			}

		}

		if (wStyle != TBSTYLE_SEP)
	{
			
			int OldMode=dc.SetBkMode(TRANSPARENT);
			IMAGEINFO pImageInfo;
			m_imgList.GetImageInfo(iBitmap,&pImageInfo);
			CRect rcima =pImageInfo.rcImage;
			CSize siImg=CSize(rcima.Width(),rcima.Height());
			/* Modified By SunZhenyu 2003/6/27, Replace by the next 5 lines, CPoint pti(rcBtn.left+2,rcBtn.top+4); */
			CPoint pti(rcBtn.left+(rcBtn.Width()-siImg.cx)/2,rcBtn.top+4);
			if( dwBarStyle & TBSTYLE_LIST ) // ÎÄ×ÖÔÚÓÒ±ß
				pti = CPoint(rcBtn.left+4,rcBtn.top+(rcBtn.Height()-siImg.cy)/2);
			else if( wStyle & TBSTYLE_DROPDOWN )
				pti.x -= _AfxGetDropDownWidth()/2;
			CString mszText=GetButtonText(i);
			CFont* m_fontOld=dc.SelectObject(&m_cfont);
			if (!(wState & TBSTATE_ENABLED))
			{
				if (bOver==TRUE)
				{
					pti.x+=1;
					pti.y+=1;
					dc.DrawState(pti,siImg,m_imgList.ExtractIcon(iBitmap),DSS_MONO,CBrush (GuiDrawLayer::GetRGBColorShadow()));
							pti.x-=2;					pti.y-=2;
				
				}
				m_imgList.Draw(&dc, iBitmap, pti, ILD_TRANSPARENT);
				if (mszText.GetLength() > 0 && !bVertDocked)
				{
					CRect m_rctext=rcBtn;
					/* Modified By SunZhenyu 2003/6/27, Replace by the next 6 line, m_rctext.left+=siImg.cx+4; */
					if( dwBarStyle & TBSTYLE_LIST )
						m_rctext.left += siImg.cx+4;
					else
						m_rctext.top += siImg.cy+4;
					if( wStyle & TBSTYLE_DROPDOWN )
						m_rctext.right -= _AfxGetDropDownWidth();
					m_rctext.bottom-=1;
					/* Modified By SunZhenyu 2003/6/27, Replace by the next 5 line
					dc.DrawText(mszText,&m_rctext,DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
					*/
					m_rctext.right-=1;
					if( dwBarStyle & TBSTYLE_LIST )
						dc.DrawText(mszText,&m_rctext,DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					else
						dc.DrawText(mszText,&m_rctext,DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
				}
			}
			else
			{
				dc.DrawState(pti,siImg,m_imgList.ExtractIcon(iBitmap),DSS_DISABLED,(CBrush*)NULL);
				CRect m_rctext=rcBtn;
				/* Modified By SunZhenyu 2003/6/27, Replace by the next 5 line, m_rctext.left+=siImg.cx+4; */
				if( dwBarStyle & TBSTYLE_LIST )
					m_rctext.left += siImg.cx+4;
				else
					m_rctext.top += siImg.cy+4;
				m_rctext.bottom-=1;
				CSize szText=dc.GetTextExtent(mszText);
				/* Modified By SunZhenyu 2003/6/27, Replace by the next 4 lines,
				pti.x= m_rctext.left+2;
				pti.y= (m_rctext.Height()/2)-1;
				*/
				pti.x = m_rctext.left+(m_rctext.Width()-szText.cx)/2;
				if( wStyle & TBSTYLE_DROPDOWN )
					pti.x = m_rctext.left+(m_rctext.Width()-szText.cx-_AfxGetDropDownWidth())/2;
				pti.y= m_rctext.top+(m_rctext.Height()-szText.cy)/2;
				//DrawState(pt, m_Csize, m_szCaption, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
				dc.DrawState(pti,szText,mszText,DSS_DISABLED,TRUE,0,(HBRUSH)NULL);
			}
			
			if (wStyle == TBSTYLE_DROPDOWN) 
				DrawArrow(&dc,rcBtn);
			dc.SetBkMode(OldMode);
			// Modified By SunZhenyu
			dc.SelectObject(m_fontOld);
			iBitmap++;
			iUltbot=rcBtn.bottom;
			j++;
		}
	}
	m_imgList.Detach();
	
	
    ReleaseDC(&dc);

  
}


void CGuiToolBarWnd::DrawArrow(CDC* pDC,CRect m_rc)
{
	int difh =m_rc.Height()-mHeight.y;
	difh/=2;
	
	m_rc.left=m_rc.right-_AfxGetDropDownWidth();
	m_imgArrow.Draw(pDC,0,CPoint(m_rc.left+2,m_rc.top+difh),ILD_TRANSPARENT);		
	
}

void CGuiToolBarWnd::DrawGripper(CWindowDC* dc,CRect* rcWin)
{

	if (m_dwStyle & CBRS_FLOATING) return ;
	if(!m_bIsXp) //no es XP
	{
		if (m_dwStyle & CBRS_ORIENT_HORZ)
		{
			rcWin->top+=2;
			rcWin->left+=2;
			rcWin->right=rcWin->left+3;
			rcWin->bottom-=1;
			dc->Draw3dRect(rcWin,::GetSysColor(COLOR_BTNHIGHLIGHT),
										 ::GetSysColor(COLOR_BTNSHADOW));
			if (m_style == Office97)
			{
				rcWin->left=rcWin->right+1;
				rcWin->right=rcWin->left+3;
				dc->Draw3dRect(rcWin,::GetSysColor(COLOR_BTNHIGHLIGHT),
										 ::GetSysColor(COLOR_BTNSHADOW));
			}
		}
		else
		{
			rcWin->top+=2;
			//rcWin->left+=1;
			rcWin->right-=2;
			rcWin->bottom=rcWin->top+3;
			dc->Draw3dRect(rcWin,::GetSysColor(COLOR_BTNHIGHLIGHT),
										 ::GetSysColor(COLOR_BTNSHADOW));
			if (m_style == Office97)
			{
				rcWin->top=rcWin->bottom+1;
				rcWin->bottom=rcWin->top+3;
				dc->Draw3dRect(rcWin,::GetSysColor(COLOR_BTNHIGHLIGHT),
										 ::GetSysColor(COLOR_BTNSHADOW));
			}

		}
	}
	else
	{
		if (m_dwStyle & CBRS_ORIENT_HORZ)
		{
			
			rcWin->top+=5;
			rcWin->left+=4;
			rcWin->right=rcWin->left+3;
			rcWin->bottom-=3;

			for (int i=0; i < rcWin->Height(); i+=2)
			{
				CRect rcWindow;
				CBrush cb;
				cb.CreateSolidBrush(::GetSysColor(COLOR_BTNSHADOW));
				rcWindow=rcWin;
				rcWindow.top=rcWin->top+i;
				rcWindow.bottom=rcWindow.top+1;
				dc->FillRect(rcWindow,&cb);
		
			}
			
		}
		else
		{
			rcWin->top+=2;
			rcWin->left+=2;
			rcWin->right-=2;
			rcWin->bottom=rcWin->top+3;
			for (int i=0; i < rcWin->Width(); i+=2)
			{
				CRect rcWindow;
				CBrush cb;
				cb.CreateSolidBrush(::GetSysColor(COLOR_BTNSHADOW));
				rcWindow=rcWin;
				rcWindow.left=rcWindow.left+i;
				rcWindow.right=rcWindow.left+1;
				dc->FillRect(rcWindow,&cb);
		
			}

		}
	}

}

void CGuiToolBarWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CToolBar::OnLButtonDown(nFlags, point);
	
	if (m_iSelected == 1) return;
	m_iSelected =1;
	

		
}

BOOL CGuiToolBarWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{

	CPoint ptCurPos; 
	CRect rc;GetClientRect(rc);
	GetCursorPos (&ptCurPos);
	ScreenToClient (&ptCurPos);
	if (m_dwStyle & CBRS_ORIENT_HORZ)
	{
		rc.right=rc.left+2;
		if (ptCurPos.x< 0)
		{
			SetCursor (LoadCursor(NULL, IDC_SIZEALL));
			return TRUE;
		}
	}
	else
	{
		rc.bottom=rc.top+2;
		if (ptCurPos.y< 0)
		{
			SetCursor (LoadCursor(NULL, IDC_SIZEALL));
			return TRUE;
		}
	}
	return CToolBar::OnSetCursor(pWnd, nHitTest, message);

}



void CGuiToolBarWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CToolBar::OnLButtonUp(nFlags, point);
	
	if (m_iSelected == 1)
	{
		m_iSelected =0;
		KillTimer(1);
		SetTimer(1,10,NULL);
	}
}

void CGuiToolBarWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CRect m_rect;
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);
	GetClientRect(m_rect);
	if (nIDEvent == 1)
	{
		
		if (!m_rect.PtInRect(pt))
		{
			m_iSelected =0;
			KillTimer(1);
			Invalidate();
			UpdateWindow();
			
		}
	}
	CToolBar::OnTimer(nIDEvent);
}



//**************************************************************************
int CGuiToolBarWnd::GetLastPos()
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

//**************************************************************************
int CGuiToolBarWnd::GetFirstPos()
{
	int m_pos=m_pDockBar->FindBar(this);
	for(int i=m_pos; i>=0;i--)
	{
		if (m_pDockBar->m_arrBars[i]== NULL)
		    return i+1;	
	}
	return -1;

}
//**************************************************************************
CRect CGuiToolBarWnd::GetDockRect()
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
//**************************************************************************
int CGuiToolBarWnd::GetHiWid()
{
	CRect rcWin;
	rcWin=GetDockRect();
	if (IsVert())
		return rcWin.Height() ;
	else
		return rcWin.Width() ;
}
//**************************************************************************

void CGuiToolBarWnd::SetRealSize()
{
	CRect rcWinFrame;
	CRect rcThisBar;
	
	int nVisibles=0;
	
	int nThis = m_pDockBar->FindBar(this);
	int nFirst= GetFirstPos();
	int nLast=  GetLastPos();
	UINT m_nDockBarID = m_pDockBar->GetDlgCtrlID();
	int  nMaxlen=GetHiWid();
    int  nLen=0; 
	for (int i=nFirst;i <nLast;  i++)
	{
		
		CGuiToolBarWnd* pBar;
		pBar = (CGuiToolBarWnd*) m_pDockBar->m_arrBars[i];
		if (HIWORD(pBar) == 0) continue; 
		if (!pBar->IsVisible()) continue;
		
		CRect rc;
		pBar->GetWindowRect(rc);
		if (IsVert())
			nLen+=rc.Height() ;
		else
			nLen+= rc.Width() ;
		nVisibles++;
	}
	
	if ( nLen > nMaxlen)
	{
		int nDif=nLen-nMaxlen;
		

	}
	


/*	if (rcThisBar.Width() > nAfterThis)
	{
		if (nVisibles > 0)
			nAfterThis/=nVisibles;
		

	}
*/		
}

#define CX_OVERLAP  0
//***************************************************************************
CSize  CGuiToolBarWnd::CalcSize(TBBUTTON* pData, int nCount)
{
	ASSERT(pData != NULL && nCount > 0);
	BOOL bDrow=FALSE;
	int  nNumDrow=0;
	CPoint cur(0,0);
	CSize sizeResult(0,0);
	CClientDC dc(this);
	int xSizeMin=32;
	if(m_dwStyle & CBRS_FLOATING )  
		SetSizes(m_sizeButton, m_sizeImage);
	if (bVertDocked)
	 ::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, 0, 0);
	else
	 ::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, 1, 0);
	//CFont* m_fontOld=dc.SelectObject(&m_cfont);
//	SetRealSize();
	DWORD dwExtendedStyle = DefWindowProc(TB_GETEXTENDEDSTYLE, 0, 0);
	for (int i = 0; i < nCount; i++)
	{
		
		if ((pData[i].fsStyle &  TBSTYLE_SEP) &&(pData[i].idCommand!=0))
		{
		    if (bVertDocked)
			{
				 CWnd * pWnd =GetDlgItem(pData[i].idCommand);
				 ASSERT_VALID(pWnd);
				 pWnd->ShowWindow( SW_HIDE   );
				 pData[i].fsState |= TBSTATE_HIDDEN;
				 continue;
				
			}
			else
			{
				
					CWnd * pWnd =GetDlgItem(pData[i].idCommand);
					ASSERT_VALID(pWnd);
					pData[i].fsState &= ~TBSTATE_HIDDEN;
					pWnd->ShowWindow( SW_SHOW   );
					
								
			}
		}
		
		int cySep = pData[i].iBitmap;
		cySep = cySep * 2 / 3;
		CRect rci;
		GetItemRect(i, &rci);
		int cx=rci.Width();
		xSizeMin=min(xSizeMin,cx);
		if (pData[i].fsStyle & TBSTYLE_SEP)
		{
			// a separator represents either a height or width
			if (pData[i].fsState & TBSTATE_WRAP)
				sizeResult.cy = max(cur.y + m_sizeButton.cy + cySep, sizeResult.cy);
			else
				sizeResult.cx = max(cur.x + pData[i].iBitmap, sizeResult.cx);
		}
		else
		{
			// check for dropdown style, but only if the buttons are being drawn
			if ((pData[i].fsStyle & TBSTYLE_DROPDOWN) &&
				(dwExtendedStyle & TBSTYLE_EX_DRAWDDARROWS))
			{
				// add size of drop down
				cx += (_afxDropDownWidth);
				nNumDrow++;
				bDrow=TRUE;
			}
			sizeResult.cx = max(cur.x + cx, sizeResult.cx);
			sizeResult.cy = max(cur.y + m_sizeButton.cy, sizeResult.cy);
		}

			

		if (pData[i].fsStyle & TBSTYLE_SEP)
			cur.x += pData[i].iBitmap;
		else
			cur.x += cx - CX_OVERLAP;

		if (pData[i].fsState & TBSTATE_WRAP)
		{
			cur.x = 0;
			cur.y += m_sizeButton.cy;
			if (pData[i].fsStyle & TBSTYLE_SEP)
				cur.y += cySep;
		}
	}
	if (bDrow == TRUE)
	{
		
		if (bVertDocked)
		{
			sizeResult.cx-=_afxDropDownWidth;
			sizeResult.cy+=_afxDropDownWidth-2;//*nNumDrow;
		}
		else
		{
			sizeResult.cx-=_afxDropDownWidth*nNumDrow;
		}
	}
	else
	{
		if (bVertDocked)
			sizeResult.cy+=_afxDropDownWidth/2;
	}
	return sizeResult;
}

int  CGuiToolBarWnd::WrapToolBar(TBBUTTON* pData, int nCount, int nWidth)
{
	ASSERT(pData != NULL && nCount > 0);

	int nResult = 0;
	int x = 0;
	for (int i = 0; i < nCount; i++)
	{
		pData[i].fsState &= ~TBSTATE_WRAP;

		if (pData[i].fsState & TBSTATE_HIDDEN)
			continue;

		int dx, dxNext;
		if (pData[i].fsStyle & TBSTYLE_SEP)
		{
			dx = pData[i].iBitmap;
			dxNext = dx;
		}
		else
		{
			dx = m_sizeButton.cx;
			dxNext = dx - CX_OVERLAP;
		}

		if (x + dx > nWidth)
		{
			BOOL bFound = FALSE;
			for (int j = i; j >= 0  &&  !(pData[j].fsState & TBSTATE_WRAP); j--)
			{
				// Find last separator that isn't hidden
				// a separator that has a command ID is not
				// a separator, but a custom control.
				if ((pData[j].fsStyle & TBSTYLE_SEP) &&
					(pData[j].idCommand == 0) &&
					!(pData[j].fsState & TBSTATE_HIDDEN))
				{
					bFound = TRUE; i = j; x = 0;
					pData[j].fsState |= TBSTATE_WRAP;
					nResult++;
					break;
				}
			}
			if (!bFound)
			{
				for (int j = i - 1; j >= 0 && !(pData[j].fsState & TBSTATE_WRAP); j--)
				{
					// Never wrap anything that is hidden,
					// or any custom controls
					if ((pData[j].fsState & TBSTATE_HIDDEN) ||
						((pData[j].fsStyle & TBSTYLE_SEP) &&
						(pData[j].idCommand != 0)))
						continue;

					bFound = TRUE; i = j; x = 0;
					pData[j].fsState |= TBSTATE_WRAP;
					nResult++;
					break;
				}
				if (!bFound)
					x += dxNext;
			}
		}
		else
			x += dxNext;
	}
	return nResult + 1;
}

void   CGuiToolBarWnd::SizeToolBar(TBBUTTON* pData, int nCount, int nLength, BOOL bVert)
{
	ASSERT(pData != NULL && nCount > 0);
	bVert;
	if (!bVert)
	{
		int nMin, nMax, nTarget, nCurrent, nMid;

		// Wrap ToolBar as specified
		nMax = nLength;
		nTarget = WrapToolBar(pData, nCount, nMax);

		// Wrap ToolBar vertically
		nMin = 0;
		nCurrent = WrapToolBar(pData, nCount, nMin);

		if (nCurrent != nTarget)
		{
			while (nMin < nMax)
			{
				nMid = (nMin + nMax) / 2;
				nCurrent = WrapToolBar(pData, nCount, nMid);

				if (nCurrent == nTarget)
					nMax = nMid;
				else
				{
					if (nMin == nMid)
					{
						WrapToolBar(pData, nCount, nMax);
						break;
					}
					nMin = nMid;
				}
			}
		}
		CSize size = CalcSize(pData, nCount);
		WrapToolBar(pData, nCount, size.cx);
	}
	else
	{
		CSize sizeMax, sizeMin, sizeMid;

		// Wrap ToolBar vertically
		WrapToolBar(pData, nCount, 0);
		sizeMin = CalcSize(pData, nCount);

		// Wrap ToolBar horizontally
		WrapToolBar(pData, nCount, 32767);
		sizeMax = CalcSize(pData, nCount);

		while (sizeMin.cx < sizeMax.cx)
		{
			sizeMid.cx = (sizeMin.cx + sizeMax.cx) / 2;
			WrapToolBar(pData, nCount, sizeMid.cx);
			sizeMid = CalcSize(pData, nCount);

			if (nLength < sizeMid.cy)
			{
				if (sizeMin == sizeMid)
				{
					WrapToolBar(pData, nCount, sizeMax.cx);
					return;
				}
				sizeMin = sizeMid;
			}
			else if (nLength > sizeMid.cy)
				sizeMax = sizeMid;
			else
				return;
		}
	}
}

struct _AFX_CONTROLPOS
{
	int nIndex, nID;
	CRect rectOldPos;
};

CSize  CGuiToolBarWnd::CalcLayout(DWORD dwMode, int nLength)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	if (dwMode & LM_HORZDOCK)
		ASSERT(dwMode & LM_HORZ);
	
	if (dwMode & LM_HORZ)
		bVertDocked=FALSE;
	else
		bVertDocked=TRUE;
	int nCount;
	TBBUTTON* pData = NULL;
	CSize sizeResult(0,0);

	//BLOCK: Load Buttons
	{
		nCount = DefWindowProc(TB_BUTTONCOUNT, 0, 0);
		if (nCount != 0)
		{
			int i;
			pData = new TBBUTTON[nCount];
			for (i = 0; i < nCount; i++)
				_GetButton(i, &pData[i]);
		}
	}

	if (nCount > 0)
	{
		if (!(m_dwStyle & CBRS_SIZE_FIXED))
		{
			BOOL bDynamic = m_dwStyle & CBRS_SIZE_DYNAMIC;

			if (bDynamic && (dwMode & LM_MRUWIDTH))
				SizeToolBar(pData, nCount, m_nMRUWidth);
			else if (bDynamic && (dwMode & LM_HORZDOCK))
				SizeToolBar(pData, nCount, 32767);
			else if (bDynamic && (dwMode & LM_VERTDOCK))
				SizeToolBar(pData, nCount, 0);
			else if (bDynamic && (nLength != -1))
			{
				CRect rect; rect.SetRectEmpty();
				CalcInsideRect(rect, (dwMode & LM_HORZ));
				BOOL bVert = (dwMode & LM_LENGTHY);
				int nLen = nLength + (bVert ? rect.Height() : rect.Width());

				SizeToolBar(pData, nCount, nLen, bVert);
			}
			else if (bDynamic && (m_dwStyle & CBRS_FLOATING))
				SizeToolBar(pData, nCount, m_nMRUWidth);
			else
				SizeToolBar(pData, nCount, (dwMode & LM_HORZ) ? 32767 : 0);
		}

		sizeResult = CalcSize(pData, nCount);

		if (dwMode & LM_COMMIT)
		{
			_AFX_CONTROLPOS* pControl = NULL;
			int nControlCount = 0;
			BOOL bIsDelayed = m_bDelayedButtonLayout;
			m_bDelayedButtonLayout = FALSE;

			for (int i = 0; i < nCount; i++)
				if ((pData[i].fsStyle & TBSTYLE_SEP) && (pData[i].idCommand != 0))
					nControlCount++;

			if (nControlCount > 0)
			{
				pControl = new _AFX_CONTROLPOS[nControlCount];
				nControlCount = 0;

				for(int i = 0; i < nCount; i++)
				{
					if ((pData[i].fsStyle & TBSTYLE_SEP) && (pData[i].idCommand != 0))
					{
						pControl[nControlCount].nIndex = i;
						pControl[nControlCount].nID = pData[i].idCommand;

						CRect rect;
						GetItemRect(i, &rect);
						ClientToScreen(&rect);
						pControl[nControlCount].rectOldPos = rect;

						nControlCount++;
					}
				}
			}

			if ((m_dwStyle & CBRS_FLOATING) && (m_dwStyle & CBRS_SIZE_DYNAMIC))
				m_nMRUWidth = sizeResult.cx;
			for (i = 0; i < nCount; i++)
				_SetButton(i, &pData[i]);

			if (nControlCount > 0)
			{
				for (int i = 0; i < nControlCount; i++)
				{
					CWnd* pWnd = GetDlgItem(pControl[i].nID);
					if (pWnd != NULL)
					{
						CRect rect;
						pWnd->GetWindowRect(&rect);
						CPoint pt = rect.TopLeft() - pControl[i].rectOldPos.TopLeft();
						GetItemRect(pControl[i].nIndex, &rect);
						pt = rect.TopLeft() + pt;
						pWnd->SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
					}
				}
				delete[] pControl;
			}
			m_bDelayedButtonLayout = bIsDelayed;
		}
		delete[] pData;
	}

	//BLOCK: Adjust Margins
	{
		CRect rect; rect.SetRectEmpty();
		CalcInsideRect(rect, (dwMode & LM_HORZ));
		sizeResult.cy -= rect.Height();
		sizeResult.cx -= rect.Width();

		CSize size = CControlBar::CalcFixedLayout((dwMode & LM_STRETCH), (dwMode & LM_HORZ));
		sizeResult.cx = max(sizeResult.cx, size.cx);
		sizeResult.cy = max(sizeResult.cy, size.cy);
	}
	return sizeResult;
}

CSize  CGuiToolBarWnd::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	DWORD dwMode = bStretch ? LM_STRETCH : 0;
	dwMode |= bHorz ? LM_HORZ : 0;

	return CalcLayout(dwMode);
}

CSize  CGuiToolBarWnd::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	if ((nLength == -1) && !(dwMode & LM_MRUWIDTH) && !(dwMode & LM_COMMIT) &&
		((dwMode & LM_HORZDOCK) || (dwMode & LM_VERTDOCK)))
	{
		return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZDOCK);
	}
	return CalcLayout(dwMode, nLength);
}

void   CGuiToolBarWnd::_GetButton(int nIndex, TBBUTTON* pButton) const
{
	CGuiToolBarWnd* pBar = (CGuiToolBarWnd*)this;
	VERIFY(pBar->DefWindowProc(TB_GETBUTTON, nIndex, (LPARAM)pButton));
	// TBSTATE_ENABLED == TBBS_DISABLED so invert it
	pButton->fsState ^= TBSTATE_ENABLED;
}

void   CGuiToolBarWnd::_SetButton(int nIndex, TBBUTTON* pButton)
{
	// get original button state
	TBBUTTON button;
	VERIFY(DefWindowProc(TB_GETBUTTON, nIndex, (LPARAM)&button));

	// prepare for old/new button comparsion
	button.bReserved[0] = 0;
	button.bReserved[1] = 0;
	// TBSTATE_ENABLED == TBBS_DISABLED so invert it
	pButton->fsState ^= TBSTATE_ENABLED;
	pButton->bReserved[0] = 0;
	pButton->bReserved[1] = 0;

	// nothing to do if they are the same
	if (memcmp(pButton, &button, sizeof(TBBUTTON)) != 0)
	{
		// don't redraw everything while setting the button
		DWORD dwStyle = GetStyle();
		ModifyStyle(WS_VISIBLE, 0);
		VERIFY(DefWindowProc(TB_DELETEBUTTON, nIndex, 0));
		VERIFY(DefWindowProc(TB_INSERTBUTTON, nIndex, (LPARAM)pButton));
		ModifyStyle(0, dwStyle & WS_VISIBLE);

		// invalidate appropriate parts
		if (((pButton->fsStyle ^ button.fsStyle) & TBSTYLE_SEP) ||
			((pButton->fsStyle & TBSTYLE_SEP) && pButton->iBitmap != button.iBitmap))
		{
			// changing a separator
			Invalidate();
		}
		else
		{
			// invalidate just the button
			CRect rect;
			if (DefWindowProc(TB_GETITEMRECT, nIndex, (LPARAM)&rect))
				InvalidateRect(rect);
		}
	}
}


BOOL CGuiToolBarWnd::IsLeft()
{
	if (nDockBarAling == AFX_IDW_DOCKBAR_LEFT)	return TRUE;
	return FALSE;
}

BOOL CGuiToolBarWnd::IsRight()
{
	if (nDockBarAling == AFX_IDW_DOCKBAR_RIGHT)	return TRUE;
	return FALSE;
}

BOOL CGuiToolBarWnd::IsTop()
{
	if (nDockBarAling == AFX_IDW_DOCKBAR_TOP) return TRUE;
	return FALSE;
}

BOOL CGuiToolBarWnd::IsBottom()
{
	if (nDockBarAling == AFX_IDW_DOCKBAR_BOTTOM) return TRUE;
	return FALSE;
}

BOOL CGuiToolBarWnd::IsVert()
{
	if (IsLeft() || IsRight())	return TRUE;
	return FALSE;
}

BOOL CGuiToolBarWnd::IsHorz()
{
	if (IsTop() || IsBottom())	return TRUE;
	return FALSE;
}

BOOL CGuiToolBarWnd::IsFloating()
{
	if (nDockBarAling == AFX_IDW_DOCKBAR_FLOAT)	return TRUE;
	return FALSE;

}