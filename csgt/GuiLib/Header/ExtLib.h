#if !defined(EXTLIB_H_INCLUDED)
#define EXTLIB_H_INCLUDED

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef	EXTLIB_DLL
	#if defined(STKLIB_STATIC) 
		#pragma comment(lib,"GuiLibStatic.lib")
		#pragma message("Automatically linking with GuiLibStatic.lib")
	#else
		#pragma comment(lib,"GuiLib.lib")
		#pragma message("Automatically linking with GuiLib.dll")
	#endif
#endif


#include "CBaseTab.h"
#include "CoolMenu.h"
#include "GuiBaseTab.h"
#include "GuiButton.h"
#include "GuiCapOutBar.h"
#include "GuicheckBox.h"
#include "GuiColors.h"
#include "GuiComboBoxExt.h"
#include "GuiContainer.h"
#include "GuiControlBar.h"
#include "GuiDocBarExten.h"
#include "GuiDockBar.h"
#include "GuiDockContext.h"
#include "GuiDockToolBar.h"
#include "GuiDocSpecial.h"
#include "GuiDrawLayer.h"
#include "GuiDropDownEdit.h"
#include "GuiEdit.h"
#include "GuiFDir.h"
#include "GuiFile.h"
#include "GuiFolder.h"
#include "GuiFrameWnd.h"
#include "GuiGroupBox.h"
#include "GuiheaderCtrl.h"
#include "GuiImageLinkButton.h"
#include "GuiLabelButton.h"
#include "GuiLib.h"
#include "GuiLinkButton.h"
#include "GuiListEdit.h"
#include "GuiMDIFrame.h"
#include "GuiMDITabbed.h"
#include "GuiMiniDockFrame.h"
#include "GuiMiniSplitter.h"
#include "GuiMiniTool.h"
#include "GuiNormalButton.h"
#include "GuiOfficeBar.h"
#include "GuiOutLook.h"
#include "GuiOutLookView.h"
#include "GuiPowerPointView.h"
#include "GuiRadioButton.h"
#include "GuiSliderCtrl.h"
#include "GuiSplitterWnd.h"
#include "GuiStatusBar.h"
#include "GuiTabbed.h"
#include "GuiTabPowerPoint.h"
#include "GuiTabWnd.h"
#include "GuiToolBarWnd.h"
#include "GuiToolButton.h"
#include "GuiVisioFolder.h"
#include "GuiWorkPanel.h"
#include "GuiWorkTab.h"
#include "GuiStaticStandar.h"
#include "MenuBar.h"
#include "Subclass.h"

#endif // EXTLIB_H_INCLUDED
