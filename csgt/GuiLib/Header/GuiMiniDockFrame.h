#pragma once
#include "Afxpriv.h"

// CGuiMiniDockFrame frame
#include "GuiLib.h"
class GUILIBDLLEXPORT CGuiMiniDockFrame : public CMiniDockFrameWnd
{
	DECLARE_DYNCREATE(CGuiMiniDockFrame)
protected:
	CGuiMiniDockFrame();           // protected constructor used by dynamic creation
	virtual ~CGuiMiniDockFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


