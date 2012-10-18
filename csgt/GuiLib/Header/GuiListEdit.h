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


// CGuiListEdit

#include "GuiLib.h" 
#include "GuiContainer.h"
#include "GuiMiniTool.h"
#include "GuiNormalButton.h"
class GUILIBDLLEXPORT CGuiListEdit : public CStatic
{
	DECLARE_DYNAMIC(CGuiListEdit)
public:
	enum Border{STYLE3D=0,STYLEPRESS=1,STYLEFRAME=2};
private:
	CGuiContainer	m_Conta;
	CGuiMiniTool	m_MiniTool;
	CListCtrl*		m_list;
	COLORREF		m_clrface;
	Border			m_border;

public:
	void Delete();
	void Insert();
	void Up();
	void Down();
	void SetStyle(Border border);
	CString GetText(int nItem);
	void AddItem(CString m_szCad);
	int GetNumItem();
	CGuiNormalButton m_toolBtn;
public:
	CGuiListEdit();
	virtual ~CGuiListEdit();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
};


