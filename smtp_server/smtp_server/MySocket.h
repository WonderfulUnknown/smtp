#pragma once
#include "afxsock.h"
class MySocket :
	public CAsyncSocket
{
public:
	MySocket();
	~MySocket();

	void OnAccept(int nErrorCode);
	void OnClose(int nErrorCode);
	void OnReceive(int nErrorCode);

	char data[4096];//�������ݵĻ�����
	char *msg;//���͵�����
	//CString log;//ÿ����������־��д������
	int length;//�������ݵĳ���
	CString receive;//��Ŵӻ�������ȡ����������
	int step;//���ڼ���smtpЭ���Ƿ�˳��
	CString pic;

	bool IsData;//������Ƿ�������
	bool Quit;//�Ƿ���ܵ�quit����
	bool IsBmp;//�����Ƿ���ͼƬ
};

