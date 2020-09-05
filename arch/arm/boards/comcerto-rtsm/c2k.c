
#include <common.h>
#include <net.h>
#include <init.h>
#include <environment.h>
#include <asm/armlinux.h>
#include <generated/mach-types.h>
#include <command.h>
#include <spi/spi.h>
#include <asm/io.h>

#include <mach/comcerto_spi.h>

//--------------
//Legacy spi

static int c2k_spi_cs[] = {0};

static struct c2k_spi_master c2k_spi_1_data = {
        .chipselect = c2k_spi_cs,
        .num_chipselect = ARRAY_SIZE(c2k_spi_cs),
};

static struct spi_board_info c2k_spi_dev_1[] = {
        {
                .name = "spi_dev_1",
                .max_speed_hz = 4000000,
                .bus_num = 0,
                .chip_select = 0,
        	.mode = SPI_CPOL | SPI_CPHA,
        }
};

static struct device_d device_spi = {
        .id = -1,
        .name = "c2k_spi",
        .map_base = SPI_BASEADDR,
        .platform_data = &c2k_spi_1_data,
};

//--------------
//Fast spi

static int c2k_fast_spi_cs[] = {0};

static struct c2k_spi_master c2k_fast_spi_1_data = {
        .chipselect = c2k_fast_spi_cs,
        .num_chipselect = ARRAY_SIZE(c2k_spi_cs),
};

static struct spi_board_info c2k_fast_spi_dev_1[] = {
        {
                .name = "fast_spi_dev_eeprom",
                .max_speed_hz = 4000000,
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

static int c2k_spi_devices_init(void)
{
	spi_register_board_info(c2k_spi_dev_1, ARRAY_SIZE(c2k_spi_dev_1));

	spi_register_board_info(c2k_fast_spi_dev_1, ARRAY_SIZE(c2k_fast_spi_dev_1));

        register_device(&device_spi);

        register_device(&device_fast_spi);

	return 0;
}

device_initcall(c2k_spi_devices_init);

