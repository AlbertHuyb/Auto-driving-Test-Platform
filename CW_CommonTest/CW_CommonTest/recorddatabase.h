#ifndef _database_h_
#define _database_h_

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
#include "cw_lib/core/base/GeoCoordinates.h"
#include "TestInfo.h"

using namespace std;
#pragma pack(1)

class recorddatabase {
public:
	char boxID[c_BOX_ID_LEN];
	float pdr1;
	float pdr2;
	int latency2;
	int receive_interval1;
	int receive_interval2;
	float box_distance;
	double last_time;

	vector<int> rx_id1;
	vector<int> rx_id2;

	double last_rx_time1;
	double last_rx_time2;
public:
	recorddatabase();
	recorddatabase(char* ID);
	~recorddatabase();

public:
	int handleRxMsg(TestInfo::s_InfoRes* pres);
};

#endif

