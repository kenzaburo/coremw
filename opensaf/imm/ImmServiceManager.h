/*
 * ImmServiceManager.h
 *
 *  Created on: Oct 7, 2015
 *      Author: trunghuynh
 */

#ifndef IMMSERVICEMANAGER_H_
#define IMMSERVICEMANAGER_H_

#include <iostream>
#include <string>
using namespace std;
class ImmServiceManager {
private:
	string dbName; // k= "/home/systemload.db";
	string tbName; // = "system_load_tb";
public:
	ImmServiceManager();
	virtual ~ImmServiceManager();
	void setup(); //Create db or open db
	bool writeDataReport(string cpuLoad, string ramLoad);
	bool readDataReport();
};

#endif /* IMMSERVICEMANAGER_H_ */
