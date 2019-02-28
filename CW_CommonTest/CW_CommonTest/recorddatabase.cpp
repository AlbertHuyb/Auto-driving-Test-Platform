#include "recorddatabase.h"

recorddatabase::recorddatabase()
{
	// TODO Auto-generated constructor stub
	bzero(boxID, sizeof(boxID));

	pdr1 = 0;
	pdr2 = 0;
	latency2 = 0;
	receive_interval1 = 0;
	receive_interval2 = 0;
	box_distance = 0;
	last_time = 0;

	rx_id1.clear();
	rx_id2.clear();

	last_rx_time1 = 0;
	last_rx_time2 = 0;
}

recorddatabase::recorddatabase(char* ID)
{
	// TODO Auto-generated constructor stub
	memcpy(boxID, ID, sizeof(boxID));

	pdr1 = 0;
	pdr2 = 0;
	latency2 = 0;
	receive_interval1 = 0;
	receive_interval2 = 0;
	box_distance = 0;
	last_time = 0;

	rx_id1.clear();
	rx_id2.clear();

	last_rx_time1 = 0;
	last_rx_time2 = 0;
}

recorddatabase::~recorddatabase() 
{
	// TODO Auto-generated destructor stub
}

int recorddatabase::handleRxMsg(TestInfo::s_InfoRes* pres)
{
	last_time = pres->rxTime;
	box_distance = pres->distance;

	switch(pres->type)
	{
	case 1:
		if(rx_id1.size()>0 && pres->msgId < rx_id1[rx_id1.size()-1] - 10)
			rx_id1.clear();

		rx_id1.push_back(pres->msgId);
		while(rx_id1[0]<= pres->msgId - 100)
			rx_id1.erase(rx_id1.begin());

		pdr1 = (float)(rx_id1.size());

		receive_interval1 = (int)((pres->rxTime - last_rx_time1)* 1000000);
		last_rx_time1 = pres->rxTime;
		break;
	case 2:
		if(rx_id2.size()>0 && pres->msgId < rx_id2[rx_id2.size()-1] - 10)
			rx_id2.clear();

		rx_id2.push_back(pres->msgId);
		while(rx_id2[0]<= pres->msgId - 100)
			rx_id2.erase(rx_id2.begin());

		pdr2 = (float)(rx_id2.size());

		latency2 = (int)(pres->latency2 * 1000000);
		receive_interval2 = (int)((pres->rxTime - last_rx_time2)* 1000000);
		last_rx_time2 = pres->rxTime;
		break;
	default:
		break;
	}

	return 1;
}
