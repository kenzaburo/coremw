/*
 * cpu_stats.h
 *
 *  Created on: Oct 8, 2015
 *      Author: trunghuynh
 */

#ifndef __CPU_STATS_H
#define __CPU_STATS_H

#include <sys/time.h>

struct cpu_info {
	int socket_id;
	int core_id;
	int cpu_id;
	int cpu_count;		/* number of cpus in this socket */
	double user_load;
	double nice_load;
	double system_load;
	double io_wait_load;
	double irq_load;
	double soft_irq_load;
	double total_load;
};

struct cpu_info_set {
	int num_cpus;
	int num_sockets;
	struct cpu_info overall;
	struct cpu_info *cpu_info;
	struct cpu_info *socket_info;
	void *priv;
};

struct cpu_info_set *cpu_info_init();

int cpu_info_update(struct cpu_info_set *set);

void cpu_info_cleanup(struct cpu_info_set *set);

#ifdef UNIT_TESTS
int validate_cpu_info_set(struct cpu_info_set *set);
#endif

#endif

