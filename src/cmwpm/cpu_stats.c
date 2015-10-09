/*
 * cpu_stats.c
 *
 *  Created on: Oct 8, 2015
 *      Author: trunghuynh
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include "cpu_stats.h"
#include "config.h"

#define STAT_FILE		"/proc/stat"
#define GET_CPU_CMD		"lscpu -p"
#define SYS_CPU_POSSIBLE	"/sys/devices/system/cpu/possible"
#define SYS_CPU_PRESENT		"/sys/devices/system/cpu/present"
#define RAW_DATA_COUNT		2

struct cpu_raw_data {
	unsigned long int user;
	unsigned long int nice;
	unsigned long int system;
	unsigned long int idle;
	unsigned long int io_wait;
	unsigned long int irq;
	unsigned long int soft_irq;
	unsigned long int total_utilized;
	unsigned long int total;
};

struct cpu_raw_data_set {
	int active;
	struct cpu_raw_data overall[RAW_DATA_COUNT];
	struct cpu_raw_data *sockets[RAW_DATA_COUNT];
	struct cpu_raw_data *cpus[RAW_DATA_COUNT];
};

static int parse_raw_info(struct cpu_info_set *cpu_info)
{
	int cpu, socket;
	char line[1024];
	char *ptr;
	struct cpu_raw_data_set *priv =
	    (struct cpu_raw_data_set *)cpu_info->priv;

	FILE *f = fopen(STAT_FILE, "r");

	if (!f)
		return -1;

	priv->active = !priv->active;

	while ((ptr = fgets(line, sizeof(line), f)) != NULL) {
		char cpu_name[8];
		struct cpu_raw_data new;
		struct cpu_raw_data *update;

		if (sscanf(ptr, "%7s %lu %lu %lu %lu %lu %lu %lu",
			   cpu_name, &new.user, &new.nice, &new.system,
			   &new.idle, &new.io_wait, &new.irq,
			   &new.soft_irq) != 8)
			continue;

		if (strncmp("cpu", cpu_name, 3) != 0)
			continue;

		if (strlen(cpu_name) == 3) {
			update = &priv->overall[priv->active];
		} else {
			char *endptr;
			long int offset;
			errno = 0;
			offset  = strtol(cpu_name + 3, &endptr, 10);
			if ((cpu_name + 3) == endptr || *endptr != '\0' || errno == ERANGE)
				continue;

			if ((offset < 0) || (offset >= cpu_info->num_cpus))
				continue;

			update = &priv->cpus[priv->active][offset];
		}

		new.total_utilized =
		    (new.user + new.nice + new.system + new.io_wait + new.irq +
		     new.soft_irq);
		new.total = new.total_utilized + new.idle;

		*update = new;
	}

	fclose(f);

	memset(priv->sockets[priv->active], 0, sizeof(struct cpu_raw_data) * cpu_info->num_sockets);
	for (cpu = 0; cpu < cpu_info->num_cpus; cpu++) {
		socket = cpu_info->cpu_info[cpu].socket_id;
		priv->sockets[priv->active][socket].user += priv->cpus[priv->active][cpu].user;
		priv->sockets[priv->active][socket].nice += priv->cpus[priv->active][cpu].nice;
		priv->sockets[priv->active][socket].system += priv->cpus[priv->active][cpu].system;
		priv->sockets[priv->active][socket].idle += priv->cpus[priv->active][cpu].idle;
		priv->sockets[priv->active][socket].io_wait += priv->cpus[priv->active][cpu].io_wait;
		priv->sockets[priv->active][socket].irq += priv->cpus[priv->active][cpu].irq;
		priv->sockets[priv->active][socket].soft_irq += priv->cpus[priv->active][cpu].soft_irq;
		priv->sockets[priv->active][socket].total_utilized += priv->cpus[priv->active][cpu].total_utilized;
		priv->sockets[priv->active][socket].total += priv->cpus[priv->active][cpu].total;
	}

	return 0;
}

static int profile_cpus(struct cpu_info_set *cpu_info)
{
	FILE *f;
	char line[1024];
	char *ptr;
	int total_sockets = 0, cpu;

	f = popen(GET_CPU_CMD, "r");
	if (!f)
		return -1;

	while ((ptr = fgets(line, sizeof(line), f)) != NULL) {
		int cpu;
		int core;
		int socket;

		/* CPU,Core,Socket,Node,,L1d,L1i,L2,L3 */
		if (sscanf(ptr, "%d,%d,%d,%*d,,%*d,%*d,%*d,%*d",
			   &cpu, &core, &socket) == 3) {
			if ((cpu < 0) || (cpu >= cpu_info->num_cpus))
				continue;

			cpu_info->cpu_info[cpu].cpu_id = cpu;
			cpu_info->cpu_info[cpu].core_id = core;
			cpu_info->cpu_info[cpu].socket_id = socket;
			if (socket > total_sockets)
				total_sockets = socket;
		}
	}
	fclose(f);

	cpu_info->num_sockets = total_sockets + 1;
	cpu_info->socket_info = malloc(sizeof(struct cpu_info) * cpu_info->num_sockets);
	if (cpu_info->socket_info == NULL)
		return -1;

	memset(cpu_info->socket_info, 0, sizeof(struct cpu_info) * cpu_info->num_sockets);

	for (cpu = 0; cpu < cpu_info->num_cpus; cpu++) {
		int socket = cpu_info->cpu_info[cpu].socket_id;
		cpu_info->socket_info[socket].cpu_count++;
	}

	return 0;
}

