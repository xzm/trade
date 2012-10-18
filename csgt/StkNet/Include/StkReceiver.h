#if !defined(AFX_STKRECEIVER_H__58975303_DC58_11D1_A883_00C04FCCA334__INCLUDED_)
#define AFX_STKRECEIVER_H__58975303_DC58_11D1_A883_00C04FCCA334__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StkReceiver.h : header file
//

#define	WM_APP_STKRECEIVER_CONNECTSERVER	WM_APP+1
#define	WM_APP_STKRECEIVER_DISCONNECTSERVER	WM_APP+2
#define	WM_APP_STKRECEIVER_INIT				WM_APP+3
#define	WM_APP_STKRECEIVER_SETUP			WM_APP+4
#define	WM_APP_STKRECEIVER_QUIT				WM_APP+5
#define	WM_APP_STKRECEIVER_DATA				WM_APP+6
#define	WM_APP_STKRECEIVER_ALARM			WM_APP+7
#define	WM_APP_STKRECEIVER_TSDATA			WM_APP+8	// 通视接收消息
#define	WM_APP_STKRECEIVER_YXDATA			WM_APP+9	// 永新接收消息

#define	STKRCV_ALARM_REFRESH			1
#define	STKRCV_ALARM_WARNING			2

class	CStkReceiver;
STKNET_API	CStkReceiver & AfxGetStkReceiver();

/////////////////////////////////////////////////////////////////////////////
// CStkReceiver window

struct	COMM_PACKET;

class STKNET_API CStkReceiver : public CWnd
{
// Construction
public:
	CStkReceiver();

	static HANDLE m_hEventKillProcessThread;
	static HANDLE m_hEventProcessThreadKilled;

// Attributes
public:
	BOOL	CreateReceiver( CWnd * pParentWnd );

	void	OnReceiveReport( REPORT * pReport );

	void	AddRcvAlarmWnd( HWND hWnd );
	BOOL	RemoveRcvAlarmWnd( HWND hWnd );
	void	NotifyWndRcvAlarm( WPARAM wParam, LPARAM lParam );

	void	AddRcvDataWnd( HWND hWnd );
	BOOL	RemoveRcvDataWnd( HWND hWnd );
	void	NotifyWndRcvData( WPARAM wParam, LPARAM lParam );

	// 网络接收引擎
	BOOL	NetEngineBeginWorking( );
	BOOL	NetEngineBeginWorking( LPCTSTR lpszAddress, UINT nPort, LPCTSTR lpszUser, LPCTSTR lpszPasswd );
	BOOL	NetEngineEndWorking( );
	BOOL	NetEngineIsWorking( );
	int		NetEngineSetAutoReport( int bAutoReport );
	int		RequestStockData( int nDataType/*CStock::DataType*/, CStockInfo * pStockInfo, int nSize, int nKType, int nDataCount );
	int		RequestKData( CStock *pstock, int period );

	// 通视卡和永新卡公共方法
	BOOL	EngineBeginWorking( BOOL bShowSelectEngineDialog );
	BOOL	EngineSetup( );
	BOOL	EngineEndWorking( );
	BOOL	EngineIsWorking( );

	int		RefreshStockContainer( CStockContainer & container, BOOL bAddIfNotExist );
	int		GetReport( CStockInfo &info, CReport &aReport );

// Operations
protected:
	// 永新卡使用方法
	BOOL	ProcessYXPacket( COMM_PACKET &packet, DWORD dwSize );
	BOOL	ProcessReceivedYXFile( LPCTSTR lpszFileName );


protected:
	// 公共数据
	CArray<HWND,HWND>	m_awndRcvData;
	CArray<HWND,HWND>	m_awndRcvAlarm;

	// 通视卡使用数据
	BOOL	m_bTongshiRunning;
	
	// 永新卡使用数据
	BOOL	m_bYongxinRunning;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStkReceiver)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStkReceiver();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStkReceiver)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnStkReceiverConnectserver( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStkReceiverDisconnectserver( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStkReceiverInit( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStkReceiverSetup( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStkReceiverQuit( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStkReceiverTsdata(UINT wFileType,LONG lPara);
	afx_msg LRESULT OnStkReceiverYxdata(UINT wFileType,LONG lPara);
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STKRECEIVER_H__58975303_DC58_11D1_A883_00C04FCCA334__INCLUDED_)
