// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "StkUI.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "StaticDoc.h"
#include "View/WizardView.h"
#include "View/SimuView.h"
#include "View/SListView.h"
#include "View/RealTimeView.h"
#include "View/MultiSortView.h"
#include "View/GraphView.h"
#include "View/BaseView.h"
#include "View/InfoView.h"
#include "View/SelectorView.h"
#include "View/StrategyView.h"
#include "View/GroupView.h"
#include "View/TechsView.h"
#include "Dialog/DownloadDlg.h"
#include "Dialog/InstallPacDlg.h"
#include "Dialog/DataSrcDlg.h"
#include "Dialog/RegisterDlg.h"
#include "Dialog/SetColumnDlg.h"
#include "Dialog/BaseIndexDlg.h"
#include "Dialog/UpgradeDlg.h"
#include "Dialog/FilterInfoDlg.h"
#include "Dialog/SetGroupDlg.h"
#include "Dialog/SelectGrpDlg.h"
#include "Dialog/SetPrpt.h"
#include "Dialog/SelectStk.h"
#include "Dialog/SetRule.h"
#include "Dialog/SetRate.h"
#include "Dialog/SimuReport.h"
#include "Dialog/SimuRealOp.h"
#include "Dialog/ExportOption.h"
#include "Dialog/ExportDest.h"
#include "Dialog/SetDrdataDlg.h"
#include "Dialog/SetColorDlg.h"
#include "Dialog/NetInfoDlg.h"
#include "Dialog/TyDataDlg.h"
#include "Dialog/SelectTraderDlg.h"
#include "Dialog/SetBaseDlg.h"
#include "Dialog/SetBasedataDlg.h"
#include "Dialog/SetKDataDlg.h"
#include "Dialog/QuoteTipDlg.h"
#include "Dialog/AlarmTipDlg.h"
#include "Dialog/AlarmSettingDlg.h"
#include "Dialog/AlarmResultDlg.h"
#include "ParamDlg/SetParamDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

// toolbar buttons - IDs are command buttons
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // stock indicator, and progress indicator
	ID_SEPARATOR,           // stock indicator
	ID_SEPARATOR,           // time indicator
};

extern	TCHAR	szRegKeyCompany[];
extern	TCHAR	szRegKeyApp[];

IMPLEMENT_DYNCREATE(CMainFrame, CGuiMDIFrame)

BEGIN_MESSAGE_MAP(CMainFrame, CGuiMDIFrame)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_MAINBAR_SETTING, OnMainbarSetting)
	ON_COMMAND(ID_MAINBAR_VIEW, OnMainbarView)
	ON_COMMAND(ID_MAINBAR_TECH, OnMainbarTech)
	ON_COMMAND(ID_MAINBAR_PERIOD, OnMainbarPeriod)
	ON_COMMAND(ID_SLISTBAR_GROUP, OnSlistbarGroup)
	ON_COMMAND(ID_SLISTBAR_DOMAIN, OnSlistbarDomain)
	ON_COMMAND(ID_SLISTBAR_CLASS, OnSlistbarClass)

	ON_COMMAND(ID_SYS_CONNECTSERVER, OnSysConnectserver)
	ON_UPDATE_COMMAND_UI(ID_SYS_CONNECTSERVER, OnUpdateSysConnectserver)
	ON_COMMAND(ID_SYS_DISCONNECTSERVER, OnSysDisconnectserver)
	ON_UPDATE_COMMAND_UI(ID_SYS_DISCONNECTSERVER, OnUpdateSysDisconnectserver)
	ON_COMMAND(ID_SYS_STARTTONGSHI, OnSysStarttongshi)
	ON_UPDATE_COMMAND_UI(ID_SYS_STARTTONGSHI, OnUpdateSysStarttongshi)
	ON_COMMAND(ID_SYS_SETUPTONGSHI, OnSysSetuptongshi)
	ON_UPDATE_COMMAND_UI(ID_SYS_SETUPTONGSHI, OnUpdateSysSetuptongshi)
	ON_COMMAND(ID_SYS_STOPTONGSHI, OnSysStoptongshi)
	ON_UPDATE_COMMAND_UI(ID_SYS_STOPTONGSHI, OnUpdateSysStoptongshi)
	ON_COMMAND(ID_SYS_AUTOUPGRADE, OnSysAutoupgrade)
	ON_COMMAND(ID_SYS_AUTOSAVE, OnSysAutosave)
	ON_UPDATE_COMMAND_UI(ID_SYS_AUTOSAVE, OnUpdateSysAutosave)
	ON_COMMAND(ID_SYS_NOTIFYICON, OnSysNotifyicon)
	ON_UPDATE_COMMAND_UI(ID_SYS_NOTIFYICON, OnUpdateSysNotifyicon)

	ON_COMMAND(ID_VIEW_MAINBARTEXT, OnViewMainbartext)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINBARTEXT, OnUpdateViewMainbartext)
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateViewFullscreen)
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_VIEW_WIZARD, OnViewWizard)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WIZARD, OnUpdateViewWizard)
	ON_COMMAND(ID_VIEW_SIMU, OnViewSimu)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIMU, OnUpdateViewSimu)
	ON_COMMAND(ID_VIEW_SLIST, OnViewSlist)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SLIST, OnUpdateViewSlist)
	ON_COMMAND(ID_VIEW_REALTIME, OnViewRealtime)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REALTIME, OnUpdateViewRealtime)
	ON_COMMAND(ID_VIEW_MULTISORT, OnViewMultisort)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MULTISORT, OnUpdateViewMultisort)
	ON_COMMAND(ID_VIEW_GRAPH, OnViewTechgraph)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRAPH, OnUpdateViewTechgraph)
	ON_COMMAND(ID_VIEW_BASE, OnViewBase)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BASE, OnUpdateViewBase)
	ON_COMMAND(ID_VIEW_INFO, OnViewInfo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_INFO, OnUpdateViewInfo)
	ON_COMMAND(ID_VIEW_SELECTOR, OnViewSelector)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECTOR, OnUpdateViewSelector)
	ON_COMMAND(ID_VIEW_F10, OnViewF10)
	ON_UPDATE_COMMAND_UI(ID_VIEW_F10, OnUpdateViewF10)
	
	ON_COMMAND(ID_SLIST_INDEX, OnSlistIndex)
	ON_COMMAND(ID_SLIST_STRATEGY, OnSlistStrategy)
	ON_COMMAND(ID_SLIST_GROUP, OnSlistGroup)
	ON_COMMAND(ID_SLIST_GROUPSELF, OnSlistGroupself)
	ON_COMMAND(ID_SLIST_DOMAIN, OnSlistDomain)
	ON_COMMAND(ID_SLIST_ALL, OnSlistAll)
	ON_COMMAND(ID_SLIST_SHAA, OnSlistShaa)
	ON_COMMAND(ID_SLIST_SHAB, OnSlistShab)
	ON_COMMAND(ID_SLIST_SZNA, OnSlistSzna)
	ON_COMMAND(ID_SLIST_SZNB, OnSlistSznb)
	ON_COMMAND(ID_SLIST_SHABOND, OnSlistShabond)
	ON_COMMAND(ID_SLIST_SZNBOND, OnSlistSznbond)
	ON_COMMAND(ID_SLIST_SHAASORTDP, OnSlistShaasortdp)
	ON_COMMAND(ID_SLIST_SHABSORTDP, OnSlistShabsortdp)
	ON_COMMAND(ID_SLIST_SZNASORTDP, OnSlistSznasortdp)
	ON_COMMAND(ID_SLIST_SZNBSORTDP, OnSlistSznbsortdp)
	ON_COMMAND(ID_SLIST_SHABONDSORTDP, OnSlistShabondsortdp)
	ON_COMMAND(ID_SLIST_SZNBONDSORTDP, OnSlistSznbondsortdp)
	ON_COMMAND(ID_SLIST_SETCOLUMN, OnSlistSetcolumn)
	ON_COMMAND(ID_SLIST_BASEINDEX, OnSlistBaseindex)
	ON_COMMAND(ID_SLISTBAR_DATE, OnSlistbarDate)
	ON_COMMAND(ID_SLISTBAR_NAME, OnSlistbarName)

	ON_COMMAND(ID_VIEW_VIEWSZZS, OnViewViewszzs)
	ON_COMMAND(ID_VIEW_VIEWSZYBL, OnViewViewszybl)
	ON_COMMAND(ID_VIEW_VIEWSZAG, OnViewViewszag)
	ON_COMMAND(ID_VIEW_VIEWSZBG, OnViewViewszbg)
	ON_COMMAND(ID_VIEW_VIEWSZNCZ, OnViewViewszncz)
	ON_COMMAND(ID_VIEW_VIEWSZNZZ, OnViewViewsznzz)
	ON_COMMAND(ID_VIEW_VIEWSZNCFAZ, OnViewViewszncfaz)
	ON_COMMAND(ID_VIEW_VIEWSZNCFBZ, OnViewViewszncfbz)
	ON_COMMAND(ID_VIEW_TECHOPTION, OnViewTechoption)
	ON_COMMAND(ID_VIEW_PDAY, OnViewPday)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PDAY, OnUpdateViewPday)
	ON_COMMAND(ID_VIEW_PWEEK, OnViewPweek)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PWEEK, OnUpdateViewPweek)
	ON_COMMAND(ID_VIEW_PMONTH, OnViewPmonth)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PMONTH, OnUpdateViewPmonth)
	ON_COMMAND(ID_VIEW_PMIN5, OnViewPmin5)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PMIN5, OnUpdateViewPmin5)
	ON_COMMAND(ID_VIEW_PMIN15, OnViewPmin15)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PMIN15, OnUpdateViewPmin15)
	ON_COMMAND(ID_VIEW_PMIN30, OnViewPmin30)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PMIN30, OnUpdateViewPmin30)
	ON_COMMAND(ID_VIEW_PMIN60, OnViewPmin60)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PMIN60, OnUpdateViewPmin60)
	ON_COMMAND(ID_VIEW_LEFT, OnViewLeft)
	ON_COMMAND(ID_VIEW_RIGHT, OnViewRight)
	ON_COMMAND(ID_VIEW_PAGELEFT, OnViewPageleft)
	ON_COMMAND(ID_VIEW_PAGERIGHT, OnViewPageright)
	ON_COMMAND(ID_VIEW_HOME, OnViewHome)
	ON_COMMAND(ID_VIEW_END, OnViewEnd)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_COMMAND(ID_VIEW_PREV, OnViewPrev)
	ON_COMMAND(ID_VIEW_NEXT, OnViewNext)
	ON_COMMAND(ID_VIEW_INSERT, OnViewInsert)
	ON_COMMAND(ID_VIEW_NORESUMEDR, OnViewNoresumedr)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NORESUMEDR, OnUpdateViewNoresumedr)
	ON_COMMAND(ID_VIEW_RESUMEDRUP, OnViewResumedrup)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESUMEDRUP, OnUpdateViewResumedrup)
	ON_COMMAND(ID_VIEW_RESUMEDRDOWN, OnViewResumedrdown)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESUMEDRDOWN, OnUpdateViewResumedrdown)
	ON_COMMAND(ID_VIEW_MAINDATACLOSE, OnViewMaindataclose)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINDATACLOSE, OnUpdateViewMaindataclose)
	ON_COMMAND(ID_VIEW_MAINDATAOPEN, OnViewMaindataopen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINDATAOPEN, OnUpdateViewMaindataopen)
	ON_COMMAND(ID_VIEW_MAINDATAAVERAGE, OnViewMaindataaverage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINDATAAVERAGE, OnUpdateViewMaindataaverage)
	ON_COMMAND(ID_VIEW_KLINECANDLE, OnViewKlinecandle)
	ON_UPDATE_COMMAND_UI(ID_VIEW_KLINECANDLE, OnUpdateViewKlinecandle)
	ON_COMMAND(ID_VIEW_KLINEAMERICA, OnViewKlineamerica)
	ON_UPDATE_COMMAND_UI(ID_VIEW_KLINEAMERICA, OnUpdateViewKlineamerica)
	ON_COMMAND(ID_VIEW_KLINETOWER, OnViewKlinetower)
	ON_UPDATE_COMMAND_UI(ID_VIEW_KLINETOWER, OnUpdateViewKlinetower)

	ON_COMMAND(ID_REALTIME_VIEWSZZS, OnRealtimeViewszzs)
	ON_COMMAND(ID_REALTIME_VIEWSZYBL, OnRealtimeViewszybl)
	ON_COMMAND(ID_REALTIME_VIEWSZAG, OnRealtimeViewszag)
	ON_COMMAND(ID_REALTIME_VIEWSZBG, OnRealtimeViewszbg)
	ON_COMMAND(ID_REALTIME_VIEWSZNCZ, OnRealtimeViewszncz)
	ON_COMMAND(ID_REALTIME_VIEWSZNZZ, OnRealtimeViewsznzz)
	ON_COMMAND(ID_REALTIME_VIEWSZNCFAZ, OnRealtimeViewszncfaz)
	ON_COMMAND(ID_REALTIME_VIEWSZNCFBZ, OnRealtimeViewszncfbz)
	ON_COMMAND(ID_REALTIME_LEFT, OnRealtimeLeft)
	ON_COMMAND(ID_REALTIME_RIGHT, OnRealtimeRight)
	ON_COMMAND(ID_REALTIME_HOME, OnRealtimeHome)
	ON_COMMAND(ID_REALTIME_END, OnRealtimeEnd)
	ON_COMMAND(ID_REALTIME_PREV, OnRealtimePrev)
	ON_COMMAND(ID_REALTIME_NEXT, OnRealtimeNext)
	ON_COMMAND(ID_REALTIME_ONESTOCK, OnRealtimeOnestock)
	ON_COMMAND(ID_REALTIME_TWOSTOCK, OnRealtimeTwostock)
	ON_COMMAND(ID_REALTIME_FOURSTOCK, OnRealtimeFourstock)
	ON_COMMAND(ID_REALTIME_SIXSTOCK, OnRealtimeSixstock)
	ON_COMMAND(ID_REALTIME_NINESTOCK, OnRealtimeNinestock)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_ONESTOCK, OnUpdateRealtimeOnestock)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_TWOSTOCK, OnUpdateRealtimeTwostock)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_FOURSTOCK, OnUpdateRealtimeFourstock)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_SIXSTOCK, OnUpdateRealtimeSixstock)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_NINESTOCK, OnUpdateRealtimeNinestock)
	ON_COMMAND(ID_REALTIME_PRICELINE, OnRealtimePriceline)
	ON_COMMAND(ID_REALTIME_REPORTDETAIL, OnRealtimeReportdetail)
	ON_COMMAND(ID_REALTIME_MINUTEDETAIL, OnRealtimeMinutedetail)
	ON_COMMAND(ID_REALTIME_BIGTRADEDETAIL, OnRealtimeBigtradedetail)
	ON_COMMAND(ID_REALTIME_LBDK, OnRealtimeLbdk)
	ON_COMMAND(ID_REALTIME_MMLD, OnRealtimeMmld)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_PRICELINE, OnUpdateRealtimePriceline)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_REPORTDETAIL, OnUpdateRealtimeReportdetail)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_MINUTEDETAIL, OnUpdateRealtimeMinutedetail)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_BIGTRADEDETAIL, OnUpdateRealtimeBigtradedetail)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_LBDK, OnUpdateRealtimeLbdk)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_MMLD, OnUpdateRealtimeMmld)
	ON_COMMAND(ID_REALTIME_MSSZAG, OnRealtimeMsszag)
	ON_COMMAND(ID_REALTIME_MSSZBG, OnRealtimeMsszbg)
	ON_COMMAND(ID_REALTIME_MSSZZQ, OnRealtimeMsszzq)
	ON_COMMAND(ID_REALTIME_MSSZNAG, OnRealtimeMssznag)
	ON_COMMAND(ID_REALTIME_MSSZNBG, OnRealtimeMssznbg)
	ON_COMMAND(ID_REALTIME_MSSZNZQ, OnRealtimeMssznzq)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_MSSZAG, OnUpdateRealtimeMsszag)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_MSSZBG, OnUpdateRealtimeMsszbg)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_MSSZZQ, OnUpdateRealtimeMsszzq)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_MSSZNAG, OnUpdateRealtimeMssznag)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_MSSZNBG, OnUpdateRealtimeMssznbg)
	ON_UPDATE_COMMAND_UI(ID_REALTIME_MSSZNZQ, OnUpdateRealtimeMssznzq)

	ON_COMMAND(ID_INFO_SELF, OnInfoSelf)
	ON_COMMAND(ID_INFO_F10, OnInfoF10)
	ON_COMMAND(ID_INFO_STOCKSTAR, OnInfoStockstar)
	ON_COMMAND(ID_INFO_SOHU, OnInfoSohu)
	ON_COMMAND(ID_INFO_SINA, OnInfoSina)
	ON_COMMAND(ID_INFO_163, OnInfo163)
	ON_COMMAND(ID_INFO_SHSE, OnInfoShse)
	ON_COMMAND(ID_INFO_SZSE, OnInfoSzse)
	ON_COMMAND(ID_INFO_HKEX, OnInfoHkex)
	ON_COMMAND(ID_INFO_LSE, OnInfoLse)
	ON_COMMAND(ID_INFO_NYSE, OnInfoNyse)
	ON_COMMAND(ID_INFO_NASDAQ, OnInfoNasdaq)
	ON_COMMAND(ID_INFO_P5W, OnInfoP5w)
	ON_COMMAND(ID_INFO_SSNEWS, OnInfoSsnews)
	ON_COMMAND(ID_INFO_CS, OnInfoCs)
	ON_UPDATE_COMMAND_UI(ID_INFO_SELF, OnUpdateInfoSelf)
	ON_UPDATE_COMMAND_UI(ID_INFO_F10, OnUpdateInfoF10)
	ON_UPDATE_COMMAND_UI(ID_INFO_STOCKSTAR, OnUpdateInfoStockstar)
	ON_UPDATE_COMMAND_UI(ID_INFO_SOHU, OnUpdateInfoSohu)
	ON_UPDATE_COMMAND_UI(ID_INFO_SINA, OnUpdateInfoSina)
	ON_UPDATE_COMMAND_UI(ID_INFO_163, OnUpdateInfo163)
	ON_UPDATE_COMMAND_UI(ID_INFO_SHSE, OnUpdateInfoShse)
	ON_UPDATE_COMMAND_UI(ID_INFO_SZSE, OnUpdateInfoSzse)
	ON_UPDATE_COMMAND_UI(ID_INFO_HKEX, OnUpdateInfoHkex)
	ON_UPDATE_COMMAND_UI(ID_INFO_LSE, OnUpdateInfoLse)
	ON_UPDATE_COMMAND_UI(ID_INFO_NYSE, OnUpdateInfoNyse)
	ON_UPDATE_COMMAND_UI(ID_INFO_NASDAQ, OnUpdateInfoNasdaq)
	ON_UPDATE_COMMAND_UI(ID_INFO_P5W, OnUpdateInfoP5w)
	ON_UPDATE_COMMAND_UI(ID_INFO_SSNEWS, OnUpdateInfoSsnews)
	ON_UPDATE_COMMAND_UI(ID_INFO_CS, OnUpdateInfoCs)
	
	ON_COMMAND(ID_STRATEGY_PROPERTY, OnStrategyProperty)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_PROPERTY, OnUpdateStrategyProperty)
	ON_COMMAND(ID_STRATEGY_STOCKS, OnStrategyStocks)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_STOCKS, OnUpdateStrategyStocks)
	ON_COMMAND(ID_STRATEGY_SETRULE, OnStrategySetRule)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_SETRULE, OnUpdateStrategySetRule)
	ON_COMMAND(ID_STRATEGY_SETRATE, OnStrategySetRate)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_SETRATE, OnUpdateStrategySetRate)
	ON_COMMAND(ID_STRATEGY_SIMU, OnStrategySimu)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_SIMU, OnUpdateStrategySimu)
	ON_COMMAND(ID_STRATEGY_REPORT, OnStrategyReport)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_REPORT, OnUpdateStrategyReport)
	ON_COMMAND(ID_STRATEGY_REALOP, OnStrategyRealOp)
	ON_UPDATE_COMMAND_UI(ID_STRATEGY_REALOP, OnUpdateStrategyRealOp)

	ON_COMMAND(ID_DATA_DOWNLOAD, OnDataDownload)
	ON_COMMAND(ID_DATA_DATASOURCE, OnDataDatasource)
	ON_COMMAND(ID_DATA_MANAGE, OnDataManage)
	ON_COMMAND(ID_DATA_EXPORT, OnDataExport)
	ON_COMMAND(ID_DATA_RECALCULATEYIELD, OnDataRecalculateyield)

	ON_COMMAND(ID_OPTION_PROXYSET, OnOptionProxyset)
	ON_COMMAND(ID_OPTION_SELECTTRADER, OnOptionSelecttrader)
	ON_COMMAND(ID_OPTION_COLOR, OnOptionColor)
	ON_COMMAND(ID_OPTION_FONTSLIST, OnOptionFontslist)
	ON_COMMAND(ID_OPTION_FONTBASE, OnOptionFontbase)
	ON_COMMAND(ID_OPTION_SETGROUP, OnOptionSetgroup)
	ON_COMMAND(ID_OPTION_ADDTOSTRATEGY, OnOptionAddtostrategy)
	ON_UPDATE_COMMAND_UI(ID_OPTION_ADDTOSTRATEGY, OnUpdateOptionAddtostrategy)

	ON_COMMAND(ID_TOOL_TRADER, OnToolTrader)
	ON_COMMAND(ID_TOOL_NOTE, OnToolNote)
	ON_COMMAND(ID_TOOL_CALCULATOR, OnToolCalculator)
	ON_COMMAND(ID_TOOL_FILTERINFO, OnToolFilterinfo)
	ON_COMMAND(ID_TOOL_ALARM, OnToolAlarm)

	ON_COMMAND(ID_HELP_SHOWHELP, OnHelpShowhelp)
	ON_COMMAND(ID_HELP_PURCHASE, OnHelpPurchase)
	ON_COMMAND(ID_HELP_PRODUCT, OnHelpProduct)
	ON_COMMAND(ID_HELP_REGISTER, OnHelpRegister)
	ON_COMMAND(ID_HELP_DECLARE, OnHelpDeclare)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_NOTIFY(TBN_DROPDOWN, IDW_MAINBAR, OnMainBarDropDown )
	ON_NOTIFY(TBN_DROPDOWN, IDW_SLISTBAR, OnSlistBarDropDown )
	ON_COMMAND_RANGE(ID_SLIST_DOMAIN_START, ID_SLIST_DOMAIN_END, OnSlistDomainRange)
	ON_COMMAND_RANGE(ID_SLIST_GROUP_START, ID_SLIST_GROUP_END, OnSlistGroupRange)
	ON_COMMAND_RANGE(ID_VIEW_TECH_START, ID_VIEW_TECH_END, OnViewTechRange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_TECH_START, ID_VIEW_TECH_END, OnUpdateViewTechRange)
	ON_COMMAND_RANGE(ID_OPTION_ADDTOGROUP_START, ID_OPTION_ADDTOGROUP_END, OnOptionAddtogroupRange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPTION_ADDTOGROUP_START, ID_OPTION_ADDTOGROUP_END, OnUpdateOptionAddtogroupRange)
	ON_UPDATE_COMMAND_UI(IDW_MENUBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDW_MAINBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDW_MAINBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDW_VIEWBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDW_VIEWBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDW_PERIODBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDW_PERIODBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDW_SLISTBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDW_SLISTBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDW_SIMUBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDW_SIMUBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDW_WORKSPBAR, OnUpdateControlBarMenu)
	ON_COMMAND(IDW_WORKSPBAR, OnToggleWorkspBar)
	ON_MESSAGE(WM_USER_CHECKSN, CreateValid)
	ON_MESSAGE(WM_USER_UPDATEBARMENU, OnUpdateBarMenu)
	ON_MESSAGE(WM_USER_NIFMESSAGE, OnNifMessage)
	ON_MESSAGE(WM_USER_INITDATES, OnInitDates)
	ON_MESSAGE(WM_USER_UPDATESLISTVIEW, OnUpdateSlistView)
	ON_MESSAGE(WM_APP_STKRECEIVER_ALARM, OnStkReceiverAlarm)
