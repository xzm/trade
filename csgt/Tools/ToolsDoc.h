// ToolsDoc.h : interface of the CToolsDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSDOC_H__D1B7F896_76A9_44B2_ACBF_9FFFA5319C88__INCLUDED_)
#define AFX_TOOLSDOC_H__D1B7F896_76A9_44B2_ACBF_9FFFA5319C88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CToolsDoc : public CDocument
{
protected: // create from serialization only
	CToolsDoc();
	DECLARE_DYNCREATE(CToolsDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolsDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CToolsDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLSDOC_H__D1B7F896_76A9_44B2_ACBF_9FFFA5319C88__INCLUDED_)
