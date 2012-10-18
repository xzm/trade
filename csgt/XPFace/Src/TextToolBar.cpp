// TextToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "TextToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTextToolBar

#define		TEXTBAR_BMPWIDTH			16
#define		TEXTBAR_BMPHEIGHT			15
#define		TEXTBAR_WIDTH_MIN			23
#define		TEXTBAR_WIDTH_MAX			65
#define		TEXTBAR_WIDTH				TEXTBAR_WIDTH_MIN
#define		TEXTBAR_HEIGHT				22
#define		TEXTBAR_MAXTEXTROWS			2

IMPLEMENT_DYNCREATE(CTextToolBar, CGuiToolBarWnd)

CTextToolBar::CTextToolBar()
{
}

CTextToolBar::~CTextToolBar()
{
}

BOOL CTextToolBar::CreateTextToolBar( CWnd * pParentWnd, UINT nBarID,
							UINT nIDToolBar, UINT nIDNormal, UINT nIDToolHot,
							int	nTextLabel,
							DWORD dwCtrlStyle, DWORD dwStyle )
{
	ASSERT( pParentWnd );

	// if you don't want button border, use this
	// if you use CGuiToolBarWnd as father class, use this
	if (!CreateEx(pParentWnd, dwCtrlStyle, dwStyle, CRect(0,0,0,0), nBarID  ) )
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;      // fail to create
	}

/*
	// if you use CToolBar as father class, use this
	if (!Create( pParentWnd, dwStyle, nBarID ) )
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;      // fail to create
	}
*/
	SetOwner( pParentWnd );

	if(	!LoadToolBar( nIDToolBar ) )
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;      // fail to create
	}

	CToolBarCtrl&	toolctrl = GetToolBarCtrl();

	if( -1 != nIDNormal )
	{
		HBITMAP	hbmp	=	(HBITMAP)::LoadImage( AfxGetResourceHandle(), MAKEINTRESOURCE(nIDNormal), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION );
		if( hbmp )
		{
			VERIFY( m_imageToolNormal.Create( TEXTBAR_BMPWIDTH, TEXTBAR_BMPHEIGHT, ILC_MASK|ILC_COLOR8, 0, 0 ) );
			VERIFY( -1 != m_imageToolNormal.Add( CBitmap::FromHandle(hbmp),  RGB(192,192,192) ) );
			DeleteObject( hbmp );
			toolctrl.SetImageList( &m_imageToolNormal );
		}
	}
	if( -1 != nIDToolHot )
	{
		HBITMAP	hbmp	=	(HBITMAP)::LoadImage( AfxGetResourceHandle(), MAKEINTRESOURCE(nIDToolHot), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION );
		if( hbmp )
		{
			VERIFY( m_imageToolHot.Create( TEXTBAR_BMPWIDTH, TEXTBAR_BMPHEIGHT, ILC_MASK|ILC_COLOR8, 0, 0 ) );
			VERIFY( -1 != m_imageToolHot.Add( CBitmap::FromHandle(hbmp),  RGB(192,192,192) ) );
			DeleteObject( hbmp );
			toolctrl.SetHotImageList( &m_imageToolHot );
		}
	}

	toolctrl.SetExtendedStyle( TBSTYLE_EX_DRAWDDARROWS );

	SetTextLabelStatus( nTextLabel );

	return TRUE;
}

BOOL CTextToolBar::IsValidStatus( int nTextLabel )
{
	return (labelBottom == nTextLabel || labelRight == nTextLabel || labelNone == nTextLabel );
}

BOOL CTextToolBar::IsTextLabelVisible( )
{
	return (labelNone != m_nTextLabel);
}

BOOL CTextToolBar::SetTextLabelStatus( int nTextLabel )
{
	if( ! IsValidStatus( nTextLabel ) )
		return FALSE;

	CToolBarCtrl&	toolctrl = GetToolBarCtrl();

	for( int i=0; i<toolctrl.GetButtonCount(); i++ )
	{
		if( GetButtonStyle(i) == TBSTYLE_BUTTON )
		{
			CString	string;
			string.LoadString( GetItemID(i) );
			int	nFind	=	string.Find('\n',0);
			if( nFind > 0 )
				string	=	string.Right(string.GetLength()-nFind-1);
			if( labelNone == nTextLabel )
				string.Empty();
			SetButtonText( i, string );
		}
	}

	
	int	nWidth	=	TEXTBAR_WIDTH;
	int	nHeight	=	TEXTBAR_HEIGHT;
	CRect	rect(0,0,nWidth,nHeight);

	if( labelBottom == nTextLabel )
	{
		toolctrl.ModifyStyle(TBSTYLE_LIST, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_ALTDRAG );
		toolctrl.SetMaxTextRows( TEXTBAR_MAXTEXTROWS );
		for( int k=0; k<GetCount(); k++ )
		{
			if( GetItemID(k) != ID_SEPARATOR )
			{
				GetItemRect( k, &rect );
				break;
			}
		}
		nWidth	=	rect.Width();
		nHeight	=	rect.Height();
		m_nTextLabel	=	nTextLabel;
	}
	else if( labelRight == nTextLabel )
	{
		toolctrl.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TRANSPARENT | TBSTYLE_ALTDRAG);
		toolctrl.SetMaxTextRows( TEXTBAR_MAXTEXTROWS );
		for( int k=0; k<GetCount(); k++ )
		{
			if( GetItemID(k) != ID_SEPARATOR )
			{
				GetItemRect( k, &rect );
				break;
			}
		}
		nWidth	=	rect.Width();
		m_nTextLabel	=	nTextLabel;
	}
	else
	{
		toolctrl.ModifyStyle(TBSTYLE_LIST, TBSTYLE_FLAT| TBSTYLE_TRANSPARENT | TBSTYLE_ALTDRAG );
		toolctrl.SetMaxTextRows( 0 );
		m_nTextLabel	=	labelNone;
	}
	
	SetSizes( CSize(nWidth, nHeight), CSize(TEXTBAR_BMPWIDTH,TEXTBAR_BMPHEIGHT) );

	CSize size = CalcFixedLayout( FALSE, TRUE );
	SetWindowPos( NULL, 0, 0, size.cx, size.cy,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );

	return	TRUE;
}

int CTextToolBar::GetTextLabelStatus( )
{ 
	if( IsValidStatus( m_nTextLabel ) )
		return m_nTextLabel;
	return labelNone;
}

void CTextToolBar::ToggleTextLabel( int nTextLabel )
{
	if( labelBottom == nTextLabel )
	{
		if( labelBottom == GetTextLabelStatus() )
		{
			SetTextLabelStatus( labelNone );
		}
		else if( labelRight == GetTextLabelStatus() )
		{
			SetTextLabelStatus( labelNone );
			SetTextLabelStatus( labelBottom );
		}
		else
		{
			SetTextLabelStatus( labelBottom );
		}
	}
	else if( labelRight == nTextLabel )
	{
		if( labelRight == GetTextLabelStatus() )
		{
			SetTextLabelStatus( labelNone );
		}
		else if( labelBottom == GetTextLabelStatus() )
		{
			SetTextLabelStatus( labelNone );
			SetTextLabelStatus( labelRight );
		}
		else
		{
			SetTextLabelStatus( labelRight );
		}
	}
	else
	{
		SetTextLabelStatus( labelNone );
	}
}

BEGIN_MESSAGE_MAP(CTextToolBar, CGuiToolBarWnd)
	//{{AFX_MSG_MAP(CTextToolBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextToolBar message handlers



