/*
 *  arch/arm/mach-comcerto/include/mach/comcerto-2000.h
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

#ifndef __COMCERTO_2000_H__
#define __COMCERTO_2000_H__

/*
 * IRAM Location to keep C2K Part Number.
 * This location should be fixed and can not be changed.
 * Microloader populates this information to be used by Linux.
 *
 * This location shouldn't overlap IRAM_CLK_REG_MIRROR space
 * defined in clkcore.h
 */
#define IRAM_C2K_PART_NO_LOCATION	0x8300FE00


/*
 * AXI Bus
 */
#define COMCERTO_AXI_DDR_BASE		0x00000000 /* 2G */
#define COMCERTO_AXI_ACP_BASE		0x80000000 /* 48MB */
#define COMCERTO_AXI_IRAM_BASE		0x83000000 /* 48MB */
#define COMCERTO_AXI_IBR_BASE		0x90000000 /* 4MB */
#define COMCERTO_AXI_APB_CFG_BASE	0x90400000 /* 12MB */
#define COMCERTO_AXI_SEM_CFG_BASE	0x91000000 /* 16MB */
#define COMCERTO_AXI_USB2p0_CFG_BASE	0x92000000 /* 16MB */
#define COMCERTO_AXI_TZ_CFG_BASE	0x93000000 /* 16MB */
#define COMCERTO_AXI_DPI0_CFG_BASE	0x94000000 /* 16MB */
#define COMCERTO_AXI_DPI1_CFG_BASE	0x95000000 /* 16MB */
#define COMCERTO_AXI_UART_SPI_CFG_BASE	0x96000000 /* 16MB */
#define COMCERTO_AXI_DDR_CFG_BASE	0x97000000 /* 16MB */
#define COMCERTO_AXI_PCIe0_CFG_BASE	0x98000000 /* 16MB */
#define COMCERTO_AXI_PCIe1_CFG_BASE	0x99000000 /* 16MB */
#define COMCERTO_AXI_ESPAH_CFG_BASE	0x9A000000 /* 16MB */
#define COMCERTO_AXI_SPACC_CFG_BASE	0x9B000000 /* 16MB */
#define COMCERTO_AXI_HFE_CFG_BASE	0x9C000000 /* 16MB */
#define COMCERTO_AXI_SATA_CFG_BASE	0x9D000000 /* 16MB */
#define COMCERTO_AXI_DECT_CFG_BASE	0x9E000000 /* 16MB */
#define COMCERTO_AXI_USB3p0_CFG_BASE	0x9F000000 /* 16MB */
#define COMCERTO_AXI_PCIe0_SLV_BASE	0xA0000000 /* 256MB */
#define COMCERTO_AXI_PCIe1_SLV_BASE	0xB0000000 /* 256MB */
#define COMCERTO_AXI_EXP_BASE		0xC0000000 /* 256MB */
#define COMCERTO_AXI_EXP_ECC_BASE	0xCFFF0000 /* 64KB */

/*
 * APB Bus
 */