END_MESSAGE_MAP()


CMainFrame* CMainFrame::CreateNewFrame( )
{
	CRuntimeClass* pFrameClass = RUNTIME_CLASS(CMainFrame);
	
	CMainFrame* pFrame = (CMainFrame*)pFrameClass->CreateObject();
	if (pFrame == NULL)
	{
		TRACE1("Warning: Dynamic create of frame %hs failed.\n",
			pFrameClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CMainFrame, pFrame);

	// create new from resource
	if (!pFrame->LoadFrame(IDR_MAINFRAME) )
	{
		TRACE0("Warning: Couldn't create a frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}

	// it worked !
	return pFrame;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	memset( &m_fullScreen, 0, sizeof(m_fullScreen) );
	m_fullScreen.bFullScreen	=	FALSE;

	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;
}

CMainFrame::~CMainFrame()
{
}

#define FRM_SEARCH_WIDTH	180
#define FRM_SEARCH_HEIGHT	22

#define	SLB_DATE_WIDTH		135
#define	SLB_DATE_HEIGHT		22

BOOL CMainFrame::CreateMenuBar( )
{
	if( -1 == InitMenu(IDR_MAINFRAME) )
		return FALSE;

	m_wndCool.LoadToolbar( IDR_MAINBAR );
	m_wndCool.LoadToolbar( IDR_VIEWBAR );
	m_wndCool.LoadToolbar( IDR_PERIODBAR );
	m_wndCool.LoadToolbar( IDR_SLISTBAR );
	m_wndCool.LoadToolbar( IDR_SIMUBAR );

	m_wndMenuBar.SetDlgCtrlID( IDW_MENUBAR );
	CString	strBarTitle;
	strBarTitle.LoadString( IDS_TITLE_MENUBAR );
	m_wndMenuBar.SetWindowText( strBarTitle );

	// Date ComboBox
	if( !m_wndMenuBar.CreateCombo( &m_DateBox, ID_SLISTBAR_DATE, SLB_DATE_WIDTH,
		WS_CHILD | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL | CBS_HASSTRINGS ) )
	{
		TRACE0("Failed to create m_DateBox\n");
		return FALSE;
	}
	m_DateBox.SetItemHeight( -1, 16 );
	
	return TRUE;
	
	/* ProfUIS Using Code
	CString	strBarTitle;
	strBarTitle.LoadString( IDS_TITLE_MENUBAR );
	m_wndMenuBar.SetUpdateFileMruList(FALSE);
	m_wndMenuBar.SetMdiWindowPopupName( _T("Window") );
	if( !m_wndMenuBar.Create( strBarTitle, this, IDW_MENUBAR )
		|| !m_wndMenuBar.LoadMenuBar(IDR_MAINFRAME) )
	{
		TRACE0("Failed to create menubar\n");
		return FALSE;      // failed to create
	}
	return TRUE;
	*/
}

BOOL CMainFrame::CreateMainBar( )
{
	if( !m_wndMainBar.CreateTextToolBar( this, IDW_MAINBAR, IDR_MAINBAR,
		-1, -1, CTextToolBar::labelBottom ) )
	{
		TRACE0("Failed to create mainbar\n");
		return FALSE;      // fail to create
	}
//	if (!m_wndMainBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
//		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),IDW_MAINBAR)
//		|| !m_wndMainBar.LoadToolBar( IDR_MAINBAR ) )
//	{
//		TRACE0("Failed to create mainbar\n");
//		return FALSE;      // fail to create
//	}
	CString	strBarTitle;
	strBarTitle.LoadString( IDS_TITLE_MAINBAR );
	m_wndMainBar.SetWindowText( strBarTitle );

	m_wndMainBar.SetButtonDropDown( ID_MAINBAR_SETTING, m_wndMainBar.CommandToIndex(ID_MAINBAR_SETTING), IDR_MENU_MAINBARSETTING);
	m_wndMainBar.SetButtonDropDown( ID_MAINBAR_VIEW, m_wndMainBar.CommandToIndex(ID_MAINBAR_VIEW), IDR_MENU_MAINBARVIEW);
	m_wndMainBar.SetButtonDropDown( ID_MAINBAR_TECH, m_wndMainBar.CommandToIndex(ID_MAINBAR_TECH), IDR_MENU_MAINBARTECH);
	m_wndMainBar.SetButtonDropDown( ID_MAINBAR_PERIOD, m_wndMainBar.CommandToIndex(ID_MAINBAR_PERIOD), IDR_MENU_MAINBARPERIOD);
	return TRUE;

	/* ProfUIS Using Code
	CString	strBarTitle;
	strBarTitle.LoadString( IDS_TITLE_MAINBAR );
	if( !m_wndMainBar.Create( strBarTitle, this, IDW_MAINBAR )
		|| !m_wndMainBar.LoadToolBar( IDR_MAINBAR ) )
	{
		TRACE0("Failed to create mainbar\n");
		return FALSE;      // fail to create
	}

	CMenu menu;

	// Main Bar
	VERIFY( menu.LoadMenu( IDR_MENU_MAINBARSETTING ) );
	VERIFY( m_wndMainBar.SetButtonMenu( m_wndMainBar.CommandToIndex( ID_MAINBAR_SETTING ), menu.Detach(), TRUE ) );

	VERIFY( menu.LoadMenu( IDR_MENU_MAINBARVIEW ) );
	VERIFY( m_wndMainBar.SetButtonMenu( m_wndMainBar.CommandToIndex( ID_MAINBAR_VIEW ), menu.Detach(), TRUE ) );

	VERIFY( menu.LoadMenu( IDR_MENU_MAINBARTECH ) );
	VERIFY( m_wndMainBar.SetButtonMenu( m_wndMainBar.CommandToIndex( ID_MAINBAR_TECH ), menu.Detach(), TRUE ) );

	VERIFY( menu.LoadMenu( IDR_MENU_MAINBARPERIOD ) );
	VERIFY( m_wndMainBar.SetButtonMenu( m_wndMainBar.CommandToIndex( ID_MAINBAR_PERIOD ), menu.Detach(), TRUE ) );
	return TRUE;
	*/
}

BOOL CMainFrame::CreateViewBar( )
{
	if (!m_wndViewBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),IDW_VIEWBAR)
		|| !m_wndViewBar.LoadToolBar( IDR_VIEWBAR ) )
	{
		TRACE0("Failed to create viewbar\n");
		return FALSE;      // fail to create
	}
	CString	strBarTitle;
	strBarTitle.LoadString( IDS_TITLE_VIEWBAR );
	m_wndViewBar.SetWindowText( strBarTitle );
	return TRUE;

	/* ProfUIS Using Code
	CString	strBarTitle;
	strBarTitle.LoadString( IDS_TITLE_VIEWBAR );
	if( !m_wndViewBar.Create( strBarTitle, this, IDW_VIEWBAR )
		|| !m_wndViewBar.LoadToolBar( IDR_VIEWBAR ) )
	{
		TRACE0("Failed to create viewbar\n");
		return FALSE;      // fail to create
	}
	return TRUE;
	*/
}

BOOL CMainFrame::CreatePeriodBar( )
{
	if (!m_wndPeriodBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),IDW_PERIODBAR)
		|| !m_wndPeriodBar.LoadToolBar( IDR_PERIODBAR ) )
	{
		TRACE0("Failed to create periodbar\n");
		return FALSE;      // fail to create
	}
	CString	strBarTitle;
	strBarTitle.LoadString( IDS_TITLE_PERIODBAR );
	m_wndPeriodBar.SetWindowText( strBarTitle );
	return TRUE;

	/* ProfUIS Using Code
	CString	strBarTitle;
	strBarTitle.LoadString( IDS_TITLE_PERIODBAR );
	if( !m_wndPeriodBar.Create( strBarTitle, this, IDW_PERIODBAR )
		|| !m_wndPeriodBar.LoadToolBar( IDR_PERIODBAR ) )
	{
		TRACE0("Failed to create periodbar\n");
		return FALSE;      // fail to create
	}
	return TRUE;
	*/
}

BOOL CMainFrame::CreateSListBar( )
{
	if( !m_wndSListBar.CreateTextToolBar( this, IDW_SLISTBAR, IDR_SLISTBAR,
		-1, -1, CTextToolBar::labelRight ) )
	{
		TRACE0("Failed to create slistbar\n");
		return FALSE;      // fail to create
	}
//	if( ! m_wndSListBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
//		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0), IDW_SLISTBAR )
//		|| !m_wndSListBar.LoadToolBar( IDR_SLISTBAR ) )
//	{
//		TRACE0("Failed to create slistbar\n");
//		return -1;      // fail to create
//	}
	CString	strTitle;
	strTitle.LoadString( IDS_TITLE_SLISTBAR );
	SetWindowText( strTitle );

	m_wndSListBar.SetButtonDropDown( ID_SLISTBAR_GROUP, m_wndSListBar.CommandToIndex(ID_SLISTBAR_GROUP), IDR_MENU_SLISTBARGROUP);
	m_wndSListBar.SetButtonDropDown( ID_SLISTBAR_DOMAIN, m_wndSListBar.CommandToIndex(ID_SLISTBAR_DOMAIN), IDR_MENU_SLISTBARDOMAIN);
//	m_wndSListBar.SetButtonDropDown( ID_SLISTBAR_CLASS, m_wndSListBar.CommandToIndex(ID_SLISTBAR_CLASS), IDR_MENU_SLISTBARCLASS);


	// The ID of the ComboBox is important for two reasons.  One, so you
	// can receive notifications from the control.  And also for ToolTips.
	// During HitTesting if the ToolBar sees that the mouse is one a child
	// control, the toolbar will lookup the controls ID and search for a
	// string in the string table with the same ID to use for ToolTips
	// and StatusBar info.

/*	// Date ComboBox
	if( !m_wndSListBar.CreateCombo( &m_DateBox, ID_SLISTBAR_DATE, SLB_DATE_WIDTH,
		WS_CHILD | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL | CBS_HASSTRINGS ) )
	{
		TRACE0("Failed to create m_DateBox\n");
		return FALSE;
	}
	m_DateBox.SetItemHeight( -1, 16 );
*/
	return TRUE;

	/* ProfUIS Using Code
	CString	strBarTitle;
	strBarTitle.LoadString( IDS_TITLE_SLISTBAR );
	if( !m_wndSListBar.Create( strBarTitle, this, IDW_SLISTBAR )
		|| !m_wndSListBar.LoadToolBar( IDR_SLISTBAR ) )
	{
		TRACE0("Failed to create slistbar\n");
		return FALSE;      // fail to create
	}
	return TRUE;

	CMenu menu;
	VERIFY( menu.LoadMenu( IDR_MENU_SLISTBARGROUP ) );
	VERIFY( m_wndSListBar.SetButtonMenu( m_wndSListBar.CommandToIndex( ID_SLISTBAR_GROUP ), menu.Detach(), TRUE ) );

	VERIFY( menu.LoadMenu( IDR_MENU_SLISTBARDOMAIN ) );
	VERIFY( m_wndSListBar.SetButtonMenu( m_wndSListBar.CommandToIndex( ID_SLISTBAR_DOMAIN ), menu.Detach(), TRUE ) );

	VERIFY( menu.LoadMenu( IDR_MENU_SLISTBARCLASS ) );
	VERIFY( m_wndSListBar.SetButtonMenu( m_wndSListBar.CommandToIndex( ID_SLISTBAR_CLASS ), menu.Detach(), TRUE ) );
	
	// Date ComboBox
	if (!m_DateBox.Create(WS_CHILD | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL | CBS_HASSTRINGS,
			CRect(-SLB_DATE_WIDTH, -SLB_DATE_HEIGHT, 0, 200), this, ID_SLISTBAR_DATE )
		|| !m_wndSListBar.SetButtonCtrl( m_wndSListBar.CommandToIndex( m_DateBox.GetDlgCtrlID() ), &m_DateBox ) )
	{
		TRACE0("Failed to create m_DateBox\n");
		return FALSE;
	}
	m_DateBox.SetItemHeight( -1, 16 );
	m_DateBox.SetFont( &g_PaintManager->m_FontNormal );
	CExtCmdManager::cmd_t	*	pcmd	=	g_CmdManager->CmdGetPtr( szRegKeyApp, m_DateBox.GetDlgCtrlID() );
	if( pcmd )
		pcmd->m_sMenuText.LoadString( ID_SLISTBAR_DATE );

	return TRUE;
	*/
}

