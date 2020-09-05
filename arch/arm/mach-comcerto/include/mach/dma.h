#ifndef _DMA_H
#define _DMA_H

#define DW_DMA_DMAC_BASEADDR	0x96000000

/* Channel Registers */
#define DW_DMA_DMAC_SAR0                                        (DW_DMA_DMAC_BASEADDR+0x000)
#define DW_DMA_DMAC_DAR0                                        (DW_DMA_DMAC_BASEADDR+0x008)
#define DW_DMA_DMAC_LLP0                                        (DW_DMA_DMAC_BASEADDR+0x010)
#define DW_DMA_DMAC_CTL0                                        (DW_DMA_DMAC_BASEADDR+0x018)
#define DW_DMA_DMAC_SSTAT0                                      (DW_DMA_DMAC_BASEADDR+0x020)
#define DW_DMA_DMAC_DSTAT0                                      (DW_DMA_DMAC_BASEADDR+0x028)
#define DW_DMA_DMAC_SSTATAR0                            (DW_DMA_DMAC_BASEADDR+0x030)
#define DW_DMA_DMAC_DSTATAR0                            (DW_DMA_DMAC_BASEADDR+0x038)
#define DW_DMA_DMAC_CFG0                                        (DW_DMA_DMAC_BASEADDR+0x040)
#define DW_DMA_DMAC_SGR0                                        (DW_DMA_DMAC_BASEADDR+0x048)
#define DW_DMA_DMAC_DSR0                                        (DW_DMA_DMAC_BASEADDR+0x050)

#define DW_DMA_DMAC_SAR1                                        (DW_DMA_DMAC_BASEADDR+0x058)
#define DW_DMA_DMAC_DAR1                                        (DW_DMA_DMAC_BASEADDR+0x060)
#define DW_DMA_DMAC_LLP1                                        (DW_DMA_DMAC_BASEADDR+0x068)
#define DW_DMA_DMAC_CTL1                                        (DW_DMA_DMAC_BASEADDR+0x070)
#define DW_DMA_DMAC_SSTAT1                                      (DW_DMA_DMAC_BASEADDR+0x078)
#define DW_DMA_DMAC_DSTAT1                                      (DW_DMA_DMAC_BASEADDR+0x080)
#define DW_DMA_DMAC_SSTATAR1                            (DW_DMA_DMAC_BASEADDR+0x088)
#define DW_DMA_DMAC_DSTATAR1                            (DW_DMA_DMAC_BASEADDR+0x090)
#define DW_DMA_DMAC_CFG1                                        (DW_DMA_DMAC_BASEADDR+0x098)
#define DW_DMA_DMAC_SGR1                                        (DW_DMA_DMAC_BASEADDR+0x0A0)
#define DW_DMA_DMAC_DSR1                                        (DW_DMA_DMAC_BASEADDR+0x0A8)

#define DW_DMA_DMAC_SAR2                                        (DW_DMA_DMAC_BASEADDR+0x0B0)
#define DW_DMA_DMAC_DAR2                                        (DW_DMA_DMAC_BASEADDR+0x0B8)
#define DW_DMA_DMAC_LLP2                                        (DW_DMA_DMAC_BASEADDR+0x0C0)
#define DW_DMA_DMAC_CTL2                                        (DW_DMA_DMAC_BASEADDR+0x0C8)
#define DW_DMA_DMAC_SSTAT2                                      (DW_DMA_DMAC_BASEADDR+0x0D0)
#define DW_DMA_DMAC_DSTAT2                                      (DW_DMA_DMAC_BASEADDR+0x0D8)
#define DW_DMA_DMAC_SSTATAR2                            (DW_DMA_DMAC_BASEADDR+0x0E0)
#define DW_DMA_DMAC_DSTATAR2                            (DW_DMA_DMAC_BASEADDR+0x0E8)
#define DW_DMA_DMAC_CFG2                                        (DW_DMA_DMAC_BASEADDR+0x0F0)
#define DW_DMA_DMAC_SGR2                                        (DW_DMA_DMAC_BASEADDR+0x0F8)
#define DW_DMA_DMAC_DSR2                                        (DW_DMA_DMAC_BASEADDR+0x100)

