// GenCodetblDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GenCodetblDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenCodetblDlg dialog


CGenCodetblDlg::CGenCodetblDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CGenCodetblDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenCodetblDlg)
	m_strSrcPath = _T("");
	m_strDestPath = _T("");
	//}}AFX_DATA_INIT
}

void CGenCodetblDlg::StoreProfile( )
{
	AfxGetApp()->WriteProfileString( "gencodetbl", "srcpath", m_strSrcPath );
	AfxGetApp()->WriteProfileString( "gencodetbl", "destpath", m_strDestPath );
}

void CGenCodetblDlg::LoadProfile( )
{
	m_strSrcPath	=	AfxGetApp()->GetProfileString( "gencodetbl", "srcpath", m_strSrcPath );
	m_strDestPath	=	AfxGetApp()->GetProfileString( "gencodetbl", "destpath", m_strDestPath );
}

void CGenCodetblDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenCodetblDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_SRCPATH, m_strSrcPath);
	DDX_Text(pDX, IDC_DESTPATH, m_strDestPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenCodetblDlg, CXFDialog)
	//{{AFX_MSG_MAP(CGenCodetblDlg)
	ON_BN_CLICKED(IDC_EXPLORER_SRC, OnExplorerSrc)
	ON_BN_CLICKED(IDC_EXPLORER_DEST, OnExplorerDest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenCodetblDlg message handlers

BOOL CGenCodetblDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadProfile( );
	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGenCodetblDlg::OnExplorerSrc() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( AfxDoSelectDirectory( m_strSrcPath ) )
	{
		UpdateData( FALSE );
	}
}

void CGenCodetblDlg::OnExplorerDest() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( AfxDoSelectDirectory( m_strDestPath ) )
	{
		UpdateData( FALSE );
	}
}

void CGenCodetblDlg::OnOK() 
{
	CWaitCursor	waitcursor;

	// TODO: Add extra validation here
	UpdateData( );

	if( m_strSrcPath.IsEmpty() || m_strDestPath.IsEmpty() || 0 != access(m_strSrcPath,0) )
	{
		AfxMessageBox( IDS_PATHERROR, MB_OK | MB_ICONINFORMATION );
		return;
	}

	// 保存对话框缺省数据
	StoreProfile( );

	// sre path
	if( m_strSrcPath.GetAt( m_strSrcPath.GetLength()-1 ) == '/' 
		|| m_strSrcPath.GetAt( m_strSrcPath.GetLength()-1 ) == '\\' )
		m_strSrcPath	=	m_strSrcPath.Left( m_strSrcPath.GetLength()-1 );
	m_strSrcPath	+=	STRING_DIRSEP;

	// dest path
	if( m_strDestPath.GetAt( m_strDestPath.GetLength()-1 ) == '/' 
		|| m_strDestPath.GetAt( m_strDestPath.GetLength()-1 ) == '\\' )
		m_strDestPath	=	m_strDestPath.Left( m_strDestPath.GetLength()-1 );
	m_strDestPath	+=	STRING_DIRSEP;

	// 创建目标路径
	if( 0 != access(m_strDestPath,0) 
		&& !CreateDirectory( m_strDestPath, NULL ) )
	{
		AfxMessageBox( IDS_NODESTPATH, MB_OK | MB_ICONINFORMATION );
		return;
	}

	// 打开源文件库
	CNetDatabase	db;
	if( !db.SetRootPath( m_strSrcPath, IStStore::dbtypeQianlong ) )
	{
		AfxMessageBox( IDS_ERROR_QIANLONGDBPATH, MB_OK | MB_ICONINFORMATION );
		return;
	}
	CStockContainer	container;
	db.LoadCodetable( container );
	db.LoadBasetable( container );

	CStDatabase::CreateSelfDB( m_strDestPath );
	CNetDatabase	db2;
	if( !db2.SetRootPath( m_strDestPath, IStStore::dbtypeSelfDB ) )
	{
		AfxMessageBox( IDS_ERROR_OPENDESTFILE, MB_OK | MB_ICONINFORMATION );
		return;
	}
	db2.StoreCodetable( container );

	// 汇报结果
	CString	strReport, strReportFmt;
	strReportFmt.LoadString( IDS_GENCODETBL_REPORTFMT );
	strReport.Format( strReportFmt, container.GetSize() );
	AfxMessageBox( strReport, MB_OK | MB_ICONINFORMATION );
}
