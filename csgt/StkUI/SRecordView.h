#if !defined(AFX_SRECORDVIEW_H__0912B904_0141_49A8_BAB3_6AAC901C496C__INCLUDED_)
#define AFX_SRECORDVIEW_H__0912B904_0141_49A8_BAB3_6AAC901C496C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SRecordView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSRecordView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CSRecordView : public CFormView
{
protected:
	CSRecordView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSRecordView)

// Form Data
public:
	//{{AFX_DATA(CSRecordView)
	enum { IDD = IDD_SLIST_FORM1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	
	CGuiWorkTab	m_wndWorkTab;

// Attributes
public:
	inline virtual CStaticDoc* GetDocument()   { return (CStaticDoc *)m_pDocument; }
	void			SetFont( LPLOGFONT pLogFont );
// Operations
public:
	virtual	void	ResetColumns( );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSRecordView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL
protected:
	// Grid
	CGridCtrl	m_Grid;
	
	BOOL	m_bShouldUpdate;
	BOOL	m_bFirstUpdate;

// Implementation
protected:
	virtual ~CSRecordView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSRecordView)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnGetViewTitle( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGetViewCmdid( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRECORDVIEW_H__0912B904_0141_49A8_BAB3_6AAC901C496C__INCLUDED_)
