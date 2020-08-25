#pragma once

#include "afxcmn.h"
#include "DBOperation.h"
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <statement.h>  
using namespace sql;
using namespace std;
// Manager 对话框

class Manager : public CDialog
{
	DECLARE_DYNAMIC(Manager)

public:
	Manager(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Manager();
protected:
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANAGER };
#endif


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_jname;
	CString m_jno;
	CString m_jmo;
	CString m_jpa;
	DBOperation mydboperation;
	int m_nSelected;
	sql::mysql::MySQL_Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::ResultSet* res;
	sql::Savepoint* savept;
	CppSQLite3DB db;
	void Flashlist(void);
	afx_msg void OnBnClickedButtonJishiadd();
	afx_msg void OnBnClickedJishidell();
	afx_msg void OnBnClickedButtonJishiupdate();
	afx_msg void OnBnClickedButtonJishisel();
	afx_msg void OnBnClickedButtonJishiflash();
	afx_msg void OnBnClickedJishinet();
	CListCtrl j_list;
	afx_msg void OnNMClickListJishi(NMHDR* pNMHDR, LRESULT* pResult);
};
