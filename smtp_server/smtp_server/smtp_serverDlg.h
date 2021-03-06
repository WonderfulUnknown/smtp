
// smtp_serverDlg.h : 头文件
//

#pragma once
#include "MySocket.h"
#include "afxwin.h"
#include "afxcmn.h"

// Csmtp_serverDlg 对话框
class Csmtp_serverDlg : public CDialogEx
{
// 构造
public:
	Csmtp_serverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMTP_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	MySocket MySock;
	CEdit m_info;
	CEdit m_log;
	CStatic m_Bmp;
	CString m_Content;
	CStatic m_picture;
	afx_msg void OnBnClickedShowpicture();
};
