
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


#include <config.h>
#include <common.h>
#include <init.h>
#include <driver.h>
#include <partition.h>
#include <fs.h>
#include <sizes.h>
#include <asm/io.h>
#include <asm/types.h>
#include <generated/mach-types.h>
#include <spi/spi.h>
#include <mach/comcerto-2000.h>
#include <mach/exp-bus.h>
#include <c2000_eth_pdata.h>
#include <mach/comcerto_spi.h>
#include <mach/gpio.h>
#include <asm/armlinux.h>
#include <fast_uart.h>
#include <i2c/i2c.h>
#include <mach/i2c.h>
#include <mach/otp.h>
#include <mach/ddr.h>


#ifdef CONFIG_SPI
//--------------
//Legacy spi

//FIXME: This will change when we know which device is connected to Legacy SPI on ASIC.

static int c2k_spi_cs[4] = {0};

static struct c2k_spi_master c2k_spi_1_data = {
        .chipselect = c2k_spi_cs,
        .num_chipselect = ARRAY_SIZE(c2k_spi_cs),
};

static struct spi_board_info c2k_spi_dev_1[] = {
        {
                .name = "S25FL064A", /* device name */
                .max_speed_hz = 4000000, /* max freq this device can work with */
                .bus_num = 0, /* which driver to attach this device to */
                .chip_select = 0, /* which chip select this device is connected to */
                .mode = SPI_CPOL | SPI_CPHA, /* operating modes */
        },
        {
                .name = "comcerto_spi1", /* device name */
                .max_speed_hz = 4000000, /* max freq this device can work with */
                .bus_num = 0, /* which driver to attach this device to */
                .chip_select = 1, /* which chip select this device is connected to */
                .mode = SPI_CPOL | SPI_CPHA, /* operating modes */
        },
        {
                .name = "comcerto_spi2", /* device name */
                .max_speed_hz = 4000000, /* max freq this device can work with */
                .bus_num = 0, /* which driver to attach this device to */
                .chip_select = 2, /* which chip select this device is connected to */
                .mode = SPI_CPOL | SPI_CPHA, /* operating modes */
        },
        {
                .name = "legerity", /* device name */
                .max_speed_hz = 4000000, /* max freq this device can work with */
                .bus_num = 0, /* which driver to attach this device to */
                .chip_select = 3, /* which chip select this device is connected to */
                .mode = SPI_CPOL | SPI_CPHA, /* operating modes */
        },
};

static struct device_d device_spi = {
        .id = -1,
        .name = "c2k_spi",
        .map_base = SPI_BASEADDR,
        .platform_data = &c2k_spi_1_data,
};
#endif

#ifdef CONFIG_DRIVER_FAST_SPI_C2K
//--------------
//Fast spi

static int c2k_fast_spi_cs[2] = {0};

static struct c2k_spi_master c2k_fast_spi_1_data = {
	.chipselect = c2k_fast_spi_cs,
	.num_chipselect = ARRAY_SIZE(c2k_spi_cs),
};

static struct spi_board_info c2k_fast_spi_dev_1[] = {
	{
		.name = "s25fl256",
		.max_speed_hz = 5*1000*1000,
		.bus_num = 1,
		.chip_select = 0,
		.mode = SPI_CPOL | SPI_CPHA,
	}
};

static struct device_d device_fast_spi = {
	.id = -1,
	.name = "c2k_fast_spi",
	.map_base = FAST_SPI_BASEADDR,
	.platform_data = &c2k_fast_spi_1_data,
};
#endif

#if defined(CONFIG_NET_COMCERTO_2000)
struct c2000_eth_platform_data eth0_pdata = {
	.gemac_port = 0,
	.mac_addr = 0,
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
	.mac_addr = 0,
};

struct device_d c2000_eth1 = {
	.id = 1,
	.name = "c2000_eth",
	.map_base = COMCERTO_AXI_HFE_CFG_BASE,
	/* .size  = 16M, */
	.platform_data = &eth1_pdata,
};

