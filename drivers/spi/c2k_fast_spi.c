/*
 *  c2k_fast_spi.c
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
#include <linux/byteorder/generic.h>
#include <linux/byteorder/little_endian.h>

#include <spi/fast_spi.h>
#include <mach/dma.h>
#include <common.h>
#include <asm/io.h>
#include <init.h>
#include <mach/comcerto_spi.h>

#include "c2k_dma.h"

#define	CONFIG_DRIVER_FAST_SPI_DMA

#define SPI_FRAME_SIZE_MAX      16
#define SPI_FRAME_SIZE_MIN      4
#define SPI_CHIP_SELECT_MAX     15
#define SPI_BAUDR_MIN           2
#define SPI_BAUDR_MAX           0xFFFE
#define SPI_SPEED_MAX           (4*1000*1000)

#define SPI_CTRLR0_SCPOL	(1 << 7)
#define SPI_CTRLR0_SCPH		(1 << 6)

#define COMCERTO_DEFAULTAXICLK          200000000 /* Hz */

//#define COMCERTO_SPI_DEBUG
#ifdef COMCERTO_SPI_DEBUG
        #define spi_debug(fmt, arg...)   printf(fmt, ##arg)
#else
        #define spi_debug(fmt, arg...)     ;
#endif

static int spi_udelay;

extern void dma_conf(U32 ch_no, U32 dst, U32 data_len, U32 src, struct dma_conf_param *dmaconf);
extern void dma_start(U32 chan);
extern retcode dma_ssi_xfer_cmplete_chk(U32 ch_no);
extern int do_eeprom_read(struct spi_adapter *, u8, u8 *, int *, u8 *, int *);
extern int do_write_read_transfer(struct spi_adapter *, u8, u8 *, int *, u8 *rbuf, int *);
extern int do_write_only_transfer8(struct spi_adapter *, u8 *, int *);
extern int do_write_only_transfer16(struct spi_adapter *, u16 *, int *);
extern int do_read_only_transfer8(struct spi_adapter *, u8 *, int *);
extern int do_read_only_transfer16(struct spi_adapter *, u16 *, int *);
void dma_out_of_reset(void);

//extern u32 HAL_get_axi_clk();

#ifdef	CONFIG_DRIVER_FAST_SPI_DMA
static int read_dma_complete(U32 chan, U32 dst, U32 data_len, U32 src, struct dma_conf_param *dmaconf)
{
	/* Program the DMA */
	dma_conf(chan, (U32) dst, data_len, src, dmaconf);

	/* verify we are not busy, done with last transaction */
	while (readl(DW_DMA_SSI_SR) & BUSY) ;

	/* Send 1 byte dummy data to start read*/
	writel(0, DW_DMA_SSI_DR);

	/* NOW start DMA */
	dma_start(chan);

	/* Wait for completion of DMA xfer (till channel is disabled by HW) */
	if (RETCODE_OK != dma_ssi_xfer_cmplete_chk(chan))
		return -1;

	return 0;

}

static int write_dma_complete(U32 chan, U32 dst, U32 data_len, U32 src, struct dma_conf_param *dmaconf)
{
	/* Program the DMA */
	dma_conf(chan, dst, data_len, (U32) src, dmaconf);

	/* verify we are not busy, done with last transaction */
	while (readl(DW_DMA_SSI_SR) & BUSY) ;

	/* NOW start DMA */
	dma_start(chan);

	/* Wait for completion of DMA xfer (till channel is disabled by HW) */
	if (RETCODE_OK != dma_ssi_xfer_cmplete_chk(chan))
		return -1;

	return 0;

}

static int eeprom_writeread_dma_complete(U32 wr_ch, U32 rd_ch, U32 rbuf, U32 wbuf,
			U32 data_len, U32 dr, struct dma_conf_param *wr_dmaconf, struct dma_conf_param *rd_dmaconf)
{
	/* Program dma channel for write */
	dma_conf(wr_ch, dr, data_len, wbuf, wr_dmaconf);

	/* Program dma channel for read */
	dma_conf(rd_ch, rbuf, data_len, dr, rd_dmaconf);

	/* NOW start write DMA */
	dma_start(wr_ch);

	/* NOW start read DMA */
	dma_start(rd_ch);

	/* Wait for completion of DMA write (till channel is disabled by HW) */
	if (RETCODE_OK != dma_ssi_xfer_cmplete_chk(wr_ch))
		return -1;

	/* Wait for completion of DMA read (till channel is disabled by HW) */
	if (RETCODE_OK != dma_ssi_xfer_cmplete_chk(rd_ch))
		return -1;

	return 0;
}

