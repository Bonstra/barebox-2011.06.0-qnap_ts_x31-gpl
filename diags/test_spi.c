/*
 * test_spi.c
 *
 * Copyright (C) 2004,2005 Mindspeed Technologies, Inc.
 *
 * In case of Diags test for SPI/Fast SPI, test will be specific
 * to the device connected to through the SPI/Fast SPI controller.
 * This Diags test is specific to the NOR S25FL128P/S25FL256S connected
 * through SPI/fast SPI in case of EVM/ASIC.
 *
 * FLASH 128_P(Legacy SPI on EVM)
 *	- READ: command is written to SI, followed by a 3-byte address (A23-A0).
 *	- PP: A WREN command is required prior to writing the PP command.
 *	write the PP command, three address bytes, and at least one data byte on SI.
 *	- SE: The Sector Erase (SE) command sets all bits at all addresses within a
 *	specified sector to a logic 1. A WREN command is required prior to writing
 *	the SE command. Write the SE command plus three address bytes on SI.
 *
 * FLASH 256_S(Fast SPI on ASIC)
 *	- Extended Address (EXTADD) BAR[7]: EXTADD controls the address
 *	field size for legacy SPI commands. BY DEFAULT (power up reset,
 *	hardware reset, and software reset), it is cleared to 0 for 3 bytes (24 bits)
 *	of address. When set to 1, the legacy commands will require 4 bytes (32 bits)
 *	for the address field. This is a volatile bit.
 *
 *	- READ:
 *	  03h (ExtAdd=0) is followed by a 3-byte address (A23-A0) or
 *	  03h (ExtAdd=1) is followed by a 4-byte address (A31-A0) or
 *
 *	- Page Program:
 *	  02h (ExtAdd=0) is followed by a 3-byte address (A23-A0) or
 *	  02h (ExtAdd=1) is followed by a 4-byte address (A31-A0) or
 *
 *	- Erase:
 *	  20h [ExtAdd=0] is followed by a 3-byte address (A23-A0), or
 *	  20h [ExtAdd=1] is followed by a 4-byte address (A31-A0), or
 */

#include <malloc.h>
#include <diags.h>
#include <spi/spi.h>
#include <mach/comcerto_spi.h>
#include <common.h>
#include <linux/list.h>
#include <linux/types.h>

#define SPI_FAILURE	-1
#define SPI_SUCCESS	0

#define DEVICE_FAST_SPI	"s25fl2560"

//#define COMCERTO_SPI_DEBUG
#ifdef COMCERTO_SPI_DEBUG
	#define spi_debug(fmt, arg...)   printf(fmt, ##arg)
#else
	#define spi_debug(fmt, arg...)	;
#endif

#define C2K_ASIC
#define BUF_SIZE	8

#define NOR_FLASH_SIZE	(16 * 1024 * 1024) /* 16 MB */
#define	SECTOR_64KB	(64 * 1024)
#define	SECTOR_256KB	(256 * 1024)
#define NOR_PAGE_SIZE	256 /* Bytes */
#define SECTOR_SIZE	SECTOR_64KB
#define NUM_SECTORS	(NOR_FLASH_SIZE/SECTOR_SIZE)
#define NUM_PAGES_PER_SEC	(SECTOR_SIZE/NOR_PAGE_SIZE)

extern int spi_write(struct spi_device *spi, const void *buf, size_t len);
extern int spi_read(struct spi_device *spi, void *buf, size_t len);
extern int spi_write_then_read(struct spi_device *spi,\
                const void *txbuf, unsigned n_tx,\
                void *rxbuf, unsigned n_rx);
extern int spi_eeprom_read(struct spi_device *spi,\
                const void *txbuf, unsigned n_tx,\
                void *rxbuf, unsigned n_rx);

/* S25FL128: rd_cmd: |1B_cmd|3B_addr|dummy_bytes| */
uint8_t s25fl128_rd_cmd[4+SECTOR_SIZE] = {
	0x03, 0x00, 0x00, 0x00,
};

/* buffer to receive data */
uint8_t s25fl128_rd_buf[SECTOR_SIZE*2] = {11,22,33,44,55,66,77,88,99};

