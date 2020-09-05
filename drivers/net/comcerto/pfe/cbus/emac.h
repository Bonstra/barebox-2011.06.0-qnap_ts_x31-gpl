#ifndef _EMAC_H_
#define _EMAC_H_

#define EMAC_NETWORK_CONTROL		0x000
#define EMAC_NETWORK_CONFIG		0x004
#define EMAC_NETWORK_STATUS		0x008

#define EMAC_PHY_MANAGEMENT		0x034

#define EMAC_SPEC1_ADD_BOT		0x088
#define EMAC_SPEC1_ADD_TOP		0x08c
#define EMAC_SPEC2_ADD_BOT		0x090
#define EMAC_SPEC2_ADD_TOP		0x094
#define EMAC_SPEC3_ADD_BOT		0x098
#define EMAC_SPEC3_ADD_TOP		0x09c
#define EMAC_SPEC4_ADD_BOT		0x0a0
#define EMAC_SPEC4_ADD_TOP		0x0a4

#define EMAC_SPEC1_ADD_MASK_BOT		0x0c8
#define EMAC_SPEC1_ADD_MASK_TOP		0x0cc

#define EMAC_OCT_TX_BOT			0x100
#define EMAC_OCT_TX_TOP			0x104
#define EMAC_STATS_FRAMES_TX		0x108
#define EMAC_BROADCAST_TX		0x10C
#define EMAC_MULTICAST_TX		0x110
#define EMAC_STATS_PAUSE_TX		0x114
#define EMAC_FRAME64_TX			0x118
#define EMAC_FRAME65_TX			0x11C
#define EMAC_FRAME128_TX		0x120
#define EMAC_FRAME256_TX		0x124
#define EMAC_FRAME512_TX		0x128
#define EMAC_FRAME1024_TX		0x12C
#define EMAC_FRAME1519_TX		0x130
#define EMAC_STATS_TX_URUN		0x134
#define EMAC_STATS_SINGLE_COL		0x138
#define EMAC_STATS_MULTI_COL		0x13C
#define EMAC_STATS_EXCESS_COL		0x140
#define EMAC_STATS_LATE_COL		0x144
#define EMAC_STATS_DEF_TX		0x148
#define EMAC_STATS_CRS_ERRORS		0x14C
#define EMAC_OCT_RX_BOT			0x150
#define EMAC_OCT_RX_TOP			0x154
#define EMAC_STATS_FRAMES_RX		0x158
#define EMAC_BROADCAST_RX		0x15C
#define EMAC_MULTICAST_RX		0x160
#define EMAC_STATS_PAUSE_RX		0x164
#define EMAC_FRAME64_RX			0x168
#define EMAC_FRAME65_RX			0x16C
#define EMAC_FRAME128_RX		0x170
#define EMAC_FRAME256_RX		0x174
#define EMAC_FRAME512_RX		0x178
#define EMAC_FRAME1024_RX		0x17C
#define EMAC_FRAME1519_RX		0x180
#define EMAC_STATS_USIZE_FRAMES		0x184
#define EMAC_STATS_EXCESS_LEN		0x188
#define EMAC_STATS_JABBERS		0x18C
#define EMAC_STATS_FCS_ERRORS		0x190
#define EMAC_STATS_LENGTH_ERRORS	0x194
#define EMAC_STATS_RX_SYM_ERR		0x198
#define EMAC_STATS_ALIGN_ERRORS		0x19C
#define EMAC_STATS_RX_RES_ERR		0x1a0
#define EMAC_STATS_RX_ORUN		0x1a4

