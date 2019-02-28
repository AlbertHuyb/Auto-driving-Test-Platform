/*
 * DSRCPrvMsg.h
 *
 *  Created on: 2017年11月22日
 *  Author: wyz
 *  Description: 
 *
 */

#ifndef DSRCPRVMSG_H_
#define DSRCPRVMSG_H_

#include "DSRCComm/IFDSRCDev.h"
#include "cw_lib/tools/UDPCommTool.h"
#include "cw_lib/tools/ThreadTool.h"

// port: Platform recv private msg and broadcast to DSRC
#define c_PRIVATE_MSG_DOWN_PORT	11251
// port: Platform send private msg received from DSRC up to local application
#define c_PRIVATE_MSG_UP_PORT 11252

using namespace std;
#pragma pack(1)

class DSRCPrvMsg : public IF_DSRCDev {
public:
	DSRCPrvMsg();
	~DSRCPrvMsg();

public:
	// Send buffer through this socket
	void DSRCSend(char* buf, int length);

	// Satrt receive thread
	int start_thr_DSRCReceive(void* (*func) (char*, struct sockaddr*, int));
};

#endif /* DSRCPRVMSG_H_ */