static int min(int a, int b)
{
	return (a <= b) ? a:b;
}

static int count_cpus()
{
	FILE *f;
	int pos_begin, pos_end, pre_begin, pre_end, count = 0;

	f = fopen(SYS_CPU_POSSIBLE, "r");

	if (!f)
		return 0;

	count = fscanf(f, "%d-%d", &pos_begin, &pos_end);
	fclose(f);

	if (count < 1)
		return 0;

	if (count == 1)
		return 1; /* There's only one CPU */

	/* count = 2 */
	/* Only possible CPUs that are present can be brought online */
	f = fopen(SYS_CPU_PRESENT, "r");

	if (!f)
		return 0;

	count = fscanf(f, "%d-%d", &pre_begin, &pre_end);
	fclose(f);

	if (count < 1)
		return 0;

	if (count == 1)
		return 1; /* There's only one present CPU */

	return min(pre_end + 1 - pre_begin, pos_end + 1 - pos_begin);
}

struct cpu_info_set *cpu_info_init()
{
	struct cpu_raw_data_set *raw;
	int i;
	struct cpu_info_set *new = malloc(sizeof(struct cpu_info_set));

	if (!new)
		return NULL;

	memset(new, 0, sizeof(struct cpu_info_set));

	new->num_cpus = count_cpus();
	if (new->num_cpus < 1) {
		cpu_info_cleanup(new);
		return NULL;
	}

	new->cpu_info = malloc(sizeof(struct cpu_info) * new->num_cpus);
	if (new->cpu_info == NULL) {
		cpu_info_cleanup(new);
		return NULL;
	}

	memset(new->cpu_info, 0, sizeof(struct cpu_info) * new->num_cpus);


	new->priv = malloc(sizeof(struct cpu_raw_data_set));
	if (new->priv == NULL) {
		cpu_info_cleanup(new);
		return NULL;
	}

	memset(new->priv, 0, sizeof(struct cpu_raw_data_set));

	if (profile_cpus(new) != 0) {
		cpu_info_cleanup(new);
		return NULL;
	}

	raw = (struct cpu_raw_data_set *)new->priv;
	for (i = 0; i < RAW_DATA_COUNT; i++) {
		raw->cpus[i] = malloc(sizeof(struct cpu_raw_data) *
						new->num_cpus);

		if (raw->cpus[i] == NULL) {
			cpu_info_cleanup(new);
			return NULL;
		}

		memset(raw->cpus[i], 0,
			sizeof(struct cpu_raw_data) * new->num_cpus);

		raw->sockets[i] = malloc(sizeof(struct cpu_raw_data) *
						new->num_sockets);

		if (raw->sockets[i] == NULL) {
			cpu_info_cleanup(new);
			return NULL;
		}

		memset(raw->sockets[i], 0,
			sizeof(struct cpu_raw_data) * new->num_sockets);
	}

	if (parse_raw_info(new) != 0) {
		cpu_info_cleanup(new);
		return NULL;
	}

	return new;
}