BOOL CMainFrame::CreateSimuBar( )
{
	if (!m_wndSimuBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),IDW_SIMUBAR)
		|| !m_wndSimuBar.LoadToolBar( IDR_SIMUBAR ) )
	{
		TRACE0("Failed to create simubar\n");
		return FALSE;      // fail to create
	}
	CString	strBarTitle;
	strBarTitle.LoadString( IDS_TITLE_SIMUBAR );
	m_wndSimuBar.SetWindowText( strBarTitle );
	return TRUE;

	/* ProfUIS Using Code
	CString	strBarTitle;
	strBarTitle.LoadString( IDS_TITLE_SIMUBAR );
	if( !m_wndSimuBar.Create( strBarTitle, this, IDW_SIMUBAR )
		|| !m_wndSimuBar.LoadToolBar( IDR_SIMUBAR ) )
	{
		TRACE0("Failed to create simubar\n");
		return FALSE;      // fail to create
	}
	return TRUE;
	*/
}


BOOL CMainFrame::CreateWorkspBar( )
{
	/* Old for CCJTabCtrlBar
	// Create the image list to be used with the control bars.
	m_ImageWorkspBox.Create(IDB_WORKSP_BOX, 13, 1, RGB(0,255,0));

	// CG: The following block was inserted by the 'Dialog Bar' component
	m_wndWorkspBar.SetBtnImageList (&m_ImageWorkspBox);
	// m_wndWorkspBar.SetMenuID(IDR_POPUP_WORKSP); Move Menu To View

	// Initialize dialog bar m_wndWorkspBar
	CString	strWorkspTitle;
	strWorkspTitle.LoadString( IDS_TITLE_WORKSPBAR );
	if (!m_wndWorkspBar.Create(this, IDW_WORKSPBAR,
		strWorkspTitle, CSize(235,200)))
	{
		TRACE0("Failed to create dialog bar m_wndWorkspBar\n");
		return TRUE;		// fail to create
	}
	*/

	CString	strWorkspTitle;
	strWorkspTitle.LoadString( IDS_TITLE_WORKSPBAR );
	if (!m_wndWorkspBar.Create( strWorkspTitle, WS_CHILD | WS_VISIBLE, this, IDW_WORKSPBAR))
	{
		TRACE0("Failed to create workspbar\n");
		return -1;     
	}
	m_wndWorkspBar.SetFrameWnd( this );

	// Add the views to the tab control.
	CString	strViewTitle;
	strViewTitle.LoadString( IDS_TITLE_STRATEGYVIEW );
	m_wndWorkspBar.AddView(strViewTitle, RUNTIME_CLASS(CStrategyView));
	strViewTitle.LoadString( IDS_TITLE_GROUPVIEW );
	m_wndWorkspBar.AddView(strViewTitle, RUNTIME_CLASS(CGroupView));
	strViewTitle.LoadString( IDS_TITLE_TECHSVIEW );
	m_wndWorkspBar.AddView(strViewTitle, RUNTIME_CLASS(CTechsView));

	/* Old for CCJTabCtrlBar
	// Define the image list to use with the tab control
	m_ImageWorkspTab.Create (IDB_WORKSP_TAB, 16, 1, RGB(0,255,0));
	m_wndWorkspBar.SetTabImageList(&m_ImageWorkspTab);
	*/
	m_wndWorkspBar.SetTabImageList(IDB_WORKSP_TAB, 16, 1, RGB(0,255,0));

	// allow bar to be resized when floating
	m_wndWorkspBar.SetBarStyle(m_wndWorkspBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	return TRUE;
}

char	szBarSection[]	=	"StockAnaBar";
char	szMainBar[]		=	"MainBar";
char	szSListBar[]	=	"SListBar";
char	szSimuBar[]		=	"SimuBar";
char	szWorkspBar[]	=	"WorkspBar";
char	szViewBar[]		=	"ViewBar";
char	szPeriodBar[]	=	"PeriodBar";

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiMDIFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	/* ProfUIS Using Code
	// g_CmdManager
	g_CmdManager->ProfileSetup( szRegKeyApp, GetSafeHwnd() );
	VERIFY( g_CmdManager->UpdateFromMenu( szRegKeyApp, IDR_MAINFRAME ) );
	*/
//	CExtPopupMenuWnd::g_bMenuWithShadows		=	false;
	CExtPopupMenuWnd::g_bMenuExpanding			=	false;
//	CExtPopupMenuWnd::g_bMenuHighlightRarely	=	false;
//	CExtPopupMenuWnd::g_bMenuShowCoolTips		=	false;
//	CExtPopupMenuWnd::g_bMenuExpandAnimation	=	false;
//	CExtPopupMenuWnd::g_bUseDesktopWorkArea		=	false;
	CExtPopupMenuWnd::g_DefAnimationType		=	CExtPopupMenuWnd::__AT_ROLL;

	// Create ToolBars
	if( !CreateMenuBar( ) || !CreateMainBar( )
		|| !CreateViewBar( ) || !CreatePeriodBar( )
		|| !CreateSListBar( ) || !CreateSimuBar( )
//		|| !CreateStatusBar( ) 
		|| !CreateWorkspBar( ) )
	{
		return -1;      // failed to create
	}

	// Name Search ComboBox
	if( !m_SearchBox.Create(WS_CHILD | CBS_DROPDOWN | CBS_SORT | CBS_AUTOHSCROLL | WS_VSCROLL | CBS_HASSTRINGS,
			CRect(-FRM_SEARCH_WIDTH, -FRM_SEARCH_HEIGHT, 0, 200), this, ID_SLISTBAR_NAME ) )
	{
		TRACE0("Failed to create m_SearchBox\n");
		return FALSE;
	}
	m_SearchBox.SetItemHeight( -1, 16 ); 
	m_SearchBox.SetAutoHide( TRUE );
	
	SendMessage( WM_USER_UPDATEBARMENU, 0, 0 );

	// Enable Docking
	EnableDocking(CBRS_ALIGN_ANY);
	/* ProfUIS Using Code
	CExtControlBar::FrameEnableDocking(this);
	*/

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMainBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndViewBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSListBar.EnableDocking(CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM);
	m_wndPeriodBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSimuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkspBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);

	// Set Bar Position
	DockControlBar(&m_wndMenuBar,AFX_IDW_DOCKBAR_TOP);

	DockControlBar(&m_wndMainBar,AFX_IDW_DOCKBAR_TOP);
	DockControlBar(&m_wndSListBar,AFX_IDW_DOCKBAR_TOP);
	DockControlBarLeftOf(&m_wndSimuBar,&m_wndSListBar);
	
	DockControlBar(&m_wndWorkspBar, AFX_IDW_DOCKBAR_LEFT);
	DockControlBar(&m_wndViewBar, AFX_IDW_DOCKBAR_LEFT);
	DockControlBarLeftOf(&m_wndPeriodBar,&m_wndViewBar);

	// Bar State and Window Placement
	ShowControlBar( &m_wndMainBar, FALSE, FALSE );
	ShowControlBar( &m_wndSListBar, FALSE, FALSE );
	ShowControlBar( &m_wndSimuBar, FALSE, FALSE );
	ShowControlBar( &m_wndWorkspBar, FALSE, FALSE );
	ShowControlBar( &m_wndViewBar, FALSE, FALSE );
	ShowControlBar( &m_wndPeriodBar, FALSE, FALSE );
	
	/* ProfUIS Using Code
	sProfile = _T("BarState");
	// LoadBars();

	// Prof-UIS Bar State and WindowPlacement
	// VERIFY( g_CmdManager->SetBasicCommands( szRegKeyApp, g_statBasicCommands ) );
	CExtControlBar::ProfileBarStateLoad( this, szRegKeyCompany, szRegKeyApp, &m_dataFrameWP );
	// g_CmdManager->SerializeState( szRegKeyApp, szRegKeyCompany, szRegKeyApp, false );
	*/

	// My Own Bar State and Window Placement
	// LoadBarState(_T("BarState"));
	
	InitMDITabbed();

	// Set Timer
	SetTimer( TIMER_TIME, 1000, NULL );
	SetTimer( TIMER_STOCKINDEXREFRESH, 5000, NULL );
	// SetTimer( TIMER_CHECKVALID, 1000, NULL );
	// SetTimer( TIMER_SHOWNETINFO, 1000, NULL );

	// Create QuoteTip and AlarmTip
	CQuoteTipDlg::GetInstance();
	CAlarmTipDlg::GetInstance();

	// Notify Icon
	NOTIFYICONDATA	nid;
	memset( &nid, 0, sizeof(nid) );
	nid.cbSize	= sizeof(nid);
	nid.hWnd	= GetSafeHwnd();
	nid.uID		= 1;
	nid.uFlags	= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_USER_NIFMESSAGE;
	nid.hIcon	= AfxGetApp()->LoadIcon( IDR_MAINFRAME );
	CString strName = AfxGetStockIndexReportTip();
	strncpy( nid.szTip, strName, min(strName.GetLength(),sizeof(nid.szTip)-1) );
	Shell_NotifyIcon( NIM_ADD, &nid );

	// StkReceiver Alarm
	AfxGetStkReceiver().AddRcvAlarmWnd( GetSafeHwnd() );

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CGuiMDIFrame::PreCreateWindow(cs) )
		return FALSE;
	cs.dwExStyle	&=	~WS_EX_CLIENTEDGE;
	return TRUE;
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// window placement persistence
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CFrameWnd::ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	
	CGuiMDIFrame::ActivateFrame(nCmdShow);
}




void CMainFrame::ShowWorkspBar( BOOL bShow )
{
	m_wndWorkspBar.ShowNormal( bShow );
}

void CMainFrame::OnToggleWorkspBar( )
{
	m_wndWorkspBar.ToggleShow( );
}

void CMainFrame::SetStatusMsg( LPCTSTR lpszText )
{

}

void CMainFrame::SetProgress( int nPercent )
{

}

