/*
 *  c2k_spi.c
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

#include <common.h>
#include <init.h>
#include <linux/byteorder/generic.h>
#include <linux/byteorder/little_endian.h>

#include <spi/spi.h>
#include <common.h>
#include <asm/io.h>
#include <mach/comcerto_spi.h>

#define SPI_FRAME_SIZE_MAX      16
#define SPI_FRAME_SIZE_MIN      4
#define SPI_CHIP_SELECT_MAX     15
#define SPI_BAUDR_MIN           2
#define SPI_BAUDR_MAX           0xFFFE
#define SPI_SPEED_MAX           (4*1000*1000)

#define SPI_CTRLR0_SCPOL	(1 << 7)
#define SPI_CTRLR0_SCPH		(1 << 6)

#define COMCERTO_DEFAULTAXICLK          200000000 /* Hz */

#ifdef COMCERTO_SPI_DEBUG
        #define spi_debug(fmt, arg...)   printf(fmt, ##arg)
#else
        #define spi_debug(fmt, arg...)     ;
#endif

static int spi_udelay;

extern int do_write_read_transfer(struct spi_adapter *, u8, u8 *, int *, u8 *rbuf, int *);
extern int do_write_only_transfer8(struct spi_adapter *, u8 *, int *);
extern int do_write_only_transfer16(struct spi_adapter *, u16 *, int *);
extern int do_read_only_transfer8(struct spi_adapter *, u8 *, int *);
extern int do_read_only_transfer16(struct spi_adapter *, u16 *, int *);

extern u32 HAL_get_axi_clk(void);
extern int spi_register_master(struct spi_master *master);

/**
 * comcerto_spi_hw_init -
 *
 *
 */
static void comcerto_spi_hw_init(struct spi_adapter *adaptr)
{
	/* disable SPI operation */
	writel(0, adaptr->membase + COMCERTO_SPI_SSIENR);

	/* mask all SPI irq's */
	writel(0, adaptr->membase + COMCERTO_SPI_IMR);
}

/**
 * comcerto_spi_hw_reset -
 *
 *
 */
#if 0
static void comcerto_spi_hw_reset(struct spi_adapter *adaptr)
{
	/* disable SPI operation */
	writel(0, adaptr->membase + COMCERTO_SPI_SSIENR);

	/* mask all SPI irq's */
	writel(0, adaptr->membase + COMCERTO_SPI_IMR);
}
#endif

static int c2k_spi_setup(struct spi_device *spi)
{
	u32 hz;

        spi_debug("%s mode 0x%08x bits_per_word: %d speed: %d\n",
                        __FUNCTION__, spi->mode, spi->bits_per_word,
                        spi->max_speed_hz);

	hz = COMCERTO_DEFAULTAXICLK; //HAL_get_axi_clk();

	if (!spi->bits_per_word)
		spi->bits_per_word = 8;

	if(spi->bits_per_word < SPI_FRAME_SIZE_MIN || spi->bits_per_word > SPI_FRAME_SIZE_MAX)
		return -1;

	if (spi->max_speed_hz < (hz/SPI_BAUDR_MAX))
		return -1;

	if (spi->max_speed_hz > SPI_SPEED_MAX) {
                printf("decreasing speed %u to max supported %u\n",
                        spi->max_speed_hz, SPI_SPEED_MAX);
                spi->max_speed_hz = SPI_SPEED_MAX;
        }

	if (spi->chip_select > SPI_CHIP_SELECT_MAX) {
                printf("chip select %u out of range 0..%u\n",
                        spi->max_speed_hz, SPI_CHIP_SELECT_MAX);
                return -1;
        }

	spi_udelay = 1 + ((1000000 * spi->bits_per_word) / spi->max_speed_hz);

        return 0;
}

