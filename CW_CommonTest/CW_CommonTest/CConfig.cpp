/*
 * CConfig.cpp
 *
 * Created on: 2016年7月27日
 * Author: smk
 */

#include "CConfig.h"

//Initialize the static member
ifstream Config::configfile;
string Config::CommType;
string Config::DestIPAddr;
short Config::DestPort;
short Config::LocalPort;
long Config::SendFrequency;
short Config::PacketLength;
char Config::TotalHops;
string Config::tx_flag;
string Config::rx_flag;
string Config::cohda;

int Config::LoadConfig()
{
	DestIPAddr = "";
	DestPort = 0;
	SendFrequency = 0;
	PacketLength = 0;

	if(configfile.is_open() == true)
		return -1;

	configfile.open("/etc/CommTest.conf", ios::binary);

	if(configfile.is_open() != true)
		return -2;

	string str;
	char buf[100];
	char buf1[100];
	char equal;

	while(!configfile.eof())
	{
		memset(buf, 0, 100);
		memset(buf1, 0, 100);

		configfile.get(buf, 100, '=');

		configfile.get(equal);

		if(strcmp((const char*)buf, "CommType") == 0)
		{
			configfile.getline(buf1, 100);
			CommType = buf1;
		}
		else if(strcmp((const char*)buf, "DestIPAddr") == 0)
		{
			configfile.getline(buf1, 100);
			DestIPAddr = buf1;
		}
		else if(strcmp((const char*)buf, "DestPort") == 0)
		{
			configfile.getline(buf1, 100);
			DestPort = (short)std::atoi((const char*)buf1);
		}
		else if(strcmp((const char*)buf, "LocalPort") == 0)
		{
			configfile.getline(buf1, 100);
			LocalPort = (short)std::atoi((const char*)buf1);
		}
		else if(strcmp((const char*)buf, "SendFrequency") == 0)
		{
			configfile.getline(buf1, 100);
			SendFrequency = (long)std::atoi((const char*)buf1);
		}
		else if(strcmp((const char*)buf, "PacketLength") == 0)
		{
			configfile.getline(buf1, 100);
			PacketLength = (short)std::atoi((const char*)buf1);
		}
		else if(strcmp((const char*)buf, "TotalHops") == 0)
		{
			configfile.getline(buf1, 100);
			int temp = std::atoi((const char*)buf1);
			TotalHops = (char)temp;
		}
		else if(strcmp((const char*)buf, "tx_flag") == 0)
		{
			configfile.getline(buf1, 100);
			tx_flag = buf1;
		}
		else if(strcmp((const char*)buf, "rx_flag") == 0)
		{
			configfile.getline(buf1, 100);
			rx_flag = buf1;
		}
		else if(strcmp((const char*)buf, "cohda") == 0)
		{
			configfile.getline(buf1, 100);
			cohda = buf1;
		}
		else
			configfile.getline(buf1, 100);
	}

	cout<<"dest ip addr:"<<DestIPAddr<<endl;
	cout<<"Send frequency:"<<SendFrequency<<endl;
	int totalhops = (int)TotalHops;
	cout<<"TotalHops in number:"<<totalhops<<endl;
	cout<<"TotalHops in ascii:"<<TotalHops<<endl;
	if(DestIPAddr == "")
		return -3;
	if(DestPort == 0)
		return -4;
	if(SendFrequency == 0)
		return -5;
	if(PacketLength == 0)
		return -6;

	return 0;
}

