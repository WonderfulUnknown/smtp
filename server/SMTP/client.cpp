#include<WINSOCK2.H>
#include<STDIO.H>
#include<iostream>
#include<cstring>
#include<WS2tcpip.h>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
int main()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}
	while (true) {
		SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sclient == INVALID_SOCKET)
		{
			printf("invalid socket!");
			return 0;
		}

		sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(8888);
		//serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//新版vs不支持的老函数，需要换成新的函数inet_pton()
		char addrptr[100];
		inet_pton(AF_INET,"127.0.0.1",addrptr);//将ipv4地址的二进制结果放在addrptr中
		if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{  //连接失败 
			printf("connect error !");
			closesocket(sclient);
			return 0;
		}

		//string data;
		char data[100];
		cin>>data;
		const char * sendData = NULL;
		//sendData = data.c_str();   //string转const char* 
								   //char * sendData = "你好，TCP服务端，我是客户端\n";
		send(sclient, sendData, strlen(sendData), 0);
		//send()用来将数据由指定的socket传给对方主机
		//int send(int s, const void * msg, int len, unsigned int flags)
		//s为已建立好连接的socket，msg指向数据内容，len则为数据长度，参数flags一般设0
		//成功则返回实际传送出去的字符数，失败返回-1，错误原因存于error 

		char recData[255];
		int ret = recv(sclient, recData, 255, 0);
		if (ret>0) {
			recData[ret] = 0x00;
			printf(recData);
		}
		closesocket(sclient);
	}


	WSACleanup();
	return 0;

}

