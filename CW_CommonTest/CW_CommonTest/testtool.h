#ifndef _testtool_h_
#define _testtool_h_

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
#include "CConfig.h"
#include "recorddatabase.h"
#include "TestInfo.h"

using namespace std;
#pragma pack(1)

class Testtool {
public:
	map<string, recorddatabase> displaydata;

public:
	Testtool();
	~Testtool();
};

#endif /* TESTINFO_H_ */

