// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Tools.h"

#include "MainFrm.h"
#include "Resource.h"
#include "RegisterDlg.h"
#include "GenEvaDlg.h"
#include "GenSerialDlg.h"
#include "GenBaseDlg.h"
#include "GenDomainDlg.h"
#include "RegisterDlg.h"
#include "BaseRmrightDlg.h"
#include "FiletimeDlg.h"
#include "GenCodetblDlg.h"
#include "GenNameEnuDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_MAINBARTEXTBOT, OnViewMainbartextbot)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINBARTEXTBOT, OnUpdateViewMainbartextbot)
	ON_COMMAND(ID_VIEW_MAINBARTEXTRIGHT, OnViewMainbartextright)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINBARTEXTRIGHT, OnUpdateViewMainbartextright)
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateViewFullscreen)
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_TOOL_GENSERIAL, OnToolGenserial)
	ON_COMMAND(ID_HELP_REGISTER, OnHelpRegister)
	ON_COMMAND(ID_TOOL_OPENDBFILE, OnToolOpendbfile)
	ON_COMMAND(ID_TOOL_GENBASE, OnToolGenbase)
	ON_COMMAND(ID_TOOL_BASERMCR, OnToolBasermcr)
	ON_COMMAND(ID_TOOL_FILETIME, OnToolFiletime)
	ON_COMMAND(ID_TOOL_GENDOMAIN, OnToolGendomain)
	ON_COMMAND(ID_TOOL_GENCODETBL, OnToolGencodetbl)
	ON_COMMAND(ID_TOOL_GENNAMEENU, OnToolGennameenu)
	ON_COMMAND(ID_TOOL_GENEVA, OnToolGeneva)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TBN_DROPDOWN, IDW_MAINBAR, OnMainBarDropDown )
	ON_COMMAND_EX(IDW_MAINBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDW_MAINBAR, OnUpdateControlBarMenu)
	ON_MESSAGE(WM_USER_CHECKSN, CreateValid)
END_MESSAGE_MAP()

// toolbar buttons - IDs are command buttons
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // time indicator
	ID_SEPARATOR,           // stock indicator
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	memset( &m_fullScreen, 0, sizeof(m_fullScreen) );
	m_fullScreen.bFullScreen	=	FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create ToolBars
	if( !m_wndMainBar.CreateTextToolBar( this, IDW_MAINBAR, IDR_MAINBAR,
				-1, -1, CTextToolBar::labelBottom ) )
	{
		TRACE0("Failed to create mainbar\n");
		return -1;      // fail to create
	}
