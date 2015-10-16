/*
 * CmwPmManager.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: trunghuynh
 */

#include "CmwPmManager.h"
#include <iostream>
extern "C" {
	#include "cpu_stats.h" //a C header, so wrap it in extern "C"
	#include "mem_stats.h"
}
using namespace std;

CmwPmManager::CmwPmManager() {
	// TODO Auto-generated constructor stub
}

int main()
{
//	struct cpu_info_set {
//		int num_cpus;
//		int num_sockets;
//		struct cpu_info overall;
//		struct cpu_info *cpu_info;
//		struct cpu_info *socket_info;
//		void *priv;
//
//	struct cpu_info {
//		int socket_id;
//		int core_id;
//		int cpu_id;
//		int cpu_count;		/* number of cpus in this socket */
//		double user_load;
//		double nice_load;
//		double system_load;
//		double io_wait_load;
//		double irq_load;
//		double soft_irq_load;
//		double total_load;
//	};

	cout<< "Hello Trung" << endl;
	struct cpu_info_set* cpuInfo;
	cpuInfo = cpu_info_init();
	int i = cpu_info_update(cpuInfo);
	if(i != 0){
		cout<< "can't reading cpu info"<< endl;
	}else{
		cout<<"successful reading cpu info"<< endl;
		struct cpu_info cpu_set = cpuInfo->overall;
		cout<< "number of cpus: "<< cpuInfo->num_cpus<<endl;
		cout<< "core_id: "<< cpu_set.core_id<<endl;
		cout<< "socket id: "<< cpu_set.socket_id<<endl;
		cout<< "cpu_count: "<< cpu_set.cpu_count<<endl;
		cout<< "user_load: " << cpu_set.user_load<<endl;
		cout<< "nice_load: " << cpu_set.nice_load<<endl;
		cout<< "system_load: " <<cpu_set.system_load<<endl;
		cout<< "io_wait_load: " <<cpu_set.system_load <<endl;
		cout<< "irq_load: " <<cpu_set.irq_load<< endl;
		cout<< "soft_irq_load: " <<cpu_set.soft_irq_load<<endl;
		cout<< "total_load: "<< cpu_set.total_load<<endl;
	}

//	ImmServiceManager immManager = new ImmServiceManager();
//	immManager.readDataReport();
//	immManager.writeDataReport();

	return 0;
}

CmwPmManager::~CmwPmManager() {
	// TODO Auto-generated destructor stub

}

