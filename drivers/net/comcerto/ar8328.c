#include <common.h>
#include <init.h>
#include <miidev.h>
#include "ar8328.h"

#if defined(CONFIG_MACH_COMCERTO_C2K_MFCNEVM)
#include <mach/reset.h>
#endif

#define S17_LAN_PORT_VLAN          1
#define S17_WAN_PORT_VLAN          2

#define ENET_UNIT_GE1 1  
#define ENET_UNIT_GE0 0 /* Connected to the switch */

#define TRUE    1
#define FALSE   0

#define S17_PHY0_ADDR   0x0
#define S17_PHY1_ADDR   0x1
#define S17_PHY2_ADDR   0x2
#define S17_PHY3_ADDR   0x3
#define S17_PHY4_ADDR   0x4
//Patch by QNAP: enable dual lan
#define S17_PHY6_ADDR   0x6
///////////////////////////
#define S17_IND_PHY 4

/*
 * Track per-PHY port information.
 */
typedef struct {
    BOOL   isEnetPort;       /* normal enet port */
    BOOL   isPhyAlive;       /* last known state of link */
    int    ethUnit;          /* MAC associated with this phy port */
    uint32_t phyBase;
    uint32_t phyAddr;          /* PHY registers associated with this phy port */
    uint32_t VLANTableSetting; /* Value to be written to VLAN table */
} athrPhyInfo_t;

/*
 * Per-PHY information, indexed by PHY unit number
 * MAC port 0 - CPU port 0x100
 * All ports are connected to GE0 of the SoC MAC
 * LAN/WAN seperation by VLAN tags (port 0-3 with VLANID 1; port 4 with VLANID 2
 */
static athrPhyInfo_t athrPhyInfo[] = {
    {TRUE,   /* phy port 0 -- MAC port 1 0x200 */
     FALSE,
     ENET_UNIT_GE0,
     0,
     S17_PHY0_ADDR,
     S17_LAN_PORT_VLAN
    },

    {TRUE,   /* phy port 1 -- MAC port 2 0x300 */
     FALSE,
     ENET_UNIT_GE0,
     0,
     S17_PHY1_ADDR,
     S17_LAN_PORT_VLAN
    },

    {TRUE,   /* phy port 2 -- MAC port 3 0x400 */
     FALSE,
     ENET_UNIT_GE0,
     0,
     S17_PHY2_ADDR, 
     S17_LAN_PORT_VLAN
    },

    {TRUE,   /* phy port 3 -- MAC port 4 0x500 */
     FALSE,
     ENET_UNIT_GE0,
     0,
     S17_PHY3_ADDR, 
     S17_LAN_PORT_VLAN
    },

    {TRUE,   /* phy port 4 -- WAN port or MAC port 5 0x600 */
     FALSE,
     ENET_UNIT_GE1,
     0,
     S17_PHY4_ADDR, 
     S17_WAN_PORT_VLAN   /* set as WAN port */
    },
    
    {FALSE,  /* phy port 5 -- CPU port (no RJ45 connector) */
     FALSE,
     ENET_UNIT_GE0,
     0,
     0x00, 
     S17_LAN_PORT_VLAN    /* Send to all ports */
    },
//Patch by QNAP: enable dual lan
    {TRUE,  /* phy port 6 */
     TRUE,
     ENET_UNIT_GE0,
     0,
     S17_PHY6_ADDR, 
     S17_WAN_PORT_VLAN    /* set as WAN port */
    },
////////////////////    
};

#define S17_PHY_MAX 5

/* Range of valid PHY IDs is [MIN..MAX] */
#define S17_ID_MIN 0
#define S17_ID_MAX (S17_PHY_MAX-1)

/* Convenience macros to access myPhyInfo */
#define S17_IS_ENET_PORT(phyUnit) (athrPhyInfo[phyUnit].isEnetPort)
#define S17_IS_PHY_ALIVE(phyUnit) (athrPhyInfo[phyUnit].isPhyAlive)
#define S17_ETHUNIT(phyUnit) (athrPhyInfo[phyUnit].ethUnit)
#define S17_PHYBASE(phyUnit) (athrPhyInfo[phyUnit].phyBase)
#define S17_PHYADDR(phyUnit) (athrPhyInfo[phyUnit].phyAddr)
#define S17_VLAN_TABLE_SETTING(phyUnit) (athrPhyInfo[phyUnit].VLANTableSetting)


