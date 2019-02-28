/*
 * Main.cpp
 *
 *  Created on: 2017年5月22日
 *      Author: smk
 */

#include <iostream>
#include "LocalConstants.h"
#include "TestMng.h"

using namespace std;

int main(int argc, char *argv[])
{
	cout<<"Proc: "<<c_PROC_NAME<<endl;
	cout<<"Version: "<<c_PROC_VERSION<<endl;

	if(Config::LoadConfig() < 0)
	{
		cout<<"load error"<<endl;
		return 0;
	}

	cout<<"load config ok"<<endl;

	//Initiate UDP;
	EngrDevMng::start_thr_EngrDevListener();

	// Initiate Log File
	DataProcess::init();

	// Start V2X threads
	if(TestMng::init() < 0)
	{
		cout<<"Server run failed..."<<endl;
		return 0;
	}

	// Wait...
	while(1)
		sleep(10);

	return 0;
}

