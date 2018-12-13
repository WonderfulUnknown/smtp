#pragma once
#include "stdafx.h"
#include <iostream>
using namespace std;

CString Decode_base64(CString str)
{
	int DecodeTable[123];
	//��ʼ�������
	for (int i = 0; i < 123; i++)
	{
		DecodeTable[i] = 0;
		if (i == 43)//"+"
			DecodeTable[i] = 62;
		if (i == 47)//"/"
			DecodeTable[i] = 63;
		if (i >= 48 && i < 58)//0-9
			DecodeTable[i] = i + 4;
		if (i >= 65 && i < 91)//A-Z
			DecodeTable[i] = i - 65;
		if (i >= 97 && i < 123)//a-z
			DecodeTable[i] = i - 71;
	}

	char byte[4], out[3], curr;
	unsigned int num[4];//�洢�ַ��ڱ���������е��±�
	CString result;
	int step = 0;
	int i = 0;
	CFile file;
	if (file.Open(L"1.bmp", CFile::modeWrite | CFile::modeCreate))
	{
		while (i < str.GetLength())//һ�ζ����ĸ��ַ�
		{
			curr = str.GetAt(i);
			if (curr != '\r' && curr != '\n')
			{
				switch (step)
				{
				case 0:
					byte[0] = curr;
					step++;
					break;
				case 1:
					byte[1] = curr;
					step++;
					break;
				case 2:
					byte[2] = curr;
					step++;
					break;
				case 3:
					byte[3] = curr;
					num[0] = DecodeTable[(int)byte[0]];
					num[1] = DecodeTable[(int)byte[1]];
					num[2] = DecodeTable[(int)byte[2]];
					num[3] = DecodeTable[(int)byte[3]];
					//  �ַ���      a       b        c
					//	ASCII      97      98       99
					//	8bit   01100001 01100010 01100011
					//	6bit   011000   010110   001001   100011
					//	ʮ����      24      22        9        35
					//	��Ӧ����    Y        W        J        j
					out[0] = (char)((num[0] << 2) + (num[1] >> 4));
					out[1] = (char)((num[1] << 4) + (num[2] >> 2));
					out[2] = (char)((num[2] << 6) + num[3]);

					file.Write(out, 3);
					file.SeekToEnd();

					result.AppendChar(out[0]);

					if (byte[2] != '=')
					{
						result.AppendChar(out[1]);
						if (byte[3] != '=')
							result.AppendChar(out[2]);
					}
					step = 0;
					break;
				}
			}
			i++;
		}
	}
	file.Close();
	return result;
}
//for (int j = 0; j < 4; j++)
//{
//	byte[j] = curr;
//	if (j == 3)
//	{
//		num[0] = DecodeTable[(int)byte[0]];
//		num[1] = DecodeTable[(int)byte[1]];
//		num[2] = DecodeTable[(int)byte[2]];
//		num[3] = DecodeTable[(int)byte[3]];
//		out[0] = (char)((num[0] << 2) + (num[1] >> 4));
//		out[1] = (char)((num[1] << 4) + (num[2] >> 2));
//		out[2] = (char)((num[2] << 6) + num[3]);
//		result.AppendChar(out[0]);
//		if (byte[2] != '=')
//		{
//			result.AppendChar(out[1]);
//			if (byte[3] != '=')
//				result.AppendChar(out[2]);
//		}
//		//j = 0;
//	}
//	
//}