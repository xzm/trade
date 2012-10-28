// SListView.cpp : implementation of the CSListView class
//

#include "stdafx.h"

#include "../StaticDoc.h"
#include "SListView.h"
#include "../Dialog/ListExportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSListView

#define	SLV_TIMER_REFRESH		1

#define	IDC_SLISTVIEW_GRID		100
#define	IDC_SLISTVIEW_WORKTAB	101

#define	SL_WORKTAB_CLASS	0
#define	SL_WORKTAB_A		1
#define	SL_WORKTAB_SZNB		2
#define	SL_WORKTAB_SHB		3
#define	SL_WORKTAB_BOND		4
#define	SL_WORKTAB_FUND		5
#define	SL_WORKTAB_SELF		6
#define	SL_WORKTAB_GROUP	7
#define	SL_WORKTAB_DOMAIN	8


IMPLEMENT_DYNCREATE(CSListView, CFormView)

BEGIN_MESSAGE_MAP(CSListView, CFormView)
	//{{AFX_MSG_MAP(CSListView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_COMMAND(ID_SLIST_EXPORTLIST, OnSlistExportlist)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SLISTVIEW_WORKTAB, OnSelchangeWorktab)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_MESSAGE(WM_USER_GETVIEWTITLE, OnGetViewTitle)
	ON_MESSAGE(WM_USER_GETVIEWCMDID, OnGetViewCmdid)
	ON_MESSAGE(WM_USER_COLORCHANGE, OnColorChange)
	ON_MESSAGE(WM_APP_STKRECEIVER_DATA, OnStkReceiverData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSListView construction/destruction

CSListView::CSListView()
	: CFormView(CSListView::IDD)
{
	m_nColSort		=	-1;
	m_bSortAscend	=	FALSE;
	m_bShouldUpdate	=	FALSE;
	m_bFirstUpdate	=	TRUE;
}

CSListView::~CSListView()
{
}

void CSListView::SetFont( LPLOGFONT pLogFont )
{
	ASSERT( pLogFont );

	HFONT hFont = ::CreateFontIndirect(pLogFont);
	m_Grid.SendMessage( WM_SETFONT, (WPARAM)hFont, MAKELPARAM(1, 0) );
	m_Grid.AutoSize( );
	DeleteObject(hFont);
}

void CSListView::GetSelectedStocks( CSPStringArray & astr )
{
	CStockContainer & container = AfxGetSListStockContainer();
	container.Lock();

	int	nTotalCount	=	m_Grid.GetSelectedCount();
	astr.RemoveAll();
	astr.SetSize( 0, nTotalCount > 10 ? nTotalCount : -1 );
	for( int nRow=1; nRow<m_Grid.GetRowCount(); nRow++ )
	{
		BOOL	bSelected	=	FALSE;
		for( int nCol=0; nCol<m_Grid.GetColumnCount(); nCol ++ )
			bSelected	|=	( m_Grid.GetItemState(nRow,nCol) & GVIS_SELECTED );
		if( !bSelected )
			continue;

		LPARAM	id	=	m_Grid.GetItemData(nRow,0);
		CStockInfo & info	=	container.GetStockInfoByID(id);
		astr.Add( info.GetStockCode() );
	}
	container.UnLock();
}

void CSListView::ExportList( CListExportDlg * pDlg )
{
	ASSERT( pDlg );
	if( NULL == pDlg )
		return;

	int	nColumnCount	=	m_Grid.GetColumnCount();
	ASSERT( nColumnCount > 0 );
	if( nColumnCount <= 0 )
		return;

	CStockContainer & container = AfxGetSListStockContainer();

	// Get Current StockList Time
	DWORD	dwDate;
	CSPTime	sptime;
	CSPTime	time;
	if( container.GetCurrentType( NULL, NULL, &dwDate )
		&& (-1 != dwDate || container.GetLatestTechDate(&dwDate))
		&& sptime.FromStockTimeDay( dwDate ) )
		time	=	CSPTime( sptime.GetTime() );

	// Set Column
	CStringArray	astrColumn;
	CUIntArray		anWidth;
	CUIntArray		anParams;
	for(int nCol = 0; nCol < nColumnCount; nCol ++ )
	{
		astrColumn.Add( m_Grid.GetItemText(0,nCol) );
		anWidth.Add( m_Grid.GetColumnWidth(nCol) );
		anParams.Add( m_Grid.GetItemData(0,nCol) );
	}
	if( ! pDlg->ExportBegin( astrColumn, anWidth, TRUE )
		|| ! pDlg->ExportOpenTable( time, TRUE ) )
		return;

	container.Lock();

	// set Item
	int	nCount = 0, nTotalCount = 0;
	if( pDlg->m_bItemAll )
	{
		nTotalCount	=	m_Grid.GetRowCount()-1;
		pDlg->SetProgressRange( 0, nTotalCount );
		for( int nRow=1; nRow<m_Grid.GetRowCount(); nRow++ )
		{
			CStringArray	astrItemText;
			astrItemText.SetSize( 0, nColumnCount+1 );
			LPARAM	id	=	m_Grid.GetItemData(nRow,0);
			CStockInfo & info	=	container.GetStockInfoByID(id);
			for(nCol=0; nCol<anParams.GetSize(); nCol++ )
			{
				astrItemText.Add( m_Grid.GetItemText( nRow, nCol ) );
			}
			pDlg->ExportAddItem( astrItemText );

			nCount	++;
			pDlg->SetProgress( nCount );
		}
	}
	else if( pDlg->m_bItemSelected )
	{
		nTotalCount	=	m_Grid.GetSelectedCount();
		pDlg->SetProgressRange( 0, nTotalCount );
		for( int nRow=1; nRow<m_Grid.GetRowCount(); nRow++ )
		{
			BOOL	bSelected	=	FALSE;
			for( nCol=0; nCol<m_Grid.GetColumnCount(); nCol ++ )
				bSelected	|=	( m_Grid.GetItemState(nRow,nCol) & GVIS_SELECTED );
			if( !bSelected )
				continue;

			CStringArray	astrItemText;
			astrItemText.SetSize( 0, nColumnCount+1 );
			LPARAM	id	=	m_Grid.GetItemData(nRow,0);
			CStockInfo & info	=	container.GetStockInfoByID(id);
			for(nCol=0; nCol<anParams.GetSize(); nCol++ )
			{
				astrItemText.Add( m_Grid.GetItemText( nRow, nCol ) );
			}
			pDlg->ExportAddItem( astrItemText );

			nCount	++;
			pDlg->SetProgress( nCount );
		}
	}
	else if( pDlg->m_bItemDefine )
	{
		nTotalCount	=	min(m_Grid.GetRowCount()-1,pDlg->m_nItemEnd) - max(1,pDlg->m_nItemBegin) + 1;
		pDlg->SetProgressRange( 0, nTotalCount );
		for( int nRow=max(1,pDlg->m_nItemBegin); nRow<=min(m_Grid.GetRowCount()-1,pDlg->m_nItemEnd); nRow++ )
		{
			CStringArray	astrItemText;
			astrItemText.SetSize( 0, nColumnCount+1 );
			LPARAM	id	=	m_Grid.GetItemData(nRow,0);
			CStockInfo & info	=	container.GetStockInfoByID(id);
			for(nCol=0; nCol<anParams.GetSize(); nCol++ )
			{
				astrItemText.Add( m_Grid.GetItemText( nRow, nCol ) );
			}
			pDlg->ExportAddItem( astrItemText );

			nCount	++;
			pDlg->SetProgress( nCount );
		}
	}

	container.UnLock();

	pDlg->ExportFinish( );
}

void CSListView::RemoveSortSign( )
{
	if( m_nColSort < 0 || m_nColSort >= m_Grid.GetColumnCount() )
		return;

	LPARAM	lParam	=	m_Grid.GetItemData( 0, m_nColSort );
	CString	strColTitle	=	AfxGetVariantName(lParam,TRUE);
	m_Grid.SetItemText( 0, m_nColSort, strColTitle );

	m_nColSort		=	-1;
	m_bSortAscend	=	FALSE;
}

void CSListView::SetSortSign( )
{
	if( m_nColSort < 0 || m_nColSort >= m_Grid.GetColumnCount() )
		return;

	CString	strPrefix, strColTitle;
	if( m_bSortAscend )
		strPrefix.LoadString( IDS_SLISTVIEW_UP );
	else
		strPrefix.LoadString( IDS_SLISTVIEW_DOWN );

	LPARAM	lParam	=	m_Grid.GetItemData( 0, m_nColSort );
	strColTitle	=	AfxGetVariantName(lParam,TRUE);
	strColTitle	=	strPrefix + strColTitle;
	m_Grid.SetItemText( 0, m_nColSort, strColTitle );	
}

void CSListView::SetSortVariant( UINT nVariant )
{
	RemoveSortSign( );
	for( int nCol=0; nCol < m_Grid.GetColumnCount(); nCol++ )
	{
		LPARAM	lParam	=	m_Grid.GetItemData( 0, nCol );
		if( (UINT)lParam == nVariant )
		{
			m_nColSort		=	nCol;
			m_bSortAscend	=	FALSE;
			SetSortSign();
			break;
		}
	}
}

void CSListView::SetCurrentStatus( int nType, LPCTSTR lpszDomain, DWORD dwDate )
{
	AfxGetSListStockContainer().RetrieveFromStatic( nType, lpszDomain, AfxGetActiveStrategy(), dwDate );
	OnUpdate( NULL, UPDATE_HINT_SLISTVIEW, NULL );

	CMainFrame	* pFrame = AfxGetMainFrame();
	if( pFrame )
		pFrame->m_DateBox.SetCurrentWindowText( );
}

void CSListView::StoreColumnOrderArray( )
{
	// Now The CGridCtrl cannot drag and drop columns, so nothing to do
	// in this function
	return;
}

void CSListView::ResetColumns( )
{
	CStockContainer & container = AfxGetSListStockContainer();
	container.Lock();

	//  its list control through a call to GetListCtrl().
	CSPDWordArray	& auint = AfxGetProfile().GetSListColumnsShow( );
	ASSERT( auint.GetSize() > 0 );

	m_Grid.SetRedraw( FALSE );

	m_Grid.DeleteAllItems();

	
	const char * strColName[] ={"名称","代码","股票余额","拥股数量","可用数量","最新价","成本","证券市值","浮动盈亏"};
	const int colCnt= 9;

	m_Grid.SetRowCount(1);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetColumnCount(colCnt);
	
/*
	for( int nCol=0; nCol<auint.GetSize(); nCol++ )
	{
		CString	string	=	AfxGetVariantName(auint[nCol],TRUE);
		m_Grid.SetItemText( 0, nCol, string );
		m_Grid.SetItemData( 0, nCol, auint[nCol] );
		m_Grid.SetItemFormat( 0, nCol, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
		m_Grid.SetItemBkColour( 0, nCol, AfxGetProfile().GetColor(CColorClass::clrSListBK) );
		m_Grid.SetItemFgColour( 0, nCol, AfxGetProfile().GetColor(CColorClass::clrTitle) );
	}
	*/
    for( int nCol=0; nCol<colCnt; nCol++ )
	{
		CString	string(strColName[nCol]);
		m_Grid.SetItemText( 0, nCol, string );
		m_Grid.SetItemData( 0, nCol, auint[nCol] );
		m_Grid.SetItemFormat( 0, nCol, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
		m_Grid.SetItemBkColour( 0, nCol, AfxGetProfile().GetColor(CColorClass::clrSListBK) );
		m_Grid.SetItemFgColour( 0, nCol, AfxGetProfile().GetColor(CColorClass::clrTitle) );
	}
	m_Grid.AutoSizeColumns();

	m_Grid.SetRedraw( TRUE, TRUE );
	m_Grid.Invalidate( );

	container.UnLock();
}

BOOL CSListView::SetAverageItem( CGridCtrl &grid, CStockContainer & container, BOOL bRedraw )
{
	if( grid.GetColumnCount() <= 0 )
		return FALSE;

	container.Lock();

	// Get LPARAM
	CUIntArray	anParams;
	anParams.SetSize( 0, grid.GetColumnCount() );
	for( int nCol=0; nCol < grid.GetColumnCount(); nCol ++ )
	{
		LPARAM	lParam	=	grid.GetItemData( 0, nCol );
		anParams.Add( lParam );
	}

	// Set Average
	CStockInfo & infoAve = container.GetAverage( );
	CStockInfo & infoWAve = container.GetWeightAverage( );
	int	iRowAve=0, iRowWAve=0;

	if( grid.GetRowCount() >= 3
		&& grid.GetItemData(grid.GetRowCount()-2,0) == (LPARAM)ID_STOCKCNTN_AVERAGE
		&& grid.GetItemData(grid.GetRowCount()-1,0) == (LPARAM)ID_STOCKCNTN_WEIGHTAVERAGE )
	{
		// get item id
		iRowAve		=	grid.GetRowCount()-2;
		iRowWAve	=	grid.GetRowCount()-1;
	}
	else
	{
		// Insert item
		iRowAve = grid.InsertRow( infoAve.GetStockName() );
		grid.SetItemData( iRowAve, 0, (LPARAM)ID_STOCKCNTN_AVERAGE );
		iRowWAve = grid.InsertRow( infoWAve.GetStockName() );
		grid.SetItemData( iRowWAve, 0, (LPARAM)ID_STOCKCNTN_WEIGHTAVERAGE );
	}

	// Set Average
	for( nCol=0; nCol<anParams.GetSize(); nCol++ )
	{
		grid.SetItemText( iRowAve, nCol, AfxGetVariantDispString(anParams[nCol], infoAve, NULL) );
		grid.SetItemBkColour( iRowAve, nCol, AfxGetProfile().GetColor(CColorClass::clrSListBK) );
		grid.SetItemFgColour( iRowAve, nCol, AfxGetVariantColor( anParams[nCol], infoAve ) );
	}

	// Set Weight Average
	for( nCol=0; nCol<anParams.GetSize(); nCol++ )
	{
		grid.SetItemText( iRowWAve, nCol, AfxGetVariantDispString(anParams[nCol], infoWAve, NULL) );
		grid.SetItemBkColour( iRowWAve, nCol, AfxGetProfile().GetColor(CColorClass::clrSListBK) );
		grid.SetItemFgColour( iRowWAve, nCol, AfxGetVariantColor( anParams[nCol], infoWAve ) );
	}

	//	Set Param which is
	//	SLH_MARKETVALUE, SLH_MARKETVALUEA, SLH_MARKETVALUEB and etc,  and more than SLH_USERDEFINE_BEGIN
	for( nCol=0; nCol < anParams.GetSize(); nCol ++ )
	{
		UINT	lParam	=	anParams[nCol];
		if( SLH_DIFF == lParam || SLH_DIFFPERCENT == lParam || SLH_SCOPE == lParam
			|| SLH_DIFFPERCENT_MIN5 == lParam || SLH_PE == lParam
			|| SLH_PMAININCOME == lParam || SLH_RATIO_PCASH == lParam
			|| SLH_RATIO_CURRENCY == lParam || SLH_RATIO_CHANGEHAND == lParam
			|| SLH_RATIO_VOLUME == lParam || SLH_RS == lParam
			|| SLH_MARKETVALUE == lParam || SLH_MARKETVALUEA == lParam
			|| SLH_MARKETVALUEB == lParam || lParam >= SLH_USERDEFINE_BEGIN )
		{
			double	dc = 0., average = 0.;
			double	wsum = 0.0001, waverage = 0., w = 0.;
			for( int iRow=1; iRow<grid.GetRowCount(); iRow++ )
			{
				if( iRow == iRowAve || iRow == iRowWAve )
					continue;

				int	id	=	grid.GetItemData(iRow,0);
				if( id < 0 || id > container.GetSize() )
					continue;

				CStockInfo	& info = container.ElementAt(id);
				w	=	info.m_fShare_count_total;
				double	dValue	=	0.;
				if( !AfxGetVariantValue( lParam, info, &dValue, &container ) )
					continue;

				average		=	(average * dc + dValue)/(dc+1);
				waverage	=	(waverage * wsum + dValue * w)/(wsum+w);

				dc		+=	1;
				wsum	+=	w;
			}

			CString	strText;
			if( SLH_MARKETVALUE == lParam || SLH_MARKETVALUEA == lParam || SLH_MARKETVALUEB == lParam )
			{
				strText.Format( "%u", (DWORD)average );
				grid.SetItemText( iRowAve, nCol, strText );
				grid.SetItemText( iRowWAve, nCol, "-" );
			}
			else
			{
				strText.Format( "%.2f", average );
				grid.SetItemText( iRowAve, nCol, strText );
				strText.Format( "%.2f", waverage );
				grid.SetItemText( iRowWAve, nCol, strText );
			}
		}
	}

	container.UnLock();

	if( bRedraw )
	{
		grid.RedrawRow( iRowAve );
		grid.RedrawRow( iRowWAve );
	}
	return TRUE;
}

void CSListView::OnDblclkItem( int nStockIndex )
{/* 
  ** want it do nothing
  **
	CStockContainer & container = AfxGetSListStockContainer();
	int	nType;
	CSPString	strDomain;
	DWORD	dwDate	=	-1;
	if( nStockIndex >= 0 )
	{
		container.GetCurrentType( &nType, &strDomain, &dwDate );

		if( ( nType == CStockContainer::typeDomain && strDomain.IsEmpty() )
			|| ( nType == CStockContainer::typeGroup && strDomain.IsEmpty() ) )
		{
			CStockInfo	& info	=	container.GetStockInfoByID( nStockIndex );
			AfxShowSlist( nType, CString(info.GetStockName()), dwDate );
		}
		else
			AfxShowStockGraph( nStockIndex, TRUE );
	}
	*/
}

void CSListView::StockInfoChanged( LONG infoid, CStockInfo & info )
{
	CStockContainer & container = AfxGetSListStockContainer();

	container.Lock();

	// update grid value
	for( int nRow=m_Grid.GetRowCount()-1; nRow >= 0; nRow-- )
	{
		LPARAM	id	=	m_Grid.GetItemData(nRow,0);
		if( id == infoid )
		{
			for( int nCol=0; nCol<m_Grid.GetColumnCount(); nCol++ )
			{
				LPARAM lParam = m_Grid.GetItemData(0,nCol);
				m_Grid.SetItemText( nRow, nCol, AfxGetVariantDispString( lParam, info, &container ) );
				m_Grid.SetItemFgColour( nRow, nCol, AfxGetVariantColor( lParam, info ) );
			}
			m_Grid.RedrawRow( nRow );
			break;
		}
	}

	container.UnLock();
}

void CSListView::SendRequestQuote( BOOL bForced )
{
	static int nLastStart = -1;
	static int nLastCount = -1;

	BOOL bQuoteTipSended = FALSE;

	BOOL bInTrade = CSPTime::InTradeTime(CSPTime::GetCurrentTime().GetTime(),900);
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	if( bForced )
	{
		CStockContainer stocks;
		CStockInfo	info;
		if( AfxGetStockContainer().GetStockInfo( STKLIB_CODE_MAIN, &info ) )
			stocks.Add(info);
		if( AfxGetStockContainer().GetStockInfo( STKLIB_CODE_MAINSZN, &info ) )
			stocks.Add(info);
		if( stocks.GetSize() > 0 )
			AfxGetStkReceiver().RequestStockData( CStock::dataReport, stocks.GetData(), stocks.GetSize(), 0, 0 );
	}
	if( bForced || pView == this )
	{
		CCellRange cr = m_Grid.GetVisibleNonFixedCellRange();
		int nStart = cr.GetMinRow() - 1;
		int nCount = cr.GetMaxRow() - cr.GetMinRow() + 1;
		CStockContainer & container = AfxGetSListStockContainer();
		container.Lock();
		if( nStart >= 0 && nCount > 0 && container.GetSize() > 0 && (bInTrade || bForced || nLastStart != nStart || nLastCount != nCount) )
		{
			nLastStart = nStart;
			nLastCount = nCount;
			AfxGetStkReceiver().RequestStockData( CStock::dataReport, container.GetData()+nStart, min(nCount,container.GetSize()-nStart), 0, 0 );
			bQuoteTipSended = (0 == nStart && nCount >= 9);
		}
		container.UnLock();
	}

	if( bInTrade )
	{
		// 预警系统所需股票行情刷新
		CAlarmCondContainer & conds = AfxGetProfile().GetAlarmCondContainer();
		CStockContainer stocks;
		stocks.SetSize( 0, conds.GetSize() + 5 );
		for( int i=0; i<conds.GetSize(); i++ )
		{
			ALARMCOND cond = conds.GetAt(i);
			CStockInfo info;
			if( AfxGetStockContainer().GetStockInfo( cond.m_szCode, &info ) )
				stocks.Add( info );
		}
		CStockInfo infoMain, infoMainSzn;
		if( AfxGetStockContainer().GetStockInfo( STKLIB_CODE_MAIN, &infoMain )
			&& AfxGetStockContainer().GetStockInfo( STKLIB_CODE_MAINSZN, &infoMainSzn ) )
		{
			stocks.Add( infoMain );
			stocks.Add( infoMainSzn );
		}

		//  行情QuoteTip 需要行情数据
		if( !bQuoteTipSended )
		{
			CStockContainer & container = AfxGetSListStockContainer();
			container.Lock();
			for( int i=0; i<9 && i<container.GetSize(); i++ )
				stocks.Add( container.ElementAt(i) );
			container.UnLock();
		}

		if( stocks.GetSize() > 0 )
			AfxGetStkReceiver().RequestStockData( CStock::dataReport, stocks.GetData(), stocks.GetSize(), 0, 0 );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSListView overrides

BOOL CSListView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CSListView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( ::IsWindow(m_Grid.GetSafeHwnd()) )
		if( m_Grid.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;

	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

int CALLBACK ItemCompareFunc(LPARAM lParam1, LPARAM lParam2, 
	LPARAM lParamSort)
{
	if( lParam1 < 0 && lParam2 < 0 )
		return 0;
	if( lParam1 < 0 )
		return 1;
	if( lParam2 < 0 )
		return -1;

	CStockContainer	& container = AfxGetSListStockContainer();
	if( lParam1 >= container.GetSize() || lParam2 >= container.GetSize() )
	{
		ASSERT( FALSE );
		return 0;
	}
	CStockInfo &s1 = container.ElementAt(lParam1);
	CStockInfo &s2 = container.ElementAt(lParam2);
	return CStockContainer::SortFunction( &s1, &s2 );
}

BOOL CSListView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( m_Grid.GetSafeHwnd() == pMsg->hwnd )
	{
		if( WM_LBUTTONDBLCLK == pMsg->message
			|| ( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam ) )
		{
			int	nColumnCount	=	m_Grid.GetColumnCount();
			int	nRowCount		=	m_Grid.GetRowCount();
			if( nColumnCount <=0 || nRowCount <= 0 )
				return CFormView::PreTranslateMessage(pMsg);
			CRect	rectCell;
			m_Grid.GetCellRect(0,0,&rectCell);
			CPoint	pt	=	pMsg->pt;
			::ScreenToClient( m_Grid.GetSafeHwnd(), &pt );
			if( pt.y >= rectCell.top && pt.y < rectCell.bottom )
				return CFormView::PreTranslateMessage(pMsg);
/*
			int	nSelRow	=	m_Grid.GetFocusCell().row;
			if( nSelRow >= 1 && nSelRow < m_Grid.GetRowCount() )
			{
				int	id	=	m_Grid.GetItemData(nSelRow,0);
				OnDblclkItem( id );
			}
			*/
		}
		else if( WM_LBUTTONUP == pMsg->message )
		{
			int	nColumnCount	=	m_Grid.GetColumnCount();
			int	nRowCount		=	m_Grid.GetRowCount();
			if( nColumnCount <=0 || nRowCount <= 0 )
				return CFormView::PreTranslateMessage(pMsg);
			CRect	rectCell;
			m_Grid.GetCellRect(0,0,&rectCell);
			CPoint	pt	=	pMsg->pt;
			::ScreenToClient( m_Grid.GetSafeHwnd(), &pt );
			if( pt.y < rectCell.top || pt.y >= rectCell.bottom )
				return CFormView::PreTranslateMessage(pMsg);

			CCellID	cell	=	m_Grid.GetFocusCell();
			if( cell.col >=0 && cell.col < nColumnCount )
			{/*
				BOOL	bSortAscendNew	=	( m_nColSort == cell.col ? !m_bSortAscend : FALSE );

				RemoveSortSign( );

				m_nColSort		=	cell.col;
				m_bSortAscend	=	bSortAscendNew;
				SetSortSign( );

				LPARAM	lParam	=	m_Grid.GetItemData( 0, cell.col );
				if( lParam >= 0 )
				{
					CStockContainer & container = AfxGetSListStockContainer();
					container.Lock();
					//if( container.Sort( lParam, m_bSortAscend ) )
					//	OnUpdate( NULL, UPDATE_HINT_SLISTVIEW, NULL );
					CStockContainer::m_pSortContainer	=	&container;
					container.m_nSortVariantID	=	lParam;
					container.m_bSortAscend		=	m_bSortAscend;
					m_Grid.SortItems( ItemCompareFunc, 0, TRUE );
					container.UnLock();
				}

				m_Grid.Invalidate( );
				*/
			}
		}
	}
	
	return CFormView::PreTranslateMessage(pMsg);
}

void CSListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSListView)
	//}}AFX_DATA_MAP
}

void CSListView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//	SetColumn
	ResetColumns( );

	// Create Tab
	if( !::IsWindow( m_wndWorkTab.GetSafeHwnd() )
		&& m_wndWorkTab.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_SLISTVIEW_WORKTAB) )
	{
		CString	strName;
		/*
		strName.LoadString( IDS_SLISTVIEW_CLASS );
		m_wndWorkTab.Addtab( &m_Grid, strName, SL_WORKTAB_CLASS);
		strName.LoadString( IDS_SLISTVIEW_A );
		m_wndWorkTab.Addtab( &m_Grid, strName, SL_WORKTAB_A);
		strName.LoadString( IDS_SLISTVIEW_SZNB );
		m_wndWorkTab.Addtab( &m_Grid, strName, SL_WORKTAB_SZNB);
		strName.LoadString( IDS_SLISTVIEW_SHB );
		m_wndWorkTab.Addtab( &m_Grid, strName, SL_WORKTAB_SHB);
		strName.LoadString( IDS_SLISTVIEW_BOND );
		m_wndWorkTab.Addtab( &m_Grid, strName, SL_WORKTAB_BOND);
		strName.LoadString( IDS_SLISTVIEW_FUND );
		m_wndWorkTab.Addtab( &m_Grid, strName, SL_WORKTAB_FUND);*/
		strName.LoadString( IDS_SLISTVIEW_SELF );
		m_wndWorkTab.Addtab( &m_Grid, strName, SL_WORKTAB_SELF);/*
		strName.LoadString( IDS_SLISTVIEW_GROUP );
		m_wndWorkTab.Addtab( &m_Grid, strName, SL_WORKTAB_GROUP);
		strName.LoadString( IDS_SLISTVIEW_DOMAIN );
		m_wndWorkTab.Addtab( &m_Grid, strName, SL_WORKTAB_DOMAIN);*/

		m_wndWorkTab.SetSelectTabColor( AfxGetProfile().GetColor(CColorClass::clrSListBK),
										AfxGetProfile().GetColor(CColorClass::clrTitle) );
		m_wndWorkTab.SetActualTitle(FALSE);
		m_wndWorkTab.SetCurtab( SL_WORKTAB_SELF );

		CFont font;
		font.CreateStockObject (DEFAULT_GUI_FONT);
		m_wndWorkTab.SetFont( &font );

		// m_Grid No HScrollBar
		m_Grid.SetOuterHScrollBar( TRUE, m_wndWorkTab.GetSafeHwnd() );
	}
}

void CSListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( UPDATE_HINT_SLISTVIEW != lHint )
		return;
	if( IsWindowVisible() )
		SetFocus( );
	int	nColumnCount	=	m_Grid.GetColumnCount();
	if( nColumnCount <= 0 )
		return;

	if( GetParentFrame()->GetSafeHwnd() != AfxGetMainFrame()->GetActiveFrame()->GetSafeHwnd() )
	{
		m_bShouldUpdate	=	TRUE;
		return;
	}
	m_bShouldUpdate	=	FALSE;

	CStockContainer & container = AfxGetSListStockContainer();
	container.Lock();

	m_Grid.DeleteNonFixedRows();

	// Progress
	CMainFrame	* pMainFrame = AfxGetMainFrame();
	if( pMainFrame )
	{
		pMainFrame->ShowProgressBar( );
		pMainFrame->SetProgress( 0 );
		pMainFrame->SetMessageText( IDS_MAINFRAME_WAITING );
	}

	CUIntArray	anParams;
	anParams.SetSize( 0, nColumnCount );
	for( int nCol=0; nCol < nColumnCount; nCol ++ )
	{
		LPARAM	lParam	=	m_Grid.GetItemData( 0, nCol );
		anParams.Add( lParam );
	}

	CRect rectClient;
	GetClientRect( &rectClient );
	int nPageCount = 1 + rectClient.Height() / abs(m_Grid.GetFixedRowHeight()) + 1;
