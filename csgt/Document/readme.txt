

    试用版下载: http://www.ninebulls.com/

    联系方式: support@ninebulls.com

常见问题：

1。源代码经编程后产生的目标文件执行后显示为试用版，这样是否正常？如何切换成专业版？

    显示为评估版是正常的，注册后即切换成专业版。
    Tools工程包含注册机，Tools在执行的时候需要StkLib.dll,StkNet.dll,GuiLib.dll,
XPFace.dll四个动态库。
    如果想在编译后直接为专业版，请修改一下StkNet\Include\Secret.inl 的函数
CSView::ValidProf(...),在函数开头直接加入一行 return TRUE;
    如果想将版本定义为“专家版”，“飓风版”等，请修改StkNet/Src/AfxCore.cpp文件中的
CString AfxGetVersionString( )方法。

2。如何处理安装步骤？

    软件执行环境需要的档案在Install\Setup目录下。执行环境主要包括一些股票初始数据
以及编译后StkUI\release\下的以下文件：
    StkLib.dll  StkNet.dll GuiLib.dll ProfUIS.dll XPFace.dll StockAna.exe
    以及Unzip.dll（该dll为解zip包用，目前无源代码，缺了这个dll也没有关系，不影响软
件的执行和使用，只是不能自动解zip包了）。
    另外，需要mfc42.dll，msvcrt.dll两个MFC库。

3。如果您想改成自有商标的软件，需要改以下部分，以免与策略为王软件发生冲突。

	0. 必改：Src\StkLib\Include\StkLib.h 中的一些OEM版常量定义
	1. 必改：软件策略文件后缀名称

	2. 修改：软件名称，软件开发商，www.zlinker.com，资讯网站
	3. 去掉或修改：菜单中的自动升级去掉，如何购买去掉，产品信息网站去掉
	4. 去掉或修改：所有如何购买按钮，立即网上购买按钮
	5. 修改帮助中软件名称，软件开发商，去掉购买页，去掉所有图，去掉十大特色，去
掉每页上面的链接
	6. 修改软件商标，软件图
	7. 修改Readme.txt, License.txt

4。如何修改软件缺省界面风格？

    修改文件Src\StkLib\Src\Profile.cpp中的构造方法
	CColorClassContainer::CColorClassContainer( )
    中的m_nSelectedColorClass	=	1一行，可以将m_nSelectedColorClass赋值为0，
1，2，3，分别代表不同的缺省颜色风格。

    对于每种缺省风格的颜色配置，可以修改以下方法的相应赋值，进行修改
	void CColorClass::SetColorDefault( )
	void CColorClass::SetColorStandard( )
	void CColorClass::SetColorByzantinesque( )
	void CColorClass::SetColorGothic( )
	void CColorClass::SetColorBaroque( )

    修改文件Src\StkUI\StkUI.cpp方法CStkUIApp::InitInstance()如下代码：
	AfxSwitchToStaticView( RUNTIME_CLASS(CSListView) );
    即可改变软件启动首页

5。执行实时行情接收时出现“无法加载接收系统”，是否缺了Stock.dll？实时行情接收除了通视
以外，还能用哪些数据源？

    实时行情可以使用通视卡接口和清华永新卡接口。目前通视接口差不多成为标准。
    Stock.dll是通视接口的库，需要安装通视卡或者兼容通视的网络软件才会有。

    通视卡是接收有线电视网络广播的股票数据的硬件接收卡，Stock.dll是它的驱动程序。以上软
件功能是将Internet服务器数据转换成通视接口数据。
   
    硬件接收卡的缺点是被动接收，使用以上软件时，如果需要历史数据，需要手工下载。

    如果以上接口不能满足您的要求，您可能需要开发股票实时行情服务器。

6。除权文件格式的如何更新？

    沪深除权信息存放在文件 data\CHNA.pwr ，CHNA.pwr 与分析家（http://www.fxj.com.cn）
    除权文件格式相同。

    另外，用户运行“数据管理”修改后的除权信息保存在data\shase\xdrdata\ 和 
    data\sznse\xdrdata下，每个股票一个文件，格式为 struct drdata_t 的数组。

    除权资料更新可以在软件“历史下载”中自动下载更新，也可以导入分析家除权文件。

7。财务资料文件的如何更新？

    沪深财务信息存放在文件 data\CHNA.bst 文件， CHNA.bst 文件为 struct basedata_t 的
    数组。

    财务资料更新可以在软件“历史下载”中自动下载更新，也可以导入通达信软件目录下的
    base.dbf 文件自行更新。

    通达信软件下载：http://www.sostock.com.cn/hq/ 下载超强行情软件。

8。注册机如何使注册码与以前的软件不同？

    修改StkLib\Include\StkLib.h中，以下四个常量的定义并重新编译后，注册码即变得完全不同，
    注意长度不要大于5

      #define OEM_SEED_STD1    "dc./x"             // 标准版注册机种子1
      #define OEM_SEED_STD2    "sfk%c"             // 标准版注册机种子2
      #define OEM_SEED_PROF1   "78cd2"             // 专业版注册机种子1
      #define OEM_SEED_PROF2   "d6$3#"             // 专业版注册机种子2

    同时要去掉以下定义的注释
      #define	CLKVER_OEM		// OEM版去掉该行注释
