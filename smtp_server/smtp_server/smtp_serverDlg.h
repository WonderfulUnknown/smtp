
// smtp_serverDlg.h : ͷ�ļ�
//

#pragma once
#include "MySocket.h"
#include "afxwin.h"
#include "afxcmn.h"

// Csmtp_serverDlg �Ի���
class Csmtp_serverDlg : public CDialogEx
{
// ����
public:
	Csmtp_serverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMTP_SERVER_DIALOG };
#endif

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
	MySocket MySock;
	CEdit m_info;
	CEdit m_log;
};
