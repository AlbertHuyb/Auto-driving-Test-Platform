/*
 * BoxInfoBase.cpp
 *
 *  Created on: 2017年9月17日
 *      Author: wyz
 */

#include "BoxInfoBase.h"

BoxInfoBase::BoxInfoBase() {
	// TODO Auto-generated constructor stub

	bzero(boxID,c_BOX_ID_LEN );
	boxLon = 0;
	boxLat = 0;
	boxSpeed = 0;
	boxSpeedAngle = 0;
	boxheading = 0;
	positioning_state = 0;
	satellite_num = 0;
	hdop = 0;
	msgstate = 0;
	msgtime = 0;
	HopsDone = 0;
}

BoxInfoBase::~BoxInfoBase() {
	// TODO Auto-generated destructor stub
}

int BoxInfoBase::Serialize(char* buf, int* startindex, int length)
{
	int index = 0;

	try{
		index = *startindex;
	
		for(int i = 0; i < c_BOX_ID_LEN;i++)
			buf[index++] = boxID[i];
		if(SerializeTool::Double2Byte(buf, &index, boxLon) < 0) return -1;
		if(SerializeTool::Double2Byte(buf, &index, boxLat) < 0) return -1;
		if(SerializeTool::Float2Byte(buf, &index, boxSpeed) < 0) return -1;
		if(SerializeTool::Float2Byte(buf, &index, boxSpeedAngle) < 0) return -1;
		if(SerializeTool::Float2Byte(buf, &index, boxheading) < 0) return -1;
		buf[index++] = positioning_state;
		buf[index++] = satellite_num;
		if(SerializeTool::Float2Byte(buf, &index, hdop) < 0) return -1;
		buf[index++] = msgstate;
		if(SerializeTool::Double2Byte(buf, &index, msgtime) < 0) return -1;
		buf[index++] = HopsDone;
		
	}catch(exception &e){
		return -1;
	}
	int len = index - *startindex;
	*startindex = index;

	return len;
}

int BoxInfoBase::DeSerialize(char* buf, int* startindex, int length)
{
	int index = 0;

	try{
		index = *startindex;

		for(int i = 0; i < c_BOX_ID_LEN;i++)
			boxID[i] = buf[index++];
		if(SerializeTool::Byte2Double(buf, &index, &boxLon) < 0) return -1;
		if(SerializeTool::Byte2Double(buf, &index, &boxLat) < 0) return -1;
		if(SerializeTool::Byte2Float(buf, &index, &boxSpeed) < 0) return -1;
		if(SerializeTool::Byte2Float(buf, &index, &boxSpeedAngle) < 0) return -1;
		if(SerializeTool::Byte2Float(buf, &index, &boxheading) < 0) return -1;
		positioning_state = buf[index++];
		satellite_num = buf[index++];
		if(SerializeTool::Byte2Float(buf, &index, &hdop) < 0) return -1;
		msgstate = buf[index++];
		if(SerializeTool::Byte2Double(buf, &index, &msgtime) < 0) return -1;
		HopsDone = buf[index++];
		
	}catch(exception &e){
		return -1;
	}

	int len = index - *startindex;
	*startindex = index;

	return len;
}

int BoxInfoBase::setBoxInfoNow()
{
	try{
		DevConfigMng dcm(false);
		dcm.update();
		dcm.getValueByKey("DeviceID", (void*)boxID);
		DevGNSSMng dgm(false);
		dgm.update();
		boxLon = dgm.gn.geo.longitude;
		boxLat = dgm.gn.geo.latitude;
		boxSpeed = dgm.gn.speed;
		boxSpeedAngle = dgm.gn.heading;
		boxheading = dgm.gn.geo.height;
		positioning_state = dgm.gn.positioning_state;
		satellite_num = dgm.gn.satellite_num;
		hdop = dgm.gn.hdop;
		
	}catch(exception &e){
		return -1;
	}
	return 0;
}

int BoxInfoBase::setTimeNow()
{
	try{
		msgtime = TimeTool::getPreciseTimeNow();
	}catch(exception &e){
		return -1;
	}
	return 0;
}

int BoxInfoBase::setTxStateNow()
{
	try{
		msgstate = 1;
	}catch(exception &e){
		return -1;
	}
	return 0;
}

int BoxInfoBase::setRxStateNow()
{
	try{
		msgstate = 2;
	}catch(exception &e){
		return -1;
	}
	return 0;
}

string BoxInfoBase::BoxInfo2string(int buflen, int check_flag)
{
	string var = "";
	stringstream ss;

	for(int i = 0; i < c_BOX_ID_LEN; i++)
		ss << boxID[i];
	ss << ",";
	ss << setprecision(15) << boxLon << ",";
	ss << setprecision(15) << boxLat << ",";
	ss << setprecision(15) << boxSpeed << ",";
	ss << setprecision(15) << boxSpeedAngle << ",";
	ss << setprecision(15) << boxheading << ",";
	short tempshort = 0;
	tempshort = (short)positioning_state;
	ss << tempshort << ",";
	tempshort = (short)satellite_num;
	ss << tempshort << ",";
	ss << setprecision(15) << hdop << ",";
	ss << setprecision(15) << msgtime << ",";
	ss << buflen << ",";
	ss << check_flag << ",";
	
	ss >> var;	
	
	return var;
}