int athrs17_init(struct mii_device *mdev)
{
	int phy_addr; 
	unsigned int dummy;

#if defined(CONFIG_MACH_COMCERTO_C2K_MFCNEVM)
	/* Make the Atheros switch out of reset */
	GPIO_reset_external_device(COMPONENT_ATHEROS_SWITCH,0);
	udelay(1000000); //here delay 1s to give switch a chance to init.
#endif
	//configure the RGMII
	/* FIXME Configure broadcast ports: the configuration below
	 * will broadcast on ports 0-5 of the switch.
	 */
	athrs17_reg_write(mdev, S17_GLOFW_CTRL1_REG, \
			S17_BROAD_DPALL | S17_MULTI_FLOOD_DPALL | S17_UNI_FLOOD_DPALL);
	/* FIXME bit 30 of the PWS_REG is marked as reserved in the datasheet. */
	athrs17_reg_write(mdev, S17_PWS_REG, S17_PWS_CHIP_AR8327);

	/* Set delays for MAC0 */
	athrs17_reg_write(mdev, S17_P0PAD_MODE_REG, S17_MAC0_RGMII_EN | S17_MAC0_RGMII_TXCLK_DELAY | \
			S17_MAC0_RGMII_RXCLK_DELAY | (1 << S17_MAC0_RGMII_TXCLK_SHIFT) | \
			(1 << S17_MAC0_RGMII_RXCLK_SHIFT));
	/* Set bit 24 to enable MAC0 RGMII delay; set MAC6 as PHY mode (PHY4), QCA */
	athrs17_reg_write(mdev, S17_P6PAD_MODE_REG, S17_PHY4_RGMII_EN | S17_MAC6_RGMII_RXCLK_DELAY | \
			(0 << S17_MAC6_RGMII_RXCLK_SHIFT));
	/* Disable MAC5 and MAC6 (due to PHY4), QCA */
	athrs17_reg_write(mdev, S17_P5STATUS_REG, 0);
	athrs17_reg_write(mdev, S17_P6STATUS_REG, 0);
	
	athrs17_reg_write(mdev, S17_P0STATUS_REG, S17_PORT_STATUS_DEFAULT);

	/* AR8327/AR8328 v1.0 fixup */
	if ((athrs17_reg_read(mdev, S17_MASK_CTRL_REG) & 0xffff) == S17_CHIPID_V1_0)
	{
		for (phy_addr = 0x0; phy_addr <= S17_PHY_MAX; phy_addr++)
		{
			/* For 100M waveform */
			athrs17_phy_debug_write(mdev, phy_addr, 0x0, 0x02ea);
			/* Turn On Gigabit Clock */
			athrs17_phy_debug_write(mdev, phy_addr, 0x3d, 0x68a0);
		}
	}
	/* Set delays for PHY4 (connected to MAC5)
	*/
	dummy = athrs17_phy_debug_read(mdev, S17_PHY4_ADDR, 0x0);
	dummy |= (1 << 15); // Enable RGMII Rx clock delay
	athrs17_phy_debug_write(mdev, S17_PHY4_ADDR, 0x0, dummy);
	dummy = athrs17_phy_debug_read(mdev, S17_PHY4_ADDR, 0x5);
	dummy |= (1 << 8); // Enable RGMII Tx clock delay
	athrs17_phy_debug_write(mdev, S17_PHY4_ADDR, 0x5, dummy);


	/* enable HOL by default */
	for (phy_addr = 0; phy_addr < S17_MAC_MAX; phy_addr++)
	{
		switch (phy_addr) {
			case 0:
			case 5:
			case 6:
				athrs17_reg_write(mdev, S17_PORT0_HOL_CTRL0 + phy_addr * 8, \
						S17_HOL_CTRL0_WAN);
				break;

			default:
				athrs17_reg_write(mdev, S17_PORT0_HOL_CTRL0 + phy_addr * 8, \
						S17_HOL_CTRL0_LAN);
				break;
		}

		athrs17_reg_write(mdev, S17_PORT0_HOL_CTRL1 + phy_addr * 8, \
				S17_HOL_CTRL1_DEFAULT);
	}

	/* LED control */
	athrs17_reg_write(mdev, S17_LED_CTRL0_REG, 0xffb7ffb7);
	athrs17_reg_write(mdev, S17_LED_CTRL1_REG, 0xffb7ffb7);
	athrs17_reg_write(mdev, S17_LED_CTRL2_REG, 0xffb7ffb7);

	printf ("%s:done\n",__func__);

	return 0;

}

