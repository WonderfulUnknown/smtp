#include <stdio.h>  
#include <winsock2.h> 
#include <windows.h>
#include <iostream>
#include <string.h>
#pragma comment(lib,"ws2_32.lib")//指示链接器查找并使用ws2_32.lib静态库

using namespace std;

#define INET_ADDRSTRLEN  16  
#define INET6_ADDRSTRLEN 46  
#define _WINSOCK_DEPRECATED_NO_WARNINGS

int main()
{
	//加载Winsock库
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
    //加载套接字库，创建套接字WSAStartup()/socket()
    //初始化socket
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return -1;
	}
	//cout << "系统支持最高版本:" << wsaData.wHighVersion << endl;
	
	//构造监听socket  
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		cout << "Sock Error!" << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	//绑定IP和端口  
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));//清空
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	//sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(5001);//5001~65535

    //绑定套接字到一个IP地址和一个端口上bind()
	if(bind(sock,(struct sockaddr*)&sin,sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "Bind Error!" << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

    //将套接字设置为监听模式等待连接请求listen()
	if (listen(sock, 5) == SOCKET_ERROR)
	{
		cout << "Listen Error!" << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	//循环接收数据  
	SOCKET client;
	sockaddr_in remote;
	int Addrlen = sizeof(remote);
	char revData[255],temp[10];
	string str;
	//const char * sendData = NULL;
	char * sendData = NULL;
	while (true)
	{
		cout<<"等待连接..."<<endl;
        //请求到来后，接受连接请求，返回一个新的对应于此次连接的套接字accept()
		client = accept(sock, (SOCKADDR *)&remote, &Addrlen);
		if (client == SOCKET_ERROR)//INVALID_SOCKET
		{
			cout << "Accept Error!" << WSAGetLastError() << endl;
			WSACleanup();
			return -1;
		}

		//输出本地地址和连接端口地址
		cout<<"接受到一个连接："<<inet_ntoa(remote.sin_addr)<<endl;

		//交互日志需要记录
		sendData = "220 163.com Anti-spam GT for Coremail System (163com) \n";
		send(client,sendData, strlen(sendData), 0);

		while (true)
		{
			//接收数据  
			memset(&revData, 0, sizeof(revData));
			if (recv(client, revData, 255, 0))
				cout << revData << endl;

			strncpy(temp,revData,5);
			cout << temp<<endl;//cout自动抹去
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
					cout << temp << endl;//cout自动抹去
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
    //关闭套接字，关闭加载的套接字库closesocket()/WSACleanup()
	closesocket(sock);
	WSACleanup();
	return 0;
}
