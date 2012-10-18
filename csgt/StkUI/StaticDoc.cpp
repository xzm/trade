// StaticDoc.cpp : implementation of the CStaticDoc class
//

#include "stdafx.h"
#include "StkUI.h"

#include "StaticDoc.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticDoc

IMPLEMENT_DYNCREATE(CStaticDoc, CDocument)

BEGIN_MESSAGE_MAP(CStaticDoc, CDocument)
	//{{AFX_MSG_MAP(CStaticDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CStaticDoc* CStaticDoc::CreateNewDocument( )
{
	CRuntimeClass *pDocClass = RUNTIME_CLASS(CStaticDoc);
	CStaticDoc* pDocument = (CStaticDoc*)pDocClass->CreateObject();
	if (pDocument == NULL)
	{
		TRACE1("Warning: Dynamic create of document type %hs failed.\n",
			pDocClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CStaticDoc, pDocument);
	return pDocument;
}

CStaticDoc* CStaticDoc::OpenDocumentFile( LPCTSTR lpszPathName )
{
	// Resolve File Name
	TCHAR szPath[_MAX_PATH];
	ASSERT(lstrlen(lpszPathName) < sizeof(szPath));
	if( NULL != lpszPathName )
	{
		TCHAR szTemp[_MAX_PATH];
		if (lpszPathName[0] == '\"')
			++lpszPathName;
		lstrcpyn(szTemp, lpszPathName, _MAX_PATH);
		LPTSTR lpszLast = _tcsrchr(szTemp, '\"');
		if (lpszLast != NULL)
			*lpszLast = 0;
		AfxFullPath(szPath, szTemp);
		TCHAR szLinkName[_MAX_PATH];
		if (AfxResolveShortcut(AfxGetMainWnd(), szPath, szLinkName, _MAX_PATH))
			lstrcpy(szPath, szLinkName);
	}

	// Create document
	CStaticDoc *pDocument = CreateNewDocument( );
	if( NULL == pDocument )
		return NULL;

	if (lpszPathName == NULL)
	{
		// create a new document
		if (!pDocument->OnNewDocument())
		{
			// user has been alerted to what failed in OnNewDocument
			TRACE0("CStaticDoc::OnNewDocument returned FALSE.\n");
			delete pDocument;
			return NULL;
		}
	}
	else
	{
		CWaitCursor wait;

		// open an existing document
		BOOL bWasModified = pDocument->IsModified();
		pDocument->SetModifiedFlag(FALSE);  // not dirty for open

		if (!pDocument->OnOpenDocument( szPath ))
		{
			// user has been alerted to what failed in OnOpenDocument
			TRACE0("CStaticDoc::OnOpenDocument returned FALSE.\n");
			if (!pDocument->IsModified())
			{
				// original document is untouched
				pDocument->SetModifiedFlag(bWasModified);
			}
			else
			{
				// we corrupted the original document
				if (!pDocument->OnNewDocument())
				{
					TRACE0("Error: OnNewDocument failed after trying to open a document - trying to continue.\n");
					// assume we can continue
				}
			}
			delete pDocument;
			return NULL;        // open failed
		}
		pDocument->SetPathName(szPath);
	}
	return pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// CStaticDoc construction/destruction

CStaticDoc::CStaticDoc()
{
	// TODO: add one-time construction code here
	m_bAutoDelete	=	FALSE;
}

CStaticDoc::~CStaticDoc()
{
}

BOOL CStaticDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


void CStaticDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class

	CDocument::OnCloseDocument();
}

UINT CStaticDoc::GetIDResource( )
{
	return IDR_SIMTYPE; //MAINFRAME;
}

CView * CStaticDoc::GetViewIfExist( CRuntimeClass *pViewClass )
{
	POSITION pos = GetFirstViewPosition( );
	CView * pView = NULL;
	while( pView = GetNextView(pos) )
	{
		if( pView && pView->IsKindOf( pViewClass ) )
		{
			return pView;
		}
	}
	return NULL;
}

CView* CStaticDoc::GetActiveView( )
{
	CMainFrame * pMainFrame = AfxGetMainFrame();
	if( pMainFrame )
	{
		CChildFrame * pChildFrame = DYNAMIC_DOWNCAST( CChildFrame, pMainFrame->MDIGetActive() );
		if( pChildFrame )
		{
			CView * pView = pChildFrame->GetActiveView();
			if( pView && this == pView->GetDocument() )
				return pView;
		}
	}
	return NULL;
}


BOOL CStaticDoc::ShowStaticView( CRuntimeClass * pViewClass, BOOL bMaximized )
{
	if( NULL == pViewClass )
		return FALSE;

	CChildFrame * pChildFrame = NULL;

	POSITION pos = GetFirstViewPosition( );
	CView * pView = NULL;
	while( pView = GetNextView(pos) )
	{
		if( pView->IsKindOf( pViewClass ) )
		{
			pChildFrame = DYNAMIC_DOWNCAST( CChildFrame, pView->GetParentFrame() );
			ASSERT( pChildFrame );
			if( pChildFrame )
				break;
		}
	}

	if( NULL == pChildFrame )
	{
		// create frame
		pChildFrame = CChildFrame::CreateNewFrame( this );
		ASSERT( pChildFrame );

		if( pChildFrame )
		{
			// create view
			CCreateContext context;
			context.m_pCurrentDoc = this;
			context.m_pCurrentFrame = pChildFrame;
			context.m_pLastView = NULL;
			context.m_pNewDocTemplate = NULL;
			context.m_pNewViewClass = pViewClass;

			CView * pNewView = DYNAMIC_DOWNCAST(CView,pChildFrame->CreateView( &context, AFX_IDW_PANE_FIRST));
			if( pNewView )
			{
				pChildFrame->SetActiveView( pNewView );
				pNewView->OnInitialUpdate( );
			}

			// if no active child frame, maximize this frame
			CMainFrame * pMainFrame = AfxGetMainFrame();
			if( pMainFrame )
			{
				CFrameWnd * pActiveFrame = AfxGetMainFrame()->GetActiveFrame();
				if( !pActiveFrame || !pActiveFrame->IsKindOf(RUNTIME_CLASS(CChildFrame)) )
					pChildFrame->MDIMaximize();
			}
		}
	}
	
	if( pChildFrame )
	{
		// activate it
		pChildFrame->MDIActivate();
		if( bMaximized )
			pChildFrame->MDIMaximize();
		else
			pChildFrame->ShowWindow( SW_SHOW );
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CStaticDoc serialization

void CStaticDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CStaticDoc diagnostics

#ifdef _DEBUG
void CStaticDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStaticDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStaticDoc commands

void CStaticDoc::SetTitle(LPCTSTR lpszTitle) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_strTitle = lpszTitle;
}

BOOL CStaticDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
	// Save the document data to a file
	// lpszPathName = path name where to save document file
	// if lpszPathName is NULL then the user will be prompted (SaveAs)
	// note: lpszPathName can be different than 'm_strPathName'
	// if 'bReplace' is TRUE will change file name if successful (SaveAs)
	// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			if (newName.IsEmpty())
				VERIFY(newName.LoadString(AFX_IDS_UNTITLED));
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(" #%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			newName += AfxGetStrategyFileExt( );
		}

		if( !AfxDoPromptFileName(newName, bReplace ? IDS_SAVEFILE : IDS_SAVEFILECOPY,
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING, FALSE ) )
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			try
			{
				CFile::Remove(newName);
			}
			catch(CException *e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				if( !e->m_bAutoDelete )
					e->Delete();
			}
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName);

	return TRUE;        // success
}

BOOL CStaticDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	return TRUE;
	// return CDocument::SaveModified();
}
