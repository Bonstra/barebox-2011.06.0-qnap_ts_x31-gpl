/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

/* The DEBUG define must be before common to enable debugging */
/* #define DEBUG	*/

#include <common.h>
#include <asm/byteorder.h>
#include <environment.h>
#include <clock.h>
#include <init.h>
#include <malloc.h>
#include <asm/io.h>
#include <errno.h>
#include <progress.h>

#include "csi_flash.h"

extern int spi_copy_read(char *, unsigned int, unsigned int, unsigned int);
extern int update_spi_flash(unsigned char *, ulong, ulong, ulong);
extern int spi_sector_erase(int, int);

static ssize_t csi_read(struct cdev *cdev, const void *buf, size_t count, \
		ulong offset, ulong flags)
{
        struct csi_flash_info *finfo = (struct flash_info *)cdev->priv;
	ulong sec = offset/finfo->sector_size;
	ulong off = offset%finfo->sector_size;

	debug("%s:%d: buf=0x%p count=0x%08x offset=0x%x flags=0x%x \
		      \nsec=%d off=0x%x\n",__func__, __LINE__, \
		      buf, count, offset, flags, sec, off);

	spi_copy_read((char*)buf, count, sec, off);

	return count;
}

static ssize_t csi_write(struct cdev *cdev, const void *buf, size_t count, \
		unsigned long offset, ulong flags)
{
        struct csi_flash_info *finfo = (struct flash_info *)cdev->priv;
	ulong sec = offset/finfo->sector_size;
	ulong off = offset%finfo->sector_size;

	debug("%s:%d: buf=0x%p count=0x%08x offset=0x%x flags=0x%x sec=%d \
		       \noff=0x%x\n",__func__, __LINE__, \
		       buf, count, offset, flags, sec, off);

	update_spi_flash((char*)buf, sec, off, count);

	return count;
}

static int csi_erase(struct cdev *cdev, size_t count, unsigned long offset)
{
        struct csi_flash_info *finfo = (struct flash_info *)cdev->priv;
	ulong sec = offset/finfo->sector_size;

	debug ("%s: sec=%d offset=0x%x\n", __func__, sec, offset);

	spi_sector_erase(sec, 2);

	return 0;
}

struct file_operations csi_ops = {
	.read	= csi_read,
	.write	= csi_write,
	.erase	= csi_erase,
};

static int csi_probe (struct device_d *dev)
{
	struct csi_flash_info *info = xzalloc(sizeof(*info));

	dev->priv = (void *)info;

	debug("%s:%d: %s size: 0x%08x\n", __func__, __LINE__, \
			dev->name, dev->size);

	/* Init: no FLASHes known */
	info->flash_id = FLASH_UNKNOWN;
	info->size = dev->size;
	info->sector_size = SPI_FLASH_SECTOR_SIZE;
	info->sector_count = info->size/info->sector_size;
	

	if (dev->size == 0) {
		printf("cfi_probe: size : 0x%08lx\n", info->size);
		dev->size = info->size;
	}

	info->cdev.name = asprintf("spi%d", dev->id);
	info->cdev.size = info->size;
	info->cdev.dev = dev;
	info->cdev.ops = &csi_ops;
	info->cdev.priv = info;

	devfs_create(&info->cdev);

	return 0;
}

static struct driver_d csi_driver = {
        .name    = "csi_flash",
        .probe   = csi_probe,
};

static int csi_init(void)
{
        return register_driver(&csi_driver);
}

device_initcall(csi_init);