struct c2000_eth_platform_data eth2_pdata = {
	.gemac_port = 2,
	.mac_addr = 0,
};

struct device_d c2000_eth2 = {
	.id = 2,
	.name = "c2000_eth",
	.map_base = COMCERTO_AXI_HFE_CFG_BASE,
	/* .size  = 16M, */
	.platform_data = &eth2_pdata,
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
	.size = SZ_512M,
	.platform_data = &sdram_pdata,
};

static struct fast_uart_plat serial_plat = {
	.clock = 200000000,      /* 200MHz  */
};

static struct device_d fast_uart_device = {
	.id       = -1,
	.name     = "fast_uart",
	.map_base = UART_BASEADDR,
	.platform_data = (void *)&serial_plat,
};

#ifdef CONFIG_I2C_C2K
struct i2c_platform_data pdata = {
	.bitrate = 0x40000
};

struct device_d c2k_i2c_dev = {
	.id       = -1,
	.name     = "c2k_i2c",
	.map_base = COMCERTO_I2C_BASE,
	.platform_data = &pdata,
};

static struct i2c_board_info i2c_devices[] = {
	{
		I2C_BOARD_INFO("eeprom0", CFG_I2C_EEPROM0_ADDR),
	},
	{
		I2C_BOARD_INFO("eeprom1", CFG_I2C_EEPROM1_ADDR),
	},
};
#endif

#ifdef CONFIG_OTP
struct device_d c2k_otp_dev = {
        .name     = "c2k_otp",
        .map_base = COMCERTO_OTP_BASE,
        .size     = OTP_SIZE,
};
#endif

struct device_d c2k_nor_dev = {
	.id       = -1,
	.name     = "cfi_flash",
	.map_base = COMCERTO_AXI_EXP_BASE,
	.size     = NOR_FLASH_SIZE,
};

struct device_d csi_flash_dev = {
	.id       = -1,
	.name     = "csi_flash",
};