/*	for( int k=0; k<m_wndMainBar.GetCount(); k++ )
	{
		// Set DropDown Style and Message
		UINT nID, nStyle;	int iImage;
		m_wndMainBar.GetButtonInfo( k, nID, nStyle, iImage );
		if( ID_PRJ_SETTING == nID || ID_VIEW_WIZARD == nID
			|| ID_VIEW_MACD == nID || ID_VIEW_PDAY == nID )
			m_wndMainBar.SetButtonInfo( k, nID, TBSTYLE_DROPDOWN, iImage );
	}
*/

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Set Bar Title
	CString	strBarTitle;
	strBarTitle.LoadString( IDS_TITLE_MAINBAR );
	m_wndMainBar.SetWindowText(strBarTitle);
	m_wndMainBar.EnableDocking(CBRS_ALIGN_ANY);

	// Set Bar Position
	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndMainBar,AFX_IDW_DOCKBAR_TOP);
	
	// Set Status Bar Styles
	UINT	nID, nStyle;
	int		cxWidth;
	m_wndStatusBar.GetPaneInfo( 1, nID, nStyle, cxWidth );
	m_wndStatusBar.SetPaneInfo( 1, nID, nStyle, 130  );
	m_wndStatusBar.GetPaneInfo( 2, nID, nStyle, cxWidth );
	m_wndStatusBar.SetPaneInfo( 2, nID, nStyle, 130  );

	m_wndStatusBar.SetBorders( 15, 2, 1, 3 );
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight( 21 );
	
	HICON	icon = AfxGetApp()->LoadIcon( IDI_INDICATOR );
	m_wndStatusBar.GetStatusBarCtrl().SetIcon( 0,  icon );

	// Set Timer
	SetTimer( TIMER_TIME, 100, NULL );
	// SetTimer( TIMER_CHECKVALID, 100, NULL );

	// Load Bar State
	LoadBarState(_T("BarState"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.dwExStyle	&=	~WS_EX_CLIENTEDGE;

	return TRUE;
}

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

CPoint CMainFrame::CalcPopupMenuPoint( CToolBar * pBar, UINT nID, CMenu *pMenu )
{
	ASSERT( pBar && pBar->IsKindOf(RUNTIME_CLASS(CToolBar)) );

	// Button rect
	CRect rectButton;
	pBar->GetToolBarCtrl().GetRect( nID, &rectButton );
	pBar->ClientToScreen( &rectButton );

	// Screen size
	int cyScreen = GetSystemMetrics( SM_CYSCREEN );

	// Menu height
	int nMenuHeight	=	cyScreen/4;
	if( pMenu )
	{
		nMenuHeight	=	5;
		for( UINT i=0; i<pMenu->GetMenuItemCount(); i++ )
		{
			if( ID_SEPARATOR == pMenu->GetMenuItemID(i) )
				nMenuHeight += 9;
			else
				nMenuHeight += 19;
		}
	}

	// Left Width
	int nLeftWidth = -1;
	for( int i=0; i<pBar->GetCount(); i++ )
	{
		if( pBar->GetButtonStyle(i) & TBSTYLE_BUTTON )
		{
			CRect rectItem;
			pBar->GetItemRect( i, &rectItem );
			nLeftWidth = rectItem.Width();
			break;
		}
	}
	if( nLeftWidth < 0 )
		nLeftWidth = rectButton.Height();

	// Track Point
	CPoint	pt;
	if( cyScreen-rectButton.bottom-2 >= nMenuHeight
		|| rectButton.top-2 < nMenuHeight )
	{
		pt.x = rectButton.left + nLeftWidth;
		pt.y = rectButton.bottom + 1;
	}
	else
	{
		pt.x = rectButton.left + nLeftWidth;
		pt.y = rectButton.top - 1;
	}

	return pt;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( TIMER_TIME == nIDEvent )
	{
		CSPTime	time	=	CSPTime::GetCurrentTime();
		CString	string	=	time.Format( "%a,%b %d,%Y %H:%M:%S" );
		m_wndStatusBar.SetPaneText( 2, string );
	}
	else if( TIMER_CHECKVALID == nIDEvent )
	{
		static	unsigned	long	count	=	0;
		count	++;
		if( count == 2 )
		{
			KillTimer( TIMER_CHECKVALID );
			PostMessage( WM_USER_CHECKSN );
		}
	}

	CFrameWnd::OnTimer(nIDEvent);
}

LRESULT CMainFrame::CreateValid(WPARAM wParam, LPARAM lParam)
{
	if( wParam < 100 )
	{
		if( AfxGetSView().IsEva() )
		{
			CRegisterDlg	dlg;
			dlg.DoModal();
			PostMessage( WM_USER_CHECKSN, 250 );
		}
	}
	else if( wParam >= 100 && wParam < 200 )
	{
		PostMessage( lParam );
	}
	else
	{
		AfxAssertQuit();
	}
	return wParam;
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if( m_fullScreen.bFullScreen )
		OnViewFullscreen( );

	SaveBarState(_T("BarState"));
	
	WINDOWPLACEMENT wp;
	wp.length = sizeof wp;
	if (GetWindowPlacement(&wp))
	{
		wp.flags = 0;
		if (IsZoomed())
			wp.flags |= WPF_RESTORETOMAXIMIZED;
		// and write it to the .INI file
		AfxGetProfile().SetWindowPlacement(&wp);
	}

	CFrameWnd::OnClose();
}

void CMainFrame::OnMainBarDropDown(NMHDR* pNMHDR, LRESULT* pResult)
{
/*	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNMHDR;

	BOOL	bLoad	=	FALSE;
	CMenu menu;
	if( pNMToolBar->iItem == ID_PRJ_SETTING )
		bLoad	=	menu.LoadMenu( IDR_MENU_MAINBARSETTING );
	else if( pNMToolBar->iItem == ID_VIEW_WIZARD )
		bLoad	=	menu.LoadMenu( IDR_MENU_MAINBARVIEW );
	else if( pNMToolBar->iItem == ID_VIEW_MACD )
		bLoad	=	menu.LoadMenu( IDR_MENU_MAINBARTECH );
	else if( pNMToolBar->iItem == ID_VIEW_PDAY )
		bLoad	=	menu.LoadMenu( IDR_MENU_MAINBARPERIOD );
	else
		ASSERT( FALSE );

	if( bLoad )
	{
		CMenu* pPopupMenu = menu.GetSubMenu(0);
		if( pPopupMenu )
		{
			CPoint pt = CalcPopupMenuPoint( &m_wndMainBar, pNMToolBar->iItem, pPopupMenu );
			pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this );
		}
	}
*/
	*pResult = TBDDRET_DEFAULT;
}

void CMainFrame::OnViewMainbartextbot() { m_wndMainBar.ToggleTextLabel( CTextToolBar::labelBottom ); RecalcLayout(); }
void CMainFrame::OnUpdateViewMainbartextbot(CCmdUI* pCmdUI) { pCmdUI->SetCheck( CTextToolBar::labelBottom == m_wndMainBar.GetTextLabelStatus() ); }
void CMainFrame::OnViewMainbartextright()  { m_wndMainBar.ToggleTextLabel( CTextToolBar::labelRight ); RecalcLayout(); }
void CMainFrame::OnUpdateViewMainbartextright(CCmdUI* pCmdUI) { pCmdUI->SetCheck( CTextToolBar::labelRight == m_wndMainBar.GetTextLabelStatus() ); }

void CMainFrame::OnViewFullscreen() 
{
	// TODO: Add your command handler code here
	int nHeight	=	GetSystemMetrics( SM_CYMENU );
	if( m_fullScreen.bFullScreen )
	{
		m_fullScreen.bFullScreen	=	FALSE;

		if( m_fullScreen.bMaximized )
		{
			MoveWindow(	-GetSystemMetrics( SM_CXFRAME ),
						-GetSystemMetrics( SM_CYFRAME )-nHeight,
						GetSystemMetrics( SM_CXSCREEN ) + 2*GetSystemMetrics( SM_CXFRAME ),
						GetSystemMetrics( SM_CYSCREEN ) + 2*GetSystemMetrics( SM_CYFRAME ) + nHeight );
		}
		else
			PostMessage( WM_SYSCOMMAND, SC_RESTORE );

		if( m_fullScreen.bStatusBar && !m_wndStatusBar.IsWindowVisible() )
			OnBarCheck( AFX_IDW_STATUS_BAR );

		memset( &m_fullScreen, 0, sizeof(m_fullScreen) );
	}
	else
	{
		m_fullScreen.bFullScreen	=	TRUE;
		m_fullScreen.bMaximized	=	IsZoomed();

		if( m_wndStatusBar.IsWindowVisible() )
		{
			m_fullScreen.bStatusBar = 1;
			OnBarCheck( AFX_IDW_STATUS_BAR );
		}

		if( !m_fullScreen.bMaximized )
			PostMessage( WM_SYSCOMMAND, SC_MAXIMIZE );
		else
			MoveWindow(	-GetSystemMetrics( SM_CXFRAME ),
						-GetSystemMetrics( SM_CYFRAME )	- GetSystemMetrics( SM_CYCAPTION ) - nHeight,
						GetSystemMetrics( SM_CXSCREEN ) + 2*GetSystemMetrics( SM_CXFRAME ),
						GetSystemMetrics( SM_CYSCREEN ) + 2*GetSystemMetrics( SM_CYFRAME ) + GetSystemMetrics( SM_CYCAPTION ) + nHeight );
	}
}

void CMainFrame::OnUpdateViewFullscreen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_fullScreen.bFullScreen );
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFrameWnd::OnGetMinMaxInfo(lpMMI);

	if( m_fullScreen.bFullScreen )
	{
		int nHeight	=	GetSystemMetrics( SM_CYMENU );
		lpMMI->ptMaxPosition.y	=	-GetSystemMetrics( SM_CYFRAME ) - GetSystemMetrics( SM_CYCAPTION ) - nHeight;
		lpMMI->ptMaxTrackSize.y	=	lpMMI->ptMaxSize.y
								=	GetSystemMetrics( SM_CYSCREEN )
									+ 2*GetSystemMetrics( SM_CYFRAME )
									+ GetSystemMetrics( SM_CYCAPTION )
									+ nHeight;
	}
}