/** disabled 
	for( int i=0; i<container.GetSize(); i++ )
	{
		CStockInfo & info = container.GetStockInfoByID(i);

		int nRow = m_Grid.InsertRow( info.GetStockName() );
		m_Grid.SetItemData( nRow, 0, i );

		for( int nCol=0; nCol<anParams.GetSize(); nCol++ )
		{
			m_Grid.SetItemFormat( nRow, nCol, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
			m_Grid.SetItemText( nRow, nCol, AfxGetVariantDispString( anParams[nCol], info, &container ) );
			m_Grid.SetItemBkColour( nRow, nCol, AfxGetProfile().GetColor(CColorClass::clrSListBK) );
			m_Grid.SetItemFgColour( nRow, nCol, AfxGetVariantColor( anParams[nCol], info ) );
			if( anParams[nCol] == SLH_DATE )
				m_Grid.SetColumnWidth( nCol, 80 );
		}

		if( i == nPageCount+5 )
		{
			m_Grid.Invalidate( );
			MSG		msg;
			while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
				AfxGetApp()->PumpMessage();
		}
		if( pMainFrame )
			pMainFrame->SetProgress( 100*i/container.GetSize() );
	}

	// Set Average and Weight Average
	SetAverageItem( m_Grid, container, FALSE );

	// Sort If Needed
	if( -1 != m_nColSort )
	{
		LPARAM	lParam	=	m_Grid.GetItemData( 0, m_nColSort );
		if( lParam >= 0 )
		{
			CStockContainer::m_pSortContainer	=	&container;
			container.m_nSortVariantID	=	lParam;
			container.m_bSortAscend		=	m_bSortAscend;
			m_Grid.SortItems( ItemCompareFunc, 0, TRUE );
		}
	}

*/
	for(int i= 0;i<1;i++)
	{
		int nRow = m_Grid.InsertRow( "sz002376");
		m_Grid.SetItemData( nRow, 0, i );
        const char * paramValues [] = {"新北洋","sz002376","1000","1000","1000","17.74","19.08","17740","-1340.0"};
		for( int nCol=0; nCol<9; nCol++ )
		{
			m_Grid.SetItemFormat( nRow, nCol, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
			m_Grid.SetItemText( nRow, nCol, paramValues[nCol] );
			m_Grid.SetItemBkColour( nRow, nCol, AfxGetProfile().GetColor(CColorClass::clrSListBK) );
			//m_Grid.SetItemFgColour( nRow, nCol, );
			if( anParams[nCol] == SLH_DATE )
				m_Grid.SetColumnWidth( nCol, 80 );
		}
	}

	if( pMainFrame )
	{
		pMainFrame->SetProgress( 100 );
		pMainFrame->HideProgressBar( );
		pMainFrame->SetMessageText( IDS_MAINFRAME_FINISHED );
		pMainFrame->SetMessageText( IDS_HELPTIP_SLIST );
	}

	if( m_bFirstUpdate )
		m_Grid.AutoSizeColumns( );
	else
		m_Grid.SetColumnWidth( 0, 80 );
	m_bFirstUpdate	=	FALSE;
	m_Grid.Invalidate( );

	SendRequestQuote(TRUE);

	container.UnLock();
}

