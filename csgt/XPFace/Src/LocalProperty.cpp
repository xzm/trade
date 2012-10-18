#include "stdafx.h"
#include "LocalProperty.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CLocalPropertySheet

CLocalPropertySheet::CLocalPropertySheet()
{
}

CLocalPropertySheet::~CLocalPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CLocalPropertySheet, CEnTabCtrl)
        //{{AFX_MSG_MAP(CLocalPropertySheet)
        ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Change the tab on the top
void CLocalPropertySheet::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetActivePage (GetCurSel ());
	*pResult = 0;
}


////////////////////////////////////////////////////////////////////////////

// This page will be the active page
int CLocalPropertySheet::SetActivePage (int nPage) {
	CLocalPropertyPage*	pPropPage;

	int		nN;
	if (nPage < 0 || nPage > m_nPages) nPage = 0;
	
	// get the actual page
	pPropPage = (CLocalPropertyPage *) m_Dialogs.GetAt (m_nActPage);
	// deactevate it
	nN = pPropPage->m_pDialogPage->SetWindowPos (NULL,
		pPropPage->m_Rect.left, pPropPage->m_Rect.top, 0, 0, SWP_HIDEWINDOW |
		SWP_NOSIZE);

	// get the new page
	pPropPage = (CLocalPropertyPage *) m_Dialogs.GetAt (nPage);
	nN = pPropPage->m_pDialogPage->SetWindowPos (NULL,
		pPropPage->m_Rect.left, pPropPage->m_Rect.top, 0, 0, SWP_SHOWWINDOW |
		SWP_NOSIZE);
	
	m_nActPage = nPage;

	// set the CEnTabCtrl-element
	SetCurSel(m_nActPage);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////

// Set this page as the active page
int CLocalPropertySheet::SetActivePage (CLocalPropDialog* pPage){
	CLocalPropertyPage*	pPropPage;
	int	nPage, nPageFound=0;
	for (nPage = 0; nPage <= m_nPages; nPage++) {
		pPropPage = (CLocalPropertyPage *) m_Dialogs.GetAt (nPage);
		if (pPropPage->m_pDialogPage == pPage) {
			nPageFound = nPage;
			break;
		}
	}

	SetActivePage (nPageFound);
	
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////

// Show the next or previous page
int CLocalPropertySheet::DispPage (int nCommand){
	int	nPage;
	nPage = m_nActPage;

	if (nCommand == ID_NEXT_FIELD) 
		nPage++;
	else
		nPage--;

	// are there correct page numbers?
	if (nPage < 0) nPage = m_nPages;
	if (nPage > m_nPages) nPage = 0;

	SetActivePage (nPage);
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////

// Get page number X
CLocalPropDialog* CLocalPropertySheet::GetPage (int nPage){
	CLocalPropertyPage*	pPropPage;

	if (nPage < 0 || nPage > m_nPages) nPage = 0;

	pPropPage = (CLocalPropertyPage *) m_Dialogs.GetAt (nPage);
	return pPropPage->m_pDialogPage;
}

////////////////////////////////////////////////////////////////////////////

// Get the active = current page
CLocalPropDialog* CLocalPropertySheet::GetActivePage (void){
	CLocalPropertyPage*	pPropPage;

	pPropPage = (CLocalPropertyPage *) m_Dialogs.GetAt (m_nActPage);
	return pPropPage->m_pDialogPage;
	
}

////////////////////////////////////////////////////////////////////////////

// Get the number of possible pages
int CLocalPropertySheet::GetPageCount (void) {
	return	m_nPages;
}
	

////////////////////////////////////////////////////////////////////////////

// CLocalPropDialog 
CLocalPropDialog::CLocalPropDialog(CWnd* pParent /*=NULL*/)
	: CDialog()
//	: CDialog(CLocalPropDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocalPropDialog)
		// 
	//}}AFX_DATA_INIT
}


void CLocalPropDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocalPropDialog)
		// 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocalPropDialog, CDialog)
	//{{AFX_MSG_MAP(CLocalPropDialog)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////

// Messages for CLocalPropDialog 

////////////////////////////////////////////////////////////////////////////

// handle CTRL-PGUP & PGDN
BOOL CLocalPropDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO:

	//First Property Sheet tab key translation
	//Ctrl+PageUp, and Ctrl+PageDown 

	if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) < 0 &&
		pMsg->wParam == VK_PRIOR) {
		((CLocalPropertySheet *) GetParent ())->DispPage (ID_PREV_FIELD);
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) < 0 &&
		pMsg->wParam == VK_NEXT) {
		((CLocalPropertySheet *) GetParent ())->DispPage (ID_NEXT_FIELD);
		return TRUE;
	}
		
	
	return CDialog::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////////////////////////

