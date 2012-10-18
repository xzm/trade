// TipDialog.h : header file
//

#if !defined(AFX_TIPDIALOG_H__4CEFAE42_FBA7_435F_8F4F_69D7C605BBE8__INCLUDED_)
#define AFX_TIPDIALOG_H__4CEFAE42_FBA7_435F_8F4F_69D7C605BBE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XFDialog.h"

#define TIMER_TIPAUTOHIDE		828
#define TIMER_TIPSHOWGRADUAL	829
#define TIMER_TIPHIDEGRADUAL	830

/////////////////////////////////////////////////////////////////////////////
// CTipDialog dialog

class CLASS_EXPORT CTipDialog : public CXFDialog
{
// Construction
public:
	CTipDialog();
	CTipDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	CTipDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);

	void	SetAutoHide( int nWaitSecs, BOOL bResetWhenMouseMove );

	virtual	void	Show( );
	virtual	void	Hide( );
	virtual	void	ShowGradual( );
	virtual	void	HideGradual( );

// Dialog Data
	//{{AFX_DATA(CTipDialog)
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipDialog)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COLORREF	m_clrBkground;
	CFont		m_font;

	int			m_nWaitSecs;
	BOOL		m_bResetWhenMouseMove;

	CPoint		GetNormalPos();

	// Generated message map functions
	//{{AFX_MSG(CTipDialog)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPDIALOG_H__4CEFAE42_FBA7_435F_8F4F_69D7C605BBE8__INCLUDED_)
