// AfxCore.cpp : implementation of the AfxCore Functions
//

#include	"stdafx.h"
#include	"AfxCore.h"

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, CSPTime& value)
{
	CTime	tm	=	value.GetTime();
	DDX_DateTimeCtrl(pDX, nIDC,tm);
	value	=	tm.GetTime();
}

AFX_STATIC_DATA const TCHAR _afxShellOpenFmt[] = _T("%s\\shell\\open\\%s");
AFX_STATIC_DATA const TCHAR _afxShellPrintFmt[] = _T("%s\\shell\\print\\%s");
AFX_STATIC_DATA const TCHAR _afxShellPrintToFmt[] = _T("%s\\shell\\printto\\%s");
AFX_STATIC_DATA const TCHAR _afxDefaultIconFmt[] = _T("%s\\DefaultIcon");
AFX_STATIC_DATA const TCHAR _afxShellNewFmt[] = _T("%s\\ShellNew");

#define DEFAULT_ICON_INDEX 0

AFX_STATIC_DATA const TCHAR _afxIconIndexFmt[] = _T(",%d");
AFX_STATIC_DATA const TCHAR _afxCommand[] = _T("command");
AFX_STATIC_DATA const TCHAR _afxOpenArg[] = _T(" \"%1\"");
AFX_STATIC_DATA const TCHAR _afxPrintArg[] = _T(" /p \"%1\"");
AFX_STATIC_DATA const TCHAR _afxPrintToArg[] = _T(" /pt \"%1\" \"%2\" \"%3\" \"%4\"");
AFX_STATIC_DATA const TCHAR _afxDDEArg[] = _T(" /dde");

AFX_STATIC_DATA const TCHAR _afxDDEExec[] = _T("ddeexec");
AFX_STATIC_DATA const TCHAR _afxDDEOpen[] = _T("[open(\"%1\")]");
AFX_STATIC_DATA const TCHAR _afxDDEPrint[] = _T("[print(\"%1\")]");
AFX_STATIC_DATA const TCHAR _afxDDEPrintTo[] = _T("[printto(\"%1\",\"%2\",\"%3\",\"%4\")]");

AFX_STATIC_DATA const TCHAR _afxShellNewValueName[] = _T("NullFile");
AFX_STATIC_DATA const TCHAR _afxShellNewValue[] = _T("");

void AfxUnregisterShellFileTypes()
{
	CString strPathName, strTemp;

	AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

	CString	strMainFrame;
	VERIFY(strMainFrame.LoadString( IDR_MAINFRAME ));

	CString strFilterExt, strFileTypeId, strFileTypeName;
	if (AfxExtractSubString( strFileTypeId,strMainFrame,
	   CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
	{
		// enough info to register it
		if (!AfxExtractSubString( strFileTypeName, strMainFrame,
		   CDocTemplate::regFileTypeName))
			strFileTypeName = strFileTypeId;    // use id name

		ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

		strTemp.Format(_afxDefaultIconFmt, (LPCTSTR)strFileTypeId);
		AfxDeleteRegKey(strTemp);

		// path\shell\open\command = path filename
		strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
			(LPCTSTR)_afxCommand);
		AfxDeleteRegKey(strTemp);

		// path\shell\print\command = path /p filename
		strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
			(LPCTSTR)_afxCommand);
		AfxDeleteRegKey(strTemp);

		// path\shell\printto\command = path /pt filename printer driver port
		strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
			(LPCTSTR)_afxCommand);
		AfxDeleteRegKey(strTemp);

		AfxExtractSubString( strFilterExt, strMainFrame, CDocTemplate::filterExt);
		if (!strFilterExt.IsEmpty())
		{
			ASSERT(strFilterExt[0] == '.');

			LONG lSize = _MAX_PATH * 2;
			LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
				strTemp.GetBuffer(lSize), &lSize);
			strTemp.ReleaseBuffer();

			if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
				strTemp == strFileTypeId)
			{
				strTemp.Format(_afxShellNewFmt, (LPCTSTR)strFilterExt);
				AfxDeleteRegKey(strTemp);

				// no association for that suffix
				AfxDeleteRegKey(strFilterExt);
			}
		}
	}
}

