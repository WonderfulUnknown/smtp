
#if !defined(AFX_NEWSOCKCLASS_H__8EB4225C_953E_4C9D_B82B_735228DD045A__INCLUDED_)
#define AFX_NEWSOCKCLASS_H__8EB4225C_953E_4C9D_B82B_735228DD045A__INCLUDED_
#define BufferLen 4096
#endif
// mailsocket ÃüÁîÄ¿±ê
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
	char data[BufferLen];
};