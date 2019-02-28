/*
 * EngrDevBase.cpp
 *
 *  Created on: 2017年11月27日
 *  Author: wyz
 *  Description: 
 *
 */

#include "EngrDevBase.h"

EngrDevBase::EngrDevBase(struct sockaddr_in* add) {

	memcpy(&dest_add, add, sizeof(struct sockaddr_in));
	refreshTime();
}

EngrDevBase::~EngrDevBase() {
	// TODO Auto-generated destructor stub

}

bool EngrDevBase::equals(struct sockaddr_in* add)
{
	if(add != NULL && add->sin_addr.s_addr == dest_add.sin_addr.s_addr && add->sin_port == dest_add.sin_port)
		return true;

	return false;
}

int EngrDevBase::udpSend(char* buf, int len)
{
	return UDPCommTool::UDPSend(&dest_add, buf, len);
}