/******************************************************************************
*
* athrs17_phy_is_link_alive - test to see if the specified link is alive
*
* RETURNS:
*    TRUE  --> link is alive
*    FALSE --> link is down
*/
BOOL
athrs17_phy_is_link_alive(struct mii_device *mdev,int phyUnit)
{
    uint16_t phyHwStatus;
    uint32_t phyAddr;

    phyAddr = S17_PHYADDR(phyUnit);
 
    phyHwStatus = mdev->read(mdev, phyAddr, S17_PHY_SPEC_STATUS);

    if (phyHwStatus & S17_STATUS_LINK_PASS)
        return TRUE;

    printf("phy%d link down\n",phyUnit);	
    return FALSE;
}

/******************************************************************************
*
* athrs17_phy_stat 
*
*/

int
athrs17_phy_stat(struct mii_device *mdev)
{
    uint16_t  phyHwStatus;
    uint32_t  phyAddr;
    int       phyUnit;	
    int       ii = 200;

    for (phyUnit=0; phyUnit < S17_PHY_MAX; phyUnit++) {

        phyAddr = S17_PHYADDR(phyUnit);

        if (athrs17_phy_is_link_alive(mdev, phyUnit)) {

            do {
                phyHwStatus = mdev->read(mdev, phyAddr, 
                                              S17_PHY_SPEC_STATUS);
		        if(phyHwStatus & S17_STATUS_RESOLVED)
			        break;
                mdelay(10);
            }while(--ii);
            
            phyHwStatus = ((phyHwStatus & S17_STATUS_LINK_MASK) >>
                           S17_STATUS_LINK_SHIFT);

	    printf("phy%d phyhwstat:%x\n",phyUnit,phyHwStatus);
        } 

    }
    return 0;
}

/******************************************************************************
*
* athrs17_phy_init - reset and setup the PHY associated 
*
* Resets the associated PHY port.
*
* RETURNS:
*    TRUE  --> associated PHY is alive
*    FALSE --> no LINKs on this ethernet unit
*/

