/*
 * TestCtrlBase.h
 *
 *  Created on: 2017年9月17日
 *      Author: wyz
 */

#ifndef TESTCTRLBASE_H_
#define TESTCTRLBASE_H_

#include "cw_lib/tools/SerializeTool.h"

using namespace std;
#pragma pack(1)

class TestCtrlBase {
public:
	TestCtrlBase();
	virtual ~TestCtrlBase();

public:
	int sequence;
	char totalHops;
	char hopsDone;

public:
	int Serialize(char* buf, int* startindex, int length);
	int DeSerialize(char* buf, int* startindex, int length);
};

#endif /* TESTCTRLBASE_H_ */

