///////////////////////////////////////////////////////////////////////////////
//
// Tools.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// Check if the specified window is child of a docked toolbar
bool ChildOfDockedToolbar (CWnd* pWnd)
{
    CWnd* pParent = pWnd->GetParent();
    
    return pParent->IsKindOf (RUNTIME_CLASS(CControlBar)) &&
           ((CControlBar*)pParent)->m_pDockBar != NULL;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CMouseMgr::CMouseMgr () : m_bOver (false)
{
}

///////////////////////////////////////////////////////////////////////////////
void CMouseMgr::Init (HWND hWnd, WORD wFlags)
{
    m_hWnd = hWnd;
    m_wFlags = wFlags;
}

///////////////////////////////////////////////////////////////////////////////
bool CMouseMgr::MouseOver () const
{
    return m_bOver;
}

extern "C" WINUSERAPI BOOL WINAPI TrackMouseEvent (LPTRACKMOUSEEVENT lpEventTrack);
///////////////////////////////////////////////////////////////////////////////
bool CMouseMgr::OnMouseMove (HWND hTrack)
{
    ASSERT(m_hWnd != NULL);
    if ( hTrack == NULL ) hTrack = m_hWnd;

    if ( !m_bOver )
    {
        m_bOver = true;

        if ( m_wFlags & MMS_PAINT )
        {
            ::InvalidateRect (m_hWnd, NULL, false);
        }
        if ( m_wFlags & MMS_NCPAINT )
        {
            ::SetWindowPos (m_hWnd, NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
        }
        // Prepare for tracking MouseOut
        TRACKMOUSEEVENT tme = { sizeof TRACKMOUSEEVENT, TME_LEAVE, m_hTrack = hTrack, 0 };
        ::TrackMouseEvent (&tme);

        return true;
    }
    if ( hTrack != m_hTrack )
    {
        // Cancel tracking MouseOut for the previous window (main or child)
        TRACKMOUSEEVENT tme = { sizeof TRACKMOUSEEVENT, TME_CANCEL, m_hTrack, 0 };
        ::TrackMouseEvent (&tme);

        // Track MouseOut
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hTrack = hTrack;
        ::TrackMouseEvent (&tme);
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
bool CMouseMgr::OnMouseOut (HWND hTrack)
{
    ASSERT(m_hWnd != NULL);
    if ( hTrack == NULL ) hTrack = m_hWnd;

    if ( hTrack != m_hTrack )
    {
        return false;
    }
    ASSERT(m_bOver);
    m_bOver = false;

    if ( m_wFlags & MMS_PAINT )
    {
        ::InvalidateRect (m_hWnd, NULL, false);
    }
    if ( m_wFlags & MMS_NCPAINT )
    {
        ::SetWindowPos (m_hWnd, NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
    }
    return true;
}
