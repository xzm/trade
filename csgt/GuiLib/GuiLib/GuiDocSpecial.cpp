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
#include "GuiDocSpecial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#define _AfxSetDlgCtrlID(hWnd, nID)     SetWindowLong(hWnd, GWL_ID, nID)
#define _AfxGetDlgCtrlID(hWnd)          ((UINT)(WORD)::GetDlgCtrlID(hWnd))

BEGIN_MESSAGE_MAP(CGuiDocSpecial,CDockBar)
 //{{AFX_MSG_MAP(CGuiDocSpecial)
 ON_WM_SIZE()
 ON_WM_CREATE()
 //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CGuiDocSpecial::CGuiDocSpecial()
{

}

CGuiDocSpecial::~CGuiDocSpecial()
{
	

}


CSize CGuiDocSpecial::CalcDynamicLayout(int nLength,DWORD nMode)
{
	CSize sz=CControlBar::CalcDynamicLayout(nLength,nMode);
//	sz.cx+=1;
//	sz.cy+=1;
	return sz;
}

int CGuiDocSpecial::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockBar::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}

void CGuiDocSpecial::OnNcPaint()
{
	CRect rcWindow;
	CRect rcClient;
	int m_iBorder=2; 
	CWindowDC	dc(this);
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);
	CBrush cbr;
	cbr.CreateSysColorBrush(GuiDrawLayer::GetRGBColorTabs());
	rcClient.OffsetRect(-rcWindow.TopLeft());
    rcWindow.OffsetRect(-rcWindow.TopLeft());
    ScreenToClient(rcWindow);
	rcClient.OffsetRect(-rcWindow.left,-rcWindow.top);
	dc.ExcludeClipRect(rcClient);   
	rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);
	int ibotton=rcWindow.bottom;
	rcWindow.top=rcWindow.bottom-m_iBorder;
	dc.FillRect(rcWindow,&cbr); 
	rcWindow.top=0;
	rcWindow.bottom+=3;
	dc.FillRect(rcWindow,&cbr); 
	//un truco primitivo que evita el efecto estar a un nivel encima de la otra barra
	dc.FillSolidRect(0,rcWindow.top,rcWindow.right+1,1,::GetSysColor(COLOR_BTNSHADOW)); 
	dc.FillSolidRect(0,rcWindow.top+1,rcWindow.right+1,1,::GetSysColor(COLOR_BTNHIGHLIGHT)); 

}


void CGuiDocSpecial::OnSize(UINT nType,int cx,int cy)
{
	CDockBar::OnSize(nType,cx,cy);
	Invalidate();
}


/*
void CGuiDocSpecial::RecalTabs()
{
	int inumTabs=GetDockedCount();
	if (inumTabs > 1)
	{

		for (int i=0; i< inumTabs; i++)
		{
			//GetNumWnd(int m_numtab)
			CString m_caption;
			CWnd* pParent=(CWnd*)m_arrBars[i];
			ASSERT(pParent);
			int j=0;
			BOOL bFound=FALSE;
			while(j< m_tabwnd.GetCount())
			{
				if (m_tabwnd.GetNumWnd(j) == pParent)
				{
					bFound=TRUE;
					break;
				}
				j++;
			}
			if (bFound == FALSE)
			{
				pParent->GetWindowText(m_caption);
				m_tabwnd.Addtab(pParent,m_caption,i);		
			}
		}
	}

}


*/