#define DW_DMA_DMAC_SAR3                                        (DW_DMA_DMAC_BASEADDR+0x108)
#define DW_DMA_DMAC_DAR3                                        (DW_DMA_DMAC_BASEADDR+0x110)
#define DW_DMA_DMAC_LLP3                                        (DW_DMA_DMAC_BASEADDR+0x118)
#define DW_DMA_DMAC_CTL3                                        (DW_DMA_DMAC_BASEADDR+0x120)
#define DW_DMA_DMAC_SSTAT3                                      (DW_DMA_DMAC_BASEADDR+0x128)
#define DW_DMA_DMAC_DSTAT3                                      (DW_DMA_DMAC_BASEADDR+0x130)
#define DW_DMA_DMAC_SSTATAR3                            (DW_DMA_DMAC_BASEADDR+0x138)
#define DW_DMA_DMAC_DSTATAR3                            (DW_DMA_DMAC_BASEADDR+0x140)
#define DW_DMA_DMAC_CFG3                                        (DW_DMA_DMAC_BASEADDR+0x148)
#define DW_DMA_DMAC_SGR3                                        (DW_DMA_DMAC_BASEADDR+0x150)
#define DW_DMA_DMAC_DSR3                                        (DW_DMA_DMAC_BASEADDR+0x158)

#define DW_DMA_DMAC_SAR4                                        (DW_DMA_DMAC_BASEADDR+0x160)
#define DW_DMA_DMAC_DAR4                                        (DW_DMA_DMAC_BASEADDR+0x168)
#define DW_DMA_DMAC_LLP4                                        (DW_DMA_DMAC_BASEADDR+0x170)
#define DW_DMA_DMAC_CTL4                                        (DW_DMA_DMAC_BASEADDR+0x178)
#define DW_DMA_DMAC_SSTAT4                                      (DW_DMA_DMAC_BASEADDR+0x180)
#define DW_DMA_DMAC_DSTAT4                                      (DW_DMA_DMAC_BASEADDR+0x188)
#define DW_DMA_DMAC_SSTATAR4                            (DW_DMA_DMAC_BASEADDR+0x190)
#define DW_DMA_DMAC_DSTATAR4                            (DW_DMA_DMAC_BASEADDR+0x198)
#define DW_DMA_DMAC_CFG4                                        (DW_DMA_DMAC_BASEADDR+0x1A0)
#define DW_DMA_DMAC_SGR4                                        (DW_DMA_DMAC_BASEADDR+0x1A8)
#define DW_DMA_DMAC_DSR4                                        (DW_DMA_DMAC_BASEADDR+0x1B0)

#define DW_DMA_DMAC_SAR5                                        (DW_DMA_DMAC_BASEADDR+0x1B8)
#define DW_DMA_DMAC_DAR5                                        (DW_DMA_DMAC_BASEADDR+0x1C0)
#define DW_DMA_DMAC_LLP5                                        (DW_DMA_DMAC_BASEADDR+0x1C8)
#define DW_DMA_DMAC_CTL5                                        (DW_DMA_DMAC_BASEADDR+0x1D0)
#define DW_DMA_DMAC_SSTAT5                                      (DW_DMA_DMAC_BASEADDR+0x1D8)
#define DW_DMA_DMAC_DSTAT5                                      (DW_DMA_DMAC_BASEADDR+0x1E0)
#define DW_DMA_DMAC_SSTATAR5                            (DW_DMA_DMAC_BASEADDR+0x1E8)
#define DW_DMA_DMAC_DSTATAR5                            (DW_DMA_DMAC_BASEADDR+0x1F0)
#define DW_DMA_DMAC_CFG5                                        (DW_DMA_DMAC_BASEADDR+0x1F8)
#define DW_DMA_DMAC_SGR5                                        (DW_DMA_DMAC_BASEADDR+0x200)
#define DW_DMA_DMAC_DSR5                                        (DW_DMA_DMAC_BASEADDR+0x208)