void CMainFrame::DockControlBarLeftOf(CGuiToolBarWnd* Bar,CGuiToolBarWnd* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

CPoint CMainFrame::CalcPopupMenuPoint( CToolBar * pBar, UINT nID, CMenu *pMenu )
{
	ASSERT( pBar && pBar->IsKindOf(RUNTIME_CLASS(CToolBar)) );

	// Button rect
	CRect rectButton;
	pBar->GetToolBarCtrl().GetRect( nID, &rectButton );
	pBar->ClientToScreen( &rectButton );

	// Screen size
	int cyScreen = GetSystemMetrics( SM_CYSCREEN );

	// Menu height
	int nMenuHeight	=	cyScreen/4;
	if( pMenu )
	{
		nMenuHeight	=	5;
		for( UINT i=0; i<pMenu->GetMenuItemCount(); i++ )
		{
			if( ID_SEPARATOR == pMenu->GetMenuItemID(i) )
				nMenuHeight += 9;
			else
				nMenuHeight += 19;
		}
	}

	// Left Width
	int nLeftWidth = rectButton.Height();
	for( int i=0; i<pBar->GetCount(); i++ )
	{
		if( pBar->GetButtonStyle(i) == TBSTYLE_BUTTON )
		{
			CRect rectItem;
			pBar->GetToolBarCtrl().GetRect( i, &rectItem );
			nLeftWidth = rectItem.Width();
			break;
		}
	}

	// Track Point
	CPoint	pt;
	if( cyScreen-rectButton.bottom-2 >= nMenuHeight
		|| rectButton.top-2 < nMenuHeight )
	{
		pt.x = rectButton.left + nLeftWidth;
		pt.y = rectButton.bottom + 1;
	}
	else
	{
		pt.x = rectButton.left + nLeftWidth;
		pt.y = rectButton.top - 1;
	}

	return pt;
}

void CMainFrame::InitMenuPopup(CMenu* pPopupMenu)
{
	if( NULL == pPopupMenu || NULL == pPopupMenu->GetSafeHmenu() )
		return;

	for( UINT nMenu=0; nMenu<pPopupMenu->GetMenuItemCount(); nMenu++ )
	{
		if( ID_SLIST_GROUP1 == pPopupMenu->GetMenuItemID( nMenu ) )
		{
			pPopupMenu->ModifyMenu( nMenu, MF_BYPOSITION | MF_GRAYED, ID_SLIST_GROUP1, (LPCTSTR)NULL );
			for( UINT k=pPopupMenu->GetMenuItemCount()-1; k>nMenu; k-- )
				pPopupMenu->DeleteMenu( k, MF_BYPOSITION );
			AfxMenuInsertGroup( pPopupMenu, nMenu+1, ID_SLIST_GROUP_START, ID_SLIST_GROUP_END, TRUE );
			break;
		}
		else if( ID_SLIST_DOMAIN1 == pPopupMenu->GetMenuItemID( nMenu ) )
		{
			pPopupMenu->DeleteMenu( nMenu, MF_BYPOSITION );
			AfxMenuInsertDomain( pPopupMenu, nMenu, ID_SLIST_DOMAIN_START, ID_SLIST_DOMAIN_END );
			break;
		}
		else if( ID_OPTION_ADDTOGROUP == pPopupMenu->GetMenuItemID( nMenu ) )
		{
			pPopupMenu->ModifyMenu( nMenu, MF_BYPOSITION | MF_GRAYED, ID_OPTION_ADDTOGROUP, (LPCTSTR)NULL );
			for( UINT k=pPopupMenu->GetMenuItemCount()-1; k>nMenu; k-- )
				pPopupMenu->DeleteMenu( k, MF_BYPOSITION );
			CView * pView = AfxGetStaticDoc()->GetActiveView();
			AfxMenuInsertGroup( pPopupMenu, nMenu+1, ID_OPTION_ADDTOGROUP_START, ID_OPTION_ADDTOGROUP_END,
						pView && ( pView->IsKindOf(RUNTIME_CLASS(CSListView))||pView->IsKindOf(RUNTIME_CLASS(CGraphView))||pView->IsKindOf(RUNTIME_CLASS(CBaseView)) ) );
			break;
		}
		else if( ID_VIEW_TECHKLINE == pPopupMenu->GetMenuItemID( nMenu ) )
		{
			CView * pView = AfxGetStaticDoc()->GetActiveView();
			UINT	nFlags	=	( (pView && pView->IsKindOf(RUNTIME_CLASS(CGraphView))) ? MF_BYPOSITION | MF_STRING : MF_BYPOSITION | MF_STRING | MF_GRAYED );
			pPopupMenu->DeleteMenu( nMenu, MF_BYPOSITION );
			for( UINT nID = STT_KLINE_MAX; nID >= STT_KLINE_MIN; nID -- )
			{
				UINT	nFlagsLocal	=	nFlags;
				if( AfxGetProfile().IsGraphTechShow( nID ) )	nFlagsLocal	|=	MF_CHECKED;
				pPopupMenu->InsertMenu( nMenu, nFlagsLocal, ID_VIEW_TECH_START+nID, AfxGetSTTFullName(nID) );
			}
		}
		else if( ID_VIEW_TECHTREND == pPopupMenu->GetMenuItemID( nMenu ) )
		{
			CView * pView = AfxGetStaticDoc()->GetActiveView();
			UINT	nFlags	=	( (pView && pView->IsKindOf(RUNTIME_CLASS(CGraphView))) ? MF_BYPOSITION | MF_STRING : MF_BYPOSITION | MF_STRING | MF_GRAYED );
			pPopupMenu->DeleteMenu( nMenu, MF_BYPOSITION );
			for( UINT nID = STT_TREND_MAX; nID >= STT_TREND_MIN; nID -- )
			{
				UINT	nFlagsLocal	=	nFlags;
				if( AfxGetProfile().IsGraphTechShow( nID ) )	nFlagsLocal	|=	MF_CHECKED;
				pPopupMenu->InsertMenu( nMenu, nFlagsLocal, ID_VIEW_TECH_START+nID, AfxGetSTTFullName(nID) );
			}
		}
		else if( ID_VIEW_TECHENERGY == pPopupMenu->GetMenuItemID( nMenu ) )
		{
			pPopupMenu->DeleteMenu( nMenu, MF_BYPOSITION );
			CView * pView = AfxGetStaticDoc()->GetActiveView();
			UINT	nFlags	=	( (pView && pView->IsKindOf(RUNTIME_CLASS(CGraphView))) ? MF_BYPOSITION | MF_STRING : MF_BYPOSITION | MF_STRING | MF_GRAYED );
			for( UINT nID = STT_ENERGY_MAX; nID >= STT_ENERGY_MIN; nID -- )
			{
				UINT	nFlagsLocal	=	nFlags;
				if( AfxGetProfile().IsGraphTechShow( nID ) )	nFlagsLocal	|=	MF_CHECKED;
				pPopupMenu->InsertMenu( nMenu, nFlagsLocal, ID_VIEW_TECH_START+nID, AfxGetSTTFullName(nID) );
			}
		}
		else if( ID_VIEW_TECHSWING == pPopupMenu->GetMenuItemID( nMenu ) )
		{
			CView * pView = AfxGetStaticDoc()->GetActiveView();
			UINT	nFlags	=	( (pView && pView->IsKindOf(RUNTIME_CLASS(CGraphView))) ? MF_BYPOSITION | MF_STRING : MF_BYPOSITION | MF_STRING | MF_GRAYED );
			pPopupMenu->DeleteMenu( nMenu, MF_BYPOSITION );
			for( UINT nID = STT_SWING_MAX; nID >= STT_SWING_MIN; nID -- )
			{
				UINT	nFlagsLocal	=	nFlags;
				if( AfxGetProfile().IsGraphTechShow( nID ) )	nFlagsLocal	|=	MF_CHECKED;
				pPopupMenu->InsertMenu( nMenu, nFlagsLocal, ID_VIEW_TECH_START+nID, AfxGetSTTFullName(nID) );
			}
		}
		else if( ID_VIEW_TECHOTHER == pPopupMenu->GetMenuItemID( nMenu ) )
		{
			CView * pView = AfxGetStaticDoc()->GetActiveView();
			UINT	nFlags	=	( (pView && pView->IsKindOf(RUNTIME_CLASS(CGraphView))) ? MF_BYPOSITION | MF_STRING : MF_BYPOSITION | MF_STRING | MF_GRAYED );
			pPopupMenu->DeleteMenu( nMenu, MF_BYPOSITION );
			for( UINT nID = STT_OTHER_MAX; nID >= STT_OTHER_MIN; nID -- )
			{
				UINT	nFlagsLocal	=	nFlags;
				if( AfxGetProfile().IsGraphTechShow( nID ) )	nFlagsLocal	|=	MF_CHECKED;
				pPopupMenu->InsertMenu( nMenu, nFlagsLocal, ID_VIEW_TECH_START+nID, AfxGetSTTFullName(nID) );
			}
		}
		else if( ID_VIEW_TECHCLK == pPopupMenu->GetMenuItemID( nMenu ) )
		{
			CView * pView = AfxGetStaticDoc()->GetActiveView();
			UINT	nFlags	=	( (pView && pView->IsKindOf(RUNTIME_CLASS(CGraphView))) ? MF_BYPOSITION | MF_STRING : MF_BYPOSITION | MF_STRING | MF_GRAYED );
			pPopupMenu->DeleteMenu( nMenu, MF_BYPOSITION );
			for( UINT nID = STT_CLK_MAX; nID >= STT_CLK_MIN; nID -- )
			{
				UINT	nFlagsLocal	=	nFlags;
				if( AfxGetProfile().IsGraphTechShow( nID ) )	nFlagsLocal	|=	MF_CHECKED;
				pPopupMenu->InsertMenu( nMenu, nFlagsLocal, ID_VIEW_TECH_START+nID, AfxGetSTTFullName(nID) );
			}
		}
		else if( ID_VIEW_TECHUSER == pPopupMenu->GetMenuItemID( nMenu ) )
		{
			CView * pView = AfxGetStaticDoc()->GetActiveView();
			UINT	nFlags	=	( (pView && pView->IsKindOf(RUNTIME_CLASS(CGraphView))) ? MF_BYPOSITION | MF_STRING : MF_BYPOSITION | MF_STRING | MF_GRAYED );
			pPopupMenu->DeleteMenu( nMenu, MF_BYPOSITION );
			
			UINT nTechUserCount	=	CTechUser::GetTechUserCount();
			UINT nIDEnd = STT_USER_MIN+nTechUserCount-1;
			if( nIDEnd > STT_USER_MIN+14 )
				nIDEnd	=	STT_USER_MIN+14;
			for( UINT nID = nIDEnd; nID >= STT_USER_MIN; nID -- )
			{
				UINT	nFlagsLocal	=	nFlags;
				if( AfxGetProfile().IsGraphTechShow( nID ) )	nFlagsLocal	|=	MF_CHECKED;
				pPopupMenu->InsertMenu( nMenu, nFlagsLocal, ID_VIEW_TECH_START+nID-STT_USER_MIN+1+STT_MAX, AfxGetSTTFullName(nID) );
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CGuiMDIFrame::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CGuiMDIFrame::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if( TIMER_TIME == nIDEvent )
	{

	}
	else if( TIMER_STOCKINDEXREFRESH == nIDEvent )
	{
		CString strSHTipFmt, strSZTipFmt;
		strSHTipFmt.LoadString( IDS_MAINFRAME_SHINDEXTIP );
		strSZTipFmt.LoadString( IDS_MAINFRAME_SZINDEXTIP );

		double dDevided = 100000000;
#ifdef CLKLAN_ENGLISH_US
		dDevided = 1000000000;
#endif

		CString	string;
		double	dDiff = 0;
		CStockInfo	info;
		COLORREF	clrRise		=	AfxGetProfile().GetColor(CColorClass::clrRise);
		COLORREF	clrFall		=	AfxGetProfile().GetColor(CColorClass::clrFall);

		if( AfxGetStockContainer().GetStockInfo( STKLIB_CODE_SZZS, &info ) )
		{
			info.GetDiff(&dDiff,info.m_datetech,1);
			string.Format( strSHTipFmt, info.m_fClose, dDiff, info.m_fAmount/dDevided );
		}
		if( AfxGetStockContainer().GetStockInfo( STKLIB_CODE_SZNCZ, &info ) )
		{
			info.GetDiff(&dDiff,info.m_datetech,1);
			string.Format( strSZTipFmt, info.m_fClose, dDiff, info.m_fAmount/dDevided );
		}

		PostMessage( WM_APP_STKRECEIVER_ALARM, STKRCV_ALARM_REFRESH, 0 );
	}
	else if( TIMER_CHECKVALID == nIDEvent )
	{
		static	unsigned	long	count	=	0;
		static	unsigned	long	next	=	0;
		count	++;
		if( count == 2 )
		{
			PostMessage( WM_USER_CHECKSN );
			srand( time(NULL) );
			next	=	300 + rand() % 300;
		}
		if( count >= next )
		{
			PostMessage( WM_USER_CHECKSN );
			next	=	300 + rand() % 300;
			count = 2;
		}
	}
	else if( TIMER_SHOWNETINFO == nIDEvent )
	{
		static	unsigned	long	countShow	=	0;
		countShow	++;
		if( countShow == 10 )
		{
			KillTimer( TIMER_SHOWNETINFO );
			CNetInfoDlg	dlg;
			dlg.DoModal( );
			if( AfxGetProfile().HasNewVersion()
				&& IDYES == AfxMessageBox( IDS_DOWNLOAD_UPGRADENOW, MB_YESNO ) )
			{
				CUpgradeDlg	* dlg = new CUpgradeDlg;
				dlg->Create( CUpgradeDlg::IDD, this );
				dlg->SetAutoDelete( TRUE );
				dlg->SetAutoRun( TRUE );
				dlg->CenterWindow(this);
				dlg->SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOSIZE );
			}
		}
	}

	CGuiMDIFrame::OnTimer(nIDEvent);
}

// 判断软件是否合法函数
LRESULT CMainFrame::CreateValid(WPARAM wParam, LPARAM lParam)
{
	if( wParam < 100 )
	{
		if( AfxGetSView().IsEva() && AfxGetSView().GetLeft() < 30 )
		{
			CRegisterDlg	dlg;
			dlg.DoModal();
			PostMessage( WM_USER_CHECKSN, 250 );
		}
	}
	else if( wParam >= 100 && wParam < 200 )
	{
		PostMessage( lParam );
	}
	else
	{
		AfxAssertQuit();
	}
	return wParam;
}

LRESULT CMainFrame::OnUpdateBarMenu(WPARAM wParam, LPARAM lParam)
{
	return 0;

	/* ProfUIS Using Code
	// Menu Bar Group
	UINT nMenu	=	0;
	for( nMenu=0; nMenu<m_wndMenuBar.GetMenu().GetMenuItemCount(); nMenu ++ )
	{
		CMenu	*	pMenu	=	m_wndMenuBar.GetMenu().GetSubMenu(nMenu);
		if( !pMenu )
			continue;
		for( UINT nSubMenu=0; nSubMenu<pMenu->GetMenuItemCount(); nSubMenu ++ )
		{
			CMenu	*	pPopupMenu	=	pMenu->GetSubMenu(nSubMenu);
			InitMenuPopup( pPopupMenu );
		}
	}

	CMenu	menu;
	CMenu	*	pMenu		=	NULL;
	CMenu	*	pPopupMenu	=	NULL;

	// Main Bar
	menu.Attach( m_wndMainBar.GetButtonMenu( m_wndMainBar.CommandToIndex( ID_MAINBAR_SETTING ) ) );
	pMenu	=	menu.GetSubMenu(0);
	for( nMenu=0; pMenu && nMenu<pMenu->GetMenuItemCount(); nMenu++ )
	{
		pPopupMenu	=	pMenu->GetSubMenu(nMenu);
		InitMenuPopup( pPopupMenu );
	}
	menu.Detach( );

	menu.Attach( m_wndMainBar.GetButtonMenu( m_wndMainBar.CommandToIndex( ID_MAINBAR_TECH ) ) );
	pMenu	=	menu.GetSubMenu(0);
	for( nMenu=0; pMenu && nMenu<pMenu->GetMenuItemCount(); nMenu++ )
	{
		pPopupMenu	=	pMenu->GetSubMenu(nMenu);
		InitMenuPopup( pPopupMenu );
	}
	menu.Detach( );

	// SList Bar
	menu.Attach( m_wndSListBar.GetButtonMenu( m_wndSListBar.CommandToIndex( ID_SLISTBAR_GROUP ) ) );
	pPopupMenu	=	menu.GetSubMenu(0);
	InitMenuPopup( pPopupMenu );
	menu.Detach( );

	menu.Attach( m_wndSListBar.GetButtonMenu( m_wndSListBar.CommandToIndex( ID_SLISTBAR_DOMAIN ) ) );
	pPopupMenu	=	menu.GetSubMenu(0);
	InitMenuPopup( pPopupMenu );
	menu.Detach( );

	return 0;
	*/
}

LRESULT CMainFrame::OnNifMessage(WPARAM wParam, LPARAM lParam)
{
	static BOOL g_bQuoteTipVisited = FALSE;

	if( WM_LBUTTONDOWN == lParam )
	{
		if( !IsIconic() )
		{
			ShowWindow( SW_MINIMIZE );
			if( AfxGetProfile().GetNotifyIcon() )
				ShowWindow( SW_HIDE );
		}
		else
		{
			BOOL bQuoteTipVisible = CQuoteTipDlg::GetInstance().IsWindowVisible();
			CQuoteTipDlg::GetInstance().Hide();
			SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );
			ShowWindow( SW_RESTORE );
			UpdateWindow();

			if( !g_bQuoteTipVisited || bQuoteTipVisible )
			{
				DockControlBar(&m_wndViewBar, AFX_IDW_DOCKBAR_LEFT);
				DockControlBarLeftOf(&m_wndPeriodBar,&m_wndViewBar);
			}
			g_bQuoteTipVisited = FALSE;
			RecalcLayout();
		}
	}
	else if( WM_RBUTTONDOWN == lParam )
	{
		// 临时刷新一下行情
		CStockContainer & container = AfxGetSListStockContainer();
		AfxGetStkReceiver().RequestStockData( CStock::dataReport, container.GetData(), container.GetSize(), 0, 0 );

		// Show Quote
		CQuoteTipDlg::GetInstance().ShowGradual();
		CQuoteTipDlg::GetInstance().SetAutoHide( 5, TRUE );
		g_bQuoteTipVisited = TRUE;
	}
	return 0;
}

LRESULT CMainFrame::OnInitDates(WPARAM wParam, LPARAM lParam)
{
	if( ::IsWindow( m_DateBox.GetSafeHwnd() ) )
	{
		m_DateBox.InitDates( );
		m_DateBox.SetCurrentWindowText( );
	}
	return 0;
}

LRESULT CMainFrame::OnUpdateSlistView(WPARAM wParam, LPARAM lParam)
{
	AfxGetSListStockContainer().ReRetrieveFromStatic( AfxGetActiveStrategy() );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SLISTVIEW, NULL );
	return 0;
}

LRESULT CMainFrame::OnStkReceiverAlarm(WPARAM wParam, LPARAM lParam)
{
	if( STKRCV_ALARM_WARNING == wParam )
	{
		CAlarmTipDlg::GetInstance().ShowGradual();
		CAlarmTipDlg::GetInstance().SetAutoHide( 5, TRUE );
	}

	UINT nIDIcon = IDR_MAINFRAME;

	if( AfxGetAlarmContainer().HasWarning() )
	{
		nIDIcon = IDI_ALARM_WARNING;
	}
	else
	{
		CStockInfo info;
		double dDiff;
		if( AfxGetStockContainer().GetStockInfo( STKLIB_CODE_MAIN, &info )
			&& info.GetDiff(&dDiff,info.m_datetech,1) )
		{
			if( dDiff > 0 )
				nIDIcon = IDI_ALARM_INDEXRED;
			if( dDiff < 0 )
				nIDIcon = IDI_ALARM_INDEXGREEN;
		}
	}

	// Notify Icon
	NOTIFYICONDATA	nid;
	memset( &nid, 0, sizeof(nid) );
	nid.cbSize	= sizeof(nid);
	nid.hWnd	= GetSafeHwnd();
	nid.uID		= 1;
	nid.uFlags	= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_USER_NIFMESSAGE;
	nid.hIcon	= AfxGetApp()->LoadIcon( nIDIcon );
	CString strName = AfxGetStockIndexReportTip();
	strncpy( nid.szTip, strName, min(strName.GetLength(),sizeof(nid.szTip)-1) );
	Shell_NotifyIcon( NIM_MODIFY, &nid );

	return 0;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	/* ProfUIS Using Code
	if( m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	*/

	if( WM_KEYDOWN == pMsg->message )
	{
		int	nVirtKey = (int) pMsg->wParam;
		if( VK_ESCAPE == nVirtKey && m_fullScreen.bFullScreen )
			OnViewFullscreen( );

		if( m_SearchBox.IsWantChar(pMsg->wParam) )
		{
			CView * pView = AfxGetStaticDoc()->GetActiveView();
			if( !pView ||
				( pView->IsKindOf( RUNTIME_CLASS(CWizardView) )
					|| pView->IsKindOf( RUNTIME_CLASS(CGraphView) )
					|| pView->IsKindOf( RUNTIME_CLASS(CRealTimeView) )
					|| pView->IsKindOf( RUNTIME_CLASS(CMultiSortView) )
					|| pView->IsKindOf( RUNTIME_CLASS(CSListView) )
					|| pView->IsKindOf( RUNTIME_CLASS(CBaseView) )
					|| pView->IsKindOf( RUNTIME_CLASS(CSelectorView) ) ) )
			{
				CRect	rect;
				GetClientRect( &rect );
				m_SearchBox.SetWindowPos(NULL, rect.right-FRM_SEARCH_WIDTH-18, rect.bottom-FRM_SEARCH_HEIGHT-22,
										0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
				m_SearchBox.OnChangeStatus( pMsg->wParam, pMsg->lParam, FALSE );
			}
		}
		else if( ::IsWindow(m_wndWorkspBar.GetSafeHwnd())
			 && !m_SearchBox.GetDroppedState() && !m_wndWorkspBar.IsWindowVisible() )
		{
			CView * pView = AfxGetStaticDoc()->GetActiveView();
			if( pView && 
				( pView->IsKindOf( RUNTIME_CLASS(CGraphView) )
				|| pView->IsKindOf( RUNTIME_CLASS(CRealTimeView) )
				|| pView->IsKindOf( RUNTIME_CLASS(CMultiSortView) )
				|| pView->IsKindOf( RUNTIME_CLASS(CSListView) ) ) )
				pView->SetFocus();
		}
	}

	return CGuiMDIFrame::PreTranslateMessage(pMsg);
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
//	CGuiMDIFrame::OnUpdateFrameTitle( bAddToTitle );
	if(::IsWindow(m_MdiTabbed.GetSafeHwnd()))
		m_MdiTabbed.UpdateWindows();

	if (bAddToTitle)
	{
		CString	strMainFrame;
		VERIFY(strMainFrame.LoadString( IDR_MAINFRAME ));

		CString	strTitle;
		if( AfxExtractSubString( strTitle,strMainFrame,
			CDocTemplate::windowTitle) && !strTitle.IsEmpty())
		{
			strTitle	+=	"(";
			strTitle	+=	AfxGetVersionString( );
			strTitle	+=	")";
			// reset title
			AfxSetWindowText(m_hWnd, strTitle);
		}
	}
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CGuiMDIFrame::OnSize(nType, cx, cy);
	
	if( SIZE_MINIMIZED == nType )
	{
		if( AfxGetProfile().GetNotifyIcon() )
			ShowWindow( SW_HIDE );
	}
}

void CMainFrame::OnClose() 
{
	if( m_fullScreen.bFullScreen )
		OnViewFullscreen( );

	BOOL	bCanClose;
	
	bCanClose	=	TRUE;
	CSimuView	*	pSimuView = AfxGetSimuView();
	if( pSimuView && 0 == pSimuView->SendMessage( WM_USER_CANCLOSEVIEW, NULL, (LPARAM)(&bCanClose) ) && !bCanClose )
		return;

	bCanClose	=	TRUE;
	CSelectorView	*	pSelectorView = AfxGetSelectorView();
	if( pSelectorView && 0 == pSelectorView->SendMessage( WM_USER_CANCLOSEVIEW, NULL, (LPARAM)(&bCanClose) ) && !bCanClose )
		return;

	if( IsWindowVisible() )
	{
		// My Own Bar State and Window Placement
		// SaveBarState(_T("BarState"));
		AfxGetApp()->WriteProfileInt( szBarSection, szMainBar, m_wndMainBar.IsWindowVisible() );
		AfxGetApp()->WriteProfileInt( szBarSection, szSListBar, m_wndSListBar.IsWindowVisible() );
		AfxGetApp()->WriteProfileInt( szBarSection, szSimuBar, m_wndSimuBar.IsWindowVisible() );
		AfxGetApp()->WriteProfileInt( szBarSection, szWorkspBar, m_wndWorkspBar.IsWindowVisible() );
		AfxGetApp()->WriteProfileInt( szBarSection, szViewBar, m_wndViewBar.IsWindowVisible() );
		AfxGetApp()->WriteProfileInt( szBarSection, szPeriodBar, m_wndPeriodBar.IsWindowVisible() );
		
		WINDOWPLACEMENT wp;
		wp.length = sizeof wp;
		if (GetWindowPlacement(&wp))
		{
			wp.flags = 0;
			if (IsZoomed())
				wp.flags |= WPF_RESTORETOMAXIMIZED;
			// and write it to the .INI file
			AfxGetProfile().SetWindowPlacement(&wp);
		}
	}

	// delete notify icon
	NOTIFYICONDATA	nid;
	memset( &nid, 0, sizeof(nid) );
	nid.cbSize	= sizeof(nid);
	nid.hWnd	= GetSafeHwnd();
	nid.uID		= 1;
	nid.uFlags	= 0;
	Shell_NotifyIcon( NIM_DELETE, &nid );
	
	CGuiMDIFrame::OnClose();
}

BOOL CMainFrame::DestroyWindow() 
{
	/* ProfUIS Using Code
	// Prof-UIS Bar State and WindowPlacement
	VERIFY( CExtControlBar::ProfileBarStateSave( this, szRegKeyCompany, szRegKeyApp ) );
	// VERIFY( g_CmdManager->SerializeState( szRegKeyApp, szRegKeyCompany, szRegKeyApp, true ) );
	// g_CmdManager->ProfileWndRemove( GetSafeHwnd() );
	*/
	
	return CGuiMDIFrame::DestroyWindow();
}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	if( !bSysMenu && pPopupMenu )
	{
		InitMenuPopup( pPopupMenu );
	}

	CGuiMDIFrame::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

////////////////////////////////////////////////////////////////////////////////
// ToolBar
// 工具条下拉菜单显示
void CMainFrame::OnMainBarDropDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNMHDR;

	BOOL	bLoad	=	FALSE;
	CMenu menu;
	if( ID_MAINBAR_SETTING == pNMToolBar->iItem )
		bLoad	=	menu.LoadMenu( IDR_MENU_MAINBARSETTING );
	else if( ID_MAINBAR_VIEW == pNMToolBar->iItem)
		bLoad	=	menu.LoadMenu( IDR_MENU_MAINBARVIEW );
	else if( ID_MAINBAR_TECH == pNMToolBar->iItem )
		bLoad	=	menu.LoadMenu( IDR_MENU_MAINBARTECH );
	else if( ID_MAINBAR_PERIOD == pNMToolBar->iItem )
		bLoad	=	menu.LoadMenu( IDR_MENU_MAINBARPERIOD );
	else
		ASSERT( FALSE );

	if( bLoad )
	{
		CMenu* pPopupMenu = menu.GetSubMenu(0);
		if( pPopupMenu )
		{
			CPoint pt = CalcPopupMenuPoint( &m_wndMainBar, pNMToolBar->iItem, pPopupMenu );
			pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this );
		}
	}

	*pResult = TBDDRET_DEFAULT;
}

