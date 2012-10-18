#if !defined(AFX_GENEVADLG_H__F84EEE6D_4BEE_44C3_87E6_58106B40E28D__INCLUDED_)
#define AFX_GENEVADLG_H__F84EEE6D_4BEE_44C3_87E6_58106B40E28D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenEvaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGenEvaDlg dialog

class CGenEvaDlg : public CDialog
{
// Construction
public:
	CGenEvaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGenEvaDlg)
	enum { IDD = IDD_GENEVA };
	CString	m_strRegCode;
	CSPTime	m_time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenEvaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenEvaDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENEVADLG_H__F84EEE6D_4BEE_44C3_87E6_58106B40E28D__INCLUDED_)
