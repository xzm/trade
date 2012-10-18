#if !defined(AFX_GENBASEDLG_H__C6A430C3_4433_4FC1_A0B2_20C0E4B5E6F6__INCLUDED_)
#define AFX_GENBASEDLG_H__C6A430C3_4433_4FC1_A0B2_20C0E4B5E6F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenBaseDlg.h : header file
//

#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CGenBaseDlg dialog

#pragma pack(1)

typedef	struct	stockex263_header_t	{
	char	temp[0x1A8A];	//	0x278
} STOCKEX263_HEADER, *PSTOCKEX263_HEADER;

typedef	struct	stockex263_t	{
	char	code[8];
	char	name[20];
	LONG	dw1;
	LONG	dw2;
	LONG	dw3;
	LONG	dw4;
	LONG	dw5;
	LONG	dw6;
	LONG	dw7;
	LONG	dw8;
	LONG	dw9;
	LONG	dw10;
	LONG	dw11;
	LONG	dw12;
	LONG	dw13;
	LONG	dw14;
	LONG	dw15;
	LONG	dw16;
	LONG	dw17;
	LONG	dw18;
	LONG	dw19;
	LONG	dw20;
	LONG	dw21;
	LONG	dw22;
	LONG	dw23;
	LONG	dw24;
	LONG	dw25;
	LONG	dw26;	//	Á÷Í¨¹É
	LONG	dw27;
	LONG	dw28;
	char	province[10];
	char	domain[8];
	LONG	dw29;
	LONG	dw30;
	LONG	dw31;
	LONG	dw32;
	LONG	dw33;
	LONG	dw34;
	LONG	dw35;
	LONG	dw36;
	LONG	dw37;
	LONG	dw38;
} STOCKEX263, * PSTOCKEX263;

#pragma pack()

class CGenBaseDlg : public CDialog
{
// Construction
public:
	CGenBaseDlg(CWnd* pParent = NULL);   // standard constructor

	CString	m_strDomain_class;
	CString	m_strNetasset_ps;
	CString	m_strNetasset_ps_regulate;
	CString	m_strEps;
	CString	m_strEps_deduct;
	CString	m_strNet_profit;
	CString	m_strMain_profit;
	CString	m_strTotal_profit;
	CString	m_strRatio_liquidity;
	CString	m_strRatio_quick;
	CString	m_strVelocity_receivables;
	CString	m_strVelocity_merchandise;
	CString	m_strMain_income;
	CString	m_strCash_ps;
	CString	m_strProfit_margin;
	CString	m_strNetasset_yield;
	CString	m_strDate_begin;
	CString	m_strDate_begin2;
	CString	m_strShare_count_total;
	CString	m_strShare_count_a;
	CString	m_strShare_count_b;
	CString	m_strShare_count_h;
	CString	m_strShare_count_national;
	CString	m_strShare_count_corp;
	CString	m_strProfit_psud;
	CString	m_strAsset;
	CString	m_strRatio_holderright;
	CString	m_strRatio_longdebt;
	CString	m_strRatio_debt;
	CString	m_strProfit_inc;
	CString	m_strIncome_inc;
	CString	m_strAsset_inc;
	CString	m_strFund_Netvalue;
	CString	m_strFund_ShareCountTotal;
	CString	m_strFund_ShareCountCurrency;
	CString	m_strRefresh;
	CString	m_strReportAnn;
	CString	m_strReportMid;
	CString	m_strReportQuarter;

// Dialog Data
	//{{AFX_DATA(CGenBaseDlg)
	enum { IDD = IDD_GENBASE };
	CButton	m_btnOK;
	CString	m_strDestFile;
	CString	m_strSrcPath;
	BOOL	m_bNewFile;
	CSPTime	m_tmDate;
	CString	m_str263SrcFile;
	CString	m_strErateDollar;
	CString	m_strErateHKDollar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenBaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	StoreProfile( );
	void	LoadProfile( );

	// Generated message map functions
	//{{AFX_MSG(CGenBaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExplorerQlsrc();
	afx_msg void OnExplorerDest();
	virtual void OnOK();
	afx_msg void OnRadioAnn();
	afx_msg void OnRadioMid();
	afx_msg void OnRadioQuarter();
	afx_msg void OnExplorer263src();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENBASEDLG_H__C6A430C3_4433_4FC1_A0B2_20C0E4B5E6F6__INCLUDED_)
