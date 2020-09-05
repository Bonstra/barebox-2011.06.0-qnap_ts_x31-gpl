/*
 *  arch/arm/mach-comcerto/include/mach/comcerto-common.h
 *
 *  Copyright (C) 2004,2005 Mindspeed Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ASM_ARCH_HARDWARE_H
#error "Do not include this directly, instead #include <asm/hardware.h>"
#endif

#ifndef __ASM_COMCERTO_COMMON_H__
#define __ASM_COMCERTO_COMMON_H__

#include <asm/types.h>

#define  BIT_0_MSK   0x00000001
#define  BIT_1_MSK   0x00000002
#define  BIT_2_MSK   0x00000004
#define  BIT_3_MSK   0x00000008
#define  BIT_4_MSK   0x00000010
#define  BIT_5_MSK   0x00000020
#define  BIT_6_MSK   0x00000040
#define  BIT_7_MSK   0x00000080
#define  BIT_8_MSK   0x00000100
#define  BIT_9_MSK   0x00000200
#define  BIT_10_MSK  0x00000400
#define  BIT_11_MSK  0x00000800
#define  BIT_12_MSK  0x00001000
#define  BIT_13_MSK  0x00002000
#define  BIT_14_MSK  0x00004000
#define  BIT_15_MSK  0x00008000
#define  BIT_16_MSK  0x00010000
#define  BIT_17_MSK  0x00020000
#define  BIT_18_MSK  0x00040000
#define  BIT_19_MSK  0x00080000
#define  BIT_20_MSK  0x00100000
#define  BIT_21_MSK  0x00200000
#define  BIT_22_MSK  0x00400000
#define  BIT_23_MSK  0x00800000
#define  BIT_24_MSK  0x01000000
#define  BIT_25_MSK  0x02000000
#define  BIT_26_MSK  0x04000000
#define  BIT_27_MSK  0x08000000
#define  BIT_28_MSK  0x10000000
#define  BIT_29_MSK  0x20000000
#define  BIT_30_MSK  0x40000000
#define  BIT_31_MSK  0x80000000

#define RETCODE_OK      0
#define RETCODE_ERROR   1

/* Bit 20 */
#define SPI_SC_POLARITY_MASK                            (BIT_20_MSK)
#define SPI_SC_POLARITY_SHIFT                           20

/*
 * GPIO
 */
	/* Set gpio pins specified by gpiomask to be outputs */
	#define comcerto_gpio_enable_output(gpiomask)	__raw_writel(__raw_readl(COMCERTO_GPIO_OE_REG) | (gpiomask), COMCERTO_GPIO_OE_REG)

	/* Set output pins specified by gpiomask to low */
	#define comcerto_gpio_set_0(gpiomask)	__raw_writel(__raw_readl(COMCERTO_GPIO_OUTPUT_REG) & ~(gpiomask), COMCERTO_GPIO_OUTPUT_REG)

	/* Set output pins specified by gpiomask to high */
	#define comcerto_gpio_set_1(gpiomask)	__raw_writel(__raw_readl(COMCERTO_GPIO_OUTPUT_REG) | (gpiomask), COMCERTO_GPIO_OUTPUT_REG)

	/* Read status of input pins specified by gpiomask */
	#define comcerto_gpio_read(gpiomask)	(__raw_readl(COMCERTO_GPIO_INPUT_REG) & (gpiomask))

#endif /* __ASM_COMCERTO_COMMON_H__ */
