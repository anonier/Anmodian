
// ybDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "yb.h"
#include "ybDlg.h"
#include "afxdialogex.h"
#include "DbOperation.h"
#include "ExportFunc.h"
#include "HexEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define BAUD_NUMBER  7
int BaudValue[BAUD_NUMBER] = { 9600,14400,19200,28800,38400,57600,115200 };

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CybDlg 对话框


CybDlg::CybDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CybDlg::IDD, pParent)
	, m_vno(_T(""))
	, m_vname(_T(""))
	, m_vmon(_T(""))
	, m_vop(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hInstMaster = NULL;
}

void CybDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NO, m_vno);
	DDX_Text(pDX, IDC_EDIT_MYNAME, m_vname);
	DDX_Control(pDX, IDC_LIST_STUDENT, m_mylist);
	DDX_Text(pDX, IDC_EDIT_MO, m_vmon);
	DDX_Text(pDX, IDC_EDIT_OP, m_vop);

	DDX_Control(pDX, IDC_EDIT_SERIAL, m_edit_serial);
	DDX_Control(pDX, IDC_EDIT_KEY, m_edit_key);
	DDX_Control(pDX, IDC_EDIT_DATA, m_edit_data);
}

BEGIN_MESSAGE_MAP(CybDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CybDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SEL, &CybDlg::OnBnClickedSelect)
	ON_BN_CLICKED(IDDELL, &CybDlg::OnBnClickedDell)
	ON_NOTIFY(NM_CLICK, IDC_LIST_STUDENT, &CybDlg::OnNMClickListStudent)
	ON_BN_CLICKED(ID_NET, &CybDlg::OnBnClickedNet)
	ON_BN_CLICKED(IDC_BUTTON_FLASH, &CybDlg::OnBnClickedButtonFlash)
	ON_BN_CLICKED(IDC_BUTTON_update, &CybDlg::OnBnClickedButtonupdate)

	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CybDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CybDlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CybDlg::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_HALT, &CybDlg::OnBnClickedButtonHalt)
	ON_CBN_SELCHANGE(IDC_COMBO_RAUD, &CybDlg::OnCbnSelchangeComboRaud)

END_MESSAGE_MAP()


// CybDlg 消息处理程序

