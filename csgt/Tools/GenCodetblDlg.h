#if !defined(AFX_GENCODETBLDLG_H__880D48EA_4494_4E36_BAA2_69D986527DC9__INCLUDED_)
#define AFX_GENCODETBLDLG_H__880D48EA_4494_4E36_BAA2_69D986527DC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenCodetblDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGenCodetblDlg dialog

class CGenCodetblDlg : public CXFDialog
{
// Construction
public:
	CGenCodetblDlg(CWnd* pParent = NULL);   // standard constructor

	void	StoreProfile( );
	void	LoadProfile( );

// Dialog Data
	//{{AFX_DATA(CGenCodetblDlg)
	enum { IDD = IDD_GENCODETBL };
	CButton	m_btnOK;
	CString	m_strSrcPath;
	CString	m_strDestPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenCodetblDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenCodetblDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExplorerSrc();
	afx_msg void OnExplorerDest();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENCODETBLDLG_H__880D48EA_4494_4E36_BAA2_69D986527DC9__INCLUDED_)
