/*
 * EngrDevMng.cpp
 *
 *  Created on: 2017年11月27日
 *  Author: wyz
 *  Description: 
 *
 */

#include "EngrDevMng.h"

vector<EngrDevBase> EngrDevMng::engr_devs;
UDPCommTool::s_ListnPara EngrDevMng::para;
pthread_mutex_t EngrDevMng::mt;

EngrDevMng::EngrDevMng() {
	// TODO Auto-generated constructor stub

}

EngrDevMng::~EngrDevMng() {
	// TODO Auto-generated destructor stub
}

void* EngrDevMng::engrDevHandler (char* buf, struct sockaddr* add, int len)
{
	VI2CFrame frame;
	int index = 0;

	// This is a legal message
	if(frame.resolveFrame(buf, &index, len) < 0 || frame.type != 0x21)
	{
		return 0;
	}
		

	pthread_mutex_lock(&mt);
	// An old register
	for(unsigned int i=0; i<engr_devs.size(); i++)
	{
		if(engr_devs[i].equals((struct sockaddr_in*)add))
		{
			engr_devs[i].refreshTime();
			pthread_mutex_unlock(&mt);
			return 0;
		}
	}

	// A new register
	EngrDevBase newdev((struct sockaddr_in*)add);
	engr_devs.push_back(newdev);
	pthread_mutex_unlock(&mt);

	return 0;
}

int EngrDevMng::start_thr_EngrDevListener()
{
	pthread_mutex_init(&mt, NULL);

	para.port = c_ENGINEER_TEST_PORT;
	para.func = engrDevHandler;

	return ThreadTool::createNewThread(UDPCommTool::thr_Litsen2Port, (void*)&para);
}

int EngrDevMng::send2EngrDevs(char* data, int len)
{
	char buf[2048];
	int index = 0;

	VI2CFrame frame;

	frame.ver = 1;
	frame.type = 0x22;
	frame.dataLen = len;
	memcpy(frame.data, data, len);

	frame.buildFrame(buf, &index);

	vector<EngrDevBase>:: iterator it;

	pthread_mutex_lock(&mt);
	for(it=engr_devs.begin();it!=engr_devs.end();)
	{
		 if(it->isPreciseOverTime(c_ENGR_DEV_EXPIRE_TIME))
			  it=engr_devs.erase(it);
		 else
		 {
			 it->udpSend(buf, index);
			 ++it;
		 }
	}

	pthread_mutex_unlock(&mt);

	return 1;
}
