/*
 * mem_stats.c
 *
 *  Created on: Oct 8, 2015
 *      Author: trunghuynh
 */


#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "mem_stats.h"

#define MEMINFO_FILE		"/proc/meminfo"

struct other_mem_data {
	unsigned long long buffer;
	unsigned long long cache;
};

void mem_info_cleanup(struct mem_info_set *mem_info_set)
{
	if (mem_info_set->priv) {
		struct other_mem_data *other = (struct other_mem_data *)(mem_info_set->priv);
		free(other);
		other = NULL;
	}

	free(mem_info_set);
}

struct mem_info_set *mem_info_init()
{
	/* allocate structure */
	struct mem_info_set *mem_info_set_ptr = malloc(sizeof(struct mem_info_set));

	/* Not enough memory */
	if (!mem_info_set_ptr)
		return NULL;

	memset(mem_info_set_ptr, 0, sizeof(struct mem_info_set));

	mem_info_set_ptr->priv = malloc(sizeof(struct other_mem_data));

	/* Not enough memory */
	if (mem_info_set_ptr->priv == NULL) {
		mem_info_cleanup(mem_info_set_ptr);
		return NULL;
	}

	memset(mem_info_set_ptr->priv, 0, sizeof(struct other_mem_data));

	return mem_info_set_ptr;
}

static void calculate_percentage(struct mem_info_set *mem_info_set)
{
	mem_info_set->mem_info.mem_percent_used = ((double)(mem_info_set->mem_info.mem_used)
					/(double)(mem_info_set->mem_info.mem_total))*100;
	mem_info_set->mem_info.mem_percent_free = ((double)(mem_info_set->mem_info.mem_free)
					/(double)(mem_info_set->mem_info.mem_total))*100;

	/* Avoid divide by zero. */
	if (mem_info_set->mem_info.swap_total == 0) {
		mem_info_set->mem_info.swap_percent_used = 0;
		mem_info_set->mem_info.swap_percent_free = 0;
	} else {
		mem_info_set->mem_info.swap_percent_used = ((double)(mem_info_set->mem_info.swap_used)
						/(double)(mem_info_set->mem_info.swap_total))*100;
		mem_info_set->mem_info.swap_percent_free = ((double)(mem_info_set->mem_info.swap_free)
						/(double)(mem_info_set->mem_info.swap_total))*100;
	}
}

int mem_info_update(struct mem_info_set *mem_info_set)
{
	struct other_mem_data *other_data = (struct other_mem_data *)(mem_info_set->priv);
	FILE* file;
	unsigned long long mem_total, mem_free, swap_total, swap_free, cached_mem, buffer_mem,
			shmem, sreclaimable_mem;
	int count;

	file = fopen(MEMINFO_FILE, "r");
	if (!file)
		return -1;

	count = fscanf(file,
			"MemTotal: %llu kB\n"			/* MemTotal */
			"MemFree: %llu kB\n"			/* MemFree */
			"Buffers: %llu kB\n"			/* Buffers */
			"Cached: %llu kB\n"			/* Cached */
			"%*[^\n]\n"				/* Ignore */
			"%*[^\n]\n"				/* Ignore */
			"%*[^\n]\n"				/* Ignore */
			"%*[^\n]\n"				/* Ignore */
			"%*[^\n]\n"				/* Ignore */
			"%*[^\n]\n"				/* Ignore */
			"%*[^\n]\n"				/* Ignore */
			"%*[^\n]\n"				/* Ignore */
			"%*[^\n]\n"				/* Ignore */
			"SwapTotal: %llu kB\n"			/* SwapTotal */
			"SwapFree: %llu kB\n"			/* SwapFree */
			"%*[^\n]\n"				/* Ignore */
			"%*[^\n]\n"				/* Ignore */
			"%*[^\n]\n"				/* Ignore */
			"%*[^\n]\n"				/* Ignore */
			"Shmem: %llu kB\n"			/* Shmem */
			"%*[^\n]\n"				/* Ignore */
			"SReclaimable: %llu kB\n"		/* SReclaimable */
			,					/* Ignore the rest */
			&mem_total,
			&mem_free,
			&buffer_mem,
			&cached_mem,
			&swap_total,
			&swap_free,
			&shmem,
			&sreclaimable_mem);

	/* Close file */
	fclose(file);

	if (count != 8)
		return -1;

	/* Update latest value */
	mem_info_set->mem_info.mem_total = mem_total;
	mem_info_set->mem_info.mem_free = mem_free + cached_mem + buffer_mem
					+ sreclaimable_mem - shmem;
	mem_info_set->mem_info.mem_used = mem_total - mem_info_set->mem_info.mem_free;
	mem_info_set->mem_info.swap_total = swap_total;
	mem_info_set->mem_info.swap_free = swap_free;
	mem_info_set->mem_info.swap_used = swap_total - swap_free;
	other_data->buffer = buffer_mem;
	other_data->cache = cached_mem;

	calculate_percentage(mem_info_set);

	return 0;
}
