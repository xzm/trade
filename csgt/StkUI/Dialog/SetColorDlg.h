#if !defined(AFX_SETCOLORDLG_H__AAA444B5_A158_49D8_8548_A8C3862CA6CE__INCLUDED_)
#define AFX_SETCOLORDLG_H__AAA444B5_A158_49D8_8548_A8C3862CA6CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetColorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetColorDlg dialog
/***
	设置颜色对话框
*/
class CSetColorDlg : public CXFDialog
{
// Construction
public:
	CSetColorDlg(CWnd* pParent = NULL);   // standard constructor

	void	LoadFromColorClass( CColorClass & cls );
	void	StoreToColorClass( CColorClass & cls );

// Dialog Data
	//{{AFX_DATA(CSetColorDlg)
	enum { IDD = IDD_SETCOLOR };
	CGuiButton	m_btnCancel;
	CGuiButton	m_btnOK;
	CGuiButton	m_btnPreview;
	CGuiButton	m_btnSaveAs;
	CExtColorButton	m_btnLine6;
	CExtColorButton	m_btnLine5;
	CExtColorButton	m_btnLine4;
	CExtColorButton	m_btnLine3;
	CExtColorButton	m_btnLine2;
	CExtColorButton	m_btnLine1;
	CExtColorButton	m_btnText;
	CExtColorButton	m_btnTitle;
	CExtColorButton	m_btnDJ;
	CExtColorButton	m_btnCW;
	CExtColorButton	m_btnNewKLine;
	CExtColorButton	m_btnPlane;
	CExtColorButton	m_btnFallEntity;
	CExtColorButton	m_btnFall;
	CExtColorButton	m_btnRise;
	CExtColorButton	m_btnBorder;
	CExtColorButton	m_btnGraphBK;
	CExtColorButton	m_btnBaseText;
	CExtColorButton	m_btnBaseBK;
	CExtColorButton	m_btnSListSelected;
	CExtColorButton	m_btnSListBK;
	CExtColorButton	m_btnSimuText;
	CExtColorButton	m_btnSimuBK;
	CExtColorButton	m_btnWorkspText;
	CExtColorButton	m_btnWorkspBK;
	CGuiButton	m_btnDelete;
	CGuiComboBoxExt	m_comboColorClass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString		m_strNameCurrent;
	CColorClass	m_clsOriginal;

	// Generated message map functions
	//{{AFX_MSG(CSetColorDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPreview();
	afx_msg void OnSaveas();
	afx_msg void OnDelete();
	afx_msg void OnSelchangeColorclass();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCOLORDLG_H__AAA444B5_A158_49D8_8548_A8C3862CA6CE__INCLUDED_)
