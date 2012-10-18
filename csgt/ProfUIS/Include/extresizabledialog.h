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

#if (!defined __EXT_RESIZABLE_DIALOG_H)
#define __EXT_RESIZABLE_DIALOG_H

//
// Mostly based on the Paolo Messina's idea of the resizable dialog
// with anchoring its inner child windows to its borders
//
// The following changes have been made:
//   - simplified repositioning
//   - redrawing appearance
//   - system menu
//
// Original credits:
//
// Copyright (C) 2000 by Paolo Messina
// (ppescher@yahoo.com)
//

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const SIZE __RDA_NONE = {	 -1,	 -1 };
const SIZE __RDA_LT   = {	  0,	  0 };
const SIZE __RDA_RT   = {	100,	  0 };
const SIZE __RDA_LB   = {	  0,	100 };
const SIZE __RDA_RB   = {	100,	100 };

/////////////////////////////////////////////////////////////////////////////
// CExtResizableDialog window

class __PROF_UIS_API CExtResizableDialog : public CDialog
{

// Construction
public:
	DECLARE_DYNAMIC( CExtResizableDialog )
	
	CExtResizableDialog();
	CExtResizableDialog(
		UINT nIDTemplate,
		CWnd * pParentWnd = NULL
		);
	CExtResizableDialog(
		LPCTSTR lpszTemplateName,
		CWnd * pParentWnd = NULL
		);

// Attributes
private:

	// internal flags
	bool m_bShowGrip : 1;
	bool m_bUseMaxTrack : 1;
	bool m_bUseMinTrack : 1;
	bool m_bUseMaxRect : 1;
	bool m_bEnableSaveRestore : 1;
	bool m_bSysMenuTracking : 1;
	bool m_bInitDone : 1;

	// internal status
	CString m_sSection;			// section name and
	CString m_sEntry;			// entry for save/restore

	POINT m_ptMinTrackSize;		// min tracking size
	POINT m_ptMaxTrackSize;		// max tracking size
	POINT m_ptMaxPos;			// maximized position
	POINT m_ptMaxSize;			// maximized size

	struct __PROF_UIS_API RDI_t
	{
		// upper-left corner
		SIZE m_AnchLT;
		SIZE m_MargLT;
		// bottom-right corner
		SIZE m_AnchRB;
		SIZE m_MargRB;
	
		RDI_t(
			SIZE AnchLT = __RDA_LT,
			SIZE MargLT = CSize(0,0), 
			SIZE AnchRB = __RDA_LT,
			SIZE MargRB = CSize(0,0)
			)
			: m_AnchLT( AnchLT )
			, m_MargLT( MargLT )
			, m_AnchRB( AnchRB )
			, m_MargRB( MargRB )
		{
		};
		
		RDI_t(
			const RDI_t & other
			)
			: m_AnchLT( other.m_AnchLT )
			, m_MargLT( other.m_MargLT )
			, m_AnchRB( other.m_AnchRB )
			, m_MargRB( other.m_MargRB )
		{
		};
		
		RDI_t & operator=(
			const RDI_t & other
			)
		{
			m_AnchLT = other.m_AnchLT;
			m_MargLT = other.m_MargLT;
			m_AnchRB = other.m_AnchRB;
			m_MargRB = other.m_MargRB;
			return *this;
		};

	}; // struct __PROF_UIS_API RDI_t

	// map of repositionable controls
	CMap < HWND, HWND, RDI_t, RDI_t > m_mapRDI;

// Operations
public:
	BOOL _TrackDialogSystemMenu(
		CPoint * pPoint,
		BOOL bSelectAny
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtResizableDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:

	void LoadWindowRect(
		bool bForceHideWindow = false
		);
	void SaveWindowRect();

	void ArrangeLayout(
		int cx = -1,
		int cy = -1
		);

	bool AddAnchor(
		UINT nDlgCtrlID,
		const CRect & rcAnch
		);
	bool AddAnchor(
		HWND hWnd,
		const CRect & rcAnch
		);
	bool AddAnchor(
		UINT nDlgCtrlID,
		CSize AnchLT,
		CSize AnchRB = __RDA_NONE
		);
	bool AddAnchor(
		HWND hWnd,
		CSize AnchLT,
		CSize AnchRB = __RDA_NONE
		);

	void ShowSizeGrip(BOOL bShow);				// show or hide the size grip
	
	void SetMaximizedRect(const CRect& rc);		// set window rect when maximized
	void ResetMaximizedRect();					// reset to default maximized rect
	
	void SetMinTrackSize(const CSize& size);	// set minimum tracking size
	void ResetMinTrackSize();					// reset to default minimum tracking size
	
	void SetMaxTrackSize(const CSize& size);	// set maximum tracking size
	void ResetMaxTrackSize();					// reset to default maximum tracking size
	
	void EnableSaveRestore(
		LPCTSTR pszSection,
		LPCTSTR pszEntry,
		bool bForceHideWindow = false
		);	// section and entry in app's profile

// Generated message map functions
protected:
	//{{AFX_MSG(CExtResizableDialog)
	virtual BOOL OnInitDialog();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	virtual void OnCancel();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__EXT_RESIZABLE_DIALOG_H)
