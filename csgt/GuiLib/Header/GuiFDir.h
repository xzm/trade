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

#if !defined(AFX_FDIR_H__25E4B82F_FB23_45B2_B475_91668BFF681B__INCLUDED_)
#define AFX_FDIR_H__25E4B82F_FB23_45B2_B475_91668BFF681B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FDir.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFDir dialog

#include "GuiLib.h"

class  CFWnd: public CWnd
{
public:
		//***************************
		void OnOpen();
		void OnReSize();

	DECLARE_MESSAGE_MAP()
};

class GUILIBDLLEXPORT CGuiFDir : public CFileDialog
{
	DECLARE_DYNAMIC(CGuiFDir)

public:
	//**************************************
	CGuiFDir(CWnd* parent);
	void OnInitDone();
	void OnFolderChange();
	//**************************************
public:
	CWnd*		cw;
	CString		m_GetPath;
	CEdit		m_cedit;
	CFWnd		cfwn;
protected:
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FDIR_H__25E4B82F_FB23_45B2_B475_91668BFF681B__INCLUDED_)
