#ifndef	thnht_hibrocast_stkfmt_h_
#define	thnht_hibrocast_stkfmt_h_

#if ! defined(__cplusplus) || defined(_MSC_VER) && _MSC_VER <= 1200

#include <time.h>

#else

#include <ctime>

using std::time_t;
using std::time;
using std::localtime;
using std::tm;

#endif

#ifdef	__cplusplus
extern "C" {
#endif

#pragma pack( push, 8 )

const unsigned MAX_PACKET_SIZE = 4096;

const unsigned short STKCODE_LEN = 10;
// const unsigned short STKNAME_LEN = 16;	与通视卡命名冲突
const unsigned short YX_STKNAME_LEN = 16;

const unsigned short MARKET_SH = 0;
const unsigned short MARKET_SZ = 1;
const unsigned short MARKET_STARTUP = 2;

// constants used by STOCK_MINLINE.market:
const unsigned char ML_MARKET_UNKNOWN = 0;
const unsigned char ML_MARKET_SH = (unsigned char)( MARKET_SH + 1 );
const unsigned char ML_MARKET_SZ = (unsigned char)( MARKET_SZ + 1 );
const unsigned char ML_MARKET_STARTUP = (unsigned char)( MARKET_STARTUP + 1 );

// packet type constants:

enum PT_CONST {
	PT_REPORT = 1,	// 行情数据
	PT_MINLINE = 2,
	PT_OUTLINE = 3,

	PT_THIS_ENUM_IS_AT_LEAST_TWO_BYTES_IN_SIZE = 0xffff
};

enum FILE_TYPE {
    FILE_NORMAL = 1,	// 通用新闻
    FILE_SHANEWS = 2,	// 沪市新闻
    FILE_SZNNEWS = 3,	// 深市新闻
    FILE_QSNEWS = 4,	// 券商新闻
	FILE_SHAGGMSG = 5,	// 上海个股信息
	FILE_SZNGGMSG = 6,	// 深圳个股信息
	FILE_SHDAYLINE = 7,	// 上海日线历史数据
	FILE_SZDAYLINE = 8,	// 深圳日线历史数据

	FILE_THIS_ENUM_IS_AT_LEAST_TWO_BYTES_IN_SIZE = 0xffff
};

typedef struct STOCK_REPORT {
	WORD	m_wMarket;					// 证券市场类型
	char	m_szCode[STKCODE_LEN];		// 证券代码,以'\0'结尾
	char	m_szName[YX_STKNAME_LEN];	// 证券名称,以'\0'结尾

	BYTE	m_btFlag;					// 停牌标志
	char	fill1[3];

	float	m_fLast;					// 昨收
	float	m_fOpen;					// 今开
	float	m_fHigh;					// 最高
	float	m_fLow;						// 最低
	float	m_fNew;						// 最新
	DWORD	m_dwVolume;					// 成交量
	float	m_fAmount;					// 成交额
	DWORD	m_dwDeltaVolume;			// 成交量变化
	float	m_fDeltaAmount;				// 成交额变化

	float	m_fBuyPrice[3];				// 申买价1,2,3
	DWORD	m_dwBuyVolume[3];			// 申买量1,2,3
	float	m_fSellPrice[3];			// 申卖价1,2,3
	DWORD	m_dwSellVolume[3];			// 申卖量1,2,3
} STOCK_REPORT;

typedef struct STOCK_REPORTSET {
	time_t	m_time;
	WORD	m_wReportCount;
	WORD	m_wDataSize;
	STOCK_REPORT	data[1];
} STOCK_REPORTSET;

typedef struct STOCK_MINLINE {
	BYTE	m_btType;	// 1 min, 5 min, 15 min, 30 min line

	char	m_szCode[STKCODE_LEN];
	unsigned char	market;		// see ML_MARKET_*.

	time_t	m_time;
	float	m_fHigh;
	float	m_fLow;
	float	m_fNew;
	DWORD	m_dwDeltaVolume;
	float	m_fDeltaAmount;
	DWORD	m_dwVolume;
	float	m_fAmount;
} STOCK_MINLINE;

typedef	struct STOCK_MINLINESET {
	WORD	m_wLineCount;
	WORD	m_wDataSize;
	STOCK_MINLINE	data[1];
} STOCK_MINLINESET;

typedef struct STOCK_OUTLINE {
	time_t	m_time;

	DWORD	m_dwShHq;
	DWORD	m_dwSzHq;
	DWORD	m_dwShPriceUp;
	DWORD	m_dwShPriceDown;
	DWORD	m_dwSzPriceUp;
	DWORD	m_dwSzPriceDown;
	DWORD	m_dwShWbUp;
	DWORD	m_dwShWbDown;
	DWORD	m_dwSzWbUp;
	DWORD	m_dwSzWbDown;
	DWORD	m_dwShStockCount;
	DWORD	m_dwSzStockCount;

	DWORD	reserved;
} STOCK_OUTLINE;

typedef struct COMM_PACKET {
	WORD	m_wType;		// enum PT_CONST.
	WORD	m_wReserved;
	union
	{
		STOCK_REPORTSET		m_ReportSet;
		STOCK_MINLINESET	m_MinLineSet;
		STOCK_OUTLINE		m_Outline;
	};
} COMM_PACKET;

// 历史数据文件结构:

typedef struct HisItemDesc
{
	char	szCode[STKCODE_LEN];
	char	fill1[2];
	DWORD	dwItemStartPos;				// 本只股票STOCK_DAYLINE结构开始处
	DWORD	dwItemCount;				// 本只股票STOCK_DAYLINE结构数目
} HisItemDesc;

const DWORD HISC_MAGIC = DWORD('C') + (DWORD('S') << 8) + (DWORD('I') << 16) + (DWORD('H') << 24);

typedef struct HisCollectHeader
{
	DWORD	dwMagic;			// == HISC_MAGIC
	DWORD   dwComplement;		// == ~ dwMagic

	DWORD	dwItemDescCount;
	DWORD	dwItemDescSize;
	DWORD   dwItemDescOffset;           // HisItemDesc列表开始处
	DWORD   dwItemCollectOffset;        // STOCK_DAYLINE结构开始处
	//HisItemDesc	itemDesc[];			// 在偏移dwItemDescOffset处
	//STOCK_DAYLINE	item[];				// 在偏移dwItemCollectOffset处
} HisCollectHeader;

typedef struct STOCK_DAYLINE {
	WORD	m_wMarket;

	char	m_szCode[STKCODE_LEN];
	time_t	m_time;

	float	m_fOpen;
	float	m_fHigh;
	float	m_fLow;
	float	m_fClose;
	DWORD	m_dwVolume;
	float	m_fAmount;

	WORD	m_wAdvance;
	WORD	m_wDecline;
} STOCK_DAYLINE;


#pragma pack(pop)

#ifdef	__cplusplus
}
#endif

#endif
