#if !defined(AFX_GENSERIALDLG_H__B2FA5D12_726C_4EEB_BA6E_CD1105D1A436__INCLUDED_)
#define AFX_GENSERIALDLG_H__B2FA5D12_726C_4EEB_BA6E_CD1105D1A436__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenSerialDlg.h : header file
//
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CGenSerialDlg dialog

class CGenSerialDlg : public CDialog
{
// Construction
public:
	CGenSerialDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGenSerialDlg)
	enum { IDD = IDD_GENSERIAL };
	CButton	m_btnSaveDispatch;
	CButton	m_btnSaveReg;
	CButton	m_btnOK;
	CButton	m_btnSave;
	CString	m_strUserName;
	CString	m_strSoftNO;
	CString	m_strRegCode;
	BOOL	m_bProfessional;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenSerialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenSerialDlg)
	virtual void OnOK();
	afx_msg void OnSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnSavereg();
	afx_msg void OnSavedispatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENSERIALDLG_H__B2FA5D12_726C_4EEB_BA6E_CD1105D1A436__INCLUDED_)
