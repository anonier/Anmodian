// DlgLoad.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "yb.h"
#include "DlgLoad.h"
#include "DBOperation.h"

int Global_Flag = 0;

// CDlgLoad �Ի���

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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CDlgLoad, CDHtmlDialog)
	ON_BN_CLICKED(IDOK, &CDlgLoad::OnBnClickedOk)
	ON_BN_CLICKED(IDC_EDITPW, &CDlgLoad::OnBnClickedEditpw)
	ON_BN_CLICKED(IDC_MA, &CDlgLoad::OnBnClickedMa)
	ON_BN_CLICKED(IDC_JI, &CDlgLoad::OnBnClickedJi)
END_MESSAGE_MAP()

// CDlgLoad ��Ϣ�������

void CDlgLoad::OnBnClickedOk()
{
	UpdateData(true);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DBOperation myDBOperation;
	if (Global_Flag == 0) {
		MessageBox(TEXT("��ѡ���û�����"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
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
			::AfxMessageBox("�û����������");
	}
	else {
		if (myDBOperation.loadjishi(m_myname, m_mypassword)) {
			OnOK();
		}
		else
			::AfxMessageBox("�û����������");
	}
}


void CDlgLoad::OnBnClickedEditpw()
{
	UpdateData(true);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DBOperation myDBOperation;
	if (Global_Flag == 1) {
		if (myDBOperation.editmanager(m_myname, m_mypassword))
			::AfxMessageBox("����Ա�����޸ĳɹ���");
		else
			::AfxMessageBox("����Ա�����޸�ʧ�ܣ�");
	}
	else if (Global_Flag == 2) {
		if (myDBOperation.editjishi(m_myname, m_mypassword))
			::AfxMessageBox("�û��������޸ĳɹ���");
		else
			::AfxMessageBox("�û��������޸�ʧ�ܣ�");
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