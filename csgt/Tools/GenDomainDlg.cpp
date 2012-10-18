// GenDomainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tools.h"
#include "GenDomainDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenDomainDlg dialog


CGenDomainDlg::CGenDomainDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CGenDomainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenDomainDlg)
	m_strSrcPath = _T("");
	m_strDestFile = _T("");
	//}}AFX_DATA_INIT
}

void CGenDomainDlg::StoreProfile( )
{
	AfxGetApp()->WriteProfileString( "gendomaindlg", "srcpath", m_strSrcPath );
	AfxGetApp()->WriteProfileString( "gendomaindlg", "destfile", m_strDestFile );
}

void CGenDomainDlg::LoadProfile( )
{
	m_strSrcPath	=	AfxGetApp()->GetProfileString( "gendomaindlg", "srcpath", m_strSrcPath );
	m_strDestFile	=	AfxGetApp()->GetProfileString( "gendomaindlg", "destfile", m_strDestFile );
}

void CGenDomainDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenDomainDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_SRCPATH, m_strSrcPath);
	DDX_Text(pDX, IDC_DESTFILE, m_strDestFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenDomainDlg, CXFDialog)
	//{{AFX_MSG_MAP(CGenDomainDlg)
	ON_BN_CLICKED(IDC_EXPLORER_SRC, OnExplorerSrc)
	ON_BN_CLICKED(IDC_EXPLORER_DEST, OnExplorerDest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenDomainDlg message handlers

BOOL CGenDomainDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadProfile( );
	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGenDomainDlg::OnExplorerSrc() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( AfxDoSelectDirectory( m_strSrcPath ) )
	{
		UpdateData( FALSE );
	}
}

void CGenDomainDlg::OnExplorerDest() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	CKSFileDialog dlg (FALSE, NULL, NULL, OFN_EXPLORER,
		"Base files (*.dat)|*.dat|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		m_strDestFile	=	dlg.GetPathName();
		UpdateData( FALSE );
	}
}

void CGenDomainDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( );

	if( m_strSrcPath.IsEmpty() || m_strDestFile.IsEmpty() || 0 != access(m_strSrcPath,0) )
	{
		AfxMessageBox( IDS_BASERMRIGHT_PATHERROR, MB_OK | MB_ICONINFORMATION );
		return;
	}

	// 保存对话框缺省数据
	StoreProfile( );

	CStdioFile	fileDest;
	if( !fileDest.Open( m_strDestFile, CFile::modeCreate | CFile::modeWrite ) )
	{
		AfxMessageBox( IDS_BASERMRIGHT_PATHERROR, MB_OK | MB_ICONINFORMATION );
		return;
	}

	// strFinder
	CString	strFinder	=	m_strSrcPath;
	if( strFinder.GetAt( strFinder.GetLength()-1 ) == '/' 
		|| strFinder.GetAt( strFinder.GetLength()-1 ) == '\\' )
		strFinder	=	strFinder.Left( strFinder.GetLength()-1 );
	strFinder	+=	STRING_DIRSEP;
	strFinder	+=	"*";

	// 开始
/*	CString	strCount;
	strCount.Format( "%d", nFileCount );
	fileDest.WriteString( strCount );
	fileDest.WriteString( STRING_CRLF );
*/
	int	nCount	=	0;
	CFileFind	finder;
	BOOL	bWorking	=	finder.FindFile( strFinder );
	while( bWorking )
	{
		bWorking	=	finder.FindNextFile();
		DWORD	attr = GetFileAttributes( finder.GetFilePath() );
		if( 0xFFFFFFFF != attr && !(attr & FILE_ATTRIBUTE_DIRECTORY) )
		{
			CStdioFile	fileSrc;
			if( fileSrc.Open( finder.GetFilePath(), CFile::modeRead ) )
			{
				CString	strDomainName	=	finder.GetFileName();
				int	nIndex	=	strDomainName.Find( "." );
				if( -1 != nIndex )
					strDomainName	=	strDomainName.Left( nIndex );

				BOOL	bDomainNameWritten	=	FALSE;

				CString	strStock;
				while( fileSrc.ReadString( strStock ) )
				{
					if( strStock.GetLength() == 4 && strStock[0] == '0')
						strStock	=	"00" + strStock;
					if( strStock.GetLength() == 4 )
						continue;
					if( strStock.GetLength() > 0 )
					{
						if( !bDomainNameWritten )
						{
							fileDest.WriteString( strDomainName );
							fileDest.WriteString( STRING_CRLF );
							bDomainNameWritten	=	TRUE;
						}
						fileDest.WriteString( strStock );
						fileDest.WriteString( STRING_CRLF );
					}
				}

				if( bDomainNameWritten )
					fileDest.WriteString( STRING_CRLF );
				
				fileSrc.Close();
				nCount	++;
			}
		}
	}
	finder.Close();
	fileDest.Close();

	// 汇报结果
	CString	strReport, strReportFmt;
	strReportFmt.LoadString( IDS_GENDOMAIN_REPORTFMT );
	strReport.Format( strReportFmt, nCount );
	AfxMessageBox( strReport, MB_OK | MB_ICONINFORMATION );
}
