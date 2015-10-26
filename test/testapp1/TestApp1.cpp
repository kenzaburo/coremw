/*
 * TestApp1.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: trunghuynh
 */

#include "TestApp1.h"
#include "ImmServiceManager.h"
TestApp1::TestApp1() {
	// TODO Auto-generated constructor stub

}

TestApp1::~TestApp1() {
	// TODO Auto-generated destructor stub
}

//Try to reading and show data from immlib
int main(){

	ImmServiceManager imm;


	imm.writeDataReport(NULL, NULL);
	imm.readDataReport();

}
