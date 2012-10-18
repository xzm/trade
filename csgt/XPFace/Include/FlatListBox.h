#if !defined(AFX_FLATLISTBOX_H__611C2EA3_5AD0_11D2_B1E0_525400DAACF0__INCLUDED_)
#define AFX_FLATLISTBOX_H__611C2EA3_5AD0_11D2_B1E0_525400DAACF0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FlatListBox.h : header file
//
#define FE_MOUSEOVER 1
#define FE_MOUSEOUT 2

/////////////////////////////////////////////////////////////////////////////
// CFlatListBox window

class CLASS_EXPORT CFlatListBox : public CListBox
{
// Construction
public:
	CFlatListBox();
	void DrawListBox(int style, COLORREF clrTopLeft, COLORREF clrBottomRight);


// Attributes
public:
	bool m_bLBtnDown;
	COLORREF m_clrHilite;
	COLORREF m_clrShadow;
	COLORREF m_clrDkShad;
	COLORREF m_clrButton;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlatListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFlatListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFlatListBox)
	afx_msg void OnKillfocus();
	afx_msg void OnSetfocus();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATLISTBOX_H__611C2EA3_5AD0_11D2_B1E0_525400DAACF0__INCLUDED_)