#define DW_DMA_DMAC_SAR6                                        (DW_DMA_DMAC_BASEADDR+0x210)
#define DW_DMA_DMAC_DAR6                                        (DW_DMA_DMAC_BASEADDR+0x218)
#define DW_DMA_DMAC_LLP6                                        (DW_DMA_DMAC_BASEADDR+0x220)
#define DW_DMA_DMAC_CTL6                                        (DW_DMA_DMAC_BASEADDR+0x228)
#define DW_DMA_DMAC_SSTAT6                                      (DW_DMA_DMAC_BASEADDR+0x230)
#define DW_DMA_DMAC_DSTAT6                                      (DW_DMA_DMAC_BASEADDR+0x238)
#define DW_DMA_DMAC_SSTATAR6                            (DW_DMA_DMAC_BASEADDR+0x240)
#define DW_DMA_DMAC_DSTATAR6                            (DW_DMA_DMAC_BASEADDR+0x248)
#define DW_DMA_DMAC_CFG6                                        (DW_DMA_DMAC_BASEADDR+0x250)
#define DW_DMA_DMAC_SGR6                                        (DW_DMA_DMAC_BASEADDR+0x258)
#define DW_DMA_DMAC_DSR6                                        (DW_DMA_DMAC_BASEADDR+0x260)

#define DW_DMA_DMAC_SAR7                                        (DW_DMA_DMAC_BASEADDR+0x268)
#define DW_DMA_DMAC_DAR7                                        (DW_DMA_DMAC_BASEADDR+0x270)
#define DW_DMA_DMAC_LLP7                                        (DW_DMA_DMAC_BASEADDR+0x278)
#define DW_DMA_DMAC_CTL7                                        (DW_DMA_DMAC_BASEADDR+0x280)
#define DW_DMA_DMAC_SSTAT7                                      (DW_DMA_DMAC_BASEADDR+0x288)
#define DW_DMA_DMAC_DSTAT7                                      (DW_DMA_DMAC_BASEADDR+0x290)
#define DW_DMA_DMAC_SSTATAR7                            (DW_DMA_DMAC_BASEADDR+0x298)
#define DW_DMA_DMAC_DSTATAR7                            (DW_DMA_DMAC_BASEADDR+0x2A0)
#define DW_DMA_DMAC_CFG7                                        (DW_DMA_DMAC_BASEADDR+0x2A8)
#define DW_DMA_DMAC_SGR7                                        (DW_DMA_DMAC_BASEADDR+0x2B0)
#define DW_DMA_DMAC_DSR7                                        (DW_DMA_DMAC_BASEADDR+0x2B8)