/////////////////////////////////////////////////////////////////////////////
// CSListView printing

BOOL CSListView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSListView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if( ::IsWindow( m_Grid.GetSafeHwnd() ) )
		m_Grid.OnBeginPrinting(pDC, pInfo);
}

void CSListView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	if( ::IsWindow( m_Grid.GetSafeHwnd() ) )
		m_Grid.OnPrint(pDC, pInfo);
}

void CSListView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if( ::IsWindow( m_Grid.GetSafeHwnd() ) )
		m_Grid.OnEndPrinting(pDC, pInfo);
}

void CSListView::OnFilePrint() 
{
	if( ::IsWindow( m_Grid.GetSafeHwnd() ) )
		m_Grid.Print( );
}

void CSListView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( ::IsWindow( m_Grid.GetSafeHwnd() ) );
}

/////////////////////////////////////////////////////////////////////////////
// CSListView diagnostics

#ifdef _DEBUG
void CSListView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSListView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSListView message handlers

int CSListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create GridCtrl
	CRect rect;
	GetClientRect(rect);
	if( !m_Grid.Create(rect, this, IDC_SLISTVIEW_GRID, WS_CHILD|WS_BORDER|WS_TABSTOP|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL) )
	{
		TRACE( "CSListView::OnCreate(...), Create m_Grid Error.\n" );
		return -1;
	}

	// fill it up with stuff
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(TRUE);
	m_Grid.SetHeaderSort( FALSE ); 
	m_Grid.SetSingleRowSelection(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.SetGridLines( GVL_NONE );
	m_Grid.EnableTitleTips( TRUE );
	m_Grid.SetRowResize( TRUE );
	m_Grid.SetColumnResize( TRUE );

	m_Grid.SetBkColor( AfxGetProfile().GetColor(CColorClass::clrSListBK) );
	m_Grid.SetTextBkColor( AfxGetProfile().GetColor(CColorClass::clrSListBK) );
	m_Grid.SetSelectedBkColor(AfxGetProfile().GetColor(CColorClass::clrSListSelected));

	LOGFONT lf;
	memset( &lf, 0, sizeof(lf) );
	AfxGetProfile().GetFontSListView( &lf );
	SetFont( &lf );

	// 实时行情刷新
	AfxGetStkReceiver().AddRcvDataWnd( GetSafeHwnd() );

	SetTimer( SLV_TIMER_REFRESH, 20000, NULL );
	
	return 0;
}

