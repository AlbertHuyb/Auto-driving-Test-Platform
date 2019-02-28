/*
 * TestInfo.cpp
 *
 *  Created on: 2017年9月17日
 *      Author: wyz
 */

#include "TestInfo.h"

int TestInfo::sequence = 0;

TestInfo::TestInfo() {
	// TODO Auto-generated constructor stub
	msglen = 0;
}

TestInfo::~TestInfo() {
	// TODO Auto-generated destructor stub
}

int TestInfo::init()
{
	try{
		tcb.sequence = sequence++;
		tcb.totalHops = Config::TotalHops;
		tcb.hopsDone = 0;

		BoxInfoBase bib;
		DevConfigMng dcm(false);
		dcm.update();
		dcm.getValueByKey("DeviceID", bib.boxID);
		DevGNSSMng dgm(false);
		dgm.update();
		bib.boxLon = dgm.gn.geo.longitude;
		bib.boxLat = dgm.gn.geo.latitude;
		bib.boxSpeed = dgm.gn.speed;
		bib.boxSpeedAngle = dgm.gn.heading;
		bib.boxheading = dgm.gn.geo.height;
		bib.positioning_state = dgm.gn.positioning_state;
		bib.satellite_num = dgm.gn.satellite_num;
		bib.hdop = dgm.gn.hdop;
		bib.msgstate = 1;//tx 
		bib.msgtime = TimeTool::getPreciseTimeNow();
		bib.HopsDone = 0;
		transferbibs.push_back(bib);
		
	}catch(exception &e){
		return -1;
	}
	return 0;
}

int TestInfo::Serialize(char* buf, int* startindex, int length)
{
	int index = 0;

	try{
		index = *startindex;

		if(tcb.Serialize(buf, &index, length) < 0) return -1;
		short num = transferbibs.size();
		if(SerializeTool::Short2Byte(buf, &index, num) < 0) return -1;
		for(int i = 0; i < num; i++)
			if(transferbibs[i].Serialize(buf, &index, length) < 0)
				return -1;

	}catch(exception &e) {
		return -1;
	}

	int len = index - *startindex;
	*startindex = index;

	return len;
}

int TestInfo::DeSerialize(char* buf, int* startindex, int length)
{
	int index = 0;
	BoxInfoBase bib;

	try{
		index = *startindex;

		if(tcb.DeSerialize(buf, &index, length) < 0) return -1;
		short num = 0;
		if(SerializeTool::Byte2Short(buf, &index, &num) < 0) return -1;
		for(int i = 0; i < num;i++)
		{
			if(bib.DeSerialize(buf, &index, length) < 0)
				return -1;
			transferbibs.push_back(bib);
		}

	}catch(exception &e) {
		return -1;
	}

	int len = index - *startindex;
	*startindex = index;

	return len;
}

int TestInfo::getTestInfoRes(s_InfoRes* res)
{
	short num = transferbibs.size();

	memcpy(res->boxID, transferbibs[num - 2].boxID, c_BOX_ID_LEN);

	GeoCoordinates geo0, geo1;
	geo0.longitude = transferbibs[num - 2].boxLon;
	geo0.latitude = transferbibs[num - 2].boxLat;
	geo1.longitude = transferbibs[num - 1].boxLon;
	geo1.latitude = transferbibs[num - 1].boxLat;
	res->distance = SpaceToolGeo::DistancePoint2Point(geo0, geo1);

	res->msgId = tcb.sequence;

	res->rxTime = transferbibs[num - 1].msgtime;

	res->latency2 = -1;

	if(num >= 4)
	{
		char boxID[c_BOX_ID_LEN] = "";
		DevConfigMng dcm(false);
		dcm.update();
		dcm.getValueByKey("DeviceID", boxID);
		if(strncmp(boxID, transferbibs[num - 4].boxID, c_BOX_ID_LEN) == 0)
		{
			res->type = 2;
			res->latency2 = transferbibs[num - 1].msgtime - transferbibs[num - 4].msgtime;
		}
		else
		{
			res->type = 1;
		}
	}
	else
	{
		res->type = 1;
	}

	return 1;
}

string TestInfo::TI2string()
{
	string var = "";
	stringstream ss;

	ss << msglen << ",";
	ss << tcb.sequence << ",";
	short tempshort = 0;
	tempshort = (short)tcb.totalHops;
	ss << tempshort << ",";
	tempshort = (short)tcb.hopsDone;
	ss << tempshort << ",";
	
	for(unsigned int i = 0; i < transferbibs.size(); i++)
	{
		for(int j = 0; j < c_BOX_ID_LEN; j++)
			ss << transferbibs[i].boxID[j];
		ss << ",";
		ss << setprecision(15) << transferbibs[i].boxLon << ',';
		ss << setprecision(15) << transferbibs[i].boxLat << ',';
		ss << setprecision(15) << transferbibs[i].boxSpeed << ',';
		ss << setprecision(15) << transferbibs[i].boxSpeedAngle << ',';
		ss << setprecision(15) << transferbibs[i].boxheading << ',';
		tempshort = (short)transferbibs[i].positioning_state;
		ss << tempshort << ",";
		tempshort = (short)transferbibs[i].satellite_num;
		ss << tempshort << ",";
		ss << setprecision(15) << transferbibs[i].hdop << ",";
		tempshort = (short)transferbibs[i].msgstate;
		ss << tempshort << ",";
		ss << setprecision(15) << transferbibs[i].msgtime << ",";
		tempshort = (short)transferbibs[i].HopsDone;
		ss << tempshort << ",";
	}

	ss >> var;	
	
	return var;
}

