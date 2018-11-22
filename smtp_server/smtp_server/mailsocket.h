
#if !defined(AFX_NEWSOCKCLASS_H__8EB4225C_953E_4C9D_B82B_735228DD045A__INCLUDED_)
#define AFX_NEWSOCKCLASS_H__8EB4225C_953E_4C9D_B82B_735228DD045A__INCLUDED_
#define BufferLen 4096
#endif
// mailsocket 命令目标
#if _MSC_VER > 1000
#pragma once
#endif



class mailsocket : public CAsyncSocket
{
public:
	mailsocket();
	virtual ~mailsocket();
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	bool IsData;
	bool Quit;
	bool IsBmp;
	CString pic;

	char data[BufferLen];//接受数据的缓冲区
	char* send;//每次发送的数据
	//CString Log;//每次往交互日志中写的内容
	int length;//每次返回数据的长度
	CString receive;//存放从缓冲区中取出来的数据
	int step;//检验smtp协议到第几步
};