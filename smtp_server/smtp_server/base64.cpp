//#include <iostream>
//#include <string>
//#include <cstring>
#include "stdafx.h"
//#include "base64.h"
//
//using namespace std;
//
////string Base64::Decode(const char *str, int length)
//CString Base64::Decode(CString str) 
//{
//	CString result;
//	int DecodeTable[123];
//	//初始化解码表
//	for (int i = 0; i< 123; i++)
//	{
//		DecodeTable[i] = -1;
//		if(i == 43)
//			DecodeTable[i] = 62;//"+"
//		if(i == 47)
//			DecodeTable[i] = 63;//"/"
//		if (i >= 48 && i < 58)
//			DecodeTable[i] = i + 4;//0-9
//		if(i >= 65 && i < 91)
//			DecodeTable[i] = i - 65;//A-Z
//		if(i >= 97 && i < 123)
//			DecodeTable[i] = i - 71;//a-z
//	}
//	char first, second, third, fourth;
//	unsigned int  num1, num2, num3, num4;
//	char out1, out2, out3;
//	int step = 0;
//	int i = 0;
//	while (i<str.GetLength())   //一次读入四个字符
//	{
//		char temp = str.GetAt(i);
//		if (temp == '\r' || temp == '\n')
//		{
//		}
//		else
//		{
//			switch (step)
//			{
//			case 0:
//				first = temp;
//				step++;
//				break;
//			case 1:
//				second = temp;
//				step++;
//				break;
//			case 2:
//				third = temp;
//				step++;
//				break;
//			case 3:
//				fourth = temp;
//				num1 = DecodeTable[(int)first];
//				num2 = DecodeTable[(int)second];
//				num3 = DecodeTable[(int)third];
//				num4 = DecodeTable[(int)fourth];
//				out1 = (char)((num1 << 2) + (num2 >> 4));
//				out2 = (char)((num2 << 4) + (num3 >> 2));
//				out3 = (char)((num3 << 6) + num4);
//				result.AppendChar(out1);
//				if (third != '=')
//				{
//					result.AppendChar(out2);
//					if (fourth != '=')
//					{
//						result.AppendChar(out3);
//					}
//				}
//				step = 0;
//				break;
//			}
//		}
//		i++;
//	}
//	return result;
//	//int bin = 0, i = 0, pos = 0;
//	//string result;
//	//const char *current = str;
//	//char ch;
//	//while ((ch = *current++) != '\0' && length-- > 0)
//	//{
//	//	/*
//	//	在解码时，4个字符为一组进行一轮字符匹配。
//	//	两个条件：
//	//	1、如果某一轮匹配的第二个是“=”且第三个字符不是“=”，说明这个带解析字符串不合法，直接返回空
//	//	2、如果当前“=”不是第二个字符，且后面的字符只包含空白符，则说明这个这个条件合法，可以继续。
//	//	*/
//	//	if (ch == '=') { // 当前一个字符是“=”号
//	//		if (*current != '=' && (i % 4) == 1) {
//	//			return NULL;
//	//		}
//	//		continue;
//	//	}
//	//	ch = DecodeTable[ch];
//	//	//这个很重要，用来过滤所有不合法的字符
//	//	if (ch < 0) { /* a space or some other separator character, we simply skip over */
//	//		continue;
//	//	}
//	//	switch (i % 4)
//	//	{
//	//	case 0:
//	//		bin = ch << 2;
//	//		break;
//	//	case 1:
//	//		bin |= ch >> 4;
//	//		result += bin;
//	//		bin = (ch & 0x0f) << 4;
//	//		break;
//	//	case 2:
//	//		bin |= ch >> 2;
//	//		result += bin;
//	//		bin = (ch & 0x03) << 6;
//	//		break;
//	//	case 3:
//	//		bin |= ch;
//	//		result += bin;
//	//		break;
//	//	}
//	//	i++;
//	//}
//	//return result;
//}