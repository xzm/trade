#if !defined(AFX_TEXTTOOLBAR_H__6673D747_929F_451B_BB82_EFD0F22B8E37__INCLUDED_)
#define AFX_TEXTTOOLBAR_H__6673D747_929F_451B_BB82_EFD0F22B8E37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextToolBar window

class CLASS_EXPORT CTextToolBar : public CGuiToolBarWnd
{
// Construction
public:
	CTextToolBar();
	DECLARE_DYNCREATE(CTextToolBar)
	
	enum TextLabel {
		labelNone	=	0x01,
		labelBottom	=	0x02,
		labelRight	=	0x03
	};

	BOOL	CreateTextToolBar( CWnd * pParentWnd, UINT nBarID, /* see AFX_IDW_TOOLBAR */
							UINT nIDToolBar, UINT nIDNormal = -1, UINT nIDToolHot = -1,
							int	nTextLabel = labelNone,
							DWORD dwCtrlStyle = TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_ALTDRAG,
							DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP |
							CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY | 
							CBRS_BORDER_3D | CBRS_GRIPPER );

// Attributes
public:

protected:
	CImageList	m_imageToolHot, m_imageToolNormal;
	int			m_nTextLabel;

// Operations
public:
	static	BOOL	IsValidStatus( int nTextLabel );
	
	BOOL	IsTextLabelVisible( );
	int		GetTextLabelStatus( );
	BOOL	SetTextLabelStatus( int nTextLabel );
	void	ToggleTextLabel( int nTextLabel );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTextToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextToolBar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTTOOLBAR_H__6673D747_929F_451B_BB82_EFD0F22B8E37__INCLUDED_)
