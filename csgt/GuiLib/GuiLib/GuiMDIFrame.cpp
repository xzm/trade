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
#include "GuiMDIFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuiMDIFrame

IMPLEMENT_DYNCREATE(CGuiMDIFrame, CMDIFrameWnd)

CGuiMDIFrame::CGuiMDIFrame()
{
	
}

CGuiMDIFrame::~CGuiMDIFrame()
{
}


BEGIN_MESSAGE_MAP(CGuiMDIFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CGuiMDIFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//**************************************************************************

/*
*/

BOOL CGuiMDIFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	return CMDIFrameWnd::PreCreateWindow(cs);
}


BOOL CGuiMDIFrame::InitMDITabbed()
{
	if(::IsWindow(m_MdiTabbed.GetSafeHwnd())) return TRUE;
	if (!m_MdiTabbed.Create(WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,0x333))
		return FALSE;
	if (::IsWindow(m_wndMenuBar.GetSafeHwnd())) m_wndMenuBar.SetTabbed(TRUE);

	return TRUE;
}
//*************************************************************************
BOOL CGuiMDIFrame::PreCreateWindow(CREATESTRUCT& cs, UINT nIconID)
{
	cs.lpszClass = AfxRegisterWndClass( 0, NULL, NULL,
		AfxGetApp()->LoadIcon(nIconID));
	ASSERT(cs.lpszClass);

	return CMDIFrameWnd::PreCreateWindow(cs);
}
//*************************************************************************
void CGuiMDIFrame::ShowHideBar(CGuiControlBar* pBar)
{
	if (pBar->IsWindowVisible())
		ShowControlBar(pBar, FALSE, FALSE);
	else
		ShowControlBar(pBar, TRUE, FALSE);
}

void CGuiMDIFrame::ShowHideBar(CControlBar* pBar)
{
	if (pBar->IsWindowVisible())
		ShowControlBar(pBar, FALSE, FALSE);
	else
		ShowControlBar(pBar, TRUE, FALSE);
}
//*************************************************************************
void CGuiMDIFrame::EnableDocking(DWORD dwDockStyle)
{
	ASSERT((dwDockStyle & ~ (CBRS_ALIGN_ANY | CBRS_FLOAT_MULTI)) == 0);


	m_pFloatingFrameClass = RUNTIME_CLASS(CMiniDockFrameWnd);
	CGuiDocBarExten* pDock;
	DWORD dwStyle = WS_CHILD| WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	DWORD dwstyle;

	pDock = new CGuiDocBarExten();
	pDock->Create(this,dwStyle|CBRS_TOP, AFX_IDW_DOCKBAR_TOP);
	dwstyle = pDock->GetBarStyle();
	dwstyle &= ~CBRS_BORDER_ANY;
	pDock->SetBarStyle(dwstyle);

	pDock = new CGuiDocBarExten();
	pDock->Create(this, dwStyle|CBRS_BOTTOM, AFX_IDW_DOCKBAR_BOTTOM);
	dwstyle = pDock->GetBarStyle();
	dwstyle &= ~CBRS_BORDER_ANY;
	pDock->SetBarStyle(dwstyle);

	pDock = new CGuiDocBarExten();
	pDock->Create(this,dwStyle|CBRS_LEFT, AFX_IDW_DOCKBAR_LEFT);
	dwstyle = pDock->GetBarStyle();
	dwstyle &= ~CBRS_BORDER_ANY;
	pDock->SetBarStyle(dwstyle);

	pDock = new CGuiDocBarExten();
	pDock->Create(this,dwStyle|CBRS_RIGHT, AFX_IDW_DOCKBAR_RIGHT);
	dwstyle = pDock->GetBarStyle();
	dwstyle &= ~CBRS_BORDER_ANY;
	pDock->SetBarStyle(dwstyle);
//	DockSpecialBars();

}


