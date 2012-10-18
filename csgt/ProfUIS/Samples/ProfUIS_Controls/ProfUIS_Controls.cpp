// ProfUIS_Controls.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ProfUIS_Controls.h"
#include "MainDlg.h"

#include <ExtCmdManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainApp

BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
	//{{AFX_MSG_MAP(CMainApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainApp construction

CMainApp::CMainApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMainApp object

CMainApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainApp initialization

BOOL CMainApp::InitInstance()
{
///	if( !C ,,, ::InitInstance() )
///		return FALSE;

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	SetRegistryKey( _T("Foss") );

	VERIFY(
		g_CmdManager->ProfileSetup(
			m_pszRegistryKey
			)
		);

	VERIFY(
		g_CmdManager->UpdateFromMenu(
			m_pszRegistryKey,
			IDR_MENU1
			)
		);

	static UINT basic_commands[] =
	{
		ID_APP_ABOUT,
		IDC_CHECK_SHOW_IN_TRAY,
		IDC_ENABLE_DIALOG_CONTEXT_MENU,

		0 // end of list
	};
	VERIFY(
		g_CmdManager->SetBasicCommands(
			m_pszRegistryKey,
			basic_commands
			)
		);
	
	CMainDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
