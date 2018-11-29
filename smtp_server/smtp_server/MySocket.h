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

	char data[8192];//�������ݵĻ�����
	char *msg;//���͵�����
	//CString log;//ÿ����������־��д������
	int length;//�������ݵĳ���
	CString receive;//��Ŵӻ�������ȡ����������
	int step;//���ڼ���smtpЭ���Ƿ�˳��
	CString all;//���ڴ洢�ʼ����ĵ�base64��

	bool IsData;//������Ƿ�������
	bool Quit;//�Ƿ���ܵ�quit����
	bool IsBmp;//�����Ƿ���ͼƬ
	bool IsError;//�ж������Ƿ��д�
};

