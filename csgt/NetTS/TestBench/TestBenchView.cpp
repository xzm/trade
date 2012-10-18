// TestBenchView.cpp : implementation of the CTestBenchView class
//

#include "stdafx.h"
#include "TestBench.h"

#include "TestBenchDoc.h"
#include "TestBenchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestBenchView

IMPLEMENT_DYNCREATE(CTestBenchView, CView)

BEGIN_MESSAGE_MAP(CTestBenchView, CView)
	//{{AFX_MSG_MAP(CTestBenchView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestBenchView construction/destruction

CTestBenchView::CTestBenchView()
{
	// TODO: add construction code here

}

CTestBenchView::~CTestBenchView()
{
}

BOOL CTestBenchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestBenchView drawing

void CTestBenchView::OnDraw(CDC* pDC)
{
	CTestBenchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTestBenchView printing

BOOL CTestBenchView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestBenchView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestBenchView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestBenchView diagnostics

#ifdef _DEBUG
void CTestBenchView::AssertValid() const
{
	CView::AssertValid();
}

void CTestBenchView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestBenchDoc* CTestBenchView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestBenchDoc)));
	return (CTestBenchDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestBenchView message handlers
