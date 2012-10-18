// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProfUIS_Controls.h"
#include "MainDlg.h"

#include <ExtPopupMenuWnd.h>
#include <ExtPaintManager.h>
#include <exdlgbase.h>
//#include <ExtAnimEffect.h>
#include <ExtCmdManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_ICON_NOTIFY (WM_USER+11)
NOTIFYICONDATA g_NotifyIconData;
LPCTSTR g_sFriendlyProgramName = _T("Prof-UIS");

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CExtDlgBase
{
	bool m_bFirstShow;
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CExtButton	m_BtnOK;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CExtDlgBase(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	m_bFirstShow = true;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CExtDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDOK, m_BtnOK);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CExtDlgBase)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CExtResizableDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bDialogInitComplete = false;
	m_bShowIconInTray = false;
	m_bDialogWithCtxMenu = false;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CExtResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_CHECK_MENU_WITH_SHADOWS, m_CheckMenuWithShadows);
	DDX_Control(pDX, IDC_COMBO_MENU_OPEN_EFFECT, m_ComboMenuEffect);
	DDX_Control(pDX, IDC_CHECK_MENU_ANIM_EXPANDING, m_CheckAnimateMenuWhenExpanding);
	DDX_Control(pDX, IDC_BUTTON_COLOR2, m_btnColor2);
	DDX_Control(pDX, IDC_BUTTON_COLOR1, m_btnColor1);
	DDX_Control(pDX, IDC_BUTTON_WITH_MENU, m_btn2);
	DDX_Control(pDX, IDC_EDIT_SAMPLE, m_EditSample);
	DDX_Control(pDX, IDC_COMBO_EDITABLE, m_ComboEditable);
	DDX_Control(pDX, IDC_COMBO_UI_LOOK, m_ComboUiLook);
	DDX_Control(pDX, IDC_ENABLE_DIALOG_CONTEXT_MENU, m_CheckEnableContextMenu);
	DDX_Control(pDX, IDC_CHECK_SHOW_IN_TRAY, m_CheckShowIconInTray);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDOK, m_BtnOK);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainDlg, CExtResizableDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_WM_CONTEXTMENU()
	ON_CBN_SELENDOK(IDC_COMBO_UI_LOOK, OnSelendokComboUiLook)
	ON_COMMAND(IDC_CHECK_SHOW_IN_TRAY, OnCheckShowInTray)
	ON_COMMAND(IDC_ENABLE_DIALOG_CONTEXT_MENU, OnEnableDialogContextMenu)
	ON_COMMAND(IDC_LOOK_2K, OnLook2k)
	ON_UPDATE_COMMAND_UI(IDC_LOOK_2K, OnUpdateLook2k)
	ON_COMMAND(IDC_LOOK_XP, OnLookXp)
	ON_UPDATE_COMMAND_UI(IDC_LOOK_XP, OnUpdateLookXp)
	ON_BN_CLICKED(IDC_CHECK_MENU_ANIM_EXPANDING, OnCheckMenuAnimExpanding)
	ON_CBN_SELENDOK(IDC_COMBO_MENU_OPEN_EFFECT, OnSelendokComboMenuOpenEffect)
	ON_BN_CLICKED(IDC_CHECK_MENU_WITH_SHADOWS, OnCheckMenuWithShadows)
	ON_BN_CLICKED(IDC_CHECK_SHOW_IN_TRAY, OnCheckShowInTray)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_CHECK_SHOW_IN_TRAY, OnUpdateCheckShowInTray)
	ON_UPDATE_COMMAND_UI(IDC_ENABLE_DIALOG_CONTEXT_MENU, OnUpdateEnableDialogContextMenu)
	ON_MESSAGE(WM_ICON_NOTIFY,OnInconNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

LRESULT CMainDlg::OnInconNotify(WPARAM wParam, LPARAM lParam)
{
	if(WM_ICON_NOTIFY != g_NotifyIconData.uCallbackMessage)
		return 0;
UINT nShellNotifyCode = (UINT)lParam; // LOWORD(lParam);
	if( nShellNotifyCode != WM_RBUTTONUP
		&& nShellNotifyCode != WM_LBUTTONUP
		)
		return 0;
CPoint ptCursorPos;
	VERIFY( ::GetCursorPos( &ptCursorPos ) );
	ShowCtxMenu( ptCursorPos );
	return 1;
}

BOOL CMainDlg::OnInitDialog()
{
	CExtResizableDialog::OnInitDialog();

	VERIFY(
		g_CmdManager->ProfileWndAdd(
			(::AfxGetApp())->m_pszRegistryKey,
			GetSafeHwnd()
			)
		);

	VERIFY(
		g_CmdManager->UpdateFromToolBar(
			(::AfxGetApp())->m_pszRegistryKey,
			IDR_TOOLBAR1
			)
		);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, ID_APP_ABOUT /*IDM_ABOUTBOX*/, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

CMenu _menu;
	VERIFY(
		_menu.LoadMenu(IDR_MENU1)
		);
	m_btn2.m_menu.Attach(
		_menu.Detach()
		);
	m_btn2.SetIcon( GetIcon(TRUE) );

	AddAnchor( IDOK, __RDA_RT );
	AddAnchor( IDCANCEL, __RDA_RT );
	AddAnchor( IDC_COMBO_EDITABLE, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_EDIT_SAMPLE, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_STATIC_BEFORE_MENU, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_CHECK_MENU_ANIM_EXPANDING, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_COMBO_MENU_OPEN_EFFECT, __RDA_LT, __RDA_RT );
	
	
	EnableSaveRestore( "ExtResizableDialog", "MainDlg" );

	m_ComboUiLook.SetCurSel( 0 );
	m_ComboMenuEffect.SetCurSel( (int)CExtPopupMenuWnd::__AT_FADE );
	m_CheckMenuWithShadows.SetCheck( 1 );
	m_CheckEnableContextMenu.SetCheck( 1 );
	m_CheckAnimateMenuWhenExpanding.SetCheck( CExtPopupMenuWnd::g_bMenuExpandAnimation ? 1 : 0 );
	m_bDialogInitComplete = true;

	PostMessage(WM_COMMAND,IDC_LOOK_2K);


	memset( &g_NotifyIconData, 0, sizeof(NOTIFYICONDATA) );
	g_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	g_NotifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

	g_NotifyIconData.hWnd   = GetSafeHwnd();
	g_NotifyIconData.uCallbackMessage = WM_ICON_NOTIFY;

	g_NotifyIconData.hIcon  = m_hIcon;
		
	// tooltip is 64-1 chars max
	strcpy(g_NotifyIconData.szTip,g_sFriendlyProgramName);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CExtResizableDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CExtResizableDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CExtDlgBase::OnInitDialog();

	EnableResizing( false );
	EnableSaveRestore( "ExtResizableDialog", "AboutDlg" );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CExtDlgBase::OnShowWindow(bShow, nStatus);
	
	if( !bShow )
		return;
	if( !m_bFirstShow )
		return;
	m_bFirstShow = false;
//ExtAnimEffect effect;
//CRect r;
//	GetWindowRect( &r );
//	effect.Open(  &r );
}

void CAboutDlg::OnOK() 
{
//ExtAnimEffect effect;
//CRect r;
//	GetWindowRect( &r );
//	effect.Close(  &r );
	
	CExtDlgBase::OnOK();
}

void CAboutDlg::OnCancel() 
{
//ExtAnimEffect effect;
//CRect r;
//	GetWindowRect( &r );
//	effect.Close(  &r );
	
	CExtDlgBase::OnCancel();
}

void CMainDlg::OnAppAbout() 
{
CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CMainDlg::ShowCtxMenu(CPoint ptScreen)
{
	if( m_menu.GetSafeHmenu() == NULL )
	{
		if( !m_menu.LoadMenu(IDR_MENU1) )
		{
			ASSERT( FALSE );
			return;
		}
	}
CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
	pPopup->UpdateFromMenu(
		GetSafeHwnd(),
		&m_menu
		);
	pPopup->TrackPopupMenu(
		0,
		ptScreen.x,
		ptScreen.y,
		GetSafeHwnd()
		);
}

void CMainDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	pWnd;
	if( m_CheckEnableContextMenu.GetCheck() == 0 )
		return;
	//ClientToScreen( &point );
	ShowCtxMenu( point );
}

void CMainDlg::OnSelendokComboUiLook() 
{
	if( !m_bDialogInitComplete )
		return;
int nCurSel = m_ComboUiLook.GetCurSel();
	if(nCurSel == 0)
	{
		VERIFY(
			g_PaintManager.InstallPaintManager(
				new CExtPaintManager
				)
			);
	}
	else
	{
		VERIFY(
			g_PaintManager.InstallPaintManager(
				new CExtPaintManagerXP
				)
			);
	}
	RedrawWindow( NULL, NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
			| RDW_ALLCHILDREN
		);
}

void CMainDlg::OnCheckShowInTray() 
{
	m_bShowIconInTray = !m_bShowIconInTray;
	m_CheckShowIconInTray.SetCheck(
		m_bShowIconInTray ? 1 : 0
		);
/*
BOOL bShell_NotifyIconRetVal =
		Shell_NotifyIcon(NIM_ADD,&g_NotifyIconData);
	ASSERT( bShell_NotifyIconRetVal );
	if( !bShell_NotifyIconRetVal )
		return FALSE;
*/
	if( m_bShowIconInTray )
	{
		VERIFY(
			Shell_NotifyIcon(
				NIM_ADD,
				&g_NotifyIconData
				)
			);
	}
	else
	{
		VERIFY(
			Shell_NotifyIcon(
				NIM_DELETE,
				&g_NotifyIconData
				)
			);
	}
}
void CMainDlg::OnUpdateCheckShowInTray(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck( m_bShowIconInTray ? TRUE : FALSE );
}

void CMainDlg::OnEnableDialogContextMenu() 
{
	m_bDialogWithCtxMenu = !m_bDialogWithCtxMenu;
	m_CheckEnableContextMenu.SetCheck(
		m_bDialogWithCtxMenu ? 1 : 0
		);
}
void CMainDlg::OnUpdateEnableDialogContextMenu(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck( m_bDialogWithCtxMenu ? TRUE : FALSE );
}


void CMainDlg::OnLook2k() 
{
	m_ComboUiLook.SetCurSel(0);
	OnSelendokComboUiLook();
}
void CMainDlg::OnUpdateLook2k(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio( (m_ComboUiLook.GetCurSel()==0) ? TRUE : FALSE );
}

void CMainDlg::OnLookXp() 
{
	m_ComboUiLook.SetCurSel(1);
	OnSelendokComboUiLook();
}
void CMainDlg::OnUpdateLookXp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio( (m_ComboUiLook.GetCurSel()==1) ? TRUE : FALSE );
}

void CMainDlg::OnCheckMenuAnimExpanding() 
{
	CExtPopupMenuWnd::g_bMenuExpandAnimation =
		m_CheckAnimateMenuWhenExpanding.GetCheck() ?
			true : false;
}

void CMainDlg::OnSelendokComboMenuOpenEffect() 
{
	if( !m_bDialogInitComplete )
		return;
int nCurSel = m_ComboMenuEffect.GetCurSel();
	CExtPopupMenuWnd::g_DefAnimationType =
		(CExtPopupMenuWnd::e_animation_type_t)nCurSel;

	
}

void CMainDlg::OnCheckMenuWithShadows() 
{
	CExtPopupMenuWnd::g_bMenuWithShadows =
		m_CheckMenuWithShadows.GetCheck() ?
			true : false;
}

void CMainDlg::OnDestroy() 
{
	if( m_bShowIconInTray )
	{
		VERIFY(
			Shell_NotifyIcon(
				NIM_DELETE,
				&g_NotifyIconData
				)
			);
	}	

	VERIFY(
		g_CmdManager->ProfileWndRemove(
			GetSafeHwnd()
			)
		);

	CExtResizableDialog::OnDestroy();
}