BOOL CybDlg::OnInitDialog()
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
	m_mylist.InsertColumn(1, _T("姓名"), LVCFMT_LEFT, 85, -1);
	m_mylist.InsertColumn(2, _T("金额"), LVCFMT_LEFT, 85, -1);
	m_mylist.InsertColumn(3, _T("操作"), LVCFMT_LEFT, 85, -1);
	m_mylist.InsertColumn(0, _T("学号"), LVCFMT_LEFT, 85, -1);
	m_mylist.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	//刷新所有数据
	mydboperation.selallstudent();
	Flashlist();
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	TCHAR szBuf[MAX_PATH];
	GetModuleFileName(NULL, (LPTSTR)szBuf, MAX_PATH);
	*strrchr(szBuf, '\\') = 0;
	strcat_s(szBuf, _T("\\MasterRD.dll"));

	m_hInstMaster = LoadLibrary(szBuf);

	if (m_hInstMaster) {
		(FARPROC&)lib_ver = GetProcAddress(m_hInstMaster, _T("lib_ver"));
		(FARPROC&)des_encrypt = GetProcAddress(m_hInstMaster, _T("des_encrypt"));
		(FARPROC&)des_decrypt = GetProcAddress(m_hInstMaster, _T("des_decrypt"));
		(FARPROC&)rf_init_com = GetProcAddress(m_hInstMaster, _T("rf_init_com"));
		(FARPROC&)rf_init_device_number = GetProcAddress(m_hInstMaster, _T("rf_init_device_number"));
		(FARPROC&)rf_get_device_number = GetProcAddress(m_hInstMaster, _T("rf_get_device_number"));
		(FARPROC&)rf_get_model = GetProcAddress(m_hInstMaster, _T("rf_get_model"));
		(FARPROC&)rf_get_snr = GetProcAddress(m_hInstMaster, _T("rf_get_snr"));
		(FARPROC&)rf_beep = GetProcAddress(m_hInstMaster, _T("rf_beep"));
		(FARPROC&)rf_init_sam = GetProcAddress(m_hInstMaster, _T("rf_init_sam"));
		(FARPROC&)rf_sam_rst = GetProcAddress(m_hInstMaster, _T("rf_sam_rst"));
		(FARPROC&)rf_sam_cos = GetProcAddress(m_hInstMaster, _T("rf_sam_cos"));
		(FARPROC&)rf_init_type = GetProcAddress(m_hInstMaster, _T("rf_init_type"));
		(FARPROC&)rf_antenna_sta = GetProcAddress(m_hInstMaster, _T("rf_antenna_sta"));
		(FARPROC&)rf_request = GetProcAddress(m_hInstMaster, _T("rf_request"));
		(FARPROC&)rf_anticoll = GetProcAddress(m_hInstMaster, _T("rf_anticoll"));
		(FARPROC&)rf_select = GetProcAddress(m_hInstMaster, _T("rf_select"));
		(FARPROC&)rf_halt = GetProcAddress(m_hInstMaster, _T("rf_halt"));
		(FARPROC&)rf_download_key = GetProcAddress(m_hInstMaster, _T("rf_download_key"));
		(FARPROC&)rf_M1_authentication1 = GetProcAddress(m_hInstMaster, _T("rf_M1_authentication1"));
		(FARPROC&)rf_M1_authentication2 = GetProcAddress(m_hInstMaster, _T("rf_M1_authentication2"));
		(FARPROC&)rf_M1_read = GetProcAddress(m_hInstMaster, _T("rf_M1_read"));
		(FARPROC&)rf_M1_write = GetProcAddress(m_hInstMaster, _T("rf_M1_write"));
		(FARPROC&)rf_M1_initval = GetProcAddress(m_hInstMaster, _T("rf_M1_initval"));
		(FARPROC&)rf_M1_readval = GetProcAddress(m_hInstMaster, _T("rf_M1_readval"));
		(FARPROC&)rf_M1_decrement = GetProcAddress(m_hInstMaster, _T("rf_M1_decrement"));
		(FARPROC&)rf_M1_increment = GetProcAddress(m_hInstMaster, _T("rf_M1_increment"));
		(FARPROC&)rf_M1_restore = GetProcAddress(m_hInstMaster, _T("rf_M1_restore"));
		(FARPROC&)rf_M1_transfer = GetProcAddress(m_hInstMaster, _T("rf_M1_transfer"));
		(FARPROC&)rf_typea_rst = GetProcAddress(m_hInstMaster, _T("rf_typea_rst"));
		(FARPROC&)rf_cos_command = GetProcAddress(m_hInstMaster, _T("rf_cos_command"));
		(FARPROC&)rf_atqb = GetProcAddress(m_hInstMaster, _T("rf_atqb"));
		(FARPROC&)rf_attrib = GetProcAddress(m_hInstMaster, _T("rf_attrib"));
		(FARPROC&)rf_typeb_cos = GetProcAddress(m_hInstMaster, _T("rf_typeb_cos"));
		(FARPROC&)rf_hltb = GetProcAddress(m_hInstMaster, _T("rf_hltb"));
		(FARPROC&)rf_at020_check = GetProcAddress(m_hInstMaster, _T("rf_at020_check"));
		(FARPROC&)rf_at020_read = GetProcAddress(m_hInstMaster, _T("rf_at020_read"));
		(FARPROC&)rf_at020_write = GetProcAddress(m_hInstMaster, _T("rf_at020_write"));
		(FARPROC&)rf_at020_lock = GetProcAddress(m_hInstMaster, _T("rf_at020_lock"));
		(FARPROC&)rf_at020_count = GetProcAddress(m_hInstMaster, _T("rf_at020_count"));
		(FARPROC&)rf_at020_deselect = GetProcAddress(m_hInstMaster, _T("rf_at020_deselect"));
		(FARPROC&)rf_light = GetProcAddress(m_hInstMaster, _T("rf_light"));
		(FARPROC&)rf_ClosePort = GetProcAddress(m_hInstMaster, _T("rf_ClosePort"));
		(FARPROC&)rf_GetErrorMessage = GetProcAddress(m_hInstMaster, _T("rf_GetErrorMessage"));



		if (NULL == lib_ver ||
			NULL == des_encrypt ||
			NULL == des_decrypt ||
			NULL == rf_init_com ||
			NULL == rf_init_device_number ||
			NULL == rf_get_device_number ||
			NULL == rf_get_model ||
			NULL == rf_beep ||
			NULL == rf_init_sam ||
			NULL == rf_sam_rst ||
			NULL == rf_sam_cos ||
			NULL == rf_init_type ||
			NULL == rf_antenna_sta ||
			NULL == rf_request ||
			NULL == rf_anticoll ||
			NULL == rf_select ||
			NULL == rf_halt ||
			NULL == rf_download_key ||
			NULL == rf_M1_authentication1 ||
			NULL == rf_M1_authentication2 ||
			NULL == rf_M1_read ||
			NULL == rf_M1_write ||
			NULL == rf_M1_initval ||
			NULL == rf_M1_readval ||
			NULL == rf_M1_decrement ||
			NULL == rf_M1_increment ||
			NULL == rf_M1_restore ||
			NULL == rf_M1_transfer ||
			NULL == rf_typea_rst ||
			NULL == rf_cos_command ||
			NULL == rf_atqb ||
			NULL == rf_attrib ||
			NULL == rf_typeb_cos ||
			NULL == rf_hltb ||
			NULL == rf_at020_check ||
			NULL == rf_at020_read ||
			NULL == rf_at020_write ||
			NULL == rf_at020_lock ||
			NULL == rf_at020_count ||
			NULL == rf_at020_deselect ||
			NULL == rf_light ||
			NULL == rf_ClosePort ||
			NULL == rf_GetErrorMessage) {
			AfxMessageBox(_T("Load MasterRD.dll failed !"));

		}
	}
	else {
		AfxMessageBox(_T("Load MasterRD.dll failed !"));

	}


	//Choose KEY A
	((CButton*)GetDlgItem(IDC_RADIO_KEYA))->SetCheck(TRUE);


	CComboBox* pMassCBB = (CComboBox*)GetDlgItem(IDC_COMBO_MASS);
	CString str;

	for (int i = 0; i < 64; i++) {
		str.Format(_T("%d"), i);
		pMassCBB->AddString(str);
		pMassCBB->SetItemData(i, i);
	}

	pMassCBB->SetCurSel(0);
	///////////////////////////////
	//set edit control size
	m_edit_key.SetLimitTextEx(6);
	m_edit_data.SetLimitTextEx(16);

	//////////////////////////////
	//initialize 
	CComboBox* pPortCBB = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	for (int i = 0; i < 8; i++) {
		str.Format(_T("%d"), i + 1);
		pPortCBB->AddString(str);
		pPortCBB->SetItemData(i, i + 1);
	}

	pPortCBB->SetCurSel(0);

	CComboBox* pRaudCBB = (CComboBox*)GetDlgItem(IDC_COMBO_RAUD);
	for (int i = 0; i < BAUD_NUMBER; i++) {
		str.Format(_T("%d"), BaudValue[i]);
		pRaudCBB->AddString(str);
		pRaudCBB->SetItemData(i, BaudValue[i]);
	}
	pRaudCBB->SetCurSel(2);



	//open port 
	int state = 1;
	//port : com1
	//baud : 19200
	state = rf_init_com(1, 19200);

	if (state != LIB_SUCCESS) {
		rf_ClosePort();
		MessageBox("Open port error!", "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}









	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CybDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CybDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CybDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CybDlg::OnDestroy()
{
	CDialog::OnDestroy();

	//close port
	rf_ClosePort();
	//Release masterRD.dll
	if (m_hInstMaster) FreeLibrary(m_hInstMaster);
}

void CybDlg::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	WORD icdev = 0x0000;
	unsigned char mode = 0x52;
	int status;
	unsigned short TagType;
	unsigned char bcnt = 0x04;//mifare card use 0x04
	unsigned char Snr[MAX_RF_BUFFER];
	unsigned char len;
	unsigned char Size;

	status = rf_request(icdev, mode, &TagType);//search all card
	if (status) {//error
		m_edit_serial.SetWindowText("");
		return;
	}

	status = rf_anticoll(icdev, bcnt, Snr, &len);//return serial number of card
	if (status || len != 4) { //error
		m_edit_serial.SetWindowText("");
		return;
	}

	status = rf_select(icdev, Snr, len, &Size);//lock ISO14443-3 TYPE_A 
	if (status) {//error
		m_edit_serial.SetWindowText("");
		return;
	}

	m_edit_serial.SetWindowTextEx(Snr, len);
}

void CybDlg::OnBnClickedButtonRead()
{
	// TODO: 在此添加控件通知处理程序代码
	WORD icdev = 0x0000;
	unsigned char mode = 0x60;
	unsigned char secnr = 0x00;
	int state;
	CString strKey;
	unsigned char pData[MAX_RF_BUFFER];
	unsigned char cLen;

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_KEYA);
	if ((pButton->GetCheck())) {//key A
		mode = 0x60; //key value
	}
	else {//key B
		mode = 0x61; //key value
	}

	m_edit_key.GetWindowTextEx(strKey);
	if (m_edit_key.GetTextLenEx() != 6) {
		MessageBox(_T("Please input 6 bytes in key area"), _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	//choose block
	CComboBox* pCBB = (CComboBox*)GetDlgItem(IDC_COMBO_MASS);
	int nSel = pCBB->GetCurSel();
	secnr = (unsigned char)(pCBB->GetItemData(nSel));


	state = rf_M1_authentication2(icdev, mode, (secnr / 4) * 4, (unsigned char*)strKey.GetBuffer(strKey.GetLength()));
	if (state) {
		MessageBox(_T("authenticate key error"), _T("Error"), MB_OK | MB_ICONERROR);
		m_edit_data.SetWindowText("");
		return;
	}

	state = rf_M1_read(icdev, secnr, pData, &cLen);
	if (state || cLen != 16) {
		MessageBox(_T("Read card error"), _T("Error"), MB_OK | MB_ICONERROR);
		m_edit_data.SetWindowText("");
		return;
	}

	m_edit_data.SetWindowTextEx(pData, 16);
}

void CybDlg::OnBnClickedButtonWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	WORD icdev = 0x0000;
	unsigned char mode = 0x60;
	unsigned char secnr = 0x00;
	int state;
	CString strKey, strEdit;
	unsigned char Data[16];

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_KEYA);
	if ((pButton->GetCheck())) {//KEY A
		mode = 0x60; //KEY value		
	}
	else {//key B
		mode = 0x61; //key value
	}

	m_edit_key.GetWindowTextEx(strKey);
	if (m_edit_key.GetTextLenEx() != 6) {
		MessageBox(_T("Please input 6 bytes in key area"), _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	//choose block
	CComboBox* pCBB = (CComboBox*)GetDlgItem(IDC_COMBO_MASS);
	int nSel = pCBB->GetCurSel();
	secnr = (unsigned char)(pCBB->GetItemData(nSel));


	//======================	
	state = rf_M1_authentication2(icdev, mode, (secnr / 4) * 4, (unsigned char*)strKey.GetBuffer(strKey.GetLength()));
	if (state) {
		MessageBox(_T("authenticate key error"), _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}


	m_edit_data.GetWindowTextEx(strEdit);
	memcpy(Data, strEdit.GetBuffer(strEdit.GetLength()), strEdit.GetLength());
	//write mifare one
	state = rf_M1_write(icdev, secnr, Data);
	if (state) {
		MessageBox(_T("write card error"), _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}
}

void CybDlg::OnBnClickedButtonHalt()
{
	// TODO: 在此添加控件通知处理程序代码
	WORD icdev = 0x0000;
	int state;

	state = rf_halt(icdev);
	if (state) {
		MessageBox(_T("HALT error"), _T("Error"), MB_OK | MB_ICONERROR);
	}
}

void CybDlg::OnCbnSelchangeComboRaud()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox* pPortCBB = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	CComboBox* pBaudCBB = (CComboBox*)GetDlgItem(IDC_COMBO_RAUD);

	int nPort = pPortCBB->GetItemData(pPortCBB->GetCurSel());
	int nBaud = pBaudCBB->GetItemData(pBaudCBB->GetCurSel());

	//close port
	rf_ClosePort();

	//open
	int state = 7;

	state = rf_init_com(nPort, nBaud);

	if (state != LIB_SUCCESS) {
		rf_ClosePort();
		MessageBox("Open error!", "Error", MB_OK | MB_ICONERROR);
		return;
	}
}


void CybDlg::Flashlist(void)
{
	m_mylist.DeleteAllItems();
	POSITION pos = mydboperation.m_studentlist.GetHeadPosition();
	for (int i = 0; i < mydboperation.m_studentlist.GetCount(); i++)
	{
		mydboperation.m_mystudent = mydboperation.m_studentlist.GetNext(pos);
		m_mylist.InsertItem(i, mydboperation.m_mystudent.userid, -1);
		m_mylist.SetItemText(i, 1, mydboperation.m_mystudent.name);
		m_mylist.SetItemText(i, 2, mydboperation.m_mystudent.money);
		m_mylist.SetItemText(i, 3, mydboperation.m_mystudent.operation);
	}
}


void CybDlg::OnBnClickedButtonAdd()
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
	StrSql.Format(_T("insert into server(userid,type,money,time,serverid) values('%s','%s','%s','%s','%s')"), m_vno, "增加用户", NULL, m_strTime, "user");
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return; }
	StrSql.Format(_T("insert into user(userid,name,money,operation) values('%s','%s','%s','%s')"), m_vno, m_vname, m_vmon, m_vop);
	try { stmt->execute(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return; }
	db.execDML(StrSql);
	mydboperation.selallstudent();
	Flashlist();
}

void CybDlg::OnBnClickedSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	mydboperation.selstudent(m_vno);
	Flashlist();

}


void CybDlg::OnBnClickedDell()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	DBOperation myDBOperation;
	if (myDBOperation.dellstudent(m_vno))
		::AfxMessageBox("删除成功！");
	else
		::AfxMessageBox("删除失败！");
	mydboperation.selallstudent();
	Flashlist();

}

