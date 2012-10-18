// BaseRmrightDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tools.h"
#include "BaseRmrightDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseRmrightDlg dialog


CBaseRmrightDlg::CBaseRmrightDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CBaseRmrightDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBaseRmrightDlg)
	m_strSrcPath = _T("");
	m_strDestPath = _T("");
	//}}AFX_DATA_INIT
}

void CBaseRmrightDlg::StoreProfile( )
{
	AfxGetApp()->WriteProfileString( "basermright", "srcpath", m_strSrcPath );
	AfxGetApp()->WriteProfileString( "basermright", "destpath", m_strDestPath );
}

void CBaseRmrightDlg::LoadProfile( )
{
	m_strSrcPath	=	AfxGetApp()->GetProfileString( "basermright", "srcpath", m_strSrcPath );
	m_strDestPath	=	AfxGetApp()->GetProfileString( "basermright", "destpath", m_strDestPath );
}

void CBaseRmrightDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaseRmrightDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_SRCPATH, m_strSrcPath);
	DDX_Text(pDX, IDC_DESTPATH, m_strDestPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBaseRmrightDlg, CXFDialog)
	//{{AFX_MSG_MAP(CBaseRmrightDlg)
	ON_BN_CLICKED(IDC_EXPLORER_SRC, OnExplorerSrc)
	ON_BN_CLICKED(IDC_EXPLORER_DEST, OnExplorerDest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseRmrightDlg message handlers

BOOL CBaseRmrightDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadProfile( );
	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBaseRmrightDlg::OnExplorerSrc() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( AfxDoSelectDirectory( m_strSrcPath ) )
	{
		UpdateData( FALSE );
	}
}

void CBaseRmrightDlg::OnExplorerDest() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( AfxDoSelectDirectory( m_strDestPath ) )
	{
		UpdateData( FALSE );
	}
}

class CBaseString : public CString
{
public:
	int	ReplaceRange( LPCTSTR lpszBegin, LPCTSTR lpszEnd, LPCTSTR lpszNew, size_t nStart )
	{
		UINT	nCount	=	0;
		while( nStart >= 0 && nStart < (GetLength()-strlen(lpszBegin)-strlen(lpszEnd)) )
		{
			int	nIndexBegin	=	Find( lpszBegin, nStart );
			if( -1 == nIndexBegin )
				return nCount;
			int	nIndexEnd	=	Find( lpszEnd, nIndexBegin );
			if( -1 == nIndexEnd )
				return nCount;

			CString	strNew	=	Left( nIndexBegin );
			if( lpszNew && strlen(lpszNew) > 0)
				strNew	+=	lpszNew;
			strNew	+=	Mid( nIndexEnd );

			*(CString *)this	=	strNew;
			nStart	=	nIndexBegin;
			if( lpszNew && strlen(lpszNew) > 0)
				nStart	+=	strlen(lpszNew);
			nCount	++;
		}
		return nCount;
	}

	int	ReplaceAfter( LPCTSTR lpszBegin, LPCTSTR lpszNew, int nStart )
	{
		int	nIndexBegin	=	Find( lpszBegin, nStart );
		if( -1 == nIndexBegin )
			return 0;
		CString	strNew = Left( nIndexBegin );
		if( lpszNew && strlen(lpszNew)>0 )
			strNew	+=	lpszNew;
		*(CString *)this	=	strNew;
		return 1;
	}
};

void CBaseRmrightDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData( );

	if( m_strSrcPath.IsEmpty() || m_strDestPath.IsEmpty() || 0 != access(m_strSrcPath,0) )
	{
		AfxMessageBox( IDS_BASERMRIGHT_PATHERROR, MB_OK | MB_ICONINFORMATION );
		return;
	}

	// 保存对话框缺省数据
	StoreProfile( );

	// strFinder
	CString	strFinder	=	m_strSrcPath;
	if( strFinder.GetAt( strFinder.GetLength()-1 ) == '/' 
		|| strFinder.GetAt( strFinder.GetLength()-1 ) == '\\' )
		strFinder	=	strFinder.Left( strFinder.GetLength()-1 );
	strFinder	+=	STRING_DIRSEP;
	strFinder	+=	"*";

	// dest path
	if( m_strDestPath.GetAt( m_strDestPath.GetLength()-1 ) == '/' 
		|| m_strDestPath.GetAt( m_strDestPath.GetLength()-1 ) == '\\' )
		m_strDestPath	=	m_strDestPath.Left( m_strDestPath.GetLength()-1 );
	m_strDestPath	+=	STRING_DIRSEP;

	// 创建目标路径
	if( 0 != access(m_strDestPath,0) 
		&& !CreateDirectory( m_strDestPath, NULL ) )
	{
		AfxMessageBox( IDS_BASERMRIGHT_NODESTPATH, MB_OK | MB_ICONINFORMATION );
		return;
	}

	// 等待对话框
	CWaitDlg	waitdlg;
	int	nFileCount	=	CNetDatabase::GetFileCount( m_strSrcPath );
	waitdlg.SetProgressRange( 0, nFileCount );
	waitdlg.SetProgress( 0 );

	char ** paFiles	= new char * [nFileCount];
	for( int i=0; i<nFileCount && paFiles; i++ )
		paFiles[i]	=	NULL;

	// 开始
	int	nCount	=	0;
	CFileFind	finder;
	BOOL	bWorking	=	finder.FindFile( strFinder );
	while( bWorking )
	{
		waitdlg.SetProgress( nCount );
		if( waitdlg.WaitForCancel( ) )
			break;

		bWorking	=	finder.FindNextFile();
		DWORD	attr = GetFileAttributes( finder.GetFilePath() );
		if( 0xFFFFFFFF != attr && !(attr & FILE_ATTRIBUTE_DIRECTORY) )
		{
			CString	strSrcFile	=	finder.GetFilePath();
			CString	strDestFileName	=	finder.GetFileName();
			CString	strDestFile	=	m_strDestPath + strDestFileName;

			CFile	fileSrc, fileDest;
			if( 0 == strncmp( ".txt", strSrcFile.Right(4), 4 )
				&& fileSrc.Open( strSrcFile, CFile::modeRead ) )
			{
				if( fileDest.Open( strDestFile, CFile::modeCreate | CFile::modeWrite ) )
				{
					size_t	nLen	=	(size_t)fileSrc.GetLength();
					CBaseString	strText;
					if( nLen == fileSrc.Read(strText.GetBuffer(nLen+1),nLen) )
					{
						strText.ReleaseBuffer();

						// Replace Begin
						strText.ReplaceAfter( "`七、经营分析`", "", 0 );
						strText.Replace( "`", "★" );
						if( !strText.Replace( "★一、", "★" ) )
							strText.Replace( "一、", "★" );
						if( !strText.Replace( "★二、", "★" ) )
							strText.Replace( "二、", "★" );
						if( !strText.Replace( "★三、", "★" ) )
							strText.Replace( "三、", "★" );
						if( !strText.Replace( "★四、", "★" ) )
							strText.Replace( "四、", "★" );
						if( !strText.Replace( "★五、", "★" ) )
							strText.Replace( "五、", "★" );
						if( !strText.Replace( "★六、", "★" ) )
							strText.Replace( "六、", "★" );
						if( !strText.Replace( "★七、", "★" ) )
							strText.Replace( "七、", "★" );
						if( !strText.Replace( "★八、", "★" ) )
							strText.Replace( "八、", "★" );
						if( !strText.Replace( "★九、", "★" ) )
							strText.Replace( "九、", "★" );
						if( !strText.Replace( "★十、", "★" ) )
							strText.Replace( "十、", "★" );
						if( !strText.Replace( "★十一、", "★" ) )
							strText.Replace( "十一、", "★" );
						strText.ReplaceAfter( "★经营分析", "", 0 );
						strText.ReplaceAfter( "★募集资金投向", "", 0 );
						strText.ReplaceAfter( "★公司大事记", "", 0 );
						strText.ReplaceAfter( "★公司快讯", "", 0 );

						strText.ReplaceRange( " ", "【更新提示】", "                个股基本资料\r\n\r\n", 0 );
						strText.ReplaceRange( "【近日股评】", "【基本面分析】", "", 0 );
						strText.Replace( "【基本面分析】", "★基本指标★" );
						strText.Replace( "【更新提示】", "★最新提示★\r\n" );
						strText.Replace( "【最近分红】", "★最近分红★\r\n" );
						strText.Replace( "【最近配股】", "★最近配股★\r\n" );
						strText.Replace( "【", "  " );
						strText.Replace( "】", "  " );
						strText.Replace( "万得(Wind)资讯系统个股基本资料", "" );
						strText.Replace( "Wind客户服务：", "" );
						strText.ReplaceRange( "Tel:", "@wind.com.cn", "", 0 );
						strText.Replace( "@wind.com.cn", "" );
						strText.ReplaceAfter( "☆☆☆☆☆", "", 0 );
						if( -1 != strText.Find( "Wind" ) || -1 != strText.Find( "wind" ) )
							AfxMessageBox( "以下文件中含有Wind标记：" + strDestFileName, MB_OK | MB_ICONINFORMATION );
						// Replace End

						fileDest.Write( strText.GetBuffer(strText.GetLength()+1), strText.GetLength() );
						
						char	* pFileName = new char[strDestFileName.GetLength()+1];
						if( pFileName && nCount < nFileCount )
						{
							strncpy( pFileName, strDestFileName, strDestFileName.GetLength() );
							pFileName[strDestFileName.GetLength()]	=	0;
							paFiles[nCount]	=	pFileName;
						}
						nCount	++;
					}
					strText.ReleaseBuffer();
					fileDest.Close();
				}
				fileSrc.Close();
			}
		}
	}
	finder.Close();

	// Zip
	CInfoZip InfoZip;
	CString	sTempFile;

	BOOL	bZipOK	=	FALSE;
	if( InfoZip.InitializeZip() )
	{
		SetCurrentDirectory( m_strDestPath );
		bZipOK	=	InfoZip.AddFiles( m_strDestPath + "base.zip", paFiles, nCount );
		InfoZip.FinalizeZip();
	}
	for( i=0; i<nFileCount; i++ )
		if( paFiles[i] )	delete [] paFiles[i];
	delete [] paFiles;

	// 关闭等待对话框
	waitdlg.DestroyWindow();

	// 汇报结果
	CString	strReport, strReportFmt;
	strReportFmt.LoadString( IDS_BASERMRIGHT_REPORTFMT );
	strReport.Format( strReportFmt, nCount );
	if( bZipOK )
	{
		CString	strTemp;
		strTemp.LoadString( IDS_BASERMRIGHT_ZIPOK );
		strReport	+=	strTemp + m_strDestPath + "base.zip";
	}
	AfxMessageBox( strReport, MB_OK | MB_ICONINFORMATION );
}
