/*
 * TestMng.cpp
 *
 *  Created on: 2017年9月16日
 *      Author: wyz
 */

#include "TestMng.h"

pthread_mutex_t TestMng::mutex_tx;
pthread_mutex_t TestMng::mutex_record;
IF_DSRCDev* TestMng::pDEV = 0;

int TestMng::init()
{
	// init device
	if(strcmp((const char*)Config::CommType.c_str(), "11p") == 0)
		pDEV = new DSRCLocalDev11p();
	else if(strcmp((const char*)Config::CommType.c_str(), "cwave") == 0)
		pDEV = new DSRCLocalDevIPv6();
	else if(strcmp((const char*)Config::CommType.c_str(), "net") == 0)
		pDEV = new DSRCNetDevice((char*)Config::DestIPAddr.c_str(), Config::DestPort, Config::LocalPort);
	else if(strcmp((const char*)Config::CommType.c_str(), "prvmsg") ==0)
		pDEV = new DSRCPrvMsg();
	else
		return -1;

	// init send mutex
	pthread_mutex_init(&mutex_tx, NULL);
	pthread_mutex_init(&mutex_record, NULL);

	// start_thr_send
	if(strcmp((const char*)Config::tx_flag.c_str(), "yes") == 0)
	{
		s_SendPara sendpara;
		sendpara.formBuf = DataProcess::formTestMsg;
		sendpara.sleepMicroSec = Config::SendFrequency;
		sendpara.sendBuf = Send;
		ThreadTool::createNewThread(thr_Send, (void*)&sendpara);
	}
	// start UPD
	ThreadTool::createNewThread(pad_Send, NULL);
	
	// start_thr_recv
	if(strcmp((const char*)Config::rx_flag.c_str(), "yes") == 0)
		pDEV->start_thr_DSRCReceive(DataProcess::resolveTestMsg);

	return 1;
}

void* TestMng::thr_Send(void* args)
{
	s_SendPara* sendpara1 = (s_SendPara*)args;

	s_SendPara sendpara;

	memcpy(&sendpara, sendpara1, sizeof(s_SendPara));

	sendpara.sleepMicroSec = sendpara1->sleepMicroSec;
	sendpara.formBuf = sendpara1->formBuf;
	sendpara.sendBuf = sendpara1->sendBuf;
	sendpara.p_buf = sendpara1->p_buf;

	char buf[c_NORMAL_BUF_LEN];
	int length = 0;

	while(1)
	{
		bzero(buf, c_NORMAL_BUF_LEN);
		length = sendpara.formBuf(buf, 0, c_NORMAL_BUF_LEN);
		if(length > 0)
			sendpara.sendBuf(buf, length);
		usleep(sendpara.sleepMicroSec);
	}

	return NULL;
}

int TestMng::Send(char* buf, int length)
{
	try{
		pthread_mutex_lock(&mutex_tx);
		pDEV->DSRCSend(buf, length);
		pthread_mutex_unlock(&mutex_tx);
		
	}catch (exception &e){
		pthread_mutex_unlock(&mutex_tx);
		return -1;
	}

	return 1;
}

void* TestMng::pad_Send(void* args)
{
	int index = 0;
	char txbuf[2048] = "";
	while(1)
	{
		index = 0;
		memset(txbuf, 0x00, sizeof(txbuf));
		pthread_mutex_lock(&mutex_record);
		txbuf[index++] = DataProcess::TT.displaydata.size();
		map<string, recorddatabase>::iterator it;
		for(it = DataProcess::TT.displaydata.begin(); it != DataProcess::TT.displaydata.end(); it++)
		{
			double msgtime = 0.0;
			msgtime = TimeTool::getPreciseTimeNow();

			memcpy(txbuf+index, it->second.boxID, c_BOX_ID_LEN);
			index += c_BOX_ID_LEN;

			if(SerializeTool::Float2Byte(txbuf, &index, it->second.pdr1) < 0) return NULL;
			if(SerializeTool::Float2Byte(txbuf, &index, it->second.pdr2) < 0) return NULL;
			if(SerializeTool::Int2Byte(txbuf, &index, it->second.latency2) < 0) return NULL;
			if(SerializeTool::Int2Byte(txbuf, &index, it->second.receive_interval1) < 0) return NULL;
			if(SerializeTool::Int2Byte(txbuf, &index, it->second.receive_interval2) < 0) return NULL;
			if(SerializeTool::Float2Byte(txbuf, &index, it->second.box_distance) < 0) return NULL;
			if(SerializeTool::Double2Byte(txbuf, &index, msgtime - it->second.last_time) < 0) return NULL;
		}
		pthread_mutex_unlock(&mutex_record);
		EngrDevMng::send2EngrDevs(txbuf, index);
		sleep(1);
	}
	return NULL;
}