static int c2k_spi_transfer(struct spi_device *spi, struct spi_message *mesg)
{
	struct spi_transfer     *t = NULL;
	unsigned int	op = mesg->status & 0xff; /* what operation */
	int ret = -1;
        struct spi_adapter *adapter = container_of(spi->master, struct spi_adapter, master);
        u32 ctrlr0 = 0;
        u32 baudr  = 0;
        u32 ser    = 0;
	u32 hz;

        if (spi->mode & SPI_CPOL)
                ctrlr0 |= SPI_CTRLR0_SCPOL;

        if (spi->mode & SPI_CPHA)
                ctrlr0 |= SPI_CTRLR0_SCPH;

        ctrlr0 |= (spi->bits_per_word - 1) & 0xf;

        hz = COMCERTO_DEFAULTAXICLK; //Later on will use HAL_get_axi_clk() to get the current AXI freq

        if (spi->max_speed_hz > 0) {
                baudr = hz / spi->max_speed_hz;

                if (baudr < SPI_BAUDR_MIN)
                        baudr = SPI_BAUDR_MIN;
                if (baudr > SPI_BAUDR_MAX)
                        baudr = SPI_BAUDR_MAX;
        }
        else
	{
		baudr = 0;
		printf("%s: Baud rate not set!!\n", __func__);
	}

	ser = (1 << spi->chip_select) ;

	while(readl(adapter->membase + COMCERTO_SPI_SR) & BUSY);

	writel(0, adapter->membase + COMCERTO_SPI_SSIENR);

	list_for_each_entry (t, &mesg->transfers, transfer_list) {
		u8 *txbuf = (u8 *)t->tx_buf;
		u8 *rxbuf = t->rx_buf;

		switch(op) {
			case SPI_TRANSFER_MODE_WRITE_ONLY:

                                ctrlr0 |= (0x0001 << 8);
                                writel(ctrlr0, adapter->membase + COMCERTO_SPI_CTRLR0);
                                writel(baudr, adapter->membase + COMCERTO_SPI_BAUDR);
                                writel(ser, adapter->membase + COMCERTO_SPI_SER);
                                writel(8, adapter->membase + COMCERTO_SPI_RXFTLR);
                                writel(0, adapter->membase + COMCERTO_SPI_TXFTLR);
                                writel(0, adapter->membase + COMCERTO_SPI_IMR);
                                writel(1, adapter->membase + COMCERTO_SPI_SSIENR);

				if (spi->bits_per_word <= 8)
					ret = do_write_only_transfer8(adapter, txbuf, &t->len);
				else
					ret = do_write_only_transfer16(adapter, (u16*)txbuf, &t->len);

				break;

			case SPI_TRANSFER_MODE_READ_ONLY:

                                ctrlr0 |= (0x0002 << 8);
                                writel(ctrlr0, adapter->membase + COMCERTO_SPI_CTRLR0);
                                writel(baudr, adapter->membase + COMCERTO_SPI_BAUDR);
                                writel(ser, adapter->membase + COMCERTO_SPI_SER);
                                writel(8, adapter->membase + COMCERTO_SPI_RXFTLR);
                                writel(0, adapter->membase + COMCERTO_SPI_TXFTLR);
                                writel(0, adapter->membase + COMCERTO_SPI_IMR);
                                writel(1, adapter->membase + COMCERTO_SPI_SSIENR);

				if (spi->bits_per_word <= 8)
					ret = do_read_only_transfer8(adapter, rxbuf, &t->len);
				else
					ret = do_read_only_transfer16(adapter, (u16 *)rxbuf, &t->len);

				break;

			case SPI_TRANSFER_MODE_WRITE_READ:

                                ctrlr0 |= (0x0000 << 8);
                                writel(ctrlr0, adapter->membase + COMCERTO_SPI_CTRLR0);
                                writel(baudr, adapter->membase + COMCERTO_SPI_BAUDR);
                                writel(ser, adapter->membase + COMCERTO_SPI_SER);
                                writel(8, adapter->membase + COMCERTO_SPI_RXFTLR);
                                writel(0, adapter->membase + COMCERTO_SPI_TXFTLR);
                                writel(0, adapter->membase + COMCERTO_SPI_IMR);
                                writel(1, adapter->membase + COMCERTO_SPI_SSIENR);

				ret = do_write_read_transfer(adapter, spi->bits_per_word, txbuf, &t->len, rxbuf, &t->len);

				break;

			default:
				printk ("Transfer mode not supported.\n");
				return ret;
		};

		/* deassert the chip select at least for this long */
		udelay (spi_udelay);
		while(readl(adapter->membase + COMCERTO_SPI_SR) & BUSY);
	}

	return 0;
}

static int c2k_spi_probe(struct device_d *dev)
{
	struct spi_master *master;
	struct spi_adapter *adapter;
	struct c2k_spi_master *pdata = dev->platform_data;

	printf("%s\n",__func__);

	adapter = (struct spi_adapter *) xzalloc(sizeof(struct spi_adapter));

	master = &adapter->master;

        master->dev = dev;

        master->setup = c2k_spi_setup;
        master->transfer = c2k_spi_transfer;
        master->bus_num = 0;
	master->num_chipselect = pdata->num_chipselect;

	adapter->membase = dev->map_base;

	comcerto_spi_hw_init(adapter);

	spi_register_master(master);

	return 0;
}

/**
 * comcerto_spi_remove -
 *
 *
 */
#if 0
static int comcerto_spi_remove(struct spi_adapter *spi)
{
	spi_unregister_master(spi->master);

	comcerto_spi_hw_reset(spi);

	return 0;
}
#endif

/* barebox framework */
static struct driver_d c2k_spi_driver = {
        .name  = "c2k_spi",
        .probe = c2k_spi_probe,
};

int c2k_spi_init(void)
{
        register_driver(&c2k_spi_driver);
        return 0;
}

device_initcall(c2k_spi_init);
/***/

