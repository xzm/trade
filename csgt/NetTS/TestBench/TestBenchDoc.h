// TestBenchDoc.h : interface of the CTestBenchDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTBENCHDOC_H__474E79CB_E431_4BD6_8082_21D8F0EDD78F__INCLUDED_)
#define AFX_TESTBENCHDOC_H__474E79CB_E431_4BD6_8082_21D8F0EDD78F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestBenchDoc : public CDocument
{
protected: // create from serialization only
	CTestBenchDoc();
	DECLARE_DYNCREATE(CTestBenchDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestBenchDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestBenchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestBenchDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTBENCHDOC_H__474E79CB_E431_4BD6_8082_21D8F0EDD78F__INCLUDED_)
