/*
 * (C) Copyright 2003
 *  Author : Laurent Brando (Mindspeed Technologies)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/**
 * @file
 * @brief Retrieve PFE Statistics
 */

//#define SBL_EMUL	/* for compilation and debug on Linux Station */

#ifndef SBL_EMUL

#include <common.h>
#include <command.h>
#include <environment.h>
#include <driver.h>
#include <net.h>
#include <fs.h>
#include <errno.h>
#include <libbb.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "../drivers/net/comcerto/pfe/pfe.h"

#else

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define simple_strtoul(a,b,c) strtoul(a,b,c)

enum {
	CLASS0_ID = 0,
	CLASS1_ID,
	CLASS2_ID,
	CLASS3_ID,
	CLASS4_ID,
	CLASS5_ID,
	TMU0_ID,
	TMU1_ID,
	TMU2_ID,
	TMU3_ID,
	UTIL_ID,
	MAX_PE
};

#endif

typedef struct pfe_stat_queue_response {
	u_int32_t	emitted_pkts; 
	u_int32_t	dropped_pkts;
	u_int32_t	peak_queue_occ; 
} pfe_stat_queue_response_t;

typedef struct pfe_stat_interface_pkt_response {
	u_int32_t	total_bytes_received[2]; /* 64 bit counter stored as 2*32 bit counters */
	u_int32_t	total_bytes_transmitted[2]; /* 64 bit counter stored as 2*32 bit counters */ 
	u_int32_t	total_pkts_received;
	u_int32_t	total_pkts_transmitted;
} pfe_stat_interface_pkt_response_t;


#define NUM_QUEUES 8
#define NUM_INTERFACES 2

/* used to store either dmem base address of statistics counters structure or
statistics control register address (for enable/disable stats). Specified as command line argument */
unsigned int class_dmem_addr = 0;
unsigned int tmu_dmem_addr = 0;

/* Host copy of the statistics memory layout in PFE classifier */
typedef struct _tFppStatistics {
	u_int64_t FS_total_bytes_received[NUM_INTERFACES];
	u_int64_t FS_total_bytes_transmitted[NUM_INTERFACES];
	u_int32_t FS_total_pkts_received[NUM_INTERFACES];	
	u_int32_t FS_total_pkts_transmitted[NUM_INTERFACES];
	u_int64_t FS_emitted_pkts[NUM_INTERFACES][NUM_QUEUES];
	u_int64_t FS_dropped_pkts[NUM_INTERFACES][NUM_QUEUES];
	u_int32_t FS_peak_queue_occ[NUM_INTERFACES][NUM_QUEUES];
} tFppStatistics;

struct _tFppStatistics host_stats;