static int eeprom_read_dma_complete(U32 chan, U32 dst, U32 data_len, U8 *wbuf, U32 src, struct dma_conf_param *dmaconf)
{
	/* Program the DMA */
	dma_conf(chan, (U32) dst, data_len, src, dmaconf);

	/* verify we are not busy, done with last transaction */
	while (readl(DW_DMA_SSI_SR) & BUSY) ;

        /*                                             ______________________________
	 * Write wbuf data which is in special format: |8b op|8b uppr adr|8b low adr |
	 *                                             -------------------------------
	 * Need to check with specs whether these three bytes could be more or less
	 * If yes then wlen will be considered to write bytes to DR
	 */
        writel(*wbuf, DW_DMA_SSI_DR); //8 bit opcode
        writel(*(wbuf+1), DW_DMA_SSI_DR); //8 bit upper address
        writel(*(wbuf+2), DW_DMA_SSI_DR); //8 bit lower address
        writel(*(wbuf+3), DW_DMA_SSI_DR); //8 bit lower address

	/* NOW start DMA */
	dma_start(chan);

	/* Wait for completion of DMA xfer (till channel is disabled by HW) */
	if (RETCODE_OK != dma_ssi_xfer_cmplete_chk(chan))
		return -1 ;

	/* disable slave to be safe in sharing the bus with other controllers */
	writel(0, DW_DMA_SSI_SER);

	return 0;

}

/**
 * fast_spi_write_mem -
 *
 *
 */
static int fast_spi_write_mem(struct spi_device *spi, u8 *wbuf, int wlen)
{
	struct spi_adapter *adapter = container_of(spi->master, struct spi_adapter, master);
	int rc = 0;
	U32 dma_len ;
	U32 dr = adapter->membase + COMCERTO_SPI_DR;
	struct dma_conf_param dmaconf;

	dmaconf.dir = 0x1; /* Memory to peripheral with DW_ahb_dmac as flow controller */
	dmaconf.hs_src = 0x1; /* S/W handshaking */ 
	dmaconf.hs_dst = 0x0; /* H/W handshaking */
	dmaconf.sinc = 0x0; /* Source Address Inc */
	dmaconf.dinc = 0x2; /* No Destination Address Inc */

	/*  Enable the DMA TX FIFO and RX FIFO channel */
	writel(((SSI_TDMAE << 1) | (SSI_RDMAE << 0)), adapter->membase + COMCERTO_SPI_DMACR);
	/* DMA TX FIFO threshold level */
	writel(SSI_DMATDL, adapter->membase + COMCERTO_SPI_DMATDLR);
	/* DMA RX FIFO threshold level */
	writel(SSI_DMARDL, adapter->membase + COMCERTO_SPI_DMARDLR);

	while ( wlen ) {

		if ( wlen > SPI_NDF )
			dma_len = SPI_NDF;
		else
			dma_len = wlen;

		/* Program SSI data Xfer length */
		writel(dma_len-1, adapter->membase+COMCERTO_SPI_CTRLR1);

		/* done, programming regs, enable the ssi */
		writel(1, adapter->membase+COMCERTO_SPI_SSIENR);

		/* write it in single DMA Req -- Single-block Transfer */
		rc = write_dma_complete(DMA_CHANNEL_5, dr, dma_len, (U32) wbuf, &dmaconf);

		wlen -= dma_len;

		wbuf += dma_len;
	}

	/* deassert the chip select at least for this long */
	udelay (spi_udelay);

	return rc;
}

/**
 * fast_spi_read_mem -
 *
 *
 */
