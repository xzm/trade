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

#if (!defined __EXT_BUTTON_H)
#define __EXT_BUTTON_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
	#include <ExtRichToolTipWnd.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtButton window

class __PROF_UIS_API CExtButton : public CButton
{
	DECLARE_DYNCREATE(CExtButton);

// Construction
public:
    CExtButton();
	~CExtButton();

// Attributes
public:

// Operations
public:
	CMenu m_menu;
	int m_nMenuOpt; // TPMX_TOPALIGN is default

	virtual void _OnClick();
	virtual bool _OnTrackPopup();
	virtual bool _IsMenuAvail();
	
	bool _IsMouseOnButton();

private:
	// RTF tool tip support
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
	CExtRichToolTipWnd m_wndRtfToolTipCtrl;
	BOOL m_bToolTipSet;
	CString m_strToolTip;
#endif

// Operations
public:
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
    void SetupRtfToolTip(
		const CString & tip
		)
	{
		m_strToolTip=tip;
		if(!m_bToolTipSet)
		{
			m_bToolTipSet=m_wndRtfToolTipCtrl.Create(this);
		}
	};
    void SetupRtfToolTip(
		LPCTSTR pszMsgResId,
		LPCTSTR pszMsgResType="RTF"
		)
	{
		if(!m_bToolTipSet)
		{
			if(CExtRichToolTipWnd::_LoadStringEx(
					pszMsgResId,
					pszMsgResType,
					m_strToolTip
				) )
			{
				m_bToolTipSet=m_wndRtfToolTipCtrl.Create(this);
			}
			else
				m_strToolTip="";
		}
	};
#endif // __EXT_MFC_NO_RTF_TOOLTIPS

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtButton)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL GetDefault();

	void SetTooltipText(int nId, BOOL bActivate = TRUE);
	void SetTooltipText(CString* spText, BOOL bActivate = TRUE);
	void ActivateTooltip(BOOL bEnable = TRUE);

	BOOL SetBtnCursor(int nCursorId = -1);

	void SetAlign(int nAlign);
	int GetAlign();

	void SetFlat(BOOL bState = TRUE);
	BOOL GetFlat();

	void SetDrawBorder(BOOL bEnable = TRUE);
	void SetIcon(int nIconInId, int nIconOutId = NULL);
	void SetIcon(HICON hIconIn, HICON hIconOut = NULL);

protected:
    //{{AFX_MSG(CExtButton)
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void _FreeWinObjects();

protected:
	static void _CbPaintCombinedContent(
		LPVOID pCookie,
		CDC & dc,
		const CWnd & refWndMenu,
		const CRect & rcExcludeArea, // in screen coords
		int eCombineAlign // CExtPopupMenuWnd::e_combine_align_t values
		);
protected:
	void _DrawImpl( // buffered paint
		CDC & dc,
		bool bTransparent = false
		);
	virtual void _RenderImpl( // non-buffered paint
		CDC & dc,
		bool bTransparent = false
		);
	void InitToolTip();

	int m_nAlign;
	BOOL m_bDrawBorder,m_bIsFlat,m_bMouseOver,m_bPushed,m_bPushTracking,m_bKeyTracking;

	HCURSOR m_hCursor;
	CToolTipCtrl m_ToolTip;

	HICON m_hIconIn;
	HICON m_hIconOut;

	BOOL m_bIsDefault;
}; // class CExtButton

/////////////////////////////////////////////////////////////////////////////

class __PROF_UIS_API CExtColorButton : public CExtButton
{
	DECLARE_DYNCREATE(CExtColorButton);

// Construction
public:
    CExtColorButton();
	~CExtColorButton();

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExtColorButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	bool m_bEnableBtnColorDefault;
	bool m_bEnableBtnColorCustom;
	bool m_bUseStdColorDlg;
	COLORREF m_clrDefault,m_clrSelected;

protected:
	virtual bool _OnTrackPopup();
	virtual bool _IsMenuAvail();
	virtual void _RenderImpl( // non-buffered paint
		CDC & dc,
		bool bTransparent = false
		);
	
    //{{AFX_MSG(CExtColorButton)
	//}}AFX_MSG
	afx_msg LRESULT OnColorChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorSelectCustom(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

}; // class CExtColorButton

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif /// __EXT_BUTTON_H