void CSListView::OnDestroy() 
{
	// 停止行情刷新通知消息
	AfxGetStkReceiver().RemoveRcvDataWnd( GetSafeHwnd() );
	KillTimer( SLV_TIMER_REFRESH );

	StoreColumnOrderArray( );

	CFormView::OnDestroy();
}

void CSListView::OnTimer(UINT nIDEvent) 
{
	if( SLV_TIMER_REFRESH == nIDEvent )
	{
		SendRequestQuote(FALSE);
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CSListView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMainFrame	* pFrame	=	AfxGetMainFrame( );
	if( pFrame )
		pFrame->SetMessageText( bActivate ? IDS_HELPTIP_SLIST : AFX_IDS_IDLEMESSAGE );

	if( bActivate && m_bShouldUpdate )
	{
		OnUpdate( NULL, UPDATE_HINT_SLISTVIEW, NULL );
		m_bShouldUpdate	=	FALSE;
	}

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if( ::IsWindow(m_wndWorkTab.GetSafeHwnd()) )
	{
		m_wndWorkTab.Invalidate( );
		m_wndWorkTab.UpdateWindow( );
	}
}

void CSListView::OnSelchangeWorktab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nCur = m_wndWorkTab.GetCurtab();
	CRect	rect = m_wndWorkTab.GetTabRect( nCur );
	CMenu	menu;
	switch( nCur )
	{
	case SL_WORKTAB_CLASS:
		if( menu.LoadMenu( IDR_MENU_SLISTBARCLASS ) )
		{
			CMenu* pPopupMenu = menu.GetSubMenu(0);
			if( pPopupMenu )
				pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON , rect.left+3, rect.top+6, AfxGetMainFrame() );
		}
		break;
	case SL_WORKTAB_A:
		SetCurrentStatus( CStockContainer::typeA, NULL, -1 );
		break;
	case SL_WORKTAB_SZNB:
		SetCurrentStatus( CStockContainer::typeClassSznb, NULL, -1 );
		break;
	case SL_WORKTAB_SHB:
		SetCurrentStatus( CStockContainer::typeClassShab, NULL, -1 );
		break;
	case SL_WORKTAB_BOND:
		SetCurrentStatus( CStockContainer::typeBond, NULL, -1 );
		break;
	case SL_WORKTAB_FUND:
		SetCurrentStatus( CStockContainer::typeFund, NULL, -1 );
		break;
	case SL_WORKTAB_SELF:
		{
			CString	strSelf;
			strSelf.LoadString( IDS_DOMAIN_SELF );
			SetCurrentStatus( CStockContainer::typeGroup, strSelf, -1 );
		}
		break;
	case SL_WORKTAB_GROUP:
		if( menu.LoadMenu( IDR_MENU_SLISTBARGROUP ) )
		{
			CMenu* pPopupMenu = menu.GetSubMenu(0);
			if( pPopupMenu )
				pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON , rect.left+3, rect.top+6, AfxGetMainFrame() );
		}
		break;
	case SL_WORKTAB_DOMAIN:
		if( menu.LoadMenu( IDR_MENU_SLISTBARDOMAIN ) )
		{
			CMenu* pPopupMenu = menu.GetSubMenu(0);
			if( pPopupMenu )
				pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON , rect.left+3, rect.top+6, AfxGetMainFrame() );
		}
		break;
	default:;
	}
	
	*pResult = 0;
}