void AfxRegisterShellFileTypes(BOOL bCompat)
{
	CString strPathName, strTemp;

	AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

	CString strOpenCommandLine = strPathName;
	CString strPrintCommandLine = strPathName;
	CString strPrintToCommandLine = strPathName;
	CString strDefaultIconCommandLine = strPathName;

	CString	strMainFrame;
	VERIFY(strMainFrame.LoadString( IDR_MAINFRAME ));

	if (bCompat)
	{
		CString strIconIndex;
		HICON hIcon = ::ExtractIcon(AfxGetInstanceHandle(), strPathName, 1);
		if (hIcon != NULL)
		{
			strIconIndex.Format(_afxIconIndexFmt, 1);
			DestroyIcon(hIcon);
		}
		else
		{
			strIconIndex.Format(_afxIconIndexFmt, DEFAULT_ICON_INDEX);
		}
		strDefaultIconCommandLine += strIconIndex;
	}

	CString strFilterExt, strFileTypeId, strFileTypeName;

	if (AfxExtractSubString( strFileTypeId, strMainFrame,
		CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
	{
		// enough info to register it
		if (!AfxExtractSubString( strFileTypeName, strMainFrame, 
		   CDocTemplate::regFileTypeName))
			strFileTypeName = strFileTypeId;    // use id name

		ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

		// first register the type ID of our server
		if (!AfxSetRegKey(strFileTypeId, strFileTypeName))
			return;       // just skip it

		if (bCompat)
		{
			// path\DefaultIcon = path,1
			strTemp.Format(_afxDefaultIconFmt, (LPCTSTR)strFileTypeId);
			if (!AfxSetRegKey(strTemp, strDefaultIconCommandLine))
				return;       // just skip it
		}

		// path\shell\open\command = path filename
		// path\shell\print\command = path /p filename
		// path\shell\printto\command = path /pt filename printer driver port
		strOpenCommandLine += _afxOpenArg;
		if (bCompat)
		{
			strPrintCommandLine += _afxPrintArg;
			strPrintToCommandLine += _afxPrintToArg;
		}

		// path\shell\open\command = path filename
		strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
			(LPCTSTR)_afxCommand);
		if (!AfxSetRegKey(strTemp, strOpenCommandLine))
			return;       // just skip it

		if (bCompat)
		{
			// path\shell\print\command = path /p filename
			strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			if (!AfxSetRegKey(strTemp, strPrintCommandLine))
				return;       // just skip it

			// path\shell\printto\command = path /pt filename printer driver port
			strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			if (!AfxSetRegKey(strTemp, strPrintToCommandLine))
				return;       // just skip it
		}

		AfxExtractSubString( strFilterExt, strMainFrame, CDocTemplate::filterExt);
		if (!strFilterExt.IsEmpty())
		{
			ASSERT(strFilterExt[0] == '.');

			LONG lSize = _MAX_PATH * 2;
			LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
				strTemp.GetBuffer(lSize), &lSize);
			strTemp.ReleaseBuffer();

			if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
				strTemp == strFileTypeId)
			{
				// no association for that suffix
				if (!AfxSetRegKey(strFilterExt, strFileTypeId))
					return;

				if (bCompat)
				{
					strTemp.Format(_afxShellNewFmt, (LPCTSTR)strFilterExt);
					(void)AfxSetRegKey(strTemp, _afxShellNewValue, _afxShellNewValueName);
				}
			}
		}
	}
}

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

AFX_STATIC void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
											  CString *pstrDefaultExt)
{
	CString	strMainFrame;
	VERIFY(strMainFrame.LoadString( IDR_MAINFRAME ));

	CString strFilterExt, strFilterName;
	if (AfxExtractSubString(strFilterExt, strMainFrame, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 AfxExtractSubString(strFilterName, strMainFrame, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}

BOOL AfxDoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog )
{
	CKSFileDialog dlgFile(bOpenFileDialog);

	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;
	
	CString strFilter;
	CString strDefault;
	
	_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn,&strDefault);

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	CString	strPath = dlgFile.GetPathName( );
	return nResult == IDOK;
}

BOOL AfxDoSelectDirectory( CString & strSrc, HWND hWnd )
{
/*	The Old Version */
/*
	BROWSEINFO bi;
	char szWorkDir[MAX_PATH+1]	=	"";
	LPITEMIDLIST piidlRoot;
	SHGetSpecialFolderLocation(hWnd, CSIDL_DRIVES, &piidlRoot);
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = hWnd;
	bi.pidlRoot = piidlRoot;
	bi.pszDisplayName = szWorkDir;
	CString	strTitle;
	strTitle.LoadString( IDS_SELECT_DIRECTORY );
	if( GetSystemMetrics(SM_DBCSENABLED) )
	{
		bi.lpszTitle = strTitle.GetBuffer(strTitle.GetLength()+1);
	}
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	LPITEMIDLIST	piidlTarget = SHBrowseForFolder(&bi);
	if( piidlTarget )
	{
		::SHGetPathFromIDList( piidlTarget, szWorkDir ) ;
		strSrc		=	szWorkDir;
		::CoTaskMemFree( piidlTarget );
		return TRUE;
	}
	return FALSE;
*/
	/* The New Version Using CPathDialog */
	CString strCaption;
	CString strTitle;
	strCaption.LoadString( IDS_SELECT_DIRECTORY );
	strTitle.LoadString( IDS_SELECT_DIRECTORY );

	CPathDialog dlg(strCaption, strTitle, strSrc);

	if(dlg.DoModal()==IDOK
		&& CPathDialog::MakeSurePathExists(dlg.GetPathName())==0 )
	{
		strSrc	=	dlg.GetPathName();
		return TRUE;
	}
	return FALSE;
}

CString	AfxGetPrjFileExt( )
{
	CString	strMainFrame;
	strMainFrame.LoadString( IDR_MAINFRAME );
	CString strExt;
	VERIFY( AfxExtractSubString( strExt, strMainFrame,CDocTemplate::filterExt) );
	ASSERT( !strExt.IsEmpty() && strExt[0] == '.');
	return strExt;
}

#ifdef AFX_CORE2_SEG
#pragma code_seg(AFX_CORE2_SEG)
#endif

#ifdef AFX_TERM_SEG
#pragma code_seg(AFX_TERM_SEG)
#endif

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

/////////////////////////////////////////////////////////////////////////////
