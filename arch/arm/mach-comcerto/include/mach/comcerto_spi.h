/*
 *  linux/drivers/spi/busses/comcerto_spi.h
 *
 *  Copyright (C) Mindspeed Technologies
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _COMCERTO_SPI_H
#define _COMCERTO_SPI_H

#include <linux/list.h>
#include <spi/spi.h>
#include <mach/comcerto-2000.h>
#include "types.h"
#include <mach/clkcore.h>

#define SPI_BASEADDR		COMCERTO_SPI_BASE

#define	FAST_SPI_OFFSET		0x500000
#define FAST_SPI_BASEADDR	COMCERTO_AXI_UART_SPI_CFG_BASE + FAST_SPI_OFFSET

struct c2k_spi_master {
        int     *chipselect;
        int     num_chipselect;
};

#define COMCERTO_SPI_DRIVER_NAME	"Comcerto SPI"


#define COMCERTO_SPI_CTRLR0               0x00
#define COMCERTO_SPI_CTRLR1               0x04
#define COMCERTO_SPI_SSIENR               0x08
#define COMCERTO_SPI_MWCR                 0x0C
#define COMCERTO_SPI_SER                  0x10
#define COMCERTO_SPI_BAUDR                0x14
#define COMCERTO_SPI_TXFTLR               0x18
#define COMCERTO_SPI_RXFTLR               0x1C
#define COMCERTO_SPI_TXFLR                0x20
#define COMCERTO_SPI_RXFLR                0x24
#define COMCERTO_SPI_SR                   0x28
#define COMCERTO_SPI_IMR                  0x2C
#define COMCERTO_SPI_ISR                  0x30
#define COMCERTO_SPI_RISR                 0x34
#define COMCERTO_SPI_TXOICR               0x38
#define COMCERTO_SPI_RXOICR               0x3C
#define COMCERTO_SPI_RXUICR               0x40
#define COMCERTO_SPI_MSTICR               0x44
#define COMCERTO_SPI_ICR                  0x48
#define COMCERTO_SPI_DMACR                0x4C
#define COMCERTO_SPI_DMATDLR              0x50
#define COMCERTO_SPI_DMARDLR              0x54
#define COMCERTO_SPI_IDR                  0x58
#define COMCERTO_SPI_DR                   0x60


/* SR - status register bits */
#define BUSY		(1<<0)	/* SSI busy flag, serial transfer in progress */
#define TFNF		(1<<1)	/* Transmit FIFO not full */
#define TFE		(1<<2)	/* Transmit FIFO empty */
#define RFNE		(1<<3)	/* Receive FIFO not empty */
#define RFF		(1<<4)	/* Receive FIFO full */
#define TXE		(1<<5)	/* Transmission error */
#define DCOL		(1<<6)	/* Data collision error */

/* Interrupt status after being masked */
#define TXEIS		(1<<0)	/* Transmit FIFO empty interrupt status */
#define TXOIS		(1<<1)	/* Transmit FIFO overflow interrupt status */
#define RXUIS		(1<<2)	/* Receive FIFO underflow interrupt status */
#define RXOIS		(1<<3)	/* Receive FIFO overflow interrupt status */
#define RXFIS		(1<<4)	/* Receive FIFO full interrupt status */
#define MSTIS		(1<<5)	/* Multi-Master contention interrupt status */

/* Interrupt status before being masked */
#define TXEIR		(1<<0)	/* Transmit FIFO empty interrupt status */
#define TXOIR		(1<<1)	/* Transmit FIFO overflow interrupt status */
#define RXUIR		(1<<2)	/* Receive FIFO underflow interrupt status */
#define RXOIR		(1<<3)	/* Receive FIFO overflow interrupt status */
#define RXFIR		(1<<4)	/* Receive FIFO full interrupt status */
#define MSTIR		(1<<5)	/* Multi-Master contention interrupt status */


/* Interrupt mask register */
#define TXEIM		(1<<0)	/* Transmit FIFO empty interrupt status */
#define TXOIM		(1<<1)	/* Transmit FIFO overflow interrupt status */
#define RXUIM		(1<<2)	/* Receive FIFO underflow interrupt status */
#define RXOIM		(1<<3)	/* Receive FIFO overflow interrupt status */
#define RXFIM		(1<<4)	/* Receive FIFO full interrupt status */
#define MSTIM		(1<<5)	/* Multi-Master contention interrupt status */


#define SPI_TRANSFER_MODE_WRITE_ONLY	0x01
#define SPI_TRANSFER_MODE_READ_ONLY	0x02
#define SPI_TRANSFER_MODE_WRITE_READ	0x03
#define SPI_TRANSFER_MODE_EEPROM_READ	0x04

/* A SPI bus adapter instance */
struct spi_adapter
{
	unsigned long membase;
	unsigned long clock_rate;	
	struct spi_master master;
};

#endif /* _COMCERTO_SPI_H */
