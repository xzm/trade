/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		Yongxin interface convertor functions;
*/

#include	"StdAfx.h"
#include	"stkfmt.h"
#include	"stkfilter.h"
#include	"cppstkfilter.h"
#include	"Yongxin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
DWORD	YXMarket_to_Market( WORD wMarket )
{
	DWORD	dwMarket	=	CStock::marketCHNA;

	if( MARKET_SH == wMarket )
		dwMarket	=	CStock::marketSHSE;
	else if( MARKET_SZ == wMarket )
		dwMarket	=	CStock::marketSZSE;
	else if( MARKET_STARTUP == wMarket )
		dwMarket =	CStock::marketCYSE;
	return dwMarket;
}

DWORD	MLMarket_to_Market( WORD market )
{
	DWORD	dwMarket	=	CStock::marketCHNA;
	
	if( ML_MARKET_SH == market )
		dwMarket	=	CStock::marketSHSE;
	else if( ML_MARKET_SZ == market )
		dwMarket	=	CStock::marketSZSE;
	else if( ML_MARKET_STARTUP == market )
		dwMarket =	CStock::marketCYSE;
	return dwMarket;
}

BOOL convert_YXREPORT_to_REPORT( time_t time, STOCK_REPORT * pYXReport, REPORT * pReport )
{
	SP_ASSERT( pYXReport && pReport );
	if( NULL == pYXReport || NULL == pReport )
		return FALSE;

	memset( pReport, 0, sizeof(REPORT) );

	// 股票市场
	pReport->m_dwMarket	=	YXMarket_to_Market( pYXReport->m_wMarket );

	strncpy( pReport->m_szCode, pYXReport->m_szCode, min(sizeof(pReport->m_szCode)-1,sizeof(pYXReport->m_szCode)) );
	strncpy( pReport->m_szName, pYXReport->m_szName, min(sizeof(pReport->m_szName)-1,sizeof(pYXReport->m_szName)) );

	pReport->m_dwFlag	=	pYXReport->m_btFlag;
	
	pReport->m_fBuyPrice[0]		=	pYXReport->m_fBuyPrice[0];
	pReport->m_fBuyPrice[1]		=	pYXReport->m_fBuyPrice[1];
	pReport->m_fBuyPrice[2]		=	pYXReport->m_fBuyPrice[2];
	
	pReport->m_fBuyVolume[0]	=	(float)(pYXReport->m_dwBuyVolume[0]);
	pReport->m_fBuyVolume[1]	=	(float)(pYXReport->m_dwBuyVolume[1]);
	pReport->m_fBuyVolume[2]	=	(float)(pYXReport->m_dwBuyVolume[2]);
	
	pReport->m_fSellPrice[0]	=	pYXReport->m_fSellPrice[0];
	pReport->m_fSellPrice[1]	=	pYXReport->m_fSellPrice[1];
	pReport->m_fSellPrice[2]	=	pYXReport->m_fSellPrice[2];

	pReport->m_fSellVolume[0]	=	(float)(pYXReport->m_dwSellVolume[0]);
	pReport->m_fSellVolume[1]	=	(float)(pYXReport->m_dwSellVolume[1]);
	pReport->m_fSellVolume[2]	=	(float)(pYXReport->m_dwSellVolume[2]);

	return TRUE;
}

BOOL convert_YXMINLINE_to_MINUTE( STOCK_MINLINE * pYXMinline, MINUTE * pMinute )
{
	SP_ASSERT( pYXMinline && pMinute );
	if( NULL == pYXMinline || NULL == pMinute )
		return FALSE;

	memset( pMinute, 0, sizeof(MINUTE) );

	// 股票市场
	pMinute->m_dwMarket	=	MLMarket_to_Market( pYXMinline->market );

	strncpy( pMinute->m_szCode, pYXMinline->m_szCode, min(sizeof(pMinute->m_szCode)-1,sizeof(pYXMinline->m_szCode)) );

	pMinute->m_dwType	=	pYXMinline->m_btType;


	pMinute->m_time		=	pYXMinline->m_time;
	pMinute->m_fNew		=	pYXMinline->m_fNew;
	pMinute->m_fHigh	=	pYXMinline->m_fHigh;
	pMinute->m_fLow		=	pYXMinline->m_fLow;
	pMinute->m_fVolume	=	(float)(pYXMinline->m_dwVolume);
	pMinute->m_fAmount	=	pYXMinline->m_fAmount;
	return TRUE;
}

BOOL convert_YXOUTLINE_to_OUTLINE( STOCK_OUTLINE * pYXOutline, OUTLINE * pOutline )
{
	SP_ASSERT( pYXOutline && pOutline );
	if( NULL == pYXOutline || NULL == pOutline )
		return FALSE;

	memset( pOutline, 0, sizeof(OUTLINE) );

	pOutline->m_time			=	pYXOutline->m_time;

	pOutline->m_dwShHq			=	pYXOutline->m_dwShHq;
	pOutline->m_dwSzHq			=	pYXOutline->m_dwSzHq;
	pOutline->m_dwShPriceUp		=	pYXOutline->m_dwShPriceUp;
	pOutline->m_dwShPriceDown	=	pYXOutline->m_dwShPriceDown;
	pOutline->m_dwSzPriceUp		=	pYXOutline->m_dwSzPriceUp;
	pOutline->m_dwSzPriceDown	=	pYXOutline->m_dwSzPriceDown;
	pOutline->m_dwShWbUp		=	pYXOutline->m_dwShWbUp;
	pOutline->m_dwShWbDown		=	pYXOutline->m_dwShWbDown;
	pOutline->m_dwSzWbUp		=	pYXOutline->m_dwSzWbUp;
	pOutline->m_dwSzWbDown		=	pYXOutline->m_dwSzWbDown;
	pOutline->m_dwShStockCount	=	pYXOutline->m_dwShStockCount;
	pOutline->m_dwSzStockCount	=	pYXOutline->m_dwSzStockCount;
	return TRUE;
}

BOOL convert_DAYLINE_to_KDATA( STOCK_DAYLINE * pYXDayline, KDATA * pkd )
{
	SP_ASSERT( pYXDayline && pkd );
	if( NULL == pYXDayline || NULL == pkd )
		return FALSE;

	memset( pkd, 0, sizeof(KDATA) );

	// 股票市场
	pkd->m_dwMarket	=	YXMarket_to_Market( pYXDayline->m_wMarket );

	strncpy( pkd->m_szCode, pYXDayline->m_szCode, min(sizeof(pkd->m_szCode)-1,sizeof(pYXDayline->m_szCode)) );

	pkd->m_time		=	pYXDayline->m_time;
	CSPTime	sptime(pkd->m_time);
	if( 0 != pkd->m_time && -1 != pkd->m_time )
		pkd->m_date	=	sptime.ToStockTimeDay();

	pkd->m_fOpen	=	pYXDayline->m_fOpen;
	pkd->m_fHigh	=	pYXDayline->m_fHigh;
	pkd->m_fLow		=	pYXDayline->m_fLow;
	pkd->m_fClose	=	pYXDayline->m_fClose;
	pkd->m_fVolume	=	(float)pYXDayline->m_dwVolume;
	pkd->m_fAmount	=	pYXDayline->m_fAmount;

	pkd->m_dwAdvance=	pYXDayline->m_wAdvance;
	pkd->m_dwDecline=	pYXDayline->m_wDecline;
	return TRUE;
}