static void calculate_usage(struct cpu_info *info, struct cpu_raw_data *then,
		struct cpu_raw_data *now)
{
	unsigned long int delta = now->total - then->total;

	/* Avoid divide by zero */
	if (delta == 0) {
		info->user_load = 0;
		info->nice_load = 0;
		info->system_load = 0;
		info->io_wait_load = 0;
		info->irq_load = 0;
		info->soft_irq_load = 0;
		info->total_load = 0;
	} else {
		info->user_load =
			(((double)(now->user - then->user)) / ((double)delta)) * 100;
		info->nice_load =
			(((double)(now->nice - then->nice)) / ((double)delta)) * 100;
		info->system_load =
			(((double)(now->system - then->system)) / ((double)delta)) * 100;
		info->io_wait_load =
			(((double)(now->io_wait - then->io_wait)) / ((double)delta)) * 100;
		info->irq_load =
			(((double)(now->irq - then->irq)) / ((double)delta)) * 100;
		info->soft_irq_load =
			(((double)(now->soft_irq - then->soft_irq)) / ((double)delta)) *
			100;
		info->total_load =
			(((double)(now->total_utilized - then->total_utilized)) /
			((double)delta)) * 100;
	}
}

int cpu_info_update(struct cpu_info_set *cpu_info_set)
{
	struct cpu_raw_data *old_raw_data, *new_raw_data;
	struct cpu_raw_data_set *raw =
	    (struct cpu_raw_data_set *)cpu_info_set->priv;
	int cpu, socket;

	if (parse_raw_info(cpu_info_set) != 0)
		return -1;

	old_raw_data = &raw->overall[!raw->active];
	new_raw_data = &raw->overall[raw->active];
	calculate_usage(&cpu_info_set->overall, old_raw_data, new_raw_data);

	for (cpu = 0; cpu < cpu_info_set->num_cpus; cpu++) {
		old_raw_data = &raw->cpus[!raw->active][cpu];
		new_raw_data = &raw->cpus[raw->active][cpu];
		calculate_usage(&cpu_info_set->cpu_info[cpu], old_raw_data, new_raw_data);
	}

	for (socket = 0; socket < cpu_info_set->num_sockets; socket++) {
		if (cpu_info_set->socket_info[socket].cpu_count <= 0)
			continue;

		old_raw_data = &raw->sockets[!raw->active][socket];
		new_raw_data = &raw->sockets[raw->active][socket];
		calculate_usage(&cpu_info_set->socket_info[socket], old_raw_data, new_raw_data);
	}

	return 0;
}

void cpu_info_cleanup(struct cpu_info_set *cpu_info)
{
	if (cpu_info->priv) {
		struct cpu_raw_data_set *raw =
		    (struct cpu_raw_data_set *)cpu_info->priv;
		int i;

		for (i = 0; i < RAW_DATA_COUNT; i++) {
			if (raw->cpus[i])
				free(raw->cpus[i]);

			if (raw->sockets[i])
				free(raw->sockets[i]);
		}
		free(raw);
	}

	if (cpu_info->cpu_info)
		free(cpu_info->cpu_info);

	if (cpu_info->socket_info)
		free(cpu_info->socket_info);

	free(cpu_info);
}

#ifdef UNIT_TESTS
int validate_raw_cpu_info(struct cpu_raw_data *p)
{
	if ((p->total_utilized == 0) || (p->total == 0))
		return -1;

	return 0;
}

int validate_cpu_info_set(struct cpu_info_set *p)
{
	int i, j;
	struct cpu_raw_data_set *raw = (struct cpu_raw_data_set *)p->priv;
	for (i = 0; i < RAW_DATA_COUNT; i++) {
		struct cpu_raw_data *overall = &raw->overall[i];

		if (validate_raw_cpu_info(overall) < 0)
			return -1;

		for (j = 0; j < p->num_sockets; j++) {
			struct cpu_raw_data *socket = &raw->sockets[i][j];
			if (validate_raw_cpu_info(socket) < 0)
				return -1;
		}

		for (j = 0; j < p->num_cpus; j++) {
			struct cpu_raw_data *cpu = &raw->cpus[i][j];
			if (validate_raw_cpu_info(cpu) < 0)
				return -1;
		}
	}

	return 0;
}
#endif

