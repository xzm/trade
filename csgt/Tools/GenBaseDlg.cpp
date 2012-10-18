// GenBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tools.h"
#include "GenBaseDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenBaseDlg dialog

void CGenBaseDlg::StoreProfile( )
{
	AfxGetApp()->WriteProfileString( "genbase", "srcpath", m_strSrcPath );
	AfxGetApp()->WriteProfileString( "genbase", "263srcfile", m_str263SrcFile );
	AfxGetApp()->WriteProfileString( "genbase", "destfile", m_strDestFile );
	AfxGetApp()->WriteProfileInt( "genbase", "newfile", m_bNewFile );
	AfxGetApp()->WriteProfileString( "genbase", "eratedollar", m_strErateDollar );
	AfxGetApp()->WriteProfileString( "genbase", "eratehkdollar", m_strErateHKDollar );
}

void CGenBaseDlg::LoadProfile( )
{
	m_strSrcPath	=	AfxGetApp()->GetProfileString( "genbase", "srcpath", m_strSrcPath );
	m_str263SrcFile	=	AfxGetApp()->GetProfileString( "genbase", "263srcfile", m_str263SrcFile );
	m_strDestFile	=	AfxGetApp()->GetProfileString( "genbase", "destfile", m_strDestFile );
	m_bNewFile	=	AfxGetApp()->GetProfileInt( "genbase", "newfile", m_bNewFile );
	m_strErateDollar	=	AfxGetApp()->GetProfileString( "genbase", "eratedollar", m_strErateDollar );
	m_strErateHKDollar	=	AfxGetApp()->GetProfileString( "genbase", "eratehkdollar", m_strErateHKDollar );
}


CGenBaseDlg::CGenBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGenBaseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenBaseDlg)
	m_strDestFile = _T("");
	m_strSrcPath = _T("");
	m_bNewFile = FALSE;
	m_tmDate = 0;
	m_str263SrcFile = _T("");
	m_strErateDollar = _T("");
	m_strErateHKDollar = _T("");
	//}}AFX_DATA_INIT

	m_strErateDollar = _T("8.277");
	m_strErateHKDollar = _T("1.061");

/*  巨灵信息2002，F10资料关键字 */
	m_strDomain_class	= _T("所属行业:");
	m_strNetasset_ps = _T("每股净资产");
	m_strNetasset_ps_regulate = _T("调整每股净资产");
	m_strEps = _T("每股收益");
	m_strEps_deduct = _T("扣除每股收益");
	m_strNet_profit = _T("净利润");
	m_strMain_profit= _T("主营业务利润 ");		// avoid to confilict with 主营业务利润率
	m_strTotal_profit	= _T("利润总额");
	m_strRatio_liquidity = _T("流动比率");
	m_strRatio_quick = _T("速动比率");
	m_strVelocity_receivables = _T("应收帐款周率");
	m_strVelocity_merchandise = _T("存货周转率");
	m_strMain_income = _T("主营业务收入");
	m_strCash_ps = _T("经营活动现金流量");
	m_strProfit_margin = _T("主营业务利润率");
	m_strNetasset_yield = _T("净资产收益率");
	m_strDate_begin		= _T("上市日期");
	m_strDate_begin2	= _T("上市时间");
	m_strShare_count_total = _T("总股本");
	m_strShare_count_a = _T("流通A股");
	m_strShare_count_b = _T("流通B股");
	m_strShare_count_h = _T("流通H股");
	m_strShare_count_national = _T("国家股");
	m_strShare_count_corp = _T("境内法人股");
	m_strProfit_psud = _T("每股未分利润");		// not support in 2002
	m_strAsset = _T("总资产(万元)");			//	avoid to conflict with总资产增长率
	m_strRatio_holderright = _T("股东权益比率");
	m_strRatio_longdebt = _T("长期负债率");		//	should convert to ratio
	m_strRatio_debt = _T("资产负债率");
	m_strProfit_inc = _T("主营利润增长率");		// not support in 2002
	m_strIncome_inc = _T("主营收入增长率");		// not support in 2002
	m_strAsset_inc = _T("总资产增长率");		// not support in 2002

	m_strFund_Netvalue	=	_T("单位净值");
	m_strFund_ShareCountTotal	=	_T("基金总份额");
	m_strFund_ShareCountCurrency	=	_T("基金流通份额");

	m_strRefresh	=	_T("【最新指标】");
	m_strReportAnn	=	_T("年报数据");
	m_strReportMid	=	_T("中报数据");
	m_strReportQuarter	=	_T("季报数据");

