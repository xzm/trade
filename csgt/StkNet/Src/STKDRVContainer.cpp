// STKDRVContainer.cpp: implementation of the CSTKDRVContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StkNet.h"
#include "STKDRVContainer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSTKDRVContainer::CSTKDRVContainer()
{

}

CSTKDRVContainer::~CSTKDRVContainer()
{
	UnloadDriver( );
}

int CSTKDRVContainer::LoadDriver( CStringArray & astrDriverPath )
{
	int	nCount	=	0;
	m_aDriver.SetSize( astrDriverPath.GetSize() );
	for( int i=0; i<astrDriverPath.GetSize(); i++ )
	{
		if( m_aDriver.ElementAt(i).LoadDriver( astrDriverPath[i] ) )
			nCount	++;
	}
	return nCount;
}

void CSTKDRVContainer::UnloadDriver( )
{
	for( int i=0; i<m_aDriver.GetSize(); i++ )
		m_aDriver.ElementAt(i).UnloadDriver();
	m_aDriver.RemoveAll();
}

int CSTKDRVContainer::Stock_Init(HWND hWnd,UINT uMsg,int nWorkMode)
{
	int	nRet	=	-1;
	for( int i=0; i<m_aDriver.GetSize(); i++ )
	{
		if( m_aDriver.ElementAt(i).Stock_Init(hWnd,uMsg,nWorkMode) > 0 )
			nRet	=	1;
	}
	return nRet;
}

int CSTKDRVContainer::Stock_Quit(HWND hWnd)
{
	int	nRet	=	1;
	for( int i=0; i<m_aDriver.GetSize(); i++ )
	{
		if( m_aDriver.ElementAt(i).Stock_Quit(hWnd) < 0 )
			nRet	=	-1;
	}
	return nRet;
}

int CSTKDRVContainer::GetTotalNumber()
{
	int	nRet	=	0;
	for( int i=0; i<m_aDriver.GetSize(); i++ )
	{
		nRet	=	m_aDriver.ElementAt(i).GetTotalNumber();
		if( nRet > 0 )
			return nRet;
	}
	return nRet;
}

int CSTKDRVContainer::GetStockByNoEx(int nNo,RCV_REPORT_STRUCTEx * pBuf)
{
	int	nRet	=	-1;
	for( int i=0; i<m_aDriver.GetSize(); i++ )
	{
		nRet	=	m_aDriver.ElementAt(i).GetStockByNoEx(nNo, pBuf);
		if( nRet > 0 )
			return nRet;
	}
	return nRet;
}

int CSTKDRVContainer::GetStockByCodeEx(char * pszStockCode,int nMarket,RCV_REPORT_STRUCTEx * pBuf)
{
	int	nRet	=	-1;
	for( int i=0; i<m_aDriver.GetSize(); i++ )
	{
		nRet	=	m_aDriver.ElementAt(i).GetStockByCodeEx(pszStockCode, nMarket, pBuf);
		if( nRet > 0 )
			return nRet;
	}
	return nRet;
}

int CSTKDRVContainer::SetupReceiver(BOOL bShowWindow)
{
	int	nRet	=	-1;
	for( int i=0; i<m_aDriver.GetSize(); i++ )
	{
		nRet	=	m_aDriver.ElementAt(i).SetupReceiver(bShowWindow);
		if( nRet > 0 )
			return nRet;
	}
	return nRet;
}