static int fast_spi_read_mem(struct spi_device *spi, u8 *rbuf, int rlen)
{
	struct spi_adapter *adapter = container_of(spi->master, struct spi_adapter, master);
	int rc = 0;
	U32 dma_len ;
	U32 dr = adapter->membase + COMCERTO_SPI_DR;
	struct dma_conf_param dmaconf;

	dmaconf.dir = 0x2; /* Peripheral to memory with DW_ahb_dmac as flow controller */
	dmaconf.hs_src = 0x0; /* H/W handshaking */ 
	dmaconf.hs_dst = 0x1; /* S/W handshaking */
	dmaconf.sinc = 0x2; /* No Source Address Inc */
	dmaconf.dinc = 0x0; /* Destination Address Inc */


	/*  Enable the DMA TX FIFO and RX FIFO channel */
	writel(((SSI_TDMAE << 1) | (SSI_RDMAE << 0)), adapter->membase + COMCERTO_SPI_DMACR);
	/* DMA TX FIFO threshold level */
	writel(SSI_DMATDL, adapter->membase + COMCERTO_SPI_DMATDLR);	
	/* DMA RX FIFO threshold level */
	writel(SSI_DMARDL, adapter->membase + COMCERTO_SPI_DMARDLR);	

	while ( rlen ) {

		if ( rlen > SPI_NDF )
			dma_len = SPI_NDF;
		else
			dma_len = rlen;

		/* Program SSI data Xfer length */
		writel(dma_len-1, adapter->membase+COMCERTO_SPI_CTRLR1);

		/* done, programming regs, enable the ssi */
		writel(1, adapter->membase+COMCERTO_SPI_SSIENR);

		/* read it in single DMA Req -- Single-block Transfer */
		rc = read_dma_complete(DMA_CHANNEL_4, (U32) rbuf, dma_len, dr, &dmaconf);

		rlen -= dma_len;

		rbuf += dma_len;
	}

	/* deassert the chip select at least for this long */
	udelay (spi_udelay);

	return rc;
}

/**
 * fast_spi_writeread_mem -
 *
 *
 */
static int fast_spi_writeread_mem(struct spi_device *spi, u8 *rbuf, int rlen, u8 *wbuf, int wlen)
{
	struct spi_adapter *adapter = container_of(spi->master, struct spi_adapter, master);
	int rc = 0;
	U32 dma_len ;
	U32 dr = adapter->membase + COMCERTO_SPI_DR;
	struct dma_conf_param wr_dmaconf;
	struct dma_conf_param rd_dmaconf;

	wr_dmaconf.dir = 0x1; /* Memory to peripheral with DW_ahb_dmac as flow controller */
	wr_dmaconf.hs_src = 0x1; /* H/W handshaking */ 
	wr_dmaconf.hs_dst = 0x0; /* S/W handshaking */
	wr_dmaconf.sinc = 0x0; /* Source Address Inc */
	wr_dmaconf.dinc = 0x2; /* No Destination Address Inc */

	rd_dmaconf.dir = 0x2; /* Peripheral to memory with DW_ahb_dmac as flow controller */
	rd_dmaconf.hs_src = 0x0; /* H/W handshaking */ 
	rd_dmaconf.hs_dst = 0x1; /* S/W handshaking */
	rd_dmaconf.sinc = 0x2; /* No Source Address Inc */
	rd_dmaconf.dinc = 0x0; /* Destination Address Inc */

	/*  Enable the DMA TX FIFO and RX FIFO channel */
	writel(((SSI_TDMAE << 1) | (SSI_RDMAE << 0)), adapter->membase + COMCERTO_SPI_DMACR);
	/* DMA TX FIFO threshold level */
	writel(SSI_DMATDL, adapter->membase + COMCERTO_SPI_DMATDLR);	
	/* DMA RX FIFO threshold level */
	writel(SSI_DMARDL, adapter->membase + COMCERTO_SPI_DMARDLR);	

	/* Looping only for number of bytes to be written because same number
	 * Number of bytes are read every time. We can not have unequal wlen
	 * and rlen because DW_ahb_ssi has one register ctrlr1 for the length
	 * to be transferred. If we write first and then read, there will be 
	 * read data overwritten */
	while ( wlen ) {

		if ( wlen > SPI_NDF )
			dma_len = SPI_NDF;
		else
			dma_len = wlen;

		/* Program SSI data Xfer length */
		writel(dma_len-1, adapter->membase+COMCERTO_SPI_CTRLR1);

		/* done, programming regs, enable the ssi */
		writel(1, adapter->membase+COMCERTO_SPI_SSIENR);

		/* write it in single DMA Req -- Single-block Transfer */
		rc = eeprom_writeread_dma_complete(DMA_CHANNEL_4, DMA_CHANNEL_5, (U32) rbuf, (U32)wbuf, \
				dma_len, dr, &wr_dmaconf, &rd_dmaconf) ;

		wlen -= dma_len;

		wbuf += dma_len;

		rbuf += dma_len;
	}

	/* deassert the chip select at least for this long */
	udelay (spi_udelay);

	return rc;
}

