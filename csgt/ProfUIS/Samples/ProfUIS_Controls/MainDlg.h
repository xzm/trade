// MainDlg.h : header file
//

#if !defined(AFX_MAINDLG_H__6F25FADE_3D19_4371_A550_B74561E7B764__INCLUDED_)
#define AFX_MAINDLG_H__6F25FADE_3D19_4371_A550_B74561E7B764__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ExtButton.h>
#include <ExtBtnOnFlat.h>
#include <ExtEdit.h>
#include <ExtComboBox.h>
#include <ExtResizableDialog.h>

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class CMainDlg : public CExtResizableDialog
{
	bool m_bDialogInitComplete;
	bool m_bShowIconInTray,m_bDialogWithCtxMenu;
	void ShowCtxMenu(CPoint ptScreen);
	CMenu m_menu;
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_PROFUIS_CONTROLS_DIALOG };
	CExtBtnOnFlat	m_CheckMenuWithShadows;
	CExtComboBox	m_ComboMenuEffect;
	CExtBtnOnFlat	m_CheckAnimateMenuWhenExpanding;
	CExtColorButton	m_btnColor2;
	CExtColorButton	m_btnColor1;
	CExtButton	m_btn2;
	CExtEdit	m_EditSample;
	CExtComboBox	m_ComboEditable;
	CExtComboBox	m_ComboUiLook;
	CExtBtnOnFlat	m_CheckEnableContextMenu;
	CExtBtnOnFlat	m_CheckShowIconInTray;
	CExtButton	m_BtnCancel;
	CExtButton	m_BtnOK;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAppAbout();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSelendokComboUiLook();
	afx_msg void OnCheckShowInTray();
	afx_msg void OnEnableDialogContextMenu();
	afx_msg void OnLook2k();
	afx_msg void OnUpdateLook2k(CCmdUI* pCmdUI);
	afx_msg void OnLookXp();
	afx_msg void OnUpdateLookXp(CCmdUI* pCmdUI);
	afx_msg void OnCheckMenuAnimExpanding();
	afx_msg void OnSelendokComboMenuOpenEffect();
	afx_msg void OnCheckMenuWithShadows();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnUpdateCheckShowInTray(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEnableDialogContextMenu(CCmdUI* pCmdUI);
	afx_msg LRESULT OnInconNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__6F25FADE_3D19_4371_A550_B74561E7B764__INCLUDED_)
