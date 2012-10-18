// DeclareDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DeclareDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeclareDlg dialog


CDeclareDlg::CDeclareDlg(CWnd* pParent /*=NULL*/)
	: CXFResDialog(CDeclareDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeclareDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDeclareDlg::~CDeclareDlg()
{
}

BOOL CDeclareDlg::SetBitmap( HBITMAP hbmp )
{
	return m_bitmap.Attach( hbmp );
}

BOOL CDeclareDlg::SetDeclareText( LPCTSTR lpszText )
{
	m_text	=	lpszText;
	return m_text.GetLength()>0;
}

void CDeclareDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFResDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeclareDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_XFEDIT_DECLARE, m_editDeclare);
	DDX_Control(pDX, IDC_XFPICTURE, m_staticPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeclareDlg, CXFResDialog)
	//{{AFX_MSG_MAP(CDeclareDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeclareDlg message handlers

BOOL CDeclareDlg::OnInitDialog() 
{
	CXFResDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_staticPicture.SetBitmap( (HBITMAP)m_bitmap.GetSafeHandle() );

/*
	HGLOBAL		hRead = NULL;
	HRSRC		hReSrc = ::FindResource( AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_TEXT_DECLARE), _T("Text") );
	if( hReSrc )
		hRead	=	::LoadResource( AfxGetResourceHandle(), hReSrc );
	if( hRead )
	{
		LPCTSTR	lpText	=	(LPCTSTR)::LockResource( hRead );
		if( lpText )
			m_editDeclare.SetWindowText( lpText );
		::UnlockResource( hRead );
	}
*/
	m_editDeclare.SetWindowText(m_text);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
