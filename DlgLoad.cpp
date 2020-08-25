// DlgLoad.cpp : 实现文件
//

#include "stdafx.h"
#include "yb.h"
#include "DlgLoad.h"
#include "DBOperation.h"

int Global_Flag = 0;

// CDlgLoad 对话框

IMPLEMENT_DYNCREATE(CDlgLoad, CDHtmlDialog)

CDlgLoad::CDlgLoad(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CDlgLoad::IDD, CDlgLoad::IDH, pParent)
	, m_myname(_T(""))
	, m_mypassword(_T(""))
{

}

CDlgLoad::~CDlgLoad()
{
}

void CDlgLoad::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_myname);
	DDX_Text(pDX, IDC_EDIT2, m_mypassword);
}

BOOL CDlgLoad::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CDlgLoad, CDHtmlDialog)
	ON_BN_CLICKED(IDOK, &CDlgLoad::OnBnClickedOk)
	ON_BN_CLICKED(IDC_EDITPW, &CDlgLoad::OnBnClickedEditpw)
	ON_BN_CLICKED(IDC_MA, &CDlgLoad::OnBnClickedMa)
	ON_BN_CLICKED(IDC_JI, &CDlgLoad::OnBnClickedJi)
END_MESSAGE_MAP()

// CDlgLoad 消息处理程序

void CDlgLoad::OnBnClickedOk()
{
	UpdateData(true);
	// TODO: 在此添加控件通知处理程序代码
	DBOperation myDBOperation;
	if (Global_Flag == 0) {
		MessageBox(TEXT("请选择用户类型"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
	}
	else if (Global_Flag == 1) {
		if (myDBOperation.loadmanager(m_myname, m_mypassword)) {
			ShowWindow(SW_HIDE);
			Manager dlg;
			if (IDCANCEL == dlg.DoModal())
				EndDialog(-1);
			else
				ShowWindow(SW_SHOW);
		}

		else
			::AfxMessageBox("用户名密码错误！");
	}
	else {
		if (myDBOperation.loadjishi(m_myname, m_mypassword)) {
			OnOK();
		}
		else
			::AfxMessageBox("用户名密码错误！");
	}
}


void CDlgLoad::OnBnClickedEditpw()
{
	UpdateData(true);
	// TODO: 在此添加控件通知处理程序代码
	DBOperation myDBOperation;
	if (Global_Flag == 1) {
		if (myDBOperation.editmanager(m_myname, m_mypassword))
			::AfxMessageBox("管理员密码修改成功！");
		else
			::AfxMessageBox("管理员密码修改失败！");
	}
	else if (Global_Flag == 2) {
		if (myDBOperation.editjishi(m_myname, m_mypassword))
			::AfxMessageBox("用户名密码修改成功！");
		else
			::AfxMessageBox("用户名密码修改失败！");
	}
}

void CDlgLoad::OnBnClickedMa()
{
	Global_Flag = 1;
}


void CDlgLoad::OnBnClickedJi()
{
	Global_Flag = 2;
}