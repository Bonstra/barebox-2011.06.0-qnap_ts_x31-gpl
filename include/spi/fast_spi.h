#ifndef _SPI_H
#define _SPI_H

/* Includes */
#include <mach/comcerto-2000.h>
#include <mach/dma.h>
#include "types.h"
#include "config.h"

typedef enum _retcode
{
   RETCODE_OK = 0,
   RETCODE_ERROR,
   RETCODE_BAD_PARAMETER,
   RETCODE_MISMATCH,                /* Security violation. */
   RETCODE_FAILED,
} retcode;

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;
typedef volatile unsigned char V8;
typedef volatile unsigned short V16;
typedef volatile unsigned long V32;

#define DUS_AXI_RESET           (1 << 0)                        /* When Set to 1, DMA UART SMI AXI clock domain is in Reset. */
/*
 *   * DW DMA SSI
 *     */
#define DW_DMA_SSI_BASEADDR                                     (DW_DMA_DMAC_BASEADDR+0x00500000)

#define DW_DMA_SSI_CTRLR0                                       (DW_DMA_SSI_BASEADDR+0x00)
#define DW_DMA_SSI_CTRLR1                                       (DW_DMA_SSI_BASEADDR+0x04)
#define DW_DMA_SSI_SSIENR                                       (DW_DMA_SSI_BASEADDR+0x08)
#define DW_DMA_SSI_MWCR                                         (DW_DMA_SSI_BASEADDR+0x0C)
#define DW_DMA_SSI_SER                                          (DW_DMA_SSI_BASEADDR+0x10)
#define DW_DMA_SSI_BAUDR                                        (DW_DMA_SSI_BASEADDR+0x14)
#define DW_DMA_SSI_TXFTLR                                       (DW_DMA_SSI_BASEADDR+0x18)
#define DW_DMA_SSI_RXFTLR                                       (DW_DMA_SSI_BASEADDR+0x1C)
#define DW_DMA_SSI_TXFLR                                        (DW_DMA_SSI_BASEADDR+0x20)
#define DW_DMA_SSI_RXFLR                                        (DW_DMA_SSI_BASEADDR+0x24)
#define DW_DMA_SSI_SR                                           (DW_DMA_SSI_BASEADDR+0x28)
#define DW_DMA_SSI_IMR                                          (DW_DMA_SSI_BASEADDR+0x2C)
#define DW_DMA_SSI_ISR                                          (DW_DMA_SSI_BASEADDR+0x30)
#define DW_DMA_SSI_RISR                                         (DW_DMA_SSI_BASEADDR+0x34)
#define DW_DMA_SSI_TXOICR                                       (DW_DMA_SSI_BASEADDR+0x38)
#define DW_DMA_SSI_RXOICR                                       (DW_DMA_SSI_BASEADDR+0x3C)
#define DW_DMA_SSI_RXUICR                                       (DW_DMA_SSI_BASEADDR+0x40)
#define DW_DMA_SSI_MSTICR                                       (DW_DMA_SSI_BASEADDR+0x44)
#define DW_DMA_SSI_ICR                                          (DW_DMA_SSI_BASEADDR+0x48)
#define DW_DMA_SSI_DMACR                                        (DW_DMA_SSI_BASEADDR+0x4C)
#define DW_DMA_SSI_DMATDLR                                      (DW_DMA_SSI_BASEADDR+0x50)
#define DW_DMA_SSI_DMARDLR                                      (DW_DMA_SSI_BASEADDR+0x54)
#define DW_DMA_SSI_IDR                                          (DW_DMA_SSI_BASEADDR+0x58)
#define DW_DMA_SSI_SSI_COMP_VERSION                     (DW_DMA_SSI_BASEADDR+0x5C)
#define DW_DMA_SSI_DR                                           (DW_DMA_SSI_BASEADDR+0x60) /* Data Register (0x60 to 0xec) */
#define DW_DMA_SSI_RX_SAMPLE_DLY                        (DW_DMA_SSI_BASEADDR+0xF0)
#define DW_DMA_SSI_RSVD_0                                       (DW_DMA_SSI_BASEADDR+0xF4)
#define DW_DMA_SSI_RSVD_1                                       (DW_DMA_SSI_BASEADDR+0xF8)
#define DW_DMA_SSI_RSVD_2                                       (DW_DMA_SSI_BASEADDR+0xFC)

/* SPI CTRLR0 settings */
#define SPI_SC_POLARITY		0x0		/* inactive state of serial clock is high */
#define SPI_CFS   			0x7		/* Control Frame Size: 8-bit control word*/
#define SPI_SC_PHASE		0x1		/* serial clock toggles at the start of first data bit */
#define SPI_DFS			0x7		/* set it to fs-1; so for 8bit frames, set it to 7 */
#define SPI_TMOD_RW    		0x0		/* set transfer mode to write / read */
#define SPI_TMOD_WRITE          0x1		/* set transfer mode to write only */
#define SPI_TMOD_READ		0x2		/* set transfer mode to read only */
#define SPI_TMOD_RESERVED	0x3		/* set transfer mode to Resered (for DMA) */

/* SPI CTRLR1 settings */
#define SPI_NDF		    	0x0fff	/* Number of Data Frames: Maximum */

#define FAST_SPI_SCKDV		0xA	/* FIXME */


#define SPI_SLAVE_CS      	0x1		/* the chip select bit, for Dali it should be 0x1 */
#define SPI_FIFO_SIZE		8

/* SSI DMA Settings */
#define SSI_RDMAE			0x1
#define SSI_TDMAE			0x1
#define SSI_DMATDL      	0x10	/* Transmit Data Level */
#define SSI_DMARDL      	0xF		/* Recv Data Level */

/* SPI Commands */

#define SPI_CMD_WREN 		0x06
#define SPI_CMD_WRDI 		0x04
#define SPI_CMD_RDSR 		0x05
#define SPI_CMD_WRSR 		0x01
#define SPI_CMD_READ 		0x03
#define SPI_CMD_WRITE 		0x02
#define SPI_CMD_FAST_READ 	0x0B
#define SPI_CMD_RDID 		0x9F
#define SPI_CMD_SE 			0xD8
#define SPI_CMD_BE 			0xC7
#define SPI_CMD_PP 			0x02
#define SPI_CMD_DP 			0xB9
#define SPI_CMD_RES 		0xAB


/* SR - status register bits */
#define BUSY		(1<<0)	/* SSI busy flag, serial transfer in progress */
#define TFNF		(1<<1)	/* Transmit FIFO not full */
#define TFE			(1<<2)	/* Transmit FIFO empty */
#define RFNE		(1<<3)	/* Receive FIFO not empty */
#define RFF			(1<<4)	/* Receive FIFO full */
#define TXE			(1<<5)	/* Transmission error */
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


/* Exported Functions */
#if 0 //pratap
void spi_hw_init(void);
retcode spi_read(U32 eeprom_address, U8 *target_address, U32 len);
void fast_spi_hw_init(void);
retcode fast_spi_read(U32 start_address, U8 *target_address, U32 len);
retcode fast_spi_write(U32 start_address, U8 *target_address, U32 len);
U16 get_spi_addr_len( void );
U32 get_spi_polarity( void );
#endif

#endif
