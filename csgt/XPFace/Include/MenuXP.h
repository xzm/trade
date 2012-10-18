///////////////////////////////////////////////////////////////////////////////
//
// MenuXP.h : header file
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <afxtempl.h>

///////////////////////////////////////////////////////////////////////////////
class CImgDesc
{
public:
    HIMAGELIST m_hImgList;
    int        m_nIndex;

    CImgDesc (HIMAGELIST hImgList = NULL, int nIndex = 0) : m_hImgList (hImgList), m_nIndex (nIndex)
    {
    }
};

///////////////////////////////////////////////////////////////////////////////
class CLASS_EXPORT CMenuXP : public CMenu
{
// Attributes
public:
    static CMap <int, int, CString, CString&> m_sCaptions;
    static CMap <int, int, CImgDesc, CImgDesc&> m_Images;

// Operations
public:
    static void SetWinXPStyle (HMENU hMenu, CWnd* pWnd = NULL);
    static bool IsOwnerDrawn (HMENU hMenu);
    static void OnMeasureItem (MEASUREITEMSTRUCT* pMeasureItemStruct);
    static void OnDrawItem (DRAWITEMSTRUCT* pDrawItemStruct);
    static LRESULT OnMenuChar (HMENU hMenu, UINT nChar, UINT nFlags);
};

#define DECLARE_MENUXP()                                                             \
    protected:                                                                       \
    afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);     \
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct); \
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);          \
    afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);

#define ON_MENUXP_MESSAGES() \
    ON_WM_INITMENUPOPUP()    \
    ON_WM_MEASUREITEM()      \
    ON_WM_DRAWITEM()         \
    ON_WM_MENUCHAR()

#define IMPLEMENT_MENUXP(theClass, baseClass, bFlag)                               \
    void theClass::OnInitMenuPopup (CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) \
    {                                                                              \
        baseClass::OnInitMenuPopup (pPopupMenu, nIndex, bSysMenu);                 \
                                                                                   \
        if ( bFlag && !bSysMenu )                                                  \
        {                                                                          \
            CMenuXP::SetWinXPStyle (pPopupMenu->m_hMenu, this);                    \
        }                                                                          \
    }                                                                              \
    void theClass::OnMeasureItem (int, LPMEASUREITEMSTRUCT lpMeasureItemStruct)    \
    {                                                                              \
        CMenuXP::OnMeasureItem (lpMeasureItemStruct);                              \
    }                                                                              \
    void theClass::OnDrawItem (int, LPDRAWITEMSTRUCT lpDrawItemStruct)             \
    {                                                                              \
        CMenuXP::OnDrawItem (lpDrawItemStruct);                                    \
    }                                                                              \
    LRESULT theClass::OnMenuChar (UINT nChar, UINT nFlags, CMenu* pMenu)           \
    {                                                                              \
        if ( CMenuXP::IsOwnerDrawn (pMenu->m_hMenu) )                              \
        {                                                                          \
            return CMenuXP::OnMenuChar (pMenu->m_hMenu, nChar, nFlags);            \
        }                                                                          \
        return baseClass::OnMenuChar (nChar, nFlags, pMenu);                       \
    }

///////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
