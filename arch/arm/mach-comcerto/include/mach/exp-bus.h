/*
 *  arch/arm/mach-comcerto/include/mach/exp-bus.h
 *
 *  Copyright (C) 2011 Mindspeed Technologies, Inc.
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

#ifndef __COMCERTO_EXP_BUS_H__
#define __COMCERTO_EXP_BUS_H__

/*
 *  Expansion Bus control registers
 */
#define EXP_SWRST_REG            (COMCERTO_EXP_CONF_BASE + 0x00)
#define EXP_CSEN_REG             (COMCERTO_EXP_CONF_BASE + 0x04)
#define EXP_CS0_BASE_REG         (COMCERTO_EXP_CONF_BASE + 0x08)
#define EXP_CS1_BASE_REG         (COMCERTO_EXP_CONF_BASE + 0x0C)
#define EXP_CS2_BASE_REG         (COMCERTO_EXP_CONF_BASE + 0x10)
#define EXP_CS3_BASE_REG         (COMCERTO_EXP_CONF_BASE + 0x14)
#define EXP_CS4_BASE_REG         (COMCERTO_EXP_CONF_BASE + 0x18)
#define EXP_CS0_SEG_REG          (COMCERTO_EXP_CONF_BASE + 0x1C)
#define EXP_CS1_SEG_REG          (COMCERTO_EXP_CONF_BASE + 0x20)
#define EXP_CS2_SEG_REG          (COMCERTO_EXP_CONF_BASE + 0x24)
#define EXP_CS3_SEG_REG          (COMCERTO_EXP_CONF_BASE + 0x28)
#define EXP_CS4_SEG_REG          (COMCERTO_EXP_CONF_BASE + 0x2C)
#define EXP_CS0_CFG_REG		 (COMCERTO_EXP_CONF_BASE + 0x30)
#define EXP_CS1_CFG_REG		 (COMCERTO_EXP_CONF_BASE + 0x34)
#define EXP_CS2_CFG_REG		 (COMCERTO_EXP_CONF_BASE + 0x38)
#define EXP_CS3_CFG_REG		 (COMCERTO_EXP_CONF_BASE + 0x3C)
#define EXP_CS4_CFG_REG		 (COMCERTO_EXP_CONF_BASE + 0x40)
#define EXP_CS0_TMG1_REG         (COMCERTO_EXP_CONF_BASE + 0x44)
#define EXP_CS1_TMG1_REG         (COMCERTO_EXP_CONF_BASE + 0x48)
#define EXP_CS2_TMG1_REG         (COMCERTO_EXP_CONF_BASE + 0x4C)
#define EXP_CS3_TMG1_REG         (COMCERTO_EXP_CONF_BASE + 0x50)
#define EXP_CS4_TMG1_REG         (COMCERTO_EXP_CONF_BASE + 0x54)
#define EXP_CS0_TMG2_REG         (COMCERTO_EXP_CONF_BASE + 0x58)
#define EXP_CS1_TMG2_REG         (COMCERTO_EXP_CONF_BASE + 0x5C)
#define EXP_CS2_TMG2_REG         (COMCERTO_EXP_CONF_BASE + 0x60)
#define EXP_CS3_TMG2_REG         (COMCERTO_EXP_CONF_BASE + 0x64)
#define EXP_CS4_TMG2_REG         (COMCERTO_EXP_CONF_BASE + 0x68)
#define EXP_CS0_TMG3_REG         (COMCERTO_EXP_CONF_BASE + 0x6C)
#define EXP_CS1_TMG3_REG         (COMCERTO_EXP_CONF_BASE + 0x70)
#define EXP_CS2_TMG3_REG         (COMCERTO_EXP_CONF_BASE + 0x74)
#define EXP_CS3_TMG3_REG         (COMCERTO_EXP_CONF_BASE + 0x78)
#define EXP_CS4_TMG3_REG         (COMCERTO_EXP_CONF_BASE + 0x7C)
#define EXP_CLOCK_DIV_REG        (COMCERTO_EXP_CONF_BASE + 0x80)

#define EXP_MFSM_REG             (COMCERTO_EXP_CONF_BASE + 0x100)
#define EXP_CSFSM_REG            (COMCERTO_EXP_CONF_BASE + 0x104)
#define EXP_WRFSM_REG            (COMCERTO_EXP_CONF_BASE + 0x108)
#define EXP_RDFSM_REG            (COMCERTO_EXP_CONF_BASE + 0x10C)

#define EXP_SW_RST	 0x00000001
#define EXP_CLK_EN       0x00000001
#define EXP_CSBOOT_EN    0x00000002
#define EXP_CS0_EN       0x00000002
#define EXP_CS1_EN       0x00000004
#define EXP_CS2_EN       0x00000008
#define EXP_CS3_EN       0x00000010
#define EXP_CS4_EN       0x00000020
#define EXP_MEM_BUS_8    0x00000000
#define EXP_MEM_BUS_16   0x00000002
#define EXP_CS_HIGH      0x00000008
#define EXP_WE_HIGH      0x00000010
#define EXP_RE_HIGH      0x00000020
#define EXP_ALE_MODE     0x00000040
#define EXP_STRB_MODE    0x00000080
#define EXP_DM_MODE      0x00000100
#define EXP_NAND_MODE    0x00000200
#define EXP_RDY_EN       0x00000400
#define EXP_RDY_EDGE     0x00000800

#define COMCERTO_NAND_RDY	GPIO_29
#define COMCERTO_NAND_CE	GPIO_28

#if defined(CONFIG_MACH_COMCERTO_C2K_ASIC) && defined(CONFIG_NAND_TYPE_SLC)
//SLC NAND on ASIC is 16-bit, and bit[0] of Exp bus is used for data mask
//hence the ALE and CLE signal is shifted by 1 bit
#define COMCERTO_NAND_ALE	(BIT_9_MSK << 1)
#define COMCERTO_NAND_CLE	(BIT_10_MSK << 1)
#else
#define COMCERTO_NAND_ALE	BIT_9_MSK
#define COMCERTO_NAND_CLE	BIT_10_MSK
#endif

#endif	/* __COMCERTO_EXP_BUS_H__ */
