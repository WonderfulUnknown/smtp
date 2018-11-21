// mailsocket.cpp : 实现文件
//

#include "stdafx.h"
#include "smtp_server.h"
#include "mailsocket.h"
#include "base.h"
#include "smtp_serverDlg.h"


// mailsocket

mailsocket::mailsocket()
{
	IsData = false;
	Quit = false;
}

mailsocket::~mailsocket()
{
}


// mailsocket 成员函数


void mailsocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CString PreLog, Log;
	AfxGetMainWnd()->GetDlgItemText(IDC_Log, PreLog);
	Log += PreLog;
	mailsocket *NewSock = new mailsocket();

	int IsConnect = Accept(*NewSock);
	if (IsConnect != 0)
	{
		char* send = "1611305 SMTP is ready \r\n";
		NewSock->Send(send, strlen(send), 0);
		NewSock->AsyncSelect(FD_READ);//调用接受函数进行操作。
		Log += "TCP连接成功\r\n";
	}
	else
	{
		Log += "TCP连接失败\r\n";

	}
	AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
	CAsyncSocket::OnAccept(nErrorCode);
}


void mailsocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CAsyncSocket::OnClose(nErrorCode);
}


void mailsocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	int length = Receive(data, sizeof(data), 0);//接收到的数据存到data
	CString receive;
	receive += data;
	CString PreLog, Log;
	AfxGetMainWnd()->GetDlgItemText(IDC_Log, PreLog);
	if (!IsData)//写接收日志
	{
		Log = PreLog + L"C:" + receive.Left(length) + L"\r\n";
	}
	else
	{
		Log = PreLog + L"C:" + L"data" + L"\r\n";
	}

	if (length != 0)//如果接收到数据
	{
		if (!IsData)//如果接收到的是命令
		{
			//分别对不同命令进行应答
			if (receive.Left(4) == "helo" || receive.Left(4) == "HELO")
			{
				char* send = "250 OK 127.0.0.1\r\n";//应答

				Send(send, strlen(send), 0);//发送应答
				Log = Log + L"S:" + L"127.0.0.1\r\n";
				AsyncSelect(FD_READ);//触发接收函数
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);//写发送日志
				return;
			}
			//if (receive.Left(4) == "EHLO")
			//{
			//	char* send = "250 OK 127.0.0.1\r\n";//应答

			//	Send(send, strlen(send), 0);//发送应答
			//	Log = Log + L"S:" + L"127.0.0.1\r\n";
			//	AsyncSelect(FD_READ);//触发接收函数
			//	AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);//写发送日志
			//	return;
			//}
			if (receive.Left(10) == "MAIL FROM:")
			{
				char *send = "250 Sender OK\r\n";

				Send(send, strlen(send), 0);
				Log = Log + L"S:" + L"250 Sender OK\r\n";
				AsyncSelect(FD_READ);
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
				return;
			}
			if (receive.Left(8) == "RCPT TO:")
			{
				char* send = "250 Receiver OK\r\n";

				Send(send, strlen(send), 0);
				Log = Log + L"S:" + L"250 Receiver OK\r\n";
				AsyncSelect(FD_READ);
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
				return;
			}
			if (receive.Left(4) == "DATA")
			{
				IsData = true;//如果收到DATA命令，说明接下来的接收到的是数据
				char* send = "354 Go ahead\r\n";

				Send(send, strlen(send), 0);

				Log = Log + "S:" + (CString)send;
				AsyncSelect(FD_READ);
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
				return;
			}
			if (receive.Left(4) == "QUIT" || receive.Left(4) == "RSET")
			{
				char* send = "221 Quit,Goodbye\r\n";

				Send(send, strlen(send), 0);
				Log = Log + "S:" + (CString)send;
				Quit = true;//客户端退出命令，终止程序
				AsyncSelect(FD_READ);
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
				return;
			}
			char* send = "500 order is wrong\r\n";

			Send(send, strlen(send), 0);
			Log = Log + "S:" + (CString)send;
			Quit = true;
			AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
			return;
		}
		else//接收的是数据，在数据未接收完全时，不发送应答
		{
			CString str1, str2;
			AfxGetMainWnd()->GetDlgItemText(IDC_INFO, str1);
			str2 = str1 + receive.Left(length);
			AfxGetMainWnd()->SetDlgItemText(IDC_INFO, str2);
			if (receive.Find(L"\r\n.\r\n") != -1)//数据接收完成
			{
				IsData = false;
				char* send;
				send = "250 Message accepted for delivery\r\n";//发送应答

				Send(send, strlen(send), 0);
				Log = Log + "S:" + (CString)send;
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
				AfxGetMainWnd()->GetDlgItemText(IDC_INFO, pic);
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
		Close();//关闭套接字
		Quit = false;
		return;

	}
	CAsyncSocket::OnReceive(nErrorCode);
}