/*  巨灵信息2001，F10资料关键字
	m_strDomain_class	= _T("板块类别:");
	m_strNetasset_ps = _T("每股净资产");
	m_strNetasset_ps_regulate = _T("调整后净资产");
	m_strEps = _T("每股收益       (元)");
	m_strEps_deduct = _T("扣除非经常性损益\r\n后的每股收益");
	m_strNet_profit = _T("净利润      (万元)");
	m_strMain_profit= _T("主营业务利润(万元)");
	m_strTotal_profit	= _T("利润总额");
	m_strRatio_liquidity = _T("流动比率");
	m_strRatio_quick = _T("速动比率");
	m_strVelocity_receivables = _T("应收帐款周转率");
	m_strVelocity_merchandise = _T("存货周转率");
	m_strMain_income = _T("主营业务收入");
	m_strCash_ps = _T("经营现金净流量");
	m_strProfit_margin = _T("主营业务利润率");
	m_strNetasset_yield = _T("净资产收益率");
	m_strDate_begin		= _T("上市日期");
	m_strDate_begin2	= _T("上市时间");
	m_strShare_count_total = _T("●总股本");
	m_strShare_count_a = _T("流通A 股");
	m_strShare_count_b = _T("流通B 股");
	m_strShare_count_h = _T("流通H 股");
	m_strShare_count_national = _T("国有法人股");
	m_strShare_count_corp = _T("境内法人股");
	m_strProfit_psud = _T("每股未分利润");
	m_strAsset = _T("总资产  ");					//	avoid to conflict with总资产增长率
	m_strRatio_holderright = _T("股东权益比率");
	m_strRatio_longdebt = _T("长期负债");			//	should convert to ratio
	m_strRatio_debt = _T("资产负债比率");
	m_strProfit_inc = _T("主营利润增长率");
	m_strIncome_inc = _T("主营收入增长率");
	m_strAsset_inc = _T("总资产增长率");

	m_strFund_Netvalue	=	_T("最新单位资产净值");
	m_strFund_ShareCountTotal	=	_T("基金份数总额");
	m_strFund_ShareCountCurrency	=	_T("已流通份数");

	m_strRefresh	=	_T("更新提示");
	m_strReportAnn	=	_T("年度报告");
	m_strReportMid	=	_T("中期报告");
	m_strReportQuarter	=	_T("季度报告");
*/

	CSPTime	tmNow	=	CSPTime::GetCurrentTime();
	int nYear	=	tmNow.GetYear();
	int	nMonth	=	6;
	if( tmNow.GetMonth() <= 6 )
	{
		nYear	--;
		nMonth	=	12;
	}
	m_tmDate	=	CSPTime( nYear, nMonth, 31, 0, 0, 0 );
}


void CGenBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenBaseDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_DESTFILE, m_strDestFile);
	DDX_Text(pDX, IDC_SRCPATH, m_strSrcPath);
	DDX_Check(pDX, IDC_NEWFILE, m_bNewFile);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME, m_tmDate);
	DDX_Text(pDX, IDC_263SRCFILE, m_str263SrcFile);
	DDX_Text(pDX, IDC_ERATE_DOLLAR, m_strErateDollar);
	DDX_Text(pDX, IDC_ERATE_HKDOLLAR, m_strErateHKDollar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenBaseDlg, CDialog)
	//{{AFX_MSG_MAP(CGenBaseDlg)
	ON_BN_CLICKED(IDC_EXPLORER_QLSRC, OnExplorerQlsrc)
	ON_BN_CLICKED(IDC_EXPLORER_DEST, OnExplorerDest)
	ON_BN_CLICKED(IDC_RADIO_ANN, OnRadioAnn)
	ON_BN_CLICKED(IDC_RADIO_MID, OnRadioMid)
	ON_BN_CLICKED(IDC_RADIO_QUARTER, OnRadioQuarter)
	ON_BN_CLICKED(IDC_EXPLORER_263SRC, OnExplorer263src)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenBaseDlg message handlers

