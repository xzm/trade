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

#include "stdafx.h"
#include <ExtBtnOnFlat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CWinApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CExtBtnOnFlat

CExtBtnOnFlat::CExtBtnOnFlat()
{
	// RTF tool tip support
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
	m_bToolTipSet = FALSE;
#endif

	m_bCurrentlyIsFlat = TRUE;
}

CExtBtnOnFlat::~CExtBtnOnFlat()
{
}

IMPLEMENT_DYNCREATE(CExtBtnOnFlat, CButton);

BEGIN_MESSAGE_MAP(CExtBtnOnFlat, CButton)
	//{{AFX_MSG_MAP(CExtBtnOnFlat)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtBtnOnFlat message handlers
void CExtBtnOnFlat::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetTimer(1, 50, NULL);
	CButton::OnMouseMove(nFlags, point);
	// RTF tool tip support
#if (!defined __EXT_MFC_NO_RTF_TOOLTIPS)
	if(m_bToolTipSet)
		m_wndRtfToolTipCtrl.
			SetupRtfFromString(
				point, m_strToolTip );
#endif
}

void CExtBtnOnFlat::OnTimer(UINT nIDEvent) 
{
	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);

	static bool bPainted = false;

	if(!rcItem.PtInRect(pt))
	{
		KillTimer(1);
		if(!m_bCurrentlyIsFlat)
		{
			m_bCurrentlyIsFlat = TRUE;
			ModifyStyle(0,BS_FLAT);
			RedrawWindow();
		}
		return;
	}
	else
	{
		if(m_bCurrentlyIsFlat)
		{
			m_bCurrentlyIsFlat = FALSE;
			ModifyStyle(BS_FLAT,0);
			RedrawWindow();
		}
	}



	CButton::OnTimer(nIDEvent);
}


BOOL CExtBtnOnFlat::OnEraseBkgnd(CDC* pDC) 
{
//	return CButton::OnEraseBkgnd(pDC);
	return TRUE;
}

void CExtBtnOnFlat::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
//	m_bDefault=GetStyle() & (DWORD)BS_DEFPUSHBUTTON; //this is necessary as 
//	ModifyStyle(0, BS_OWNERDRAW);
	ModifyStyle(0, BS_FLAT);
}
