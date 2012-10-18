//-----------------------------------------------------------------------//
// This is a part of the GuiLib MFC Extention.							 //	
// Autor  :  Francisco Campos											 //
// (C) 2002 Francisco Campos <www.beyondata.com> All rights reserved     //
// This code is provided "as is", with absolutely no warranty expressed  //
// or implied. Any use is at your own risk.								 //		
// You must obtain the author's consent before you can include this code //
// in a software library.												 //
// If the source code in  this file is used in any application			 //
// then acknowledgement must be made to the author of this program		 //	
// fcampos@tutopia.com													 //
//-----------------------------------------------------------------------//


#if !defined(AFX_GUITABWND_H__4C966B28_CE7A_407D_9A7A_9BE7A80B395D__INCLUDED_)
#define AFX_GUITABWND_H__4C966B28_CE7A_407D_9A7A_9BE7A80B395D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabWnd window

#include "GuiLib.h"
#include "GuiDrawlayer.h"

// Modified By SunZhenyu, 2003/11/11, add the next 1 line
#define	WM_GUITABWND_COLORCHANGE	WM_USER+1

//*********************************************************
class GUILIBDLLEXPORT  CGuiTabWnd : public CWnd
{
// Construction

public:

	//************************************************
	CGuiTabWnd();
	virtual ~CGuiTabWnd();
	//************************************************

	enum Style{S3D=0,FLAT=1,SHADOW=2};
	enum AlingTab{ALN_TOP=0,ALN_BOTTOM=1};
	enum TypeTab{TYPE_NORMAL=0,TYPE_MDI=1};

public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

public:
	//***********************************************************************	
	void	SetStyle(Style estyle);
	void    SetTypeTab(TypeTab Type=TYPE_NORMAL);
	void	virtual Drawtabs(CDC* dc);
	void	Addtab(CWnd* pParent,CString lpMsg,UINT uIcon);
	void	SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask);
	void	RecalLayout();
	virtual void AjustTabs();
	void    SetCurtab(int m_numtab);
	int     GetCurtab();
	void    DeleteTab(int m_numTab);
	void	ShowTitle(CString m_Caption);
	CWnd*   GetActiveWnd () const;
	CWnd*   GetNumWnd(int m_numtab)  const;
	int     GetCount(){return m_Numtabs;}
	void	SetActualTitle(BOOL bActual);
	void    SetALingTabs(AlingTab alnTab);
	BOOL	FindItem(HWND hWnd);
	void    UpdateCaption(CWnd* pWnd,CString m_Title);
	//***********************************************************************

	// Modified By SunZhenyu, 2003/10/28, add the next 2 function
	CRect	GetTabRect( int m_numtab );
	void  SetSelectTabColor( COLORREF clrBK, COLORREF clrText );
	
protected:
	TypeTab			m_TypeTab;
	BOOL			m_ActualTitle;
	CPtrArray		m_pArray;
	CSize			m_sizeImag;
	int				m_Numtabs;
	int				m_iMinValRec;
	int				m_ActualVar;
	int				m_iSelectTab;
	CRect			m_rectCliente;
	CRect			m_rectTab;
	CToolTipCtrl	CToolTip;
	CImageList		Image;
	CFont			m_cfont;
	BOOL			m_style;
	GuiDrawLayer	m_wd;
	CWnd*			pParent;
	AlingTab		m_alnTab;

	// Modified By SunZhenyu, 2003/11/09, add the next 2 member
	COLORREF		m_clrSelectTabBK;
	COLORREF		m_clrSelectTabText;
protected:

	//{{AFX_MSG(CGuiTabWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange( );
	//}}AFX_MSG
	// Modified By SunZhenyu, 2003/11/11, add the next 1 function
	afx_msg LRESULT OnColorChange( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUITABWND_H__4C966B28_CE7A_407D_9A7A_9BE7A80B395D__INCLUDED_)
