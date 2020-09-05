#define APB_BASEADDR            0x90400000
#define GPIO_BASEADDR           (APB_BASEADDR + 0x070000)

/* General Purpose I/O */
#define GPIO_OUTPUT_REG         (GPIO_BASEADDR + 0x00)
#define GPIO_OE_REG             (GPIO_BASEADDR + 0x04)
#define GPIO_PIN_SELECT_REG     (GPIO_BASEADDR + 0x58)
#define GPIO_MISC_PIN_REG       (GPIO_BASEADDR + 0x60)

/* DW DMA DMAC */
#define DW_DMA_DMAC_BASEADDR            0x96000000

/* Channel Registers */
#define DW_DMA_DMAC_SAR0		(DW_DMA_DMAC_BASEADDR+0x000)
#define DW_DMA_DMAC_DAR0		(DW_DMA_DMAC_BASEADDR+0x008)
#define DW_DMA_DMAC_LLP0                (DW_DMA_DMAC_BASEADDR+0x010)
#define DW_DMA_DMAC_CTL0                (DW_DMA_DMAC_BASEADDR+0x018)
#define DW_DMA_DMAC_SSTAT0              (DW_DMA_DMAC_BASEADDR+0x020)
#define DW_DMA_DMAC_DSTAT0              (DW_DMA_DMAC_BASEADDR+0x028)
#define DW_DMA_DMAC_SSTATAR0            (DW_DMA_DMAC_BASEADDR+0x030)
#define DW_DMA_DMAC_DSTATAR0            (DW_DMA_DMAC_BASEADDR+0x038)
#define DW_DMA_DMAC_CFG0                (DW_DMA_DMAC_BASEADDR+0x040)
#define DW_DMA_DMAC_SGR0                (DW_DMA_DMAC_BASEADDR+0x048)
#define DW_DMA_DMAC_DSR0                (DW_DMA_DMAC_BASEADDR+0x050)

#define DW_DMA_DMAC_SAR1                (DW_DMA_DMAC_BASEADDR+0x058)
#define DW_DMA_DMAC_DAR1                (DW_DMA_DMAC_BASEADDR+0x060)
#define DW_DMA_DMAC_LLP1                (DW_DMA_DMAC_BASEADDR+0x068)
#define DW_DMA_DMAC_CTL1                (DW_DMA_DMAC_BASEADDR+0x070)
#define DW_DMA_DMAC_SSTAT1              (DW_DMA_DMAC_BASEADDR+0x078)
#define DW_DMA_DMAC_DSTAT1              (DW_DMA_DMAC_BASEADDR+0x080)
#define DW_DMA_DMAC_SSTATAR1            (DW_DMA_DMAC_BASEADDR+0x088)
#define DW_DMA_DMAC_DSTATAR1            (DW_DMA_DMAC_BASEADDR+0x090)
#define DW_DMA_DMAC_CFG1                (DW_DMA_DMAC_BASEADDR+0x098)
#define DW_DMA_DMAC_SGR1                (DW_DMA_DMAC_BASEADDR+0x0A0)
#define DW_DMA_DMAC_DSR1                (DW_DMA_DMAC_BASEADDR+0x0A8)

/* Interrupt Registers */
#define DW_DMA_DMAC_RAW_TFR                             (DW_DMA_DMAC_BASEADDR+0x2C0)
#define DW_DMA_DMAC_RAW_BLOCK                           (DW_DMA_DMAC_BASEADDR+0x2C8)
#define DW_DMA_DMAC_RAW_SRC_TRAN                        (DW_DMA_DMAC_BASEADDR+0x2D0)
#define DW_DMA_DMAC_RAW_DST_TRAN                        (DW_DMA_DMAC_BASEADDR+0x2D8)
#define DW_DMA_DMAC_RAW_ERR                             (DW_DMA_DMAC_BASEADDR+0x2E0)
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
#define DW_DMA_DMAC_DMA_COMP_PA_RAMS_6			(DW_DMA_DMAC_BASEADDR+0x3C8)
#define DW_DMA_DMAC_DMA_COMP_PA_RAMS_5			(DW_DMA_DMAC_BASEADDR+0x3D0)
#define DW_DMA_DMAC_DMA_COMP_PA_RAMS_4			(DW_DMA_DMAC_BASEADDR+0x3D8)
#define DW_DMA_DMAC_DMA_COMP_PA_RAMS_3			(DW_DMA_DMAC_BASEADDR+0x3E0)
#define DW_DMA_DMAC_DMA_COMP_PA_RAMS_2			(DW_DMA_DMAC_BASEADDR+0x3E8)
#define DW_DMA_DMAC_DMA_COMP_PA_RAMS_1			(DW_DMA_DMAC_BASEADDR+0x3F0)
#define DW_DMA_DMAC_DMA_COMP_ID_REG                     (DW_DMA_DMAC_BASEADDR+0x3F8)

