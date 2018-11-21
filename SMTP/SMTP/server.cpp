#include <stdio.h>  
#include <winsock2.h> 
#include <windows.h>
#include <iostream>
#include <string.h>
#pragma comment(lib,"ws2_32.lib")//ָʾ���������Ҳ�ʹ��ws2_32.lib��̬��

using namespace std;

#define INET_ADDRSTRLEN  16  
#define INET6_ADDRSTRLEN 46  
#define _WINSOCK_DEPRECATED_NO_WARNINGS

int main()
{
	//����Winsock��
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
    //�����׽��ֿ⣬�����׽���WSAStartup()/socket()
    //��ʼ��socket
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return -1;
	}
	//cout << "ϵͳ֧����߰汾:" << wsaData.wHighVersion << endl;
	
	//�������socket  
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		cout << "Sock Error!" << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	//��IP�Ͷ˿�  
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));//���
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	//sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(5001);//5001~65535

    //���׽��ֵ�һ��IP��ַ��һ���˿���bind()
	if(bind(sock,(struct sockaddr*)&sin,sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "Bind Error!" << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

    //���׽�������Ϊ����ģʽ�ȴ���������listen()
	if (listen(sock, 5) == SOCKET_ERROR)
	{
		cout << "Listen Error!" << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	//ѭ����������  
	SOCKET client;
	sockaddr_in remote;
	int Addrlen = sizeof(remote);
	char revData[255],temp[10];
	string str;
	//const char * sendData = NULL;
	char * sendData = NULL;
	while (true)
	{
		cout<<"�ȴ�����..."<<endl;
        //�������󣬽����������󣬷���һ���µĶ�Ӧ�ڴ˴����ӵ��׽���accept()
		client = accept(sock, (SOCKADDR *)&remote, &Addrlen);
		if (client == SOCKET_ERROR)//INVALID_SOCKET
		{
			cout << "Accept Error!" << WSAGetLastError() << endl;
			WSACleanup();
			return -1;
		}

		//������ص�ַ�����Ӷ˿ڵ�ַ
		cout<<"���ܵ�һ�����ӣ�"<<inet_ntoa(remote.sin_addr)<<endl;

		//������־��Ҫ��¼
		sendData = "220 163.com Anti-spam GT for Coremail System (163com) \n";
		send(client,sendData, strlen(sendData), 0);

		while (true)
		{
			//��������  
			memset(&revData, 0, sizeof(revData));
			if (recv(client, revData, 255, 0))
				cout << revData << endl;

			strncpy(temp,revData,5);
			cout << temp<<endl;//cout�Զ�Ĩȥ
			str = temp;
			//if (revData[0] == 'h' && revData[1] == 'e' && revData[2] == 'l' && revData[3] == 'o')
			if(str == "helo")
			{
				sendData = "250 OK \n";
				send(client, sendData, strlen(sendData), 0);

				while (true)
				{
					memset(&revData, 0, sizeof(revData));
					if (recv(client, revData, 255, 0))
						cout << revData << endl;

					strncpy(temp, revData, 5);
					cout << temp << endl;//cout�Զ�Ĩȥ
					str = temp;
					if (str == "auth")
					{
						sendData = "250 OK \n";
						send(client, sendData, strlen(sendData), 0);
					}
				}
			}
			else
			{
				sendData = "502 Error: command not implemented \n";
				send(client, sendData, strlen(sendData), 0);
				//break;
			}
		}
		//closesocket(client);
	}
	closesocket(client);
    //�ر��׽��֣��رռ��ص��׽��ֿ�closesocket()/WSACleanup()
	closesocket(sock);
	WSACleanup();
	return 0;
}
