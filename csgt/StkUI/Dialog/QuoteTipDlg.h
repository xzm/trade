#if !defined(AFX_QUOTETIPDLG_H__E8F078CD_1996_44BC_B4A8_7C704A774975__INCLUDED_)
#define AFX_QUOTETIPDLG_H__E8F078CD_1996_44BC_B4A8_7C704A774975__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuoteTipDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQuoteTipDlg dialog

class CQuoteTipDlg : public CTipDialog
{
// Construction
public:
	CQuoteTipDlg(CWnd* pParent = NULL);   // standard constructor

	static CQuoteTipDlg & GetInstance();

	void	SetItemHeight( int nHeight = 15 );

	virtual	void	Show( );
	virtual	void	Hide( );
	virtual	void	ShowGradual( );
	virtual	void	HideGradual( );

// Dialog Data
	//{{AFX_DATA(CQuoteTipDlg)
	enum { IDD = IDD_QUOTETIP };
	CGuiButton	m_btnNext;
	CGuiButton	m_btnPrior;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuoteTipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	UpdateButtons( );

	// Generated message map functions
	//{{AFX_MSG(CQuoteTipDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPrior();
	afx_msg void OnNext();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int		m_nIndexStart;
	int		m_nPageCount;
	int		m_nItemHeight;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUOTETIPDLG_H__E8F078CD_1996_44BC_B4A8_7C704A774975__INCLUDED_)
