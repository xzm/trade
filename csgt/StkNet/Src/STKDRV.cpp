// STKDRV.cpp: implementation of the CSTKDRV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Stockdrv.h"
#include "STKDRV.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSTKDRV::CSTKDRV()
{
	m_pfnStock_Init = NULL;
	m_pfnStock_Quit = NULL;
	m_pfnGetTotalNumber = NULL;
	m_pfnGetStockByNoEx = NULL;
	m_pfnGetStockByCodeEx = NULL;
	m_pfnSetupReceiver = NULL;
	m_pfnGetStockDrvInfo = NULL;
	m_hSTKDrv = NULL;

	// GetAdress();
}

CSTKDRV::~CSTKDRV()
{
}

BOOL CSTKDRV::LoadDriver( LPCTSTR lpszDrvPath )
{
	ASSERT( NULL != lpszDrvPath );
	CString	strLibrary	=	lpszDrvPath;
	if( strLibrary.GetLength() <= 0 )
		return FALSE;

	m_hSTKDrv = LoadLibrary( strLibrary );
	if( !m_hSTKDrv ) 
		return FALSE;

	m_pfnStock_Init = \
		    (int (WINAPI *)(HWND,UINT,int)) GetProcAddress(m_hSTKDrv,"Stock_Init");
	m_pfnStock_Quit = \
			(int (WINAPI *)(HWND)) GetProcAddress(m_hSTKDrv,"Stock_Quit");
	m_pfnGetTotalNumber = \
			(int (WINAPI *)())GetProcAddress(m_hSTKDrv,"GetTotalNumber");
	m_pfnGetStockByNoEx = \
			(int (WINAPI *)(int,RCV_REPORT_STRUCTEx *))GetProcAddress(m_hSTKDrv,"GetStockByNoEx");
	m_pfnGetStockByCodeEx = \
			(int (WINAPI *)(char *,int,RCV_REPORT_STRUCTEx *))GetProcAddress(m_hSTKDrv,"GetStockByCodeEx");
	m_pfnSetupReceiver = \
			(int	(WINAPI *)(BOOL))GetProcAddress(m_hSTKDrv,"SetupReceiver");
	m_pfnGetStockDrvInfo = \
			(DWORD (WINAPI *)(int,void * ))GetProcAddress(m_hSTKDrv,"GetStockDrvInfo");

	return TRUE;
}

void CSTKDRV::UnloadDriver( )
{
	if( m_hSTKDrv )
	{
		FreeLibrary(m_hSTKDrv);
		m_hSTKDrv = NULL;

		m_pfnStock_Init = NULL;
		m_pfnStock_Quit = NULL;
		m_pfnGetTotalNumber = NULL;
		m_pfnGetStockByNoEx = NULL;
		m_pfnGetStockByCodeEx = NULL;
		m_pfnSetupReceiver = NULL;
		m_pfnGetStockDrvInfo = NULL;
	}
}

int CSTKDRV::Stock_Init(HWND hWnd, UINT uMsg, int nWorkMode)
{
	if( !m_pfnStock_Init )
		return -1;
	 return( (*m_pfnStock_Init)(hWnd,uMsg,nWorkMode));
}

int CSTKDRV::Stock_Quit(HWND hWnd)
{
	if( !m_pfnStock_Quit )
		return -1;
	return( (*m_pfnStock_Quit)(hWnd));
}

int CSTKDRV::GetTotalNumber()
{
	if( !m_pfnGetTotalNumber )
		return 0;
	return( (*m_pfnGetTotalNumber)());
}

int CSTKDRV::GetStockByNoEx(int nNo,RCV_REPORT_STRUCTEx * pBuf)
{
	if( !m_pfnGetStockByNoEx )
		return 0;
	return( (*m_pfnGetStockByNoEx)(nNo,pBuf));
}

int CSTKDRV::GetStockByCodeEx(char * pszStockCode,int nMarket,RCV_REPORT_STRUCTEx * pBuf)
{
	if( !m_pfnGetStockByCodeEx )
		return 0;
	return( (*m_pfnGetStockByCodeEx)(pszStockCode,nMarket,pBuf));
}

int CSTKDRV::SetupReceiver(BOOL bShowWindow)
{
	if( !m_pfnSetupReceiver )
		return -1;
	return( (*m_pfnSetupReceiver)(bShowWindow));
}

DWORD CSTKDRV::GetStockDrvInfo(int nInfo, void *pBuf)
{
	if( !m_pfnGetStockDrvInfo ) 
		return 0;
	return( (*m_pfnGetStockDrvInfo)(nInfo,pBuf));
}

////////////////////////////////////////////////////////////////////////////////////////
// CNetSTKDRV

CNetSTKDRV::CNetSTKDRV()
{
	m_pfnStock_Init_Nodialog=	NULL;
	m_pfnIsEngineWorking	=	NULL;
	m_pfnSetAutoReport		=	NULL;
	m_pfnRequestStockData	=	NULL;
}

CNetSTKDRV::~CNetSTKDRV()
{
}

BOOL CNetSTKDRV::LoadDriver( LPCTSTR lpszDrvPath )
{
	if( CSTKDRV::LoadDriver( lpszDrvPath ) )
	{
		m_pfnStock_Init_Nodialog = \
				(int (WINAPI *)(HWND,UINT,int,const char*,UINT,const char*,const char*)) GetProcAddress(m_hSTKDrv,"Stock_Init_Nodialog");

		m_pfnIsEngineWorking = \
				(int (WINAPI *)())GetProcAddress(m_hSTKDrv,"IsEngineWorking");
		m_pfnSetAutoReport = \
				(int (WINAPI *)(int))GetProcAddress(m_hSTKDrv,"SetAutoReport");
		m_pfnRequestStockData = \
				(int (WINAPI *)(int, STOCK_STRUCTEx *, int, int, int))GetProcAddress(m_hSTKDrv,"RequestStockData");
		return TRUE;
	}

	return FALSE;
}

void CNetSTKDRV::UnloadDriver( )
{
	CSTKDRV::UnloadDriver();

	m_pfnStock_Init_Nodialog=	NULL;
	m_pfnIsEngineWorking	=	NULL;
	m_pfnSetAutoReport		=	NULL;
	m_pfnRequestStockData	=	NULL;
}

int CNetSTKDRV::Stock_Init_Nodialog(HWND hWnd, UINT uMsg, int nWorkMode,
				const char *szAddress,UINT nPort,const char *szUser,const char *szPasswd)
{
	if( !m_pfnStock_Init_Nodialog )
		return -1;
	 return( (*m_pfnStock_Init_Nodialog)(hWnd,uMsg,nWorkMode,szAddress,nPort,szUser,szPasswd));
}

int CNetSTKDRV::IsEngineWorking()
{
	if( !m_pfnIsEngineWorking )
		return FALSE;
	return( (*m_pfnIsEngineWorking)());
}

int CNetSTKDRV::SetAutoReport( int bAutoReport )
{
	if( !m_pfnSetAutoReport )
		return -1;
	return( (*m_pfnSetAutoReport)(bAutoReport));
}

int CNetSTKDRV::RequestStockData( int nDataType, STOCK_STRUCTEx * pStocks, int nSize, int nKType, int nDataCount )
{
	if( !m_pfnRequestStockData )
		return 0;
	return( (*m_pfnRequestStockData)(nDataType,pStocks,nSize,nKType,nDataCount));
}
