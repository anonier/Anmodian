// Manager.cpp: 实现文件
//

#include "stdafx.h"
#include "yb.h"
#include "Manager.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Manager 对话框

IMPLEMENT_DYNAMIC(Manager, CDialog)

Manager::Manager(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MANAGER, pParent)
	, m_jname(_T(""))
	, m_jno(_T(""))
	, m_jmo(_T(""))
	, m_jpa(_T(""))
{

}

Manager::~Manager()
{
}

void Manager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_JISHINAME, m_jname);
	DDX_Text(pDX, IDC_EDIT_JISHINO, m_jno);
	DDX_Text(pDX, IDC_EDIT_JISHIMO, m_jmo);
	DDX_Text(pDX, IDC_EDIT_JISHIPA, m_jpa);
	DDX_Control(pDX, IDC_LIST_JISHI, j_list);
}


BEGIN_MESSAGE_MAP(Manager, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_JISHIADD, &Manager::OnBnClickedButtonJishiadd)
	ON_BN_CLICKED(IDC_JISHiDELL, &Manager::OnBnClickedJishidell)
	ON_BN_CLICKED(IDC_BUTTON_JISHIupdate, &Manager::OnBnClickedButtonJishiupdate)
	ON_NOTIFY(NM_CLICK, IDC_LIST_JISHI, &Manager::OnNMClickListJishi)
	ON_BN_CLICKED(IDC_BUTTON_JISHISEL, &Manager::OnBnClickedButtonJishisel)
	ON_BN_CLICKED(IDC_BUTTON_JISHIFLASH, &Manager::OnBnClickedButtonJishiflash)
	ON_BN_CLICKED(IDC_JISHINET, &Manager::OnBnClickedJishinet)

END_MESSAGE_MAP()

BOOL Manager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	m_nSelected = -1;
	//2.初始化表头
	j_list.InsertColumn(1, _T("姓名"), LVCFMT_LEFT, 85, -1);
	j_list.InsertColumn(2, _T("金额"), LVCFMT_LEFT, 85, -1);
	j_list.InsertColumn(3, _T("密码"), LVCFMT_LEFT, 85, -1);
	j_list.InsertColumn(0, _T("学号"), LVCFMT_LEFT, 85, -1);
	j_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	//刷新所有数据
	mydboperation.selalljishi();
	Flashlist();
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void Manager::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR Manager::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// Manager 消息处理程序
void Manager::Flashlist(void)
{
	j_list.DeleteAllItems();
	POSITION pos = mydboperation.m_jishilist.GetHeadPosition();
	for (int i = 0; i < mydboperation.m_jishilist.GetCount(); i++)
	{
		mydboperation.m_jishi = mydboperation.m_jishilist.GetNext(pos);
		j_list.InsertItem(i, mydboperation.m_jishi.userid, -1);
		j_list.SetItemText(i, 1, mydboperation.m_jishi.name);
		j_list.SetItemText(i, 2, mydboperation.m_jishi.money);
		j_list.SetItemText(i, 3, mydboperation.m_jishi.password);
	}
}

void Manager::OnBnClickedButtonJishiadd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString strDbPath;
	strDbPath = _T("dboperation.db");
	//打开或新建一个数据库
	db.open(strDbPath);
	driver = sql::mysql::get_mysql_driver_instance();
	//防止网络堵塞
	try
	{
		con = driver->connect("tcp://rdsc70m84m457732q4q5.mysql.rds.aliyuncs.com:3306", "rfid", "Rfid123456");

	}
	catch (SQLException & e)
	{
		return;
	}
	stmt = con->createStatement();
	stmt->execute("USE student");
	stmt->execute("set names 'gbk'");
	CString StrSql;
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象
	CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
	StrSql.Format(_T("insert into server(userid,type,money,time,serverid) values('%s','%s','%s','%s','%s')"), m_jno, "增加技师", NULL, m_strTime, "jishi");
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return; }
	StrSql.Format(_T("insert into jishi(userid,name,money,password) values('%s','%s','%s','%s')"), m_jno, m_jname, m_jmo, m_jpa);
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return; }
	db.execDML(StrSql);
	mydboperation.selalljishi();
	Flashlist();
}


void Manager::OnBnClickedJishidell()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	DBOperation myDBOperation;
	if (myDBOperation.delljishi(m_jno))
		::AfxMessageBox("删除成功！");
	else
		::AfxMessageBox("删除失败！");
	mydboperation.selalljishi();
	Flashlist();
}