void CMainFrame::OnSlistBarDropDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNMHDR;

	BOOL	bLoad	=	FALSE;
	CMenu menu;
	if( ID_SLISTBAR_GROUP == pNMToolBar->iItem )
		bLoad	=	menu.LoadMenu( IDR_MENU_SLISTBARGROUP );
	else if( ID_SLISTBAR_DOMAIN == pNMToolBar->iItem)
		bLoad	=	menu.LoadMenu( IDR_MENU_SLISTBARDOMAIN );
	else if( ID_SLISTBAR_CLASS == pNMToolBar->iItem )
		bLoad	=	menu.LoadMenu( IDR_MENU_SLISTBARCLASS );
	else
		ASSERT( FALSE );

	if( bLoad )
	{
		CMenu* pPopupMenu = menu.GetSubMenu(0);
		if( pPopupMenu )
		{
			CPoint pt = CalcPopupMenuPoint( &m_wndSListBar, pNMToolBar->iItem, pPopupMenu );
			pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this );
		}
	}

	*pResult = TBDDRET_DEFAULT;
}

#define	ON_DROPDOWN_MENU( wndBar, nItem )					\
	NMTOOLBAR nmTB;											\
	memset( &nmTB, 0, sizeof(nmTB) );						\
	nmTB.iItem = nItem;										\
	nmTB.hdr.hwndFrom	=	wndBar.GetSafeHwnd();			\
	nmTB.hdr.idFrom		=	wndBar.GetDlgCtrlID();			\
	nmTB.hdr.code		=	TBN_DROPDOWN;					\
	SendMessage( WM_NOTIFY, IDW_MAINBAR, (LPARAM)&nmTB );

void CMainFrame::OnMainbarSetting()	{	ON_DROPDOWN_MENU(m_wndMainBar,ID_MAINBAR_SETTING);	}
void CMainFrame::OnMainbarView()	{	ON_DROPDOWN_MENU(m_wndMainBar,ID_MAINBAR_VIEW);	}
void CMainFrame::OnMainbarTech()	{	ON_DROPDOWN_MENU(m_wndMainBar,ID_MAINBAR_TECH);	}
void CMainFrame::OnMainbarPeriod()	{	ON_DROPDOWN_MENU(m_wndMainBar,ID_MAINBAR_PERIOD);	}

void CMainFrame::OnSlistbarGroup()	{	ON_DROPDOWN_MENU(m_wndSListBar,ID_SLISTBAR_GROUP);	}
void CMainFrame::OnSlistbarDomain()	{	ON_DROPDOWN_MENU(m_wndSListBar,ID_SLISTBAR_DOMAIN);	}
void CMainFrame::OnSlistbarClass()	{	ON_DROPDOWN_MENU(m_wndSListBar,ID_SLISTBAR_CLASS);	}

////////////////////////////////////////////////////////////////////////////////
// “系统”菜单
// 实时行情接收
void CMainFrame::OnSysConnectserver()
{
	// AfxGetStkReceiver().NetEngineBeginWorking( );
	extern BOOL CALLBACK LoadProgram( HWND hWnd, int nMinProgress, int nMaxProgress );

	CStartupDlg	startup;
	CBitmap	bmp;
	bmp.LoadBitmap(IDB_DLGLEFTLOGO);
	startup.SetBitmap( (HBITMAP)bmp.GetSafeHandle() );
	startup.SetLoadProgramFunc( LoadProgram );
	startup.SetDisableOffline( TRUE );
	startup.DoModal( );
}

void CMainFrame::OnUpdateSysConnectserver(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( !AfxGetStkReceiver().NetEngineIsWorking() );
}

void CMainFrame::OnSysDisconnectserver()
{
	AfxGetStkReceiver().NetEngineEndWorking();
}

void CMainFrame::OnUpdateSysDisconnectserver(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( AfxGetStkReceiver().NetEngineIsWorking() );
}

void CMainFrame::OnSysStarttongshi()
{
	AfxGetStkReceiver().EngineBeginWorking( TRUE );
	AfxGetSListStockContainer().ReRetrieveFromStatic( AfxGetActiveStrategy() );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SLISTVIEW, NULL );
}

void CMainFrame::OnUpdateSysStarttongshi(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( AfxGetStkReceiver().EngineIsWorking() );
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnSysSetuptongshi()
{
	AfxGetStkReceiver().EngineSetup();
	AfxGetSListStockContainer().ReRetrieveFromStatic( AfxGetActiveStrategy() );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SLISTVIEW, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );
}

void CMainFrame::OnUpdateSysSetuptongshi(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( AfxGetStkReceiver().EngineIsWorking() );
}

void CMainFrame::OnSysStoptongshi()
{
	AfxGetStkReceiver().EngineEndWorking();
}

void CMainFrame::OnUpdateSysStoptongshi(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( AfxGetStkReceiver().EngineIsWorking() );
}

void CMainFrame::OnSysAutoupgrade() 
{
	CUpgradeDlg	* dlg = new CUpgradeDlg;
	dlg->Create( CUpgradeDlg::IDD, this );
	dlg->SetAutoDelete( TRUE );
	dlg->CenterWindow(this);
	dlg->SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOSIZE );
}

void CMainFrame::OnSysAutosave() 
{
	AfxGetProfile().SetAutoSaveKDataMin( !AfxGetProfile().GetAutoSaveKDataMin() );
}

void CMainFrame::OnUpdateSysAutosave(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( AfxGetProfile().GetAutoSaveKDataMin() );
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnSysNotifyicon() 
{
	AfxGetProfile().SetNotifyIcon( !AfxGetProfile().GetNotifyIcon() );
}

void CMainFrame::OnUpdateSysNotifyicon(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( AfxGetProfile().GetNotifyIcon() );
	pCmdUI->Enable( TRUE );
}

////////////////////////////////////////////////////////////////////////////////
// “查看”菜单
// 标准工具条文字标签显示
void CMainFrame::OnViewMainbartext()
{
	m_wndMainBar.ToggleTextLabel( CTextToolBar::labelBottom );
	m_wndSListBar.ToggleTextLabel( CTextToolBar::labelRight );
	RecalcLayout();
}

void CMainFrame::OnUpdateViewMainbartext(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( m_wndMainBar.IsTextLabelVisible() );
}

void CMainFrame::OnViewFullscreen() 
{
	int nHeight	=	0;	//GetSystemMetrics( SM_CYMENU );
	if( m_fullScreen.bFullScreen )
	{
		m_fullScreen.bFullScreen	=	FALSE;

		if( m_fullScreen.bMaximized )
		{
			MoveWindow(	-GetSystemMetrics( SM_CXFRAME ),
						-GetSystemMetrics( SM_CYFRAME )-nHeight,
						GetSystemMetrics( SM_CXSCREEN ) + 2*GetSystemMetrics( SM_CXFRAME ),
						GetSystemMetrics( SM_CYSCREEN ) + 2*GetSystemMetrics( SM_CYFRAME ) + nHeight );
		}
		else
			SendMessage( WM_SYSCOMMAND, SC_RESTORE );

		if( m_fullScreen.bMenuBar && !m_wndMenuBar.IsWindowVisible() )
			OnBarCheck( IDW_MENUBAR );
		if( m_fullScreen.bMainBar && !m_wndMainBar.IsWindowVisible() )
			OnBarCheck( IDW_MAINBAR );
		if( m_fullScreen.bViewBar && !m_wndViewBar.IsWindowVisible() )
			OnBarCheck( IDW_VIEWBAR );
		if( m_fullScreen.bPeriodBar && !m_wndPeriodBar.IsWindowVisible() )
			OnBarCheck( IDW_PERIODBAR );
		if( m_fullScreen.bSListBar && !m_wndSListBar.IsWindowVisible() )
			OnBarCheck( IDW_SLISTBAR );
		if( m_fullScreen.bSimuBar && !m_wndSimuBar.IsWindowVisible() )
			OnBarCheck( IDW_SIMUBAR );
		if( m_fullScreen.bWorkspBar && !m_wndWorkspBar.IsWindowVisible() )
			OnToggleWorkspBar( );

		memset( &m_fullScreen, 0, sizeof(m_fullScreen) );
	}
	else
	{
		m_fullScreen.bFullScreen	=	TRUE;
		m_fullScreen.bMaximized	=	IsZoomed();

		if( m_wndMenuBar.IsWindowVisible() )
		{
			m_fullScreen.bMenuBar = 1;
			OnBarCheck( IDW_MENUBAR );
		}
//		if( m_wndMainBar.IsWindowVisible() )
//		{
//			m_fullScreen.bMainBar = 1;
//			OnBarCheck( IDW_MAINBAR );
//		}
//		if( m_wndViewBar.IsWindowVisible() )
//		{
//			m_fullScreen.bViewBar = 1;
//			OnBarCheck( IDW_VIEWBAR );
//		}
//		if( m_wndPeriodBar.IsWindowVisible() )
//		{
//			m_fullScreen.bPeriodBar = 1;
//			OnBarCheck( IDW_PERIODBAR );
//		}
//		if( m_wndSListBar.IsWindowVisible() )
//		{
//			m_fullScreen.bSListBar = 1;
//			OnBarCheck( IDW_SLISTBAR );
//		}
//		if( m_wndSimuBar.IsWindowVisible() )
//		{
//			m_fullScreen.bSimuBar = 1;
//			OnBarCheck( IDW_SIMUBAR );
//		}
		if( m_wndWorkspBar.IsWindowVisible() )
		{
			m_fullScreen.bWorkspBar = 1;
			OnToggleWorkspBar( );
		}

		if( !m_fullScreen.bMaximized )
			SendMessage( WM_SYSCOMMAND, SC_MAXIMIZE );
		else
			MoveWindow(	-GetSystemMetrics( SM_CXFRAME ),
						-GetSystemMetrics( SM_CYFRAME )	- GetSystemMetrics( SM_CYCAPTION ) - nHeight,
						GetSystemMetrics( SM_CXSCREEN ) + 2*GetSystemMetrics( SM_CXFRAME ),
						GetSystemMetrics( SM_CYSCREEN ) + 2*GetSystemMetrics( SM_CYFRAME ) + GetSystemMetrics( SM_CYCAPTION ) + nHeight );
	}
}

void CMainFrame::OnUpdateViewFullscreen(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_fullScreen.bFullScreen );
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CGuiMDIFrame::OnGetMinMaxInfo(lpMMI);

	if( m_fullScreen.bFullScreen )
	{
		int nHeight	=	GetSystemMetrics( SM_CYMENU );
		lpMMI->ptMaxPosition.y	=	-GetSystemMetrics( SM_CYFRAME ) - GetSystemMetrics( SM_CYCAPTION ) - nHeight;
		lpMMI->ptMaxTrackSize.y	=	lpMMI->ptMaxSize.y
								=	GetSystemMetrics( SM_CYSCREEN )
									+ 2*GetSystemMetrics( SM_CYFRAME )
									+ GetSystemMetrics( SM_CYCAPTION )
									+ nHeight;
	}
}

// 显示功能视图
void CMainFrame::OnViewWizard() 
{
	AfxSwitchToStaticView( RUNTIME_CLASS(CWizardView) );
}

void CMainFrame::OnUpdateViewWizard(CCmdUI* pCmdUI) 
{
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	pCmdUI->SetCheck( pView && pView->IsKindOf(RUNTIME_CLASS(CWizardView)) );
}

void CMainFrame::OnViewSimu() 
{
	AfxSwitchToStaticView( RUNTIME_CLASS(CSimuView) );
}