BOOL CGenBaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadProfile( );
	UpdateData( FALSE );

	if( m_tmDate.GetMonth() > 7 )
		CheckRadioButton( IDC_RADIO_ANN, IDC_RADIO_QUARTER, IDC_RADIO_MID );
	else
		CheckRadioButton( IDC_RADIO_ANN, IDC_RADIO_QUARTER, IDC_RADIO_ANN );
	
	CheckRadioButton( IDC_RADIO_DEFAULT1, IDC_RADIO_DEFAULT3, IDC_RADIO_DEFAULT1 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGenBaseDlg::OnExplorerQlsrc() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	if( AfxDoSelectDirectory( m_strSrcPath ) )
	{
		UpdateData( FALSE );

		CNetDatabase	testdb;
		if( !testdb.SetRootPath( m_strSrcPath, IStStore::dbtypeSelfDB ) )
		{
			AfxMessageBox( IDS_ERROR_SELFDBPATH, MB_OK | MB_ICONINFORMATION );
		}
	}
}

void CGenBaseDlg::OnExplorer263src() 
{
	// TODO: Add your control notification handler code here
	UpdateData( );

	CKSFileDialog dlg (TRUE, NULL, NULL, OFN_EXPLORER,
		"263 Source files (*.dat)|*.dat|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		m_str263SrcFile	=	dlg.GetPathName();
		UpdateData( FALSE );
	}
}

void CGenBaseDlg::OnExplorerDest() 
{
	// TODO: Add your control notification handler code here
	UpdateData( );

	CKSFileDialog dlg (FALSE, NULL, NULL, OFN_EXPLORER,
		"Base files (*.bst)|*.bst|All files (*.*)|*.*||", NULL);
	if( IDOK == dlg.DoModal() )
	{
		m_strDestFile	=	dlg.GetPathName();
		UpdateData( FALSE );
	}
}

