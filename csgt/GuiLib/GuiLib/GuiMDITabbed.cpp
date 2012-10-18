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
#include "afxpriv.h"
#include "..\header\guimditabbed.h"


BEGIN_MESSAGE_MAP(CGuiMDITabbed, CGuiTabbed)
	  ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	  	ON_WM_TIMER()
END_MESSAGE_MAP()


LRESULT CGuiMDITabbed::OnSizeParent(WPARAM, LPARAM lParam)
{
   AFX_SIZEPARENTPARAMS* pParams = (AFX_SIZEPARENTPARAMS*)lParam;
   CRect rc=pParams->rect;
   pParams->rect.top += 24;	
   pParams->rect.left+=2;
   pParams->rect.right-=1;	
   pParams->rect.bottom-=1;		
   MoveWindow(rc.left, rc.top , rc.Width(), rc.Height(), true);
   CClientDC dc(this);
   GetClientRect(rc);
   dc.Draw3dRect(rc,GuiDrawLayer::GetRGBColorShadow(),GuiDrawLayer::GetRGBColorShadow());
   return 0;
}

void CGuiMDITabbed::OnTimer(UINT nIDEvent) 
{

	UpdateWindows();
}

CGuiMDITabbed::CGuiMDITabbed(void)
{
	pParentWindow = NULL;
}

CGuiMDITabbed::~CGuiMDITabbed(void)
{
}


void CGuiMDITabbed::OnScrollClose()
{
	CWnd* pWnd=GetNumWnd(GetCurtab());
	if (pWnd==NULL) return;
	pWnd->SendMessage(WM_CLOSE);
	UpdateWindows();
	if (GetCurtab()== 0)
		GetParentFrame()->RecalcLayout();
}

void CGuiMDITabbed::UpdateWindows()
{
	
	/* Modified by SunZhenyu
	SetTimer(1,200,NULL);
	*/
	KillTimer(1);

	CWnd* pWnd=CWnd::FromHandle(::GetTopWindow(GetParentFrame()->GetSafeHwnd()));	
	char ClassName[32];
	
	while (pWnd != NULL)
	{
		::GetClassName(pWnd->GetSafeHwnd(),ClassName,32);
		CString szClassName=ClassName;
		if(szClassName=="MDIClient")
			break;
		pWnd=pWnd->GetNextWindow();
	}
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
	//primero adicionar los nuevos
	
	
	CWnd* pChild= CWnd::FromHandle(::GetTopWindow(pWnd->GetSafeHwnd()));	
	while(pChild != NULL)
	{
		CString m_Caption;
		pChild->GetWindowText(m_Caption);
		if (!m_Caption.IsEmpty()) 
		{
			if (!FindItem(pChild->GetSafeHwnd()))
				Addtab(pChild,m_Caption,-1);
			else
				UpdateCaption(pChild,m_Caption);
		}
		
		pChild= CWnd::FromHandle(::GetNextWindow(pChild->GetSafeHwnd(), GW_HWNDNEXT));
		
	}
	
	/* Modified by SunZhenyu */
	// get active child frame, modified 2003/6/19
	CMDIFrameWnd* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, GetParentFrame());
	CMDIChildWnd* pActiveChild = NULL;
	if( pFrame )
		pActiveChild	=	pFrame->MDIGetActive();
	// get active child frame, modified 2003/6/19

	
	//---eliminar las ventanas borradas
	int nNumtabs=GetCount();
	for (int i=0; i< nNumtabs; i++)
	{
			CWnd* pWnd=GetNumWnd(i);
			if (!::IsWindow(HWND(pWnd->GetSafeHwnd())))
			{
				DeleteTab(i);
				UpdateWindow();
				break;
			}
			/* Modified by SunZhenyu */
			// set cur tab by active child frame, modified 2003/6/19
			else if( pWnd->GetSafeHwnd() == pActiveChild->GetSafeHwnd() )
			{
				SetCurtab(i);
			}
			// set cur tab by active child frame, modified 2003/6/19
	}

	SetTimer(1,200,NULL);
}



