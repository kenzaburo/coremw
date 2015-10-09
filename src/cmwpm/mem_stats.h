/*
 * mem_stats.h
 *
 *  Created on: Oct 8, 2015
 *      Author: trunghuynh
 */

#ifndef __MEM_STATS_H
#define __MEM_STATS_H

struct mem_info {
	unsigned long long mem_total;
	unsigned long long mem_used;
	unsigned long long mem_free;
	unsigned long long swap_total;
	unsigned long long swap_used;
	unsigned long long swap_free;
	double mem_percent_used;
	double mem_percent_free;
	double swap_percent_used;
	double swap_percent_free;
};

struct mem_info_set {
	struct mem_info mem_info;
	void *priv;
};

struct mem_info_set *mem_info_init();
void mem_info_cleanup(struct mem_info_set *mem_info_set);
int mem_info_update(struct mem_info_set *mem_info_set);

#endif