void CGenBaseDlg::OnOK() 
{
	// 对话框操作
	m_btnOK.EnableWindow( FALSE );
	UpdateData( );
	CButton	* pbtnAnn	= (CButton *)GetDlgItem( IDC_RADIO_ANN );
	CButton	* pbtnMid	= (CButton *)GetDlgItem( IDC_RADIO_MID );
	CButton	* pbtnQuarter	= (CButton *)GetDlgItem( IDC_RADIO_QUARTER );
	CButton	* pbtnQuarter3	= (CButton *)GetDlgItem( IDC_RADIO_QUARTER3 );
	DWORD	dwReportType	=	CStock::reportAnnals;
	if( pbtnMid->GetCheck() )
		dwReportType	=	CStock::reportMid;
	else if( pbtnQuarter->GetCheck() )
		dwReportType	=	CStock::reportQuarter;
	else if( pbtnQuarter3->GetCheck() )
		dwReportType	=	CStock::reportQuarter3;
	CButton	* pbtnDefault1	= (CButton *)GetDlgItem( IDC_RADIO_DEFAULT1 );
	CButton	* pbtnDefault3	= (CButton *)GetDlgItem( IDC_RADIO_DEFAULT3 );
	DWORD	dwDefault	=	CStock::reportQuarter;
	if( pbtnDefault3->GetCheck() )
		dwDefault	=	CStock::reportQuarter3;


	// 打开源文件库
	CNetDatabase	db;
	if( !db.SetRootPath( m_strSrcPath, IStStore::dbtypeSelfDB ) )
	{
		AfxMessageBox( IDS_ERROR_SELFDBPATH, MB_OK | MB_ICONINFORMATION );
		m_btnOK.EnableWindow( TRUE );
		return;
	}

	// 打开目标文件
	UINT	nOpenMode	=	CFile::modeCreate | CFile::modeWrite;
	if( !m_bNewFile )
		nOpenMode	|=	CFile::modeNoTruncate;
	CFile	fileDest;
	if( !fileDest.Open( m_strDestFile, nOpenMode ) )
	{
		AfxMessageBox( IDS_OPENFILE_FAILED, MB_OK | MB_ICONINFORMATION );
		m_btnOK.EnableWindow( TRUE );
		return;
	}
	if( !m_bNewFile )
		fileDest.SeekToEnd();

	// 等待对话框
	CWaitDlg	waitdlg;
	
	// 准备股票表
	CStockContainer container;
	int nLen = db.LoadCodetable( container );
	db.LoadBasetable( container );
	waitdlg.SetProgressRange( 0, container.GetSize() );
	waitdlg.SetProgress( 0 );

	// 计算市场平均收益率
	double	market_yield_average = 0.0, market_yield_d = 0.0;
	double	market_dayyield_average = 0.0, market_dayyield_d = 0.0;
	CKData	& kdayMain	=	AfxGetStockMain().GetKDataDay();
	int nDays = 3*STKLIB_DAYS_INONEYEAR;
	if( kdayMain.GetSize() <= nDays )
	{
		AfxMessageBox( IDS_GENBASE_NOSZZS, MB_OK | MB_ICONINFORMATION );
	}
	BOOL bmarket_yield_ok = CalculateYieldYear( kdayMain, &market_yield_average, &market_yield_d, nDays );
	BOOL bmarket_dayyield_ok = CalculateYieldDay( kdayMain, &market_dayyield_average, &market_dayyield_d, nDays );

	// 准备263板块数据
	int	nLen263	=	0;
	CMapStringToPtr	map;
	CFile	file263;
	if( file263.Open( m_str263SrcFile, CFile::modeRead ) )
	{
		int	nLen263	=	(int)(file263.GetLength() / sizeof(struct stockex263_t));
		map.InitHashTable( nLen263 + nLen263 + 10 );

		struct	stockex263_header_t	header;
		file263.Read( &header, sizeof(header) );
		while( TRUE )
		{
			struct	stockex263_t	* p = new struct stockex263_t;
			if( sizeof(struct stockex263_t) == file263.Read( p,sizeof(struct stockex263_t) ) )
			{
				char	buffer263[STKLIB_MAX_CODE+1];
				memset( buffer263, 0, sizeof(buffer263) );
				strncpy( buffer263, p->code, STKLIB_MAX_CODE );
				void	*	ptrTemp	=	NULL;
				if( !map.Lookup( buffer263, ptrTemp ) )
					map.SetAt( buffer263, p );
				else
					delete	p;
			}
			else
			{
				delete	p;
				break;
			}
		}
		file263.Close();
	}
	CString	strFund, strFundIndex;
	strFund.LoadString( IDS_DOMAINFUND );
	strFundIndex.LoadString( IDS_GENBASE_FUNDINDEX );

	// 保存对话框缺省数据
	StoreProfile( );

	// 准备字符串查找
	BoyerMoore domain_class( m_strDomain_class );
	BoyerMoore ratio_liquidity( m_strRatio_liquidity );
	BoyerMoore ratio_quick( m_strRatio_quick );
	BoyerMoore velocity_receivables( m_strVelocity_receivables );

	BoyerMoore velocity_merchandise( m_strVelocity_merchandise );
	BoyerMoore main_income( m_strMain_income );
	BoyerMoore cash_ps( m_strCash_ps );

	BoyerMoore profit_margin( m_strProfit_margin );
	BoyerMoore netasset_yield( m_strNetasset_yield );

	BoyerMoore date_begin( m_strDate_begin );
	BoyerMoore date_begin2( m_strDate_begin2 );
	BoyerMoore share_count_total( m_strShare_count_total );
	BoyerMoore share_count_a( m_strShare_count_a );
	BoyerMoore share_count_b( m_strShare_count_b );
	BoyerMoore share_count_h( m_strShare_count_h );
	BoyerMoore share_count_national( m_strShare_count_national );
	BoyerMoore share_count_corp( m_strShare_count_corp );
	BoyerMoore profit_psud( m_strProfit_psud );
	BoyerMoore asset( m_strAsset );
	BoyerMoore ratio_holderright( m_strRatio_holderright );
	BoyerMoore ratio_longdebt( m_strRatio_longdebt );
	BoyerMoore ratio_debt( m_strRatio_debt );

	BoyerMoore netasset_ps( m_strNetasset_ps );
	BoyerMoore netasset_ps_regulate( m_strNetasset_ps_regulate );
	BoyerMoore eps( m_strEps );
	BoyerMoore eps_deduct( m_strEps_deduct );
	BoyerMoore net_profit( m_strNet_profit );
	BoyerMoore main_profit( m_strMain_profit );
	BoyerMoore total_profit( m_strTotal_profit );

	BoyerMoore profit_inc( m_strProfit_inc );
	BoyerMoore income_inc( m_strIncome_inc );
	BoyerMoore asset_inc( m_strAsset_inc );

	BoyerMoore fund_netvalue( m_strFund_Netvalue );
	BoyerMoore fund_sharecounttotal( m_strFund_ShareCountTotal );
	BoyerMoore fund_sharecountcurrency( m_strFund_ShareCountCurrency );

	BoyerMoore refresh(m_strRefresh);
	BoyerMoore report_ann(m_strReportAnn);
	BoyerMoore report_mid(m_strReportMid);
	BoyerMoore report_quarter(m_strReportQuarter);

	int	nCountReportQuarter = 0;
	int	nCountReportMid = 0;

	// 开始生成结果数据
	int	nCount	=	0;
	for( int i=0; i<container.GetSize(); i++ )
	{
		waitdlg.SetProgress( i );
		if( waitdlg.WaitForCancel( ) )
			break;

		CStockInfo	info	=	container.ElementAt(i);

		if( CStock::typeshIndex != info.GetType() && CStock::typeshA != info.GetType() && CStock::typeshB != info.GetType()
			&& CStock::typeszIndex != info.GetType() && CStock::typeszA != info.GetType() && CStock::typeszB != info.GetType() )
			continue;
		
		CStock	stock;
		stock.SetStockInfo( &info );
		stock.SetDatabase( &db );
		stock.PrepareData( CStock::dataBasetext, 0, TRUE );
		char	*	pBaseText	=	stock.GetBaseTextPtr();
		int			nLength		=	stock.GetBaseTextLength();
		if( NULL == pBaseText || 0 == nLength )
			continue;

		BASEDATA	block;
		memset( &block, 0, sizeof(block) );

		block.m_dwMarket	=	info.GetMarket();
		strncpy( block.m_szCode, info.GetStockCode(), min(sizeof(block.m_szCode)-1, strlen(info.GetStockCode())) );
		
		// 板块
		BOOL	bFund	=	FALSE;
		void	*	pointer	=	NULL;
		if( map.Lookup( info.GetStockCode(), pointer ) )
		{
			struct stockex263_t	* p263	=	(struct stockex263_t *)pointer;
			strncpy( block.m_szDomain, p263->domain, min(sizeof(block.m_szDomain)-1,sizeof(p263->domain)));
			strncpy( block.m_szProvince, p263->province, min(sizeof(block.m_szProvince)-1,sizeof(p263->province)));
		}
		if( info.IsFund() )
		{
			strncpy( block.m_szDomain, strFund, min(sizeof(block.m_szDomain)-1,strFund.GetLength()));
			bFund	=	TRUE;
		}
		{
			char	bufferDomain[64];
			memset( bufferDomain, 0, sizeof(bufferDomain) );
			int rlen = domain_class.GetPhraseAfter( pBaseText, nLength, 0, bufferDomain, sizeof(bufferDomain)-1 );
			if( rlen > 0 )
			{
				CString	strDomain	=	bufferDomain;
				strDomain.TrimLeft();
				strDomain.TrimRight();
				if( strDomain.GetLength() < sizeof(block.m_szDomain) )
					strcpy( block.m_szDomain, strDomain );
			}
		}

		// 其他技术指标

		block.m_date	=	m_tmDate.GetYear() * 10000 + m_tmDate.GetMonth() * 100 + m_tmDate.GetDay();
		block.m_time	=	m_tmDate.GetTime();
		block.m_reporttype	=	dwReportType;
		size_t	nRefreshPos	=	refresh.FindIn(pBaseText, nLength, 0 );
		if( nRefreshPos >= 0 )
		{
			if( report_ann.FindIn(pBaseText, nLength, nRefreshPos ) < nRefreshPos+100 )
				block.m_reporttype	=	CStock::reportAnnals;
			else if( report_mid.FindIn(pBaseText, nLength, nRefreshPos ) < nRefreshPos+100 )
			{
				block.m_reporttype	=	CStock::reportMid;
				nCountReportMid ++;
			}
			else if( report_quarter.FindIn(pBaseText, nLength, nRefreshPos ) < nRefreshPos+100 )
			{
				block.m_reporttype	=	dwDefault;
				nCountReportQuarter ++;
			}
		}
		block.m_fErate_dollar	=	(float)( atof( m_strErateDollar ) );
		block.m_fErate_hkdollar	=	(float)( atof( m_strErateHKDollar ) );

		block.m_fRatio_liquidity	=	(float)(ratio_liquidity.GetNumberAfter( pBaseText, nLength, 0 ) );
		block.m_fRatio_quick		=	(float)(ratio_quick.GetNumberAfter( pBaseText, nLength, 0 ) );
		block.m_fVelocity_receivables	=	(float)(velocity_receivables.GetNumberAfter( pBaseText, nLength, 0 ) );

		block.m_fVelocity_merchandise	=	(float)(velocity_merchandise.GetNumberAfter( pBaseText, nLength, 0 ) );
		block.m_fMain_income		=	(float)main_income.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;
		float	tempCash			=	(float)(cash_ps.GetNumberAfter( pBaseText, nLength, 0 ) ) * 10000.;

		block.m_fProfit_margin		=	(float)(profit_margin.GetNumberAfter( pBaseText, nLength, 0 ) );
		block.m_fNetasset_yield		=	(float)(netasset_yield.GetNumberAfter( pBaseText, nLength, 0 ) );

		block.m_datebegin		=	(LONG)date_begin.GetDateAfter( pBaseText, nLength, 0 );
		if( 0 == block.m_datebegin )
			block.m_datebegin	=	(LONG)date_begin2.GetDateAfter( pBaseText, nLength, 0 );
		block.m_fShare_count_total	=	(float)share_count_total.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;
		if( bFund )
			block.m_fShare_count_total	=	(float)fund_sharecounttotal.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;

		if( block.m_fShare_count_total > 1e-4 )
			block.m_fCash_ps		=	(float)( tempCash / block.m_fShare_count_total );
		block.m_fShare_count_a		=	(float)share_count_a.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;
		if( bFund )
			block.m_fShare_count_a		=	(float)fund_sharecountcurrency.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;

		block.m_fShare_count_b		=	(float)share_count_b.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;
		block.m_fShare_count_h		=	(float)share_count_h.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;
		block.m_fShare_count_national	=	(float)share_count_national.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;
		block.m_fShare_count_corp	=	(float)share_count_corp.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;
		block.m_fProfit_psud		=	(float)(profit_psud.GetNumberAfter( pBaseText, nLength, 0 ) );
		block.m_fAsset				=	(float)asset.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;
		block.m_fRatio_holderright	=	(float)(ratio_holderright.GetNumberAfter( pBaseText, nLength, 0 ) );
		block.m_fRatio_longdebt	=	(float)(ratio_longdebt.GetNumberAfter( pBaseText, nLength, 0 ) );
		if( block.m_fAsset > 1e-4 )
			block.m_fRatio_longdebt	=	(float)(block.m_fRatio_longdebt * 100. / block.m_fAsset);
		block.m_fRatio_debt		=	(float)(ratio_debt.GetNumberAfter( pBaseText, nLength, 0 ) );

		block.m_fNetasset_ps		=	(float)(netasset_ps.GetNumberAfter( pBaseText, nLength, 0 ) );
		if( bFund )
			block.m_fNetasset_ps	=	(float)(fund_netvalue.GetNumberAfter( pBaseText, nLength, 0 ) );
		block.m_fNetasset_ps_regulate	=	(float)(netasset_ps_regulate.GetNumberAfter( pBaseText, nLength, 0 ) );
		block.m_fEps				=	(float)(eps.GetNumberAfter( pBaseText, nLength, 0 ) );
		block.m_fEps_deduct		=	(LONG)(eps_deduct.GetNumberAfter( pBaseText, nLength, 0 ) );
		if( block.m_fEps_deduct > 2 * block.m_fEps )
			block.m_fEps_deduct	=	0;
		block.m_fNet_profit		=	(float)net_profit.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;
		block.m_fMain_profit	=	(float)main_profit.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;
		block.m_fTotal_profit		=	(LONG)total_profit.GetNumberAfter( pBaseText, nLength, 0 ) * 10000.;

		block.m_fProfit_inc		=	(LONG)(profit_inc.GetNumberAfter( pBaseText, nLength, 0 ) );
		block.m_fIncome_inc		=	(LONG)(income_inc.GetNumberAfter( pBaseText, nLength, 0 ) );
		block.m_fAsset_inc			=	(LONG)(asset_inc.GetNumberAfter( pBaseText, nLength, 0 ) );

		block.m_fYield_average		=	(float)STKLIB_DATA_INVALID;
		block.m_fYield_stddev		=	(float)STKLIB_DATA_INVALID;
		block.m_fBeite				=	(float)STKLIB_DATA_INVALID;
		AfxPrepareStockData(&AfxGetDB(),stock,CKData::ktypeDay,CKData::formatXDRup,CKData::mdtypeClose,TRUE,TRUE);
		CKData	& kday	=	stock.GetKDataDay();

		double	yield_average = 0., yield_d = 0., beite = 0. ;
		if( CalculateYieldYear( kday, &yield_average, &yield_d, nDays ) )
		{
			block.m_fYield_average	=	(float)( 100. * yield_average );
			block.m_fYield_stddev	=	(float)( 100. * yield_d );
			if( bmarket_yield_ok && CalculateBeiteYear( kday, kdayMain, market_yield_average, market_yield_d, &beite, nDays ) )
				block.m_fBeite		=	(float)( beite );
			else if( bmarket_dayyield_ok && CalculateBeiteDay( kday, kdayMain, market_dayyield_average, market_dayyield_d, &beite, nDays ) )
				block.m_fBeite		=	(float)( beite );
		}
		else if( CalculateYieldDay( kday, &yield_average, &yield_d, nDays ) )
		{
			block.m_fYield_average	=	(float)( 100. * (pow(1+yield_average,STKLIB_DAYS_INONEYEAR)-1) );
			block.m_fYield_stddev	=	(float)( 100. * sqrt((double)STKLIB_DAYS_INONEYEAR) * yield_d );
			if( bmarket_dayyield_ok && CalculateBeiteDay( kday, kdayMain, market_dayyield_average, market_dayyield_d, &beite, nDays ) )
				block.m_fBeite		=	(float)( beite );
		}

		fileDest.Write( &block, sizeof(block) );
		nCount	++;
	}

	// 关闭等待对话框和目标文件
	waitdlg.DestroyWindow();
	fileDest.Close();

	// 释放内存
	POSITION	pos263	=	map.GetStartPosition();
	while( pos263 )
	{
		CString	rKey;
		void	*	ptr;
		map.GetNextAssoc( pos263, rKey, ptr );
		if( ptr )
			delete	(struct stockex263_t *)ptr;
	}
	map.RemoveAll();

	// 汇报结果
	if( nCount == 0 )
		AfxMessageBox( IDS_GENBASE_FAILED, MB_OK | MB_ICONINFORMATION );
	else
	{
		CString	strReport, strReportFmt;
		strReportFmt.LoadString( IDS_GENBASE_REPORTFMT );
		strReport.Format( strReportFmt, nCount, nCountReportMid, nCountReportQuarter );
		AfxMessageBox( strReport, MB_OK | MB_ICONINFORMATION );
	}
	m_btnOK.EnableWindow( TRUE );
}

