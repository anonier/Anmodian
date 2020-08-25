
// ybDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "DBOperation.h"
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <statement.h>
#include "HexEdit.h"
using namespace sql;
using namespace std;


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CybDlg �Ի���
class CybDlg : public CDialog
{
// ����
public:
	CybDlg(CWnd* pParent = NULL);	// ��׼���캯��
	HINSTANCE m_hInstMaster;
// �Ի�������
	enum { IDD = IDD_MYPCMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	CString m_vno;
	CString m_vname;
	CString m_vmon;
	CString m_vop;
	CListCtrl m_mylist;
	DBOperation mydboperation;
	int m_nSelected;
	void Flashlist(void);
	CppSQLite3DB db;
	afx_msg void OnBnClickedSelect();
	afx_msg void OnBnClickedDell();
	afx_msg void OnNMClickListStudent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedNet();
	afx_msg void OnBnClickedButtonAdd();
	sql::mysql::MySQL_Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::ResultSet* res;
	sql::Savepoint* savept;
	afx_msg void OnBnClickedButtonFlash();
	afx_msg void OnBnClickedButtonupdate();

	CHexEdit m_edit_serial;
	CHexEdit m_edit_key;
	CHexEdit m_edit_data;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonHalt();
	afx_msg void OnCbnSelchangeComboRaud();

};
