#pragma once
#include "ybDlg.h"


#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 
#include "Manager.h"


// CDlgLoad 对话框

class CDlgLoad : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDlgLoad)

public:
	CDlgLoad(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLoad();
	
	CybDlg* OneDlg;
	Manager* TwoDlg;

// 对话框数据
	enum { IDD = IDD_LOAD, IDH = IDR_HTML_DLGLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_myname="1";
	CString m_mypassword="1";
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedEditpw();

	afx_msg void OnBnClickedMa();
	afx_msg void OnBnClickedJi();
};
