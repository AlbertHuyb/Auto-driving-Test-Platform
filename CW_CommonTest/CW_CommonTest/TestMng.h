/*
 * TestMng.h
 *
 *  Created on: 2017年9月16日
 *      Author: wyz
 */

#ifndef TESTMNG_H_
#define TESTMNG_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "DSRCComm/DSRCLocalDevIPv6.h"
#include "DSRCComm/DSRCLocalDev11p.h"
#include "DSRCComm/DSRCNetDevice.h"
#include "CConfig.h"
#include "LocalConstants.h"
#include "cw_lib/tools/ThreadTool.h"
#include "cw_lib/tools/SerializeTool.h"
#include "DataProcess.h"
#include "DSRCComm/IFDSRCDev.h"
#include <pthread.h>
#include "DSRCPrvMsg.h"
#include "EngrDevMng.h"

using namespace std;
#pragma pack(1)

class TestMng {
public:
	typedef struct SendPara
	{
		char *p_buf;
		long sleepMicroSec;
		int (*formBuf) (char*, int, int);// para:1.sendBuf;2.beginIndex;3.maxSendLength
		int (*sendBuf) (char*, int);// para:1.sendBuf;2.SendLength
//		int (*handledisconnection) (int);//para: fd, handle disconnection event
	}s_SendPara;

private:
	static IF_DSRCDev* pDEV;
	static pthread_mutex_t mutex_tx;
	
public:
	static pthread_mutex_t mutex_record;
	static int init();
	static int Send(char* buf, int length);

private:
	static void *thr_Send(void* args);
	static void* pad_Send(void* args);
};

#endif /* TESTMNG_H_ */