LRESULT CSListView::OnGetViewTitle(WPARAM wParam, LPARAM lParam)
{
	CString	strTitle;
	strTitle.LoadString( IDS_TITLE_SLISTVIEW );

	lstrcpyn((LPTSTR)lParam, (LPCTSTR)strTitle, wParam);
	if ((int)wParam > strTitle.GetLength())
		wParam = strTitle.GetLength();
	return wParam;
}

LRESULT CSListView::OnGetViewCmdid(WPARAM wParam, LPARAM lParam)
{
	if( lParam )
		*((LPARAM *)lParam)	=	ID_VIEW_SLIST;
	return 0L;
}

LRESULT CSListView::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	m_wndWorkTab.SetSelectTabColor( AfxGetProfile().GetColor(CColorClass::clrSListBK),
									AfxGetProfile().GetColor(CColorClass::clrTitle) );
	m_wndWorkTab.Invalidate( );

	m_Grid.SetBkColor( AfxGetProfile().GetColor(CColorClass::clrSListBK) );
	m_Grid.SetTextBkColor( AfxGetProfile().GetColor(CColorClass::clrSListBK) );
	m_Grid.SetSelectedBkColor(AfxGetProfile().GetColor(CColorClass::clrSListSelected));

	OnUpdate( NULL, UPDATE_HINT_SLISTVIEW, NULL );
	return 0L;
}

