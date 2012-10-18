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


#if !defined(AFX_GUIDOCKCONTEXT_H__B30C848F_0A82_46A1_BAE2_E2C73CE5C2BB__INCLUDED_)
#define AFX_GUIDOCKCONTEXT_H__B30C848F_0A82_46A1_BAE2_E2C73CE5C2BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GuiLib.h"
#include "AfxPriv.h"
//***********************************************************

class GUILIBDLLEXPORT CGuiDockContext: public CDockContext
{
public:
// Construction
	CGuiDockContext(CControlBar* pBar);

// Attributes

	virtual void StartDrag(CPoint pt);
	void Move(CPoint pt);       // called when mouse has moved
	void EndDrag();             // drop
	void OnKey(int nChar, BOOL bDown);
	void AdjustWindowForFloat(CRect& rect);
// Resize Operations
	virtual void StartResize(int nHitTest, CPoint pt);
	void Stretch(CPoint pt);
	void EndResize();
	int  VerLeft(int i,int nSize,CDockBar* pDockBar,CControlBar* pBar);
	int  VerRight(int i,int nSize,CDockBar* pDockBar,CControlBar* pBar);
// Double Click Operations
	virtual void ToggleDocking();

// Operations
	void InitLoop();
	void CancelLoop();
	DWORD CanDock(CRect rect, DWORD dwDockStyle,
		CDockBar** ppDockBar = NULL); // called 
// Implementation
public:
	~CGuiDockContext();
	BOOL Track();
	BOOL bFirstClic;
	BOOL bSecondClic;
	void DrawFocusRect(BOOL bRemoveRect = FALSE);
		// draws the correct outline
	void UpdateState(BOOL* pFlag, BOOL bNewValue);
	DWORD CanDock();
	CDockBar* GetDockBar(DWORD dwOverDockStyle);
};

#endif // !defined(AFX_GUIDOCKCONTEXT_H__B30C848F_0A82_46A1_BAE2_E2C73CE5C2BB__INCLUDED_)
