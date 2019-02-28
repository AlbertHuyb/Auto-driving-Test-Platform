/*
 * EngrDevMng.h
 *
 *  Created on: 2017年11月27日
 *  Author: wyz
 *  Description: 
 *
 */

#ifndef ENGRDEVMNG_H_
#define ENGRDEVMNG_H_

#include "EngrDevBase.h"
#include "cw_lib/shared/VI2CFrame.h"
#include <pthread.h>

#define c_ENGINEER_TEST_PORT 11300
#define c_ENGR_DEV_EXPIRE_TIME 120

#pragma pack(1)
using namespace std;

class EngrDevMng {

public:
	EngrDevMng();
	~EngrDevMng();

private:
	// List of engineer devices
	static vector<EngrDevBase> engr_devs;
	// Receive thread parameters
	static UDPCommTool::s_ListnPara para;
	// mutex of the vector above
	static pthread_mutex_t mt;

private:
	// Callback function of UDP Receive
	static void* engrDevHandler (char*, struct sockaddr*, int);

public:
	// Initialization and start engineer device listener
	static int start_thr_EngrDevListener();
	// Send data 2 engineer devices, integrate the frame function
	static int send2EngrDevs(char*, int);
};

#endif /* ENGRDEVMNG_H_ */