static int c2000_device_init(void)
{
#ifdef	CONFIG_COMCERTO_BOOTLOADER
	u32 bootopt;
#endif
	int clk = HAL_get_axi_clk(); // Get AXI bus freq in MHz

	serial_plat.clock = clk * 1000 * 1000;
	register_device(&fast_uart_device);

#ifdef CONFIG_I2C_C2K
	i2c_register_board_info(0, i2c_devices, ARRAY_SIZE(i2c_devices));
	register_device(&c2k_i2c_dev);
#endif

#ifdef CONFIG_SPI
        spi_register_board_info(c2k_spi_dev_1, ARRAY_SIZE(c2k_spi_dev_1));
        register_device(&device_spi);
#endif

#ifdef CONFIG_OTP
        register_device(&c2k_otp_dev);
#endif

#ifdef CONFIG_DRIVER_FAST_SPI_C2K
        spi_register_board_info(c2k_fast_spi_dev_1, ARRAY_SIZE(c2k_fast_spi_dev_1));
        register_device(&device_fast_spi);
#endif

#if defined(CONFIG_NET_COMCERTO_2000)
	register_device(&c2000_eth0);
	register_device(&c2000_eth1);
	register_device(&c2000_eth2);
#endif
	register_device(&c2k_nor_dev);
	register_device(&csi_flash_dev);

	//Exp bus soft reset
	writel (0x1, EXP_SWRST_REG);
	while(readl(EXP_SWRST_REG) & 0x1);	

	//16-bit NOR bus is used on C2K ASIC board
	writel(((0x1) << 1), EXP_CS0_CFG_REG);

	/* IBR, when booting from NOR, is changing expansion bus CS0 Mem Segment size to 1MiB
	instead of the default reset value of 128MiB. Put back the reset default value */
	writel(EXP_CS0_SEG_END_VAL, EXP_CS0_SEG_REG);

	writel(EXP_CS1_BASE_VAL, EXP_CS1_BASE_REG);
	writel(EXP_CS1_SEG_END_VAL, EXP_CS1_SEG_REG);

	writel(EXP_CS2_BASE_VAL, EXP_CS2_BASE_REG);
	writel(EXP_CS2_SEG_END_VAL, EXP_CS2_SEG_REG);

	writel(EXP_CS3_BASE_VAL, EXP_CS3_BASE_REG);
	writel(EXP_CS3_SEG_END_VAL, EXP_CS3_SEG_REG);

#ifdef CONFIG_COMCERTO_NAND
	writel(EXP_CS4_BASE_VAL, EXP_CS4_BASE_REG);
	writel(EXP_CS4_SEG_END_VAL, EXP_CS4_SEG_REG);
#ifdef CONFIG_NAND_TYPE_SLC
        //SLC NAND on ASIC is 16-bit. Enable 16-bit bus
        writel(readl(EXP_CS4_CFG_REG) | 0x2, EXP_CS4_CFG_REG);
#else
        //MLC NAND on ASIC is 8-bit. Enable 8-bit bus
        writel((readl(EXP_CS4_CFG_REG) & (~0x6)), EXP_CS4_CFG_REG);
#endif
#endif

	/* External reset to PCIe devices, Atheros Switch and FXS block, GPIO27 is used for TM_EXT_RESET*/
	writel(readl(COMCERTO_GPIO_OUTPUT_REG) & ~GPIO_27, COMCERTO_GPIO_OUTPUT_REG);
	writel( readl(COMCERTO_GPIO_OE_REG) | GPIO_27, COMCERTO_GPIO_OE_REG);
	writel(readl(COMCERTO_GPIO_OUTPUT_REG) | GPIO_27, COMCERTO_GPIO_OUTPUT_REG);

	//For 16bit ddr, reduce the size to half 
	if (is_16bitDDR_config())
		sdram_dev.size >>= 1;

#ifdef	CONFIG_COMCERTO_BOOTLOADER
	armlinux_add_dram(&sdram_dev);
	armlinux_set_bootparams((void *)0x100);
	armlinux_set_architecture(MACH_TYPE_COMCERTO);

	bootopt = ((readl(COMCERTO_GPIO_SYSTEM_CONFIG) >>  BOOTSTRAP_BOOT_OPT_SHIFT) & BOOTSTRAP_BOOT_OPT_MASK);
	if(!bootopt){
		/* ENV for SPI */
		printf("Using ENV from SPI Flash.\n");
		devfs_add_partition("spi0", 0xA0000, 0x20000, PARTITION_FIXED, "env0");
		protect_file("/dev/env0", 1);
	}else{
		/* ENV for NOR */
		printf("Using ENV from NOR Flash.\n");
		devfs_add_partition("nor0", 0xA0000, 0x20000, PARTITION_FIXED, "env0");
		protect_file("/dev/env0", 1);
	}
#endif
	return 0;
}

int c2000_eth_board_init(int eth_port)
{
	//eth0 port is chosen as criteria for bringingup out of reset because
	//all MDIO access can happen through EMAC0 and without bringing eth0 first
	//no PHY configuration can happen and no point in removing reset without eth0
	if(eth_port != 0)
		return 0;

	printf("%s: Bringing PHY out of reset\n",__func__);
	//Bring all PHYs out of reset. The correct way is to assert, deassert and re-assert the GPIO reset signal
	//with delay in between
        writel(readl(COMCERTO_GPIO_OE_REG) | GPIO_3, COMCERTO_GPIO_OE_REG);

	writel(readl(COMCERTO_GPIO_OUTPUT_REG) | GPIO_3, COMCERTO_GPIO_OUTPUT_REG);
        udelay(100000);
	writel(readl(COMCERTO_GPIO_OUTPUT_REG) & ~GPIO_3, COMCERTO_GPIO_OUTPUT_REG);
        udelay(100000);
	writel(readl(COMCERTO_GPIO_OUTPUT_REG) | GPIO_3, COMCERTO_GPIO_OUTPUT_REG);
        udelay(100000);

}

device_initcall(c2000_device_init);