void CMainFrame::OnUpdateViewSimu(CCmdUI* pCmdUI) 
{
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	pCmdUI->SetCheck( pView && pView->IsKindOf(RUNTIME_CLASS(CSimuView)) );
}

void CMainFrame::OnViewSlist() 
{
	AfxSwitchToStaticView( RUNTIME_CLASS(CSListView) );
}

void CMainFrame::OnUpdateViewSlist(CCmdUI* pCmdUI) 
{
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	pCmdUI->SetCheck( pView && pView->IsKindOf(RUNTIME_CLASS(CSListView)) );
}

void CMainFrame::OnViewRealtime() 
{
	AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );
}

void CMainFrame::OnUpdateViewRealtime(CCmdUI* pCmdUI) 
{
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	pCmdUI->SetCheck( pView && pView->IsKindOf(RUNTIME_CLASS(CRealTimeView)) );
}

void CMainFrame::OnViewMultisort() 
{
	AfxSwitchToStaticView( RUNTIME_CLASS(CMultiSortView) );
}

void CMainFrame::OnUpdateViewMultisort(CCmdUI* pCmdUI) 
{
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	pCmdUI->SetCheck( pView && pView->IsKindOf(RUNTIME_CLASS(CMultiSortView)) );
}

void CMainFrame::OnViewTechgraph() 
{
	AfxSwitchToStaticView( RUNTIME_CLASS(CGraphView) );
}

void CMainFrame::OnUpdateViewTechgraph(CCmdUI* pCmdUI) 
{
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	pCmdUI->SetCheck( pView && pView->IsKindOf(RUNTIME_CLASS(CGraphView)) );
}

void CMainFrame::OnViewBase() 
{
	AfxSwitchToStaticView( RUNTIME_CLASS(CBaseView) );
}

void CMainFrame::OnUpdateViewBase(CCmdUI* pCmdUI) 
{
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	pCmdUI->SetCheck( pView && pView->IsKindOf(RUNTIME_CLASS(CBaseView)) );
}

void CMainFrame::OnViewInfo() 
{
	AfxSwitchToStaticView( RUNTIME_CLASS(CInfoView) );
}

void CMainFrame::OnUpdateViewInfo(CCmdUI* pCmdUI) 
{
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	pCmdUI->SetCheck( pView && pView->IsKindOf(RUNTIME_CLASS(CInfoView)) );
}

void CMainFrame::OnViewSelector() 
{
	AfxSwitchToStaticView( RUNTIME_CLASS(CSelectorView) );
}

void CMainFrame::OnUpdateViewSelector(CCmdUI* pCmdUI) 
{
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	pCmdUI->SetCheck( pView && pView->IsKindOf(RUNTIME_CLASS(CSelectorView)) );
}

void CMainFrame::OnViewF10() 
{
	AfxSwitchToStaticView( RUNTIME_CLASS(CBaseView) );
//	AfxSwitchToStaticView( RUNTIME_CLASS(CInfoView) );
//	::PostMessage( AfxGetInfoView()->GetSafeHwnd(), WM_COMMAND, ID_VIEW_F10, 0 );
}

void CMainFrame::OnUpdateViewF10(CCmdUI* pCmdUI) 
{
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	pCmdUI->SetCheck( pView && pView->IsKindOf(RUNTIME_CLASS(CBaseView)) );
//	CView * pView = AfxGetStaticDoc()->GetActiveView();
//	CInfoView * pInfoView = AfxGetInfoView();
//	pCmdUI->SetCheck( pView && pView->IsKindOf(RUNTIME_CLASS(CInfoView))
//					&& pInfoView && pInfoView->IsViewF10() );
}

////////////////////////////////////////////////////////////////////////////////
// “列表”菜单
// 股票列表
void CMainFrame::OnSlistIndex()	{	AfxShowSlist( CStockContainer::typeIndex );	}
void CMainFrame::OnSlistStrategy()	{	AfxShowSlist( CStockContainer::typeStrategy );	}
void CMainFrame::OnSlistGroup()	{	AfxShowSlist( CStockContainer::typeGroup );	}
void CMainFrame::OnSlistGroupself()
{	CString	strSelf;
	strSelf.LoadString( IDS_DOMAIN_SELF );
	AfxShowSlist( CStockContainer::typeGroup, strSelf );	}
void CMainFrame::OnSlistDomain(){	AfxShowSlist( CStockContainer::typeDomain );}

void CMainFrame::OnSlistDomainRange( UINT nCmdID )
{
	if( nCmdID < ID_SLIST_DOMAIN_START || nCmdID > ID_SLIST_DOMAIN_END )
	{
		ASSERT( FALSE );
		return;
	}

	CDomainContainer &container = AfxGetDomainContainer();
	int nIndex	=	nCmdID - ID_SLIST_DOMAIN_START;
	CString	strDomainName;
	if( ID_SLIST_DOMAIN_END == nCmdID )
	{
		CSelectGrpDlg	dlg;
		dlg.SetMode( CSelectGrpDlg::modeDomain );
		if( IDOK == dlg.DoModal( ) )
			strDomainName	=	dlg.GetSelectedName();
		else
			return;
	}
	else if( nIndex < container.GetSize() )
	{
		CDomain	& domain = container.ElementAt(nIndex);
		strDomainName	=	domain.m_strName;
	}
	else
	{
		ASSERT( FALSE );
		return;
	}
	AfxShowSlist( CStockContainer::typeDomain, strDomainName );
}

void CMainFrame::OnSlistGroupRange( UINT nCmdID )
{
	if( nCmdID < ID_SLIST_GROUP_START || nCmdID > ID_SLIST_GROUP_END )
	{
		ASSERT( FALSE );
		return;
	}

	CDomainContainer &container = AfxGetGroupContainer();
	int nIndex	=	nCmdID - ID_SLIST_GROUP_START;
	CString	strGroupName;
	if( ID_SLIST_GROUP_END == nCmdID )
	{
		CSelectGrpDlg	dlg;
		dlg.SetMode( CSelectGrpDlg::modeGroup );
		if( IDOK == dlg.DoModal( ) )
			strGroupName	=	dlg.GetSelectedName();
		else
			return;
	}
	else if( nIndex < container.GetSize() )
	{
		CDomain	& domain = container.ElementAt(nIndex);
		strGroupName	=	domain.m_strName;
	}
	else
	{
		ASSERT( FALSE );
		return;
	}

	AfxShowSlist( CStockContainer::typeGroup, strGroupName );
}

// 股票列表报价
void CMainFrame::OnSlistAll()		{	AfxShowSlist( CStockContainer::typeAll );			}
void CMainFrame::OnSlistShaa()		{	AfxShowSlist( CStockContainer::typeClassShaa );		}
void CMainFrame::OnSlistShab()		{	AfxShowSlist( CStockContainer::typeClassShab );		}
void CMainFrame::OnSlistSzna()		{	AfxShowSlist( CStockContainer::typeClassSzna );		}
void CMainFrame::OnSlistSznb()		{	AfxShowSlist( CStockContainer::typeClassSznb );		}
void CMainFrame::OnSlistShabond()	{	AfxShowSlist( CStockContainer::typeClassShabond );	}
void CMainFrame::OnSlistSznbond()	{	AfxShowSlist( CStockContainer::typeClassSznbond );	}
// 股票列表涨跌排名
void CMainFrame::OnSlistShaasortdp()	{	AfxShowSlistSortDP( CStockContainer::typeClassShaa );		}
void CMainFrame::OnSlistShabsortdp()	{	AfxShowSlistSortDP( CStockContainer::typeClassShab );		}
void CMainFrame::OnSlistSznasortdp()	{	AfxShowSlistSortDP( CStockContainer::typeClassSzna );		}
void CMainFrame::OnSlistSznbsortdp()	{	AfxShowSlistSortDP( CStockContainer::typeClassSznb );		}
void CMainFrame::OnSlistShabondsortdp()	{	AfxShowSlistSortDP( CStockContainer::typeClassShabond );	}
void CMainFrame::OnSlistSznbondsortdp()	{	AfxShowSlistSortDP( CStockContainer::typeClassSznbond );	}

void CMainFrame::OnSlistSetcolumn()
{
	CSListView	* pView =	AfxGetSListView( );
	if( pView )
		pView->StoreColumnOrderArray( );

	CSetColumnDlg	dlg;
	if( IDOK == dlg.DoModal( ) )
	{
		// Show
		CSListView	* pView =	AfxGetSListView( );
		if( pView )
			pView->ResetColumns( );
		AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SLISTVIEW, NULL );
		AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SELECTORVIEW, NULL );
	}
}

void CMainFrame::OnSlistBaseindex() 
{
	CBaseIndexDlg	dlg;
	if( IDOK == dlg.DoModal( ) && dlg.m_bAddToList )
	{
		// Show
		CSListView	* pView =	AfxGetSListView( );
		if( pView )
			pView->ResetColumns( );
		AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SLISTVIEW, NULL );
		AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SELECTORVIEW, NULL );
	}
}

void CMainFrame::OnSlistbarDate( )	{	}
void CMainFrame::OnSlistbarName( )	{	}


////////////////////////////////////////////////////////////////////////////////
// “技术”菜单
// 指数日线显示
void CMainFrame::OnViewViewszzs()	{	AfxShowStockGraph( STKLIB_CODE_SZZS );	}
void CMainFrame::OnViewViewszybl()	{	AfxShowStockGraph( STKLIB_CODE_SZYBL );	}
void CMainFrame::OnViewViewszag()	{	AfxShowStockGraph( STKLIB_CODE_SZAG );	}
void CMainFrame::OnViewViewszbg()	{	AfxShowStockGraph( STKLIB_CODE_SZBG );	}
void CMainFrame::OnViewViewszncz()	{	AfxShowStockGraph( STKLIB_CODE_SZNCZ );	}
void CMainFrame::OnViewViewsznzz()	{	AfxShowStockGraph( STKLIB_CODE_SZNZZ );	}
void CMainFrame::OnViewViewszncfaz(){	AfxShowStockGraph( STKLIB_CODE_SZNCFAZ );	}
void CMainFrame::OnViewViewszncfbz(){	AfxShowStockGraph( STKLIB_CODE_SZNCFBZ );	}

void CMainFrame::OnViewTechoption()
{
	CSetParamDlg	dlg;
	dlg.DoModal();

	CGraphView * pGraphView = AfxGetGraphView();
	if( pGraphView )
		pGraphView->OnTechParametersChange( );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_GRAPHVIEW, NULL );
}

#define	SWITCH_SHOW_GRAPH_COMMAND( nID, nIDLast )				\
		AfxSwitchToStaticView( RUNTIME_CLASS(CGraphView) );		\
		::PostMessage( AfxGetGraphView()->GetSafeHwnd(), WM_COMMAND, nID, nIDLast );

#define	UPDATE_GRAPH_CMDUI( pCmdUI, updateFunc )				\
		CGraphView * pGraphView = AfxGetGraphView();			\
		if( pGraphView )										\
			pGraphView->updateFunc(pCmdUI);						\
		else													\
			pCmdUI->Enable(TRUE);

// K线图周期
void CMainFrame::OnViewPday()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_PDAY, 0 );	}
void CMainFrame::OnUpdateViewPday(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewPday);	}
void CMainFrame::OnViewPweek()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_PWEEK, 0 );	}
void CMainFrame::OnUpdateViewPweek(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewPweek);	}
void CMainFrame::OnViewPmonth()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_PMONTH, 0 );	}
void CMainFrame::OnUpdateViewPmonth(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewPmonth);	}
void CMainFrame::OnViewPmin5()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_PMIN5, 0 );	}
void CMainFrame::OnUpdateViewPmin5(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewPmin5);	}
void CMainFrame::OnViewPmin15()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_PMIN15, 0 );	}
void CMainFrame::OnUpdateViewPmin15(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewPmin15);	}
void CMainFrame::OnViewPmin30()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_PMIN30, 0 );	}
void CMainFrame::OnUpdateViewPmin30(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewPmin30);	}
void CMainFrame::OnViewPmin60()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_PMIN60, 0 );	}
void CMainFrame::OnUpdateViewPmin60(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewPmin60);	}

// K线图移动
void CMainFrame::OnViewLeft()		{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_LEFT, 0 );		}
void CMainFrame::OnViewRight()		{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_RIGHT, 0 );		}
void CMainFrame::OnViewPageleft()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_PAGELEFT, 0 );	}
void CMainFrame::OnViewPageright()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_PAGERIGHT, 0 );	}
void CMainFrame::OnViewHome()		{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_HOME, 0 );		}
void CMainFrame::OnViewEnd()		{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_END, 0 );		}
void CMainFrame::OnViewZoomin()		{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_ZOOMIN, 0 );		}
void CMainFrame::OnViewZoomout()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_ZOOMOUT, 0 );	}
void CMainFrame::OnViewPrev()		{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_PREV, 0 );		}
void CMainFrame::OnViewNext()		{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_NEXT, 0 );		}
void CMainFrame::OnViewInsert()		{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_INSERT, 0 );		}

void CMainFrame::OnViewTechRange( UINT nCmdID )	{	SWITCH_SHOW_GRAPH_COMMAND( nCmdID, 0 );	}
void CMainFrame::OnUpdateViewTechRange(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewTechRange);	}

// K Format
void CMainFrame::OnViewNoresumedr()		{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_NORESUMEDR, 0 );		}
void CMainFrame::OnUpdateViewNoresumedr(CCmdUI* pCmdUI)		{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewNoresumedr);	}
void CMainFrame::OnViewResumedrup()		{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_RESUMEDRUP, 0 );		}
void CMainFrame::OnUpdateViewResumedrup(CCmdUI* pCmdUI)		{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewResumedrup);	}
void CMainFrame::OnViewResumedrdown()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_RESUMEDRDOWN, 0 );	}
void CMainFrame::OnUpdateViewResumedrdown(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewResumedrdown);	}
// Main data Type
void CMainFrame::OnViewMaindataclose()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_MAINDATACLOSE, 0 );	}
void CMainFrame::OnUpdateViewMaindataclose(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewMaindataclose);	}
void CMainFrame::OnViewMaindataopen()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_MAINDATAOPEN, 0 );	}
void CMainFrame::OnUpdateViewMaindataopen(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewMaindataopen);	}
void CMainFrame::OnViewMaindataaverage(){	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_MAINDATAAVERAGE, 0 );}
void CMainFrame::OnUpdateViewMaindataaverage(CCmdUI* pCmdUI){	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewMaindataaverage);	}
// kline Mode
void CMainFrame::OnViewKlinecandle()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_KLINECANDLE, 0 );	}
void CMainFrame::OnUpdateViewKlinecandle(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewKlinecandle);		}
void CMainFrame::OnViewKlineamerica()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_KLINEAMERICA, 0 );	}
void CMainFrame::OnUpdateViewKlineamerica(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewKlineamerica);		}
void CMainFrame::OnViewKlinetower()	{	SWITCH_SHOW_GRAPH_COMMAND( ID_VIEW_KLINETOWER, 0 );	}
void CMainFrame::OnUpdateViewKlinetower(CCmdUI* pCmdUI)	{	UPDATE_GRAPH_CMDUI(pCmdUI,OnUpdateViewKlinetower);		}

////////////////////////////////////////////////////////////////////////////////
// “行情”菜单
// 指数实时行情显示
void CMainFrame::OnRealtimeViewszzs()		{	AfxShowStockRealTime( STKLIB_CODE_SZZS );	}
void CMainFrame::OnRealtimeViewszybl()		{	AfxShowStockRealTime( STKLIB_CODE_SZYBL );	}
void CMainFrame::OnRealtimeViewszag()		{	AfxShowStockRealTime( STKLIB_CODE_SZAG );	}
void CMainFrame::OnRealtimeViewszbg()		{	AfxShowStockRealTime( STKLIB_CODE_SZBG );	}
void CMainFrame::OnRealtimeViewszncz()		{	AfxShowStockRealTime( STKLIB_CODE_SZNCZ );	}
void CMainFrame::OnRealtimeViewsznzz()		{	AfxShowStockRealTime( STKLIB_CODE_SZNZZ );	}
void CMainFrame::OnRealtimeViewszncfaz()	{	AfxShowStockRealTime( STKLIB_CODE_SZNCFAZ );	}
void CMainFrame::OnRealtimeViewszncfbz()	{	AfxShowStockRealTime( STKLIB_CODE_SZNCFBZ );	}

#define	SWITCH_SHOW_REALTIME_COMMAND( nID, nIDLast )				\
		AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );		\
		::PostMessage( AfxGetRealTimeView()->GetSafeHwnd(), WM_COMMAND, nID, nIDLast );

#define	SWITCH_SHOW_REALTIME_MULTISTOCK( nMultiStockCount )			\
		AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );		\
		CRealTimeView * pView = AfxGetRealTimeView();				\
		if( pView )	pView->ShowMultiStock( nMultiStockCount );


