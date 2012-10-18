// RealTimeView.h : interface of the CRealTimeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALTIMEVIEW_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_)
#define AFX_REALTIMEVIEW_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"RealTime.h"

#define	RTV_MAX_MULTICOUNT	9	// 最多定义为9
/***
	实时行情视图（分时行情）
*/
class CRealTimeView : public CView
{
protected: // create from serialization only
	CRealTimeView();
	DECLARE_DYNCREATE(CRealTimeView)

	// Attributes
	int			m_nMultiStockCount;				// 多股同列，股数
	CRealTime	m_realtime[RTV_MAX_MULTICOUNT];	// 多股同列

public:
	inline virtual CStaticDoc* GetDocument()   { return (CStaticDoc *)m_pDocument; }

	CGuiTabPowerPoint	m_wndReportTab;

// Operations
public:
	void	ShowMultiStock( int nMultiStockCount );
	int		GetShowMultiStock( )	{	return m_nMultiStockCount;	}

	void	ChangeDrawMode( int nDrawMode );
	void	ToggleDrawTechLine( int nTechLine );
	int		GetDrawMode( )	{	return m_realtime[0].GetDrawMode();	}
	int		GetDrawTechLine()	{	return m_realtime[0].GetDrawTechLine();	}

	void	SendRequestQuote( BOOL bForced );
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealTimeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRealTimeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void	RedrawAll( CDC * pDC, CRect rectClient );

// Generated message map functions
protected:
	//{{AFX_MSG(CRealTimeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRealtimePrev();
	afx_msg void OnRealtimeNext();
	afx_msg void OnRealtimeLeft();
	afx_msg void OnRealtimeRight();
	afx_msg void OnRealtimeUp();
	afx_msg void OnRealtimeDown();
	afx_msg void OnRealtimeHome();
	afx_msg void OnRealtimeEnd();
	afx_msg void OnSelchangeReporttab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnGetViewTitle( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetViewCmdid( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnColorChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStkReceiverData( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALTIMEVIEW_H__0A8D66A9_30E0_4AAA_8E17_06B721FC0E97__INCLUDED_)
