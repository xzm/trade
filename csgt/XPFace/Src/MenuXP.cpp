///////////////////////////////////////////////////////////////////////////////
//
// MenuXP.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MenuXP.h"
#include "Tools.h"
#include "Draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IMGWIDTH 16
#define IMGHEIGHT 16
#define IMGPADDING 6
#define TEXTPADDING 8

///////////////////////////////////////////////////////////////////////////////
class CMenuXPItem
{
protected:
    MENUITEMINFO m_miInfo;
    CString      m_sCaption;
    CImgDesc     m_ImgDesc;
    HIMAGELIST   m_hImgList;
    int          m_nIndex;

public:
    CMenuXPItem ();
    CMenuXPItem (HMENU hMenu, UINT uItem, bool fByPosition = true);
   ~CMenuXPItem ();

// Properties
public:
    int   GetCaption   (CString& sCaption) const;
    int   GetShortCut  (CString& sShortCut) const;
    bool  GetSeparator () const;
    bool  GetChecked   () const;
    bool  GetDisabled  () const;
    bool  GetDefault   () const;
    HMENU GetPopup     () const;
    UINT  GetID        () const;

// Methods
public:
    int  GetCaptionWidth  (CDC* pDC) const;
    int  GetShortCutWidth (CDC* pDC) const;
    int  GetHeight        (CDC* pDC) const;
    void Draw             (CDC* pDC, LPCRECT pRect, bool bSelected) const;
};

CMap <int, int, CString, CString&> CMenuXP::m_sCaptions;
CMap <int, int, CImgDesc, CImgDesc&> CMenuXP::m_Images;

///////////////////////////////////////////////////////////////////////////////
bool CMenuXP::IsOwnerDrawn (HMENU hMenu)
{
    MENUITEMINFO mii = { sizeof MENUITEMINFO, MIIM_TYPE };

    ::GetMenuItemInfo (hMenu, 0, true, &mii);

    return (mii.fType & MFT_OWNERDRAW) != 0;
}

///////////////////////////////////////////////////////////////////////////////
void CMenuXP::OnMeasureItem (MEASUREITEMSTRUCT* pMeasureItemStruct)
{
    if ( pMeasureItemStruct->CtlType == ODT_MENU )
    {
        pMeasureItemStruct->itemHeight = LOBYTE(LOWORD(pMeasureItemStruct->itemData));
        pMeasureItemStruct->itemWidth = IMGWIDTH + IMGPADDING + HIWORD(pMeasureItemStruct->itemData) + TEXTPADDING + HIBYTE(LOWORD(pMeasureItemStruct->itemData)) + TEXTPADDING + 4;
    }
}

