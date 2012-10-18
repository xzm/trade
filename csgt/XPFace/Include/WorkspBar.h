// WorkspBar.h: interface for the CWorkspBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPBAR_H__75301050_9149_4325_B98C_3C9D4151D9A4__INCLUDED_)
#define AFX_WORKSPBAR_H__75301050_9149_4325_B98C_3C9D4151D9A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLASS_EXPORT CWorkspBar : public CGuiControlBar  
{
public:
	CWorkspBar();
	virtual ~CWorkspBar();

	void	SetFrameWnd( CFrameWnd * pFrame )	{	m_pFrameWnd = pFrame;	}

// Attributes
public:
	CGuiWorkTab	m_tabctrl;
	CToolTipCtrl* m_ToolTip;

protected:
	int m_nActiveTab;
	CView* m_pActiveView;
	CList <TCB_ITEM *,TCB_ITEM *> m_views;
	CFrameWnd	*	m_pFrameWnd;

// Operations
public:
	BOOL IsSizeNormal( );
	void ShowNormal( BOOL bShowNormal = TRUE );
	void ToggleShow( );
	void SetTabImageList( UINT nBitmapID, int cx, int nGrow, COLORREF crMask );
	BOOL ModifyTabStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags);

	void SetActiveView(int nNewTab);
	void SetActiveView(CRuntimeClass *pViewClass);
	CView* GetActiveView();
	CView* GetView(int nView);
	CView* GetView(CRuntimeClass *pViewClass);
	BOOL AddView(LPCTSTR lpszLabel, CRuntimeClass *pViewClass, CCreateContext *pContext = NULL);
	void RemoveView(int nView);

// Overrides
public:
    // ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJTabCtrlBar)
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkspBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult) ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_WORKSPBAR_H__75301050_9149_4325_B98C_3C9D4151D9A4__INCLUDED_)
