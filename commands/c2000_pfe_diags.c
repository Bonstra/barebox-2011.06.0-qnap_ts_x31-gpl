/*
 * (C) Copyright 2012
 *  Author : Yannick Vignon (Mindspeed Technologies)
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
 * @brief Retrieve PFE diagnostics messages
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <driver.h>
#include <net.h>
#include <fs.h>
#include <errno.h>
#include <libbb.h>
#include <malloc.h>
#include "../drivers/net/comcerto/c2000_eth.h"

#define FPPDIAG_PAGE_SIZE_SHIFT		8
#define FPPDIAG_PAGE_SIZE			(1 << FPPDIAG_PAGE_SIZE_SHIFT)
#define FPPDIAG_ENTRY_SHIFT			6
#define	FPPDIAG_ENTRY_SIZE			(1 << FPPDIAG_ENTRY_SHIFT)
#define FPPDIAG_ENTRIES_PER_PAGE 	(FPPDIAG_PAGE_SIZE / FPPDIAG_ENTRY_SIZE)
#define FPPDIAG_ENTRY_HEADER_SIZE	4
#define FPPDIAG_BUFFER_SIZE			(FPPDIAG_ENTRY_SIZE - FPPDIAG_ENTRY_HEADER_SIZE) /* Account for the 32 bits of metadata at start of buffer */

#define	FPPDIAG_MASK_ENTRY_BITS		(FPPDIAG_PAGE_SIZE_SHIFT - FPPDIAG_ENTRY_SHIFT)
#define FPPDIAG_MASK_ENTRY			(( 1 << FPPDIAG_MASK_ENTRY_BITS ) - 1)

#if FPPDIAG_PAGE_TOTAL_SIZE < FPPDIAG_PAGE_SIZE*11
#error "Not enough space for FPP diags"
#endif

#define FPPDIAG_MAX_ARGS 4 /* 4 ought to be enough for anybody... */
typedef u32 FPPDIAG_ARG_TYPE; /* Type size must be a multiple of 32-bit */

typedef struct
{
        unsigned int read_index;        /* Read index into Ring buffer array */
        unsigned int write_index;       /* Write index into Ring buffer array */
} fppdiag_ctl_t;

typedef struct {
	u8 flags;
	u8 dummy1;
	u8 dummy2;
	u8 dummy3;
	u8 buffer[FPPDIAG_BUFFER_SIZE];
} fppdiag_entry_t;

/* Flags to be written at beginning of each entry */
#define FPPDIAG_PRINT_ENTRY	0
#define FPPDIAG_DUMP_ENTRY 	1
#define FPPDIAG_EXPT_ENTRY 	2

#define FPPDIAG_CTL(pe_num) ((fppdiag_ctl_t *)(pfediags_ctl_base_addr + pe_num*sizeof(fppdiag_ctl_t)))

static size_t pfediags_ctl_base_addr = FPPDIAG_CTL_BASE_ADDR;
static size_t pfediags_page_base_addr = FPPDIAG_PAGE_BASE_ADDR;


static char pe_names[11][6] = {
	"PE0: ",
	"PE1: ",
	"PE2: ",
	"PE3: ",
	"PE4: ",
	"PE5: ",
	"TM0: ",
	"TM1: ",
	"TM2: ",
	"TM3: ",
	"UPE: "};

void fppdiag_print(fppdiag_entry_t *entry, char *line_start)
{
	FPPDIAG_ARG_TYPE *args = (FPPDIAG_ARG_TYPE *)(entry->buffer + FPPDIAG_BUFFER_SIZE-FPPDIAG_MAX_ARGS*sizeof(FPPDIAG_ARG_TYPE));

	printf(line_start);
	printf(entry->buffer, ntohl(args[0]), ntohl(args[1]), ntohl(args[2]), ntohl(args[3]));
}

void fppdiag_dump(fppdiag_entry_t *entry, char *line_start)
{
	unsigned int i;

	printf("%s0x00  %02x %02x %02x %02x ", line_start, entry->flags, entry->dummy1, entry->dummy2, entry->dummy3);
	for (i=0; i < FPPDIAG_BUFFER_SIZE; i++)
	{
		if (((i+FPPDIAG_ENTRY_HEADER_SIZE) & 0x7) == 0)
			printf("\n%s0x%02x  ", line_start, i+FPPDIAG_ENTRY_HEADER_SIZE);
		printf("%02x ", entry->buffer[i]);
	}
	printf(" \n");
}

