// mailsocket.cpp : ʵ���ļ�
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


// mailsocket ��Ա����

//����nErrorCode,��������ʱ��MFC����ṩ�������׽������µ�״̬
//�����0����������ִ�У�û�д��󣻷�0���׽��ֶ������
void mailsocket::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	//Csmtp_serverDlg* dlg = (Csmtp_serverDlg*)AfxGetApp()->GetMainWnd(); //���������ָ�����
	CString Log, PreLog;
	Log += PreLog;
	mailsocket *NewSock = new mailsocket();
	AfxGetMainWnd()->GetDlgItemText(IDC_Log, PreLog);
	if (Accept(*NewSock))
	{
		send = "220 ready\r\n";
		NewSock->Send(send, strlen(send), 0);
		//����FD_READb �¼�������OnReceive����
		NewSock->AsyncSelect(FD_READ);
		//dlg->m_log.AddString(L"TCP���ӳɹ�");
		Log += "TCP���ӳɹ�\r\n";
	}
	else
	{
		//dlg->m_log.AddString(L"TCP����ʧ��");
		Log += "TCP����ʧ��\r\n";
		NewSock->Close();
	}
	AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
	CAsyncSocket::OnAccept(nErrorCode);
}


void mailsocket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CAsyncSocket::OnClose(nErrorCode);
}


void mailsocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	memset(data, 0, sizeof(data));  //ÿ��receive��֮ǰ��Ҫ�ѻ���������
	length = Receive(data, sizeof(data), 0);//�����յ���Ϣ�ĳ��ȣ����յ������ݴ浽data��

	//�����ݴӵȴ����ж��뻺���������Ҳ������ݴӻ��������
	//length = Receive(data, sizeof(data), MSG_PEEK);
	//�����������
	//length = Receive(data, sizeof(data), MSG_OOB);

	//receive.Format(_T("%s"), data);
	receive = data;
	CString PreLog,Log;
	AfxGetMainWnd()->GetDlgItemText(IDC_Log, PreLog);
	if (!IsData)//д������־
	{
		Log = PreLog + L"C:" + receive.Left(length) + L"\r\n";
	}
	else
	{
		Log = PreLog + L"C:" + L"data" ;
	}

	//if (length != 0)//������յ�����
	if (length != SOCKET_ERROR)
	{
		if (!IsData)//������յ���������
		{
			//�ֱ�Բ�ͬ�������Ӧ��
			//if (receive.Left(4) == "HELO" && step == 1)
			if (receive.Left(4) == "HELO")
			{
				send = "250 OK 127.0.0.1\r\n";//Ӧ��
				Send(send, strlen(send), 0);//����Ӧ��
				Log = Log + L"S:" + L"127.0.0.1\r\n";
				AsyncSelect(FD_READ);//�������պ���
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);//д������־
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
				IsData = true;//����յ�DATA���˵���������Ľ��յ���������
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
				Quit = true;//�ͻ����˳������ֹ����
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
		else//���յ������ݣ�������δ������ȫʱ��������Ӧ��
		{
			CString str1, str2;
			AfxGetMainWnd()->GetDlgItemText(IDC_INFO, str1);
			str2 = str1 + receive.Left(length);
			AfxGetMainWnd()->SetDlgItemText(IDC_INFO, str2);
			if (receive.Find(L"\r\n.\r\n") != -1)//���ݽ������
			{
				IsData = false;
				send;
				send = "250 Message accepted for delivery\r\n";//����Ӧ��

				Send(send, strlen(send), 0);
				Log = Log + "S:" + (CString)send;
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, Log);
				AfxGetMainWnd()->GetDlgItemText(IDC_INFO, pic);
				if (pic.Find(L"Content-Type: image/bmp") != -1)//��������bmpͼƬ
				{
					//��ȡbmpͼƬ��base64����
					int Attachment_Start = pic.Find(L"Content-Disposition: attachment", 0);
					int Bmp_Start = pic.Find(L"\r\n\r\n", Attachment_Start);
					CString Start = pic.Mid(Bmp_Start + 4, pic.GetLength() - Bmp_Start - 4);
					int length = Start.Find(L"\r\n\r\n", 0);
					pic = Start.Left(length);
					HBITMAP picture;
					//����
					DeCode(pic, picture);
					//���뵽�Ի���
				    Csmtp_serverDlg *CurrentApp = (Csmtp_serverDlg *)AfxGetApp();
					Csmtp_serverDlg *CurrentDlg = (Csmtp_serverDlg *)CurrentApp->m_hWnd;
					//CurrentDlg->m_Bmp.SetBitmap(picture);

				}
			}
			AsyncSelect(FD_READ);

			return;
		}
	}
	if (Quit)//�˳�
	{
		Close();//�ر��׽���
		Quit = false;
		return;
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
