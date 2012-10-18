// TestBenchDoc.cpp : implementation of the CTestBenchDoc class
//

#include "stdafx.h"
#include "TestBench.h"

#include "TestBenchDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestBenchDoc

IMPLEMENT_DYNCREATE(CTestBenchDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestBenchDoc, CDocument)
	//{{AFX_MSG_MAP(CTestBenchDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestBenchDoc construction/destruction

CTestBenchDoc::CTestBenchDoc()
{
	// TODO: add one-time construction code here

}

CTestBenchDoc::~CTestBenchDoc()
{
}

BOOL CTestBenchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestBenchDoc serialization

void CTestBenchDoc::Serialize(CArchive& ar)
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
// CTestBenchDoc diagnostics

#ifdef _DEBUG
void CTestBenchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestBenchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestBenchDoc commands
