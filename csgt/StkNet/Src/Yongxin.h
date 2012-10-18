/*
	Cross Platform Core Code.

	Copyright(R) 2001-2002 Balang Software.
	All rights reserved.

	Using:
		Yongxin interface convertor functions.
*/

#if	!defined( __SP_YONGXIN_H__ )
#define	__SP_YONGXIN_H__

DWORD	YXMarket_to_Market( WORD wMarket );
DWORD	MLMarket_to_Market( WORD market );
BOOL convert_YXREPORT_to_REPORT( time_t time, STOCK_REPORT * pYXReport, REPORT * pReport );
BOOL convert_YXMINLINE_to_MINUTE( STOCK_MINLINE * pYXMinline, MINUTE * pMinute );
BOOL convert_YXOUTLINE_to_OUTLINE( STOCK_OUTLINE * pYXOutline, OUTLINE * pOutline );
BOOL convert_DAYLINE_to_KDATA( STOCK_DAYLINE * pYXDayline, KDATA * pkd );

#endif	// __SP_YONGXIN_H__

