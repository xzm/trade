//-----------------------------------------------------------------------//
// This is a part of the GuiLib MFC Extention.							 //	
// Autor  :  Francisco Campos											 //
// (C) 2002 Francisco Campos <www.beyondata.com> All rights reserved     //
// This code is provided "as is", with absolutely no warranty expressed  //
// or implied. Any use is at your own risk.								 //		
// You must obtain the author's consent before you can include this code //
// in a software library.												 //
// If the source code in  this file is used in any application			 //
// then acknowledgement must be made to the author of this program		 //	
// fcampos@tutopia.com													 //
//-----------------------------------------------------------------------//


#if !defined(AFX_GUIDOCBAREXTEN_H__2B746533_9AEB_11D5_A8C3_0008C7B2934C__INCLUDED_)
#define AFX_GUIDOCBAREXTEN_H__2B746533_9AEB_11D5_A8C3_0008C7B2934C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include <afxpriv.h>	
#include "GuiLib.h"

const DWORD dwDockBarMap[4][2] =
{
	{ AFX_IDW_DOCKBAR_TOP,      CBRS_TOP    },
	{ AFX_IDW_DOCKBAR_BOTTOM,   CBRS_BOTTOM },
	{ AFX_IDW_DOCKBAR_LEFT,     CBRS_LEFT   },
	{ AFX_IDW_DOCKBAR_RIGHT,    CBRS_RIGHT  },
};

class GUILIBDLLEXPORT CGuiDocBarExten : public CDockBar  
{
	//*******************************************
		DECLARE_DYNCREATE(CGuiDocBarExten)
public:

	CGuiDocBarExten();
	virtual ~CGuiDocBarExten();

public:
	//********************************************	
	void	BarsDocking(CFrameWnd * pFrame, DWORD dwDockStyle);
public:
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDockBarEx)
		virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_GUIDOCBAREXTEN_H__2B746533_9AEB_11D5_A8C3_0008C7B2934C__INCLUDED_)
