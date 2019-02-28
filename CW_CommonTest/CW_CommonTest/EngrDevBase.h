/*
 * EngrDevBase.h
 *
 *  Created on: 2017年11月27日
 *  Author: wyz
 *  Description: 
 *
 */

#ifndef ENGRDEVBASE_H_
#define ENGRDEVBASE_H_

#include "cw_lib/core/base/IFRefreshable.h"
#include "cw_lib/tools/UDPCommTool.h"
#include <vector>
#include "cw_lib/tools/ThreadTool.h"

#pragma pack(1)
using namespace std;

class EngrDevBase : public IF_Refreshable {
public:
	EngrDevBase(struct sockaddr_in* add);
	~EngrDevBase();

private:
	// Engineer device socket address
	struct sockaddr_in dest_add;

public:
	// Compare engineer devices by their socket address
	bool equals(struct sockaddr_in* add);
	// Send function
	int udpSend(char* buf, int len);

};

#endif /* ENGRDEVBASE_H_ */
