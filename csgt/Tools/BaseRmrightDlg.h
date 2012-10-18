#if !defined(AFX_BASERMRIGHTDLG_H__DFA6CF03_991E_43D1_A99A_B16D7B54EC0F__INCLUDED_)
#define AFX_BASERMRIGHTDLG_H__DFA6CF03_991E_43D1_A99A_B16D7B54EC0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseRmrightDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseRmrightDlg dialog

class CBaseRmrightDlg : public CXFDialog
{
// Construction
public:
	CBaseRmrightDlg(CWnd* pParent = NULL);   // standard constructor

	void	StoreProfile( );
	void	LoadProfile( );

// Dialog Data
	//{{AFX_DATA(CBaseRmrightDlg)
	enum { IDD = IDD_BASERMRIGHT };
	CButton	m_btnOK;
	CString	m_strSrcPath;
	CString	m_strDestPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseRmrightDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBaseRmrightDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExplorerSrc();
	afx_msg void OnExplorerDest();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASERMRIGHTDLG_H__DFA6CF03_991E_43D1_A99A_B16D7B54EC0F__INCLUDED_)
