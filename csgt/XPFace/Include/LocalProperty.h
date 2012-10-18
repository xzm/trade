// ExampelDlg.h : Header-Datei
//

#if !defined(AFX_CPROPERTYG_H__3F20C546_869F_11D2_B6F4_0000C0E36C6E__INCLUDED_)
#define AFX_CPROPERTY_H__3F20C546_869F_11D2_B6F4_0000C0E36C6E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define	ID_NEXT_FIELD		1
#define	ID_PREV_FIELD	2

class	CLocalPropDialog;

////////////////////////////////////////////////////////////////////////////

// Class to handle the pages
class CLASS_EXPORT CLocalPropertyPage : public CObject {
public:
	DLGTEMPLATE	m_dlgTemplate;	// Templatestructure
	DLGTEMPLATE	*m_pResource;	// I create my dialogs on the fly
					// so I need a pointer to the memory
	HLOCAL			m_hLocal;		// Memory-Handle
	CString			m_strTitle;
	CLocalPropDialog	*m_pDialogPage;	// pointer to the dialog structur
	CRect			m_Rect;		// size of the dialog on the screen
	
};

 ////////////////////////////////////////////////////////////////////////////

// Class to handle the TabCtrl
class CLocalPropertySheet : public CEnTabCtrl
{
// Construction
public:
	CLocalPropertySheet();

	CRect   m_Rect;             // Rectangle coordinates
	UINT    m_nCtrlID;          // CTrlID in the dialog
	int             m_nPages;   // number of pages
	int             m_nActPage; // Actual page
	// Array of dialogs
	CObArray	m_Dialogs;
	
// Attributes
public:

// Operations
public:
	int		SetActivePage (int nPage);
	int		SetActivePage (CLocalPropDialog* pPage);
	CLocalPropDialog	*GetPage (int nPage);
	CLocalPropDialog	*GetActivePage (void);
	int		GetPageCount (void);
	BOOL	DispPage (int	nCommand);		// handling pgup and pgdn
		

        //{{AFX_VIRTUAL(CLocalPropertySheet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CLocalPropertySheet();

protected:
        //{{AFX_MSG(CLocalPropertySheet)
        afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////

// CLocalPropDialog the dialog

class CLocalPropDialog : public CDialog
{
// Konstruction
public:
	CLocalPropDialog(CWnd* pParent = NULL);   // Standardconstruction
	
// Dialogfields
	//{{AFX_DATA(CLocalPropDialog)
	// enum { IDD = _UNKNOWN_RESOURCE_ID_ }; I create my  dialogs on the fly

	//}}AFX_DATA


	//{{AFX_VIRTUAL(CLocalPropDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
				AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	//{{AFX_MSG(CLocalPropDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_CPROPERTY_H__3F20C546_869F_11D2_B6F4_0000C0E36C6E__INCLUDED_)
