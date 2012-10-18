#if !defined(AFX_REGAPPLYEVADLG_H__0FA9A12D_3AD2_4E92_A02D_16599F29595B__INCLUDED_)
#define AFX_REGAPPLYEVADLG_H__0FA9A12D_3AD2_4E92_A02D_16599F29595B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegApplyEvaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegApplyEvaDlg dialog

class CRegApplyEvaDlg : public CXFDialog
{
// Construction
public:
	CRegApplyEvaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRegApplyEvaDlg)
	enum { IDD = IDD_REGAPPLYEVA };
	CComboBox	m_cmbDistrict;
	CString	m_strUserName;
	CString	m_strEmail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegApplyEvaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegApplyEvaDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGAPPLYEVADLG_H__0FA9A12D_3AD2_4E92_A02D_16599F29595B__INCLUDED_)
