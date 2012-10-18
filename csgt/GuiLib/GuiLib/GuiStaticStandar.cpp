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
// GuiStaticStandar.cpp : implementation file
//

#include "stdafx.h"
#include "GuiStaticStandar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuiStaticStandar

CGuiStaticStandar::CGuiStaticStandar()
{
	m_pWnd=NULL;
}

CGuiStaticStandar::~CGuiStaticStandar()
{
}


BEGIN_MESSAGE_MAP(CGuiStaticStandar, CStatic)
	//{{AFX_MSG_MAP(CGuiStaticStandar)
	ON_WM_SIZE()
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuiStaticStandar message handlers

void CGuiStaticStandar::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CStatic::PreSubclassWindow();
}

void CGuiStaticStandar::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	RecalLayout();
	// TODO: Add your message handler code here
	
}

void CGuiStaticStandar::AddChild(CWnd* pWnd)
{
	m_pWnd=pWnd;
	RecalLayout();
}

void CGuiStaticStandar::RecalLayout()
{
	if (m_pWnd== NULL) return;
	CRect rcClient;
	GetClientRect(rcClient);
	m_pWnd->MoveWindow(rcClient);
	
}