/* redefined here, because declared as static in /lib/vsprintf.c */
#define is_digit(c)	((c) >= '0' && (c) <= '9')
static int skip_atoi(char **s)
{
	int i=0;
	
	while (is_digit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}


/* compute address of a given statistics counters whithin the PE's memory */
static unsigned int get_target_counter_address(unsigned int target_base, unsigned int counter)
{
	//printf("%s:\n", __FUNCTION__);
	
	return (target_base + (counter - (unsigned int)&host_stats));
}


/* retrieves the base address of the statistics structure within the PE memory */
static unsigned int get_target_dmem_address(unsigned char pe_id)
{
	//printf("%s:\n", __FUNCTION__);
	
	return (pe_id <= CLASS5_ID)? class_dmem_addr: tmu_dmem_addr;
}

/* Definitions of Bit Masks for the features */
#define STAT_QUEUE_BITMASK 		0x00000001
#define STAT_INTERFACE_BITMASK 		0x00000002

#ifndef caseretstr
#define caseretstr(x) case x: return #x
#endif
char *stats_mask_to_string(unsigned int mask)
{
	switch (mask)
	{
		caseretstr(STAT_QUEUE_BITMASK);
		caseretstr(STAT_INTERFACE_BITMASK);
		default:
		return "unknown\n";
	}
}

unsigned int stats_string_to_mask(char * name)
{	
	if(strcmp(name, "queue") == 0)
		return STAT_QUEUE_BITMASK;
	if(strcmp(name, "interface") == 0)
		return STAT_INTERFACE_BITMASK;
	
	return 0;
}



#ifdef SBL_EMUL

unsigned int dummy_dmem[128] = {0};

static unsigned int pe_dmem_read(int id, unsigned int addr, unsigned char size)
{
	printf("%s: read pe %i - @%08x val = %x - %d bytes\n", __FUNCTION__, id, addr, dummy_dmem[0], size);
	
	return dummy_dmem[0];
}

static void pe_dmem_write(int id, unsigned int addr, unsigned int val, unsigned char size)
{
	dummy_dmem[0] = val;
	
	printf("%s: write pe %i - @%08x val = %x - %d bytes\n", __FUNCTION__, id, addr, val, size);
}

#define COMMAND_ERROR_USAGE (-1)

#else

extern unsigned int pe_dmem_read(int id, unsigned int addr, unsigned char size);
extern void pe_dmem_write(int id, unsigned int addr, unsigned int val, unsigned char size);

#endif


static void class_aggregate_stats(unsigned int *host_addr, unsigned int dmem_addr, unsigned char nbytes)
{
	int pe_id;
	unsigned char nwords;
	unsigned int *dst;
 	unsigned int *src;
	
	//printf("%s:\n", __FUNCTION__);
		
	for(pe_id = CLASS0_ID; pe_id <= CLASS5_ID; pe_id++)
	{	
		nwords = nbytes;
		dst = host_addr;
		src = (unsigned int *)dmem_addr;
		while(nwords--)
		{	
			*dst += pe_dmem_read(pe_id, (unsigned int)src, 4); 
			dst++;
			src++;
		}
	}
}

static void tmu_aggregate_stats(unsigned int *host_addr, unsigned int dmem_addr, unsigned char nbytes, unsigned char interface)
{
	int pe_id;
	unsigned char nwords;
	unsigned int *dst;
	unsigned int *src;
	
	//printf("%s:\n", __FUNCTION__);

	if(interface == 0)
		pe_id = TMU0_ID; 
	else if(interface == 1)
		pe_id = TMU1_ID;
	else
		pe_id = TMU2_ID;
	
	nwords = nbytes;
	dst = host_addr;
	src = (unsigned int *)dmem_addr;
	while(nwords--)
	{	
		*dst += pe_dmem_read(pe_id, (unsigned int)src, 4); 
		dst++;
		src++;
	}
}


static void pe_enable_stats(unsigned int mask, unsigned char enable)
{
	int pe_id;
	unsigned int stat_ctrl_addr;
	unsigned int stat_ctrl;
	
	printf("%s: %s %s\n", __FUNCTION__, (enable)? "enable":"disable",stats_mask_to_string(mask));
	
	for(pe_id = CLASS0_ID; pe_id <= TMU2_ID; pe_id++)
	{
		stat_ctrl_addr = get_target_dmem_address(pe_id);
		
		stat_ctrl = pe_dmem_read(pe_id, stat_ctrl_addr, 4);
		
		if(enable)
			stat_ctrl |= mask;
		else
			stat_ctrl &= ~mask;
		
		pe_dmem_write(pe_id, stat_ctrl_addr, stat_ctrl, 4);
	}

}

#ifndef SBL_EMUL
static int pfe_stats(struct command *cmdtp, int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	pfe_stat_queue_response_t queue_stats = {0};
	pfe_stat_interface_pkt_response_t interface_pkt_stats = {{0}};
	unsigned int counter_address, interface, queue;
	
	if(argc < 4)
		return COMMAND_ERROR_USAGE;
	
	class_dmem_addr = simple_strtoul(argv[1], NULL, 0);
	tmu_dmem_addr = simple_strtoul(argv[2], NULL, 0);
	
	if(strcmp(argv[3], "enable") == 0)
	{
		if(argc != 5)
			return COMMAND_ERROR_USAGE;
		
		pe_enable_stats(stats_string_to_mask(argv[4]), 1);
	}
	else if(strcmp(argv[3], "disable") == 0)
	{
		if(argc != 5)
			return COMMAND_ERROR_USAGE;
		
		pe_enable_stats(stats_string_to_mask(argv[4]), 0);
	}
	
	else if(strcmp(argv[3], "query") == 0)
	{		
		if(strcmp(argv[4], "queue") == 0)
		{
			if(argc != 8)
				return COMMAND_ERROR_USAGE;
			
			queue = simple_strtoul(argv[5], NULL, 0);
			interface = simple_strtoul(argv[7], NULL, 0); 
			
			printf("QUEUE STATS: tmu dmem base 0x%x queue %d interface %d\n", tmu_dmem_addr, queue, interface);
			
			if((queue > (NUM_QUEUES - 1)) || (interface > (NUM_INTERFACES - 1)))
				return COMMAND_ERROR_USAGE;
			
			/* get corresponding dmem address */
			counter_address = get_target_counter_address(tmu_dmem_addr, (unsigned int)&host_stats.FS_emitted_pkts[interface][queue]);

			/* copy and aggregate all queue_stats counters for the selected interface */
			tmu_aggregate_stats((unsigned int *)&queue_stats, counter_address, sizeof(pfe_stat_queue_response_t)/4, interface);
			
			printf("Emitted Pkts: %u \n" 
			"Dropped Packets: %u\n" 
			"Peak Queue Occupancy: %u \n",
				queue_stats.emitted_pkts, queue_stats.dropped_pkts, queue_stats.peak_queue_occ);
		}
		else if(strcmp(argv[4], "interface") == 0)
		{
			if(argc != 6)
				return COMMAND_ERROR_USAGE;
			
			interface = simple_strtoul(argv[5], NULL, 0);
			
			printf("INTERFACE STATS: class dmem base 0x%x tmu dmem base 0x%x interface %d\n", class_dmem_addr, tmu_dmem_addr, interface);
			
			if(interface > (NUM_INTERFACES - 1))
				return COMMAND_ERROR_USAGE;
			
			/* get CLASSIFIER statistics for received counters */
			counter_address = get_target_counter_address(class_dmem_addr, (unsigned int)&host_stats.FS_total_bytes_received[interface]);
			class_aggregate_stats((unsigned int *)&interface_pkt_stats.total_bytes_received[0], counter_address, sizeof(u_int64_t));
			
			counter_address = get_target_counter_address(class_dmem_addr, (unsigned int)&host_stats.FS_total_pkts_received[interface]);
			class_aggregate_stats((unsigned int *)&interface_pkt_stats.total_pkts_received, counter_address, sizeof(u_int32_t));
			
			/* get TMU statistics for transmit counters */
			counter_address = get_target_counter_address(tmu_dmem_addr, (unsigned int)&host_stats.FS_total_bytes_transmitted[interface]);
			tmu_aggregate_stats((unsigned int *)&interface_pkt_stats.total_bytes_transmitted[0], counter_address, sizeof(u_int64_t), interface);
			
			counter_address = get_target_counter_address(tmu_dmem_addr, (unsigned int)&host_stats.FS_total_pkts_transmitted[interface]);
			tmu_aggregate_stats((unsigned int *)&interface_pkt_stats.total_pkts_transmitted, counter_address, sizeof(u_int32_t), interface);
						
			printf("Total Bytes Received: %llu \n"
			"Total Bytes Transmitted: %llu \n"
			"Total Packets Received: %u \n"
			"Total Packets Transmitted: %u \n",
			(unsigned long long)(interface_pkt_stats.total_bytes_received[0] | (interface_pkt_stats.total_bytes_received[1] << 31)), 
			(unsigned long long)(interface_pkt_stats.total_bytes_transmitted[0] | (interface_pkt_stats.total_bytes_transmitted[1] << 31)), 
			interface_pkt_stats.total_pkts_received, interface_pkt_stats.total_pkts_transmitted);
		}
		else 
		{
			printf("statistics type %s not supported\n", argv[1]);
		}	
	}
	else
	{
		return COMMAND_ERROR_USAGE;
	}
	
	return 0;
}

#ifndef SBL_EMUL
static const __maybe_unused char cmd_pfe_stats_help[] =
"Usage:\n \
Enable/Disable Statistics:    pfe_stat <class_gFpStatFeatureBitMask> <tmu_gFpStatFeatureBitMask> [enable | disable] [queue | interface]\n \
Query Queue Statistics:       pfe_stat <class_gFpStatistics> <tmu_gFpStatistics> query queue <queue number> <interface number>\n \
Query Interface Statistics:   pfe_stat <class_gFpStatistics> <tmu_gFpStatistics> query interface <interface number> ";

BAREBOX_CMD_START(pfe_stats)
	.cmd		= pfe_stats,
	.usage		= "retrieve PFE statistics",
	BAREBOX_CMD_HELP(cmd_pfe_stats_help)
BAREBOX_CMD_END
#endif

