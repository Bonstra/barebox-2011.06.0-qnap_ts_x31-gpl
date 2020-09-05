/*
 *  c2k_spi_common.c
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
#include <mach/comcerto_spi.h>

/**
 * do_write_read_transfer -
 *
 *
 */
int do_write_read_transfer(struct spi_adapter *spi, u8 fs, u8 *wbuf, U32 *wlen, u8 *rbuf, U32 *rlen)
{
	u32 sr, dr;
	U32 wlen_now = 0, rlen_now = 0;
	int rc = 0;

	while (wlen_now < *wlen) {
		sr = readl(spi->membase + COMCERTO_SPI_SR);

		if (sr & TFNF) {
			if (wlen_now < *wlen) {
				writew(cpu_to_le16((u16) *wbuf), spi->membase + COMCERTO_SPI_DR);
				wbuf++;
				wlen_now++;
			}
		}
	}

	while (rlen_now < *rlen) {
		sr = readl(spi->membase + COMCERTO_SPI_SR);

		if (sr & (RFF | DCOL)) {
			/* read overrun, data collision */
			printf ("%s: sr=0x%x: Read Overrun.\n", __func__, sr);
			rc = -1;
			goto out;
		}

		if (sr & RFNE) {
			dr = readw(spi->membase + COMCERTO_SPI_DR);
			if (rlen_now < *rlen) {
				*rbuf = (u8) (le16_to_cpu(dr) & 0xff);
				rbuf++;
				rlen_now++;
			} else {
				printf ("%s: Read Overflow.\n", __func__);
				/* read overflow */

				rc = -1;
				goto out;
			}
		}
	}

      out:
	*rlen = rlen_now;
	*wlen = wlen_now;

	return rc;
}

int do_eeprom_read(struct spi_adapter *spi, u8 fs, u8 *wbuf, U32 *wlen, u8 *rbuf, U32 *ndf)
{
	u32 sr, dr;
	U32 wlen_now = 0, rlen_now = 0;
	int rc = 0;

	while (wlen_now < *wlen) {
		sr = __raw_readl(spi->membase + COMCERTO_SPI_SR);

		if (sr & TFNF) {
			if (wlen_now < *wlen) {
				__raw_writew(cpu_to_le16((u16) *wbuf), spi->membase + COMCERTO_SPI_DR);
				wbuf++;
				wlen_now++;
			}
		}
	}

	while (rlen_now < *ndf) {
		sr = __raw_readl(spi->membase + COMCERTO_SPI_SR);

		if (sr & (RFF | DCOL)) {
			/* read overrun, data collision */
			rc = -1;
			goto out;
		}

		if (sr & RFNE) {
			dr = __raw_readw(spi->membase + COMCERTO_SPI_DR);
			if (rlen_now < *ndf) {
				*rbuf = (u8) (le16_to_cpu(dr) & 0xff);
				rbuf++;
				rlen_now++;
			} else {
				/* read overflow */
				rc = -1;
				goto out;
			}
		}
	}

out:
	*ndf = rlen_now;
	*wlen = wlen_now;

	return rc;
}

/**
 * do_write_only_transfer8 -
 *
 *
 */
int do_write_only_transfer8(struct spi_adapter *spi, u8 *buf, U32 *len)
{
	U32 len_now;
	int rc = 0;
	U32 tmp = *len;
	u32 dr = spi->membase + COMCERTO_SPI_DR;
	u32 txflr = spi->membase + COMCERTO_SPI_TXFLR;

	while (tmp)
	{
		len_now = 8 - readl(txflr);
		if (len_now > tmp)
			len_now = tmp;

		tmp -= len_now;

		/* warm-up write fifo to avoid underruns */
		while (len_now--)
		{
			writew(cpu_to_le16((u16) *buf++), dr);
		}
	}

	*len -= tmp;

	return rc;
}

/**
 * do_write_only_transfer -
 *
 *
 */
int do_write_only_transfer16(struct spi_adapter *spi, u16 *buf, U32 *len)
{
	U32 len_now;
	int rc = 0;
	U32 tmp = *len;
	u32 dr = spi->membase + COMCERTO_SPI_DR;
	u32 txflr = spi->membase + COMCERTO_SPI_TXFLR;

	while (tmp)
	{
		len_now = 8 - readl(txflr);
		if (len_now > tmp)
			len_now = tmp;

		tmp -= len_now;

		/* warm-up write fifo to avoid underruns */
		while (len_now--)
			writew(cpu_to_le16(*buf++), dr);
	}

	*len -= tmp;

	return rc;
}


/**
 * do_read_only_transfer -
 *
 *
 */
int do_read_only_transfer8(struct spi_adapter *spi, u8 *buf, U32 *len)
{
	U32 len_now;
	int rc = 0;
	U32 tmp = *len;
	u32 dr = spi->membase + COMCERTO_SPI_DR;
	u32 rxflr = spi->membase + COMCERTO_SPI_RXFLR;

	/* start the serial clock */
	writew(0, dr);

	while (tmp)
	{
		len_now = readl(rxflr);
		if (len_now > tmp)
			len_now = tmp;

		tmp -= len_now;

		while (len_now--) {
			*buf = (u8) (le16_to_cpu(readw(dr)) & 0xff);
			buf++;
		}
	}

	*len -= tmp;

	return rc;
}

/**
 * do_read_only_transfer -
 *
 *
 */
int do_read_only_transfer16(struct spi_adapter *spi, u16 *buf, U32 *len)
{
	U32 len_now;
	int rc = 0;
	U32 tmp = *len;
	u32 dr = spi->membase + COMCERTO_SPI_DR;
	u32 rxflr = spi->membase + COMCERTO_SPI_RXFLR;

	/* start the serial clock */
	writew(0, dr);

	while (tmp)
	{
		len_now = readl(rxflr);
		if (len_now > tmp)
			len_now = tmp;

		tmp -= len_now;

		while (len_now--) {
			*buf = le16_to_cpu(readw(dr));
			buf++;
		}
	}

	*len -= tmp;

	return rc;
}

/***/