/* Interrupt Registers */
#define DW_DMA_DMAC_RAW_TFR                                     (DW_DMA_DMAC_BASEADDR+0x2C0)
#define DW_DMA_DMAC_RAW_BLOCK                           (DW_DMA_DMAC_BASEADDR+0x2C8)
#define DW_DMA_DMAC_RAW_SRC_TRAN                        (DW_DMA_DMAC_BASEADDR+0x2D0)
#define DW_DMA_DMAC_RAW_DST_TRAN                        (DW_DMA_DMAC_BASEADDR+0x2D8)
#define DW_DMA_DMAC_RAW_ERR                                     (DW_DMA_DMAC_BASEADDR+0x2E0)
#define DW_DMA_DMAC_STATUS_TFR                          (DW_DMA_DMAC_BASEADDR+0x2E8)
#define DW_DMA_DMAC_STATUS_BLK                          (DW_DMA_DMAC_BASEADDR+0x2F0)
#define DW_DMA_DMAC_STATUS_SRC_TRAN                     (DW_DMA_DMAC_BASEADDR+0x2F8)
#define DW_DMA_DMAC_STATUS_DST_TRAN                     (DW_DMA_DMAC_BASEADDR+0x300)
#define DW_DMA_DMAC_STATUS_ERR                          (DW_DMA_DMAC_BASEADDR+0x308)
#define DW_DMA_DMAC_MASK_TFR                            (DW_DMA_DMAC_BASEADDR+0x310)
#define DW_DMA_DMAC_MASK_BLOCK                          (DW_DMA_DMAC_BASEADDR+0x318)
#define DW_DMA_DMAC_MASK_SRC_TRAN                       (DW_DMA_DMAC_BASEADDR+0x320)
#define DW_DMA_DMAC_MASK_DST_TRAN                       (DW_DMA_DMAC_BASEADDR+0x328)
#define DW_DMA_DMAC_MASK_ERR                            (DW_DMA_DMAC_BASEADDR+0x330)
#define DW_DMA_DMAC_CLEAR_TFR                           (DW_DMA_DMAC_BASEADDR+0x338)
#define DW_DMA_DMAC_CLEAR_BLK                           (DW_DMA_DMAC_BASEADDR+0x340)
#define DW_DMA_DMAC_CLEAR_SRC_TRAN                      (DW_DMA_DMAC_BASEADDR+0x348)
#define DW_DMA_DMAC_CLEAR_DST_TRAN                      (DW_DMA_DMAC_BASEADDR+0x350)
#define DW_DMA_DMAC_CLEAR_ERR                           (DW_DMA_DMAC_BASEADDR+0x358)
#define DW_DMA_DMAC_STATUS_INT                          (DW_DMA_DMAC_BASEADDR+0x360)

/* Software Handshaking Registers */
#define DW_DMA_DMAC_REQ_SRC_REG                         (DW_DMA_DMAC_BASEADDR+0x368)
#define DW_DMA_DMAC_REQ_DST_REG                         (DW_DMA_DMAC_BASEADDR+0x370)
#define DW_DMA_DMAC_SGL_REQ_SRC_REG                     (DW_DMA_DMAC_BASEADDR+0x378)
#define DW_DMA_DMAC_SGL_REQ_DST_REG                     (DW_DMA_DMAC_BASEADDR+0x380)
#define DW_DMA_DMAC_LST_SRC_REG                         (DW_DMA_DMAC_BASEADDR+0x388)
#define DW_DMA_DMAC_LST_DST_REG                         (DW_DMA_DMAC_BASEADDR+0x390)

/* Miscellaneous Registers */
#define DW_DMA_DMAC_DMA_CFG_REG                         (DW_DMA_DMAC_BASEADDR+0x398)
#define DW_DMA_DMAC_CH_EN_REG                           (DW_DMA_DMAC_BASEADDR+0x3A0)
#define DW_DMA_DMAC_DMA_LD_REG                          (DW_DMA_DMAC_BASEADDR+0x3A8)
#define DW_DMA_DMAC_DMA_TEST_REG                        (DW_DMA_DMAC_BASEADDR+0x3B0)
#define DW_DMA_DMAC_DMA_COMP_PA_RAMS_6          (DW_DMA_DMAC_BASEADDR+0x3C8)
#define DW_DMA_DMAC_DMA_COMP_PA_RAMS_5          (DW_DMA_DMAC_BASEADDR+0x3D0)
#define DW_DMA_DMAC_DMA_COMP_PA_RAMS_4          (DW_DMA_DMAC_BASEADDR+0x3D8)
#define DW_DMA_DMAC_DMA_COMP_PA_RAMS_3          (DW_DMA_DMAC_BASEADDR+0x3E0)
#define DW_DMA_DMAC_DMA_COMP_PA_RAMS_2          (DW_DMA_DMAC_BASEADDR+0x3E8)
#define DW_DMA_DMAC_DMA_COMP_PA_RAMS_1          (DW_DMA_DMAC_BASEADDR+0x3F0)
#define DW_DMA_DMAC_DMA_COMP_ID_REG                     (DW_DMA_DMAC_BASEADDR+0x3F8)


