#include<WINSOCK2.H>
#include<STDIO.H>
#include<iostream>
#include<cstring>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

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
		serAddr.sin_port = htons(25);
		serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{  //¡¨Ω” ß∞‹ 
			printf("connect error !");
			closesocket(sclient);
			return 0;
		}

		printf("connect succeed !");
		char recData[255];
		char data[100];
		while (true)
		{
			memset(&recData, 0, sizeof(recData));
			if (recv(sclient, recData, 255, 0))
				cout << recData;
			//char ch;
			//ch = cin.get();
			//cout << ch;
			
			//string abc=cin.get();
			//cin >> noskipws;//…Ë÷√cin∂¡»°ø’∞◊∑˚£ª
			//cin >> data;

			cin.getline(data, 100);
			//cout << data;
			send(sclient, data, strlen(data), 0);
		}

		closesocket(sclient);
	}

	WSACleanup();
	return 0;

}

