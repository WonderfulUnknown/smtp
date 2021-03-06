
// smtp_serverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "smtp_server.h"
#include "smtp_serverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Csmtp_serverDlg 对话框



Csmtp_serverDlg::Csmtp_serverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SMTP_SERVER_DIALOG, pParent)
	, m_Content(_T(""))
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Csmtp_serverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO, m_info);
	DDX_Control(pDX, IDC_Log, m_log);
	DDX_Text(pDX, IDC_Content, m_Content);
	DDX_Control(pDX, IDC_Picture, m_picture);
}

BEGIN_MESSAGE_MAP(Csmtp_serverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ShowPicture, &Csmtp_serverDlg::OnBnClickedShowpicture)
END_MESSAGE_MAP()


// Csmtp_serverDlg 消息处理程序

BOOL Csmtp_serverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//创建底层套接字句柄
	int bFlag = MySock.Create(25, SOCK_STREAM, FD_ACCEPT);//, (LPCTSTR)"127.0.0.1"
	if (!bFlag)
	{
		m_log.SetWindowText(L"创建Socket失败\r\n");
		AfxMessageBox(_T("创建Socket失败!"));
		MySock.Close();
	}
	else
	{
		//指定监听套接字对象等待队列最大请求个数
		if (!MySock.Listen(5))
		{
			m_log.SetWindowText(L"SMTP服务器监听失败\r\n");
			AfxMessageBox(_T("SMTP服务器监听失败"));
			MySock.Close();
		}
		//接受请求后，触发FD_ACCEPT事件，调用OnAccpet函数
		else
			m_log.SetWindowText(L"SMTP服务器准备就绪\r\n");
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Csmtp_serverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Csmtp_serverDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Csmtp_serverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Csmtp_serverDlg::OnBnClickedShowpicture()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilePath = _T("1.bmp");
	if (strFilePath == _T(""))
		return;

	CImage image;
	image.Load(strFilePath);
	CRect rectControl;//控件矩形对象
	CRect rectPicture;//图片矩形对象
	int x = image.GetWidth();
	int y = image.GetHeight();
	CWnd  *pWnd = GetDlgItem(IDC_Picture);
	pWnd->GetClientRect(rectControl);
	CDC *pDc = GetDlgItem(IDC_Picture)->GetDC();
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	rectPicture = CRect(rectControl.TopLeft(), CSize((int)rectControl.Width(), (int)rectControl.Height()));
	((CStatic*)GetDlgItem(IDC_Picture))->SetBitmap(NULL);

	image.Draw(pDc->m_hDC, rectPicture);//将图片绘制到Picture控件表示的矩形区域
	image.Destroy();
	pWnd->ReleaseDC(pDc);
}
