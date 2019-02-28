/*
 * TestCtrlBase.cpp
 *
 *  Created on: 2017年9月17日
 *      Author: wyz
 */

#include "TestCtrlBase.h"

TestCtrlBase::TestCtrlBase() {
	// TODO Auto-generated constructor stub
	sequence = 0;
	totalHops = 0;
	hopsDone = 0;
}

TestCtrlBase::~TestCtrlBase() {
	// TODO Auto-generated destructor stub
}

int TestCtrlBase::Serialize(char* buf, int* startindex, int length)
{
	int index = 0;

	try{
		index = *startindex;
		if(SerializeTool::Int2Byte(buf, &index, sequence) < 0) return -1;
		buf[index++] = totalHops;
		buf[index++] = hopsDone;
	}catch(exception &e){
		return -1;
	}
	int len = index - *startindex;
	*startindex = index;

	return len;
}

int TestCtrlBase::DeSerialize(char* buf, int* startindex, int length)
{
	int index = 0;

	try{
		index = *startindex;
		if(SerializeTool::Byte2Int(buf, &index, &sequence) < 0) return -1;
		totalHops = buf[index++];
		hopsDone = buf[index++];
	}catch(exception &e) {
		return -1;
	}
	int len = index - *startindex;
	*startindex = index;

	return len;
}