// 
BOOL CLocalPropDialog::OnCmdMsg(UINT nID, int nCode, void* pExtra,
AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: do nothing here
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


////////////////////////////////////////////////////////////////////////////

// CEnTabCtrl-Handling
BOOL CLocalPropertySheet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO:

	// First Property Sheet tab key translation
	// Ctrl+PageUp, and Ctrl+PageDown 

	if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) < 0 &&
		pMsg->wParam == VK_PRIOR) {
		DispPage (ID_PREV_FIELD);
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) < 0 &&
		pMsg->wParam == VK_NEXT) {
		DispPage (ID_NEXT_FIELD);
		return TRUE;
	}
		
	return CEnTabCtrl::PreTranslateMessage(pMsg);
}

void CLocalPropertySheet::PostNcDestroy() 
{
	// TODO: Free and unlock
	
	int		nN;
	CLocalPropertyPage*	pPropPage;

	// pPropPages deleten
	for (nN = 0; nN <= m_nPages; nN++) {
		//  get page per page
		pPropPage = (CLocalPropertyPage *) m_Dialogs.GetAt (0);
		if (pPropPage->m_hLocal) {	// you created the dialog on the fly?
			LocalUnlock (pPropPage->m_hLocal);	//pBuffer =
							// (BYTE*)LocalLock(pPropPage->m_hLocal);
			LocalFree (pPropPage->m_hLocal);	// LocalAlloc(LHND, nBufferSize);
		}
		m_Dialogs.RemoveAt (0);
		delete (pPropPage);
	}
	CEnTabCtrl::PostNcDestroy();
	// delete (this);		// free Resources
	// only needed if you work with NEW
}

void CLocalPropDialog::PostNcDestroy() 
{
	// TODO: 
	
	CDialog::PostNcDestroy();
	delete (this);		// free it
}


////////////////////////////////////////////////////////////////////////////
// Examples
/*	m_sheet.m_Rect.left = 188;	// set the position on the screen
	m_sheet.m_Rect.top = 10;
	m_sheet.m_Rect.right = 515;
	m_sheet.m_Rect.bottom = 295;
	m_sheet.m_nPages = -1;		// init this membervariable
	m_sheet.m_nActPage = 0;		// init this membervariable
	m_sheet.m_nCtrlID = 2000;	// control-id in the dialog

	// and create it on the screen
	if( !m_sheet.Create( WS_VISIBLE | WS_CHILD | WS_TABSTOP,
			m_sheet.m_Rect, this, m_sheet.m_nCtrlID) )
		return FALSE;

	// Now add the dialogs page per page
	TC_ITEM Item;
	Item.mask = TCIF_TEXT;
	Item.pszText = "MA";
//	m_sheet.InsertItem (0, &Item);	// this is fot CTabWnd

	CLocalPropertyPage * pPage = new CLocalPropertyPage();
	pPage->m_strTitle = "MA";
	pPage->m_pDialogPage = (CLocalPropDialog *) &m_ma;
	pPage->m_pDialogPage->Create (IDDP_MA, &m_sheet);
	pPage->m_hLocal = NULL;	// nothing is created on the fly
							// important information on delete!

	// add it to the array
	m_sheet.m_Dialogs.Add (pPage);
	m_sheet.m_nPages++;	// one more page


	// the size of CTabWnd is m_rect
	// the size of the dialog is smaller
	pPage->m_Rect.top	= 30;	// above there must be enough place for the tab-control
	pPage->m_Rect.left	= 10;	// border of 10 units is good
	pPage->m_Rect.bottom= m_sheet.m_Rect.bottom - m_sheet.m_Rect.top - 30;
	pPage->m_Rect.right	= m_sheet.m_Rect.right - m_sheet.m_Rect.left - 10;

	pPage->m_pDialogPage->SetWindowPos(NULL, pPage->m_Rect.left,
						pPage->m_Rect.top, 0, 0,
						SWP_SHOWWINDOW | SWP_NOSIZE);
	
*/	


