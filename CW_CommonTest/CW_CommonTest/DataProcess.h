/*
 * DataProcess.h
 *
 *  Created on: 2017年9月16日
 *      Author: wyz
 */

#ifndef DATAPROCESS_H_
#define DATAPROCESS_H_

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <string>
#include "BoxInfoBase.h"
#include "TestInfo.h"
#include "TestMng.h"
#include "cw_lib/tools/TextFileTool.h"
#include "LocalConstants.h"
#include "testtool.h"
#include "TestMng.h"
#include "misc.h"

using namespace std;
#pragma pack(1)

typedef struct
{
	char *p_buf;
	int buf_len;	
} msg_info;

class DataProcess {
private:
	static TextFileAppendTool log_record;
	static pthread_mutex_t receive_mutex;
	static pthread_mutex_t writelog_mutex;
	static sem_t parameter_sem;
public:
	static Testtool TT;
public:
	static msg_info _msg_info;
	static int init(void);
	static int formTestMsg(char* buf, int startindex, int length);
	static void *resolveTestMsg(char* buf, struct sockaddr* sender, int length);
	static int formTestReply(char* buf, int startindex, int length, TestInfo ti, int packet_len);
	static void *analytical_data(void* args);
};

#endif /* DATAPROCESS_H_ */

