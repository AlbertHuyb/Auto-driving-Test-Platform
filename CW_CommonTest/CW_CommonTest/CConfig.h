/*
 * CConfig.h
 *
 * Created on: 2016年7月27日
 * Author: smk
 *
 */

#ifndef CCONFIG_H_
#define CCONFIG_H_

#include <string>
#include <strings.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string.h>

using namespace std;
#pragma pack(1)

class Config{
	private:
	static ifstream configfile;

	public:
	static string CommType;
	static string DestIPAddr;
	static short DestPort;
	static short LocalPort;
	static long SendFrequency;
	static short PacketLength;
	static char TotalHops;
	static string tx_flag;
	static string rx_flag;
	static string cohda;
	
	public:
	static int LoadConfig();
};

#endif /* CCONFIG_H_ */

