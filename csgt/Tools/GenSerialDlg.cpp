// GenSerialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tools.h"
#include "GenSerialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenSerialDlg dialog


CGenSerialDlg::CGenSerialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGenSerialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenSerialDlg)
	m_strUserName = _T("");
	m_strSoftNO = _T("");
	m_strRegCode = _T("");
	m_bProfessional = FALSE;
	//}}AFX_DATA_INIT
	m_bProfessional = TRUE;
}


void CGenSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenSerialDlg)
	DDX_Control(pDX, IDC_SAVEDISPATCH, m_btnSaveDispatch);
	DDX_Control(pDX, IDC_SAVEREG, m_btnSaveReg);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_SAVE, m_btnSave);
	DDX_Text(pDX, IDC_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_SOFTNO, m_strSoftNO);
	DDX_Text(pDX, IDC_REGCODE, m_strRegCode);
	DDX_Check(pDX, IDC_PROFESSIONAL, m_bProfessional);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenSerialDlg, CDialog)
	//{{AFX_MSG_MAP(CGenSerialDlg)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_SAVEREG, OnSavereg)
	ON_BN_CLICKED(IDC_SAVEDISPATCH, OnSavedispatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenSerialDlg message handlers

void CGenSerialDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_btnOK.EnableWindow( FALSE );
	UpdateData( TRUE );

	m_strUserName.TrimLeft();
	m_strUserName.TrimRight();
	m_strSoftNO.TrimLeft();
	m_strSoftNO.TrimRight();

	if( m_strUserName.GetLength() < SE_MIN_USERNAME
		|| m_strUserName.GetLength() > SE_MAX_USERNAME )
	{
		CString	strTooShort, strTooShortfmt;
		strTooShortfmt.LoadString( IDS_USERNAMETOOSHORTFMT );
		strTooShort.Format( strTooShortfmt, SE_MIN_USERNAME, SE_MAX_USERNAME );
		AfxMessageBox( strTooShort, MB_OK | MB_ICONINFORMATION );
		m_btnOK.EnableWindow( TRUE );
		return;
	}

	BOOL	bOK	=	FALSE;
	CSPString	strRegCode	=	(LPCTSTR)m_strRegCode;
	if( m_bProfessional )
		bOK	=	AfxGetSView().DumpProf( m_strUserName, m_strSoftNO, strRegCode );
	else
		bOK	=	AfxGetSView().DumpStd( m_strUserName, m_strSoftNO, strRegCode );
	m_strRegCode	=	(LPCTSTR)strRegCode;
	if( !bOK )
	{
		AfxMessageBox( IDS_BUILTREGCODE_FAILED, MB_OK | MB_ICONINFORMATION );
		m_btnOK.EnableWindow( TRUE );
		return;
	}

	UpdateData( FALSE );
	
	m_btnSave.EnableWindow( TRUE );
	m_btnSaveReg.EnableWindow( TRUE );
	m_btnOK.EnableWindow( TRUE );
}

BOOL CGenSerialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnSave.EnableWindow( FALSE );
	m_btnSaveReg.EnableWindow( FALSE );

	m_strSoftNO	=	CSView::GetS();

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

char	sz_userdbfile[]	=	"users.txt";
void CGenSerialDlg::OnSave( )
{
	// TODO: Add your control notification handler code here
	m_btnSave.EnableWindow( FALSE );
	UpdateData( TRUE );

	// User DB
	CFile	file;
	if( file.Open( sz_userdbfile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite ) )
	{
		file.SeekToEnd();
		CString	strVersion;
		if( m_bProfessional )
			strVersion.LoadString( IDS_GENSERIAL_PROF );
		else
			strVersion.LoadString( IDS_GENSERIAL_STD );
		CString	string	=	m_strUserName + "\t" + m_strSoftNO + "\t" + m_strRegCode + "\t" + strVersion + "\r\n";
		file.Write( string.GetBuffer(string.GetLength()+1), string.GetLength() );
		string.ReleaseBuffer();
		file.Close();
	}
	else
		m_btnSave.EnableWindow( TRUE );
}

void CGenSerialDlg::OnSavereg() 
{
	// TODO: Add your control notification handler code here
	m_btnSaveReg.EnableWindow( FALSE );
	UpdateData( TRUE );

	CKSFileDialog dlg (FALSE, NULL, NULL, OFN_EXPLORER,
		"Profile Data files (*.dat)|*.dat|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		CString	strFile	=	dlg.GetPathName();

		CSView	sec;
		sec.SetME( );
		sec.SetURDir( m_strUserName, m_strRegCode );

		HRSRC		hReSrc = NULL;
		HGLOBAL		hRead = NULL;
		LPCTSTR		lpText;
		HINSTANCE	hModule	=	AfxGetResourceHandle();
		hReSrc = ::FindResource( hModule, MAKEINTRESOURCE(IDR_TEXT_NSA), _T("Text") );
		if( hReSrc )
			hRead	=	::LoadResource( hModule, hReSrc );
		if( hRead )
		{
			lpText	=	(LPCTSTR)::LockResource( hRead );
			if( lpText )
			{
				sec.SetNSAText( lpText );
			}
			::UnlockResource( hRead );
		}

		if( !sec.Store( strFile ) )
			AfxMessageBox( IDS_STORESECRETFILE_FAILED, MB_OK | MB_ICONINFORMATION );
	}
	else
		m_btnSaveReg.EnableWindow( TRUE );
}

void CGenSerialDlg::OnSavedispatch() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	CKSFileDialog dlg (FALSE, NULL, NULL, OFN_EXPLORER,
		"Profile Data files (*.dat)|*.dat|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		CString	strFile	=	dlg.GetPathName();

		CSView	sec;
		sec.SetME( );

		HRSRC		hReSrc = NULL;
		HGLOBAL		hRead = NULL;
		LPCTSTR		lpText;
		HINSTANCE	hModule	=	AfxGetResourceHandle();
		hReSrc = ::FindResource( hModule, MAKEINTRESOURCE(IDR_TEXT_NSA), _T("Text") );
		if( hReSrc )
			hRead	=	::LoadResource( hModule, hReSrc );
		if( hRead )
		{
			lpText	=	(LPCTSTR)::LockResource( hRead );
			if( lpText )
			{
				sec.SetNSAText( lpText );
			}
			::UnlockResource( hRead );
		}

		if( !sec.Store( strFile ) )
			AfxMessageBox( IDS_STORESECRETFILE_FAILED, MB_OK | MB_ICONINFORMATION );
	}
}
