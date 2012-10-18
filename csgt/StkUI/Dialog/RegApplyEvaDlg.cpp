// RegApplyEvaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegApplyEvaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegApplyEvaDlg dialog


CRegApplyEvaDlg::CRegApplyEvaDlg(CWnd* pParent /*=NULL*/)
	: CXFDialog(CRegApplyEvaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegApplyEvaDlg)
	m_strUserName = _T("");
	m_strEmail = _T("");
	//}}AFX_DATA_INIT
}


void CRegApplyEvaDlg::DoDataExchange(CDataExchange* pDX)
{
	CXFDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegApplyEvaDlg)
	DDX_Control(pDX, IDC_COMBO_DISTRICT, m_cmbDistrict);
	DDX_Text(pDX, IDC_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EMAIL, m_strEmail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegApplyEvaDlg, CXFDialog)
	//{{AFX_MSG_MAP(CRegApplyEvaDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegApplyEvaDlg message handlers

BOOL CRegApplyEvaDlg::OnInitDialog() 
{
	CXFDialog::OnInitDialog();
	
	CheckRadioButton( IDC_RADIO_PERSONAL, IDC_RADIO_COMPANY, IDC_RADIO_PERSONAL );

	CSPStringArray	astr;
	AfxGetAllDistrict( astr );
	for( int i=0; i<astr.GetSize(); i++ )
	{
		int nItem = m_cmbDistrict.AddString( astr[i] );
		m_cmbDistrict.SetItemData( nItem, 0 );
	}
	m_cmbDistrict.SetCurSel( -1 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRegApplyEvaDlg::OnOK() 
{
	UpdateData( );

	m_strUserName.TrimLeft();
	m_strUserName.TrimRight();
	m_strEmail.TrimLeft();
	m_strEmail.TrimRight();

	if( m_strUserName.IsEmpty() )
	{
		AfxMessageBox( IDS_REGAPPLYEVA_NOUSERNAME );
		return;
	}

	int nSel = m_cmbDistrict.GetCurSel();
	if( CB_ERR == nSel || -1 == m_cmbDistrict.GetItemData(nSel) )
	{
		AfxMessageBox( IDS_REGAPPLYEVA_NOCOUNTRY );
		return;
	}

	if( m_strEmail.IsEmpty() )
	{
		AfxMessageBox( IDS_REGAPPLYEVA_NOEMAIL );
		return;
	}

	CString	strDistrict;
	m_cmbDistrict.GetLBText( nSel, strDistrict );

	CString	strClass = "Personal";
	CButton * pbtnOrganization = (CButton *)GetDlgItem(IDC_RADIO_ORGANIZATION);
	CButton * pbtnCompany = (CButton *)GetDlgItem(IDC_RADIO_COMPANY);
	if( pbtnOrganization && pbtnOrganization->GetCheck() )
		strClass = "Organization";
	else if( pbtnCompany && pbtnCompany->GetCheck() )
		strClass = "Company";

	BOOL bOK = FALSE;
	CString		strErrorMsg;

	{
		CWaitDlg	dlg(NULL,FALSE);
		bOK = AfxApplyForEvaluation( m_strUserName, strDistrict, strClass, m_strEmail, strErrorMsg );
		dlg.DestroyWindow();
	}

	if( bOK )
		AfxMessageBox( IDS_REGAPPLYEVA_APPLYOK );
	else
	{
		CString strTemp;
		strTemp.LoadString(IDS_REGAPPLYEVA_APPLYFAIL);
		AfxMessageBox( strTemp + "\n" + strErrorMsg );
	}

	CXFDialog::OnOK();
}
