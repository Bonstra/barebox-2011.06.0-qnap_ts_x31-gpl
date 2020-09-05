/*
 * Copyright (C) Mindspeed Technologies, Inc. 2011. All rights reserved.
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
 *
 * @file nand.c
 * @brief Expansion block and GPIO initialization for NAND device support.
 * @date 11/10/2011
 */

#include <common.h>
#include <init.h>
#include <partition.h>
#include <asm/io.h>
#include <asm/hardware.h>

#include <mach/comcerto-2000.h>
#include <mach/exp-bus.h>
#include <mach/gpio.h>
#include <mach/comcerto-common.h>

static struct device_d comcerto_nand_dev = {
	.id	  = -1,
	.name     = "comcerto_nand",
	.map_base = COMCERTO_EXP_CS4_BASE_ADDR,
	.size     = COMCERTO_NAND_FLASH_SIZE,
};

/*
 * Initialize NAND related registers.
 */
static void comcerto_nand_hw_init(void)
{
	/* Initialize GPIO registers related to NAND signals */

	comcerto_gpio_enable_output(EXP_NAND_CS);
}

static int comcerto_nand_init(void)
{
	comcerto_nand_hw_init();
	register_device(&comcerto_nand_dev);

#if defined(CONFIG_COMCERTO_BOOTLOADER) && defined(CONFIG_NAND_COMCERTO)
/*
 * Warning, please make sure to synchronize with the nand_parts environement variable.
 */
	devfs_add_partition("nand0", 0x00000, 0x100000, PARTITION_FIXED, "self_raw");
	dev_add_bb_dev("self_raw", "self0");

/*
	devfs_add_partition("nand0", 0x100000, 0x100000, PARTITION_FIXED, "env_raw");
	dev_add_bb_dev("env_raw", "env0");

	printf("Using environment in NAND Flash\n" );
*/

#endif /* CONFIG_COMCERTO_NAND_ULOADER */
	return 0;
}

device_initcall(comcerto_nand_init);
