// TipDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TipDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTipDialog dialog

CTipDialog::CTipDialog() : CXFDialog()
{
	m_clrBkground	=	RGB( 96, 64, 255 );
	m_nWaitSecs		=	5;
	m_bResetWhenMouseMove	=	FALSE;
}

CTipDialog::CTipDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
	: CXFDialog(lpszTemplateName, pParentWnd)
{
	m_clrBkground	=	RGB( 96, 64, 255 );
	m_nWaitSecs		=	5;
	m_bResetWhenMouseMove	=	FALSE;
}

CTipDialog::CTipDialog(UINT nIDTemplate, CWnd* pParentWnd)
	: CXFDialog(nIDTemplate,pParentWnd)
{
	m_clrBkground	=	RGB( 96, 64, 255 );
	m_nWaitSecs		=	5;
	m_bResetWhenMouseMove	=	FALSE;
}

CPoint CTipDialog::GetNormalPos()
{
	ASSERT( ::IsWindow(GetSafeHwnd()) );
	CRect rectDlg;
	GetWindowRect( &rectDlg );
	int x = ::GetSystemMetrics(SM_CXSCREEN) - rectDlg.Width() - 2;
	int y = ::GetSystemMetrics(SM_CYSCREEN) - rectDlg.Height() - ::GetSystemMetrics(SM_CYCAPTION) - 5;
	return CPoint(x,y);
}

void CTipDialog::SetAutoHide( int nWaitSecs, BOOL bResetWhenMouseMove )
{
	m_nWaitSecs = nWaitSecs;
	m_bResetWhenMouseMove = bResetWhenMouseMove;

	SetTimer( TIMER_TIPAUTOHIDE, m_nWaitSecs*1000, NULL );
}

void CTipDialog::Show( )
{
	CPoint pt = GetNormalPos();
	SetWindowPos( &CWnd::wndTopMost, pt.x, pt.y, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOACTIVATE );
}

void CTipDialog::Hide( )
{
	CPoint pt = GetNormalPos();
	SetWindowPos( &CWnd::wndTopMost, pt.x, pt.y, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE );
}

void CTipDialog::ShowGradual( )
{
	KillTimer( TIMER_TIPSHOWGRADUAL );
	KillTimer( TIMER_TIPHIDEGRADUAL );

	CPoint pt = GetNormalPos();
	CRect rectDlg;
	GetWindowRect( &rectDlg );
	int y = pt.y + rectDlg.Height();
	SetWindowPos( &CWnd::wndTopMost, pt.x, y, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOACTIVATE );
	SetTimer( TIMER_TIPSHOWGRADUAL, 12, NULL );
}

void CTipDialog::HideGradual( )
{
	KillTimer( TIMER_TIPSHOWGRADUAL );
	KillTimer( TIMER_TIPHIDEGRADUAL );
	SetTimer( TIMER_TIPHIDEGRADUAL, 12, NULL );
}

void CTipDialog::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTipDialog)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTipDialog, CXFDialog)
	//{{AFX_MSG_MAP(CTipDialog)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTipDialog message handlers

