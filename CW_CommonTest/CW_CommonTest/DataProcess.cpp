/*
 * DataProcess.cpp
 *
 *  Created on: 2017年9月16日
 *      Author: wyz
 */

#include "DataProcess.h"

TextFileAppendTool DataProcess::log_record;
pthread_mutex_t DataProcess::receive_mutex;
pthread_mutex_t DataProcess::writelog_mutex;
sem_t DataProcess::parameter_sem;
msg_info DataProcess::_msg_info;
Testtool DataProcess::TT;

int DataProcess::init(void)
{
	pthread_mutex_init(&receive_mutex, NULL);
	pthread_mutex_init(&writelog_mutex, NULL);
	sem_init(&parameter_sem, 0, 0);
	
	char record_filename[128] = "";
	memset(record_filename, 0x00, sizeof(record_filename));
	struct timeval tv;
	gettimeofday(&tv, NULL);
	snprintf(record_filename, sizeof(record_filename), "/var/record/%s-%d.txt", c_PROC_NAME, (int)tv.tv_sec);
	log_record.init_of(record_filename);
	memset(record_filename, 0x00, sizeof(record_filename));
	memcpy(record_filename, "cp /etc/CommTest.conf ", sizeof("cp /etc/CommTest.conf "));
	char conf_filename[64] = "";
	memset(conf_filename, 0x00, sizeof(conf_filename));
	snprintf(conf_filename, sizeof(conf_filename), "/var/record/%s-%d.conf", c_PROC_NAME, (int)tv.tv_sec);
	strcat(record_filename, conf_filename);
	#if CONFIG_DEBUG
	cout << "record_filename:" << record_filename << endl;
	#endif
	return system(record_filename);
}

int DataProcess::formTestMsg(char* buf, int startindex, int length)
{	
	int index = startindex;

	TestInfo ti;	
	try{		
		buf[index++] = 0xFA;	
		buf[index++] = 0xFB;		//data value length	
		int lengthindex = index;	
		index += 2;
		
		ti.init();		
		short datavallen = 0;	
		datavallen = ti.Serialize(buf, &index, length);
		if(datavallen < 0) 
			return -1;
		if(SerializeTool::Short2Byte(buf, &lengthindex, datavallen) < 0)	
			return -1;
		buf[index++] = 0xEA;	
		buf[index++] = 0xEB;	
		
		if(index - startindex > Config::PacketLength - 5)// For cohda		
		{
			cout<<"Packet Length in Config is too small"<<endl;	
			return -2;		
		}
		for(; index - startindex < Config::PacketLength - 5;)			
			buf[index++] = 0x0a;
		buf[index++] = Miscellaneous::ORCheck((unsigned char *)&buf[startindex + 2], Config::PacketLength - 7);
		for(; index - startindex < Config::PacketLength;)			
			buf[index++] = 0x0a;
		ti.msglen = Config::PacketLength;

		string var = "";		
		var = ti.TI2string();
		if(var.length() > 0)
		{
			pthread_mutex_lock(&writelog_mutex);
			log_record.write_of(var);
			pthread_mutex_unlock(&writelog_mutex);
		}
			
	}
	catch(exception &e){	
		return -1;	
	}

	return index - startindex;
}

