// GuiMiniDockFrame.cpp : implementation file
//

#include "stdafx.h"
#include "GuiLib.h"
#include "GuiMiniDockFrame.h"
#include "..\header\guiminidockframe.h"


// CGuiMiniDockFrame

IMPLEMENT_DYNCREATE(CGuiMiniDockFrame, CMiniDockFrameWnd)

CGuiMiniDockFrame::CGuiMiniDockFrame()
{
}

CGuiMiniDockFrame::~CGuiMiniDockFrame()
{
}


BEGIN_MESSAGE_MAP(CGuiMiniDockFrame, CMiniFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CGuiMiniDockFrame message handlers

int CGuiMiniDockFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->dwExStyle&=~WS_BORDER;
	if (CMiniDockFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO:  Add your specialized creation code here

	return 0;
}