/**
 * spi_eeprom_read
 *
 *
 */
static int fast_spi_eeprom_read(struct spi_device *spi, u8 *rbuf, int rlen, u8 *wbuf, int wlen)
{
	struct spi_adapter *adapter = container_of(spi->master, struct spi_adapter, master);
	int rc = 0;
	U32 dr = adapter->membase + COMCERTO_SPI_DR;
	struct dma_conf_param dmaconf;

	/* These all DMA config params can go in a struct*/
	dmaconf.dir = 0x2; /* Peripheral to memory with DW_ahb_dmac as flow controller */
	dmaconf.hs_src = 0x0; /* H/W handshaking */ 
	dmaconf.hs_dst = 0x1; /* S/W handshaking */
	dmaconf.sinc = 0x2; /* No Source Address Inc */
	dmaconf.dinc = 0x0; /* Destination Address Inc */

	/*  Enable the DMA TX FIFO and RX FIFO channel */
	writel(((SSI_TDMAE << 1) | (SSI_RDMAE << 0)), adapter->membase + COMCERTO_SPI_DMACR);
	/* DMA TX FIFO threshold level */
	writel(SSI_DMATDL, adapter->membase + COMCERTO_SPI_DMATDLR);	
	/* DMA RX FIFO threshold level */
	writel(SSI_DMARDL, adapter->membase + COMCERTO_SPI_DMARDLR);	

	/* done, programming regs, enable the ssi */
	writel(1, adapter->membase+COMCERTO_SPI_SSIENR);

	/* read it in single DMA Req -- Single-block Transfer */
	rc = eeprom_read_dma_complete(DMA_CHANNEL_4, (U32) rbuf, rlen, wbuf, dr, &dmaconf) ;

	/* deassert the chip select at least for this long */
	udelay (spi_udelay);

	return rc;
}
#endif
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

static int c2k_fast_spi_setup(struct spi_device *spi)
{
	u32 hz;

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

        spi_debug("%s: bits_per_word: %d max_speed_hz: %d\n",
                        __func__, spi->bits_per_word, spi->max_speed_hz);


        return 0;
}

