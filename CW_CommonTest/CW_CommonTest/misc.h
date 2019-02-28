#ifndef _misc_h_
#define _misc_h_

#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

#pragma pack(1)

class Miscellaneous  
{
public:
	Miscellaneous();
	~Miscellaneous();
public:
	static unsigned char ORCheck(unsigned char *pBuf, unsigned int BufLen);
};

#pragma pack()

#endif
