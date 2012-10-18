// GraphView.h : interface of the CGraphView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHVIEW_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_)
#define AFX_GRAPHVIEW_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"StockGraph.h"

#define WM_USER_MYRBUTTONDOWN	(WM_USER+1001)

/***
	股票技术指标视图
*/
class CGraphView : public CView
{
protected: // create from serialization only
	CGraphView();
	DECLARE_DYNCREATE(CGraphView)

// Attributes
	friend class	CMainFrame;

public:
	inline virtual CStaticDoc* GetDocument()   { return (CStaticDoc *)m_pDocument; }

	CGuiTabPowerPoint	m_wndReportTab;

// Operations
public:
	void	ResetClientRect( );
	void	ChangeKType( int ktype );
	void	ChangeKFormat( int kformat );
	void	OnToggleKType();
	void	OnToggleKFormat( int kformat );
	void	ChangeMaindataType( int maindatatype );
	void	ChangeKLineMode( int klineMode );
	void	OnTechParametersChange( );
	void	ShowMultiStock( int nMultiStockCount );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStockGraph	m_graph;

// Generated message map functions
protected:
	//{{AFX_MSG(CGraphView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewPday();
	afx_msg void OnViewPweek();
	afx_msg void OnViewPmonth();
	afx_msg void OnViewPmin5();
	afx_msg void OnViewPmin15();
	afx_msg void OnViewPmin30();
	afx_msg void OnViewPmin60();
	afx_msg void OnViewLeft();
	afx_msg void OnViewRight();
	afx_msg void OnViewPageleft();
	afx_msg void OnViewPageright();
	afx_msg void OnViewHome();
	afx_msg void OnViewEnd();
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnViewPrev();
	afx_msg void OnViewNext();
	afx_msg void OnViewInsert();
	afx_msg void OnViewNoresumedr();
	afx_msg void OnViewResumedrup();
	afx_msg void OnViewResumedrdown();
	afx_msg void OnViewMaindataclose();
	afx_msg void OnViewMaindataopen();
	afx_msg void OnViewMaindataaverage();
	afx_msg void OnViewKlinecandle();
	afx_msg void OnViewKlineamerica();
	afx_msg void OnViewKlinetower();
	afx_msg void OnUpdateViewPday(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPweek(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPmonth(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPmin5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPmin15(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPmin30(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPmin60(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewNoresumedr(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewResumedrup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewResumedrdown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMaindataclose(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMaindataopen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMaindataaverage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewKlinecandle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewKlineamerica(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewKlinetower(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeReporttab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnViewHidethistech();
	afx_msg void OnUpdateViewHidethistech(CCmdUI* pCmdUI);
	afx_msg void OnViewThistechparam();
	afx_msg void OnUpdateViewThistechparam(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnGetViewTitle( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetViewCmdid( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnColorChange( WPARAM wParam, LPARAM lParam );
	afx_msg void OnViewTechRange(UINT nCmdID);
	afx_msg void OnUpdateViewTechRange(CCmdUI* pCmdUI);
	afx_msg LRESULT OnMyRButtonDown( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStkReceiverData( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHVIEW_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_)