/* DW DMA UART0/UART1 */
#define DW_DMA_UART_RBR                                        (UART_BASEADDR+0x00)
#define DW_DMA_UART_THR                                        (UART_BASEADDR+0x00)
#define DW_DMA_UART_DLL                                        (UART_BASEADDR+0x00)
#define DW_DMA_UART_DLH                                        (UART_BASEADDR+0x04)
#define DW_DMA_UART_IER                                        (UART_BASEADDR+0x04)
#define DW_DMA_UART_IIR                                        (UART_BASEADDR+0x08)
#define DW_DMA_UART_FCR                                        (UART_BASEADDR+0x08)
#define DW_DMA_UART_LCR                                        (UART_BASEADDR+0x0C)
#define DW_DMA_UART_MCR                                        (UART_BASEADDR+0x10)
#define DW_DMA_UART_LSR                                        (UART_BASEADDR+0x14)
#define DW_DMA_UART_MSR                                        (UART_BASEADDR+0x18)
#define DW_DMA_UART_SCR                                        (UART_BASEADDR+0x1C)
#define DW_DMA_UART_LPDLL                                      (UART_BASEADDR+0x20)
#define DW_DMA_UART_LPDLH                                      (UART_BASEADDR+0x24)
#define DW_DMA_UART_SRBR                                       (UART_BASEADDR+0x30) /* Shadow Receive Buffer        Register (0x30 to 0x6C) */
#define DW_DMA_UART_STHR                                       (UART_BASEADDR+0x30) /* Shadow Transmit Holding      Register (0x30 to 0x6C) */
#define DW_DMA_UART_FAR                                        (UART_BASEADDR+0x40)
#define DW_DMA_UART_TFR                                        (UART_BASEADDR+0x70)
#define DW_DMA_UART_RFW                                        (UART_BASEADDR+0x74)
#define DW_DMA_UART_USR                                        (UART_BASEADDR+0x78)
#define DW_DMA_UART_TFL                                        (UART_BASEADDR+0x7C)
#define DW_DMA_UART_RFL                                        (UART_BASEADDR+0x80)
#define DW_DMA_UART_SRR                                        (UART_BASEADDR+0x84)
#define DW_DMA_UART_SRTS                                       (UART_BASEADDR+0x88)
#define DW_DMA_UART_DR                                         (UART_BASEADDR+0x8C)
#define DW_DMA_UART_SBCR                                       (UART_BASEADDR+0x90)
#define DW_DMA_UART_SDMAM                                      (UART_BASEADDR+0x94)
#define DW_DMA_UART_SFE                                        (UART_BASEADDR+0x98)
#define DW_DMA_UART_SRT                                        (UART_BASEADDR+0x9C)
#define DW_DMA_UART_STET                                       (UART_BASEADDR+0xA0)
#define DW_DMA_UART_HTX                                        (UART_BASEADDR+0xA4)
#define DW_DMA_UART_DMASA                                      (UART_BASEADDR+0xA8)
#define DW_DMA_UART_CPR                                        (UART_BASEADDR+0xF4)
#define DW_DMA_UART_UCV                                        (UART_BASEADDR+0xF8)
#define DW_DMA_UART_CTR                                        (UART_BASEADDR+0xFC)


#define DMA_XFER_DLEN   16
#define DMA_READ_DESTADDR       0x83000000


/* Includes */
#include "types.h"
#include "config.h"

/* DMA Channels */
#define DMA_CHANNEL_0					0x0
#define DMA_CHANNEL_1					0x1
#define DMA_CHANNEL_2					0x2
#define DMA_CHANNEL_3					0x3
#define DMA_CHANNEL_4					0x4
#define DMA_CHANNEL_5					0x5
#define DMA_CHANNEL_6					0x6
#define DMA_CHANNEL_7					0x7

