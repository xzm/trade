// AlarmTipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\stkui.h"
#include "AlarmTipDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarmTipDlg dialog

CAlarmTipDlg & CAlarmTipDlg::GetInstance()
{
	static CAlarmTipDlg	s_alarmtip;
	if( !::IsWindow(s_alarmtip.GetSafeHwnd()) )
		VERIFY( s_alarmtip.Create( CAlarmTipDlg::IDD, CWnd::GetDesktopWindow() ) );
	return s_alarmtip;
}


CAlarmTipDlg::CAlarmTipDlg(CWnd* pParent /*=NULL*/)
	: CTipDialog(CAlarmTipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlarmTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nIndexStart	=	0;
	m_nPageCount	=	9;
	m_nItemHeight	=	15;
}

void CAlarmTipDlg::SetItemHeight( int nHeight )
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

void CAlarmTipDlg::Show( )
{
	m_nIndexStart	=	0;
	UpdateButtons( );
	CTipDialog::Show();
}
void CAlarmTipDlg::Hide( )
{
	CTipDialog::Hide();
}
void CAlarmTipDlg::ShowGradual( )
{
	m_nIndexStart	=	0;
	UpdateButtons( );
	CTipDialog::ShowGradual();
}
void CAlarmTipDlg::HideGradual( )
{
	CTipDialog::HideGradual();
}

void CAlarmTipDlg::UpdateButtons( )
{
	m_btnPrior.EnableWindow( m_nIndexStart > 0 );
	m_btnNext.EnableWindow( m_nIndexStart + m_nPageCount < AfxGetAlarmContainer().GetSize() );
}

void CAlarmTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CTipDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarmTipDlg)
	DDX_Control(pDX, IDC_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_PRIOR, m_btnPrior);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlarmTipDlg, CTipDialog)
	//{{AFX_MSG_MAP(CAlarmTipDlg)
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_PRIOR, OnPrior)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmTipDlg message handlers

BOOL CAlarmTipDlg::OnInitDialog() 
{
	CTipDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetItemHeight( );

	UpdateButtons();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlarmTipDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
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

void CAlarmTipDlg::OnPrior() 
{
	m_nIndexStart -= m_nPageCount;
	if( m_nIndexStart < 0 )
		m_nIndexStart = 0;
	UpdateButtons();
	Invalidate( );
}

void CAlarmTipDlg::OnNext() 
{
	m_nIndexStart += m_nPageCount;
	if( m_nIndexStart >= AfxGetAlarmContainer().GetSize() )
		m_nIndexStart -= m_nPageCount;
	UpdateButtons();
	Invalidate( );
}

void CAlarmTipDlg::OnPaint() 
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
	CAlarmContainer & container = AfxGetAlarmContainer();

	// Draw Header Title
	{
		CString strCode = AfxGetVariantName(SLH_CODE,FALSE);
		CString strDiffPercent = AfxGetVariantName(SLH_DIFFPERCENT,FALSE);
		CString strReason;
		strReason.LoadString( IDS_ALARMRESULT_REASON );

		dc.SetTextColor( AfxGetProfile().GetColor(CColorClass::clrTitle) );
		rect = CRect(x,y,x+6*o,y+m_nItemHeight-1);
		dc.DrawText( strCode, rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS );
		rect = CRect(x+6*o,y,x+14*o,y+m_nItemHeight-1);
		dc.DrawText( strDiffPercent, rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS );
		rect = CRect(x+14*o,y,rectAll.right-5,y+m_nItemHeight-1);
		dc.DrawText( strReason, rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS );

		y += m_nItemHeight;
	}

	y += 2;

	dc.SetTextColor( AfxGetProfile().GetColor(CColorClass::clrText) );
	for( int nIndex=m_nIndexStart; nIndex<container.GetSize(); nIndex++ )
	{
		nCount ++;
		if( nCount > m_nPageCount )
			break;

		ALARM & alarm = container.ElementAt(nIndex);
		CString strCode = alarm.report.m_szCode;
		CString strDiffPercent = "-";
		if( alarm.report.m_fLast > 1e-4 )
			strDiffPercent.Format( ".2f", alarm.report.m_fNew/alarm.report.m_fLast-1 );
		CString strReason = container.GetDescript( alarm );

		rect = CRect(x,y,x+6*o,y+m_nItemHeight-1);
		dc.DrawText( strCode, rect, DT_VCENTER | DT_RIGHT | DT_SINGLELINE | DT_WORD_ELLIPSIS );
		rect = CRect(x+6*o,y,x+14*o,y+m_nItemHeight-1);
		dc.DrawText( strDiffPercent, rect, DT_VCENTER | DT_RIGHT | DT_SINGLELINE | DT_WORD_ELLIPSIS );
		rect = CRect(x+14*o,y,rectAll.right-10,y+m_nItemHeight-1);
		dc.DrawText( strReason, rect, DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_WORD_ELLIPSIS );

		y += m_nItemHeight;
	}

	dc.SelectObject(pOldFont);
}

void CAlarmTipDlg::OnLButtonDown(UINT nFlags, CPoint point) 
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