// 实时行情键盘响应
void CMainFrame::OnRealtimeLeft()		{	SWITCH_SHOW_REALTIME_COMMAND( ID_REALTIME_LEFT, 0 );		}
void CMainFrame::OnRealtimeRight()		{	SWITCH_SHOW_REALTIME_COMMAND( ID_REALTIME_RIGHT, 0 );	}
void CMainFrame::OnRealtimeHome()		{	SWITCH_SHOW_REALTIME_COMMAND( ID_REALTIME_HOME, 0 );		}
void CMainFrame::OnRealtimeEnd()		{	SWITCH_SHOW_REALTIME_COMMAND( ID_REALTIME_END, 0 );		}
void CMainFrame::OnRealtimePrev()		{	SWITCH_SHOW_REALTIME_COMMAND( ID_REALTIME_PREV, 0 );		}
void CMainFrame::OnRealtimeNext()		{	SWITCH_SHOW_REALTIME_COMMAND( ID_REALTIME_NEXT, 0 );		}

void CMainFrame::OnRealtimeOnestock()	{	SWITCH_SHOW_REALTIME_MULTISTOCK( 1 );		}
void CMainFrame::OnRealtimeTwostock()	{	SWITCH_SHOW_REALTIME_MULTISTOCK( 2 );		}
void CMainFrame::OnRealtimeFourstock()	{	SWITCH_SHOW_REALTIME_MULTISTOCK( 4 );		}
void CMainFrame::OnRealtimeSixstock()	{	SWITCH_SHOW_REALTIME_MULTISTOCK( 6 );		}
void CMainFrame::OnRealtimeNinestock()	{	SWITCH_SHOW_REALTIME_MULTISTOCK( 9 );		}

void CMainFrame::OnUpdateRealtimeOnestock(CCmdUI* pCmdUI)
{	CRealTimeView * pView = AfxGetRealTimeView();
	pCmdUI->SetCheck( pView && 1 == pView->GetShowMultiStock() );	}
void CMainFrame::OnUpdateRealtimeTwostock(CCmdUI* pCmdUI)
{	CRealTimeView * pView = AfxGetRealTimeView();
	pCmdUI->SetCheck( pView && 2 == pView->GetShowMultiStock() );	}
void CMainFrame::OnUpdateRealtimeFourstock(CCmdUI* pCmdUI)
{	CRealTimeView * pView = AfxGetRealTimeView();
	pCmdUI->SetCheck( pView && 4 == pView->GetShowMultiStock() );	}
void CMainFrame::OnUpdateRealtimeSixstock(CCmdUI* pCmdUI)
{	CRealTimeView * pView = AfxGetRealTimeView();
	pCmdUI->SetCheck( pView && 6 == pView->GetShowMultiStock() );	}
void CMainFrame::OnUpdateRealtimeNinestock(CCmdUI* pCmdUI)
{	CRealTimeView * pView = AfxGetRealTimeView();
	pCmdUI->SetCheck( pView && 9 == pView->GetShowMultiStock() );	}

void CMainFrame::OnRealtimePriceline()
{	AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );
	CRealTimeView * pView = AfxGetRealTimeView();
	if( pView )	pView->ChangeDrawMode( CRealTime::modePriceLine );	}

void CMainFrame::OnRealtimeReportdetail()
{	AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );
	CRealTimeView * pView = AfxGetRealTimeView();
	if( pView )	pView->ChangeDrawMode( CRealTime::modeReportDetail );	}

void CMainFrame::OnRealtimeMinutedetail()
{	AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );
	CRealTimeView * pView = AfxGetRealTimeView();
	if( pView )	pView->ChangeDrawMode( CRealTime::modeMinuteDetail );	}

void CMainFrame::OnRealtimeBigtradedetail()
{	AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );
	CRealTimeView * pView = AfxGetRealTimeView();
	if( pView )	pView->ChangeDrawMode( CRealTime::modeBigTradeDetail );	}

void CMainFrame::OnRealtimeLbdk()
{	AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );
	CRealTimeView * pView = AfxGetRealTimeView();
	if( pView )	pView->ChangeDrawMode( CRealTime::modePriceLine );
	if( pView )	pView->ToggleDrawTechLine( CRealTime::techLBDK );	}

void CMainFrame::OnRealtimeMmld()
{	AfxSwitchToStaticView( RUNTIME_CLASS(CRealTimeView) );
	CRealTimeView * pView = AfxGetRealTimeView();
	if( pView )	pView->ChangeDrawMode( CRealTime::modePriceLine );
	if( pView )	pView->ToggleDrawTechLine( CRealTime::techMMLD );	}

void CMainFrame::OnUpdateRealtimePriceline(CCmdUI* pCmdUI)
{	CRealTimeView * pView = AfxGetRealTimeView();
	pCmdUI->SetCheck( pView && CRealTime::modePriceLine == pView->GetDrawMode() );	}
void CMainFrame::OnUpdateRealtimeReportdetail(CCmdUI* pCmdUI)
{	CRealTimeView * pView = AfxGetRealTimeView();
	pCmdUI->SetCheck( pView && CRealTime::modeReportDetail == pView->GetDrawMode() );	}
void CMainFrame::OnUpdateRealtimeMinutedetail(CCmdUI* pCmdUI)
{	CRealTimeView * pView = AfxGetRealTimeView();
	pCmdUI->SetCheck( pView && CRealTime::modeMinuteDetail == pView->GetDrawMode() );	}
void CMainFrame::OnUpdateRealtimeBigtradedetail(CCmdUI* pCmdUI)
{	CRealTimeView * pView = AfxGetRealTimeView();
	pCmdUI->SetCheck( pView && CRealTime::modeBigTradeDetail == pView->GetDrawMode() );	}
void CMainFrame::OnUpdateRealtimeLbdk(CCmdUI* pCmdUI)
{	CRealTimeView * pView = AfxGetRealTimeView();
	pCmdUI->SetCheck( pView && CRealTime::techLBDK == pView->GetDrawTechLine() );	}
void CMainFrame::OnUpdateRealtimeMmld(CCmdUI* pCmdUI)
{	CRealTimeView * pView = AfxGetRealTimeView();
	pCmdUI->SetCheck( pView && CRealTime::techMMLD == pView->GetDrawTechLine() );	}

// 分类股票综合排序
void CMainFrame::OnRealtimeMsszag()		{	AfxShowMultiSort( CStock::typeshA );	}
void CMainFrame::OnRealtimeMsszbg()		{	AfxShowMultiSort( CStock::typeshB );	}
void CMainFrame::OnRealtimeMsszzq()		{	AfxShowMultiSort( CStock::typeshBond );	}
void CMainFrame::OnRealtimeMssznag()	{	AfxShowMultiSort( CStock::typeszA );	}
void CMainFrame::OnRealtimeMssznbg()	{	AfxShowMultiSort( CStock::typeszB );	}
void CMainFrame::OnRealtimeMssznzq()	{	AfxShowMultiSort( CStock::typeszBond );	}

static	LONG GetMultiSortClass( )
{
	CMultiSortView * pMultiSortView = AfxGetMultiSortView();
	if( pMultiSortView )
		return pMultiSortView->GetMultiSortClass();
	return CStock::typeNone;
}
void CMainFrame::OnUpdateRealtimeMsszag(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CStock::typeshA == GetMultiSortClass() );		}
void CMainFrame::OnUpdateRealtimeMsszbg(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CStock::typeshB == GetMultiSortClass() );		}
void CMainFrame::OnUpdateRealtimeMsszzq(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CStock::typeshBond == GetMultiSortClass() );	}
void CMainFrame::OnUpdateRealtimeMssznag(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck( CStock::typeszA == GetMultiSortClass() );		}
void CMainFrame::OnUpdateRealtimeMssznbg(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck( CStock::typeszB == GetMultiSortClass() );		}
void CMainFrame::OnUpdateRealtimeMssznzq(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck( CStock::typeszBond == GetMultiSortClass() );	}


// 在线资讯
void CMainFrame::OnInfoSelf()		{	AfxShowInfo( CInfoView::serverSelf );		}
void CMainFrame::OnInfoF10()		{	AfxShowInfo( CInfoView::serverF10 );		}
void CMainFrame::OnInfoStockstar()	{	AfxShowInfo( CInfoView::serverStockstar );	}
void CMainFrame::OnInfoSohu()		{	AfxShowInfo( CInfoView::serverSohu );		}
void CMainFrame::OnInfoSina()		{	AfxShowInfo( CInfoView::serverSina );		}
void CMainFrame::OnInfo163()		{	AfxShowInfo( CInfoView::server163 );		}
void CMainFrame::OnInfoShse()		{	AfxShowInfo( CInfoView::serverShse );		}
void CMainFrame::OnInfoSzse()		{	AfxShowInfo( CInfoView::serverSzse );		}
void CMainFrame::OnInfoHkex()		{	AfxShowInfo( CInfoView::serverHkex );		}
void CMainFrame::OnInfoLse()		{	AfxShowInfo( CInfoView::serverLse );		}
void CMainFrame::OnInfoNyse()		{	AfxShowInfo( CInfoView::serverNyse );		}
void CMainFrame::OnInfoNasdaq()		{	AfxShowInfo( CInfoView::serverNasdaq );		}
void CMainFrame::OnInfoP5w()		{	AfxShowInfo( CInfoView::serverP5w );		}
void CMainFrame::OnInfoSsnews()		{	AfxShowInfo( CInfoView::serverSsnews );		}
void CMainFrame::OnInfoCs()			{	AfxShowInfo( CInfoView::serverCs );			}

static	int GetInfoServer( )
{
	CInfoView * pInfoView = AfxGetInfoView();
	if( pInfoView )
		return pInfoView->GetCurrentServer( );
	return CInfoView::serverUnknown;
}
void CMainFrame::OnUpdateInfoSelf(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverSelf == GetInfoServer() );		}
void CMainFrame::OnUpdateInfoF10(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverF10 == GetInfoServer() );		}
void CMainFrame::OnUpdateInfoStockstar(CCmdUI* pCmdUI)	{	pCmdUI->SetCheck( CInfoView::serverStockstar == GetInfoServer() );	}
void CMainFrame::OnUpdateInfoSohu(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverSohu == GetInfoServer() );		}
void CMainFrame::OnUpdateInfoSina(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverSina == GetInfoServer() );		}
void CMainFrame::OnUpdateInfo163(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::server163 == GetInfoServer() );		}
void CMainFrame::OnUpdateInfoShse(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverShse == GetInfoServer() );		}
void CMainFrame::OnUpdateInfoSzse(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverSzse == GetInfoServer() );		}
void CMainFrame::OnUpdateInfoHkex(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverHkex == GetInfoServer() );		}
void CMainFrame::OnUpdateInfoLse(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverLse == GetInfoServer() );		}
void CMainFrame::OnUpdateInfoNyse(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverNyse == GetInfoServer() );		}
void CMainFrame::OnUpdateInfoNasdaq(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverNasdaq == GetInfoServer() );		}
void CMainFrame::OnUpdateInfoP5w(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverP5w == GetInfoServer() );		}
void CMainFrame::OnUpdateInfoSsnews(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverSsnews == GetInfoServer() );		}
void CMainFrame::OnUpdateInfoCs(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck( CInfoView::serverCs == GetInfoServer() );				}

////////////////////////////////////////////////////////////////////////////////
// “策略”菜单
// 策略
void CMainFrame::OnStrategyProperty() 
{
	CSimuView	*	pSimuView	=	AfxGetSimuView();
	if( pSimuView && !pSimuView->StoreToStrategy() )
		return;

	CStrategy * pStrategy = AfxGetActiveStrategy( );
	if( pStrategy )
	{
		CSetPrpt		firstPage( NULL, IDS_STRATEGY_SETPRPT, NULL );
		firstPage.SetStrategy( pStrategy );
		
		CSelectStk		selectStk(NULL, IDS_STRATEGY_SELECTSTK, NULL);
		selectStk.m_dwButtonFlags	=	0;
		selectStk.m_domainTemp.Copy( pStrategy->GetStocks() );

		CSetRule		setrule(NULL, IDS_STRATEGY_SETRULE, NULL);
		setrule.SetTechParameters( &(pStrategy->GetTechParametersBuy()), &(pStrategy->GetSelectedTechsBuy()) );

		CSetRate		setrate(NULL, IDS_STRATEGY_SETRATE, NULL);
		setrate.SetStrategy( pStrategy );

		CXFPropertySheetEx	sht;
		sht.AddPage( &firstPage );
		sht.AddPage( &selectStk );
		sht.AddPage( &setrule );
		sht.AddPage( &setrate );
		sht.SetActivePage( &firstPage );

		CBitmap	bmp;
		bmp.LoadBitmap( IDB_BALANG );
		sht.SetBitmap( (HBITMAP)bmp.GetSafeHandle() );
		sht.DoModal();

		pStrategy->SetStocks( selectStk.m_domainTemp );
		pStrategy->DoFileSave( );
		AfxOnStrategyUpdated( pStrategy, selectStk.m_bModified || setrule.m_bModified || setrate.m_bModified );
	}
}

void CMainFrame::OnUpdateStrategyProperty(CCmdUI* pCmdUI) 
{
	CStrategy * pStrategy = AfxGetActiveStrategy( );
	pCmdUI->Enable( pStrategy && pStrategy->CanModifiedNow() );
}

void CMainFrame::OnStrategyStocks() 
{
	CSimuView	*	pSimuView	=	AfxGetSimuView();
	if( pSimuView && !pSimuView->StoreToStrategy() )
		return;

	CStrategy * pStrategy = AfxGetActiveStrategy( );
	if( pStrategy )
	{
		CSetPrpt		firstPage( NULL, IDS_STRATEGY_SETPRPT, NULL );
		firstPage.SetStrategy( pStrategy );
		
		CSelectStk		selectStk(NULL, IDS_STRATEGY_SELECTSTK, NULL);
		selectStk.m_dwButtonFlags	=	0;
		selectStk.m_domainTemp.Copy( pStrategy->GetStocks() );

		CSetRule		setrule(NULL, IDS_STRATEGY_SETRULE, NULL);
		setrule.SetTechParameters( &(pStrategy->GetTechParametersBuy()), &(pStrategy->GetSelectedTechsBuy()) );

		CSetRate		setrate(NULL, IDS_STRATEGY_SETRATE, NULL);
		setrate.SetStrategy( pStrategy );

		CXFPropertySheetEx	sht;
		sht.AddPage( &firstPage );
		sht.AddPage( &selectStk );
		sht.AddPage( &setrule );
		sht.AddPage( &setrate );
		sht.SetActivePage( &selectStk );

		CBitmap	bmp;
		bmp.LoadBitmap( IDB_BALANG );
		sht.SetBitmap( (HBITMAP)bmp.GetSafeHandle() );
		sht.DoModal();

		pStrategy->SetStocks( selectStk.m_domainTemp );
		pStrategy->DoFileSave( );
		AfxOnStrategyUpdated( pStrategy, selectStk.m_bModified || setrule.m_bModified || setrate.m_bModified );
	}
}

void CMainFrame::OnUpdateStrategyStocks(CCmdUI* pCmdUI) 
{
	CStrategy * pStrategy = AfxGetActiveStrategy( );
	pCmdUI->Enable( pStrategy && pStrategy->CanModifiedNow() );
}

void CMainFrame::OnStrategySetRule() 
{
	CSimuView	*	pSimuView	=	AfxGetSimuView();
	if( pSimuView && !pSimuView->StoreToStrategy() )
		return;

	CStrategy * pStrategy = AfxGetActiveStrategy( );
	if( pStrategy )
	{
		CSetPrpt		firstPage( NULL, IDS_STRATEGY_SETPRPT, NULL );
		firstPage.SetStrategy( pStrategy );
		
		CSelectStk		selectStk(NULL, IDS_STRATEGY_SELECTSTK, NULL);
		selectStk.m_dwButtonFlags	=	0;
		selectStk.m_domainTemp.Copy( pStrategy->GetStocks() );

		CSetRule		setrule(NULL, IDS_STRATEGY_SETRULE, NULL);
		setrule.SetTechParameters( &(pStrategy->GetTechParametersBuy()), &(pStrategy->GetSelectedTechsBuy()) );

		CSetRate		setrate(NULL, IDS_STRATEGY_SETRATE, NULL);
		setrate.SetStrategy( pStrategy );

		CXFPropertySheetEx	sht;
		sht.AddPage( &firstPage );
		sht.AddPage( &selectStk );
		sht.AddPage( &setrule );
		sht.AddPage( &setrate );
		sht.SetActivePage( &setrule );

		CBitmap	bmp;
		bmp.LoadBitmap( IDB_BALANG );
		sht.SetBitmap( (HBITMAP)bmp.GetSafeHandle() );
		sht.DoModal();

		pStrategy->SetStocks( selectStk.m_domainTemp );
		pStrategy->DoFileSave( );
		AfxOnStrategyUpdated( pStrategy, selectStk.m_bModified || setrule.m_bModified || setrate.m_bModified );
	}
}

void CMainFrame::OnUpdateStrategySetRule(CCmdUI* pCmdUI) 
{
	CStrategy * pStrategy = AfxGetActiveStrategy( );
	pCmdUI->Enable( pStrategy && pStrategy->CanModifiedNow() );
}