DWORD CGuiMDIFrame::CanDock(CRect rect, DWORD dwDockStyle, CDockBar** ppDockBar)
{
	// dwDockStyle -- allowable styles of bar
	// don't allow to dock to floating unless multi is specified
	dwDockStyle &= CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI;

	if (ppDockBar != NULL)
		*ppDockBar = NULL;
	POSITION pos = m_listControlBars.GetHeadPosition();
	while (pos != NULL)
	{
		CDockBar* pDockBar = (CDockBar*)m_listControlBars.GetNext(pos);
		if (pDockBar->IsDockBar() && pDockBar->IsWindowVisible() &&
			(pDockBar->m_dwStyle & dwDockStyle & CBRS_ALIGN_ANY) &&
			(!pDockBar->m_bFloating ||
				(dwDockStyle & pDockBar->m_dwStyle & CBRS_FLOAT_MULTI)))
		{
			CRect rectBar;
			pDockBar->GetWindowRect(&rectBar);
			if (rectBar.Width() == 0)
				rectBar.right++;
			if (rectBar.Height() == 0)
				rectBar.bottom++;
			if (rectBar.IntersectRect(rectBar, rect))
			{
				if (ppDockBar != NULL)
					*ppDockBar = pDockBar;
				return pDockBar->m_dwStyle & dwDockStyle;
			}
		}
	}
	return 0;
}

void CGuiMDIFrame::DockSpecialBars()
{
	m_dockToolbarTop.Create(this,CBRS_ALIGN_BOTTOM);
	m_dockToolbarBottom.Create(this,CBRS_ALIGN_TOP);
	m_dockToolbarLeft.Create(this,CBRS_ALIGN_LEFT);
	m_dockToolbarRight.Create(this,CBRS_ALIGN_RIGHT);

/*	m_dockToolbarTop.EnableDocking(CBRS_ALIGN_TOP);
	m_dockToolbarBottom.EnableDocking(CBRS_ALIGN_BOTTOM);
	m_dockToolbarLeft.EnableDocking(CBRS_ALIGN_LEFT);
	m_dockToolbarRight.EnableDocking(CBRS_ALIGN_RIGHT);

	DockControlBar(&m_dockToolbarTop);
	DockControlBar(&m_dockToolbarBottom);
	DockControlBar(&m_dockToolbarLeft);
	DockControlBar(&m_dockToolbarRight);*/
}
/////////////////////////////////////////////////////////////////////////////
// CGuiMDIFrame message handlers
//***********************************************************************
BOOL CGuiMDIFrame::DestroyWindow()
{

	SaveBarState(sProfile);
	return CFrameWnd::DestroyWindow();
}

//***********************************************************************
BOOL CGuiMDIFrame::VerifyBarState(LPCTSTR lpszProfileName)
{
    CDockState state;
    state.LoadState(lpszProfileName);

    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
        ASSERT(pInfo != NULL);
        int nDockedCount = pInfo->m_arrBarID.GetSize();
        if (nDockedCount > 0)
        {
            // dockbar
            for (int j = 0; j < nDockedCount; j++)
            {
                UINT nID = (UINT) pInfo->m_arrBarID[j];
                if (nID == 0) continue; // row separator
                if (nID > 0xFFFF)
                    nID &= 0xFFFF; // placeholder - get the ID
                if (GetControlBar(nID) == NULL)
                    return FALSE;
            }
        }

        if (!pInfo->m_bFloating) // floating dockbars can be created later
            if (GetControlBar(pInfo->m_nBarID) == NULL)
                return FALSE; // invalid bar ID
    }

    return TRUE;
}


void CGuiMDIFrame::LoadBars()
{
	
	if (VerifyBarState(sProfile))
	{
		LoadBarState(sProfile);
	}
}
void CGuiMDIFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// TODO: Add your specialized code here and/or call the base class

	CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
	if(::IsWindow(m_MdiTabbed.GetSafeHwnd()))
		m_MdiTabbed.UpdateWindows();
}

int CGuiMDIFrame::InitMenu(UINT uIDMenu)
{
	if (!m_wndMenuBar.CreateEx(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_SIZE_DYNAMIC ) ||
		!m_wndMenuBar.LoadMenuBar(uIDMenu))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}	
	m_NewMenu.LoadMenu(uIDMenu);

	m_wndCool.Install(this);
	m_wndCool.LoadToolbar(uIDMenu);
	return 0;
}

//int CGuiMDIFrame::InitMenu(UINT uIDMenu)

int CGuiMDIFrame::InitStatusBar(UINT *indicators,int nSize)
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  nSize))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	return 0;
}

int CGuiMDIFrame::InitToolBar(UINT uIDMenu)
{

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),uIDMenu) ||
		!m_wndToolBar.LoadToolBar(uIDMenu))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	return 0;
}