void* DataProcess::analytical_data(void* args)
{
	msg_info *p_msg_info = (msg_info *)args;
	int buf_len = 0, index = 0;
	char buf[2048] = "";

	buf_len = p_msg_info->buf_len;
	memcpy(buf, p_msg_info->p_buf, buf_len);	
	sem_post(&parameter_sem);

	TestInfo ti;
	
	if(buf[index] != 0xFA || buf[index + 1] != 0xFB)
	{
		BoxInfoBase othersbib;
		othersbib.setTimeNow();
		othersbib.setRxStateNow();
		othersbib.setBoxInfoNow();
		string var = "";
		var = othersbib.BoxInfo2string(buf_len, 0);
		if(var.length() > 0)
		{
			pthread_mutex_lock(&writelog_mutex);
			log_record.write_of(var);
			pthread_mutex_unlock(&writelog_mutex);
		}
		return NULL;
	}
	
	int rx_check_code = 0, cal_check_code = 0;
	if((strcmp((const char*)Config::CommType.c_str(), "net") == 0) && \
	   (strcmp((const char*)Config::cohda.c_str(), "yes") == 0))
	{
		rx_check_code = buf[buf_len - 1];
		cal_check_code = Miscellaneous::ORCheck((unsigned char *)&buf[2], buf_len - 3);
		buf_len += 4;
		ti.msglen = buf_len;
	}
	else
	{
		rx_check_code = buf[buf_len - 5];
		cal_check_code = Miscellaneous::ORCheck((unsigned char *)&buf[2], buf_len - 7);
		ti.msglen = buf_len;
	}
	
	if(rx_check_code != cal_check_code)
	{
		BoxInfoBase othersbib;
		othersbib.setTimeNow();
		othersbib.setRxStateNow();
		othersbib.setBoxInfoNow();
		string var = "";
		var = othersbib.BoxInfo2string(buf_len, 1);
		if(var.length() > 0)
		{
			pthread_mutex_lock(&writelog_mutex);
			log_record.write_of(var);
			pthread_mutex_unlock(&writelog_mutex);
		}
		return NULL;
	}
	
	BoxInfoBase rxbib;
	rxbib.setTimeNow();
	rxbib.setRxStateNow();
	rxbib.setBoxInfoNow();

	index += 2;
	short datavallen = 0;
	if(SerializeTool::Byte2Short(buf, &index, &datavallen) < 0)
		return NULL;
	if(ti.DeSerialize(buf, &index, datavallen) < 0)
		return NULL;
	
	ti.tcb.hopsDone++;
	rxbib.HopsDone = ti.tcb.hopsDone;
	ti.transferbibs.push_back(rxbib);
	
	TestInfo::s_InfoRes res;
	ti.getTestInfoRes(&res);

	char temp[c_BOX_ID_LEN + 1];
	bzero(temp, c_BOX_ID_LEN + 1);
	memcpy(temp, res.boxID, c_BOX_ID_LEN);
	string devId = temp;

	pthread_mutex_lock(&TestMng::mutex_record);
	if(TT.displaydata.count(devId) == 0)
	{
		recorddatabase newrdd(res.boxID);
		newrdd.handleRxMsg(&res);
		TT.displaydata[devId] = newrdd;
	}
	else
	{
		TT.displaydata[devId].handleRxMsg(&res);
	}
	pthread_mutex_unlock(&TestMng::mutex_record);

	int cal_flag = 0;
	if(ti.tcb.hopsDone < ti.tcb.totalHops)
	{
		cal_flag = 1;
		BoxInfoBase txbib;
		txbib.setTxStateNow();
		txbib.setBoxInfoNow();
		txbib.setTimeNow();
		txbib.HopsDone = ti.tcb.hopsDone;
		ti.transferbibs.push_back(txbib);
		
		char sendbuf[c_NORMAL_BUF_LEN];
		int len = formTestReply(sendbuf, 0, c_NORMAL_BUF_LEN, ti, buf_len);
		string var = "";
		var = ti.TI2string();
		if(var.length() > 0)
		{
			pthread_mutex_lock(&writelog_mutex);
			log_record.write_of(var);
			pthread_mutex_unlock(&writelog_mutex);
		}
		TestMng::Send(sendbuf, len);
	}
	if(cal_flag == 0)
	{
		string var = "";
		var = ti.TI2string();
		if(var.length() > 0)
		{
			pthread_mutex_lock(&writelog_mutex);
			log_record.write_of(var);
			pthread_mutex_unlock(&writelog_mutex);
		}
	}
	return NULL;
}

void* DataProcess::resolveTestMsg(char* buf, struct sockaddr* sender, int length)
{
	pthread_mutex_lock(&receive_mutex);
	if((strcmp((const char*)Config::CommType.c_str(), "net") == 0) && \
	   (strcmp((const char*)Config::cohda.c_str(), "yes") == 0))
	{
		_msg_info.p_buf = buf + 4;
		_msg_info.buf_len = length - 4;
	}
	else
	{
		_msg_info.p_buf = buf;
		_msg_info.buf_len = length;
	}
	#if CONFIG_DEBUG
	cout << "rx length:" << length << endl;
	#endif

	pthread_t tid = 0;
	int ret = pthread_create(&tid, NULL, analytical_data, (void*)&_msg_info);
	if(ret == 0)
	{
		#if CONFIG_DEBUG
		cout << "pthread_create successfully..." << endl;
		#endif
		sem_wait(&parameter_sem);	
		pthread_mutex_unlock(&receive_mutex);
		pthread_detach(tid);	
		return NULL;
	}
	else
	{
		#if CONFIG_DEBUG
		cout << "pthread_create failed..." << endl;
		#endif
		pthread_mutex_unlock(&receive_mutex);
		return NULL;
	}
}

int DataProcess::formTestReply(char* buf, int startindex, int length, TestInfo ti, int packet_len)
{	
	int index = startindex;	
	try{		
		buf[index++] = 0xFA;	
		buf[index++] = 0xFB;		//data value length	
		int lengthindex = index;	
		index += 2;		
		short datavallen = 0;	
		datavallen = ti.Serialize(buf, &index, length);	
		if(datavallen < 0) 
			return -1;		
		if(SerializeTool::Short2Byte(buf, &lengthindex, datavallen) < 0)	
			return -1;	
		buf[index++] = 0xEA;	
		buf[index++] = 0xEB;

		for(; index - startindex < packet_len - 5;)			
			buf[index++] = 0x0a;
		buf[index++] = Miscellaneous::ORCheck((unsigned char *)&buf[startindex + 2], packet_len - 7);
		for(; index - startindex < packet_len;)			
			buf[index++] = 0x0a;
	}
	catch(exception &e)	{		
		return -1;
	}

	return index - startindex;
}

