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

#if (!defined __EXT_EDIT_H)
#define __EXT_EDIT_H

#if (!defined __EXT_MFC_DEF_H)
	#include <ExtMfcDef.h>
#endif // __EXT_MFC_DEF_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CExtEdit window

class __PROF_UIS_API CExtEdit : public CEdit
{
	void _PostRedraw();
	void _DrawEditImpl(
		CRect rectClient,
		CDC * pDC = NULL
		);
	bool m_bMouseOver;

public:
	CExtEdit();
	
	void SetIndent(int margin);
	
// Attributes
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtEdit)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __EXT_EDIT_H
