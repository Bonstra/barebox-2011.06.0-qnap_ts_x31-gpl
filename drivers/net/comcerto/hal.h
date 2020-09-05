/*
 *  (C) Copyright 2011
 *  Author : Mindspeed Technologes
 *  
 *  See file CREDITS for list of people who contributed to this
 *  project.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *  MA 02111-1307 USA
 * */

#ifndef _HAL_H_
#define _HAL_H_

#if defined(CONFIG_PLATFORM_PCI)  
/* For ChipIT */

#include <linux/types.h>
#include <linux/elf.h>
#include <linux/errno.h>
#include <linux/pci.h>
#include <asm/io.h>
#include <linux/slab.h>
#include <linux/firmware.h>

#include "pfe_mod.h"

#define free(x)  kfree(x)
#define xzalloc(x)  kmalloc(x, GFP_DMA)
#define printf  printk

//#define dprint(fmt, arg...)	printk(fmt, ##arg)
#define dprint(fmt, arg...)	

#else

#include <linux/types.h>
#include <elf.h>
#include <common.h>
#include <errno.h>
#include <asm/byteorder.h>
#include <miidev.h>
#include <malloc.h>
#include <asm/io.h>


#include "c2000_eth.h"

#endif


#endif /* _HAL_H_ */

