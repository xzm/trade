#if !defined(AFX_GENNAMEENUDLG_H__880D48EA_4494_4E36_BAA2_69D986527DC9__INCLUDED_)
#define AFX_GENNAMEENUDLG_H__880D48EA_4494_4E36_BAA2_69D986527DC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenNameEnuDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGenNameEnuDlg dialog

class CGenNameEnuDlg : public CXFDialog
{
// Construction
public:
	CGenNameEnuDlg(CWnd* pParent = NULL);   // standard constructor

	void	StoreProfile( );
	void	LoadProfile( );

// Dialog Data
	//{{AFX_DATA(CGenNameEnuDlg)
	enum { IDD = IDD_GENNAMEENU };
	CButton	m_btnOK;
	CString	m_strSrcPath;
	CString	m_strDestPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenNameEnuDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenNameEnuDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExplorerSrc();
	afx_msg void OnExplorerDest();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENNAMEENUDLG_H__880D48EA_4494_4E36_BAA2_69D986527DC9__INCLUDED_)
