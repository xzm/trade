// WorkspBar.cpp: implementation of the CWorkspBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorkspBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
	CWorkspBar Window
           |
           |
           ------- CGuiTabWnd(or CGuiWorkTab) Window
                          |
                          |
                          ------------- View 1
                          |
                          ------------- View 2
                          |
                          ------------- View 3


	Old Version:
	
	CWorkspBar Window
           |
           |
           ------- CGuiTabWnd(or CGuiWorkTab) Window
           |
           |
           ------------- View 1
           |
           ------------- View 2
           |
           ------------- View 3
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkspBar::CWorkspBar()
{
	m_nActiveTab = 0;
	m_pFrameWnd	=	NULL;
}

CWorkspBar::~CWorkspBar()
{
	while(!m_views.IsEmpty())
	{
		TCB_ITEM *pMember=m_views.RemoveHead();
		delete pMember;
	}
}

#define IDC_TABCTRLBAR 1000

BEGIN_MESSAGE_MAP(CWorkspBar, CGuiControlBar)
	//{{AFX_MSG_MAP(CWorkspBar)
	ON_WM_CREATE()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCPAINT()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRLBAR, OnTabSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspBar message handlers

// *** K.Stowell
int CWorkspBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Create the Tab Control
	if (!m_tabctrl.Create(WS_VISIBLE|WS_CHILD|TCS_BOTTOM|TCS_TOOLTIPS,
		CRect(0,0,0,0), this, IDC_TABCTRLBAR))
	{
		TRACE0("Unable to create tab control bar\n");
		return -1;
	}

	// set "normal" GUI-font
	CFont *font = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	m_tabctrl.SetFont(font);
	
/*
// VC5 Support.
#if _MSC_VER >= 1200
	m_ToolTip = m_tabctrl.GetToolTips();
	m_tabctrl.SetToolTips(m_ToolTip);
#else
	m_ToolTip = m_tabctrl.GetTooltips();
	m_tabctrl.SetTooltips(m_ToolTip);
#endif
*/
	return 0;
}

void CWorkspBar::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	CRect rc;
	CRect rcT=m_rcCloseBtn;
	ClientToScreen(rcT);
	point.y+=23;
	point.x+=5;
	if (rcT.PtInRect(point))
	{
		if (m_stateBtn ==PRESS)
		{
			m_stateBtn=NORMAL;
			KillTimer(1);
			//The Old GetDockingFrame()->ShowControlBar(this, FALSE, FALSE);
			ShowNormal( !IsSizeNormal() );
		}
		//SendMessage(WM_NCPAINT);
		return;
	}
}

void CWorkspBar::OnNcPaint()
{
	if( IsFloating() )
	{
		m_CloseBtn.SetData( 6, "¹Ø±Õ" );
	}
	else if( IsSizeNormal() )
	{
		m_CloseBtn.SetData( 13, "Òþ²Ø" );
		// m_CloseBtn.SetImageList( IDB_WORKSPBAR_BTN, 9, 2, RGB(0,255,0) );
	}
	else
	{
		m_CloseBtn.SetData( 12, "Õ¹¿ª" );
		// m_CloseBtn.SetImageList( IDB_WORKSPBAR_BTN, 9, 2, RGB(0,255,0) );
	}

	CGuiControlBar::OnNcPaint();
}

BOOL CWorkspBar::IsSizeNormal( )
{
	if( !::IsWindow(GetSafeHwnd()) )
		return FALSE;

	CRect	rect;
	GetWindowRect( &rect );
	return ( rect.Width() > 50 );
}

void CWorkspBar::ShowNormal( BOOL bShowNormal )
{
	if( !::IsWindow( GetSafeHwnd() ) )
		return;

	CRect	rect;
	GetWindowRect( &rect );
	if( bShowNormal )
	{
		if( !IsSizeNormal() )
		{
			m_sizeVert.cx	=	210;
			m_tabctrl.ResetDeplaza( );
			RecalWindowPos();
		}
		if( !IsWindowVisible() )
		{
			CFrameWnd * pFrame = DYNAMIC_DOWNCAST(CFrameWnd,m_pFrameWnd);
			if( pFrame )
				pFrame->ShowControlBar( this, TRUE, FALSE );
		}
	}
	else
	{
		if( IsFloating() )
		{
			CFrameWnd * pFrame = DYNAMIC_DOWNCAST(CFrameWnd,m_pFrameWnd);
			if( pFrame )
				pFrame->ShowControlBar( this, FALSE, FALSE );
		}
		else
		{
			m_sizeVert.cx	=	30;
			RecalWindowPos();
		}
	}
}

void CWorkspBar::ToggleShow( )
{
	if( !::IsWindow(GetSafeHwnd()) )
		return;

	if( IsWindowVisible() )
	{
		CFrameWnd * pFrame = DYNAMIC_DOWNCAST(CFrameWnd,m_pFrameWnd);
		if( pFrame )
			pFrame->ShowControlBar( this, FALSE, FALSE );
	}
	else
		ShowNormal( TRUE );
}

// *** K.Stowell
void CWorkspBar::SetTabImageList( UINT nBitmapID, int cx, int nGrow, COLORREF crMask )
{
	m_tabctrl.SetImageList( nBitmapID, cx, nGrow, crMask );
}

// *** K.Stowell
BOOL CWorkspBar::ModifyTabStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	return m_tabctrl.ModifyStyle(dwRemove, dwAdd);
}

// *** KStowell
void CWorkspBar::OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetActiveView(m_tabctrl.GetCurtab());
	Invalidate();
}

//////////////////////////////////////////////////
// The remainder of this class was written by Dirk Clemens...

