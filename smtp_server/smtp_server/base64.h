#pragma once
#include "stdafx.h"
#include <iostream>
using namespace std;

CString Decode_base64(CString str)
{
	int DecodeTable[123];
	//初始化解码表
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
	unsigned int num[4];//存储字符在编码表数组中的下标
	CString result;

	int i = 0;
	while (i < str.GetLength())//一次读入四个字符
	{
		curr = str.GetAt(i);
		if (curr != '\r' && curr != '\n')
		{
			for (int j = 0; j < 4; j++)
			{
				byte[j] = curr;

				if (j == 3)
				{
					//  字符串      a       b        c
					//	ASCII      97      98       99
					//	8bit   01100001 01100010 01100011
					//	6bit   011000   010110   001001   100011
					//	十进制      24      22        9        35
					//	对应编码    Y        W        J        j

					num[0] = DecodeTable[(int)byte[0]];
					num[1] = DecodeTable[(int)byte[1]];
					num[2] = DecodeTable[(int)byte[2]];
					num[3] = DecodeTable[(int)byte[3]];

					out[0] = (char)((num[0] << 2) + (num[1] >> 4));
					out[1] = (char)((num[1] << 4) + (num[2] >> 2));
					out[2] = (char)((num[2] << 6) + num[3]);

					result.AppendChar(out[0]);

					if (byte[2] != '=')
					{
						result.AppendChar(out[1]);
						if (byte[3] != '=')
							result.AppendChar(out[2]);
					}
					//j = 0;
				}
			}
		}
		i++;
	}
	return result;
}