/* S25FL128: wren_cmd: |1B_cmd| */
uint8_t s25fl128_wren_cmd[BUF_SIZE] = {
	0x06, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

#if (SECTOR_SIZE == SECTOR_64KB)
/* S25FL128: erase_cmd: |1B_cmd|3B_addr| */
uint8_t s25fl128_erase_cmd[BUF_SIZE] = {
	0xd8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};
#else
 /* S25FL128: erase_cmd: |1B_cmd|3B_addr| */
uint8_t s25fl128_erase_cmd[BUF_SIZE] = {
 	0xd8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};
#endif

/* S25FL128: pp_cmd: |1B_cmd|3B_addr|at_least_1B_data|erite_buf| */
uint8_t s25fl128_pp_cmd[BUF_SIZE+NOR_PAGE_SIZE] = {
	0x02, 0x00, 0x00, 0x00,
};

void print_buffer(char *buf, int len)
{
	int i = 0;

	printf ("Buffer(0x%p) of len(%d): ", buf, len);

	while(i++ < len)
		printf("0x%x:", *buf++);

	printf (" Buffer End###\n");
}

static int c2k_fast_spi_erase(struct spi_device *spi, void *tx, unsigned len)
{
	return spi_write(spi, tx, len);
}

static int c2k_fast_spi_write(struct spi_device *spi, void *tx, unsigned len)
{
	return spi_write(spi, tx, len);
}

static int c2k_fast_spi_write_read(struct spi_device *spi, void *tx, unsigned n_tx, \
			void *rx, unsigned n_rx)
{
	return spi_write_then_read(spi, tx, n_tx, rx, n_rx);
}

static int c2k_fast_spi_read(struct spi_device *spi, void *rx, unsigned len)
{
	return spi_read(spi, rx, len);
}

static int c2k_fast_spi_eeprom_read(struct spi_device *spi, void *tx, unsigned n_tx, \
			void *rx, unsigned n_rx)
{
        return spi_eeprom_read(spi, tx, n_tx, rx, n_rx);
}

int nor_s25fl128_test(struct diags_test_param *p)
{
	struct device_d *d_fast_spi = get_device_by_name(DEVICE_FAST_SPI);
	struct spi_device *fast_spi_test = d_fast_spi->type_data;
	int ret = SPI_FAILURE;

	/* S25FL128: rd_cmd: |1B_cmd|3B_addr|dummy_bytes| */
	uint8_t s25fl128_rdid_cmd[4] = {
		0x90, 0x00,0x00,0x01
	};

	uint8_t s25fl128_rdsr1_cmd[4] = {
		0x05, 0x99,
	};

	printf("write: ");
	print_buffer(s25fl128_rdid_cmd, 4);
	ret = c2k_fast_spi_eeprom_read(fast_spi_test, s25fl128_rdid_cmd, 4, s25fl128_rdid_cmd, 2);
	if(ret == SPI_FAILURE)
	{
		printf ("NOR_S25FL128_READ failed.\n");
		return SPI_FAILURE;
	}
	mdelay(100);
	printf("read: ");
	print_buffer(s25fl128_rdid_cmd, 2);

	printf ("----------------\n");
	printf("write: ");
	print_buffer(s25fl128_rdsr1_cmd, 1);
	ret = c2k_fast_spi_eeprom_read(fast_spi_test, s25fl128_rdsr1_cmd, 1, s25fl128_rdsr1_cmd, 1);
	if(ret == SPI_FAILURE)
	{
		printf ("s25fl128_rdsr1_cmd failed.\n");
		return SPI_FAILURE;
	}
	mdelay(100);
	printf("read: ");
	print_buffer(s25fl128_rdsr1_cmd, 1);
	printf ("----------------\n");

	printf("write: ");
	print_buffer(s25fl128_wren_cmd, 1);
	ret = c2k_fast_spi_write(fast_spi_test, s25fl128_wren_cmd, 1);
	if(ret == SPI_FAILURE)
	{
		printf ("NOR_S25FL128_WREN failed.\n");
		return SPI_FAILURE;
	}
	mdelay(100);
	printf ("----------------\n");

	s25fl128_rdsr1_cmd[0] = 0x05;
	s25fl128_rdsr1_cmd[1] = 0x88;
	printf("write: ");
	print_buffer(s25fl128_rdsr1_cmd, 1);
	ret = c2k_fast_spi_eeprom_read(fast_spi_test, s25fl128_rdsr1_cmd, 1, s25fl128_rdsr1_cmd, 1);
	if(ret == SPI_FAILURE)
	{
		printf ("s25fl128_rdsr1_cmd failed.\n");
		return SPI_FAILURE;
	}
	mdelay(100);
	printf("read: ");
	print_buffer(s25fl128_rdsr1_cmd, 1);
	printf ("----------------\n");

	return SPI_SUCCESS;
}

int Comcerto_spi_test (struct diags_test_param *p)
{
	u32 ret = SPI_FAILURE;

	printf ("%s: Testing SPI...\n", __func__);

	ret = nor_s25fl128_test(p);

	if(ret == SPI_SUCCESS)
	{
		printf ("NOR_S25FL128_SUCCESS.\n");
	}
	else
	{
		printf ("NOR_S25FL128_FAILURE.\n");
		return SPI_FAILURE;
	}
	return ret;
}