int
athrs17_phy_init(struct mii_device *mdev, int phyUnit)
{
	uint16_t  phyHwStatus;
	uint16_t  timeout;
	int       liveLinks = 0;
	uint32_t  phyBase = 0;
	uint32_t  phyAddr = 0;

	printf("athrs17_phy_init %d\n", phyUnit);

	phyBase = S17_PHYBASE(phyUnit);
	phyAddr = S17_PHYADDR(phyUnit);

	mdev->write(mdev, phyAddr, S17_AUTONEG_ADVERT,
			S17_ADVERTISE_ALL);

	mdev->write(mdev, phyAddr, S17_1000BASET_CONTROL,
			S17_ADVERTISE_1000FULL);

	/* Reset PHYs*/
	mdev->write(mdev, phyAddr, S17_PHY_CONTROL,
			S17_CTRL_AUTONEGOTIATION_ENABLE 
			| S17_CTRL_SOFTWARE_RESET);

	/*
	 * After the phy is reset, it takes a little while before
	 * it can respond properly.
	 */
	//FIXME: Now since this function is called multiple time for each LAN PHY the overrall delay
	//will also increase. Maybe this is not required as the phy status is being check 
	//So this is subjected to change during the bringup
	mdelay(1000);


	/*
	 * Wait up to 3 seconds for ALL associated PHYs to finish
	 * autonegotiation.  The only way we get out of here sooner is
	 * if ALL PHYs are connected AND finish autonegotiation.
	 */

	timeout=20;
	for (;;) {
		phyHwStatus = mdev->read(mdev, phyAddr, S17_PHY_CONTROL);

		if (S17_RESET_DONE(phyHwStatus)) {
			printf("Port %d, Neg Success\n", phyUnit);

			break;
		}
		if (timeout == 0) {
			printf("Port %d, Negogiation timeout\n", phyUnit);
			break;
		}
		if (--timeout == 0) {
			printf("Port %d, Negogiation timeout\n", phyUnit);
			break;
		}

		mdelay(150);
	}

	/*
	 * The PHY have had adequate time to autonegotiate.
	 * Now initialize software status.
	 *
	 * It's possible that some ports may take a bit longer
	 * to autonegotiate; but we can't wait forever.  They'll
	 * get noticed by mv_phyCheckStatusChange during regular
	 * polling activities.
	 */

	/* fine-tune PHY 0 and PHY 1*/
	if ((phyUnit == 0) || (phyUnit == 1))
	{
		mdev->write(mdev, S17_PHYADDR(phyUnit), S17_MMD_CTRL_REG, 0x3);
		mdev->write(mdev, S17_PHYADDR(phyUnit), S17_MMD_DATA_REG, 0x8007);
		mdev->write(mdev, S17_PHYADDR(phyUnit), S17_MMD_CTRL_REG, 0x4003);
		mdev->write(mdev, S17_PHYADDR(phyUnit), S17_MMD_DATA_REG, 0x8315);
	}

	/* fine-tune PHYs */
	mdev->write(mdev, S17_PHYADDR(phyUnit), S17_MMD_CTRL_REG, 0x3);
	mdev->write(mdev, S17_PHYADDR(phyUnit), S17_MMD_DATA_REG, 0x800d);
	mdev->write(mdev, S17_PHYADDR(phyUnit), S17_MMD_CTRL_REG, 0x4003);
	mdev->write(mdev, S17_PHYADDR(phyUnit), S17_MMD_DATA_REG, 0x103f);

	athrs17_phy_debug_write(mdev, S17_PHYADDR(phyUnit), 0x3d, 0x6860);
	
	/* for PHY4, QCA */
	if (phyUnit == S17_PHY4_ADDR)
	{
		athrs17_phy_debug_write(mdev, S17_PHYADDR(phyUnit), 0x12, 0x4c0c);
		athrs17_phy_debug_write(mdev, S17_PHYADDR(phyUnit), 0x0, 0x82ee);
		athrs17_phy_debug_write(mdev, S17_PHYADDR(phyUnit), 0x5, 0x3d46);
		athrs17_phy_debug_write(mdev, S17_PHYADDR(phyUnit), 0xb, 0xbc20);
	}

	if (athrs17_phy_is_link_alive(mdev, phyUnit)) {
		liveLinks++;
		S17_IS_PHY_ALIVE(phyUnit) = TRUE;
	} else {
		S17_IS_PHY_ALIVE(phyUnit) = FALSE;
	}

//	printf("eth%d: Phy Specific Status=%4.4x\n", mdev->read(mdev, S17_PHYADDR(phyUnit),S17_PHY_SPEC_STATUS));
//	printk("Phy setup Complete\n"); 


}

int
athrs17_phy_setup(struct mii_device *mdev, int phyUnit)
{
	int i;

        if(phyUnit == S17_PHY4_ADDR)
        {//WAN PHY
                athrs17_phy_init(mdev, phyUnit);
        }
        else if(phyUnit < S17_PHY4_ADDR)
        {//LAN PHYs
                for(i = 0 ; i <S17_PHY4_ADDR; i++)
                {
                        athrs17_phy_init(mdev, i);
                }
        }
}


