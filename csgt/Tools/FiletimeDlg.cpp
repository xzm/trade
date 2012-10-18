// FiletimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tools.h"
#include "FiletimeDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFiletimeDlg dialog


CFiletimeDlg::CFiletimeDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CFiletimeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFiletimeDlg)
	m_strSrcPath = _T("");
	m_tmCreate = 0;
	m_tmAccess = 0;
	m_tmWrite = 0;
	//}}AFX_DATA_INIT
}

void CFiletimeDlg::StoreProfile( )
{
	AfxGetApp()->WriteProfileString( "filetime", "srcpath", m_strSrcPath );
	if( -1 != m_tmCreate.GetTime() )
		AfxGetApp()->WriteProfileInt( "filetime", "createtime", m_tmCreate.GetTime() );
	if( -1 != m_tmAccess.GetTime() )
		AfxGetApp()->WriteProfileInt( "filetime", "accesstime", m_tmAccess.GetTime() );
	if( -1 != m_tmWrite.GetTime() )
		AfxGetApp()->WriteProfileInt( "filetime", "writetime", m_tmWrite.GetTime() );
}

void CFiletimeDlg::LoadProfile( )
{
	CTime	tmNow	=	CTime::GetCurrentTime();
	m_strSrcPath	=	AfxGetApp()->GetProfileString( "filetime", "srcpath", m_strSrcPath );
	m_tmCreate		=	CTime( AfxGetApp()->GetProfileInt( "filetime", "createtime", tmNow.GetTime() ) );
	m_tmAccess		=	CTime( AfxGetApp()->GetProfileInt( "filetime", "accesstime", tmNow.GetTime() ) );
	m_tmWrite		=	CTime( AfxGetApp()->GetProfileInt( "filetime", "writetime", tmNow.GetTime() ) );
}

void CFiletimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFiletimeDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_SRCPATH, m_strSrcPath);
	DDX_DateTimeCtrl(pDX, IDC_TIMECREATE, m_tmCreate);
	DDX_DateTimeCtrl(pDX, IDC_TIMEACCESS, m_tmAccess);
	DDX_DateTimeCtrl(pDX, IDC_TIMEWRITE, m_tmWrite);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFiletimeDlg, CXFDialog)
	//{{AFX_MSG_MAP(CFiletimeDlg)
	ON_BN_CLICKED(IDC_EXPLORER_SRC, OnExplorerSrc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFiletimeDlg message handlers

BOOL CFiletimeDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadProfile( );
	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFiletimeDlg::OnExplorerSrc() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( AfxDoSelectDirectory( m_strSrcPath ) )
	{
		UpdateData( FALSE );
	}
}

int SetFileTimeInPath( LPCTSTR lpszPath, CTime tmCreate, CTime tmAccess, CTime tmWrite, CWaitDlg *pwaitdlg, int &nCount )
{
	// strFinder
	if( NULL == lpszPath )
		return 0;

	CString	strFinder	=	lpszPath;
	if( strFinder.GetAt( strFinder.GetLength()-1 ) == '/' 
		|| strFinder.GetAt( strFinder.GetLength()-1 ) == '\\' )
		strFinder	=	strFinder.Left( strFinder.GetLength()-1 );
	strFinder	+=	STRING_DIRSEP;
	strFinder	+=	"*";

	CFileFind	finder;
	BOOL	bWorking	=	finder.FindFile( strFinder );
	while( bWorking )
	{
		if( pwaitdlg )
		{
			pwaitdlg->SetProgress( nCount );
			if( pwaitdlg->WaitForCancel( ) ) break;
		}

		bWorking	=	finder.FindNextFile();
		DWORD	attr = GetFileAttributes( finder.GetFilePath() );
		
		if( 0xFFFFFFFF != attr && (attr & FILE_ATTRIBUTE_DIRECTORY) )
		{
			CString	strName	=	finder.GetFileName();
			if( 0 != strName.CompareNoCase(".") && 0 != strName.CompareNoCase("..") )
				nCount	=	SetFileTimeInPath( finder.GetFilePath() + STRING_DIRSEP, tmCreate, tmAccess, tmWrite, pwaitdlg, nCount );
		}
		if( 0xFFFFFFFF != attr && !(attr & FILE_ATTRIBUTE_DIRECTORY) )
		{
			nCount	++;
			CFileStatus	status;
			if( CFile::GetStatus( finder.GetFilePath(), status ) )
			{
				status.m_ctime	=	tmCreate;
				status.m_atime	=	tmAccess;
				status.m_mtime	=	tmWrite;
				CFile::SetStatus( finder.GetFilePath(), status );
			}
		}
	}
	finder.Close();
	return nCount;
}

void CFiletimeDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( );

	if( m_strSrcPath.IsEmpty() || 0 != access(m_strSrcPath,0) )
	{
		AfxMessageBox( IDS_FILETIME_PATHERROR, MB_OK | MB_ICONINFORMATION );
		return;
	}

	// 保存对话框缺省数据
	StoreProfile( );


	// 等待对话框
	CWaitDlg	waitdlg;
	int	nFileCount	=	CNetDatabase::GetFileCount( m_strSrcPath );
	waitdlg.SetProgressRange( 0, nFileCount );
	waitdlg.SetProgress( 0 );


	// 开始
	int	nCount	=	0;
	nCount = SetFileTimeInPath( m_strSrcPath, m_tmCreate, m_tmAccess, m_tmWrite, &waitdlg, nCount );

	// 关闭等待对话框
	waitdlg.DestroyWindow();

	// 汇报结果
	CString	strReport, strReportFmt;
	strReportFmt.LoadString( IDS_FILETIME_REPORTFMT );
	strReport.Format( strReportFmt, nCount );
	AfxMessageBox( strReport, MB_OK | MB_ICONINFORMATION );
}

