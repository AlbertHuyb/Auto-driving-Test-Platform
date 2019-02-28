/*
 * BoxInfoBase.h
 *
 *  Created on: 2017年9月17日
 *      Author: wyz
 */

#ifndef BOXINFOBASE_H_
#define BOXINFOBASE_H_

#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <sys/time.h>
#include "LocalConstants.h"
#include "cw_lib/tools/SerializeTool.h"
#include "cw_lib/tools/TimeTool.h"
#include "cw_lib/using/device/DevConfigMng.h"
#include "cw_lib/using/device/DevGNSSMng.h"

using namespace std;
#pragma pack(1)

class BoxInfoBase {
public:
	BoxInfoBase();
	virtual ~BoxInfoBase();

public:
	char boxID[c_BOX_ID_LEN];
	double boxLon;
	double boxLat;
	float boxSpeed;
	float boxSpeedAngle;
	float boxheading;
	unsigned char positioning_state;
	unsigned char satellite_num;
	float hdop;
	char msgstate;
	double msgtime;
	char HopsDone;
public:
	int Serialize(char* buf, int* startindex, int length);
	int DeSerialize(char* buf, int* startindex, int length);
	int setBoxInfoNow();
	int setTimeNow();
	int setTxStateNow();
	int setRxStateNow();
	string BoxInfo2string(int buflen, int check_flag);
};

#endif /* BOXINFOBASE_H_ */

