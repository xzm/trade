// Tools.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Tools.h"

#include "MainFrm.h"
#include "ToolsDoc.h"
#include "ToolsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolsApp

BEGIN_MESSAGE_MAP(CToolsApp, CWinApp)
	//{{AFX_MSG_MAP(CToolsApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolsApp construction

CToolsApp::CToolsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CToolsApp object

CToolsApp theApp;

char threadid[200];

/////////////////////////////////////////////////////////////////////////////
// CToolsApp initialization
TCHAR	szRegKeyCompanyOld[]	=	_T("Balang Software");
TCHAR	szRegKeyAppOld[]		=	_T("StockAna");

#ifdef	CLKLAN_ENGLISH_US
	TCHAR	szRegKeyCompany[]	=	_T("Ninebulls-Stock Software");
	TCHAR	szRegKeyApp[]		=	_T("Ninebulls China Stock");
#elif defined CLKVER_OEM
	TCHAR	szRegKeyCompany[]	=	OEM_REGKEY_COMPANY;
	TCHAR	szRegKeyApp[]		=	OEM_REGKEY_APP;
#else
	TCHAR	szRegKeyCompany[]	=	_T("Ninebulls-Stock Software");
	TCHAR	szRegKeyApp[]		=	_T("CLKing");
#endif

BOOL CToolsApp::InitInstance()
{
	AfxEnableControlContainer();

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey( szRegKeyCompany );
	BOOL bEnable = AfxEnableMemoryTracking(FALSE);
	if( m_pszProfileName )	free((void*)m_pszProfileName);
	m_pszProfileName = _tcsdup(AfxGetProfile().GetWorkDirectory()+m_pszExeName+".ini");
	AfxEnableMemoryTracking(bEnable);

	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	AfxGetProfile().LoadProfile( );
	::SetCurrentDirectory( AfxGetProfile().GetWorkDirectory() );
	AfxGetSView().Load( AfxGetProfile().GetSViewFile() );

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CToolsDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CToolsView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	AfxRegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if( CCommandLineInfo::AppUnregister == cmdInfo.m_nShellCommand )
	{
		AfxUnregisterShellFileTypes();
		if (!cmdInfo.m_bRunEmbedded)
			AfxMessageBox(AFX_IDP_UNREG_DONE, MB_OK | MB_ICONINFORMATION);
		if (m_pCmdInfo == NULL)
		{
			m_pCmdInfo = new CCommandLineInfo;
			m_pCmdInfo->m_nShellCommand = CCommandLineInfo::AppUnregister;
		}
		return FALSE;
	}

	// Show Splash Window
	CSplashDlg	splash;
	CBitmap		bmp;
	bmp.LoadBitmap(IDB_SPLASH);
	splash.SetBitmap((HBITMAP)bmp.GetSafeHandle());
	splash.Create(	NULL, 0, AfxGetSView().GetU(), AfxGetSView().GetS(),
					AfxGetVersionString(), FALSE );
	splash.ShowWindow( SW_SHOW );
	splash.UpdateWindow( );

	DWORD	dw	=	GetTickCount();
	DWORD	dw2	=	GetTickCount();
	if( dw2 - dw < 100 )
		Sleep( 500-(dw2-dw) );
	splash.DestroyWindow( );

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	// Load Last WindowPlacement
	WINDOWPLACEMENT wp;
	if (AfxGetProfile().GetWindowPlacement(&wp))
		m_pMainWnd->SetWindowPlacement(&wp);
	else
		m_pMainWnd->ShowWindow( SW_SHOW );
	
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CXFDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_staticVersion;
	CClickLink	m_linkHowtoReg;
	CStatic	m_staticSoftNO;
	CStatic	m_staticLeftDaysTitle;
	CHyperLink	m_linkGoPurchase;
	CStatic	m_staticLeftDays;
	CStatic	m_staticRegCode;
	CStatic	m_staticUser;
	CHyperLink	m_linkMailMe;
	CHyperLink	m_linkProductURL;
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CXFDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_VERSION, m_staticVersion);
	DDX_Control(pDX, IDC_HOWTO_REG, m_linkHowtoReg);
	DDX_Control(pDX, IDC_SOFTNO, m_staticSoftNO);
	DDX_Control(pDX, IDC_LEFTDAYSTITLE, m_staticLeftDaysTitle);
	DDX_Control(pDX, IDC_GOPURCHASE, m_linkGoPurchase);
	DDX_Control(pDX, IDC_LEFTDAYS, m_staticLeftDays);
	DDX_Control(pDX, IDC_REGCODE, m_staticRegCode);
	DDX_Control(pDX, IDC_USER, m_staticUser);
	DDX_Control(pDX, IDC_URL_MAILME, m_linkMailMe);
	DDX_Control(pDX, IDC_URL_ADDRESS, m_linkProductURL);
	//}}AFX_DATA_MAP
}

BOOL CAboutDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	CString	strMailMe	=	AfxGetProfile().GetSupportMailto();
	m_linkMailMe.SetURL( strMailMe );

	CString	strURL	=	AfxGetProfile().GetProductURL();
	m_linkProductURL.SetWindowText( strURL );
	m_linkProductURL.SetURL( strURL );

	CString	strVersion	=	AfxGetVersionString( );
	if( strVersion.GetLength() > 0 )
		m_staticVersion.SetWindowText( strVersion );

	CString	strUser	=	AfxGetSView().GetU();
	if( strUser.IsEmpty() )
		strUser.LoadString( IDS_AUTHORIZEDUSER_UNKNOWN );
	m_staticUser.SetWindowText( strUser );

	CString strSoftNO	=	AfxGetSView().GetS();
	if( !strSoftNO.IsEmpty() )
		m_staticSoftNO.SetWindowText( strSoftNO );
	
	CString	strRegCode	=	AfxGetSView().GetR();
	if( strRegCode.IsEmpty() )
		strRegCode.LoadString( IDS_REGCODE_UNPURCHASED );
	m_staticRegCode.SetWindowText( strRegCode );

	// evaluate version
	BOOL	bEvaluate	=	AfxGetSView().IsEva();
	if( bEvaluate )
	{
		CString	strLeftDays;
		if( AfxGetSView().GetLeft() > AfxGetSView().GetMaxEvaDays() )
			strLeftDays.LoadString( IDS_REGISTER_NOTIMELIMIT );
		else
			strLeftDays.Format( "%d", AfxGetSView().GetLeft() );
		m_staticLeftDays.SetWindowText( strLeftDays );
		m_linkGoPurchase.SetURL( AfxGetProfile().GetPurchaseURL() );
		m_linkHowtoReg.SetModeHelp( 0, "StockAna.chm::/htm/purchase.htm" );
	}
	else
	{
		m_staticLeftDaysTitle.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW );
		m_staticLeftDays.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW );
		m_linkGoPurchase.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW );
		m_linkHowtoReg.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW );
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CXFDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CToolsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CToolsApp message handlers


int CToolsApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	AfxGetProfile().StoreProfile( );
	AfxGetSView().Store( AfxGetProfile().GetSViewFile() );
	
	return CWinApp::ExitInstance();
}