void CGenBaseDlg::OnRadioAnn() 
{
	// TODO: Add your control notification handler code here
	CButton	* pbtnAnn	= (CButton *)GetDlgItem( IDC_RADIO_ANN );
	if( pbtnAnn->GetCheck() )
	{
		UpdateData( );
		CSPTime	temp( m_tmDate.GetYear(), 12, 31, 0, 0, 0 );
		m_tmDate	=	temp;
		UpdateData( FALSE );
	}
}

void CGenBaseDlg::OnRadioMid() 
{
	// TODO: Add your control notification handler code here
	UpdateData( );

	CButton	* pbtnMid	= (CButton *)GetDlgItem( IDC_RADIO_MID );
	if( pbtnMid->GetCheck() )
	{
		CSPTime	temp( m_tmDate.GetYear(), 6, 30, 0, 0, 0 );
		m_tmDate	=	temp;
		UpdateData( FALSE );
	}
}

void CGenBaseDlg::OnRadioQuarter() 
{
	// TODO: Add your control notification handler code here
	UpdateData( );

	CButton	* pbtnQuarter	= (CButton *)GetDlgItem( IDC_RADIO_QUARTER );
	if( pbtnQuarter->GetCheck() )
	{
		CSPTime	temp( m_tmDate.GetYear(), 3, 31, 0, 0, 0 );
		m_tmDate	=	temp;
		UpdateData( FALSE );
	}
}