/* DMA Channel mask */
#define DMA_CHANNEL_0_MASK				BIT_0_MSK
#define DMA_CHANNEL_1_MASK				BIT_1_MSK
#define DMA_CHANNEL_2_MASK				BIT_2_MSK
#define DMA_CHANNEL_3_MASK				BIT_3_MSK
#define DMA_CHANNEL_4_MASK				BIT_4_MSK
#define DMA_CHANNEL_5_MASK				BIT_5_MSK
#define DMA_CHANNEL_6_MASK				BIT_6_MSK
#define DMA_CHANNEL_7_MASK				BIT_7_MSK

#define DMA_GLOBAL_ENABLE				0x1
#define DMA_REG_WE_SHIFT				0x8


#define DMA_CHANNEL_REG_SAR_BASE		DW_DMA_DMAC_SAR0
#define DMA_CHANNEL_REG_DAR_BASE		DW_DMA_DMAC_DAR0
#define DMA_CHANNEL_REG_LLP_BASE		DW_DMA_DMAC_LLP0
#define DMA_CHANNEL_REG_CTL_BASE		DW_DMA_DMAC_CTL0
#define DMA_CHANNEL_REG_SSTAT_BASE		DW_DMA_DMAC_SSTAT0
#define DMA_CHANNEL_REG_DSTAT_BASE		DW_DMA_DMAC_DSTAT0
#define DMA_CHANNEL_REG_SSTATAR_BASE	DW_DMA_DMAC_SSTATAR0
#define DMA_CHANNEL_REG_DSTATAR_BASE	DW_DMA_DMAC_DSTATAR0
#define DMA_CHANNEL_REG_CFG_BASE		DW_DMA_DMAC_CFG0
#define DMA_CHANNEL_REG_SGR_BASE		DW_DMA_DMAC_SGR0
#define DMA_CHANNEL_REG_DSR_BASE		DW_DMA_DMAC_DSR0


#define DMA_CHANNEL_REG_COUNT			11
#define DMA_CHANNEL_REG_SIZE			8


/* DMA Control Register for Channel [0-31] */

/*
 * Single-block Transfer -- without write-back
 * of control and status information enabled at
 * the end of the single-block transfer
 */

#define DMA_CTL_INT_EN					0x1
#define DMA_CTL_INT_EN_MASK				0x1
#define DMA_CTL_INT_EN_SHIFT			0

#define DMA_CTL_DST_TR_WIDTH			0x0 		/* 8 bit */
#define DMA_CTL_DST_TR_WIDTH_MASK		0x7
#define DMA_CTL_DST_TR_WIDTH_SHIFT		1

#define DMA_CTL_SRC_TR_WIDTH			0x0 		/* 8 bit */
#define DMA_CTL_SRC_TR_WIDTH_MASK		0x7
#define DMA_CTL_SRC_TR_WIDTH_SHIFT		4

#define DMA_CTL_DINC					0x0 		/* Incriment */
#define DMA_CTL_DINC_MASK				0x3
#define DMA_CTL_DINC_SHIFT				7

#define DMA_CTL_SINC					0x2 		/* No Change */
#define DMA_CTL_SINC_MASK				0x3
#define DMA_CTL_SINC_SHIFT				9

#define DMA_CTL_DEST_MSIZE			0x3 //0x1 		/* Number of data items to be transferred = 4 */
#define DMA_CTL_DEST_MSIZE_WR			0x1 //0x1 		/* Number of data items to be transferred = 4 */
#define DMA_CTL_DEST_MSIZE_MASK			0x7
#define DMA_CTL_DEST_MSIZE_SHIFT		11

