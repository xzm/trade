// AfxCore.inl : implementation file
//

/////////////////////////////////////////////////////////////////////////////
// AfxCore

// Assert Valid Functions
_AFXCORE_INLINE void AfxAssertNull( )
{
	CStockInfo	infoTemp, info;
	CMathParser	parser;
	parser.Parse( );
}

_AFXCORE_INLINE BOOL AfxAssertValid( )
{
	BOOL	b	=	AfxGetSView().AssertValid();
	AfxAssertNull();
	if( b )
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return b;
}

_AFXCORE_INLINE BOOL AfxAssertValidStd( )
{
	BOOL	b	=	AfxGetSView().IsStd();
	AfxAssertNull();
	if( b )
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return b;
}

_AFXCORE_INLINE BOOL AfxAssertValidProf( )
{
	BOOL	b	=	AfxGetSView().IsProf();
	AfxAssertNull();
	if( b )
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return b;
}

_AFXCORE_INLINE BOOL AfxAssertFunclimitStd( UINT nIDString )
{
	if( !AfxGetSView().IsStd() && !AfxGetSView().IsProf() )
	{
		AfxAssertNull();
		AfxMessageBox( nIDString, MB_OK | MB_ICONINFORMATION );
		AfxAssertNull();
		return FALSE;
	}
	else
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return TRUE;
}

_AFXCORE_INLINE BOOL AfxAssertFunclimitProf( UINT nIDString )
{
	if( !AfxGetSView().IsProf() )
	{
		AfxAssertNull();
		AfxMessageBox( nIDString, MB_OK | MB_ICONINFORMATION );
		AfxAssertNull();
		return FALSE;
	}
	else
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return TRUE;
}

_AFXCORE_INLINE BOOL AfxAssertRegisterQuit( )
{
	if( !AfxGetSView().AssertValid() )
	{
		AfxAssertNull();
		CRegisterDlg	dlg;
		dlg.DoModal();
		AfxAssertQuit();
		return FALSE;
	}
	else
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return TRUE;
}

_AFXCORE_INLINE BOOL AfxAssertMessageQuit( )
{
	if( !AfxGetSView().AssertValid() )
	{
		AfxAssertNull();
		AfxMessageBox( IDS_SECRET_ASSERTMESSAGE, MB_OK | MB_ICONINFORMATION );
		AfxGetToolsApp()->SaveAllModified();
		AfxGetMainFrame()->PostMessage( WM_CLOSE );
		AfxGetMainFrame()->CreateValid( 150, WM_CLOSE );
		return FALSE;
	}
	else
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return TRUE;
}

_AFXCORE_INLINE BOOL AfxAssertQuit( )
{
	if( !AfxGetSView().AssertValid() )
	{
		AfxAssertNull();
		AfxGetToolsApp()->SaveAllModified();
		AfxGetMainFrame()->PostMessage( WM_CLOSE );
		AfxGetMainFrame()->CreateValid( 150, WM_CLOSE );
		return FALSE;
	}
	else
	{
		AfxGetSView().Load( AfxGetProfile().GetSViewFile() );
	}
	return TRUE;
}

_AFXCORE_INLINE	CToolsApp * AfxGetToolsApp( )
{
	CToolsApp * pApp = (CToolsApp *)AfxGetApp();
	ASSERT( pApp );
	return pApp;
}

_AFXCORE_INLINE	CMainFrame	* AfxGetMainFrame( )
{
	CMainFrame * pFrame = DYNAMIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	ASSERT( pFrame && pFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)) );
	return pFrame;
}

