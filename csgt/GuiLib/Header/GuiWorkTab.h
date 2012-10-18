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


#if !defined(AFX_GUIWORKTAB_H__A711D725_2268_4750_B58E_5218AD3BC0A5__INCLUDED_)
#define AFX_GUIWORKTAB_H__A711D725_2268_4750_B58E_5218AD3BC0A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiTabWnd.h"
#include "GuiLib.h"
#include "GuiDrawlayer.h"
#include "GuiMiniTool.h"

//****************************************************************

class GUILIBDLLEXPORT  CGuiWorkTab : public CGuiTabWnd  
{
public:
	
	//***************************************
		CGuiWorkTab();
		virtual ~CGuiWorkTab();
	//***************************************

public:

	void  virtual Drawtabs(CDC* dc);
	void  OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
	void  PintaSeparador(CDC* dc);
	void  OnSpiNiz();
	void  OnSpiNde();	

	// Modified By SunZhenyu, 2003/10/28, add the next 1 function
	void  ResetDeplaza( );

protected:
	
	CGuiMiniTool	m_mSpin;
	CScrollBar		m_spin;
	CScrollBar		m_scroll;
	CFont			m_cfont;
	BOOL			m_PresRight;
	BOOL			m_PresLeft;
	int				m_deplaza;
	CRect			m_btndesplaza;
	BOOL			bIniciaArrastre;
	GuiDrawLayer	m_wd;
	BOOL			m_EnableL;
	BOOL			m_EnableR;

protected:

	//{{AFX_MSG(CGuiWorkTab)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSysColorChange(); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GUIWORKTAB_H__A711D725_2268_4750_B58E_5218AD3BC0A5__INCLUDED_)