#define EMAC_SPEC5_ADD_BOT		0x300
#define EMAC_SPEC5_ADD_TOP		0x304
#define EMAC_SPEC6_ADD_BOT		0x308
#define EMAC_SPEC6_ADD_TOP		0x30c
#define EMAC_SPEC7_ADD_BOT		0x310
#define EMAC_SPEC7_ADD_TOP		0x314
#define EMAC_SPEC8_ADD_BOT		0x318
#define EMAC_SPEC8_ADD_TOP		0x31c
#define EMAC_SPEC9_ADD_BOT		0x320
#define EMAC_SPEC9_ADD_TOP		0x324
#define EMAC_SPEC10_ADD_BOT		0x328
#define EMAC_SPEC10_ADD_TOP		0x32c
#define EMAC_SPEC11_ADD_BOT		0x330
#define EMAC_SPEC11_ADD_TOP		0x334
#define EMAC_SPEC12_ADD_BOT		0x338
#define EMAC_SPEC12_ADD_TOP		0x33c
#define EMAC_SPEC13_ADD_BOT		0x340
#define EMAC_SPEC13_ADD_TOP		0x344
#define EMAC_SPEC14_ADD_BOT		0x348
#define EMAC_SPEC14_ADD_TOP		0x34c
#define EMAC_SPEC15_ADD_BOT		0x350
#define EMAC_SPEC15_ADD_TOP		0x354
#define EMAC_SPEC16_ADD_BOT		0x358
#define EMAC_SPEC16_ADD_TOP		0x35c
#define EMAC_SPEC17_ADD_BOT		0x360
#define EMAC_SPEC17_ADD_TOP		0x364
#define EMAC_SPEC18_ADD_BOT		0x368
#define EMAC_SPEC18_ADD_TOP		0x36c
#define EMAC_SPEC19_ADD_BOT		0x370
#define EMAC_SPEC19_ADD_TOP		0x374
#define EMAC_SPEC20_ADD_BOT		0x378
#define EMAC_SPEC20_ADD_TOP		0x37c
#define EMAC_SPEC21_ADD_BOT		0x380
#define EMAC_SPEC21_ADD_TOP		0x384
#define EMAC_SPEC22_ADD_BOT		0x388
#define EMAC_SPEC22_ADD_TOP		0x38c
#define EMAC_SPEC23_ADD_BOT		0x390
#define EMAC_SPEC23_ADD_TOP		0x394
#define EMAC_SPEC24_ADD_BOT		0x398
#define EMAC_SPEC24_ADD_TOP		0x39c
#define EMAC_SPEC25_ADD_BOT		0x3a0
#define EMAC_SPEC25_ADD_TOP		0x3a4
#define EMAC_SPEC26_ADD_BOT		0x3a8
#define EMAC_SPEC26_ADD_TOP		0x3ac
#define EMAC_SPEC27_ADD_BOT		0x3b0
#define EMAC_SPEC27_ADD_TOP		0x3b4
#define EMAC_SPEC28_ADD_BOT		0x3b8
#define EMAC_SPEC28_ADD_TOP		0x3bc
#define EMAC_SPEC29_ADD_BOT		0x3c0
#define EMAC_SPEC29_ADD_TOP		0x3c4
#define EMAC_SPEC30_ADD_BOT		0x3c8
#define EMAC_SPEC30_ADD_TOP		0x3cc
#define EMAC_SPEC31_ADD_BOT		0x3d0
#define EMAC_SPEC31_ADD_TOP		0x3d4
#define EMAC_SPEC32_ADD_BOT		0x3d8
#define EMAC_SPEC32_ADD_TOP		0x3dc

#define EMAC_CONTROL			0x7a0


/* GEMAC definitions and settings */

#define EMAC_PORT_0			0
#define EMAC_PORT_1			1
#define EMAC_PORT_2			2

/* The possible operating speeds of the MAC, currently supporting 10, 100 and
 * 1000Mb modes.
 */
typedef enum {SPEED_10M, SPEED_100M, SPEED_1000M, SPEED_1000M_PCS} MAC_SPEED;

#define GMII	1
#define MII	2
#define RMII	3
#define RGMII	4
#define SGMII	5

#define DUPLEX_HALF	0x00
#define DUPLEX_FULL	0x01

/* EMAC_NETWORK_CONTROL bits definition */
#define EMAC_RX_ENABLE			(1 << 2)
#define EMAC_TX_ENABLE			(1 << 3)
#define EMAC_MDIO_EN                    (1 << 4)


/* EMAC_NETWORK_CONFIG bits definition */
#define EMAC_SPEED_100		(1 << 0)
#define EMAC_HALF_DUP		(0 << 1)
#define EMAC_FULL_DUP		(1 << 1)
#define EMAC_DUPLEX_MASK	(1 << 1)
#define EMAC_ENABLE_JUMBO_FRAME (1 << 3)
#define EMAC_ENABLE_COPY_ALL	(1 << 4)	
#define EMAC_NO_BROADCAST	(1 << 5)
#define EMAC_ENABLE_MULTICAST	(1 << 6)
#define EMAC_ENABLE_UNICAST	(1 << 7)
#define EMAC_ENABLE_1536_RX	(1 << 8)
#define EMAC_SPEED_1000		(1 << 10)
#define EMAC_PCS_ENABLE		(1 << 11)
#define EMAC_ENABLE_PAUSE_RX	(1 << 13)
#define EMAC_REMOVE_FCS		(1 << 17)
#define EMAC_ENABLE_CHKSUM_RX	(1 << 24)
#define EMAC_ENABLE_FCS_RX	(1 << 26)
#define EMAC_SGMII_MODE_ENABLE	(1 << 27)
#define EMAC_DATA_BUS_WIDTH_SHIFT	21
#define EMAC_DATA_BUS_WIDTH_MASK (0x3 << EMAC_DATA_BUS_WIDTH_SHIFT)
#define EMAC_DATA_BUS_WIDTH_32	(0x00 << EMAC_DATA_BUS_WIDTH_SHIFT)
#define EMAC_DATA_BUS_WIDTH_64	(0x01 << EMAC_DATA_BUS_WIDTH_SHIFT)
#define EMAC_DATA_BUS_WIDTH_128	(0x10 << EMAC_DATA_BUS_WIDTH_SHIFT)

