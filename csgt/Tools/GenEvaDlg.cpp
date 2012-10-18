// GenEvaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tools.h"
#include "GenEvaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenEvaDlg dialog


CGenEvaDlg::CGenEvaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGenEvaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenEvaDlg)
	m_strRegCode = _T("");
	m_time = 0;
	//}}AFX_DATA_INIT

	m_time	=	CSPTime::GetCurrentTime();
	m_time	+=	CSPTimeSpan( 365*2, 0, 0, 0 );
}

void CGenEvaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenEvaDlg)
	DDX_Text(pDX, IDC_REGCODE, m_strRegCode);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME, m_time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenEvaDlg, CDialog)
	//{{AFX_MSG_MAP(CGenEvaDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenEvaDlg message handlers

BOOL CGenEvaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGenEvaDlg::OnOK() 
{
	UpdateData( );

	m_strRegCode.Empty();

	CSPString strRegCode;
	if( AfxGetSView().DumpEva( m_time.GetTime(), strRegCode ) )
		m_strRegCode = strRegCode;

	UpdateData( FALSE );
}
