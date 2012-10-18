#if !defined(AFX_SIMUSTATDLG_H__1BE15A47_2073_446B_AF6B_1DFD3835CF09__INCLUDED_)
#define AFX_SIMUSTATDLG_H__1BE15A47_2073_446B_AF6B_1DFD3835CF09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimuStatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSimuStatDlg dialog
/***
	策略统计对话框
*/
class CSimuStatDlg : public CXFDialog
{
// Construction
public:
	CSimuStatDlg(CWnd* pParent = NULL);   // standard constructor

	// operation
	void	SetStrategy( CStrategy * pStrategy );

// Dialog Data
	//{{AFX_DATA(CSimuStatDlg)
	enum { IDD = IDD_SIMUSTAT };
	CGuiButton	m_btnOK;
	CGuiEdit	m_editXiapu;
	CGuiEdit	m_editYieldStdDev;
	CGuiEdit	m_editYieldIndex;
	CGuiEdit	m_editYieldFin;
	CGuiEdit	m_editYieldMin;
	CGuiEdit	m_editYieldMax;
	CGuiEdit	m_editVictoryTimes;
	CGuiEdit	m_editBuyTimes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimuStatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStrategy	*	m_pStrategy;

	BOOL		SetResults( );

	// Generated message map functions
	//{{AFX_MSG(CSimuStatDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUSTATDLG_H__1BE15A47_2073_446B_AF6B_1DFD3835CF09__INCLUDED_)