#define DMA_CTL_SRC_MSIZE				0x3 //0x1			/* Number of data items to be transferred = 4 */
#define DMA_CTL_SRC_MSIZE_MASK			0x7
#define DMA_CTL_SRC_MSIZE_SHIFT			14

#define DMA_CTL_SRC_GATHER_EN			0x0			/* Disabled */
#define DMA_CTL_SRC_GATHER_EN_MASK		0x1
#define DMA_CTL_SRC_GATHER_EN_SHIFT		17

#define DMA_CTL_DST_SCATTER_EN			0x0			/* Disabled */
#define DMA_CTL_DST_SCATTER_EN_MASK		0x1
#define DMA_CTL_DST_SCATTER_EN_SHIFT	18

#define DMA_CTL_TT_FC					0x2			/* Peripheral to Memory, Flow Controller -- DW_ahb_dmac */
#define DMA_CTL_TT_FC_MASK				0x7
#define DMA_CTL_TT_FC_SHIFT				20

#define DMA_CTL_DMS						0x0			/* ?????? AHB master 1 */
#define DMA_CTL_DMS_WR						0x1			/* ?????? AHB master 1 */
#define DMA_CTL_DMS_MASK				0x3
#define DMA_CTL_DMS_SHIFT				23

#define DMA_CTL_SMS						0x1			/* ?????? AHB master 2 */
#define DMA_CTL_SMS_WR						0x0			/* ?????? AHB master 2 */
#define DMA_CTL_SMS_MASK				0x3
#define DMA_CTL_SMS_SHIFT				25

#define DMA_CTL_LLP_DST_EN				0x0
#define DMA_CTL_LLP_DST_EN_MASK			0x1
#define DMA_CTL_LLP_DST_EN_SHIFT		27

#define DMA_CTL_LLP_SRC_EN				0x0
#define DMA_CTL_LLP_SRC_EN_MASK			0x1
#define DMA_CTL_LLP_SRC_EN_SHIFT		28


/* DMA Control Register for Channel [32-63] */
/*
 * Single-block Transfer -- without write-back
 * of control and status information enabled at
 * the end of the single-block transfer
 */

/* DMA_CTL_BLOCK_TS
 * When the DW_ahb_dmac is the flow controller, the user writes this
 * field before the channel is enabled in order to indicate the block size.
 *
 * The number programmed into BLOCK_TS indicates the total number
 * of single transactions to perform for every block transfer.
 *
 * The width of the single transaction is determined by
 * DMA_CTL_SRC_TR_WIDTH.
 */
#define DMA_CTL_BLOCK_TS				SPI_NDF
#define DMA_CTL_BLOCK_TS_MASK			0XFFF
#define DMA_CTL_BLOCK_TS_SHIFT			0

#define DMA_CTL_DONE					0x0
#define DMA_CTL_DONE_MASK				0x1
#define DMA_CTL_DONE_SHIFT				12



/* DMA Config Register for Channel [0-31] */
/*
 * Single-block Transfer -- without write-back
 * of control and status information enabled at
 * the end of the single-block transfer
 */

#define DMA_CFG_RELOAD_DST				0x0			/* Disabled -- No Multi Block xfer */
#define DMA_CFG_RELOAD_DST_MASK			0x1
#define DMA_CFG_RELOAD_DST_SHIFT		31

#define DMA_CFG_RELOAD_SRC				0x0			/* Disabled -- No Multi Block xfer */
#define DMA_CFG_RELOAD_SRC_MASK			0x1
#define DMA_CFG_RELOAD_SRC_SHIFT		30

#define DMA_CFG_MAX_ABRST				0x0			/* max AMBA burst length -- not limited by software  */
#define DMA_CFG_MAX_ABRST_MASK			0x3FF
#define DMA_CFG_MAX_ABRST_SHIFT			20

#define DMA_CFG_SRC_HS_POL				0x0			/* Active high */
#define DMA_CFG_SRC_HS_POL_MASK			0x1
#define DMA_CFG_SRC_HS_POL_SHIFT		19

