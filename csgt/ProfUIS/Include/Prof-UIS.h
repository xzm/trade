//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///                                                                    ///
/// Professional User Interface Suite (TM)                             ///
/// Copyright (C) 2001-2002 by Foss Software Inc.                      ///
///                                                                    ///
/// http://www.fossware.com                                            ///
/// mailto:foss@fossware.com                                           ///
///                                                                    ///
///                                                                    ///
/// This software and accompanying documentation is the copyrighted    ///
/// work of Foss Software Inc.                                         ///
///                                                                    ///
/// Use of this software is governed by the following conditions:      ///
///                                                                    ///
///   a) this software is free for any kind of personal and            ///
///      commercial usage;                                             ///
///                                                                    ///
///   a) you may not reproduce, record, publish, publicly exhibit      ///
///      or distribute this source code without author's express       ///
///      prior written consent (foss@fossware.com);                    ///
///                                                                    ///
///   b) this notice should be remained intact in distributed source   ///
///      files;                                                        ///
///                                                                    ///
///   c) this software is subject to all Warranties and Disclaimers    ///
///      as set forth below;                                           ///
///                                                                    ///
/// Warranties and Disclaimers:                                        ///
///                                                                    ///
/// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND     ///
/// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,      ///
/// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.             ///
/// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,      ///
/// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENACY OF DATA, ///
/// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE   ///
/// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                ///
///                                                                    ///
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////TIAL DAMAGES, ///
/// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCUR//////////////////////

#if (!defined __PROF_UIS_H)
#define __PROF_UIS_H

#if (!defined __EXT_MFC_DEF_H)
	#include "ExtMfcDef.h"
#endif // __EXT_MFC_DEF_H

#ifndef __AFXWIN_H__
	#include <AfxWin.h>         // MFC core and standard components
#endif

#ifndef __AFXEXT_H__
	#include <afxext.h>         // MFC extensions
#endif

#ifndef _AFX_NO_OLE_SUPPORT
	#ifndef __AFXOLE_H__
		#include <AfxOle.h>     // MFC OLE classes
	#endif
	#ifndef __AFXODLGS_H__
		#include <AfxOdlgs.h>   // MFC OLE dialog classes
	#endif
	#ifndef __AFXDISP_H__
		#include <AfxDisp.h>    // MFC Automation classes
	#endif
#endif // _AFX_NO_OLE_SUPPORT

#ifndef __AFXTEMPL_H__
	#include <AfxTempl.h>
#endif

#ifndef __AFXMT_H__
	#include <AfxMt.h>
#endif

#ifndef __AFXCMN_H__
	#include <AfxCmn.h>
#endif

#ifndef __AFXDTCTL_H__
	#include <AfxDtCtl.h>
#endif

#ifndef __AFXCONV_H__
	#include <AfxConv.h>
#endif

#define __EXT_MFC_NO_RTF_TOOLTIPS

#if (!defined __PROF_UIS_IMPL__)

	#include "ExtMfcDef.h"

	#include "ExtBtnOnFlat.h"
	#include "ExtButton.h"

	#include "ExtComboBox.h"

	#include "ExtEdit.h"

	#include "ExtCmdManager.h"
	#include "ExtPaintManager.h"

	#include "ExtControlBar.h"
	#include "ExtToolControlBar.h"
	#include "ExtMenuControlBar.h"
	#include "ExtStatusControlBar.h"

	#include "ExtPopupMenuWnd.h"

	#include "ExtResizableDialog.h"
	#include "ExDlgBase.h"

	#include "ExtColorCtrl.h"
	#include "ExtColorDlg.h"

#endif // !__PROF_UIS_IMPL__

#endif // __PROF_UIS_H
