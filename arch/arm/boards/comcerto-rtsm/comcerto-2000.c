
/*
 * Copyright (C) 2011 Mindspeed Technologies Pvt Ltd.
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
 *
 */

/**
 * @file
 * @brief Comcerto 2000 Specific Board Initialization routines
 *
 */


#include <common.h>
#include <init.h>
#include <driver.h>
#include <partition.h>
#include <ata.h>
#include <asm/io.h>
#include <asm/types.h>
#include <mach/comcerto-2000.h>
#include <mach/exp-bus.h>
#include <generated/mach-types.h>
#include <sizes.h>
#include <c2000_eth_pdata.h>
#include <config.h>
#include <fast_uart.h>

#ifdef CONFIG_SATA_AHCI 
extern int ahci_write_data(struct device_d *dev, int block, int num_blocks, void *buf);
extern int ahci_read_data(struct device_d *dev, int block, int num_blocks, void *buf);

static struct device_d c2000_ata_disk = {
        .id       = -1,
        .name     = "disk",
	};

static struct ata_interface c2000_ata_if = {
	.read = ahci_read_data,
	.write = ahci_write_data,
	};

static struct device_d c2000_ahci = {
	.id = -1,
	.name = "ahci",
	.map_base = COMCERTO_SATA_AHCI_BASE,
	.platform_data = &c2000_ata_if,
	}; 
#endif

#if defined(CONFIG_NET_COMCERTO_2000)
struct c2000_eth_platform_data eth0_pdata = {
	.gemac_port = 0,
	.mac_addr = { 0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e},
};

struct device_d c2000_eth0 = {
	.id = 0,
	.name = "c2000_eth",
   .map_base = COMCERTO_AXI_HFE_CFG_BASE,
	/* .size  = 16M, */
	.platform_data = &eth0_pdata,
};

struct c2000_eth_platform_data eth1_pdata = {
	.gemac_port = 1,
	.mac_addr = { 0x00, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e},
};

struct device_d c2000_eth1 = {
	.id = 1,
	.name = "c2000_eth",
   .map_base = COMCERTO_AXI_HFE_CFG_BASE,
	/* .size  = 16M, */
	.platform_data = &eth1_pdata,
};
#endif

static struct memory_platform_data sdram_pdata = {
        .name = "ram0",
        .flags = DEVFS_RDWR,
};

static struct device_d sdram_dev = {
        .id = -1,
        .name = "mem",
        .map_base = 0x00000000,
        .size = SZ_256M,
        .platform_data = &sdram_pdata,
};

static struct fast_uart_plat serial_plat = {
	.clock = 24000000,      /* 24MHz  */
};

static struct device_d fast_uart_device = {
	.id       = -1,
	.name     = "fast_uart",
	.map_base = UART_BASEADDR,
	.platform_data = (void *)&serial_plat,
};

static int c2000_device_init(void)
{
	serial_plat.clock = 24000000; // do not use HAL_get_axi_clk API, clock block in rtsm is not functional.
	register_device(&fast_uart_device);

#ifdef CONFIG_SATA_AHCI
	register_device(&c2000_ahci);
	register_device(&c2000_ata_disk);
#endif

#if defined(CONFIG_NET_COMCERTO_2000)
	register_device(&c2000_eth0);
	register_device(&c2000_eth1);
#endif

        //Exp bus CS0 Mem Segment size reset value should be 0x7FFF but it
        // was 0xFF (* 4KB = 1MB) [Maybe IBR reconfigured it]. The NOR chip on C2K EVM is 64 MB but linux reduces its 
        // visibility to 32MB as it cannot cope with 64MB.So configuring the seg size to 32MB for CS0.
        writel(0x1FFF, EXP_CS0_SEG_REG);

        //CS0 EXP bus Timing tune. It reduces the time to flash NOR
        writel(0x03034007, EXP_CS0_TMG1_REG);
        writel(0x04040502, EXP_CS0_TMG2_REG);

#ifdef ARM_LINUX
        armlinux_add_dram(&sdram_dev);
        armlinux_set_bootparams((void *)0x100);
        armlinux_set_architecture(MACH_TYPE_COMCERTO);
#endif
	return 0;
}

device_initcall(c2000_device_init);

