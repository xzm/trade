// ToolsDoc.cpp : implementation of the CToolsDoc class
//

#include "stdafx.h"
#include "Tools.h"

#include "ToolsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolsDoc

IMPLEMENT_DYNCREATE(CToolsDoc, CDocument)

BEGIN_MESSAGE_MAP(CToolsDoc, CDocument)
	//{{AFX_MSG_MAP(CToolsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolsDoc construction/destruction

CToolsDoc::CToolsDoc()
{
	// TODO: add one-time construction code here

}

CToolsDoc::~CToolsDoc()
{
}

BOOL CToolsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CToolsDoc serialization

void CToolsDoc::Serialize(CArchive& ar)
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
// CToolsDoc diagnostics

#ifdef _DEBUG
void CToolsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CToolsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToolsDoc commands
