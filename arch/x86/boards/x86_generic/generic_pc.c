/*
 * Copyright (C) 2009 Juergen Beisert, Pengutronix
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
 * @brief Generic PC support to let barebox acting as a boot loader
 */

#include <common.h>
#include <types.h>
#include <driver.h>
#include <init.h>
#include <asm/syslib.h>
#include <ns16550.h>

static struct memory_platform_data ram_pdata = {
	.name		= "ram0",
	.flags		= DEVFS_RDWR,
};

static struct device_d sdram_dev = {
	.id		= -1,
	.name		= "mem",
	.size		= 16 * 1024 * 1024,
	.map_base	= 0,
	.platform_data	= &ram_pdata,
};

static struct device_d bios_disk_dev = {
	.id		= -1,
	.name		= "biosdrive",
	.size		= 0,	/* auto guess */
};

/*
 * These datas are from the MBR, created by the linker and filled by the
 * setup tool while installing barebox on the disk drive
 */
extern uint64_t pers_env_storage;
extern uint16_t pers_env_size;
extern uint8_t pers_env_drive;

/**
 * Persistent environment "not used" marker.
 * Note: Must be in accordance to the value the tool "setup_mbr" writes.
 */
#define PATCH_AREA_PERS_SIZE_UNUSED 0x000

static int devices_init(void)
{
	int rc;

	sdram_dev.size = bios_get_memsize();	/* extended memory only */
	sdram_dev.size <<= 10;

	register_device(&sdram_dev);
	register_device(&bios_disk_dev);

	if (pers_env_size != PATCH_AREA_PERS_SIZE_UNUSED) {
		rc = devfs_add_partition("biosdisk0",
				pers_env_storage * 512,
				(unsigned)pers_env_size * 512,
				DEVFS_PARTITION_FIXED, "env0");
		printf("Partition: %d\n", rc);
	} else
		printf("No persistent storage defined\n");

        return 0;
}
device_initcall(devices_init);

#ifdef CONFIG_DRIVER_SERIAL_NS16550

static struct NS16550_plat serial_plat = {
       .clock = 1843200,
       .f_caps = CONSOLE_STDIN | CONSOLE_STDOUT | CONSOLE_STDERR,
       .reg_read = x86_uart_read,
       .reg_write = x86_uart_write,
};

/* we are expecting always one serial interface */
static struct device_d generic_pc_serial_device = {
       .id = -1,
       .name = "serial_ns16550",
       .map_base = 0x3f8,
       .size = 8,
       .platform_data = (void *)&serial_plat,
};

static int pc_console_init(void)
{
       /* Register the serial port */
       return register_device(&generic_pc_serial_device);
}
console_initcall(pc_console_init);

#endif

/** @page generic_pc Generic PC based bootloader

This platform acts as a generic PC based bootloader. It depends on at least
one boot media that is connected locally (no network boot) and can be
handled by the regular BIOS (any kind of hard disks for example).

The created @a barebox image can be used to boot a standard x86 bzImage
Linux kernel.

Refer section @ref x86_bootloader_preparations how to do so.

How to get the binary image:

Using the default configuration:

@code
make ARCH=x86 generic_defconfig
@endcode

Build the binary image:

@code
make ARCH=x86 CROSS_COMPILE=x86compiler
@endcode

@note replace the 'x86compiler' with your x86 (cross) compiler.

*/