///////////////////////////////////////////////////////////////////////////////
void CMenuXP::OnDrawItem (DRAWITEMSTRUCT* pDrawItemStruct)
{
    ASSERT (pDrawItemStruct->CtlType == ODT_MENU);
    CBufferDC cDC (pDrawItemStruct->hDC, pDrawItemStruct->rcItem);
    CMenuXPItem item ((HMENU)pDrawItemStruct->hwndItem, pDrawItemStruct->itemID, false);

    cDC.SelectObject (CFont::FromHandle ((HFONT)GetStockObject (DEFAULT_GUI_FONT)));
    item.Draw (&cDC, &pDrawItemStruct->rcItem, (pDrawItemStruct->itemState & ODS_SELECTED)!=0);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT CMenuXP::OnMenuChar (HMENU hMenu, UINT nChar, UINT nFlags)
{
    if ( (nFlags & (MF_POPUP|MF_SYSMENU)) == MF_POPUP )
    {
        int nItemCount = ::GetMenuItemCount (hMenu);

        nChar = toupper (nChar);

        for ( int i = 0; i < nItemCount; i++ )
        {
            CMenuXPItem mnuItem (hMenu, i);
            CString sCaption;

            mnuItem.GetCaption (sCaption);
            sCaption.MakeUpper();

            for ( int nPos = sCaption.GetLength()-2; nPos >= 0; nPos-- )
            {
                if ( sCaption[nPos] == '&' && (UINT)toupper (sCaption[nPos+1]) == nChar &&
                     (nPos == 0 || sCaption[nPos-1] != '&') )
                {
                    return MAKELRESULT(i,2);
                }
            }
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void CMenuXP::SetWinXPStyle (HMENU hMenu, CWnd* pWnd)
{
    ASSERT(hMenu != NULL);
    int nItemCount = ::GetMenuItemCount (hMenu);
    MENUITEMINFO mii = { sizeof MENUITEMINFO, MIIM_ID|MIIM_TYPE };
    CClientDC cDC (AfxGetMainWnd());
    TEXTMETRIC tm;

    cDC.SelectObject ((HFONT)GetStockObject (DEFAULT_GUI_FONT));
    cDC.GetTextMetrics (&tm);

    int nHeight = max(tm.tmHeight+2,IMGHEIGHT)+4;
    int nSepHeight = tm.tmHeight/2+2;
    int nCaptionLength = 0;
    int nShortCutLength = 0;
    CPtrList* pListControlBars = NULL;

    if ( pWnd != NULL )
    {
        if ( pWnd->IsKindOf (RUNTIME_CLASS(CFrameWnd)) && !((CFrameWnd*)pWnd)->m_listControlBars.IsEmpty() )
        {
            pListControlBars = &((CFrameWnd*)pWnd)->m_listControlBars;
        }
        else
        {
            CFrameWnd* pFrame = pWnd->GetParentFrame();

            if ( pFrame != NULL )
            {
                pListControlBars = &pFrame->m_listControlBars;
            }
        }
    }
    for ( int i = 0; i < nItemCount; i++ )
    {
        TCHAR sCaption[256] = _T("");
        mii.dwTypeData = sCaption;
        mii.cch = 255;
        ::GetMenuItemInfo (hMenu, i, true, &mii);

        if ( (mii.fType & MFT_OWNERDRAW) == 0 )
        {
            mii.fType |= MFT_OWNERDRAW;
            ::SetMenuItemInfo (hMenu, i, true, &mii);

            if ( (mii.fType & MFT_SEPARATOR) == 0 )
            {
                CString sBuff(sCaption);
                CMenuXP::m_sCaptions.SetAt (mii.wID, sBuff);

                if ( pListControlBars != NULL )
                {
                    POSITION pos = pListControlBars->GetHeadPosition();

                    while ( pos != NULL )
                    {
                        CControlBar* pBar = (CControlBar*)pListControlBars->GetNext (pos);
                        ASSERT(pBar != NULL);

                        if ( pBar->IsKindOf (RUNTIME_CLASS(CToolBar)) )
                        {
                            TBBUTTONINFO tbbi = { sizeof(TBBUTTONINFO), TBIF_COMMAND|TBIF_IMAGE };

                            if ( pBar->SendMessage (TB_GETBUTTONINFO, mii.wID, (LPARAM)&tbbi) != -1 &&
                                 (UINT)tbbi.idCommand == mii.wID && tbbi.iImage != -1 )
                            {
                                CImgDesc imgDesc ((HIMAGELIST)pBar->SendMessage (TB_GETIMAGELIST, 0, 0), tbbi.iImage);

                                m_Images.SetAt (mii.wID, imgDesc);
                                break;
                            }
                        }
                    }
                }
            }
        }
        CMenuXPItem mnuItem (hMenu, i);
        int       nWidth = mnuItem.GetCaptionWidth (&cDC);

        if ( nWidth > nCaptionLength )
        {
            nCaptionLength = nWidth;
        }
        nWidth = mnuItem.GetShortCutWidth (&cDC);

        if ( nWidth > nShortCutLength )
        {
            nShortCutLength = nWidth;
        }
    }
    for ( int j = 0; j < nItemCount; j++ )
    {
        mii.fMask = MIIM_TYPE;
        ::GetMenuItemInfo (hMenu, j, true, &mii);

        if ( (mii.fType & MFT_SEPARATOR) == 0 )
        {
            mii.dwItemData = MAKELONG(MAKEWORD(nHeight, nShortCutLength), nCaptionLength);
        }
        else
        {
            mii.dwItemData = nSepHeight;
        }
        mii.fMask = MIIM_DATA;
        ::SetMenuItemInfo (hMenu, j, true, &mii);
    }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CMenuXPItem::CMenuXPItem ()
{
    memset (&m_miInfo, 0, sizeof(MENUITEMINFO));
}

///////////////////////////////////////////////////////////////////////////////
CMenuXPItem::CMenuXPItem (HMENU hMenu, UINT uItem, bool fByPosition)
{
    memset (&m_miInfo, 0, sizeof(MENUITEMINFO));
    m_miInfo.cbSize = sizeof(MENUITEMINFO);
    m_miInfo.fMask = MIIM_STATE|MIIM_SUBMENU|MIIM_TYPE|MIIM_DATA|MIIM_ID;
    VERIFY (::GetMenuItemInfo (hMenu, uItem, fByPosition, &m_miInfo));

    if ( !(m_miInfo.fType & MFT_SEPARATOR) )
    {
        CMenuXP::m_sCaptions.Lookup (m_miInfo.wID, m_sCaption);
        CMenuXP::m_Images.Lookup (m_miInfo.wID, m_ImgDesc);
    }
}

///////////////////////////////////////////////////////////////////////////////
CMenuXPItem::~CMenuXPItem ()
{
}

///////////////////////////////////////////////////////////////////////////////
int CMenuXPItem::GetCaption (CString& sCaption) const
{
    ASSERT(m_miInfo.fMask & MIIM_TYPE);
    sCaption = m_sCaption;

    int nShortCutPos = sCaption.Find ('\t');

    if ( nShortCutPos == -1 )
    {
        return sCaption.GetLength();
    }
    sCaption = sCaption.Left (nShortCutPos);

    return nShortCutPos-1;
}

///////////////////////////////////////////////////////////////////////////////
int CMenuXPItem::GetShortCut (CString& sShortCut) const
{
    ASSERT(m_miInfo.fMask & MIIM_TYPE);
    CString sCaption = m_sCaption;

    int nShortCutPos = sCaption.Find ('\t');

    if ( nShortCutPos == -1 )
    {
        sShortCut = "";
        return 0;
    }
    int nLength = sCaption.GetLength()-nShortCutPos-1;

    sShortCut = sCaption.Right (nLength);

    return nLength;
}

///////////////////////////////////////////////////////////////////////////////
bool CMenuXPItem::GetSeparator () const
{
    ASSERT(m_miInfo.fMask & MIIM_TYPE);
    return (m_miInfo.fType & MFT_SEPARATOR) == MFT_SEPARATOR;
}

///////////////////////////////////////////////////////////////////////////////
bool CMenuXPItem::GetChecked () const
{
    ASSERT(m_miInfo.fMask & MIIM_STATE);
    return (m_miInfo.fState & MFS_CHECKED) == MFS_CHECKED;
}

///////////////////////////////////////////////////////////////////////////////
bool CMenuXPItem::GetDisabled () const
{
    ASSERT(m_miInfo.fMask & MIIM_STATE);
    return (m_miInfo.fState & MFS_GRAYED) == MFS_GRAYED;
}

///////////////////////////////////////////////////////////////////////////////
bool CMenuXPItem::GetDefault () const
{
    ASSERT(m_miInfo.fMask & MIIM_STATE);
    return (m_miInfo.fState & MFS_DEFAULT) == MFS_DEFAULT;
}

///////////////////////////////////////////////////////////////////////////////
HMENU CMenuXPItem::GetPopup () const
{
    ASSERT(m_miInfo.fMask & MIIM_SUBMENU);
    return m_miInfo.hSubMenu;
}

///////////////////////////////////////////////////////////////////////////////
UINT CMenuXPItem::GetID () const
{
    ASSERT(m_miInfo.fMask & MIIM_ID);
    return m_miInfo.wID;
}

///////////////////////////////////////////////////////////////////////////////
int CMenuXPItem::GetCaptionWidth (CDC* pDC) const
{
    if ( GetSeparator() )
    {
        return 0;
    }
    CString sCaption;
    int nLength = 0;

    if ( GetCaption (sCaption) > 0 )
    {
        int nPos = sCaption.Find ('&');
        CBoldDC bold (*pDC, GetDefault());

        if ( nPos >= 0 )
        {
            sCaption = sCaption.Left (nPos) + sCaption.Right (sCaption.GetLength()-nPos-1);
        }
        nLength = pDC->GetTextExtent (sCaption).cx;
    }
    return nLength;
}

///////////////////////////////////////////////////////////////////////////////
int CMenuXPItem::GetShortCutWidth (CDC* pDC) const
{
    if ( GetSeparator() )
    {
        return 0;
    }
    CString sShortCut;
    int nLength = 0;

    if ( GetShortCut (sShortCut) > 0 )
    {
        CBoldDC bold (*pDC, GetDefault());

        nLength = pDC->GetTextExtent (sShortCut).cx;
    }
    return nLength;
}

///////////////////////////////////////////////////////////////////////////////
int CMenuXPItem::GetHeight (CDC* pDC) const
{
    TEXTMETRIC tm;

    pDC->GetTextMetrics (&tm);
                     
    return GetSeparator() ? tm.tmHeight/2+2 : tm.tmHeight+4;
}

// From <winuser.h>
#define OBM_CHECK 32760

///////////////////////////////////////////////////////////////////////////////
void CMenuXPItem::Draw (CDC* pDC, LPCRECT pRect, bool bSelected) const
{
    COLORREF crBackImg = CLR_NONE;

    if ( bSelected )
    {
        COLORREF crHighLight = ::GetSysColor (COLOR_HIGHLIGHT);
        CPenDC pen (*pDC, crHighLight);
        CBrushDC brush (*pDC, crBackImg = GetDisabled() ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +73, 0) : HLS_TRANSFORM (crHighLight, +70, -57));

        pDC->Rectangle (pRect);
    }
    else
    {
        CRect rc (pRect);

        rc.right = IMGWIDTH+IMGPADDING;
        pDC->FillSolidRect (rc, crBackImg = HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +20, 0));
        rc.left = rc.right;
        rc.right = pRect->right;
        pDC->FillSolidRect (rc, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +75, 0));
    }
    if ( GetSeparator() )
    {
        CPenDC pen (*pDC, HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -18, 0));

        pDC->MoveTo (pRect->left+IMGWIDTH+IMGPADDING+TEXTPADDING,  (pRect->top+pRect->bottom)/2);
        pDC->LineTo (pRect->right-1, (pRect->top+pRect->bottom)/2);
    }
    else
    {
        CRect rc (pRect);
        CString sCaption;

        if ( GetCaption (sCaption) > 0 )
        {
            pDC->SetTextColor (GetDisabled() ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -18, 0) : ::GetSysColor (COLOR_MENUTEXT));
            pDC->SetBkMode (TRANSPARENT);

            CBoldDC bold (*pDC, GetDefault());

            rc.left = IMGWIDTH+IMGPADDING+TEXTPADDING;
            pDC->DrawText (sCaption, rc, DT_SINGLELINE|DT_VCENTER|DT_LEFT);

            CString sShortCut;

            if ( GetShortCut (sShortCut) > 0 )
            {
                rc.right -= TEXTPADDING+4;
                pDC->DrawText (sShortCut, rc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
            }
            if ( GetChecked() )
            {
                COLORREF crHighLight = ::GetSysColor (COLOR_HIGHLIGHT);
                CPenDC pen (*pDC, crHighLight);
                CBrushDC brush (*pDC, crBackImg = GetDisabled() ? HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), +73, 0) :
                                                                  (bSelected ? HLS_TRANSFORM (crHighLight, +50, -50) : HLS_TRANSFORM (crHighLight, +70, -57)));

                pDC->Rectangle (CRect (pRect->left+1, pRect->top+1, pRect->left+20, pRect->bottom-1));
            }
            if ( m_ImgDesc.m_hImgList != NULL && m_ImgDesc.m_nIndex != -1 )
            {
                bool bOver = !GetDisabled() && bSelected;

                if ( GetDisabled() || (bSelected && !GetChecked()) )
                {
                    HICON hIcon = ImageList_ExtractIcon (NULL, m_ImgDesc.m_hImgList, m_ImgDesc.m_nIndex);
                    pDC->DrawState (CPoint (pRect->left + ( bOver ? 4 : 3 ), rc.top + ( bOver ? 4 : 3 )),
                                    CSize (IMGWIDTH, IMGHEIGHT), hIcon, DSS_MONO,
                                    CBrush (bOver ? HLS_TRANSFORM (::GetSysColor (COLOR_HIGHLIGHT), +50, -66) : HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -27, 0)));
                    DestroyIcon (hIcon);
                }
                if ( !GetDisabled() )
                {
                    ::ImageList_Draw (m_ImgDesc.m_hImgList, m_ImgDesc.m_nIndex, pDC->m_hDC,
                                      pRect->left+( (bSelected && !GetChecked()) ? 2 : 3 ), rc.top+( (bSelected && !GetChecked()) ? 2 : 3 ), ILD_TRANSPARENT);
                }
            }
            else if ( GetChecked() )
            {
                // Draw the check mark
                rc.left  = pRect->left+5;
                rc.right = rc.left + IMGWIDTH+IMGPADDING;
                pDC->SetBkColor (crBackImg);
                HBITMAP hBmp = LoadBitmap (NULL, MAKEINTRESOURCE(OBM_CHECK));
                pDC->DrawState (CPoint (rc.left,rc.top+3), CSize(rc.Size()), hBmp, DSS_NORMAL, (HBRUSH)NULL);
                DeleteObject (hBmp);
            }
        }
    }
}
