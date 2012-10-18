#if !defined(AFX_ALARMTIPDLG_H__6F138238_7A06_4F23_97BB_440F2D679D8F__INCLUDED_)
#define AFX_ALARMTIPDLG_H__6F138238_7A06_4F23_97BB_440F2D679D8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmTipDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlarmTipDlg dialog

class CAlarmTipDlg : public CTipDialog
{
// Construction
public:
	CAlarmTipDlg(CWnd* pParent = NULL);   // standard constructor

	static CAlarmTipDlg & GetInstance();

	void	SetItemHeight( int nHeight = 15 );

	virtual	void	Show( );
	virtual	void	Hide( );
	virtual	void	ShowGradual( );
	virtual	void	HideGradual( );

// Dialog Data
	//{{AFX_DATA(CAlarmTipDlg)
	enum { IDD = IDD_ALARMTIP };
	CGuiButton	m_btnNext;
	CGuiButton	m_btnPrior;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlarmTipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	UpdateButtons( );

	// Generated message map functions
	//{{AFX_MSG(CAlarmTipDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPrior();
	afx_msg void OnNext();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int		m_nIndexStart;
	int		m_nPageCount;
	int		m_nItemHeight;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMTIPDLG_H__6F138238_7A06_4F23_97BB_440F2D679D8F__INCLUDED_)