unsigned int athrs17_reg_read(struct mii_device *mdev, unsigned int reg_addr)
{
	uint32_t reg_word_addr;
	uint32_t phy_addr, tmp_val, reg_val;
	uint16_t phy_val;
	uint8_t phy_reg;

	/* change reg_addr to 16-bit word address, 32-bit aligned */
	reg_word_addr = (reg_addr & 0xfffffffc) >> 1;

	/* configure register high address */
	phy_addr = 0x18;
	phy_reg = 0x0;
	phy_val = (uint16_t) ((reg_word_addr >> 8) & 0x3ff);  /* bit16-8 of reg address */
	mdev->write(mdev, phy_addr, phy_reg, phy_val);

	/* For some registers such as MIBs, since it is read/clear, we should */
	/* read the lower 16-bit register then the higher one */

	/* read register in lower address */
	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
	phy_reg = (uint8_t) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
	reg_val = (uint32_t) mdev->read(mdev, phy_addr, phy_reg);

	/* read register in higher address */
	reg_word_addr++;
	phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
	phy_reg = (uint8_t) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
	tmp_val = (uint32_t) mdev->read(mdev, phy_addr, phy_reg);
	reg_val |= (tmp_val << 16);

	//printf("%s: reg 0x%x val 0x%x\n",__func__, reg_addr, reg_val);

	return reg_val;   
}

void athrs17_reg_write(struct mii_device *mdev, unsigned int reg_addr, unsigned int reg_val)
{
	uint32_t reg_word_addr;
	uint32_t phy_addr;
	uint16_t phy_val;
	uint8_t phy_reg;

	//printf("%s: reg 0x%x val 0x%x\n",__func__, reg_addr, reg_val);

	/* change reg_addr to 16-bit word address, 32-bit aligned */
	reg_word_addr = (reg_addr & 0xfffffffc) >> 1;

	/* configure register high address */
    phy_addr = 0x18;
    phy_reg = 0x0;
    phy_val = (uint16_t) ((reg_word_addr >> 8) & 0x3ff);  /* bit16-8 of reg address */
    mdev->write(mdev, phy_addr, phy_reg, phy_val);

    /* For S17 registers such as ARL and VLAN, since they include BUSY bit */
    /* in higher address, we should write the lower 16-bit register then the */
    /* higher one */

    /* write register in lower address */
    phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
    phy_reg = (uint8_t) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
    phy_val = (uint16_t) (reg_val & 0xffff);
    mdev->write(mdev, phy_addr, phy_reg, phy_val); 
    
    /* read register in higher address */
    reg_word_addr++;
    phy_addr = 0x10 | ((reg_word_addr >> 5) & 0x7); /* bit7-5 of reg address */
    phy_reg = (uint8_t) (reg_word_addr & 0x1f);   /* bit4-0 of reg address */
    phy_val = (uint16_t) ((reg_val >> 16) & 0xffff);
    mdev->write(mdev, phy_addr, phy_reg, phy_val);

}

unsigned int athrs17_phy_debug_read(struct mii_device *mdev, unsigned int phy_addr, unsigned int reg_addr)
{
	mdev->write(mdev, phy_addr, S17_DEBUG_PORT_ADDRESS, reg_addr);
	return (unsigned int) mdev->read(mdev, phy_addr, S17_DEBUG_PORT_DATA);
}
void athrs17_phy_debug_write(struct mii_device *mdev, unsigned int phy_addr, unsigned int reg_addr, unsigned int reg_val)
{
	mdev->write(mdev, phy_addr, S17_DEBUG_PORT_ADDRESS, reg_addr);
	mdev->write(mdev, phy_addr, S17_DEBUG_PORT_DATA, reg_val);
}


EXPORT_SYMBOL(athrs17_init)
EXPORT_SYMBOL(athrs17_phy_setup)
EXPORT_SYMBOL(athrs17_phy_stat)
EXPORT_SYMBOL(athrs17_reg_read)
EXPORT_SYMBOL(athrs17_reg_write)
EXPORT_SYMBOL(athrs17_phy_debug_read)
EXPORT_SYMBOL(athrs17_phy_debug_write)