LRESULT CSListView::OnStkReceiverData(WPARAM wParam, LPARAM lParam)
{
	PCOMMPACKET	pCommPacket	=	(PCOMMPACKET)lParam;

	switch( wParam )
	{
	case CStock::dataReport:
		if( pCommPacket && CStock::dataReport == pCommPacket->m_dwDataType 
			&& pCommPacket->m_dwCount > 0 )
		{
			CStockContainer & container = AfxGetSListStockContainer();
			// 如果当前股票列表显示的日期不是今天的，返回，不做刷新
			DWORD	dwDate = -1;;
			AfxGetSListStockContainer().GetCurrentType(NULL,NULL,&dwDate);
			if( -1 != dwDate && dwDate != CSPTime(pCommPacket->m_pReport[0].m_time).ToStockTimeDay() )
				return 0L;

			container.Lock();

			// 刷新数据
			BOOL	bNeedReSort	=	FALSE;
			for( DWORD i=0; i<pCommPacket->m_dwCount; i++ )
			{
				int	id = 0;
				if( container.GetStockInfo( pCommPacket->m_pReport[i].m_szCode, NULL, &id ) )
				{
					CStockInfo	& info	=	container.ElementAt(id);
					UpdateStockInfoByREPORT( info, &(pCommPacket->m_pReport[i]) );
					StockInfoChanged( id, info );
					bNeedReSort	=	TRUE;
				}
			}

			// 统计平均值刷新
			if( bNeedReSort )
			{
				container.SetAverage( );
				SetAverageItem( m_Grid, container, TRUE );
			}

			// 重新排序
			if( -1 != m_nColSort && bNeedReSort )
			{
				LPARAM	lParam	=	m_Grid.GetItemData( 0, m_nColSort );
				if( lParam >= 0 )
				{
					CStockContainer::m_pSortContainer	=	&container;
					container.m_nSortVariantID	=	lParam;
					container.m_bSortAscend		=	m_bSortAscend;
					m_Grid.SortItems( ItemCompareFunc, 0, TRUE );
				}
				m_Grid.Invalidate( );
			}
			container.UnLock();
		}
		break;
	}
	return 0L;
}

