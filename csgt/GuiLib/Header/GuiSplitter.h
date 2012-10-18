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


// CGuiSplitter
#include "GuiLib.h"
#include "GuiDrawlayer.h"
class GUILIBDLLEXPORT   CGuiSplitter : public CWnd
{
	DECLARE_DYNAMIC(CGuiSplitter)
public:
	enum TYPEALING {ALINGHORZ=0,ALINGVERT=1};
	enum STYLESEPARATOR{STYLE3D=0,STYLEFLAT=1};
	void SetColor(COLORREF m_clrface);
	void AddLeftCtrl(CWnd* pWnd,long MinSize);
	void AddRightCtrl(CWnd* pWnd,long MinSize);
	void SeTypeALing(TYPEALING typeAling);
	void SetPixelWidht(int nSizePix);
	void RecalLayout();
	virtual void DrawSplitter(CRect rcSeparator,CRect rcLeft,CRect rcRight,CDC* pDC);
	void SetStyleSeparator(STYLESEPARATOR StyleSeparator=STYLE3D,COLORREF clrLeftTop=GuiDrawLayer::GetRGBColorBTNHigh(),
							COLORREF clrBottomRight=GuiDrawLayer::GetRGBColorShadow());
	
public:
	CGuiSplitter();
	virtual ~CGuiSplitter();
protected:
	COLORREF	clrBg;
	CWnd*		pWndLeft;
	CWnd*		pWndRight;
	TYPEALING	TypeAling;
	CRect		m_rcSpplitter;
	int			m_nSizePix;
	CRect		m_rcSpplitterOld;
	BOOL		bIniciaArrastre;
	STYLESEPARATOR mStyleSeparator;
	COLORREF	mclrLeftTop;
	COLORREF	mclrBottomRight;
	CRect		m_rectLeft;
	CRect		m_rectRight;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL Create(CWnd* pParentWnd);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


