/*
 * (C) Copyright 2012
 *  Author : Bill Westland (Mindspeed Technologies)
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
 * @brief PFE utility commands
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <driver.h>
#include <net.h>
#include <fs.h>
#include <fcntl.h>
#include <errno.h>
#include <malloc.h>
#include <xfuncs.h>
#include <getopt.h>
#include "../drivers/net/comcerto/c2000_eth.h"
#include "../drivers/net/comcerto/pfe/pfe.h"
#include "../drivers/net/comcerto/pfe/cbus/class_csr.h"
#include "../arch/arm/mach-comcerto/include/mach/comcerto-2000.h"

static void pfe_command_help(void)
{
	printf("Usage: pfe [firmware | load | lib | pe | gemac | gem | gpi | class | tmu | util | hif | status | expt | fftest] <options>\n");
}

static void pfe_command_firmware(int argc, char *argv[])
{
	if (argc == 3 && strcmp(argv[2], "init") == 0)
	{
		pfe_firmware_init();
	}
	else if (argc == 3 && strcmp(argv[2], "exit") == 0)
	{
		pfe_firmware_exit();
	}
	else
	{
		if (argc >= 3 && strcmp(argv[2], "help") != 0)
		{
			printf("Unknown option: %s\n", argv[2]);
		}
		printf("Usage: pfe firmware [init | exit]\n");
	}
}

static void pfe_command_load(int argc, char *argv[])
{
	if (argc >= 3 && strcmp(argv[2], "elf") == 0)
	{
		if (argc == 5)
		{
			u32 mask;
			u32 image_start;
			struct firmware fw;
			mask = simple_strtoul(argv[3], NULL, 0);
			image_start = simple_strtoul(argv[4], NULL, 16);
			fw.data = (u8 *)image_start;
			pfe_load_elf(mask, &fw);
		}
		else
		{
			printf("Usage: pfe load elf <pe_mask> <image_start>\n");
		}
	}
	else
	{
		if (argc >= 3 && strcmp(argv[2], "help") != 0)
		{
			printf("Unknown option: %s\n", argv[2]);
		}
		printf("Usage: pfe load elf <parameters>\n");
	}
}

static void pfe_command_lib(int argc, char *argv[])
{
	if (argc >= 3 && strcmp(argv[2], "init") == 0)
	{
		if (argc == 3)
			pfe_lib_init((void *)COMCERTO_AXI_HFE_CFG_BASE, (void *)CONFIG_DDR_BASEADDR, CONFIG_DDR_PHYS_BASEADDR);
		else if (argc == 6)
		{
			u32 cbus_base;
			u32 ddr_base;
			u32 ddr_phys_base;
			cbus_base = simple_strtoul(argv[3], NULL, 16);
			ddr_base = simple_strtoul(argv[4], NULL, 16);
			ddr_phys_base = simple_strtoul(argv[5], NULL, 16);
			pfe_lib_init((void *)cbus_base, (void *)ddr_base, ddr_phys_base);
		}
		else
		{
			printf("Usage: pfe lib init [<cbus_base> <ddr_base> <ddr_phys_base>]\n");
		}
	}
	else
	{
		if (argc >= 3 && strcmp(argv[2], "help") != 0)
		{
			printf("Unknown option: %s\n", argv[2]);
		}
		printf("Usage: pfe lib init <parameters>\n");
	}
}

static void pfe_command_pe(int argc, char *argv[])
{
	if (argc >= 3 && strcmp(argv[2], "pmem") == 0)
	{
		if (argc >= 4 && strcmp(argv[3], "read") == 0)
		{
			int i;
			int num;
			int id;
			u32 addr;
			u32 size;
			u32 val;
			if (argc == 7)
				num = simple_strtoul(argv[6], NULL, 0);
			else if (argc == 6)
				num = 1;
			else
			{
				printf("Usage: pfe pe pmem read <id> <addr> [<num>]\n");
				return;
			}
			id = simple_strtoul(argv[4], NULL, 0);
			addr = simple_strtoul(argv[5], NULL, 16);
			size = 4;
			for (i = 0; i < num; i++, addr += 4)
			{
				val = pe_pmem_read(id, addr, size);
				val = be32_to_cpu(val);
				printf("%08x%s", val, i == num - 1 || (i & 7) == 7 ? "\n" : " ");
			}
		}
		else
		{
			printf("Usage: pfe pe pmem read <parameters>\n");
		}
	}
	else if (argc >= 3 && strcmp(argv[2], "dmem") == 0)
	{
		if (argc >= 4 && strcmp(argv[3], "read") == 0)
		{
			int i;
			int num;
			int id;
			u32 addr;
			u32 size;
			u32 val;
			if (argc == 7)
				num = simple_strtoul(argv[6], NULL, 0);
			else if (argc == 6)
				num = 1;
			else
			{
				printf("Usage: pfe pe dmem read <id> <addr> [<num>]\n");
				return;
			}
			id = simple_strtoul(argv[4], NULL, 0);
			addr = simple_strtoul(argv[5], NULL, 16);
			size = 4;
			for (i = 0; i < num; i++, addr += 4)
			{
				val = pe_dmem_read(id, addr, size);
				val = be32_to_cpu(val);
				printf("%08x%s", val, i == num - 1 || (i & 7) == 7 ? "\n" : " ");
			}
		}
		else if (argc >= 4 && strcmp(argv[3], "write") == 0)
		{
			int id;
			u32 val;
			u32 addr;
			u32 size;
			if (argc != 7)
			{
				printf("Usage: pfe pe dmem write <id> <val> <addr>\n");
				return;
			}
			id = simple_strtoul(argv[4], NULL, 0);
			val = simple_strtoul(argv[5], NULL, 16);
			val = cpu_to_be32(val);
			addr = simple_strtoul(argv[6], NULL, 16);
			size = 4;
			pe_dmem_write(id, val, addr, size);
		}
		else
		{
			printf("Usage: pfe pe dmem [read | write] <parameters>\n");
		}
	}
	else if (argc >= 3 && strcmp(argv[2], "lmem") == 0)
	{
		if (argc >= 4 && strcmp(argv[3], "read") == 0)
		{
			int i;
			int num;
			u32 val;
			u32 offset;
			if (argc == 6)
				num = simple_strtoul(argv[5], NULL, 0);
			else if (argc == 5)
				num = 1;
			else
			{
				printf("Usage: pfe pe lmem read <offset> [<num>]\n");
				return;
			}
			offset = simple_strtoul(argv[4], NULL, 16);
			for (i = 0; i < num; i++, offset += 4)
			{
				pe_lmem_read(&val, 4, offset);
				val = be32_to_cpu(val);
				printf("%08x%s", val, i == num - 1 || (i & 7) == 7 ? "\n" : " ");
			}
		}
		else if (argc >= 4 && strcmp(argv[3], "write") == 0)
		{
			u32 val;
			u32 offset;
			if (argc != 6)
			{
				printf("Usage: pfe pe lmem write <val> <offset>\n");
				return;
			}
			val = simple_strtoul(argv[4], NULL, 16);
			val = cpu_to_be32(val);
			offset = simple_strtoul(argv[5], NULL, 16);
			pe_lmem_write(&val, 4, offset);
		}
		else
		{
			printf("Usage: pfe pe lmem [read | write] <parameters>\n");
		}
	}
	else
	{
		if (strcmp(argv[2], "help") != 0)
		{
			printf("Unknown option: %s\n", argv[2]);
		}
		printf("Usage: pfe lib init <parameters>\n");
	}
	//void pe_mem_memcpy_to32(int id, u32 mem_access_addr, const void *src, unsigned int len)
	//void pe_dmem_memcpy_to32(int id, u32 dst, const void *src, unsigned int len)
	//void pe_pmem_memcpy_to32(int id, u32 dst, const void *src, unsigned int len)
	//int pe_load_elf_section(int id, const void *data, Elf32_Shdr *shdr)
}

#if 0
static void pfe_command_gemac(int argc, char *argv[])
{
void gemac_init(void *base, void *cfg)
void gemac_set_speed(void *base, MAC_SPEED gem_speed)
void gemac_set_duplex(void *base, int duplex)
void gemac_set_mode(void *base, int mode)
void gemac_reset(void *base)
void gemac_enable(void *base)
void gemac_disable(void *base)
void gemac_set_address(void *base, SPEC_ADDR *addr)
SPEC_ADDR gemac_get_address(void *base)
void gemac_set_laddr1(void *base, MAC_ADDR *address)
void gemac_set_laddr2(void *base, MAC_ADDR *address)
void gemac_set_laddr3(void *base, MAC_ADDR *address)
void gemac_set_laddr4(void *base, MAC_ADDR *address)
void gemac_set_laddrN(void *base, MAC_ADDR *address, unsigned int entry_index)
void gemac_allow_broadcast(void *base)
void gemac_no_broadcast(void *base)
void gemac_enable_unicast(void *base)
void gemac_disable_unicast(void *base)
void gemac_enable_multicast(void *base)
void gemac_disable_multicast(void *base)
void gemac_enable_fcs_rx(void *base)
void gemac_disable_fcs_rx(void *base)
void gemac_enable_1536_rx(void *base)
void gemac_disable_1536_rx(void *base)
void gemac_enable_pause_rx(void *base)
void gemac_disable_pause_rx(void *base)
void gemac_set_config(void *base, GEMAC_CFG *cfg)
unsigned int * gemac_get_stats(void *base)
}
#endif

#if 0
static void pfe_command_gem(int argc, char *argv[])
{
MAC_ADDR gem_get_laddr1(void *base)
MAC_ADDR gem_get_laddr2(void *base)
MAC_ADDR gem_get_laddr3(void *base)
MAC_ADDR gem_get_laddr4(void *base)
MAC_ADDR gem_get_laddrN(void *base, unsigned int entry_index)
}
#endif

#if 0
static void pfe_command_gpi(int argc, char *argv[])
{
void gpi_init(void *base, GPI_CFG *cfg)
void gpi_reset(void *base)
void gpi_enable(void *base)
void gpi_disable(void *base)
void gpi_set_config(void *base, GPI_CFG *cfg)
}
#endif

static void pfe_command_class(int argc, char *argv[])
{
	if (argc >= 3 && strcmp(argv[2], "init") == 0)
	{
		CLASS_CFG cfg;
		if (argc == 3)
		{
			cfg.route_table_hash_bits = ROUTE_TABLE_HASH_BITS;
			cfg.route_table_baseaddr = CONFIG_DDR_PHYS_BASEADDR + ROUTE_TABLE_BASEADDR;
		}
		else if (argc == 5)
		{
			cfg.route_table_hash_bits = simple_strtoul(argv[3], NULL, 16);
			cfg.route_table_baseaddr = simple_strtoul(argv[4], NULL, 16);
		}
		else
		{
			printf("Usage: pfe class init <route_table_hash_bits> <route_table_baseaddr>\n");
		}
		class_init(&cfg);
	}
	else if (argc == 3 && strcmp(argv[2], "reset") == 0)
	{
		class_reset();
	}
	else if (argc == 3 && strcmp(argv[2], "enable") == 0)
	{
		class_enable();
	}
	else if (argc == 3 && strcmp(argv[2], "disable") == 0)
	{
		class_disable();
	}
	else if (argc >= 3 && strcmp(argv[2], "config") == 0)
	{
		CLASS_CFG cfg;
		if (argc == 3)
		{
			cfg.route_table_hash_bits = ROUTE_TABLE_HASH_BITS;
			cfg.route_table_baseaddr = CONFIG_DDR_PHYS_BASEADDR + ROUTE_TABLE_BASEADDR;
		}
		else if (argc == 5)
		{
			cfg.route_table_hash_bits = simple_strtoul(argv[3], NULL, 16);
			cfg.route_table_baseaddr = simple_strtoul(argv[4], NULL, 16);
		}
		else
		{
			printf("Usage: pfe class config <route_table_hash_bits> <route_table_baseaddr>\n");
		}
		class_set_config(&cfg);
	}
	else if (argc >= 3 && strcmp(argv[2], "bus") == 0)
	{
		if (argc >= 4 && strcmp(argv[3], "read") == 0)
		{
			u32 addr;
			u32 size;
			u32 val;
			if (argc != 6)
			{
				printf("Usage: pfe class bus read <addr> <size>\n");
				return;
			}
			addr = simple_strtoul(argv[4], NULL, 16);
			size = simple_strtoul(argv[5], NULL, 16);
			val = class_bus_read(addr, size);
			printf("%08x\n", val);
		}
		else if (argc >= 4 && strcmp(argv[3], "write") == 0)
		{
			u32 val;
			u32 addr;
			u32 size;
			if (argc != 7)
			{
				printf("Usage: pfe class bus write <val> <addr> <size>\n");
				return;
			}
			val = simple_strtoul(argv[4], NULL, 16);
			addr = simple_strtoul(argv[5], NULL, 16);
			size = simple_strtoul(argv[6], NULL, 16);
			class_bus_write(val, addr, size);
		}
		else
		{
			printf("Usage: pfe class bus [read | write] <parameters>\n");
		}
	}
	else
	{
		if (argc >= 3 && strcmp(argv[2], "help") != 0)
		{
			printf("Unknown option: %s\n", argv[2]);
		}
		printf("Usage: pfe class [init | reset | enable | disable | config | bus] <parameters>\n");
	}
}

static void pfe_command_tmu(int argc, char *argv[])
{
	if (argc >= 3 && strcmp(argv[2], "init") == 0)
	{
		if (argc == 5)
		{
			TMU_CFG cfg;
			cfg.llm_base_addr = simple_strtoul(argv[3], NULL, 16);
			cfg.llm_queue_len = simple_strtoul(argv[4], NULL, 16);
			tmu_init(&cfg);
		}
		else
		{
			printf("Usage: pfe tmu init <llm_base_addr> <llm_queue_len>\n");
		}
	}
	else if (argc >= 3 && strcmp(argv[2], "enable") == 0)
	{
		if (argc == 4)
		{
			u32 mask;
			mask = simple_strtoul(argv[3], NULL, 16);
			tmu_enable(mask);
		}
		else
		{
			printf("Usage: pfe tmu enable <pe_mask>\n");
		}
	}
	else if (argc >= 3 && strcmp(argv[2], "disable") == 0)
	{
		if (argc == 4)
		{
			u32 mask;
			mask = simple_strtoul(argv[3], NULL, 16);
			tmu_disable(mask);
		}
		else
		{
			printf("Usage: pfe tmu disable <pe_mask>\n");
		}
	}
	else
	{
		if (argc >= 3 && strcmp(argv[2], "help") != 0)
		{
			printf("Unknown option: %s\n", argv[2]);
		}
		printf("Usage: pfe tmu [init | enable | disable] <parameters>\n");
	}
}

#define	PESTATUS_ADDR_CLASS	0x800
#define	PESTATUS_ADDR_TMU	0x80
#define	PESTATUS_ADDR_UTIL	0x0

static void pfe_command_status(int argc, char *argv[])
{
	int do_clear = 0;
	int j;
	u32 id;
	u32 dmem_addr;
	u32 cpu_state;
	u32 activity_counter;
	u32 rx;
	u32 tx;
	u32 drop;
	char statebuf[5];
	u32 class_debug_reg = 0;
	u32 debug_indicator;
	u32 debug[16];

	if (argc == 3 && strcmp(argv[2], "clear") == 0)
		do_clear = 1;
	else if (argc != 2)
	{
		printf("Usage: pfe status [clear]\n");
		return;
	}

	for (id = CLASS0_ID; id <= UTIL_ID; id++)
	{
		if (id == UTIL_ID)
		{
			printf("util:\n");
			dmem_addr = PESTATUS_ADDR_UTIL;
		}
		else if (id >= TMU0_ID)
		{
			if (id == TMU0_ID)
				printf("tmu:\n");
			dmem_addr = PESTATUS_ADDR_TMU;
		}
		else
		{
			if (id == CLASS0_ID)
				printf("class:\n");
			dmem_addr = PESTATUS_ADDR_CLASS;
			class_debug_reg = readl(CLASS_PE0_DEBUG + id * 4);
		}
		cpu_state = pe_dmem_read(id, dmem_addr, 4);
		dmem_addr += 4;
		memcpy(statebuf, (char *)&cpu_state, 4);
		statebuf[4] = '\0';
		activity_counter = pe_dmem_read(id, dmem_addr, 4);
		dmem_addr += 4;
		rx = pe_dmem_read(id, dmem_addr, 4);
		if (do_clear)
			pe_dmem_write(id, 0, dmem_addr, 4);
		dmem_addr += 4;
		tx = pe_dmem_read(id, dmem_addr, 4);
		if (do_clear)
			pe_dmem_write(id, 0, dmem_addr, 4);
		dmem_addr += 4;
		drop = pe_dmem_read(id, dmem_addr, 4);
		if (do_clear)
			pe_dmem_write(id, 0, dmem_addr, 4);
		dmem_addr += 4;
		if (id == UTIL_ID)
		{
			printf("state=%4s ctr=%08x rx=%x tx=%x\n",
					statebuf, cpu_to_be32(activity_counter),
					cpu_to_be32(rx), cpu_to_be32(tx));
		}
		else if (id >= TMU0_ID)
		{
			printf("%d: state=%4s ctr=%08x rx=%x tx=%x\n",
					id - TMU0_ID, statebuf, cpu_to_be32(activity_counter),
					cpu_to_be32(rx), cpu_to_be32(tx));
		}
		else
		{
			printf("%d: pc=1%04x ldst=%04x state=%4s ctr=%08x rx=%x tx=%x drop=%x\n",
					id - CLASS0_ID, class_debug_reg & 0xFFFF, class_debug_reg >> 16,
					statebuf, cpu_to_be32(activity_counter),
					cpu_to_be32(rx), cpu_to_be32(tx), cpu_to_be32(drop));
		}
		debug_indicator = pe_dmem_read(id, dmem_addr, 4);
		dmem_addr += 4;
		if (debug_indicator == cpu_to_be32('DBUG'))
		{
			int last = 0;
			for (j = 0; j < 16; j++)
			{
				debug[j] = pe_dmem_read(id, dmem_addr, 4);
				if (debug[j])
				{
					last = j + 1;
					if (do_clear)
						pe_dmem_write(id, 0, dmem_addr, 4);
				}
				dmem_addr += 4;
			}
			for (j = 0; j < last; j++)
			{
				printf("%08x%s", cpu_to_be32(debug[j]), (j & 0x7) == 0x7 || j == last - 1 ? "\n" : " ");
			}
		}
	}

}

#define EXPT_DUMP_ADDR 0x1fa8
#define EXPT_REG_COUNT 20
static const char *register_names[EXPT_REG_COUNT] = {
		"  pc", "ECAS", " EID", "  ED",
		"  sp", "  r1", "  r2", "  r3",
		"  r4", "  r5", "  r6", "  r7",
		"  r8", "  r9", " r10", " r11",
		" r12", " r13", " r14", " r15"
};

static void pfe_command_expt(int argc, char *argv[])
{
	unsigned int id, i, val, addr;

	if (argc == 3)
	{
		id = simple_strtoul(argv[2], NULL, 0);
		addr = EXPT_DUMP_ADDR;
		printf("Exception information for PE %d:\n", id);
		for (i = 0; i < EXPT_REG_COUNT; i++)
		{
			val = pe_dmem_read(id, addr, 4);
			val = be32_to_cpu(val);
			printf("%s:%08x%s", register_names[i], val, (i & 3) == 3 ? "\n" : " ");
			addr += 4;
		}
	}
	else
	{
		printf("Usage: pfe expt <id>\n");
	}
}

static void pfe_command_util(int argc, char *argv[])
{
	if (argc == 3 && strcmp(argv[2], "init") == 0)
	{
		UTIL_CFG cfg;
		util_init(&cfg);
	}
	else if (argc == 3 && strcmp(argv[2], "reset") == 0)
	{
		util_reset();
	}
	else if (argc == 3 && strcmp(argv[2], "enable") == 0)
	{
		util_enable();
	}
	else if (argc == 3 && strcmp(argv[2], "disable") == 0)
	{
		util_disable();
	}
	else if (argc >= 3 && strcmp(argv[2], "bus") == 0)
		{
			if (argc >= 4 && strcmp(argv[3], "read") == 0)
			{
				u32 addr;
				u32 size;
				u32 val;
				if (argc != 6)
				{
					printf("Usage: pfe util bus read <addr> <size>\n");
					return;
				}
				addr = simple_strtoul(argv[4], NULL, 16);
				size = simple_strtoul(argv[5], NULL, 16);
				val = util_bus_read(addr, size);
				printf("%08x\n", val);
			}
			else if (argc >= 4 && strcmp(argv[3], "write") == 0)
			{
				u32 val;
				u32 addr;
				u32 size;
				if (argc != 7)
				{
					printf("Usage: pfe util bus write <val> <addr> <size>\n");
					return;
				}
				val = simple_strtoul(argv[4], NULL, 16);
				addr = simple_strtoul(argv[5], NULL, 16);
				size = simple_strtoul(argv[6], NULL, 16);
				util_bus_write(val, addr, size);
			}
			else
			{
				printf("Usage: pfe util bus [read | write] <parameters>\n");
			}
		}
	else
	{
		if (argc >= 3 && strcmp(argv[2], "help") != 0)
		{
			printf("Unknown option: %s\n", argv[2]);
		}
		printf("Usage: pfe util [init | reset | enable | disable | bus] <parameters>\n");
	}
}

#if 0
static void pfe_command_hif(int argc, char *argv[])
{
void hif_nocpy_init(void)
void hif_init(void)
void hif_tx_enable(void)
void hif_tx_disable(void)
void hif_rx_enable(void)
void hif_rx_disable(void)
}
#endif

#define ROUTE_TABLE_START	(CONFIG_DDR_PHYS_BASEADDR+ROUTE_TABLE_BASEADDR)
static void pfe_command_fftest(int argc, char *argv[])
{
	int ret;
	struct eth_device *save_eth_current;
	struct eth_device *edev_eth0;
	struct eth_device *edev_eth1;

	if (argc == 3)
	{
		// open binary file to load into route table
		int fd;
		char *p = (char *)ROUTE_TABLE_START;
		int n, ntotal;
		fd = open(argv[2], O_RDONLY);
		if (fd < 0) {
			perror("open");
			return;
		}
		memset((char *)ROUTE_TABLE_START, 0, ROUTE_TABLE_SIZE);
		ntotal = 0;
		do {
			n = read(fd, p, (ROUTE_TABLE_SIZE - ntotal) < 1024 ? (ROUTE_TABLE_SIZE - ntotal) : 1024);
			if (n < 0) {
				perror("read");
				close(fd);
				return;
			}

			ntotal += n;
			p += n;
		} while (n > 0 && ntotal < ROUTE_TABLE_SIZE);
		printf("Read %d bytes from %s to route table at 0x%x\n", ntotal, argv[1], ROUTE_TABLE_START);
		close(fd);
	}
	else if (argc != 2)
	{
		printf("Usage: pfe fftest [<filename>]\n");
		return;
	}

	// open eth_current if not already open
	save_eth_current = eth_get_current();
	if (save_eth_current && !save_eth_current->active)
	{
		ret = save_eth_current->open(save_eth_current);
		if (ret)
		{
			printf("eth_current open error\n");
			return;
		}
		save_eth_current->active = 1;
	}


	// open eth0 and eth1 
	edev_eth0 = eth_get_byname("eth0");
	if (!edev_eth0)
	{
		printf("Cannot access eth0\n");
		return;
	}
	if (edev_eth0 != save_eth_current)
	{
		ret = edev_eth0->open(edev_eth0);
		if (ret)
		{
			printf("eth0 open error\n");
			return;
		}
	}
	edev_eth1 = eth_get_byname("eth1");
	if (!edev_eth1)
	{
		printf("Cannot access eth1\n");
		return;
	}

	if (edev_eth1 != save_eth_current)
	{
		ret = edev_eth1->open(edev_eth1);
		if (ret)
		{
			printf("eth0 open error\n");
			return;
		}
	}

}

static int pfe_command(struct command *cmdtp, int argc, char *argv[])
{
	if (argc == 1 || strcmp(argv[1], "help") == 0)
	{
		pfe_command_help();
		return COMMAND_SUCCESS;
	}
	if (strcmp(argv[1], "firmware") == 0)
		pfe_command_firmware(argc, argv);
	else if (strcmp(argv[1], "load") == 0)
		pfe_command_load(argc, argv);
	else if (strcmp(argv[1], "lib") == 0)
		pfe_command_lib(argc, argv);
	else if (strcmp(argv[1], "pe") == 0)
		pfe_command_pe(argc, argv);
#if 0
	else if (strcmp(argv[1], "gemac") == 0)
		pfe_command_gemac(argc, argv);
	else if (strcmp(argv[1], "gem") == 0)
		pfe_command_gem(argc, argv);
	else if (strcmp(argv[1], "gpi") == 0)
		pfe_command_gpi(argc, argv);
#endif
	else if (strcmp(argv[1], "class") == 0)
		pfe_command_class(argc, argv);
	else if (strcmp(argv[1], "tmu") == 0)
		pfe_command_tmu(argc, argv);
	else if (strcmp(argv[1], "status") == 0)
		pfe_command_status(argc, argv);
	else if (strcmp(argv[1], "expt") == 0)
		pfe_command_expt(argc, argv);
	else if (strcmp(argv[1], "util") == 0)
		pfe_command_util(argc, argv);
#if 0
	else if (strcmp(argv[1], "hif") == 0)
		pfe_command_hif(argc, argv);
#endif
	else if (strcmp(argv[1], "fftest") == 0)
		pfe_command_fftest(argc, argv);
	else
	{
		printf("Unknown option: %s\n", argv[1]);
		pfe_command_help();
		return COMMAND_ERROR;
	}
	return COMMAND_SUCCESS;
}


static const __maybe_unused char cmd_pfe_help[] =
"Performs PFE lib utility functions\n\n"
"Usage: \n"
"pfe <options>\n"
;

BAREBOX_CMD_START(pfe)
	.cmd		= pfe_command,
	.usage		= "pfe <options>",
	BAREBOX_CMD_HELP(cmd_pfe_help)
BAREBOX_CMD_END

