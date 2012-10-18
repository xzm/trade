// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__46FF78D0_75FF_469E_8B70_01C6BD088FFD__INCLUDED_)
#define AFX_MAINFRM_H__46FF78D0_75FF_469E_8B70_01C6BD088FFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	TIMER_TIME			1
#define	TIMER_CHECKVALID	2

#define	WM_USER_CHECKSN		WM_USER+1001

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	struct	tagFullScreen
	{
		BOOL		bMaximized;
		BOOL		bFullScreen;
		BOOL		bStatusBar;
	}m_fullScreen;

// Operations
public:
	void	DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);
	CPoint	CalcPopupMenuPoint( CToolBar * pBar, UINT nID, CMenu *pMenu );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CTextToolBar	m_wndMainBar;
	CStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnViewMainbartextbot();
	afx_msg void OnUpdateViewMainbartextbot(CCmdUI* pCmdUI);
	afx_msg void OnViewMainbartextright();
	afx_msg void OnUpdateViewMainbartextright(CCmdUI* pCmdUI);
	afx_msg void OnViewFullscreen();
	afx_msg void OnUpdateViewFullscreen(CCmdUI* pCmdUI);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnToolGenserial();
	afx_msg void OnHelpRegister();
	afx_msg void OnToolOpendbfile();
	afx_msg void OnToolGenbase();
	afx_msg void OnToolBasermcr();
	afx_msg void OnToolFiletime();
	afx_msg void OnToolGendomain();
	afx_msg void OnToolGencodetbl();
	afx_msg void OnToolGennameenu();
	afx_msg void OnToolGeneva();
	//}}AFX_MSG
	afx_msg void OnMainBarDropDown(NMHDR* /*pNMHDR*/, LRESULT* pResult);
public:
	afx_msg LRESULT CreateValid( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__46FF78D0_75FF_469E_8B70_01C6BD088FFD__INCLUDED_)