#define EMAC_SPEED_MASK		(EMAC_SPEED_100 | EMAC_SPEED_1000)

/* EMAC_CONTROL bits definition */
#define EMAC_TWO_BYTES_IP_ALIGN		(1 << 0) // two bytes IP alignement

//RMII enable – bit 1 / RGMII enable – bit 2
#define EMAC_RMII_MODE_ENABLE		((1 << 1) | (0 << 2))
#define EMAC_RMII_MODE_DISABLE		(0 << 1)
#define EMAC_RGMII_MODE_ENABLE		((0 << 1) | (1 << 2))
#define EMAC_RGMII_MODE_DISABLE		(0 << 2)
#define EMAC_MII_MODE_ENABLE		(EMAC_RMII_MODE_DISABLE | EMAC_RGMII_MODE_DISABLE)
#define EMAC_GMII_MODE_ENABLE		(EMAC_RMII_MODE_DISABLE | EMAC_RGMII_MODE_DISABLE)
#define EMAC_MODE_MASK			(0x3 << 1)

/* Default configuration */
#define EMAC0_DEFAULT_DUPLEX_MODE	FULLDUPLEX 
#define EMAC0_DEFAULT_EMAC_MODE		RGMII      
#define EMAC0_DEFAULT_EMAC_SPEED	SPEED_1000M   

#define EMAC1_DEFAULT_DUPLEX_MODE	FULLDUPLEX 
#define EMAC1_DEFAULT_EMAC_MODE		RGMII      
#define EMAC1_DEFAULT_EMAC_SPEED	SPEED_1000M   

#define EMAC2_DEFAULT_DUPLEX_MODE	FULLDUPLEX 
#define EMAC2_DEFAULT_EMAC_MODE		RGMII      
#define EMAC2_DEFAULT_EMAC_SPEED	SPEED_1000M   

/* The possible MDC clock division of the MAC, currently supporting 8, 16 , 32,
 * 48, 64, 96, 128, 224.
 */
typedef enum { MDC_DIV_8, MDC_DIV_16, MDC_DIV_32, MDC_DIV_48, MDC_DIV_64,
        MDC_DIV_96, MDC_DIV_128, MDC_DIV_224
} MAC_MDC_DIV;

#define MDC_DIV_MASK    0x7
#define MDC_DIV_SHIFT   18

/* The Address organisation for the MAC device.  All addresses are split into
 * two 32-bit register fields.  The first one (bottom) is the lower 32-bits of
 * the address and the other field are the high order bits - this may be 16-bits
 * in the case of MAC addresses, or 32-bits for the hash address.
 * In terms of memory storage, the first item (bottom) is assumed to be at a
 * lower address location than 'top'. i.e. top should be at address location of
 * 'bottom' + 4 bytes.
 */
typedef struct {
	u32 bottom;     /* Lower 32-bits of address. */
	u32 top;        /* Upper 32-bits of address. */
} MAC_ADDR;


/* The following is the organisation of the address filters section of the MAC
 * registers.  The Cadence MAC contains four possible specific address match
 * addresses, if an incoming frame corresponds to any one of these four
 * addresses then the frame will be copied to memory.
 * It is not necessary for all four of the address match registers to be
 * programmed, this is application dependant.
 */
typedef struct {
	MAC_ADDR one;        /* Specific address register 1. */
	MAC_ADDR two;        /* Specific address register 2. */
	MAC_ADDR three;      /* Specific address register 3. */
	MAC_ADDR four;       /* Specific address register 4. */
} SPEC_ADDR;

typedef struct {
	u32 mode; 
	u32 speed;
	u32 duplex;
} GEMAC_CFG;

#endif /* _EMAC_H_ */
