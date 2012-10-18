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


// CGuiSysTray

#include "GuiLib.h" 


class GUILIBDLLEXPORT  CGuiSysTray : public CWnd
{
	DECLARE_DYNAMIC(CGuiSysTray)

public:
	CGuiSysTray();
	virtual ~CGuiSysTray();
	BOOL Add(UINT uID, UINT uCallBackMessage, HICON hIcon, LPCTSTR lpszTip);
	BOOL Del();
	BOOL Update(UINT uID, HICON hIcon, LPCTSTR lpszTip);
	BOOL SetIcon(HICON hIcon);
	BOOL SetIcon(LPCTSTR lpszIcon);
	BOOL SetTips(LPCTSTR lpszTip);
	void SetSysMenu(CMenu* pMenu);
	void SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask);
	void StartAnimation(int nMilliseconds,int nPosInit, int nPosEnd);
	void StopAnimation();
protected:
	NOTIFYICONDATA tnid;
	CMenu* m_pMenu;
	CImageList	m_ImgList;
	BOOL	m_bStartAnimation;
	int		m_ActualImage;
	int		m_Init;
	int     m_End;
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nID,UINT uCallBackMessage, HICON hIcon, LPCTSTR lpszTip);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT nIDEvent);
};