#define DMA_CHANNEL_REG_SAR_BASE                DW_DMA_DMAC_SAR0
#define DMA_CHANNEL_REG_CFG_BASE                DW_DMA_DMAC_CFG0

/* UART */
#define UART0_BASEADDR          0x96300000

#define LCR_CHAR_LEN_5          0x00      /* default */
#define LCR_CHAR_LEN_6          0x01
#define LCR_CHAR_LEN_7          0x02
#define LCR_CHAR_LEN_8          0x03
#define LCR_ONE_STOP            0x00      /* One stop bit! - default */
#define LCR_TWO_STOP            0x04      /* Two stop bit! */
#define LCR_PEN                 0x08      /* Parity Enable */
#define LCR_PARITY_NONE         0x00
#define LCR_EPS                 0x10      /* Even Parity Select */
#define LCR_PS                  0x20      /* Enable Parity  Stuff */
#define LCR_SBRK                0x40  /* Start Break */
#define LCR_PSB                 0x80      /* Parity Stuff Bit */
#define LCR_DLAB                0x80  /* UART 16550 Divisor Latch Assess */

#define LSR_FIFOE               (1 << 7)        /* FIFO Error Status */
#define LSR_TEMT                (1 << 6)        /* Transmitter Empty */
#define LSR_TDRQ                (1 << 5)        /* Transmit Data Request */
#define LSR_BI                  (1 << 4)        /* Break Interrupt */
#define LSR_FE                  (1 << 3)        /* Framing Error */
#define LSR_PE                  (1 << 2)        /* Parity Error */
#define LSR_OE                  (1 << 1)        /* Overrun Error */
#define LSR_DR                  (1 << 0)        /* Data Ready */

#define IER_DMAE                (1 << 7)        /* DMA Requests Enable */
#define IER_UUE                 (1 << 6)        /* UART Unit Enable */
#define IER_NRZE                (1 << 5)        /* NRZ coding Enable */
#define IER_RTIOE               (1 << 4)        /* Receiver Time Out Interrupt Enable */
#define IER_MIE                 (1 << 3)        /* Modem Interrupt Enable */
#define IER_RLSE                (1 << 2)        /* Receiver Line Status Interrupt Enable */
#define IER_TIE                 (1 << 1)        /* Transmit Data request Interrupt Enable */
#define IER_RAVIE               (1 << 0)        /* Receiver Data Available Interrupt Enable */

#define IIR_FIFOES1             (1 << 7)        /* FIFO Mode Enable Status */
#define IIR_FIFOES0             (1 << 6)        /* FIFO Mode Enable Status */
#define IIR_TOD                 (1 << 3)        /* Time Out Detected */
#define IIR_IID2                (1 << 2)        /* Interrupt Source Encoded */
#define IIR_IID1                (1 << 1)        /* Interrupt Source Encoded */
#define IIR_IP                  (1 << 0)        /* Interrupt Pending (active low) */

/* UART 16550 FIFO Control Register */
#define FCR_FIFOEN              0x01
#define FCR_RCVRRES             0x02
#define FCR_XMITRES             0x04

/* Interrupt Enable Register */
/* UART 16550 */
#define IER_RXTH                0x01    /* Enable Received Data Available Interrupt */
#define IER_TXTH                0x02    /* Enable Transmitter Empty Interrupt */

#define UART_THR		0x00
#define UART_RBR		0x00
#define UART_DLL		0x00
#define UART_IER		0x04
#define UART_DLH		0x04
#define UART_IIR		0x08
#define UART_FCR		0x08
#define UART_LCR		0x0C
#define UART_MCR		0x10
#define UART_LSR		0x14
#define UART_MSR		0x18
#define UART_SCR		0x1C
#define	UART_RFL		0x84

