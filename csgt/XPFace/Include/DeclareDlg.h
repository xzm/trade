#if !defined(AFX_DECLAREDLG_H__64C20803_D579_4506_85E5_9A716D95CC60__INCLUDED_)
#define AFX_DECLAREDLG_H__64C20803_D579_4506_85E5_9A716D95CC60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeclareDlg.h : header file
//
#include	"../Src/Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDeclareDlg dialog
/***
	ÉùÃ÷¶Ô»°¿ò
*/
class CLASS_EXPORT CDeclareDlg : public CXFResDialog
{
// Construction
public:
	CDeclareDlg(CWnd* pParent = NULL);   // standard constructor
	~CDeclareDlg();

	BOOL	SetBitmap( HBITMAP hbmp );
	BOOL	SetDeclareText( LPCTSTR lpszText );

// Dialog Data
	//{{AFX_DATA(CDeclareDlg)
	enum { IDD = IDD_XFDECLARE };
	CGuiButton	m_btnOK;
	CEdit	m_editDeclare;
	CStatic	m_staticPicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeclareDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeclareDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CBitmap		m_bitmap;
	CString		m_text;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECLAREDLG_H__64C20803_D579_4506_85E5_9A716D95CC60__INCLUDED_)
