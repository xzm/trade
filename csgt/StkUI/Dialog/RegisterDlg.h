#if !defined(AFX_REGISTERDLG_H__B470E0B0_3478_45FE_9595_1A22D0E97C94__INCLUDED_)
#define AFX_REGISTERDLG_H__B470E0B0_3478_45FE_9595_1A22D0E97C94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegisterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegisterDlg dialog
/***
	×¢²á¶Ô»°¿ò
*/
class CRegisterDlg : public CXFDialog
{
// Construction
public:
	CRegisterDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL	CheckEvaCode( );

// Dialog Data
	//{{AFX_DATA(CRegisterDlg)
	enum { IDD = IDD_REGISTER };
	CGuiButton	m_btnApplyforEva;
	CGuiButton	m_btnCancel;
	CGuiButton	m_btnHowtoReg;
	CGuiButton	m_btnCopySoftNo;
	CGuiEdit	m_editSoftNO;
	CGuiButton	m_btnOK;
	CStatic	m_staticRegister;
	CStatic	m_staticLeftDaysTitle;
	CStatic	m_staticLeftDays;
	CHyperLink	m_linkGoPurchase;
	CStatic	m_staticPicture;
	CString	m_strUserName;
	CString	m_strSoftNO;
	CString	m_strRegCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegisterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegisterDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnHowtoReg();
	afx_msg void OnCopySoftno();
	virtual void OnCancel();
	afx_msg void OnApplyforEva();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERDLG_H__B470E0B0_3478_45FE_9595_1A22D0E97C94__INCLUDED_)
