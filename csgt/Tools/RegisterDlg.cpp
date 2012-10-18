// RegisterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegisterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegisterDlg dialog


CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CRegisterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegisterDlg)
	m_strUserName = _T("");
	m_strSoftNO = _T("");
	m_strRegCode = _T("");
	//}}AFX_DATA_INIT
}


void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegisterDlg)
	DDX_Control(pDX, IDC_SOFTNO, m_editSoftNO);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_STATIC_REGISTER, m_staticRegister);
	DDX_Control(pDX, IDC_LEFTDAYSTITLE, m_staticLeftDaysTitle);
	DDX_Control(pDX, IDC_LEFTDAYS, m_staticLeftDays);
	DDX_Control(pDX, IDC_GOPURCHASE, m_linkGoPurchase);
	DDX_Control(pDX, IDC_PICTURE, m_staticPicture);
	DDX_Text(pDX, IDC_USERNAME, m_strUserName);
	DDV_MaxChars(pDX, m_strUserName, SE_MAX_USERNAME);
	DDX_Text(pDX, IDC_SOFTNO, m_strSoftNO);
	DDX_Text(pDX, IDC_REGCODE, m_strRegCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CXFDialog)
	//{{AFX_MSG_MAP(CRegisterDlg)
	ON_BN_CLICKED(IDC_HOWTO_REG, OnHowtoReg)
	ON_BN_CLICKED(IDC_COPY_SOFTNO, OnCopySoftno)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegisterDlg message handlers

BOOL CRegisterDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	BOOL	bEvaluate	=	AfxGetSView().IsEva();
	if( bEvaluate )
	{
		CString	strLeftDays;
		if( AfxGetSView().GetLeft() > AfxGetSView().GetMaxEvaDays() )
			strLeftDays.LoadString( IDS_REGISTER_NOTIMELIMIT );
		else
			strLeftDays.Format( "%d", AfxGetSView().GetLeft() );
		m_staticLeftDays.SetWindowText( strLeftDays );
		m_linkGoPurchase.SetURL( AfxGetProfile().GetPurchaseURL() );
	}
	else
	{
		CString	strPurchased;
		strPurchased.LoadString( IDS_REGISTER_PURCHASED );
		m_staticLeftDaysTitle.SetWindowText( strPurchased );

		m_staticLeftDays.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW );
		m_linkGoPurchase.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW );

		m_strUserName	=	AfxGetSView().GetU();
		m_strRegCode	=	AfxGetSView().GetR();
	}

	m_strSoftNO	=	CSView::GetS();

	UpdateData( FALSE );

	CString	strInfo;
	if( bEvaluate && AfxGetSView().GetLeft() <= 0 )
	{
		strInfo.LoadString( IDS_EVALUATEINFO );
		AfxMessageBox( strInfo, MB_OK | MB_ICONINFORMATION );
	}

	AfxGetSView().Store( AfxGetProfile().GetSViewFile() );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRegisterDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_btnOK.EnableWindow( FALSE );
	UpdateData( );

	m_strUserName.TrimLeft();
	m_strUserName.TrimRight();
	m_strRegCode.TrimLeft();
	m_strRegCode.TrimRight();

	if( m_strUserName.GetLength() < SE_MIN_USERNAME
		|| m_strUserName.GetLength() > SE_MAX_USERNAME )
	{
		CString	strTooShort, strTooShortfmt;
		strTooShortfmt.LoadString( IDS_USERNAMETOOSHORTFMT );
		strTooShort.Format( strTooShortfmt, SE_MIN_USERNAME, SE_MAX_USERNAME );
		AfxMessageBox( strTooShort, MB_OK | MB_ICONINFORMATION );
		m_btnOK.EnableWindow( TRUE );
		return;
	}

	if( m_strRegCode.GetLength() < SE_MIN_RSASYSSTR )
	{
		AfxMessageBox( IDS_REGCODETOOSHORT, MB_OK | MB_ICONINFORMATION );
		m_btnOK.EnableWindow( TRUE );
		return;
	}

	AfxAssertNull( );
	if( ! AfxGetSView().SetUR( m_strUserName, m_strRegCode ) )
	{
		AfxAssertNull( );
		AfxMessageBox( IDS_INVALIDREGCODE, MB_OK | MB_ICONINFORMATION );
		m_staticRegister.SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE );
		m_btnOK.EnableWindow( TRUE );
		return;
	}

	AfxAssertNull( );

	CString	strSuccess;
	strSuccess.LoadString( IDS_REGISTER_SUCCESS );
	m_staticRegister.SetWindowPos( NULL, 0,0,0,0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);
	m_staticRegister.SetWindowText( strSuccess );
	CString	strPurchased;
	strPurchased.LoadString( IDS_REGISTER_PURCHASED );
	m_staticLeftDaysTitle.SetWindowText( strPurchased );
	m_staticLeftDays.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW );
	m_linkGoPurchase.SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW );
	m_btnOK.EnableWindow( TRUE );

	AfxGetSView().Store( AfxGetProfile().GetSViewFile() );
}

void CRegisterDlg::OnHowtoReg() 
{
	::HtmlHelp(NULL, "StockAna.chm::/htm/purchase.htm", HH_DISPLAY_TOPIC, 0);
}

void CRegisterDlg::OnCopySoftno() 
{
	m_editSoftNO.Copy( );
}
