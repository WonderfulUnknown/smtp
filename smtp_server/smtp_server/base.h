#include"StdAfx.h"

char table[64] = { 'A', 'B', 'C', 'D', 'E', 'F',
'G', 'H', 'I', 'J','K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T','U', 'V', 'W', 'X', 'Y', 'Z',
'a', 'b', 'c', 'd','e', 'f', 'g', 'h', 'i', 'j',
'k', 'l', 'm', 'n','o', 'p', 'q', 'r', 's', 't',
'u', 'v', 'w', 'x','y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7','8', '9', '+', '/' };

BOOL CharToByte(BYTE& a, char& b)
{
	for (int i = 0; i < 64; i++)
	{
		if (b == table[i])
		{
			//��0~64��Ϊ������
			a = i & 0x00ff;
			return true;
		}
	}
	return false;
}

//���벢д��bmpͼƬ
BOOL DeCode(CString &attachment, HBITMAP &picture)
{
	//����ʱ��4��byteΪһ������ַ�ƥ�䡣
	BYTE buf[4],temp[4];
	int i = 0,len = attachment.GetLength();;
	CFile file;
	char curr;

	//��������д�������ļ�
	if (file.Open(L"1.bmp", CFile::modeWrite | CFile::modeCreate))//�����ļ���д��
	{
		while (i < len)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i < len)
				{
					//�����ַ������±�Ϊi���ַ�
					curr = attachment.GetAt(i);
					if (!CharToByte(temp[j], curr))
					{
						temp[j] = 255;
						j--;
					}
					i++;
				}
			}
			//base64���룬�ĸ���λ���������λ
			buf[0] = (temp[0] << 2) | (temp[1] >> 4);
			buf[1] = (temp[1] << 4) | (temp[2] >> 2);
			buf[2] = (temp[2] << 6) | (temp[3]);
			//д���ļ�
			file.Write(buf, 3);
			file.SeekToEnd();
		}
	}
	file.Close();
	//�ļ����bmpͼ��
	picture = (HBITMAP)::LoadImage(NULL, L"1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	return true;
}