void fppdiag_exception_dump(fppdiag_entry_t *entry, char *line_start)
{
	u32 *registers = (u32 *) entry->buffer;

	printf("%sException: EPC: %8x ECAS: %8x EID: %8x ED: %8x\n%sr0/sp: %8x r1/ra: %8x r2/fp: %8x r3: %8x\n%sr4: %8x r5: %8x r6: %8x r7: %8x\n%sr8: %8x r9: %8x r10: %8x\n",
		line_start, ntohl(registers[0]), ntohl(registers[1]), ntohl(registers[2]), ntohl(registers[3]),
		line_start, ntohl(registers[4]), ntohl(registers[5]), ntohl(registers[6]), ntohl(registers[7]),
		line_start, ntohl(registers[8]), ntohl(registers[9]), ntohl(registers[10]), ntohl(registers[11]),
		line_start, ntohl(registers[12]), ntohl(registers[13]), ntohl(registers[14]) );
}

fppdiag_entry_t * fppdiag_get_entry(unsigned int pe_num, unsigned int *pread_index)
{
	unsigned int read_index;
	fppdiag_ctl_t *fppdiagctl = FPPDIAG_CTL(pe_num);
	resource_size_t pageaddr =  pfediags_page_base_addr + pe_num*FPPDIAG_PAGE_SIZE;

	read_index = ntohl(fppdiagctl->read_index);
        read_index++;
        if (read_index == FPPDIAG_ENTRIES_PER_PAGE) // Assume rng_size == 1 for now
                read_index = 0;

	*pread_index = read_index;
	return (fppdiag_entry_t *) (pageaddr+read_index*FPPDIAG_ENTRY_SIZE);
}

unsigned int fppdiag_show_current(unsigned int pe_num)
{
        fppdiag_entry_t *entry;
	unsigned int read_index;


	entry = fppdiag_get_entry(pe_num, &read_index);

	switch (entry->flags)
	{
		case FPPDIAG_EXPT_ENTRY:
                	fppdiag_exception_dump(entry, pe_names[pe_num]);
                	//fppdiag_dump(entry, pe_names[pe_num]);
			break;
		case FPPDIAG_DUMP_ENTRY:
	                fppdiag_dump(entry, pe_names[pe_num]);
			break;
		default:
			fppdiag_print(entry, pe_names[pe_num]);
			break;
	}

	return read_index;
}

unsigned int fppdiag_show_one(unsigned int pe_start, unsigned int pe_end)
{
	fppdiag_ctl_t *fppdiagctl;
	int pe_num = pe_start;

	fppdiagctl = FPPDIAG_CTL(pe_num);

	while (ntohl(fppdiagctl->read_index) == ntohl(fppdiagctl->write_index))
	{
		pe_num++;
		if (pe_num > pe_end)
			pe_num = pe_start;
		fppdiagctl = FPPDIAG_CTL(pe_num);
		if (ctrlc())
			return 1;
	}

	fppdiagctl->read_index = htonl(fppdiag_show_current(pe_num));

	return 0;
}


static int pfe_diags(struct command *cmdtp, int argc, char *argv[])
{
	unsigned int pe_start, pe_end;

	switch (argc) {
		case 1:
			pe_start = 0;
			pe_end = 11;
			break;
		case 3:
			pe_start = simple_strtoul(argv[1], NULL, 0);
			pe_end = simple_strtoul(argv[2], NULL, 0);
			break;
		case 5:
			pfediags_ctl_base_addr = simple_strtoul(argv[1], NULL, 0);
			pfediags_page_base_addr = simple_strtoul(argv[2], NULL, 0);
			pe_start = simple_strtoul(argv[3], NULL, 0);
			pe_end = simple_strtoul(argv[4], NULL, 0);
			break;
		default:
			return COMMAND_ERROR_USAGE;
	}

	if ((pe_start > 11) || (pe_end > 11)) {
		printf("pe_start and pe_end must be between 0 and 11.\n");
		return 1;
	}

	printf("pe_start: %2d pe_end: %2d ctrl_addr: 0x%zx page_addr: 0x%zx\n",
			pe_start, pe_end, pfediags_ctl_base_addr, pfediags_page_base_addr);

	while (!fppdiag_show_one(pe_start, pe_end));

	return 0;
}


static const __maybe_unused char cmd_pfe_diags_help[] =
"Prints all new messages from the PFE until stopped with Ctrl+C.\n\n"
"Usage: \n"
"pfe_diags [<ctrl_addr> <buf_addr>] <pe start #> <pe end #>\n"
"If specified, ctrl_addr and buf_addr will replace the default base \n"
"addresses for the control structures and for the buffer pages.\n"
"Those addresses are kept between calls, so further calls don't have \n"
"to specify the addresses again.\n"
"Optionally, pfe_diags may be called without arguments and will then \n"
"behave like \"pfe_diags 0 11\".\n"
;

BAREBOX_CMD_START(pfe_diags)
	.cmd		= pfe_diags,
	.usage		= "pfe_diags [<ctrl_addr> <page_addr>] <pe_start> <pe_end>",
	BAREBOX_CMD_HELP(cmd_pfe_diags_help)
BAREBOX_CMD_END
