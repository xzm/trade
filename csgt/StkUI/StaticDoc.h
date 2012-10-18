// StaticDoc.h : interface of the CStaticDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICDOC_H__4FCC43EC_4835_41E3_BC2E_9DA211B94144__INCLUDED_)
#define AFX_STATICDOC_H__4FCC43EC_4835_41E3_BC2E_9DA211B94144__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	UPDATE_HINT_WIZARDVIEW			1
#define	UPDATE_HINT_SIMUVIEW			2
#define	UPDATE_HINT_SIMUVIEW_REREALRUN	3
#define	UPDATE_HINT_SLISTVIEW			4
#define	UPDATE_HINT_REALTIMEVIEW		5
#define	UPDATE_HINT_MULTISORTVIEW		6
#define	UPDATE_HINT_GRAPHVIEW			7
#define	UPDATE_HINT_BASEVIEW			8
#define	UPDATE_HINT_INFOVIEW			9
#define	UPDATE_HINT_SELECTORVIEW		10

#define	UPDATE_HINT_GRAPHVIEW_NOTRELOADDATA	10	// extra for graphview

#define	WM_USER_GETVIEWTITLE		(WM_USER+1)
#define	WM_USER_GETVIEWCMDID		(WM_USER+2)
#define	WM_USER_CANCLOSEVIEW		(WM_USER+3)
#define	WM_USER_COLORCHANGE			(WM_USER+4)

class	CWizardView;
class	CChildFrame;
/***
	Document类，程序启动时，创建一个对象，主管几个视图的显示
*/
class CStaticDoc : public CDocument
{
protected: // create from serialization only
	CStaticDoc();
	DECLARE_DYNCREATE(CStaticDoc)

	static CStaticDoc* CreateNewDocument( );
	static CStaticDoc* OpenDocumentFile( LPCTSTR lpszPathName );

// Attributes
public:
	virtual CView * GetViewIfExist( CRuntimeClass *pViewClass );
	virtual CView*	GetActiveView( );
	BOOL	ShowStaticView( CRuntimeClass * pViewClass, BOOL bMaximized = FALSE );

// Operations
public:
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);

	virtual UINT GetIDResource( );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual void OnCloseDocument();
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStaticDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CStaticDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICDOC_H__4FCC43EC_4835_41E3_BC2E_9DA211B94144__INCLUDED_)
