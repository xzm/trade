//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///                                                                    ///
/// Professional User Interface Suite (TM)                             ///
/// Copyright (C) 2001-2002 by Foss Software Inc.                      ///
///                                                                    ///
/// http://www.fossware.com                                            ///
/// mailto:foss@fossware.com                                           ///
///                                                                    ///
///                                                                    ///
/// This software and accompanying documentation is the copyrighted    ///
/// work of Foss Software Inc.                                         ///
///                                                                    ///
/// Use of this software is governed by the following conditions:      ///
///                                                                    ///
///   a) this software is free for any kind of personal and            ///
///      commercial usage;                                             ///
///                                                                    ///
///   a) you may not reproduce, record, publish, publicly exhibit      ///
///      or distribute this source code without author's express       ///
///      prior written consent (foss@fossware.com);                    ///
///                                                                    ///
///   b) this notice should be remained intact in distributed source   ///
///      files;                                                        ///
///                                                                    ///
///   c) this software is subject to all Warranties and Disclaimers    ///
///      as set forth below;                                           ///
///                                                                    ///
/// Warranties and Disclaimers:                                        ///
///                                                                    ///
/// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND     ///
/// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,      ///
/// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.             ///
/// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,      ///
/// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES, ///
/// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA, ///
/// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE   ///
/// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                ///
///                                                                    ///
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#if( !defined __EXTMINIDOCKFRAMEWND_H)
#define __EXTMINIDOCKFRAMEWND_H

#if (!defined __AFXPRIV_H__)
	#include <AfxPriv.h>
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CExtBarNcAreaButton;
class CExtControlBar;

/////////////////////////////////////////////////////////////////////////
// CExtMiniDockFrameWnd window

class __PROF_UIS_API CExtMiniDockFrameWnd : public CMiniDockFrameWnd
{
//	/// CMiniDockFrameWnd::Create replacement
//	BOOL MDFW_Create(
//		CWnd* pParent,
//		DWORD dwBarStyle
//		);
//	/// CMiniFrameWnd::CreateEx replacement
//	BOOL CExtMiniDockFrameWnd::MFW_CreateEx(
//		DWORD dwExStyle,
//		LPCTSTR lpClassName,
//		LPCTSTR lpszWindowName,
//		DWORD dwStyle,
//		const RECT& rc,
//		CWnd* pParentWnd,
//		UINT nID
//		);

    DECLARE_DYNCREATE(CExtMiniDockFrameWnd)

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtMiniDockFrameWnd)
    public:
    virtual BOOL Create(CWnd* pParent, DWORD dwBarStyle);
    //}}AFX_VIRTUAL

// Implementation
public:
	CExtMiniDockFrameWnd()
	{
		m_rcBtnHideBar.SetRectEmpty();
		m_rcFrameCaption.SetRectEmpty();
		m_bBtnHideBar = false;
		m_nResizingMode = HTNOWHERE;
	};
	~CExtMiniDockFrameWnd()
	{
	};
    
	CControlBar * GetControlBar();
	CExtControlBar * GetControlBarExt();

    //{{AFX_MSG(CExtMiniDockFrameWnd)
	afx_msg void OnNcMouseMove( UINT nHitTest, CPoint point );
	afx_msg void OnTimer( UINT nIDEvent );
	afx_msg void OnClose();
	afx_msg void OnNcPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
	static bool g_bOnWndPosChgProcessing;
public:
	static bool g_bAutoMakeTopmostOnMouseEvent;
protected:
	CSize	m_LastSize;
	CRect	m_rcBtnHideBar;
	bool	m_bBtnHideBar;
	CRect	m_rcFrameCaption;
	
	void _AnalyzeButtonsHover(CPoint point);
	void _AnalyzeButtonsHover()
	{
		CPoint point;
		if( ::GetCursorPos(&point) )
			_AnalyzeButtonsHover(point);
	};

private:
	UINT m_nResizingMode;
	CRect m_rcWndResizingStart;
	bool _ResizingIsInProgress()
	{
		return (m_nResizingMode == HTNOWHERE) ? false : true;
	};
	void _ResizingStart(
		UINT nHitTest,
		const CPoint & point
		);
	void _ResizingEnd();
	void _ResizingUpdateState(
		const CPoint & point
		);

	friend class CExtBarNcAreaButton;
}; // class CExtMiniDockFrameWnd

#endif // __EXTMINIDOCKFRAMEWND_H
