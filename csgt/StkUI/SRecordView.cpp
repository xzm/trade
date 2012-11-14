// SRecordView.cpp : implementation file
//

#include "stdafx.h"
#include "stkui.h"
#include "SRecordView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSRecordView


#define	IDC_SRECORDVIEW_GRID		210
#define	IDC_SRECORDVIEW_WORKTAB	211

#define	SL_WORKTAB_SELF		1
IMPLEMENT_DYNCREATE(CSRecordView, CFormView)

CSRecordView::CSRecordView()
	: CFormView(CSRecordView::IDD)
{
	m_bShouldUpdate	=	FALSE;
	m_bFirstUpdate	=	TRUE;
}

CSRecordView::~CSRecordView()
{
}

void CSRecordView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSRecordView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSRecordView, CFormView)
	//{{AFX_MSG_MAP(CSRecordView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_GETVIEWTITLE, OnGetViewTitle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSRecordView diagnostics

#ifdef _DEBUG
void CSRecordView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSRecordView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSRecordView message handlers

void CSRecordView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
void CSRecordView::SetFont( LPLOGFONT pLogFont )
{
	ASSERT( pLogFont );

	HFONT hFont = ::CreateFontIndirect(pLogFont);
	m_Grid.SendMessage( WM_SETFONT, (WPARAM)hFont, MAKELPARAM(1, 0) );
	m_Grid.AutoSize( );
	DeleteObject(hFont);
}
int CSRecordView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create GridCtrl
	CRect rect;
	GetClientRect(rect);

	if( !m_Grid.Create(rect, this, IDC_SRECORDVIEW_GRID, WS_CHILD|WS_BORDER|WS_TABSTOP|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL) )
	{
		TRACE( "CSRecordView::OnCreate(...), Create m_Grid Error.\n" );
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

	return 0;
}

void CSRecordView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnTimer(nIDEvent);
}

void CSRecordView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
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
	for(int i= 0;i<1;i++)
	{
		int nRow = m_Grid.InsertRow( "20120620");
		m_Grid.SetItemData( nRow, 0, i );
        const char * paramValues [] = {"20120620","F4003323","0117363647","证券买入"
			      ,"-19172.16","19.02","1000","2163.99"};
		for( int nCol=0; nCol<8; nCol++ )
		{
			m_Grid.SetItemFormat( nRow, nCol, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
			m_Grid.SetItemText( nRow, nCol, paramValues[nCol] );
			m_Grid.SetItemBkColour( nRow, nCol, AfxGetProfile().GetColor(CColorClass::clrSListBK) );
			//m_Grid.SetItemFgColour( nRow, nCol, );
			if( anParams[nCol] == SLH_DATE )
				m_Grid.SetColumnWidth( nCol, 80 );
		}
	}


	if( m_bFirstUpdate )
		m_Grid.AutoSizeColumns( );
	else
		m_Grid.SetColumnWidth( 0, 80 );
	m_bFirstUpdate	=	FALSE;
	m_Grid.Invalidate( );


	container.UnLock();
	
}

void CSRecordView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	//	SetColumn
	ResetColumns( );

	// Create Tab
	if( !::IsWindow( m_wndWorkTab.GetSafeHwnd() )
		&& m_wndWorkTab.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_SRECORDVIEW_WORKTAB) )
	{
		CString	strName("对帐单");
		m_wndWorkTab.Addtab( &m_Grid, strName, SL_WORKTAB_SELF);
/*
		m_wndWorkTab.SetSelectTabColor( AfxGetProfile().GetColor(CColorClass::clrSListBK),
										AfxGetProfile().GetColor(CColorClass::clrTitle) );
		m_wndWorkTab.SetActualTitle(FALSE);
		m_wndWorkTab.SetCurtab( SL_WORKTAB_SELF );

		CFont font;
		font.CreateStockObject (DEFAULT_GUI_FONT);
		m_wndWorkTab.SetFont( &font );

		// m_Grid No HScrollBar
		m_Grid.SetOuterHScrollBar( TRUE, m_wndWorkTab.GetSafeHwnd() );*/
	}
}
LRESULT CSRecordView::OnGetViewCmdid(WPARAM wParam, LPARAM lParam)
{
	if( lParam )
		*((LPARAM *)lParam)	=	ID_VIEW_SLIST;
	return 0L;
}
void CSRecordView::ResetColumns( )
{
	CStockContainer & container = AfxGetSListStockContainer();
	container.Lock();

	//  its list control through a call to GetListCtrl().

	m_Grid.SetRedraw( FALSE );

	m_Grid.DeleteAllItems();

	
	const char * strColName[] ={"记帐日期","合同序号","股东代码","摘 要","交易金额","证券价格","证券数量","资金余额"};
	const int colCnt= 8;

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
		m_Grid.SetItemData( 0, nCol, 1 );
		m_Grid.SetItemFormat( 0, nCol, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
		m_Grid.SetItemBkColour( 0, nCol, AfxGetProfile().GetColor(CColorClass::clrSListBK) );
		m_Grid.SetItemFgColour( 0, nCol, AfxGetProfile().GetColor(CColorClass::clrTitle) );
	}
	m_Grid.AutoSizeColumns();

	m_Grid.SetRedraw( TRUE, TRUE );
	m_Grid.Invalidate( );

	container.UnLock();
}


LRESULT CSRecordView::OnGetViewTitle(WPARAM wParam, LPARAM lParam)
{
	CString	strTitle("对帐单");

	lstrcpyn((LPTSTR)lParam, (LPCTSTR)strTitle, wParam);
	if ((int)wParam > strTitle.GetLength())
		wParam = strTitle.GetLength();
	return wParam;
}

void CSRecordView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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

void CSRecordView::OnSize(UINT nType, int cx, int cy) 
{
	CSize	sizeTotal( cx, cy );
	SetScaleToFitSize( sizeTotal );

	CFormView::OnSize(nType, cx, cy);
	
	CRect rect;
	GetClientRect(rect);
	rect.InflateRect(1,0,0,1);
	
	if( ::IsWindow(m_wndWorkTab.GetSafeHwnd()) )
	{
		rect.InflateRect(1,0,0,1);
		m_wndWorkTab.MoveWindow(rect);
		m_wndWorkTab.Invalidate( );
		m_wndWorkTab.UpdateWindow( );
	}
	
}

BOOL CSRecordView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( ::IsWindow(m_Grid.GetSafeHwnd()) )
		if( m_Grid.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;

	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
