
// smtp_server.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Csmtp_serverApp: 
// �йش����ʵ�֣������ smtp_server.cpp
//

class Csmtp_serverApp : public CWinApp
{
public:
	Csmtp_serverApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Csmtp_serverApp theApp;