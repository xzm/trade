// QuoteTipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QuoteTipDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuoteTipDlg dialog

CQuoteTipDlg & CQuoteTipDlg::GetInstance()
{
	static CQuoteTipDlg	s_quotetip;
	if( !::IsWindow(s_quotetip.GetSafeHwnd()) )
		VERIFY( s_quotetip.Create( CQuoteTipDlg::IDD, CWnd::GetDesktopWindow() ) );
	return s_quotetip;
}


CQuoteTipDlg::CQuoteTipDlg(CWnd* pParent /*=NULL*/)
	: CTipDialog(CQuoteTipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuoteTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nIndexStart	=	0;
	m_nPageCount	=	9;
	m_nItemHeight	=	15;
}

void CQuoteTipDlg::SetItemHeight( int nHeight )
{
	if( nHeight > 0 )
		m_nItemHeight = nHeight;

	if( ::IsWindow(GetSafeHwnd()) )
	{
		CRect rectClient;
		GetClientRect( &rectClient );
		m_nPageCount = (rectClient.Height()-25) / m_nItemHeight;
		m_nPageCount -= 1;
		if( m_nPageCount < 1 )
			m_nPageCount = 1;
	}
}

void CQuoteTipDlg::Show( )
{
	m_nIndexStart	=	0;
	UpdateButtons( );
	CTipDialog::Show();
}
void CQuoteTipDlg::Hide( )
{
	CTipDialog::Hide();
}
void CQuoteTipDlg::ShowGradual( )
{
	m_nIndexStart	=	0;
	UpdateButtons( );
	CTipDialog::ShowGradual();
}
void CQuoteTipDlg::HideGradual( )
{
	CTipDialog::HideGradual();
}

void CQuoteTipDlg::UpdateButtons( )
{
	m_btnPrior.EnableWindow( m_nIndexStart > 0 );
	m_btnNext.EnableWindow( m_nIndexStart + m_nPageCount < AfxGetSListStockContainer().GetSize() );
}

void CQuoteTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CTipDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuoteTipDlg)
	DDX_Control(pDX, IDC_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_PRIOR, m_btnPrior);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuoteTipDlg, CTipDialog)
	//{{AFX_MSG_MAP(CQuoteTipDlg)
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_PRIOR, OnPrior)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuoteTipDlg message handlers

BOOL CQuoteTipDlg::OnInitDialog() 
{
	CTipDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetItemHeight( );

	UpdateButtons();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQuoteTipDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( VK_PRIOR == nChar )
	{
		OnPrior();
	}
	else if( VK_NEXT == nChar )
	{
		OnNext();
	}
	CTipDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CQuoteTipDlg::OnPrior() 
{
	m_nIndexStart -= m_nPageCount;
	if( m_nIndexStart < 0 )
		m_nIndexStart = 0;
	UpdateButtons();
	Invalidate( );
}

void CQuoteTipDlg::OnNext() 
{
	m_nIndexStart += m_nPageCount;
	if( m_nIndexStart >= AfxGetSListStockContainer().GetSize() )
		m_nIndexStart -= m_nPageCount;
	UpdateButtons();
	Invalidate( );
}

void CQuoteTipDlg::OnPaint() 
{
	// Do not call CTipDialog::OnPaint() for painting messages
	// CTipDialog::OnPaint();

	CPaintDC dc(this); // device context for painting
	
	dc.SetBkMode( TRANSPARENT );
	CFont * pOldFont = AfxSelectDCFont(&dc,14);

	CRect rectAll;
	GetClientRect( &rectAll );
	int nWidthTotal = rectAll.Width();
	double o = (nWidthTotal-10) / 26;

	CRect rect;
	int x = 5, y = 5;
	int nCount = 0;
	CStockContainer & container = AfxGetSListStockContainer();

	// Draw Header Title
	{
		CString strCode = AfxGetVariantName(SLH_CODE,FALSE);
		CString strClose = AfxGetVariantName(SLH_CLOSE,FALSE);
		CString strDiffPercent = AfxGetVariantName(SLH_DIFFPERCENT,FALSE);
		CString strVolume = AfxGetVariantName(SLH_VOLUME,FALSE);

		dc.SetTextColor( AfxGetProfile().GetColor(CColorClass::clrTitle) );
		rect = CRect(x,y,x+6*o,y+m_nItemHeight-1);
		dc.DrawText( strCode, rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS );
		rect = CRect(x+6*o,y,x+13*o,y+m_nItemHeight-1);
		dc.DrawText( strClose, rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS );
		rect = CRect(x+13*o,y,x+19*o,y+m_nItemHeight-1);
		dc.DrawText( strDiffPercent, rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS );
		rect = CRect(x+19*o,y,rectAll.right-5,y+m_nItemHeight-1);
		dc.DrawText( strVolume, rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS );

		y += m_nItemHeight;
	}

	y += 2;

	for( int nIndex=m_nIndexStart; nIndex<container.GetSize(); nIndex++ )
	{
		nCount ++;
		if( nCount > m_nPageCount )
			break;

		CStockInfo & info = container.ElementAt(nIndex);
		CString strCode = AfxGetVariantDispString(SLH_CODE,info,NULL);
		CString strClose = AfxGetVariantDispString(SLH_CLOSE,info,NULL);
		CString strDiffPercent = AfxGetVariantDispString(SLH_DIFFPERCENT,info,NULL);
		CString strVolume = AfxGetVariantDispString(SLH_VOLUME,info,NULL);

		rect = CRect(x,y,x+6*o,y+m_nItemHeight-1);
		dc.SetTextColor( AfxGetVariantColor(SLH_CODE,info) );
		dc.DrawText( strCode, rect, DT_VCENTER | DT_RIGHT | DT_SINGLELINE | DT_WORD_ELLIPSIS );
		rect = CRect(x+6*o,y,x+13*o,y+m_nItemHeight-1);
		dc.SetTextColor( AfxGetVariantColor(SLH_CLOSE,info) );
		dc.DrawText( strClose, rect, DT_VCENTER | DT_RIGHT | DT_SINGLELINE | DT_WORD_ELLIPSIS );
		rect = CRect(x+13*o,y,x+18*o,y+m_nItemHeight-1);
		dc.SetTextColor( AfxGetVariantColor(SLH_DIFFPERCENT,info) );
		dc.DrawText( strDiffPercent, rect, DT_VCENTER | DT_RIGHT | DT_SINGLELINE | DT_WORD_ELLIPSIS );
		rect = CRect(x+18*o,y,rectAll.right-10,y+m_nItemHeight-1);
		dc.SetTextColor( AfxGetVariantColor(SLH_VOLUME,info) );
		dc.DrawText( strVolume, rect, DT_VCENTER | DT_RIGHT | DT_SINGLELINE | DT_WORD_ELLIPSIS );

		y += m_nItemHeight;
	}

	dc.SelectObject(pOldFont);
}

void CQuoteTipDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ClientToScreen( &point );
	CRect rect;
	m_btnPrior.GetWindowRect( &rect );
	rect.InflateRect( 5, 5, 5, 5 );
	if( rect.PtInRect(point) )
		return;
	m_btnNext.GetWindowRect( &rect );
	rect.InflateRect( 5, 5, 5, 5 );
	if( rect.PtInRect(point) )
		return;
	
	CTipDialog::OnLButtonDown(nFlags, point);
}
