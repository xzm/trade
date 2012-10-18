// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MDI.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_LIKE_OFFICE_2K, OnViewLikeOffice2k)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIKE_OFFICE_2K, OnUpdateViewLikeOffice2k)
	ON_COMMAND(ID_VIEW_LIKE_OFFICE_XP, OnViewLikeOfficeXp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIKE_OFFICE_XP, OnUpdateViewLikeOfficeXp)
	//}}AFX_MSG_MAP

	ON_COMMAND_EX(ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENUBAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_TOOLBAR2, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR2, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_0, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_0, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_1, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_1, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_2, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_2, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_3, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_3, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_4, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_4, OnUpdateControlBarMenu)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;
}

CMainFrame::~CMainFrame()
{
}

static UINT g_statBasicCommands[] =
{
	ID_APP_ABOUT,
	ID_APP_EXIT,
	ID_FILE_NEW,
	ID_VIEW_MENUBAR,
	ID_VIEW_TOOLBAR,
	IDR_TOOLBAR2,
	ID_VIEW_RESIZABLE_BAR_0,
	ID_VIEW_RESIZABLE_BAR_1,
	ID_VIEW_RESIZABLE_BAR_2,
	ID_VIEW_RESIZABLE_BAR_3,
	ID_VIEW_RESIZABLE_BAR_4,
	ID_VIEW_STATUS_BAR,
	ID_EDIT_COPY,
	ID_EDIT_CUT,
	ID_EDIT_PASTE,
	ID_WINDOW_CASCADE,
	ID_WINDOW_TILE_HORZ,
	0, // end of list
};

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	ASSERT( pApp->m_pszProfileName != NULL );
	g_CmdManager->ProfileSetup(
		pApp->m_pszProfileName,
		GetSafeHwnd()
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_MAINFRAME
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_MDITYPE
			)
		);

	m_wndMenuBar.SetUpdateFileMruList(FALSE);
	m_wndMenuBar.SetMdiWindowPopupName( _T("Window") );
	if(
		!m_wndMenuBar.Create(
			_T("menubar"),
			this,
			ID_VIEW_MENUBAR
			)

		||
		!m_wndMenuBar.LoadMenuBar(IDR_MAINFRAME)
		)
    {
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }

	if( !m_wndToolBar.Create(
			_T("ToolBar"),
			this,
			AFX_IDW_TOOLBAR
			)
		||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if( !m_wndToolBar2.Create(
			_T("ToolBar2"),
			this,
			IDR_TOOLBAR2
			)
		||
		!m_wndToolBar2.LoadToolBar(IDR_TOOLBAR2)
		)
	{
		TRACE0("Failed to create toolbar2\n");
		return -1;      // fail to create
	}

CExtCmdManager::cmd_t * p_cmd;

	p_cmd =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_VIEW_LIKE_OFFICE_2K
			);
	ASSERT( p_cmd != NULL );
	p_cmd->m_sToolbarText = _T("Show me 2000");

	p_cmd =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_VIEW_LIKE_OFFICE_XP
			);
	ASSERT( p_cmd != NULL );
	p_cmd->m_sToolbarText = _T("Toggle to modern XP");

	if(	!m_wndResizableBar0.Create(
			_T("Resizable Bar 0"),
			this,
			ID_VIEW_RESIZABLE_BAR_0
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar0\n");
		return -1;		// fail to create
	}
	if( !m_wndInBarEdit.Create(
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|ES_MULTILINE|ES_LEFT,
			CRect(0,0,0,0),
			&m_wndResizableBar0,
			m_wndResizableBar0.GetDlgCtrlID()
			)
		)
	{
		TRACE0("Failed to create m_wndInBarEdit\n");
		return -1;		// fail to create
	}
	m_wndInBarEdit.SetFont( &g_PaintManager->m_FontNormal );

	if(	!m_wndResizableBar1.Create(
			_T("Resizable Bar 1"),
			this,
			ID_VIEW_RESIZABLE_BAR_1
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar1\n");
		return -1;		// fail to create
	}
	if( !m_wndInBarListBox.Create(
			WS_CHILD|WS_VISIBLE
				|LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT
				|LBS_EXTENDEDSEL|LBS_MULTIPLESEL
				,
			CRect(0,0,0,0),
			&m_wndResizableBar1,
			m_wndResizableBar1.GetDlgCtrlID()
			)
		)
	{
		TRACE0("Failed to create m_wndInBarListBox\n");
		return -1;		// fail to create
	}
	m_wndInBarListBox.SetFont( &g_PaintManager->m_FontNormal );
	for( INT iLbStrNo = 0; iLbStrNo < 50; iLbStrNo++ )
	{
		CString s;
		s.Format( _T("String %02d"), iLbStrNo );
		m_wndInBarListBox.AddString( LPCTSTR(s) );
	}

	if(	!m_wndResizableBar2.Create(
			_T("Resizable Bar 2"),
			this,
			ID_VIEW_RESIZABLE_BAR_2
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar2\n");
		return -1;		// fail to create
	}
	if( !m_wndInBarColorPicker.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD|WS_VISIBLE,
			CRect(0,0,0,0),
			&m_wndResizableBar2,
			m_wndResizableBar2.GetDlgCtrlID()
			)
		)
	{
		TRACE0("Failed to create m_wndInBarColorPicker\n");
		return -1;		// fail to create
	}

	if(	!m_wndResizableBar3.Create(
			_T("Resizable Bar 3"),
			this,
			ID_VIEW_RESIZABLE_BAR_3
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar3\n");
		return -1;		// fail to create
	}
	if( !m_wndInBarDlg.Create(
			IDD_IN_BAR_DLG,
			&m_wndResizableBar3
			)
		)
	{
		TRACE0("Failed to create m_wndInBarDlg\n");
		return -1;		// fail to create
	}
	m_wndInBarDlg.ShowSizeGrip( FALSE );

	if(	!m_wndResizableBar4.Create(
			_T("Resizable Bar 4"),
			this,
			ID_VIEW_RESIZABLE_BAR_4
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar4\n");
		return -1;		// fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar2.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar0.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar2.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar3.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar4.EnableDocking(CBRS_ALIGN_ANY);

///	EnableDocking(CBRS_ALIGN_ANY);
	CExtControlBar::FrameEnableDocking(this);
	
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);

	RecalcLayout();
	CRect wrAlredyDockedBar;
	m_wndToolBar.GetWindowRect( &wrAlredyDockedBar );
	wrAlredyDockedBar.OffsetRect( 1, 0 );
	DockControlBar(&m_wndToolBar2,AFX_IDW_DOCKBAR_TOP,&wrAlredyDockedBar);

	DockControlBar(&m_wndResizableBar0,AFX_IDW_DOCKBAR_LEFT);
	
	RecalcLayout();
	m_wndResizableBar0.GetWindowRect( &wrAlredyDockedBar );
	wrAlredyDockedBar.OffsetRect( 0, 1 );
	DockControlBar(&m_wndResizableBar1,AFX_IDW_DOCKBAR_LEFT,&wrAlredyDockedBar);

	DockControlBar(&m_wndResizableBar2,AFX_IDW_DOCKBAR_BOTTOM);
	
	RecalcLayout();
	m_wndResizableBar2.GetWindowRect( &wrAlredyDockedBar );
	wrAlredyDockedBar.OffsetRect( 1, 0 );
	DockControlBar(&m_wndResizableBar3,AFX_IDW_DOCKBAR_BOTTOM,&wrAlredyDockedBar);

	RecalcLayout();
	m_wndResizableBar3.GetWindowRect( &wrAlredyDockedBar );
	wrAlredyDockedBar.OffsetRect( 1, 0 );
	DockControlBar(&m_wndResizableBar4,AFX_IDW_DOCKBAR_BOTTOM,&wrAlredyDockedBar);

	ShowControlBar(&m_wndResizableBar2,FALSE,FALSE);
	ShowControlBar(&m_wndResizableBar3,FALSE,FALSE);
	ShowControlBar(&m_wndResizableBar4,FALSE,FALSE);

	VERIFY(
		g_CmdManager->SetBasicCommands(
			pApp->m_pszProfileName,
			g_statBasicCommands
			)
		);

	CExtControlBar::ProfileBarStateLoad(
		this,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		&m_dataFrameWP
		);
	g_CmdManager->SerializeState(
		pApp->m_pszProfileName,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		false
		);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

BOOL CMainFrame::DestroyWindow() 
{
	CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	VERIFY(
		CExtControlBar::ProfileBarStateSave(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName
			)
		);
	VERIFY(
		g_CmdManager->SerializeState(
			pApp->m_pszProfileName,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			true
			)
		);
	g_CmdManager->ProfileWndRemove( GetSafeHwnd() );
	
	return CMDIFrameWnd::DestroyWindow();
}

void CMainFrame::OnViewLikeOffice2k() 
{
	if( !g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) ) )
		return;
	VERIFY(
		g_PaintManager.InstallPaintManager( new CExtPaintManager )
		);
	RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
			| RDW_FRAME | RDW_ALLCHILDREN
		);
}

void CMainFrame::OnUpdateViewLikeOffice2k(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio(
		g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) )
			? FALSE
			: TRUE
		);
}

void CMainFrame::OnViewLikeOfficeXp() 
{
	if( g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) ) )
		return;
	VERIFY(
		g_PaintManager.InstallPaintManager( new CExtPaintManagerXP )
		);
	RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
			| RDW_FRAME | RDW_ALLCHILDREN
		);
}

void CMainFrame::OnUpdateViewLikeOfficeXp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio(
		g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) )
			? TRUE
			: FALSE
		);
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// window placement persistence
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CFrameWnd::ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	
	CMDIFrameWnd::ActivateFrame(nCmdShow);
}