BOOL CWorkspBar::AddView(LPCTSTR lpszLabel, CRuntimeClass *pViewClass, CCreateContext *pContext)
{	

#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

	CCreateContext context;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the currently selected
		//  client if possible
		CView* pOldView = NULL;
		if (pOldView != NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// set info about last pane
			ASSERT(context.m_pCurrentFrame == NULL);
			context.m_pLastView = pOldView;
			context.m_pCurrentDoc = pOldView->GetDocument();
			if (context.m_pCurrentDoc != NULL)
				context.m_pNewDocTemplate =
				context.m_pCurrentDoc->GetDocTemplate();
		}
		pContext = &context;
	}
	
	CWnd* pWnd;
	try
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	catch(...)
	{
		TRACE0("Out of memory creating a view.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	
    ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created
	
	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	CRect rect = CRect(0,0,0,0);
	// Create with the right size and position
	if (!pWnd->Create(NULL, NULL, dwStyle, rect, /*CJLib Version this*/&m_tabctrl, 1, pContext))
	{
		TRACE0("Warning: couldn't create client pane for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}
	m_pActiveView = (CView*) pWnd;

	TCB_ITEM *pMember=new TCB_ITEM;
	pMember->pWnd=pWnd;
	strcpy(pMember->szLabel, lpszLabel);
	m_views.AddTail(pMember);

	/*
	// ToolTip support for tabs.
	if((m_views.GetCount()-1)==0) {
		m_ToolTip->AddTool( &m_tabctrl, lpszLabel,
			NULL, m_views.GetCount()-1 );
	}
	else {
		m_ToolTip->AddTool( &m_tabctrl, lpszLabel,
			CRect(0,0,0,0), m_views.GetCount()-1 );
	}
	*/
	
	int nViews = m_views.GetCount();
	if (nViews!=1)
	{
		// pWnd->EnableWindow(FALSE);
		pWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		// MODIFIED BY SUNZHENYU ((CFrameWnd *)GetParent())->SetActiveView((CView *)m_pActiveView);
	}

	//TC_ITEM tci;
	//tci.mask = TCIF_TEXT | TCIF_IMAGE;
	//tci.pszText = (LPTSTR)(LPCTSTR)lpszLabel;
	//tci.iImage = nViews-1;
	//m_tabctrl.InsertItem(nViews, &tci);
	m_tabctrl.Addtab( pWnd, lpszLabel, nViews-1 );
	
	return TRUE;
}

void CWorkspBar::RemoveView(int nView)
{
	ASSERT_VALID(this);
	ASSERT(nView >= 0);

	// remove the page from internal list
	m_views.RemoveAt(m_views.FindIndex(nView));
}

void CWorkspBar::SetActiveView(int nNewTab)
{
	ASSERT_VALID(this);
	ASSERT(nNewTab >= 0);

	if (nNewTab!=-1 && nNewTab!=m_nActiveTab)
	{
        TCB_ITEM *newMember=m_views.GetAt(m_views.FindIndex(nNewTab));
        TCB_ITEM *oldMember=NULL;
		
        if (m_nActiveTab!=-1)
        {
            oldMember=m_views.GetAt(m_views.FindIndex(m_nActiveTab));
            //oldMember->pWnd->EnableWindow(FALSE);
            oldMember->pWnd->ShowWindow(SW_HIDE);
        }
        newMember->pWnd->EnableWindow(TRUE);
        newMember->pWnd->ShowWindow(SW_SHOW);
        newMember->pWnd->SetFocus();

        m_pActiveView = (CView *)newMember->pWnd;
		// MODIFIED BY SUNZHENYU ((CFrameWnd *)GetParent())->SetActiveView(m_pActiveView);

        m_nActiveTab = nNewTab;
		// select the tab (if tab programmatically changed)
		m_tabctrl.SetCurtab(m_nActiveTab);
    }
}

void CWorkspBar::SetActiveView(CRuntimeClass *pViewClass)
{
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	int nNewTab = 0;
	for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos))
	{
		TCB_ITEM *pMember=m_views.GetAt(pos);
		if (pMember->pWnd->IsKindOf(pViewClass))
		{
			//first hide old first view
            //m_pActiveView->EnableWindow(FALSE);
            m_pActiveView->ShowWindow(SW_HIDE);
			
			// set new active view
			m_pActiveView = (CView*)pMember->pWnd;
			// enable, show, set focus to new view
			m_pActiveView->EnableWindow(TRUE);
			m_pActiveView->ShowWindow(SW_SHOW);
			m_pActiveView->SetFocus();
			
			// MODIFIED BY SUNZHENYU ((CFrameWnd *)GetParent())->SetActiveView(m_pActiveView);

	        m_nActiveTab = nNewTab;
			// select the tab
			m_tabctrl.SetCurtab(m_nActiveTab);

			break;
		}
		nNewTab++;
    }
}

CView* CWorkspBar::GetActiveView()
{
	return m_pActiveView;
}

CView* CWorkspBar::GetView(int nView)
{
	ASSERT_VALID(this);
	ASSERT(nView >= 0);

	if (nView!=-1)
	{
        TCB_ITEM *pMember=m_views.GetAt(m_views.FindIndex(nView));
		return (CView*)pMember->pWnd;
	}
	else
		return NULL;
}

CView* CWorkspBar::GetView(CRuntimeClass *pViewClass)
{
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos))
	{
		TCB_ITEM *pMember=m_views.GetAt(pos);
		if (pMember->pWnd->IsKindOf(pViewClass))
		{
			return (CView*)pMember->pWnd;
		}
    }
	return NULL;
}
