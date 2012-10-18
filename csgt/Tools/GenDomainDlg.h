#if !defined(AFX_GENDOMAINDLG_H__0BC3ECAE_6899_48BD_81B9_D22145CA581E__INCLUDED_)
#define AFX_GENDOMAINDLG_H__0BC3ECAE_6899_48BD_81B9_D22145CA581E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenDomainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGenDomainDlg dialog

class CGenDomainDlg : public CXFDialog
{
// Construction
public:
	CGenDomainDlg(CWnd* pParent = NULL);   // standard constructor

	void	StoreProfile( );
	void	LoadProfile( );

// Dialog Data
	//{{AFX_DATA(CGenDomainDlg)
	enum { IDD = IDD_GENDOMAIN };
	CButton	m_btnOK;
	CString	m_strSrcPath;
	CString	m_strDestFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenDomainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenDomainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExplorerSrc();
	afx_msg void OnExplorerDest();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENDOMAINDLG_H__0BC3ECAE_6899_48BD_81B9_D22145CA581E__INCLUDED_)
