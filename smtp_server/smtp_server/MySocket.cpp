#include "stdafx.h"
#include "MySocket.h"
#include "smtp_server.h"
#include "smtp_serverDlg.h"
#include "base.h"

MySocket::MySocket()
{
	step = 1;
	IsData = false;
	Quit = false;
}


MySocket::~MySocket()
{
}


//参数nErrorCode,函数调用时，MFC框架提供，表明套接字最新的状态
//如果是0，函数正常执行，没有错误；非0，套接字对象出错
void MySocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString log;
	MySocket *sock = new MySocket();

	AfxGetMainWnd()->GetDlgItemText(IDC_Log, log);

	if (Accept(*sock))
	{
		msg = "220 smtp 127.0.0.1 is ready\r\n";
		sock->Send(msg, strlen(msg), 0);
		//触发FD_READ事件，调用OnReceive函数
		sock->AsyncSelect(FD_READ);

		log = "TCP连接成功\r\n";
	}
	else
	{
		log = "TCP连接失败\r\n";
		sock->Close();
	}
	AfxGetMainWnd()->SetDlgItemText(IDC_Log, log);

	CAsyncSocket::OnAccept(nErrorCode);
}


void MySocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CAsyncSocket::OnClose(nErrorCode);
}


void MySocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	//每次receive之前需要把缓冲区清零
	memset(data, 0, sizeof(data));  
	//length存储返回收到消息的长度，接收到的数据存到data中
	length = Receive(data, sizeof(data), 0);

	//将数据从等待队列读入缓存区，并且不将数据从缓冲区清除
	//length = Receive(data, sizeof(data), MSG_PEEK);
	//接受外带数据
	//length = Receive(data, sizeof(data), MSG_OOB);

	receive = data;
	CString log;
	//将上一次的log写入
	AfxGetMainWnd()->GetDlgItemText(IDC_Log, log);
	if (!IsData)
		log = log + L"C:" + receive.Left(length);
	else
		log = log + L"C:" + L"data\r\n";

	if (length != SOCKET_ERROR)
	{
		if (!IsData)//接收到命令
		{
			//根据用户输入的内容进行应答
			if ((receive.Left(4) == "HELO" || receive.Left(4) == "EHLO") && step == 1)
				msg = "250 OK hello smtp 127.0.0.1\r\n";
			else if (receive.Left(10) == "MAIL FROM:" && step == 2)
				msg = "250 Sender OK\r\n";
			else if (receive.Left(8) == "RCPT TO:" && step == 3)
				msg = "250 Receiver OK\r\n";
			else if (receive.Left(4) == "DATA" && step == 4)
			{
				IsData = true;//如果收到DATA命令，说明接下来的接收到的是数据
				msg = "354 Start mail input,end with <CRLF>.<CRLF>\r\n";
			}
			else if (receive.Left(4) == "QUIT")
			{
				msg = "221 smtp 127.0.0.1 server closing connection\r\n";
				Quit = true;//客户端退出命令，终止程序
			}
			else
				msg = "550 Error: bad syntax\r\n";
	
			Send(msg, strlen(msg), 0);//发送应答

			step++;
			log = log + L"S:" + (CString)msg;
			AsyncSelect(FD_READ);//触发接收函数
			AfxGetMainWnd()->SetDlgItemText(IDC_Log, log);//写发送日志
			return;
		}
		//接收完数据才应答
		else
		{
			//获取邮件的所有内容
			CString str = receive.Left(length);
			AfxGetMainWnd()->SetDlgItemText(IDC_INFO, str);

			//<CRLF>.<CRLF>
			if (receive.Find(L"\r\n.\r\n") != -1)//数据接收完成
			{
				IsData = false;
				
				msg = "250 Message accepted\r\n";//发送应答
				Send(msg, strlen(msg), 0);
				
				log = log + "S:" + (CString)msg;
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, log);
				
				//AfxGetMainWnd()->GetDlgItemText(IDC_INFO, pic);
				
				if (pic.Find(L"Content-Type: image/bmp") != -1)//附件中有bmp图片
				{
					//截取bmp图片的base64编码
					int Attachment_Start = pic.Find(L"Content-Disposition: attachment", 0);
					int Bmp_Start = pic.Find(L"\r\n\r\n", Attachment_Start);
					CString Start = pic.Mid(Bmp_Start + 4, pic.GetLength() - Bmp_Start - 4);
					int length = Start.Find(L"\r\n\r\n", 0);
					pic = Start.Left(length);
					HBITMAP picture;
					//解码
					DeCode(pic, picture);
					//输入到对话框
					Csmtp_serverDlg *CurrentApp = (Csmtp_serverDlg *)AfxGetApp();
					Csmtp_serverDlg *CurrentDlg = (Csmtp_serverDlg *)CurrentApp->m_hWnd;
					//CurrentDlg->m_Bmp.SetBitmap(picture);

				}
			}
			AsyncSelect(FD_READ);

			return;
		}
	}
	if (Quit)//退出
	{
		MySocket sock;
		sock.OnClose(0);
		Quit = false;
		return;
	}
	
	CAsyncSocket::OnReceive(nErrorCode);
}