// StkUI.h : main header file for the StkUI application
//

#if !defined(AFX_STKUI_H__940E5BF6_8019_45E8_A6F2_2F7B487FDE0C__INCLUDED_)
#define AFX_STKUI_H__940E5BF6_8019_45E8_A6F2_2F7B487FDE0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

/////////////////////////////////////////////////////////////////////////////
// CStkUIApp:
// See StkUI.cpp for the implementation of this class
//
class	CDocument;
/***
	主应用程序类
*/
class CStkUIApp : public CWinApp
{
public:
	CStkUIApp();

public:
	// stock database
	BOOL		OnDBChanged( BOOL bRecalculate );

	// operation
	void			SetStaticDoc( CStaticDoc * pDoc ) { m_pStaticDoc = pDoc; }
	CStaticDoc *	GetStaticDoc( ) { return m_pStaticDoc; }	// StaticDoc 是主管几个视图显示的Document
	void			SetMainWnd( CWnd * pWnd ) { m_pMainWnd = pWnd; }

	void		SaveLastOpenedStrategy( );
	int			OpenLastOpenedStrategy( );
	int			OpenDefaultStrategy( );

	CStrategy *	GetActiveStrategy( );		// 活动策略
	void		AddStrategy(CStrategy* pStrategy);
	void		RemoveStrategy(CStrategy* pStrategy);
	POSITION	GetFirstStrategyPosition() const;
	CStrategy *	GetNextStrategy(POSITION& rPos) const;
	BOOL		ExistStrategy( CStrategy * pStrategy );
	BOOL		IsFileOpened( LPCTSTR lpszFileName );
	CString		GetLastStrategyDirectory( );
	CString		GetNextNewStrategyTitle( CString & strExtBuffer, CString strPath );
	
	void		CloseAllStrategy( BOOL bEndSession = FALSE );
	CRecentFileList * GetRecentFileList( );

protected:  // standard implementation
	CPtrList	m_strategyList;          // opens of CStrategy
	CStaticDoc *	m_pStaticDoc;		// the document which is open initially and
									// user cannot close it until the program
									// exits.
	BOOL		m_bAutoUpdateViews;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStkUIApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual CStrategy* OpenStrategyFile(LPCTSTR lpszFileName);
	virtual BOOL SaveAllModified();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CStkUIApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnStrategyNew();
	afx_msg void OnFileOpen();
	afx_msg void OnStrategySave();
	afx_msg void OnUpdateStrategySave(CCmdUI* pCmdUI);
	afx_msg void OnStrategySaveAs();
	afx_msg void OnUpdateStrategySaveAs(CCmdUI* pCmdUI);
	afx_msg void OnStrategySaveall();
	afx_msg void OnUpdateStrategySaveall(CCmdUI* pCmdUI);
	afx_msg void OnStrategyClose();
	afx_msg void OnUpdateStrategyClose(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STKUI_H__940E5BF6_8019_45E8_A6F2_2F7B487FDE0C__INCLUDED_)