BOOL CTipDialog::OnInitDialog()
{
	CXFDialog::OnInitDialog();

	if( !m_font.CreateStockObject( DEFAULT_GUI_FONT ) )
		m_font.CreateStockObject( ANSI_VAR_FONT );
	SetFont( &m_font );

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	Hide();

	SetAutoHide( 5, TRUE );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTipDialog::OnTimer(UINT nIDEvent) 
{
	CXFDialog::OnTimer(nIDEvent);

	if( TIMER_TIPAUTOHIDE == nIDEvent )
	{
		KillTimer( TIMER_TIPAUTOHIDE );
		HideGradual();
	}
	else if( TIMER_TIPHIDEGRADUAL == nIDEvent )
	{
		WINDOWPLACEMENT place;
		if( IsWindowVisible() && GetWindowPlacement( &place ) )
		{
			CPoint pt = GetNormalPos();
			int x = pt.x;
			int y = place.rcNormalPosition.top;
			y += 20;
			if( y >= GetSystemMetrics(SM_CYSCREEN) )
			{
				KillTimer( TIMER_TIPHIDEGRADUAL );
				SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE );
			}
			else
				SetWindowPos( &CWnd::wndTopMost, x, y, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOACTIVATE );
		}
		else
		{
			KillTimer( TIMER_TIPHIDEGRADUAL );
			SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE );
		}
	}
	else if( TIMER_TIPSHOWGRADUAL == nIDEvent )
	{
		WINDOWPLACEMENT place;
		if( GetWindowPlacement( &place ) )
		{
			CPoint pt = GetNormalPos();
			int x = pt.x;
			int y = place.rcNormalPosition.top;
			if( y >= pt.y )
				y -= 20;
			if( y <= pt.y )
			{
				y = pt.y;
				KillTimer( TIMER_TIPSHOWGRADUAL );
			}
			SetWindowPos( &CWnd::wndTopMost, x, y, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOACTIVATE );
		}
		else
			KillTimer( TIMER_TIPSHOWGRADUAL );
	}
}

BOOL CTipDialog::OnEraseBkgnd(CDC* pDC)
{
	CBrush	*pOldBrush = (CBrush *)pDC->SelectStockObject( NULL_BRUSH );
	CBrush	brush;
	brush.CreateSolidBrush( RGB( 0, 0, 128 ) );
	CRect	rect;
	GetClientRect( rect );
	
	BYTE	r	=	GetRValue( m_clrBkground );
	BYTE	g	=	GetGValue( m_clrBkground );
	BYTE	b	=	GetBValue( m_clrBkground );

	BYTE	bGray	=	BYTE( ( r*30L + g*59L + b*11L )/100 );
	if( bGray <= 128 )
	{
		r	=	255 - r;
		g	=	255 - g;
		b	=	255 - b;
	}
	
	int nNumColors = pDC->GetDeviceCaps( NUMCOLORS );
	if( nNumColors > 256 || -1 == nNumColors )
	{
		for( int i=255; i>=128; i-=3 )
		{
			brush.DeleteObject();
			
			if( bGray <= 128 )
				brush.CreateSolidBrush( RGB( 
					255 - MulDiv( r, i, 255),
					255 - MulDiv( g, i, 255),
					255 - MulDiv( b, i, 255) ) );
			else
				brush.CreateSolidBrush( RGB( 
					MulDiv( r, i, 255),
					MulDiv( g, i, 255),
					MulDiv( b, i, 255) ) );

			pDC->FillRect( CRect( rect.left + MulDiv( rect.Width(), (255-i+3), 128), 0, 
				rect.left + MulDiv( rect.Width(), (255-i), 128), rect.bottom),
				&brush );
		}
	}
	else
	{
		brush.DeleteObject( );
		brush.CreateSolidBrush( m_clrBkground ); //pDC->GetNearestColor( m_clrBkground ) );
		pDC->FillRect( rect, &brush );
	}
	pDC->SelectStockObject( NULL_BRUSH );
	GetClientRect( rect );	
	pDC->SelectObject( pOldBrush );
	return TRUE;
}

int CTipDialog::DoModal() 
{
	return CXFDialog::DoModal();
}

void CTipDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CXFDialog::OnLButtonDown(nFlags, point);
	HideGradual();
}

void CTipDialog::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CXFDialog::OnRButtonDown(nFlags, point);
	HideGradual();
}

void CTipDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( m_bResetWhenMouseMove )
	{
		KillTimer( TIMER_TIPAUTOHIDE );
		SetTimer( TIMER_TIPAUTOHIDE, m_nWaitSecs*1000, NULL );
	}
	CXFDialog::OnMouseMove(nFlags, point);
}

BOOL CTipDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if( m_bResetWhenMouseMove )
	{
		KillTimer( TIMER_TIPAUTOHIDE );
		SetTimer( TIMER_TIPAUTOHIDE, m_nWaitSecs*1000, NULL );
	}
	return CXFDialog::OnMouseWheel(nFlags, zDelta, pt);
}