void CMainFrame::OnToolGenserial() 
{
	// TODO: Add your command handler code here
	CGenSerialDlg	dlg;
	dlg.DoModal();
}

void CMainFrame::OnToolGenbase() 
{
	// TODO: Add your command handler code here
	CGenBaseDlg	dlg;
	dlg.DoModal();
}

void CMainFrame::OnHelpRegister() 
{
	CRegisterDlg	dlg;
	dlg.DoModal();
}

extern	char	sz_userdbfile[];
void CMainFrame::OnToolOpendbfile() 
{
	// TODO: Add your command handler code here
	ShellExecute( NULL, "open", sz_userdbfile,
					NULL, AfxGetProfile().GetWorkDirectory(), SW_SHOW );
}

void CMainFrame::OnToolBasermcr() 
{
	// TODO: Add your command handler code here
	CBaseRmrightDlg	dlg;
	dlg.DoModal( );
}

void CMainFrame::OnToolFiletime() 
{
	// TODO: Add your command handler code here
	CFiletimeDlg	dlg;
	dlg.DoModal( );
}

void CMainFrame::OnToolGendomain() 
{
	// TODO: Add your command handler code here
	CGenDomainDlg	dlg;
	dlg.DoModal();
}

void CMainFrame::OnToolGencodetbl() 
{
	// TODO: Add your command handler code here
	CGenCodetblDlg	dlg;
	dlg.DoModal();
}

void CMainFrame::OnToolGennameenu() 
{
	// TODO: Add your command handler code here
	CGenNameEnuDlg	dlg;
	dlg.DoModal();
}

void CMainFrame::OnToolGeneva() 
{
	CGenEvaDlg	dlg;
	dlg.DoModal();
}
