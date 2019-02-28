/*
 * DSRCPrvMsg.cpp
 *
 *  Created on: 2017年11月22日
 *  Author: wyz
 *  Description: 
 *
 */

#include "DSRCPrvMsg.h"

DSRCPrvMsg::DSRCPrvMsg() {
	// TODO Auto-generated constructor stub

}

DSRCPrvMsg::~DSRCPrvMsg() {
	// TODO Auto-generated destructor stub
}

void DSRCPrvMsg::DSRCSend(char* buf, int length)
{
	UDPCommTool::UDPSend("127.0.0.1", c_PRIVATE_MSG_DOWN_PORT, buf, length);
}


int DSRCPrvMsg::start_thr_DSRCReceive(void* (*func) (char*, struct sockaddr*, int))
{
	UDPCommTool::s_ListnPara lp;

	lp.port = c_PRIVATE_MSG_UP_PORT;
	lp.func = (*func);
	int res = ThreadTool::createNewThread(UDPCommTool::thr_Litsen2Port, (void*)&lp);
	if (res < 0){
		return -1;//pthread_create failed
	}

	return 1;
}
