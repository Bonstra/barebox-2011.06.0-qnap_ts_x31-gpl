/*
 *  c2k_spi_api.c
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

/**
 * spi_write - SPI synchronous write
 * @spi: device to which data will be written
 * @buf: data buffer
 * @len: data buffer size
 *
 * This writes the buffer and returns zero or a negative error code.
 */
int spi_write(struct spi_device *spi, const void *buf, size_t len)
{
        struct spi_transfer     t = {
                        .tx_buf         = buf,
                        .len            = len,
                };
        struct spi_message      m;

        spi_message_init(&m);

        m.status = SPI_TRANSFER_MODE_WRITE_ONLY;

        spi_message_add_tail(&t, &m);
        return spi_sync(spi, &m);
}
EXPORT_SYMBOL(spi_write);

/**
 * spi_read - SPI synchronous read
 * @spi: device from which data will be read
 * @buf: data buffer
 * @len: data buffer size
 *
 * This reads the buffer and returns zero or a negative error code.
 */
int spi_read(struct spi_device *spi, void *buf, size_t len)
{
        struct spi_transfer     t = {
                        .rx_buf         = buf,
                        .len            = len,
                };
        struct spi_message      m;

        spi_message_init(&m);

        m.status = SPI_TRANSFER_MODE_READ_ONLY;

        spi_message_add_tail(&t, &m);
        return spi_sync(spi, &m);
}
EXPORT_SYMBOL(spi_read);

/**
 * spi_write_then_read - SPI synchronous write followed by read
 * @spi: device with which data will be exchanged
 * @txbuf: data to be written
 * @n_tx: size of txbuf, in bytes
 * @rxbuf: buffer into which data will be read
 * @n_rx: size of rxbuf, in bytes
 *
 * This performs a half duplex style transaction with the
 * device, sending txbuf and then reading rxbuf.  The return value
 * is zero for success, else a negative errno status code.
 */
int spi_write_then_read(struct spi_device *spi,
                const void *txbuf, unsigned n_tx,
                void *rxbuf, unsigned n_rx)
{
        struct spi_message      m;
        struct spi_transfer     t = {
                        .tx_buf         = txbuf,
                        .rx_buf         = rxbuf,
                        .len            = n_tx,
                };

        spi_message_init(&m);

        m.status = SPI_TRANSFER_MODE_WRITE_READ;

        spi_message_add_tail(&t, &m);

        /* do the i/o */
        return spi_sync(spi, &m);
}
EXPORT_SYMBOL(spi_write_then_read);

#ifdef CONFIG_DRIVER_FAST_SPI_C2K
int spi_eeprom_read(struct spi_device *spi,
                const void *txbuf, unsigned n_tx,
                void *rxbuf, unsigned n_rx)
{
        struct spi_message      m;
        struct spi_transfer     t = {
                        .tx_buf         = txbuf,
                        .rx_buf         = rxbuf,
                        .len            = n_rx,
                };

        spi_message_init(&m);

        m.status = SPI_TRANSFER_MODE_EEPROM_READ;

        spi_message_add_tail(&t, &m);

        /* do the i/o */
        return spi_sync(spi, &m);
}
EXPORT_SYMBOL(spi_eeprom_read);
#endif
