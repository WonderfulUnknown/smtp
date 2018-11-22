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
	step = 1;
}

mailsocket::~mailsocket()
{
}


// mailsocket 成员函数

//参数nErrorCode,函数调用时，MFC框架提供，表明套接字最新的状态
//如果是0，函数正常执行，没有错误；非0，套接字对象出错
void mailsocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	//Csmtp_serverDlg* dlg = (Csmtp_serverDlg*)AfxGetApp()->GetMainWnd(); //获得主窗口指针对象
	CString Log, PreLog;
	Log += PreLog;
	mailsocket *NewSock = new mailsocket();
	AfxGetMainWnd()->GetDlgItemText(IDC_Log, PreLog);
	if (Accept(*NewSock))
	{
		send = "220 ready\r\n";
		NewSock->Send(send, strlen(send), 0);
		//触发FD_READb 事件，调用OnReceive函数
		NewSock->AsyncSelect(FD_READ);
		//dlg->m_log.AddString(L"TCP连接成功");
		Log += "TCP连接成功\r\n";
	}
	else
	{
		//dlg->m_log.AddString(L"TCP连接失败");
		Log += "TCP连接失败\r\n";
		NewSock->Close();
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
	memset(data, 0, sizeof(data));  //每次receive的之前需要把缓冲区清零
	length = Receive(data, sizeof(data), 0);//返回收到消息的长度，接收到的数据存到data中

	//将数据从等待队列读入缓存区，并且不将数据从缓冲区清除
	//length = Receive(data, sizeof(data), MSG_PEEK);
	//接受外带数据
	//length = Receive(data, sizeof(data), MSG_OOB);

	//receive.Format(_T("%s"), data);
	receive = data;
	CString PreLog,Log;
	AfxGetMainWnd()->GetDlgItemText(IDC_Log, PreLog);
	if (!IsData)//写接收日志
	{
		Log = PreLog + L"C:" + receive.Left(length) + L"\r\n";
	}
	else
	{
		Log = PreLog + L"C:" + L"data" ;
	}

	//if (length != 0)//如果接收到数据
	if (length != SOCKET_ERROR)
	{
		if (!IsData)//如果接收到的是命令
		{
			//分别对不同命令进行应答
			//if (receive.Left(4) == "HELO" && step == 1)
			if (receive.Left(4) == "HELO")
			{
				send = "250 OK 127.0.0.1\r\n";//应答
				Send(send, strlen(send), 0);//发送应答
				Log = Log + L"S:" + L"127.0.0.1\r\n";
				AsyncSelect(FD_READ);//触发接收函数
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);//写发送日志
				step = 2;
				return;
			}
			if (receive.Left(10) == "MAIL FROM:" && step == 2)
			{
				send = "250 Sender OK\r\n";

				Send(send, strlen(send), 0);
				Log = Log + L"S:" + L"250 Sender OK\r\n";
				AsyncSelect(FD_READ);
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
				step = 3;
				return;
			}
			if (receive.Left(8) == "RCPT TO:" && step == 3)
			{
				send = "250 Receiver OK\r\n";

				Send(send, strlen(send), 0);
				Log = Log + L"S:" + L"250 Receiver OK\r\n";
				AsyncSelect(FD_READ);
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
				step = 4;
				return;
			}
			if (receive.Left(4) == "DATA" && step == 4)
			{
				IsData = true;//如果收到DATA命令，说明接下来的接收到的是数据
				send = "354 Go ahead\r\n";

				Send(send, strlen(send), 0);

				Log = Log + "S:" + (CString)send;
				AsyncSelect(FD_READ);
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
				step = 5;
				return;
			}
			if (receive.Left(4) == "QUIT" || receive.Left(4) == "RSET")
			{
				send = "221 Quit,Goodbye\r\n";
				Send(send, strlen(send), 0);
				Log = Log + "S:" + (CString)send;
				Quit = true;//客户端退出命令，终止程序
				AsyncSelect(FD_READ);
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
				return;
			}
			send = "500 Error: bad syntax\r\n";

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
				send;
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