void Manager::OnBnClickedButtonJishiupdate()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	// TODO: 在此添加控件通知处理程序代码
	DBOperation myDBOperation;
	if (myDBOperation.updatejishi(m_jname, m_jmo, m_jpa, m_jno))
		::AfxMessageBox("修改成功！");
	else
		::AfxMessageBox("修改失败！");
	mydboperation.selalljishi();
	Flashlist();
}


void Manager::OnBnClickedButtonJishisel()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	mydboperation.seljishi(m_jno);
	Flashlist();
}


void Manager::OnBnClickedButtonJishiflash()
{
	// TODO: 在此添加控件通知处理程序代码
	mydboperation.selalljishi();
	Flashlist();
}


void Manager::OnBnClickedJishinet()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUT_AUTOCONN)->SetWindowText("同步中请等待...");
	driver = sql::mysql::get_mysql_driver_instance();
	//防止网络堵塞
	try
	{
		con = driver->connect("tcp://rdsc70m84m457732q4q5.mysql.rds.aliyuncs.com:3306", "rfid", "Rfid123456");

	}
	catch (SQLException & e)
	{
		GetDlgItem(IDC_BUT_AUTOCONN)->SetWindowText("网络错误请再次同步");
		return;
	}
	stmt = con->createStatement();
	stmt->execute("USE student");
	stmt->execute("set names 'gbk'");
	CString StrSql;

	//同步服务器数据
	mydboperation.selalljishi();
	POSITION pos = mydboperation.m_jishilist.GetHeadPosition();
	for (int i = 0; i < mydboperation.m_jishilist.GetCount(); i++)
	{
		mydboperation.m_jishi = mydboperation.m_jishilist.GetNext(pos);
		j_list.InsertItem(i, mydboperation.m_jishi.userid, -1);
		j_list.SetItemText(i, 1, mydboperation.m_jishi.name);
		j_list.SetItemText(i, 1, mydboperation.m_jishi.money);
		j_list.SetItemText(i, 1, mydboperation.m_jishi.password);

		StrSql.Format(_T("select * from jishi where name='%s'"), mydboperation.m_jishi.name);
		//防止网络堵塞
		try { res = stmt->executeQuery(StrSql.GetBuffer(0)); }
		catch (SQLException & e) { continue; }
		if (res->next())
		{
			StrSql.Format(_T("update jishi set userid='%s' where name='%s'"), mydboperation.m_jishi.userid, mydboperation.m_jishi.name);
			//防止网络堵塞
			try { stmt->execute(StrSql.GetBuffer(0)); }
			catch (SQLException & e) { continue; }
		}
		else
		{
			StrSql.Format(_T("insert into jishi(userid,name,money,password) values('%s','%s','%s','%s')"), mydboperation.m_jishi.userid, mydboperation.m_jishi.name, mydboperation.m_jishi.money, mydboperation.m_jishi.password);
			//防止网络堵塞
			try { stmt->execute(StrSql.GetBuffer(0)); }
			catch (SQLException & e) { continue; }
		}
	}

	//同步数据到客户端

	StrSql.Format(_T("select * from jishi"));
	//防止网络堵塞
	try { res = stmt->executeQuery(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return; }
	mydboperation.dellALLjsihi();
	while (res->next())
	{
		mydboperation.m_jishi.userid = res->getString("userid").c_str();
		mydboperation.m_jishi.name = res->getString("name").c_str();
		mydboperation.m_jishi.money = res->getString("money").c_str();
		mydboperation.m_jishi.password = res->getString("password").c_str();
		mydboperation.addjishi(mydboperation.m_jishi.userid, mydboperation.m_jishi.name, mydboperation.m_jishi.money, mydboperation.m_jishi.password);
	}

	delete res;
	delete stmt;
	delete con;

	GetDlgItem(IDC_BUT_AUTOCONN)->SetWindowText("手动同步服务器");

	//刷新所有数据
	mydboperation.selalljishi();
	Flashlist();
}


void Manager::OnNMClickListJishi(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//选中一条记录
	if (j_list.GetSelectionMark() != -1)
	{
		m_nSelected = j_list.GetSelectionMark();
		m_jname = j_list.GetItemText(m_nSelected, 1);
		m_jno = j_list.GetItemText(m_nSelected, 0);
		m_jpa = j_list.GetItemText(m_nSelected, 3);
		m_jmo = j_list.GetItemText(m_nSelected, 2);
		UpdateData(false);
	}
	*pResult = 0;
}
