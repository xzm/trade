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


// CTab.h : implementation file
#if !defined(AFX_CBASETAB_INCLUDED_)
#define AFX_CBASETAB_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTab 
{
public:

	CTab(CWnd* mpParent,LPCSTR mlpMsg,UINT muIcon=-1)
	{
		pParent	=mpParent;
		lpMsg	=mlpMsg;
		uIcon	=muIcon;
		rect.SetRectEmpty();
	}
public:
	//************************************
	CWnd*	pParent;
	LPCSTR	lpMsg;
	UINT	uIcon;
	CRect	rect;

};



#endif