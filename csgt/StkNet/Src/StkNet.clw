; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProxySetDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "stknet.h"
LastPage=0

ClassCount=5
Class1=CStartupDlg
Class2=CStkReceiver
Class3=CEditSrvDlg
Class4=CProxySetDlg
Class5=CSelectEngineDlg

ResourceCount=8
Resource1=IDD_SELECTENGINE (English (U.S.))
Resource2=IDD_STARTUP
Resource3=IDD_SELECTENGINE
Resource4=IDD_EDITSERVER (English (U.S.))
Resource5=IDD_PROXYSET (English (U.S.))
Resource6=IDD_PROXYSET
Resource7=IDD_EDITSERVER
Resource8=IDD_STARTUP (English (U.S.))

[CLS:CStartupDlg]
Type=0
BaseClass=CDialog
HeaderFile=\Projects\CLKing\Src\StkNet\Include\StartupDlg.h
ImplementationFile=StartupDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CStartupDlg

[CLS:CStkReceiver]
Type=0
BaseClass=CWnd
HeaderFile=\Projects\CLKing\Src\StkNet\Include\StkReceiver.h
ImplementationFile=StkReceiver.cpp

[CLS:CEditSrvDlg]
Type=0
BaseClass=CDialog
HeaderFile=EditSrvDlg.h
ImplementationFile=EditSrvDlg.cpp
LastObject=CEditSrvDlg
Filter=D
VirtualFilter=dWC

[CLS:CProxySetDlg]
Type=0
BaseClass=CDialog
HeaderFile=ProxySetDlg.h
ImplementationFile=ProxySetDlg.cpp
Filter=D
VirtualFilter=dWC

[CLS:CSelectEngineDlg]
Type=0
BaseClass=CDialog
HeaderFile=SelectEngineDlg.h
ImplementationFile=SelectEngineDlg.cpp
Filter=D
VirtualFilter=dWC

[DLG:IDD_STARTUP]
Type=1
Class=CStartupDlg
ControlCount=13
Control1=IDOK,button,1342275585
Control2=IDC_PROXY_SET,button,1342275584
Control3=IDCANCEL,button,1342275584
Control4=IDC_COMBO_SERVER,combobox,1344339971
Control5=IDC_EDIT_USER,edit,1342242944
Control6=IDC_EDIT_PASSWD,edit,1342242976
Control7=IDC_CHECK_SAVEPASSWD,button,1342275587
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STARTUPPROGRESS,msctls_progress32,1073741825
Control12=IDC_STATICINFO,static,1342308352
Control13=IDC_STARTUPPICTURE,static,1342177294

[DLG:IDD_EDITSERVER]
Type=1
Class=CEditSrvDlg
ControlCount=15
Control1=IDC_EDIT_NAME,edit,1342242944
Control2=IDC_EDIT_ADDRESS,edit,1342242944
Control3=IDC_EDIT_PORT,edit,1342251136
Control4=IDC_EDIT_USER,edit,1342242944
Control5=IDC_EDIT_PASSWD,edit,1342242976
Control6=IDC_CHECK_SAVEPASSWD,button,1342275587
Control7=IDOK,button,1342275585
Control8=IDCANCEL,button,1342275584
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,static,1342177287

[DLG:IDD_PROXYSET]
Type=1
Class=CProxySetDlg
ControlCount=18
Control1=IDC_DIRECT,button,1342210057
Control2=IDC_PRECONFIG,button,1342210057
Control3=IDC_PROXY,button,1342210057
Control4=IDC_PROXY_SOCKS4,button,1342341129
Control5=IDC_PROXY_SOCKS5,button,1342210057
Control6=IDC_PROXY_HTTP,button,1342210057
Control7=IDC_EDIT_PROXYADDRESS,edit,1342242944
Control8=IDC_EDIT_PROXYPORT,edit,1342251136
Control9=IDC_EDIT_PROXYUSER,edit,1342242944
Control10=IDC_EDIT_PROXYPASSWD,edit,1342242976
Control11=IDOK,button,1342275585
Control12=IDCANCEL,button,1342275584
Control13=IDC_STATIC_PROXYPORT,static,1342308352
Control14=IDC_STATIC_PROXYADDRESS,static,1342308352
Control15=65535,static,1342177287
Control16=65535,button,1342177287
Control17=IDC_STATIC_PROXYPASSWD,static,1342308352
Control18=IDC_STATIC_PROXYUSER,static,1342308352