#define COMCERTO_TDM_BASE			(COMCERTO_AXI_APB_CFG_BASE + 0x000000)
#define COMCERTO_USB_PHY_SERDES_BASE		(COMCERTO_AXI_APB_CFG_BASE + 0x010000)
#define COMCERTO_TDMA_BASE			(COMCERTO_AXI_APB_CFG_BASE + 0x020000)
#define COMCERTO_APB_RESERVED2			(COMCERTO_AXI_APB_CFG_BASE + 0x030000)
#define COMCERTO_APB_RESERVED3			(COMCERTO_AXI_APB_CFG_BASE + 0x040000)
#define COMCERTO_TIMER_BASE   			(COMCERTO_AXI_APB_CFG_BASE + 0x050000)
#define COMCERTO_PCIE_SATA_USB_CTRL_BASE	(COMCERTO_AXI_APB_CFG_BASE + 0x060000)
#define COMCERTO_GPIO_BASE			(COMCERTO_AXI_APB_CFG_BASE + 0x070000)
#define COMCERTO_APB_RESERVED5			(COMCERTO_AXI_APB_CFG_BASE + 0x080000)
#define COMCERTO_UART0_BASE   			(COMCERTO_AXI_APB_CFG_BASE + 0x090000)
#define COMCERTO_APB_RESERVED6			(COMCERTO_AXI_APB_CFG_BASE + 0x094000)
#define COMCERTO_SPI_BASE     			(COMCERTO_AXI_APB_CFG_BASE + 0x098000)
#define COMCERTO_I2C_BASE     			(COMCERTO_AXI_APB_CFG_BASE + 0x09C000)
#define COMCERTO_USB3_0_BASE  			(COMCERTO_AXI_APB_CFG_BASE + 0x0A0000)
#define COMCERTO_CLKCORE_BASE 			(COMCERTO_AXI_APB_CFG_BASE + 0x0B0000)
#define COMCERTO_APB_RESERVED7			(COMCERTO_AXI_APB_CFG_BASE + 0x0C0000)
#define COMCERTO_APB_RESERVED8			(COMCERTO_AXI_APB_CFG_BASE + 0x0D0000)
#define COMCERTO_RTC_BASE     			(COMCERTO_AXI_APB_CFG_BASE + 0x0E0000)
#define COMCERTO_OTP_BASE     			(COMCERTO_AXI_APB_CFG_BASE + 0x0F0000)
#define COMCERTO_HFEWRAPPER_BASE		(COMCERTO_AXI_APB_CFG_BASE + 0x100000)
#define COMCERTO_APB_RESERVED10			(COMCERTO_AXI_APB_CFG_BASE + 0x110000)
#define COMCERTO_APB_RESERVED11			(COMCERTO_AXI_APB_CFG_BASE + 0x120000)
#define COMCERTO_APB_RESERVED12			(COMCERTO_AXI_APB_CFG_BASE + 0x130000)
#define COMCERTO_APB_RESERVED13			(COMCERTO_AXI_APB_CFG_BASE + 0x140000)
#define COMCERTO_APB_RESERVED14			(COMCERTO_AXI_APB_CFG_BASE + 0x150000)
#define COMCERTO_APB_RESERVED15			(COMCERTO_AXI_APB_CFG_BASE + 0x160000)
#define COMCERTO_APB_RESERVED16			(COMCERTO_AXI_APB_CFG_BASE + 0x170000)
#define COMCERTO_APB_RESERVED17			(COMCERTO_AXI_APB_CFG_BASE + 0x180000)
#define COMCERTO_SERDES_CFG_BASE		(COMCERTO_AXI_APB_CFG_BASE + 0x190000)
#define COMCERTO_EXP_CONF_BASE			(COMCERTO_AXI_APB_CFG_BASE + 0x1A0000)
#define COMCERTO_DDR_PHY			(COMCERTO_AXI_APB_CFG_BASE + 0x1B0000)
#define COMCERTO_APB_RESERVED20			(COMCERTO_AXI_APB_CFG_BASE + 0x1C0000)
#define COMCERTO_TDMA2_BASE			(COMCERTO_AXI_APB_CFG_BASE + 0x1D0000)
#define COMCERTO_MDMA_BASE			(COMCERTO_AXI_APB_CFG_BASE + 0x1E0000)
#define COMCERTO_A9_CORESIGHT_BASE		(COMCERTO_AXI_APB_CFG_BASE + 0x200000)

#define COMCERTO_APB_USBPHY_SERDES_STAT_BASE	0x90410000
#define COMCERTO_APB_GPIO_BASE		0x90470000
#define COMCERTO_APB_CLK_BASE		0x904B0000
#define COMCERTO_APB_SERDES_BASE	0x90590000
#define COMCERTO_APB_EXP_BASE		0x905A0000
#define COMCERTO_APB_DDR_BASE		0x97000000
#define COMCERTO_APB_DDR_PHY_BASE	0x905B0000
#define COMCERTO_SATA_AHCI_BASE		0x9D000000

