#if !defined(AFX_FILETIMEDLG_H__07F79BB5_8F24_4996_922E_7FCA25199252__INCLUDED_)
#define AFX_FILETIMEDLG_H__07F79BB5_8F24_4996_922E_7FCA25199252__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FiletimeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFiletimeDlg dialog
#include	"Resource.h"

class CFiletimeDlg : public CXFDialog
{
// Construction
public:
	CFiletimeDlg(CWnd* pParent = NULL);   // standard constructor

	void	StoreProfile( );
	void	LoadProfile( );

// Dialog Data
	//{{AFX_DATA(CFiletimeDlg)
	enum { IDD = IDD_FILETIME };
	CButton	m_btnOK;
	CString	m_strSrcPath;
	CTime	m_tmCreate;
	CTime	m_tmAccess;
	CTime	m_tmWrite;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFiletimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFiletimeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExplorerSrc();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETIMEDLG_H__07F79BB5_8F24_4996_922E_7FCA25199252__INCLUDED_)
