#include "stdafx.h"
#include "MySocket.h"
#include "smtp_server.h"
#include "smtp_serverDlg.h"
#include "base.h"
#include <regex>  // regular expression ������ʽ

using namespace std;

MySocket::MySocket()
{
	step = 1;
	IsData = false;
	Quit = false;
	IsBmp = false;
	IsError = false;
}


MySocket::~MySocket()
{
}


//����nErrorCode,��������ʱ��MFC����ṩ�������׽������µ�״̬
//�����0����������ִ�У�û�д��󣻷�0���׽��ֶ������
void MySocket::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CString log;
	MySocket *sock = new MySocket();

	AfxGetMainWnd()->GetDlgItemText(IDC_Log, log);

	if (Accept(*sock))
	{
		msg = "220 smtp 127.0.0.1 is ready\r\n";
		sock->Send(msg, strlen(msg), 0);
		//����FD_READ�¼�������OnReceive����
		sock->AsyncSelect(FD_READ);

		log = "TCP���ӳɹ�\r\n";
	}
	else
	{
		log = "TCP����ʧ��\r\n";
		sock->Close();
	}
	AfxGetMainWnd()->SetDlgItemText(IDC_Log, log);

	CAsyncSocket::OnAccept(nErrorCode);
}


void MySocket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CAsyncSocket::OnClose(nErrorCode);
}


void MySocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	//ÿ��receive֮ǰ��Ҫ�ѻ���������
	memset(data, 0, sizeof(data));
	//length�洢�����յ���Ϣ�ĳ��ȣ����յ������ݴ浽data��
	length = Receive(data, sizeof(data), 0);

	//�����ݴӵȴ����ж��뻺���������Ҳ������ݴӻ��������
	//length = Receive(data, sizeof(data), MSG_PEEK);
	//�����������
	//length = Receive(data, sizeof(data), MSG_OOB);

	receive = data;
	CString log;
	
	AfxGetMainWnd()->GetDlgItemText(IDC_Log, log);
	if (!IsData)
		log = log + L"C:" + receive.Left(length);
	else
		log = log + L"C:" + L"data\r\n";

	if (length != SOCKET_ERROR)
	{
		if (!IsData)//���յ�����
		{
			std::regex mail_pattern("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
			CString Mail;
			string mail;
			smatch r;
			switch (step)
			{
			case 1:
				//��ȡ���n���ַ�������˫�ֽ��ַ��������ģ�����ܻ�ض����룬n�����ֽڼ���
				if (receive.Left(4) == "HELO" || receive.Left(4) == "EHLO")
					msg = "250 OK hello smtp 127.0.0.1\r\n";
				else IsError = true;
				break;
			case 2:
				if (receive.Left(10) == "MAIL FROM:")
				{
					Mail = receive.Mid(11);//��ȡ����
					Mail.Remove('<');
					Mail.Remove('>');
					Mail.Remove('\r');
					Mail.Remove('\n');
					mail = CT2A(Mail.GetBuffer());//��CStringתΪstring
					//MessageBox(NULL,Mail, L"debug", MB_OK);

					//�ж��������ʽ�Ƿ�ƥ��
					regex_match(mail, r, mail_pattern);
					if (r.str() == mail)
					{
						msg = "250 Sender's mail is ok\r\n";
						//msg = "250 Sender: ";
						//strcat_s��strcat��ȫ���������
						//strcat_s(msg, 100, (char*)mail.c_str());//.c_str()��strת��Ϊchar *
						//strcat_s(msg, 100, " OK \r\n");//sizeof(char *)=4,����
					}
					else
						msg = "550 Mailbox is not available";
				}
				else IsError = true;
				break;
			case 3:
				if (receive.Left(8) == "RCPT TO:")
				{
					Mail = receive.Mid(11);//��ȡ����
					Mail.Remove('<');
					Mail.Remove('>');
					Mail.Remove('\r');
					Mail.Remove('\n');
					mail = CT2A(Mail.GetBuffer());//��CStringתΪstring
					//MessageBox(NULL,Mail, L"debug", MB_OK);

					//�ж��������ʽ�Ƿ�ƥ��
					regex_match(mail, r, mail_pattern);
					if (r.str() == mail)
						msg = "250 Receiver's mail is ok\r\n";	
					else
						msg = "550 Mailbox is not available";
				}
				else IsError = true;
				break;
			case 4:
				if (receive.Left(4) == "DATA")
				{
					IsData = true;//����յ�DATA���˵���������Ľ��յ���������
					msg = "354 Start mail input,end with <CRLF>.<CRLF>\r\n";
				}
				else IsError = true;
				break;
			default:
				if (receive.Left(4) == "QUIT")
				{
					msg = "221 smtp 127.0.0.1 server closing connection\r\n";
					Quit = true;//�ͻ����˳������ֹ����
				}
			}
			if (IsError)
				msg = "550 Error: bad syntax\r\n";

			Send(msg, strlen(msg), 0);//����Ӧ��

			step++;
			log = log + L"S:" + (CString)msg;
			AsyncSelect(FD_READ);//�������պ���
			AfxGetMainWnd()->SetDlgItemText(IDC_Log, log);//д������־
			return;
		}
		else
		{
			CString temp;
			//����֮ǰд�õ��ʼ���ʽ����Ϣ
			AfxGetMainWnd()->GetDlgItemText(IDC_INFO, temp);
			//��ȡ�ʼ�����������
			pic = temp + receive.Left(length);
			AfxGetMainWnd()->SetDlgItemText(IDC_INFO, pic);

			//<CRLF>.<CRLF>
			//����鵽��������0������ʼ��λ��;δ�鵽������-1
			if (receive.FindOneOf(L"\r\n.\r\n") != -1)
			{
				IsData = false;

				msg = "250 Message accepted\r\n";//����Ӧ��
				Send(msg, strlen(msg), 0);

				log = log + "S:" + (CString)msg;
				AfxGetMainWnd()->SetDlgItemText(IDC_Log, log);

				//Find����鵽��������0������ʼ��λ��;δ�鵽,����-1
				if (pic.Find(L"Content-Type: image/bmp") != -1)//��������ͼƬ
				{
					//��ȡͼƬ��base64����
					int Attachment_Start = pic.Find(L"Content-Disposition: attachment", 0);
					int Bmp_Start = pic.Find(L"\r\n\r\n", Attachment_Start);
					CString Start = pic.Mid(Bmp_Start + 4, pic.GetLength() - Bmp_Start - 4);
					int length = Start.Find(L"\r\n\r\n", 0);
					pic = Start.Left(length);
					HBITMAP picture;
					//����
					DeCode(pic, picture);
					//���뵽�Ի���
					//Csmtp_serverDlg *CurrentApp = (Csmtp_serverDlg *)AfxGetApp();
					//Csmtp_serverDlg *CurrentDlg = (Csmtp_serverDlg *)CurrentApp->m_hWnd;
					//CurrentDlg->m_Bmp.SetBitmap(picture);

				}
			}
			AsyncSelect(FD_READ);

			return;
		}
	}
	if (Quit)//�˳�
	{
		MySocket sock;
		sock.OnClose(0);
		Quit = false;
		return;
	}

	CAsyncSocket::OnReceive(nErrorCode);
}