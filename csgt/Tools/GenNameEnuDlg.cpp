// GenNameEnuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GenNameEnuDlg.h"
#include <io.h>
#include <ctype.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenNameEnuDlg dialog


CGenNameEnuDlg::CGenNameEnuDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CGenNameEnuDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenNameEnuDlg)
	m_strSrcPath = _T("");
	m_strDestPath = _T("");
	//}}AFX_DATA_INIT
}

void CGenNameEnuDlg::StoreProfile( )
{
	AfxGetApp()->WriteProfileString( "gennameenu", "srcpath", m_strSrcPath );
	AfxGetApp()->WriteProfileString( "gennameenu", "destpath", m_strDestPath );
}

void CGenNameEnuDlg::LoadProfile( )
{
	m_strSrcPath	=	AfxGetApp()->GetProfileString( "gennameenu", "srcpath", m_strSrcPath );
	m_strDestPath	=	AfxGetApp()->GetProfileString( "gennameenu", "destpath", m_strDestPath );
}

void CGenNameEnuDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenNameEnuDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_SRCPATH, m_strSrcPath);
	DDX_Text(pDX, IDC_DESTPATH, m_strDestPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenNameEnuDlg, CXFDialog)
	//{{AFX_MSG_MAP(CGenNameEnuDlg)
	ON_BN_CLICKED(IDC_EXPLORER_SRC, OnExplorerSrc)
	ON_BN_CLICKED(IDC_EXPLORER_DEST, OnExplorerDest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenNameEnuDlg message handlers

BOOL CGenNameEnuDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadProfile( );
	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGenNameEnuDlg::OnExplorerSrc() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( AfxDoSelectDirectory( m_strSrcPath ) )
	{
		UpdateData( FALSE );
	}
}

void CGenNameEnuDlg::OnExplorerDest() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( AfxDoSelectDirectory( m_strDestPath ) )
	{
		UpdateData( FALSE );
	}
}

void ResolveNameEnu( CString & strNameEnu )
{
	strNameEnu.TrimLeft();
	strNameEnu.TrimRight();
	strNameEnu.MakeLower();
	strNameEnu.Replace( "＆", "&" );
	strNameEnu.Replace( "（", "(" );
	strNameEnu.Replace( "）", ")" );
	strNameEnu.Replace( "，", "," );
	strNameEnu.Replace( "。", "." );
	strNameEnu.Replace( "．", "." );
	strNameEnu.Replace( "　", " " );

	strNameEnu.Replace( ", ", "," );
	strNameEnu.Replace( " ,", "," );
	strNameEnu.Replace( "  ", " " );
	strNameEnu.Replace( "  ", " " );
	strNameEnu.Replace( "  ", " " );
	strNameEnu.Replace( "company ltd.", "Co.,Ltd." );
	strNameEnu.Replace( "company,ltd.", "Co.,Ltd." );
	strNameEnu.Replace( "company ltd.", "Co.,Ltd." );
	strNameEnu.Replace( "co.,ltd.", "co.,ltd" );
	strNameEnu.Replace( "co.ltd", "co.,ltd" );
	strNameEnu.Replace( "co.,ltd", "Co.,Ltd." );
	strNameEnu.Replace( "company limited", "Co.,Ltd." );
	strNameEnu.Replace( "limited company", "Co.,Ltd." );
	strNameEnu.Replace( "corporation limited", "Co.,Ltd." );
	strNameEnu.Replace( "corporation ltd", "Co.,Ltd." );
	
	for( int i=0; i<strNameEnu.GetLength(); i++ )
	{
		if( 0 == i )
			strNameEnu.SetAt( i, toupper(strNameEnu[i]) );
		if( i > 0 && ' ' == strNameEnu[i-1] )
		{
			strNameEnu.SetAt( i, toupper(strNameEnu[i]) );
		}
	}
}

void CGenNameEnuDlg::OnOK() 
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
	if( !db.SetRootPath( m_strSrcPath, IStStore::dbtypeSelfDB ) )
	{
		AfxMessageBox( IDS_ERROR_SELFDBPATH, MB_OK | MB_ICONINFORMATION );
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

	BoyerMoore name_enu( "英文名称:" );
	char	buffer[1024];

	CWaitDlg	waitdlg;
	waitdlg.SetProgressRange( 0, container.GetSize() );
	waitdlg.SetProgress( 0 );
	int nCount = 0;
	for( int i=0; i<container.GetSize(); i++ )
	{
		waitdlg.SetProgress( i );
		if( waitdlg.WaitForCancel( ) )
			break;

		CStockInfo & info = container.ElementAt(i);

		CStock	stock;
		stock.SetStockInfo( &info );
		stock.SetDatabase( &db );
		stock.PrepareData( CStock::dataBasetext, 0, TRUE );
		char	*	pBaseText	=	stock.GetBaseTextPtr();
		int			nLength		=	stock.GetBaseTextLength();
		if( pBaseText && nLength > 0 )
		{
			memset( buffer, 0, sizeof(buffer) );
			int rlen = name_enu.GetPhraseAfter( pBaseText, nLength, 0, buffer, sizeof(buffer)-1 );
			if( rlen > 0 )
			{
				CString	strNameEnu	=	buffer;
				ResolveNameEnu( strNameEnu );
				info.SetStockNameEnu( strNameEnu );
				nCount ++;
			}
		}
	}
	
	int nStoreSize = db2.StoreCodetable( container );
	
	waitdlg.DestroyWindow();

	// 汇报结果
	CString	strReport, strReportFmt;
	strReportFmt.LoadString( IDS_GENNAMEENU_REPORTFMT );
	strReport.Format( strReportFmt, nCount, nStoreSize );
	AfxMessageBox( strReport, MB_OK | MB_ICONINFORMATION );
}
