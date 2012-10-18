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
#pragma once
#include "guicontrolbar.h"
#include "GuiDockContext.h"
#include "GuiLib.h"


class  CGuiDockContextOffice :public CGuiDockContext
{
public:
	CGuiDockContextOffice(CControlBar* pBar) : CGuiDockContext(pBar) {}
	virtual ~CGuiDockContextOffice();
	virtual void StartDrag(CPoint pt);
protected:
	virtual void AdjustWindowForFloat(CRect& rect);

};


class GUILIBDLLEXPORT    CGuiOfficeBar :public CGuiControlBar
{
protected:
	CGuiControlBarButton  m_ArrButtons[4];
	int m_stateBtnBack;
	int m_stateBtnClose;
	int m_stateBtnFor;
	int m_stateBtnMenu;
	UINT m_nHits;
	int m_StateBtn;
	
public:
	CGuiOfficeBar(void);
	virtual ~CGuiOfficeBar(void);
	virtual void DrawGripper(CDC* pDC,CRect* rc);
	virtual void OnBack();
	virtual void OnForWard();
	virtual void OnMenu();
	void ShowTitle(CString m_Caption);
DECLARE_MESSAGE_MAP()
	afx_msg virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

	
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
};
