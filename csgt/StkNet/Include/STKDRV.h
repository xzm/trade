// STKDRV.h: interface for the CSTKDRV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STKDRV_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
#define AFX_STKDRV_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSTKDRV  
{
public:
	CSTKDRV();
	virtual ~CSTKDRV();
public:
	virtual	BOOL	LoadDriver( LPCTSTR lpszDrvPath );
	virtual	void	UnloadDriver( );

	virtual	int Stock_Init(HWND hWnd,UINT uMsg,int nWorkMode);
	virtual	int Stock_Quit(HWND hWnd);
	virtual	int GetTotalNumber();
	virtual	int GetStockByNoEx(int nNo,RCV_REPORT_STRUCTEx * pBuf);
	virtual	int GetStockByCodeEx(char * pszStockCode,int nMarket,RCV_REPORT_STRUCTEx * pBuf);
	virtual	int SetupReceiver(BOOL bShowWindow);
	virtual	DWORD GetStockDrvInfo(int nInfo,void * pBuf);

protected:
	int (WINAPI *	m_pfnStock_Init)(HWND hWnd,UINT Msg,int nWorkMode);
	int (WINAPI *	m_pfnStock_Quit)(HWND hWnd);
	int	(WINAPI *	m_pfnGetTotalNumber)();													
	int (WINAPI *	m_pfnGetStockByNoEx)(int,RCV_REPORT_STRUCTEx *);
	int (WINAPI *	m_pfnGetStockByCodeEx)(char *,int,RCV_REPORT_STRUCTEx *);
	int	(WINAPI *	m_pfnSetupReceiver)(BOOL bSetup);
	DWORD (WINAPI * m_pfnGetStockDrvInfo)(int nInfo,void * pBuf);

	HINSTANCE	m_hSTKDrv;
};

class CNetSTKDRV : public CSTKDRV
{
public:
	CNetSTKDRV();
	virtual ~CNetSTKDRV();

	virtual	BOOL	LoadDriver( LPCTSTR lpszDrvPath );
	virtual	void	UnloadDriver( );

	virtual	int		Stock_Init_Nodialog(HWND hWnd,UINT uMsg,int nWorkMode,
								const char *szAddress, UINT nPort,
								const char *szUser, const char * szPasswd);
	virtual int		IsEngineWorking( );
	virtual int		SetAutoReport( int bAutoReport );
	virtual	int		RequestStockData( int nDataType, STOCK_STRUCTEx * pStocks, int nSize, int nKType, int nDataCount );

protected:
	int (WINAPI *	m_pfnStock_Init_Nodialog)(HWND hWnd,UINT Msg,int nWorkMode,
								const char *szAddress, UINT nPort,
								const char *szUser, const char * szPasswd );
	int (WINAPI *	m_pfnIsEngineWorking)();
	int (WINAPI *	m_pfnSetAutoReport)(int bAutoReport);
	int (WINAPI *	m_pfnRequestStockData)(int nDataType, STOCK_STRUCTEx * pStocks, int nSize, int nKType, int nDataCount );
};

#endif // !defined(AFX_STKDRV_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
