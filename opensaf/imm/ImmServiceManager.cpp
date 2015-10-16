/*
 * ImmServiceManager.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: trunghuynh
 */

#include "ImmServiceManager.h"
#include <iostream>

//Provide api to read/write data into sqlite
//Read
//Write

bool ImmServiceManager::readDataReport(){
	std::cout<<"on readDataReport";
	return true;
}

bool ImmServiceManager::writeDataReport(){
	std::cout<<"on writeDataReport()";
	return true;
}

ImmServiceManager::ImmServiceManager() {
	// TODO Auto-generated constructor stub
}

ImmServiceManager::~ImmServiceManager() {
	// TODO Auto-generated destructor stub
}

