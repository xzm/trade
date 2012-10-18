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
/// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES, ///
/// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA, ///
/// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE   ///
/// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                ///
///                                                                    ///
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#if (!defined __EXT_MFC_DEF_H)
#define __EXT_MFC_DEF_H

// Prof-UIS basic library
#ifdef _DEBUG
	#define __PROF_UIS_NAME_		_T("ProfUISd")
	#define __PROF_UIS_MODULE_NAME	_T("ProfUISd.dll")
	#define __PROF_UIS_LIB_NAME		_T("ProfUISd.lib")
#else
	#define __PROF_UIS_NAME_		_T("ProfUIS")
	#define __PROF_UIS_MODULE_NAME	_T("ProfUIS.dll")
	#define __PROF_UIS_LIB_NAME		_T("ProfUIS.lib")
#endif // _DEBUG

#if defined __PROF_UIS_IMPL__
	#define __PROF_UIS_API _declspec(dllexport)
#else 
	#define __PROF_UIS_API _declspec(dllimport)
	#pragma message("Automatically linking with ProfUIS library")
	#pragma comment( lib, __PROF_UIS_LIB_NAME ) 
#endif

#if defined __PROF_UIS_ADV_IMPL__
	#if (!defined __ENABLED_PROF_UIS_ADV__)
		#define __ENABLED_PROF_UIS_ADV__
	#endif
#endif

#if (defined __ENABLED_PROF_UIS_ADV__)
	// Prof-UIS advanced library
	// (now for internal use only)
	#ifdef _DEBUG
		#define __PROF_UIS_ADV_NAME_		_T("ProfUISAdvd")
		#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdvd.dll")
		#define __PROF_UIS_ADV_LIB_NAME		_T("ProfUISAdvd.lib")
	#else
		#define __PROF_UIS_ADV_NAME_		_T("ProfUISAdv")
		#define __PROF_UIS_ADV_MODULE_NAME	_T("ProfUISAdv.dll")
		#define __PROF_UIS_ADV_LIB_NAME		_T("ProfUISAdv.lib")
	#endif // _DEBUG

	#if defined __PROF_UIS_ADV_IMPL__
		#define __PROF_UIS_ADV_API _declspec(dllexport)
	#else 
		#define __PROF_UIS_ADV_API _declspec(dllimport)
		#if (!defined __PROF_UIS_IMPL__)
			#pragma message("Automatically linking with ProfUISAdv library")
			#pragma comment( lib, __PROF_UIS_ADV_LIB_NAME ) 
		#endif
	#endif
#endif // __ENABLED_PROF_UIS_ADV__

// Prof-UIS registry names
#define __PROF_UIS_REG_SECTION			_T("ProfUIS21")
#define __PROF_UIS_REG_PROFILES			_T("Profiles")
#define __PROF_UIS_REG_COMMAND_MANAGER	_T("Command manager")
#define __PROF_UIS_REG_CONTROL_BAR		_T("Control bar")

#endif // __EXT_MFC_DEF_H