static int c2k_fast_spi_transfer(struct spi_device *spi, struct spi_message *mesg)
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

	spi_debug ("%s: ser = 0x%x baudr = 0x%x ctrlr0 = 0x%x\n",\
			__func__, ser, baudr, ctrlr0);

	list_for_each_entry (t, &mesg->transfers, transfer_list) {
		u8 *txbuf = (u8*)t->tx_buf;
		u8 *rxbuf = (u8*)t->rx_buf;

		switch(op) {
			case SPI_TRANSFER_MODE_WRITE_ONLY:
                                ctrlr0 |= (0x0001 << 8);
                                writel(ctrlr0, adapter->membase + COMCERTO_SPI_CTRLR0);
                                writel(baudr, adapter->membase + COMCERTO_SPI_BAUDR);
                                writel(ser, adapter->membase + COMCERTO_SPI_SER);
		#ifndef CONFIG_DRIVER_FAST_SPI_DMA
                                writel(8, adapter->membase + COMCERTO_SPI_RXFTLR);
                                writel(0, adapter->membase + COMCERTO_SPI_TXFTLR);
                                writel(0, adapter->membase + COMCERTO_SPI_IMR);
                                writel(1, adapter->membase + COMCERTO_SPI_SSIENR);

				if (spi->bits_per_word <= 8)
					ret = do_write_only_transfer8(adapter, txbuf, &t->len);
				else
					ret = do_write_only_transfer16(adapter, (u16*)txbuf, &t->len);
		#else
				ret = fast_spi_write_mem(spi, txbuf, t->len);
		#endif

				break;
			case SPI_TRANSFER_MODE_READ_ONLY:
                                ctrlr0 |= (0x0002 << 8);
                                writel(ctrlr0, adapter->membase + COMCERTO_SPI_CTRLR0);
                                writel(baudr, adapter->membase + COMCERTO_SPI_BAUDR);
                                writel(ser, adapter->membase + COMCERTO_SPI_SER);
		#ifndef CONFIG_DRIVER_FAST_SPI_DMA
                                writel(8, adapter->membase + COMCERTO_SPI_RXFTLR);
                                writel(0, adapter->membase + COMCERTO_SPI_TXFTLR);
                                writel(0, adapter->membase + COMCERTO_SPI_IMR);
                                writel(1, adapter->membase + COMCERTO_SPI_SSIENR);

				if (spi->bits_per_word <= 8)
					ret = do_read_only_transfer8(adapter, rxbuf, &t->len);
				else
					ret = do_read_only_transfer16(adapter, (u16 *)rxbuf, &t->len);
		#else
				ret = fast_spi_read_mem(spi, rxbuf, t->len);
		#endif

				break;

			case SPI_TRANSFER_MODE_WRITE_READ:
                                ctrlr0 |= (0x0000 << 8);
                                writel(ctrlr0, adapter->membase + COMCERTO_SPI_CTRLR0);
                                writel(baudr, adapter->membase + COMCERTO_SPI_BAUDR);
                                writel(ser, adapter->membase + COMCERTO_SPI_SER);
		#ifndef CONFIG_DRIVER_FAST_SPI_DMA
                                writel(8, adapter->membase + COMCERTO_SPI_RXFTLR);
                                writel(0, adapter->membase + COMCERTO_SPI_TXFTLR);
                                writel(0, adapter->membase + COMCERTO_SPI_IMR);
                                writel(1, adapter->membase + COMCERTO_SPI_SSIENR);

				int len = t->len;
				int max = 8;
				while(len > 0)
				{
					if(len < max)
						max = len;

					ret = do_write_read_transfer(adapter, spi->bits_per_word,\
							txbuf, &max, rxbuf, &max);
					len -= max;
					txbuf += max;
					rxbuf += max;
				}
		#else
				ret = fast_spi_writeread_mem(spi, rxbuf, t->len, txbuf, t->len);
		#endif

				break;

			case SPI_TRANSFER_MODE_EEPROM_READ:
                                ctrlr0 |= (0x0003 << 8);
                                writel(ctrlr0, adapter->membase + COMCERTO_SPI_CTRLR0);
                                writel(baudr, adapter->membase + COMCERTO_SPI_BAUDR);
                                writel(ser, adapter->membase + COMCERTO_SPI_SER);
				writel(t->len-1, adapter->membase + COMCERTO_SPI_CTRLR1); //ndf
		#ifndef CONFIG_DRIVER_FAST_SPI_DMA
				writel(8, adapter->membase + COMCERTO_SPI_RXFTLR);
				writel(0, adapter->membase + COMCERTO_SPI_TXFTLR);
				writel(0, adapter->membase + COMCERTO_SPI_IMR);
				writel(1, adapter->membase + COMCERTO_SPI_SSIENR);

				ret = do_eeprom_read(adapter, spi->bits_per_word, txbuf, &t->len, rxbuf, &t->len);
		#else
				ret = fast_spi_eeprom_read(spi, rxbuf, t->len, txbuf, t->len);
		#endif
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

void dma_out_of_reset()
{
	U32 val;

        val = readl(AXI_RESET_1);
        writel((val & ~DUS_AXI_RESET), AXI_RESET_1);

        /* 6 uS delay for reset to happen */
        udelay(6);
}

static int c2k_fast_spi_probe(struct device_d *dev)
{
	struct spi_master *master;
	struct spi_adapter *adapter;
	struct c2k_spi_master *pdata = dev->platform_data;

	spi_debug("%s: initializing....\n",__func__);

	adapter = (struct spi_adapter *) xzalloc(sizeof(struct spi_adapter));

	master = &adapter->master;

        master->dev = dev;

        master->setup = c2k_fast_spi_setup;
        master->transfer = c2k_fast_spi_transfer;
        master->bus_num = 1;
	master->num_chipselect = pdata->num_chipselect;

	adapter->membase = dev->map_base;

	comcerto_spi_hw_init(adapter);

	dma_out_of_reset();

	spi_register_master(master);

	return 0;
}

/**
 * comcerto_spi_remove -
 *
 *
 */
#if 0
static int comcerto_fast_spi_remove(struct spi_adapter *spi)
{
	int ret = 0;

	ret = spi_unregister_master(spi->master);

	comcerto_spi_hw_reset(spi);

	return ret;
}
#endif

/* barebox framework */
static struct driver_d c2k_fast_spi_driver = {
        .name  = "c2k_fast_spi",
        .probe = c2k_fast_spi_probe,
};

int c2k_fast_spi_init(void)
{
	int ret = 0;

        ret = register_driver(&c2k_fast_spi_driver);

        return ret;
}

device_initcall(c2k_fast_spi_init);
/***/