#define DMA_CFG_DST_HS_POL				0x0			/* Active high */
#define DMA_CFG_DST_HS_POL_MASK			0x1
#define DMA_CFG_DST_HS_POL_SHIFT		18

#define DMA_CFG_LOCK_B					0x0			/* No Locked DMA Transfers */
#define DMA_CFG_LOCK_B_MASK				0x1
#define DMA_CFG_LOCK_B_SHIFT			17

#define DMA_CFG_LOCK_CH					0x0			/* No exclusive Access to the channel */
#define DMA_CFG_LOCK_CH_MASK			0x1
#define DMA_CFG_LOCK_CH_SHIFT			16

#define DMA_CFG_LOCK_B_L				0x0
#define DMA_CFG_LOCK_B_L_MASK			0x3
#define DMA_CFG_LOCK_B_L_SHIFT			14

#define DMA_CFG_LOCK_CH_L				0x0
#define DMA_CFG_LOCK_CH_L_MASK			0x3
#define DMA_CFG_LOCK_CH_L_SHIFT			12

#define DMA_CFG_HS_SEL_SRC				0x0 		/* Hardware handshaking */
#define DMA_CFG_HS_SEL_SRC_MASK			0x1
#define DMA_CFG_HS_SEL_SRC_SHIFT		11

#define DMA_CFG_HS_SEL_DST				0x1 		/* S/w handshaking */
#define DMA_CFG_HS_SEL_DST_MASK			0x1
#define DMA_CFG_HS_SEL_DST_SHIFT		10

#define DMA_CFG_FIFO_EMPTY				0x1			/* Channel FIFO empty */
#define DMA_CFG_FIFO_EMPTY_MASK			0x1
#define DMA_CFG_FIFO_EMPTY_SHIFT		9

#define DMA_CFG_CH_SUSP					0x0			/* Not suspended */
#define DMA_CFG_CH_SUSP_MASK			0x1
#define DMA_CFG_CH_SUSP_SHIFT			8

#define DMA_CFG_CH_PRIOR				0x0			/* Should be kept equal to channel no */
#define DMA_CFG_CH_PRIOR_MASK			0x7
#define DMA_CFG_CH_PRIOR_SHIFT			5


/* DMA Config Register for Channel [32-63] */
/*
 * Single-block Transfer -- without write-back
 * of control and status information enabled at
 * the end of the single-block transfer
 */

#define DMA_CFG_DEST_PER				0x0
#define DMA_CFG_DEST_PER_WR				0x5
#define DMA_CFG_DEST_PER_MASK			0xF
#define DMA_CFG_DEST_PER_SHIFT			11

#define DMA_CFG_SRC_PER					0x4
#define DMA_CFG_SRC_PER_WR				0x0
#define DMA_CFG_SRC_PER_MASK			0xF
#define DMA_CFG_SRC_PER_SHIFT			7

#define DMA_CFG_SS_UPD_EN				0x0			/* Disabled */
#define DMA_CFG_SS_UPD_EN_MASK			0x1
#define DMA_CFG_SS_UPD_EN_SHIFT			6

#define DMA_CFG_DS_UPD_EN				0x0			/* Disabled */
#define DMA_CFG_DS_UPD_EN_MASK			0x1
#define DMA_CFG_DS_UPD_EN_SHIFT			5

#define DMA_CFG_PROTCTL					0x1
#define DMA_CFG_PROTCTL_MASK			0x7
#define DMA_CFG_PROTCTL_SHIFT			2

#define DMA_CFG_FIFO_MODE				0x0			/* Single AHB Space of the specified transfer width */
#define DMA_CFG_FIFO_MODE_MASK			0x1
#define DMA_CFG_FIFO_MODE_SHIFT			1

#define DMA_CFG_FCMODE					0x0
#define DMA_CFG_FCMODE_MASK				0x1
#define DMA_CFG_FCMODE_SHIFT			0

#endif
