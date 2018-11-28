#pragma once

#include <iostream>
using namespace std;
class Base64 {
public:
	string encode_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	//必须是unsigned char*类型，否则编码中文的时候出错
	string Encode(const unsigned char * str, int bytes);
	string Decode(const char *str, int bytes);
};