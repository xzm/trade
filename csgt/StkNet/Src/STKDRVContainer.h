// STKDRVContainer.h: interface for the CSTKDRVContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STKDRVCONTAINER_H__E48FE107_3526_4F96_AB1A_27AC23E7355B__INCLUDED_)
#define AFX_STKDRVCONTAINER_H__E48FE107_3526_4F96_AB1A_27AC23E7355B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stockdrv.h"
#include "STKDRV.h"

class CSTKDRVContainer  
{
public:
	CSTKDRVContainer();
	virtual ~CSTKDRVContainer();

public:
	int		LoadDriver( CStringArray & astrDriverPath );
	void	UnloadDriver( );

	int		Stock_Init(HWND hWnd,UINT uMsg,int nWorkMode);
	int		Stock_Quit(HWND hWnd);
	int		GetTotalNumber();
	int		GetStockByNoEx(int nNo,RCV_REPORT_STRUCTEx * pBuf);
	int		GetStockByCodeEx(char * pszStockCode,int nMarket,RCV_REPORT_STRUCTEx * pBuf);
	int		SetupReceiver(BOOL bShowWindow);

protected:
	CArray<CSTKDRV,CSTKDRV &>	m_aDriver;
};

#endif // !defined(AFX_STKDRVCONTAINER_H__E48FE107_3526_4F96_AB1A_27AC23E7355B__INCLUDED_)
