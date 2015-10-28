/*
 * CmwPmManager.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: trunghuynh
 */

#include "CmwPmManager.h"
#include "ImmServiceManager.h"
#include <unistd.h>
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
	struct cpu_info_set* cpuInfo;
	struct mem_info_set* memInfo;
	cpuInfo = cpu_info_init();
	memInfo = mem_info_init();
	if(!memInfo){
		cout<< "can't init memInfo";
		return -1;
	}

	while(true){
		int i = cpu_info_update(cpuInfo);
		int j = mem_info_update(memInfo);
		// if(i != 0 || j != 0){
		// 	cout<< "can't reading cpu/ram info"<< endl;
		// }else{
		// 	cout<<"successful reading cpu info"<< endl;
		// 	struct cpu_info cpu_set = cpuInfo->overall;
		// 	cout<< "number of cpus: "<< cpuInfo->num_cpus<<endl;
		// 	cout<< "core_id: "<< cpu_set.core_id<<endl;
		// 	cout<< "socket id: "<< cpu_set.socket_id<<endl;
		// 	cout<< "cpu_count: "<< cpu_set.cpu_count<<endl;
		// 	cout<< "user_load: " << cpu_set.user_load<<endl;
		// 	cout<< "nice_load: " << cpu_set.nice_load<<endl;
		// 	cout<< "system_load: " <<cpu_set.system_load<<endl;
		// 	cout<< "io_wait_load: " <<cpu_set.system_load <<endl;
		// 	cout<< "irq_load: " <<cpu_set.irq_load<< endl;
		// 	cout<< "soft_irq_load: " <<cpu_set.soft_irq_load<<endl;
		// 	cout<< "total_load: "<< cpu_set.total_load<<endl;

		// 	cout<<"successful reading cpu info"<< endl;
		// 	struct mem_info mem_set = memInfo->mem_info;
		// 	cout<< "mem total: " << mem_set.mem_total <<endl;
		// 	cout<< "mem used: " << mem_set.mem_used <<endl;
		// 	cout<< "mem free: " << mem_set.mem_free <<endl;
		// }
		usleep(1000000);
		ImmServiceManager immManager;
		immManager.setup(); //Create system_load_tb;
		immManager.writeDataReport("100","20"); //dummy data
		immManager.readDataReport(); //show report from database
	}


	return 0;
}

CmwPmManager::~CmwPmManager() {
	// TODO Auto-generated destructor stub

}

