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

#pragma once

#include "afxext.h"
#include "afxPriv.h"
#include "GuiLib.h"
//#include "GuiToolButton.h"

#define NORMAL 0
#define OVER   1
#define PRESS  2


class GUILIBDLLEXPORT CGuiControlBarButton 
{
	
public:
	enum GuiType
	{
		GUINORMAL=0,
		GUITOOLBUTTON
	};
	CRect rcArea;
protected:

	int m_nIcon;
	LPCTSTR m_lpMsg;
	CImageList m_imgList;
	GuiType bTypeButton;
	BOOL m_bEnabled;
	CToolTipCtrl   m_toolTip;
public:

	CGuiControlBarButton();
	void SetData(int nIcon,LPCTSTR lpMsg);
	~CGuiControlBarButton();
	void Paint(CDC* pDC,int st,CRect rc,COLORREF clrFondo);
	void SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask);
	void SetTypeButton(GuiType bTypeb=GUINORMAL);
	void SetEnabled(BOOL bEnabled);
	
};


class GUILIBDLLEXPORT   CGuiControlBar : public CControlBar
{
	DECLARE_DYNAMIC(CGuiControlBar)
protected:
	int			nGapGripper;
	UINT		nDockBarAling;
	CSize		m_sizeMinFloating;
	CSize		m_sizeHorz;
	CSize		m_sizeVert;
	CSize		m_sizeHorzt;
	CSize		m_sizeVertt;
	CSize		m_sizeMinV;
	CSize		m_sizeMinH;
	int			m_pos;
	int			m_Last;
	int			m_First;
	CRect		m_rcBorder;
	CRect		m_rcOldBorder;
	BOOL		m_bTracking;
	UINT		m_SideMove;  //Que lado se arrastra
	CPoint		m_ptOld; //la ultima posicion del mouse
	BOOL		m_Initialize; // es cierto si no se ha inicializado esta ventana
	CPoint		m_ptPos;
	CPoint		m_ptStartPos;
	CPoint		m_ptActualPos;
	UINT		m_Orient;
	BOOL		m_bForcepaint;
	CGuiControlBarButton m_CloseBtn; //arreglo de botones para control de la barra
	CRect		m_rcCloseBtn;
	int			m_stateBtn;
	COLORREF	m_clrFondo;			//para el color de fondo de la barra
	CImageList m_img;
public:

	virtual void DrawGripper(CDC* pDC,CRect* rc);
	BOOL IsLegal(UINT uAlin);
	CGuiControlBar* GetGuiControlBar(int nPos,BOOL bAll=FALSE) const;
	void AjustReDinSize(CPoint cp);
	void OnActiveWindow();
	void SetEqualWidth();
	int GetHiWid();
	int GetWidthMax();
	int  GetFirstPos();
	int  GetLastPos();
    BOOL IsLeft();
	BOOL IsRight();
	BOOL IsTop();
	BOOL IsBottom();
	BOOL IsVert();
	BOOL IsHorz();
	BOOL IsFloating();
	void OnInvertTracker(const CRect& rect);
	void RecalWindowPos();
	void AjustVert(BOOL bGrow,int nDif);
	void AjustHorz(BOOL bGrow,int nDif);
	CRect GetDockRect();
	void ActiveCaption();
	void SetColorFondo(COLORREF clrFondo);
	//esta funcion permite soportar multiples vistas
	void SetSupporMultiView(BOOL bMultiView=TRUE){	m_bSupportMultiView=bMultiView;}
	BOOL m_bSupportMultiView;
protected:
	BOOL m_bActive; //esta ventana tiene el foco
	BOOL m_bOldActive;
public:
	CGuiControlBar();
	virtual ~CGuiControlBar();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
	virtual BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,CWnd* pParentWnd, UINT nID);
	void OnShowTitle();
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove( UINT, CPoint );
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSysColorChange( );
};


