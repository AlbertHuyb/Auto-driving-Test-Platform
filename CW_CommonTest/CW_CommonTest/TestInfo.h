/*
 * TestInfo.h
 *
 *  Created on: 2017年9月17日
 *      Author: wyz
 */

#ifndef TESTINFO_H_
#define TESTINFO_H_

#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include "TestCtrlBase.h"
#include "BoxInfoBase.h"
#include "LocalConstants.h"
#include <vector>
#include "cw_lib/using/device/DevConfigMng.h"
#include "cw_lib/using/device/DevGNSSMng.h"
#include "CConfig.h"
#include "cw_lib/tools/SpaceToolGeo.h"
#include "cw_lib/tools/TimeTool.h"

using namespace std;
#pragma pack(1)

class TestInfo {
public:
	typedef struct InfoRes{
		int type;
		char boxID[c_BOX_ID_LEN];
		int msgId;
		double latency2;
		double rxTime;
		double distance;
	}s_InfoRes;

public:
	TestInfo();
	virtual ~TestInfo();
public:
	short msglen;
	TestCtrlBase tcb;
	vector<BoxInfoBase> transferbibs;
public:
	static int sequence;
public:
	int init();

	int Serialize(char* buf, int* startindex, int length);
	int DeSerialize(char* buf, int* startindex, int length);

	string TI2string();

	int getTestInfoRes(s_InfoRes* res);
};

#endif /* TESTINFO_H_ */

