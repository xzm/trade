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


// CGuiDropDownEdit
#include "GuiEdit.h"
#include "GuiLib.h"
#include "GuiNormalButton.h"

class GUILIBDLLEXPORT CGuiDropDownEdit : public CStatic
{
	DECLARE_DYNAMIC(CGuiDropDownEdit)
public:
	enum Border{STYLE3D=0,STYLEPRESS=1,STYLEFRAME=2};
protected:
	CGuiEdit m_Edit;
	CGuiNormalButton m_toolBtn;
	BOOL bShowButton;
	Border m_border;
	CFont m_cfont;
public:
	CGuiDropDownEdit();
	virtual ~CGuiDropDownEdit();
	void ShowButton(BOOL bShow=TRUE);
	void SetImageButton(HICON hIcon);
	void SetStyle(Border border);
	void SetToolTipEdit(CString szToolTop);
	void SetToolTipBtn(CString szToolTop);
	void SetMask(CString mszMask,CString mszShowMask,CGuiEdit::Mask enTypeMask);
	CString GetText();
	void SetText(CString szCaption);
	void OnDropButton();
	void SetLimitText(int numText);
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	void CreateControls();
public:
	afx_msg void OnNcPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