[DLG:IDD_SELECTENGINE]
Type=1
Class=CSelectEngineDlg
ControlCount=6
Control1=IDC_LISTENGINE,listbox,1344340305
Control2=IDC_CHECKAUTOSTART,button,1342242819
Control3=IDOK,button,1342275585
Control4=IDC_SETUPRECEIVER,button,1342275584
Control5=IDCANCEL,button,1342275584
Control6=65535,static,1342177287

[DLG:IDD_SELECTENGINE (English (U.S.))]
Type=1
Class=?
ControlCount=6
Control1=IDC_LISTENGINE,listbox,1344340305
Control2=IDC_CHECKAUTOSTART,button,1342242819
Control3=IDOK,button,1342275585
Control4=IDC_SETUPRECEIVER,button,1342275584
Control5=IDCANCEL,button,1342275584
Control6=65535,static,1342177287

[DLG:IDD_PROXYSET (English (U.S.))]
Type=1
Class=?
ControlCount=18
Control1=IDC_DIRECT,button,1342210057
Control2=IDC_PRECONFIG,button,1342210057
Control3=IDC_PROXY,button,1342210057
Control4=IDC_PROXY_SOCKS4,button,1342341129
Control5=IDC_PROXY_SOCKS5,button,1342210057
Control6=IDC_PROXY_HTTP,button,1342210057
Control7=IDC_EDIT_PROXYADDRESS,edit,1342242944
Control8=IDC_EDIT_PROXYPORT,edit,1342251136
Control9=IDC_EDIT_PROXYUSER,edit,1342242944
Control10=IDC_EDIT_PROXYPASSWD,edit,1342242976
Control11=IDOK,button,1342275585
Control12=IDCANCEL,button,1342275584
Control13=IDC_STATIC_PROXYPORT,static,1342308352
Control14=IDC_STATIC_PROXYADDRESS,static,1342308352
Control15=65535,static,1342177287
Control16=65535,button,1342177287
Control17=IDC_STATIC_PROXYPASSWD,static,1342308352
Control18=IDC_STATIC_PROXYUSER,static,1342308352

[DLG:IDD_EDITSERVER (English (U.S.))]
Type=1
Class=?
ControlCount=15
Control1=IDC_EDIT_NAME,edit,1342242944
Control2=IDC_EDIT_ADDRESS,edit,1342242944
Control3=IDC_EDIT_PORT,edit,1342251136
Control4=IDC_EDIT_USER,edit,1342242944
Control5=IDC_EDIT_PASSWD,edit,1342242976
Control6=IDC_CHECK_SAVEPASSWD,button,1342275587
Control7=IDOK,button,1342275585
Control8=IDCANCEL,button,1342275584
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,static,1342177287

[DLG:IDD_STARTUP (English (U.S.))]
Type=1
Class=?
ControlCount=13
Control1=IDOK,button,1342275585
Control2=IDC_PROXY_SET,button,1342275584
Control3=IDCANCEL,button,1342275584
Control4=IDC_COMBO_SERVER,combobox,1344339971
Control5=IDC_EDIT_USER,edit,1342242944
Control6=IDC_EDIT_PASSWD,edit,1342242976
Control7=IDC_CHECK_SAVEPASSWD,button,1342275587
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STARTUPPROGRESS,msctls_progress32,1073741825
Control12=IDC_STATICINFO,static,1342308352
Control13=IDC_STARTUPPICTURE,static,1342177294