void CybDlg::OnBnClickedButtonupdate()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	// TODO: 在此添加控件通知处理程序代码
	DBOperation myDBOperation;
	if (myDBOperation.updatestudent(m_vname, m_vmon, m_vop, m_vno))
		::AfxMessageBox("修改成功！");
	else
		::AfxMessageBox("修改失败！");
	mydboperation.selallstudent();
	Flashlist();
}


void CybDlg::OnNMClickListStudent(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//选中一条记录
	if (m_mylist.GetSelectionMark() != -1)
	{
		m_nSelected = m_mylist.GetSelectionMark();
		m_vname = m_mylist.GetItemText(m_nSelected,1);
		m_vno = m_mylist.GetItemText(m_nSelected, 0);
		m_vop = m_mylist.GetItemText(m_nSelected, 3);
		m_vmon = m_mylist.GetItemText(m_nSelected, 2);
		UpdateData(false);
	}
	*pResult = 0;
}


void CybDlg::OnBnClickedNet()
{
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
	mydboperation.selallstudent();
	POSITION pos = mydboperation.m_studentlist.GetHeadPosition();
	for (int i = 0; i < mydboperation.m_studentlist.GetCount(); i++)
	{
		mydboperation.m_mystudent = mydboperation.m_studentlist.GetNext(pos);
		m_mylist.InsertItem(i, mydboperation.m_mystudent.userid, -1);
		m_mylist.SetItemText(i, 1, mydboperation.m_mystudent.name);
		m_mylist.SetItemText(i, 1, mydboperation.m_mystudent.money);
		m_mylist.SetItemText(i, 1, mydboperation.m_mystudent.operation);

		StrSql.Format(_T("select * from user where name='%s'"), mydboperation.m_mystudent.name);
		//防止网络堵塞
		try { res = stmt->executeQuery(StrSql.GetBuffer(0)); }
		catch (SQLException & e) { continue; }
		if (res->next())
		{
			StrSql.Format(_T("update user set userid='%s' where name='%s'"), mydboperation.m_mystudent.userid, mydboperation.m_mystudent.name);
			//防止网络堵塞
			try { stmt->execute(StrSql.GetBuffer(0)); }
			catch (SQLException & e) { continue; }
		}
		else
		{
			StrSql.Format(_T("insert into user(userid,name,money,operation) values('%s','%s','%s','%s')"), mydboperation.m_mystudent.userid, mydboperation.m_mystudent.name, mydboperation.m_mystudent.money, mydboperation.m_mystudent.operation);
			//防止网络堵塞
			try { stmt->execute(StrSql.GetBuffer(0)); }
			catch (SQLException & e) { continue; }
		}
	}

	//同步数据到客户端

	StrSql.Format(_T("select * from user"));
	//防止网络堵塞
	try { res = stmt->executeQuery(StrSql.GetBuffer(0)); }
	catch (SQLException & e) { return; }
	mydboperation.dellALLstudent();
	while (res->next())
	{
		mydboperation.m_mystudent.userid = res->getString("userid").c_str();
		mydboperation.m_mystudent.name = res->getString("name").c_str();
		mydboperation.m_mystudent.money = res->getString("money").c_str();
		mydboperation.m_mystudent.operation = res->getString("operation").c_str();
		mydboperation.addstudent(mydboperation.m_mystudent.userid, mydboperation.m_mystudent.name, mydboperation.m_mystudent.money, mydboperation.m_mystudent.operation);
	}

	delete res;
	delete stmt;
	delete con;

	GetDlgItem(IDC_BUT_AUTOCONN)->SetWindowText("手动同步服务器");

	//刷新所有数据
	mydboperation.selallstudent();
	Flashlist();
}

void CybDlg::OnBnClickedButtonFlash()
{
	// TODO: 在此添加控件通知处理程序代码
		//刷新所有数据
	mydboperation.selallstudent();
	Flashlist();
}