void CMainFrame::OnStrategySetRate() 
{
	CSimuView	*	pSimuView	=	AfxGetSimuView();
	if( pSimuView && !pSimuView->StoreToStrategy() )
		return;

	CStrategy * pStrategy = AfxGetActiveStrategy( );
	if( pStrategy )
	{
		CSetPrpt		firstPage( NULL, IDS_STRATEGY_SETPRPT, NULL );
		firstPage.SetStrategy( pStrategy );
		
		CSelectStk		selectStk(NULL, IDS_STRATEGY_SELECTSTK, NULL);
		selectStk.m_dwButtonFlags	=	0;
		selectStk.m_domainTemp.Copy( pStrategy->GetStocks() );

		CSetRule		setrule(NULL, IDS_STRATEGY_SETRULE, NULL);
		setrule.SetTechParameters( &(pStrategy->GetTechParametersBuy()), &(pStrategy->GetSelectedTechsBuy()) );

		CSetRate		setrate(NULL, IDS_STRATEGY_SETRATE, NULL);
		setrate.SetStrategy( pStrategy );

		CXFPropertySheetEx	sht;
		sht.AddPage( &firstPage );
		sht.AddPage( &selectStk );
		sht.AddPage( &setrule );
		sht.AddPage( &setrate );
		sht.SetActivePage( &setrate );

		CBitmap	bmp;
		bmp.LoadBitmap( IDB_BALANG );
		sht.SetBitmap( (HBITMAP)bmp.GetSafeHandle() );
		sht.DoModal();

		pStrategy->SetStocks( selectStk.m_domainTemp );
		pStrategy->DoFileSave( );
		AfxOnStrategyUpdated( pStrategy, selectStk.m_bModified || setrule.m_bModified || setrate.m_bModified );
	}
}

void CMainFrame::OnUpdateStrategySetRate(CCmdUI* pCmdUI) 
{
	CStrategy * pStrategy = AfxGetActiveStrategy( );
	pCmdUI->Enable( pStrategy && pStrategy->CanModifiedNow() );
}

void CMainFrame::OnStrategySimu() 
{
	AfxSwitchToStaticView( RUNTIME_CLASS(CSimuView) );
}

void CMainFrame::OnUpdateStrategySimu(CCmdUI* pCmdUI) 
{
	CStrategy * pStrategy = AfxGetActiveStrategy( );
	pCmdUI->Enable( NULL != pStrategy );
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	pCmdUI->SetCheck( pView && pView->IsKindOf(RUNTIME_CLASS(CSimuView)) );
}

void CMainFrame::OnStrategyReport() 
{
	CSimuView	*	pSimuView	=	AfxGetSimuView();
	if( pSimuView && !pSimuView->StoreToStrategy() )
		return;

	CStrategy * pStrategy = AfxGetActiveStrategy( );
	if( pStrategy )
	{
		CSimuReport		report;
		report.SetStrategy( pStrategy );
		report.DoModal();
	}
}

void CMainFrame::OnUpdateStrategyReport(CCmdUI* pCmdUI) 
{
	CStrategy * pStrategy = AfxGetActiveStrategy( );
	pCmdUI->Enable( pStrategy && pStrategy->CanModifiedNow() );
}

void CMainFrame::OnStrategyRealOp() 
{
	CSimuView	*	pSimuView	=	AfxGetSimuView();
	if( pSimuView && !pSimuView->StoreToStrategy() )
		return;

	CStrategy * pStrategy = AfxGetActiveStrategy( );
	if( pStrategy )
	{
		{	// 运行策略实战，选定下一步操作
			CWaitDlg	wait( AfxGetMainWnd(), FALSE );
			wait.SetProgressRange( 0, STRATEGY_MAX_PROGRESS );

			pStrategy->ClearCache( );
			pStrategy->RealRun( RealRunCallback, wait.GetSafeHwnd() );

			wait.DestroyWindow();
		}

		CSimuRealOp		realop;
		realop.SetStrategy( pStrategy );
		realop.DoModal();

		pStrategy->OnRealOpViewed( );
		pStrategy->DoFileSave( );
		AfxOnStrategyUpdated( pStrategy, realop.m_bModified );
	}
}

void CMainFrame::OnUpdateStrategyRealOp(CCmdUI* pCmdUI) 
{
	CStrategy * pStrategy = AfxGetActiveStrategy( );
	pCmdUI->Enable( pStrategy && pStrategy->CanModifiedNow() );
}

////////////////////////////////////////////////////////////////////////////////
// “数据”菜单
// 数据
void CMainFrame::OnDataDownload() 
{
	CDownloadDlg	* dlg = new CDownloadDlg;
	dlg->Create( CDownloadDlg::IDD, this );
	dlg->SetAutoDelete( TRUE );
	dlg->CenterWindow(this);
	dlg->SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOSIZE );
}

void CMainFrame::OnDataDatasource() 
{
	CDataSrcDlg	dlg;
	dlg.DoModal();
}

void CMainFrame::OnDataManage() 
{
	CInstallPacDlg	installdlg;
	CTyDataDlg		tydata;
	CSetDrdataDlg	setdr;
	CSetBaseDlg		setbase;
	CSetBasedataDlg	setbasedata;
	CSetKDataDlg	setkdata;

	CXFPropertySheetEx	sht;
	sht.AddPage( &installdlg );
	sht.AddPage( &tydata );
	sht.AddPage( &setdr );
	sht.AddPage( &setbase );
	sht.AddPage( &setbasedata );
	sht.AddPage( &setkdata );
	sht.SetActivePage( &installdlg );

	sht.DoModal();
}

void CMainFrame::OnDataExport() 
{
	CSelectStk		selectStk(NULL, IDS_EXPORT_SELECTSTK, NULL);
	selectStk.m_dwButtonFlags	=	PSWIZB_NEXT | PSWIZB_DISABLEDFINISH;
	
	CExportOption	option(NULL, IDS_EXPORT_OPTION, NULL);
	option.m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_DISABLEDFINISH;
	
	CExportDest		dest(NULL, IDS_EXPORT_DEST, NULL);
	dest.m_dwButtonFlags	=	PSWIZB_BACK | PSWIZB_NEXT;

	CXFPropertySheetEx	sht;

	sht.AddPage( &selectStk );
	sht.AddPage( &option );
	sht.AddPage( &dest );
	sht.SetWizardMode( );
	
	sht.m_psh.dwFlags |= ( PSH_WIZARD97 | PSH_WIZARDHASFINISH /* | PSH_HEADER /*Header Bitmap*/ );
	CBitmap	bmp;
	bmp.LoadBitmap( IDB_BALANG );
	sht.SetBitmap( (HBITMAP)bmp.GetSafeHandle() );
	sht.DoModal();
}

void CMainFrame::OnDataRecalculateyield() 
{
	AfxRecalculateYield( AfxGetProfile().GetYieldAverageDays(), TRUE );
	AfxGetStockContainer().ReloadBase( &AfxGetDB() );
	AfxGetSListStockContainer().ReRetrieveFromStatic( AfxGetActiveStrategy() );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SLISTVIEW, NULL );
	AfxGetStaticDoc()->UpdateAllViews( NULL, UPDATE_HINT_SELECTORVIEW, NULL );
}

////////////////////////////////////////////////////////////////////////////////
// “选项”菜单
// 选项
void CMainFrame::OnOptionAddtogroupRange( UINT nCmdID )
{
	if( nCmdID < ID_OPTION_ADDTOGROUP_START || nCmdID > ID_OPTION_ADDTOGROUP_END )
	{
		ASSERT( FALSE );
		return;
	}

	CDomainContainer &container = AfxGetGroupContainer();
	int nIndex	=	nCmdID - ID_OPTION_ADDTOGROUP_START;
	CString	strGroupName;
	if( ID_OPTION_ADDTOGROUP_END == nCmdID )
	{
		CSelectGrpDlg	dlg;
		dlg.SetMode( CSelectGrpDlg::modeGroup );
		if( IDOK == dlg.DoModal( ) )
			strGroupName	=	dlg.GetSelectedName();
		else
			return;
	}
	else if( nIndex < container.GetSize() )
	{
		CDomain	& domain = container.ElementAt(nIndex);
		strGroupName	=	domain.m_strName;
	}
	else
	{
		ASSERT( FALSE );
		return;
	}

	CSetGroupDlg	dlgSetGroup;
	CSPStringArray	astrStocks;

	CView * pView = AfxGetStaticDoc()->GetActiveView();
	if( pView && pView->IsKindOf(RUNTIME_CLASS(CSListView)) )
	{
		((CSListView *)pView)->GetSelectedStocks( astrStocks );
	}
	else if( pView && pView->IsKindOf(RUNTIME_CLASS(CSelectorView)) )
	{
		((CSelectorView *)pView)->GetSelectedStocks( astrStocks );
	}
	else if( pView &&
		( pView->IsKindOf(RUNTIME_CLASS(CGraphView))
		|| pView->IsKindOf(RUNTIME_CLASS(CBaseView))
		|| pView->IsKindOf(RUNTIME_CLASS(CRealTimeView)) ) )
	{
		CStockInfo	info;
		if( AfxGetProfile().GetCurrentStock( &info ) )
			astrStocks.Add( info.GetStockCode() );
	}
	dlgSetGroup.SetInitialGroup( strGroupName );
	dlgSetGroup.AddtoGroup( astrStocks );
	dlgSetGroup.DoModal();
}

void CMainFrame::OnUpdateOptionAddtogroupRange(CCmdUI* pCmdUI) 
{
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	if( pView && pView->IsKindOf(RUNTIME_CLASS(CSListView)) )
	{
		CSPStringArray	astrStocks;
		((CSListView *)pView)->GetSelectedStocks( astrStocks );
		pCmdUI->Enable( astrStocks.GetSize() > 0 );
	}
	else if( pView && pView->IsKindOf(RUNTIME_CLASS(CSelectorView)) )
	{
		CSPStringArray	astrStocks;
		((CSelectorView *)pView)->GetSelectedStocks( astrStocks );
		pCmdUI->Enable( astrStocks.GetSize() > 0 );
	}
	else if( pView &&
		( pView->IsKindOf(RUNTIME_CLASS(CGraphView))
		|| pView->IsKindOf(RUNTIME_CLASS(CBaseView))
		|| pView->IsKindOf(RUNTIME_CLASS(CRealTimeView)) ) )
	{
		CStockInfo	info;
		pCmdUI->Enable( AfxGetProfile().GetCurrentStock( &info ) );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}

void CMainFrame::OnOptionAddtostrategy( )
{
	CSelectGrpDlg	dlg;
	dlg.SetMode( CSelectGrpDlg::modeStrategy );
	if( IDOK != dlg.DoModal( ) )
		return;
	
	CStrategy	*	pStrategy		=	dlg.GetSelectedStrategy( );
	if( NULL == pStrategy )
		return;

	// Get Current Stock
	CSPStringArray	astrStocks;

	CView * pView = AfxGetStaticDoc()->GetActiveView();
	if( pView && pView->IsKindOf(RUNTIME_CLASS(CSListView)) )
	{
		((CSListView *)pView)->GetSelectedStocks( astrStocks );
	}
	else if( pView && pView->IsKindOf(RUNTIME_CLASS(CSelectorView)) )
	{
		((CSelectorView *)pView)->GetSelectedStocks( astrStocks );
	}
	else if( pView &&
		( pView->IsKindOf(RUNTIME_CLASS(CGraphView))
		|| pView->IsKindOf(RUNTIME_CLASS(CBaseView))
		|| pView->IsKindOf(RUNTIME_CLASS(CRealTimeView)) ) )
	{
		CStockInfo	info;
		if( AfxGetProfile().GetCurrentStock( &info ) )
			astrStocks.Add( info.GetStockCode() );
	}

	CSPStringArray	astr;
	astr.Copy( pStrategy->GetStocks() );
	astr.Append( astrStocks );
	
	pStrategy->SetStocks( astr );
	pStrategy->DoFileSave( );
	AfxOnStrategyUpdated( pStrategy, TRUE );
}

void CMainFrame::OnUpdateOptionAddtostrategy(CCmdUI* pCmdUI) 
{/*
	CView * pView = AfxGetStaticDoc()->GetActiveView();
	if( pView && pView->IsKindOf(RUNTIME_CLASS(CSListView)) )
	{
		CSPStringArray	astrStocks;
		((CSListView *)pView)->GetSelectedStocks( astrStocks );
		pCmdUI->Enable( astrStocks.GetSize() > 0 );
	}
	else if( pView && pView->IsKindOf(RUNTIME_CLASS(CSelectorView)) )
	{
		CSPStringArray	astrStocks;
		((CSelectorView *)pView)->GetSelectedStocks( astrStocks );
		pCmdUI->Enable( astrStocks.GetSize() > 0 );
	}
	else if( pView &&
		( pView->IsKindOf(RUNTIME_CLASS(CGraphView))
		|| pView->IsKindOf(RUNTIME_CLASS(CBaseView))
		|| pView->IsKindOf(RUNTIME_CLASS(CRealTimeView)) ) )
	{
		CStockInfo	info;
		pCmdUI->Enable( AfxGetProfile().GetCurrentStock( &info ) );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}*/
}

// 选项
void CMainFrame::OnOptionProxyset()
{
	AfxDoProxySetting();
}

void CMainFrame::OnOptionSelecttrader()
{
	CSelectTraderDlg	dlg;
	dlg.DoModal( );
}

void CMainFrame::OnOptionColor()
{
	CSetColorDlg	* dlg	=	new CSetColorDlg();
	dlg->Create( CSetColorDlg::IDD, this );
	dlg->SetAutoDelete( TRUE );
	dlg->CenterWindow(this);
	dlg->SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOSIZE );
}

void CMainFrame::OnOptionFontslist()
{
	LOGFONT	lf;
	memset( &lf, 0, sizeof(lf) );
	AfxGetProfile().GetFontSListView( &lf );

	CFontDialog	dlg( &lf, CF_SCREENFONTS );
	if( IDOK == dlg.DoModal() )
	{
		AfxGetProfile().SetFontSListView( &lf );
		CSListView * pSListView = AfxGetSListView( );
		if( pSListView )
			pSListView->SetFont( &lf );
	}
}

void CMainFrame::OnOptionFontbase()
{
	LOGFONT	lf;
	memset( &lf, 0, sizeof(lf) );
	AfxGetProfile().GetFontBaseView( &lf );

	CFontDialog	dlg( &lf, CF_SCREENFONTS );
	if( IDOK == dlg.DoModal() )
	{
		AfxGetProfile().SetFontBaseView( &lf );
		CBaseView * pBaseView = AfxGetBaseView( );
		if( pBaseView )
			pBaseView->SetFont( &lf );
	}
}

void CMainFrame::OnOptionSetgroup() 
{
	CSetGroupDlg	dlg;
	dlg.DoModal();
}

////////////////////////////////////////////////////////////////////////////////
// “工具”菜单
// 工具
void CMainFrame::OnToolTrader() 
{
	CString	strFile	=	AfxGetProfile().GetTrader();
	if( 0 == access(strFile,0) )
		ShellExecute( NULL, "open", strFile, NULL, NULL, SW_SHOW );
	else
		OnOptionSelecttrader( );
}

void CMainFrame::OnToolNote() 
{
	ShellExecute( NULL, "open", "notepad.exe",
					NULL, AfxGetProfile().GetWorkDirectory(), SW_SHOW );
}

void CMainFrame::OnToolCalculator() 
{
	ShellExecute( NULL, "open", "calc.exe",
					NULL, AfxGetProfile().GetWorkDirectory(), SW_SHOW );
}

void CMainFrame::OnToolFilterinfo()
{
	CFilterInfoDlg	dlg;
	dlg.DoModal();
}

void CMainFrame::OnToolAlarm()
{
	CAlarmSettingDlg	alarmsetting;
	CAlarmResultDlg		alarmresult;

	CXFPropertySheetEx	sht;
	sht.AddPage( &alarmsetting );
	sht.AddPage( &alarmresult );

	sht.DoModal();
}

////////////////////////////////////////////////////////////////////////////////
// “帮助”菜单
// 帮助
void CMainFrame::OnHelpShowhelp()
{
	::HtmlHelp(NULL, "StockAna.chm::/htm/index.htm", HH_DISPLAY_TOPIC, 0);
}

void CMainFrame::OnHelpPurchase()
{
	::HtmlHelp(NULL, "StockAna.chm::/htm/purchase.htm", HH_DISPLAY_TOPIC, 0);
}

void CMainFrame::OnHelpProduct() 
{
	CString	strURL = AfxGetProfile().GetProductURL();

	int result = (int)AfxGotoURL(strURL, SW_SHOW);
	if( result <= HINSTANCE_ERROR )
	{
		MessageBeep(MB_ICONEXCLAMATION);
	}
}

void CMainFrame::OnHelpRegister() 
{
	CRegisterDlg	dlg;
	dlg.DoModal();
}

void CMainFrame::OnHelpDeclare() 
{
	CBitmap	bmp;
	bmp.LoadBitmap(IDB_DLGLEFTLOGO);

	CString	sText;
	HGLOBAL		hRead = NULL;
	HRSRC		hReSrc = ::FindResource( AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_TEXT_DECLARE), _T("Text") );
	if( hReSrc )
		hRead	=	::LoadResource( AfxGetResourceHandle(), hReSrc );
	if( hRead )
	{
		LPCTSTR	lpText	=	(LPCTSTR)::LockResource( hRead );
		if( lpText )
			sText	=	lpText;
		::UnlockResource( hRead );
	}
	
	CDeclareDlg	dlg;
	dlg.SetBitmap( (HBITMAP)bmp.GetSafeHandle() );
	dlg.SetDeclareText( sText );
	dlg.DoModal( );
}