BOOL CSListView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	if( pDC && pDC->IsKindOf(RUNTIME_CLASS(CDC)) )
	{
		CRect rect;
		GetClientRect( &rect );
		CBrush brush;
		brush.CreateSolidBrush( AfxGetProfile().GetColor(CColorClass::clrSListBK) );
		pDC->FillRect( &rect, &brush );
		return TRUE;
	}
	return CFormView::OnEraseBkgnd(pDC);
}

void CSListView::OnSize(UINT nType, int cx, int cy) 
{
	CSize	sizeTotal( cx, cy );
	SetScaleToFitSize( sizeTotal );

	CFormView::OnSize(nType, cx, cy);
	
	CRect rect;
	GetClientRect(rect);
/*	if( ::IsWindow(m_Grid.GetSafeHwnd()) )
	{
		m_Grid.MoveWindow(rect);
	}
*/	
	if( ::IsWindow(m_wndWorkTab.GetSafeHwnd()) )
	{
		rect.InflateRect(1,0,0,1);
		m_wndWorkTab.MoveWindow(rect);
		m_wndWorkTab.Invalidate( );
		m_wndWorkTab.UpdateWindow( );
	}
}

void CSListView::OnSetFocus(CWnd* pOldWnd) 
{
	CFormView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	m_Grid.SetFocus();
	if( !m_Grid.GetFocusCell().IsValid()
		&& m_Grid.GetRowCount() > 1 && m_Grid.GetColumnCount() > 1 )
	{
		m_Grid.SetFocusCell( 1, 1 );
		for( int nCol=0; nCol<m_Grid.GetColumnCount(); nCol ++ )
			m_Grid.SetItemState(1,nCol,m_Grid.GetItemState(1,nCol) | GVIS_SELECTED);
	}
}

void CSListView::OnSlistExportlist() 
{
	// TODO: Add your command handler code here
	CListExportDlg	dlg;
	dlg.m_nItemMax	=	m_Grid.GetRowCount();
	if( m_Grid.GetSelectedCount() > 0 )
	{
		dlg.m_bItemAll		=	FALSE;
		dlg.m_bItemSelected	=	TRUE;
		dlg.m_bItemDefine	=	FALSE;
	}
	dlg.DoModal( );
}

