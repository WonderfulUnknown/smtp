#pragma once

#include <iostream>
using namespace std;
class Base64 {
public:
	string encode_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	//������unsigned char*���ͣ�����������ĵ�ʱ�����
	string Encode(const unsigned char * str, int bytes);
	string Decode(const char *str, int bytes);
};