#define COMCERTO_L2CC_BASE       0xFFF10000
#define COMCERTO_A9_PERIPH_BASE     0xFFF00000
#define COMCERTO_A9_SCU_BASE		(COMCERTO_A9_PERIPH_BASE + 0x0000)
#define COMCERTO_A9_IC_INT_BASE		(COMCERTO_A9_PERIPH_BASE + 0x100)
#define COMCERTO_A9_TIMER_BASE		(COMCERTO_A9_PERIPH_BASE + 0x600)
#define COMCERTO_A9_IC_DIST_BASE	(COMCERTO_A9_PERIPH_BASE + 0x1000)

#define COMCERTO_APB_SERDES0_BASE	(COMCERTO_APB_SERDES_BASE)
#define COMCERTO_APB_SERDES1_BASE	(COMCERTO_APB_SERDES_BASE + 0x4000)
#define COMCERTO_APB_SERDES2_BASE	(COMCERTO_APB_SERDES_BASE + 0x8000)
#define COMCERTO_SER_DES0_PHY_CFG_BASE 	0x9041002C

#define EXP_SWRST	(COMCERTO_APB_EXP_BASE + 0x0)
#define EXP_CSEN	(COMCERTO_APB_EXP_BASE + 0x4)
#define EXP_CS0_BASE 	(COMCERTO_APB_EXP_BASE + 0x8)
#define EXP_CS0_SEG 	(COMCERTO_APB_EXP_BASE + 0x1c)
#define EXP_CSO_CFG	(COMCERTO_APB_EXP_BASE + 0x30)

#define DECT_SYS_CFG0	(COMCERTO_APB_GPIO_BASE + 0xb0)
#define DECT_SYS_CFG1	(COMCERTO_APB_GPIO_BASE + 0xb4)
#define DECT_CTRL	(COMCERTO_APB_GPIO_BASE + 0xb8)

#define COMCERTO_SERDES_REG( _num, _ofst) ((COMCERTO_APB_SERDES_BASE + (0x4000 * _num)) + _ofst)

#define TEMP_STACK     0x8300C000

#define UART_BASEADDR	0x96400000

//Patch by QNAP:Enable UART0 for PIC
#define PIC_BASEADDR	0x96300000
//////////////////////////
/*
  * Reference Clock Option in Boot Strap Register
  *
  * BIT [9:8] System PLL Refclk Select
  * '00' - USB XTAL
  * '01' - Serdes #0 Refclk
  * '10' - Serdes #1 Refclk
  * '11' - Serdes XTAL
  *
  * BIT[7]   Serdes OSC PAD - Reference clock frequency selection bootstrap (inverted value of SF1)
  * '0' - 30MHz ~ 50MHz
  * '1' - 15MHz ~ 30MHz
  * Note: SF0 is tied to 1 in GPIO block and might later on be controlled during DFT
  *
  * BIT[5]   USB/Sys PLL OSC PAD - Reference clock frequency selection bootstrap (inverted value of SF1)
  *'0' - 30MHz ~ 50MHz
  * '1' - 15MHz ~ 30MHz
  * Note: SF0 is tied to 1 in GPIO block and might later on be controlled during DFT
  *
  */

#define GPIO_SYS_PLL_REF_CLK_SHIFT                      8
#define GPIO_SYS_PLL_REF_CLK_MASK                       (0x3 << GPIO_SYS_PLL_REF_CLK_SHIFT)

#define USB_XTAL_REF_CLK                                        0
#define SERDES_0_REF_CLK                                        1
#define SERDES_2_REF_CLK                                        2
#define SERDES_XTAL_REF_CLK                                     3

#define GPIO_SERDES_OSC_PAD_SHIFT                       7
#define GPIO_SERDES_OSC_PAD_MASK                        (1 << GPIO_SERDES_OSC_PAD_SHIFT)

#define GPIO_USB_OSC_PAD_SHIFT                          5
#define GPIO_USB_OSC_PAD_MASK                           (1 << GPIO_USB_OSC_PAD_SHIFT)

#endif /* __COMCERTO_2000_H__ */
