/*
 *  (C) Copyright 2011
 *  Author : Mindspeed Technologes
 *  
 *  See file CREDITS for list of people who contributed to this
 *  project.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *  MA 02111-1307 USA
 * */

#include <config.h>
#include <common.h>
#include <init.h>
#include <net.h>
#include <miidev.h>
#include <malloc.h>
#include <driver.h>
#include <asm/io.h>
#include <c2000_eth_pdata.h>
#include <mach/clkcore.h>
#include <mach/gpio.h>


#include "c2000_eth.h"
#ifdef CONFIG_AR8328_SWITCH
#include "ar8328.h"
#endif

struct gemac_s gem_info[] = {
	/* PORT_0 configuration */
	{
		/* GEMAC config */
		.gemac_mode = RGMII,
		.gemac_speed = SPEED_1000M,
		.gemac_duplex = DUPLEX_FULL,
                .flags = EMAC0_FLAGS,

		/* phy iface */
		.phy_reg_index = EMAC_PORT_0,
		.phy_address = EMAC0_PHY_ADDR,
	},
	/* PORT_1 configuration */
	{
		/* GEMAC config */
		.gemac_mode = RGMII,
		.gemac_speed = SPEED_1000M,
		.gemac_duplex = DUPLEX_FULL,
		.flags = EMAC1_FLAGS,

		/* phy iface */
		.phy_reg_index = EMAC_PORT_0,
		.phy_address = EMAC1_PHY_ADDR,
	},

	/* PORT_2 configuration */
	{
		/* GEMAC config */
		.gemac_mode = RGMII,
		.gemac_speed = SPEED_1000M,
		.gemac_duplex = DUPLEX_FULL,
		.flags = EMAC2_FLAGS,

		/* phy iface */
		.phy_reg_index = EMAC_PORT_0,
		.phy_address = 0,
	},
};


/********************************************************************
 * Helper functions for phy read/write
 *******************************************************************/

/* Max MII register/address (we support) */
#define MII_REGISTER_MAX  31
#define MII_ADDRESS_MAX   31

#define MDIO_TIMEOUT    5000

/********************************************************************
 * gem_phy_man_rd :
 * 	Performs phy management read operation. 
 *******************************************************************/
static int gem_phy_man_rd(struct c2000_eth_dev *priv, u32 phy_addr, u32 phy_reg)
{
        u32 write_data;

        write_data = 0x60020000 | ( (phy_addr & (u32) 0x1f) << 23) | ( (phy_reg & (u32) 0x1f) << 18); // read_op
        writel(write_data, priv->phyregisters + EMAC_PHY_MANAGEMENT);

        return 0;
}

static int gem_phy_man_wr(struct c2000_eth_dev *priv, u32 phy_addr, u32 phy_reg, u32 val)
{
        u32 write_data;

        write_data =  0x50020000 | ( (phy_addr & (u32) 0x1f) << 23) | ( (phy_reg & (u32) 0x1f) << 18) | (val & (u32) 0xffff); // write_op
        writel(write_data, priv->phyregisters + EMAC_PHY_MANAGEMENT);

        return 0;
}

/** gem_phy_man_data
 * 	Read the data section of phy management register.
 *    After a successful read opeeration the data will be stored in
 *    in this register in lower 16bits. 
 */
static u32 gem_phy_man_data(struct c2000_eth_dev *priv)
{
	u32 value;

	value = readl(priv->phyregisters + EMAC_PHY_MANAGEMENT) & 0xFFFF;
	return value;
}

#define EMAC_PHY_IDLE   (1 << 2)

static int gem_phy_man_idle(struct c2000_eth_dev *priv)
{
	u32 value;
	
	value = readl(priv->phyregisters + EMAC_NETWORK_STATUS);
	return ((value & EMAC_PHY_IDLE) == EMAC_PHY_IDLE);
}

static int gem_phy_timeout(struct c2000_eth_dev *priv, int timeout)
{
	while(!gem_phy_man_idle(priv)) {
		
		if (timeout-- <= 0) {
			printf("Phy MDIO read/write timeout\n");
			return -1;
		}

		//udelay(1);
	}
	return 0;
}



/** PHY read function
 * Reads a 16bit value from a MII register
 * 
 * @param[in] mdev		Pointer to MII device structure
 * @param[in] phy_addr	
 * @param[in] phy_reg		
 * 
 * @return  16bit value on success, a negivie value (-1) on error
 */
static int c2000_phy_read(struct mii_device *mdev, int phy_addr, int phy_reg)
{
	struct eth_device *edev = mdev->edev;
	struct c2000_eth_dev *priv = (struct c2000_eth_dev *)edev->priv;
	int value;

	if ((phy_addr > MII_ADDRESS_MAX) || (phy_reg > MII_REGISTER_MAX))
		return -1;

	gem_phy_man_rd(priv, phy_addr, phy_reg);
	if (gem_phy_timeout(priv, MDIO_TIMEOUT))
		return -1;

	value = gem_phy_man_data(priv); 

//	dprint("%s: Addr: %d, Reg: %d, Val: %d\n",
//						__func__, phy_addr, phy_reg, value);

	return value;	
}

/** PHY write function 
 * Writes a 16bit value to a MII register
 * 
 * @param[in] mdev		Pointer to MII device structure
 * @param[in] phy_addr
 * @param[in] phy_reg
 * @param[in] value	Value to be written to Phy
 *
 * @return		On success returns 0, a negative value (-1) on error
 */ 
static int c2000_phy_write(struct mii_device *mdev, int phy_addr,
									int phy_reg, int value)
{
	struct eth_device *edev = mdev->edev;
	struct c2000_eth_dev *priv = (struct c2000_eth_dev *)edev->priv;

	if ((phy_addr > MII_ADDRESS_MAX) || (phy_reg > MII_REGISTER_MAX))
		return -1;

	gem_phy_man_wr(priv, phy_addr, phy_reg, value);
	if (gem_phy_timeout(priv, MDIO_TIMEOUT))
		return -1;

//	dprint("%s: Addr: %d, Reg: %d, Val: %d\n",
//						__func__, phy_addr, phy_reg, value);

	return 0;
}

/** MAC Address converter
 * Convert standard byte style ethernet address to format compatible with MAC.
 * 
 * @param[in] enet_byte_addr	Pointer to the mac address in byte format
 * @param[out] Pointer to MAC_ADDR structure
 * 
 * @return	0 on success, -1 on failure 
 */
int gemac_enet_addr_byte_mac(u8 *enet_byte_addr, MAC_ADDR *enet_addr)
{
    if ((enet_byte_addr == NULL) || (enet_addr == NULL))
    {
        return -1;
    }
    else
    {
        enet_addr->bottom = enet_byte_addr[0] |
                            (enet_byte_addr[1] << 8) |
                            (enet_byte_addr[2] << 16) |
                            (enet_byte_addr[3] << 24);
        enet_addr->top = enet_byte_addr[4] |
                         (enet_byte_addr[5] << 8);
        return 0;
    }
}

/** Configures ethernet address to GEMAC ADDR1 location.
 *
 * @param[in] edev	Pointer to the eth device structure
 * @param[in] addr	Pointer to ethernet address in byte order
 *
 * @return  0 on success (always success)
 */
static int c2000_set_ethaddr(struct eth_device *edev, unsigned char *addr)
{
	struct c2000_eth_dev *priv = (struct c2000_eth_dev *)edev->priv;
	MAC_ADDR enet_address = {0x0, 0x0};

	gemac_enet_addr_byte_mac(addr, &enet_address);
	gemac_set_laddr1(priv->gem->gemac_base, &enet_address);

	return 0;
}

/** Get/Read configured ethernet mac address from GEMAC.
 *
 * @param[in] edev	Pointer to eth device structure
 * @param[out] addr	Pointer to ethernet address to be copied 
 *
 * @return	0 on success, always success.
 */
static int c2000_get_ethaddr(struct eth_device *edev, unsigned char *addr)
{
	struct c2000_eth_dev *priv = (struct c2000_eth_dev *)edev->priv;
	MAC_ADDR enet_address = {0x0, 0x0};
	u16 *p = (u16 *)addr;

	enet_address = gem_get_laddr1(priv->gem->gemac_base);
	p[0] = enet_address.bottom & 0xffff;
	p[1] = (enet_address.bottom >> 16) & 0xffff;
	p[2] = enet_address.top & 0xffff;

	return 0;
}

static int c2000_get_gemac_stats_len(struct eth_device *edev)
{
	return  sizeof(GEM_STATS);
}

static void c2000_get_gemac_stats(struct eth_device *edev, u32 *buf)
{
	struct c2000_eth_dev *priv = (struct c2000_eth_dev *)edev->priv;
	u32 *pstat = (u32*)gemac_get_stats(priv->gem->gemac_base);
	int i;
	
	for (i=0; i<(sizeof(GEM_STATS)/4); i++)
		*buf++ = *pstat++;
}

/** SBL ethernet pkt recv function.
 *  - This funciton is called by network stack
 */
static int c2000_eth_recv(struct eth_device *edev)
{
	struct c2000_eth_dev *priv = (struct c2000_eth_dev *)edev->priv;
	u32 pkt_buf;
	int len;
	int phy_port;

	len = pfe_recv(&pkt_buf, &phy_port);

	if (len < 0)
		return 0; //no packet in rx

	dprint("Rx pkt: pkt_buf(%08x), phy_port(%d), len(%d)\n", pkt_buf, phy_port, len);
	if (phy_port != priv->gemac_port)  {
		printf("Rx pkt not on expected port\n");
		return 0; 
	}

	/* send pkt to stack */
	net_receive((unsigned char *)pkt_buf, len);

	return 0;
}

/** SBL ethernet driver xmit function.
 * This function is called by Network stack to send a packet
 *
 * @param[in] edev Pointer to ethernet driver control block
 * @param[in] data Pointer to packet to send
 * @param[in] length Length the packet
 *
 */
static int c2000_eth_send(struct eth_device *edev, void *data, int length)
{
	struct c2000_eth_dev *priv = (struct c2000_eth_dev *)edev->priv;
	int rc;
	uint64_t tx_tmo;

	dprint("%s: iface %s:%d\n", __func__, edev->dev.name, edev->dev.id);

	rc = pfe_send(priv->gemac_port, data, length);

	if (rc < 0) {
		pr_err("Tx Q full\n");
		return 0;
	} 

	/* check tx done */
	tx_tmo = get_time_ns();
	while (1) {
		rc = pfe_tx_done();
		if (rc == 0)
			break;
		if (is_timeout(tx_tmo, 3 * SECOND))  {
			pr_err("Tx timeout, send failed\n");
			break;
		}
	}

	return 0;
}

/** Enables the PFE/GEMAC of specific interface.
 *
 * @param[in] edev	Pointer to eth device structure.
 *
 * @return	0, always success.	
 */
static int c2000_eth_open(struct eth_device *edev)
{
	struct c2000_eth_dev *priv = (struct c2000_eth_dev *)edev->priv;
	struct gemac_s *gem = priv->gem;

	dprint("%s\n", __func__);

	/* Re-negotiate speed and duplex */
	if (!(gem->flags & GEMAC_NO_PHY))
	{
		int speed = gem->gemac_speed;
		int duplex = gem->gemac_duplex;	

		miidev_speed_duplex(&priv->miidev, &speed, &duplex);

		switch (speed)
		{
			case MII_SPEED_10M:
				speed = SPEED_10M;
				break;

			case MII_SPEED_100M:
				speed = SPEED_100M;
				break;
			
			case MII_SPEED_1000M:
				speed = SPEED_1000M;
				break;
			
			case MII_SPEED_1000M_PCS:
				speed = SPEED_1000M_PCS;
				break;
		}
		
		duplex = (duplex == MII_DUPLEX_HALF) ? DUPLEX_HALF:DUPLEX_FULL;
		gemac_set_speed(gem->gemac_base, speed);
		gemac_set_duplex(gem->gemac_base, duplex);
	}

	/* Enable GPI */
	gpi_enable(priv->gem->egpi_base);

	/* Enable GEMAC for tx and rx */
	gemac_enable(priv->gem->gemac_base);

	return 0;
}

/** eth interface init
 * - Most of the pfe init will be done in pfe_probe().
 * - This function initializes specific GEMAC interface.
 *
 * @param[in]	edev	Pointer to eth device structure.
 *
 * @return	0, always success.
 */
static int c2000_eth_init(struct eth_device *edev)
{
	struct c2000_eth_dev *priv = (struct c2000_eth_dev *)edev->priv;
	struct gemac_s *gem = priv->gem;

	dprint("%s:Interface: %s-%d\n", __func__, edev->dev.name, edev->dev.id);

	/* GEMAC init */	
	pfe_gemac_init(gem->gemac_base, gem->gemac_mode, gem->gemac_speed, gem->gemac_duplex);

	/* set ethernet mac address */
	c2000_set_ethaddr(edev, priv->einfo->mac_addr);

	if (!(gem->flags & GEMAC_NO_PHY) && (gem->gemac_mode == RGMII) )
        {
		miidev_enable_rgmii_rx_delay(&priv->miidev);
		miidev_enable_rgmii_tx_delay(&priv->miidev);
		miidev_restart_aneg(&priv->miidev);
	}

	return 0;
}

/** Stops or Disables GEMAC pointing to this eth iface.
 *
 * @param[in]	edev	Pointer to eth device structure.
 *
 * @return	none 
 */
static void c2000_eth_halt(struct eth_device *edev)
{
	struct c2000_eth_dev *priv = (struct c2000_eth_dev *)edev->priv;

	pr_info("%s:\n",__func__);

	gemac_disable(priv->gem->gemac_base);

	gpi_disable(priv->gem->egpi_base);

	return;
}


/** Driver probe function called during board initialization sequence.
 *
 * @param[in]	dev	Pointer device data structure(devined in BSP).
 *
 * @return	0 on success, any negative value indicates failure.
 */
static int c2000_eth_probe(struct device_d *dev)
{
	struct eth_device *edev;
	struct c2000_eth_dev *priv;
	struct pfe *pfe;

	dprint("%s: dev: %s-%d\n",__func__, dev->name, dev->id);

	//Bring HFE and GEMTX out of reset
        writel(0x0, HFE_RESET);
        writel(0x0, GEMTX_RESET);

	if (dev->platform_data == NULL) {
		pr_err("C2000: No platform data\n");
		return -ENODEV;
	}

	edev = (struct eth_device *)xzalloc(sizeof(struct eth_device) + sizeof(struct c2000_eth_dev));
	if (edev == NULL) {
		pr_err("C2000: No memory\n");
		return -ENOMEM;
	}
	dev->type_data = edev;

	edev->priv =  (void *)(edev + 1);

	priv = (struct c2000_eth_dev *)edev->priv;
	priv->einfo = (struct c2000_eth_platform_data *)dev->platform_data;
	pfe = &priv->pfe;

	pfe->cbus_baseaddr = (void *)COMCERTO_AXI_HFE_CFG_BASE;
	pfe->ddr_baseaddr = (void *)CONFIG_DDR_BASEADDR;
	pfe->ddr_phys_baseaddr = (unsigned long)CONFIG_DDR_PHYS_BASEADDR;

	edev->init = c2000_eth_init;
	edev->open = c2000_eth_open;
	edev->send = c2000_eth_send;
	edev->recv = c2000_eth_recv;
	edev->halt = c2000_eth_halt;
	edev->set_ethaddr = c2000_set_ethaddr;
	edev->get_ethaddr = c2000_get_ethaddr;
	edev->get_gemac_stats = c2000_get_gemac_stats;
	edev->get_gemac_stats_len = c2000_get_gemac_stats_len;

	priv->gemac_port = priv->einfo->gemac_port; 
	priv->gem = &gem_info[priv->gemac_port];
	switch(priv->gemac_port)  {
		case EMAC_PORT_0:
		default:
  			priv->gem->gemac_base = EMAC1_BASE_ADDR;
			priv->gem->egpi_base = EGPI1_BASE_ADDR;
		break;
		case EMAC_PORT_1:
  			priv->gem->gemac_base = EMAC2_BASE_ADDR;
			priv->gem->egpi_base = EGPI2_BASE_ADDR;
		break;
		case EMAC_PORT_2:
  			priv->gem->gemac_base = EMAC3_BASE_ADDR;
			priv->gem->egpi_base = EGPI3_BASE_ADDR;
		break;
	}



	pfe_probe(pfe);

	priv->phyregisters = (void *)gem_info[gem_info[priv->gemac_port].phy_reg_index].gemac_base;
	priv->miidev.read = c2000_phy_read;
	priv->miidev.write = c2000_phy_write;
	priv->miidev.address = gem_info[priv->gemac_port].phy_address;
	priv->miidev.flags = 0;
	priv->miidev.edev = edev;

        if(priv->gemac_port == 0)
	{
		gemac_set_mdc_div(priv->gem->gemac_base, MDC_DIV_96);
		gemac_enable_mdio(priv->gem->gemac_base);
		mii_register(&priv->miidev);
	}

	c2000_eth_board_init(priv->gemac_port);
	eth_register(edev);

	return 0;
}

/** Driver remove function, called during shutdown
 * 
 * @param	dev	Poninter to device data structure.
 *
 */
static void c2000_eth_remove(struct device_d *dev)
{
	struct eth_device *edev = dev->type_data;
	struct c2000_eth_dev *priv = (struct c2000_eth_dev *)edev->priv;
	struct pfe *pfe = &priv->pfe;

	pr_info("%s\n", __func__);

	pfe_remove(pfe);

	free(edev);

}

/* Driver description data to register */
static struct driver_d c2000_eth_driver = {
	.name   = "c2000_eth",
	.probe  = c2000_eth_probe,	
	.remove = c2000_eth_remove,
};

/** Register the ethernet driver to the OS
 */
static int c2000_register(void)
{
	register_driver(&c2000_eth_driver);
#if 0
	printk(KERN_INFO "\nPFE DDR map --\n");
	printk(KERN_INFO "      BMU2: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+BMU2_DDR_BASEADDR, BMU2_DDR_SIZE);
	printk(KERN_INFO "TMU queues: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+TMU_LLM_BASEADDR, TMU_LLM_SIZE);
	printk(KERN_INFO "HIF RxPkts: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+HIF_RX_PKT_DDR_BASEADDR, HIF_RX_PKT_DDR_SIZE);
	printk(KERN_INFO "HIF TxPkts: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+HIF_TX_PKT_DDR_BASEADDR, HIF_TX_PKT_DDR_SIZE);
	printk(KERN_INFO "HIF RxDesc: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+HIF_DESC_BASEADDR, HIF_RX_DESC_SIZE);
	printk(KERN_INFO "HIF TxDesc: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+HIF_DESC_BASEADDR+HIF_RX_DESC_SIZE, HIF_TX_DESC_SIZE);
	printk(KERN_INFO "FPPDiagCtl: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+FPPDIAG_CTL_BASE_ADDR, FPPDIAG_CTL_SIZE);
	printk(KERN_INFO "FPPDiag Pg: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+FPPDIAG_PAGE_BASE_ADDR, FPPDIAG_PAGE_TOTAL_SIZE);
	printk(KERN_INFO " Util code: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+UTIL_CODE_BASEADDR, UTIL_CODE_SIZE);
	printk(KERN_INFO " Util data: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+UTIL_DDR_DATA_BASEADDR, UTIL_DDR_DATA_SIZE);
	printk(KERN_INFO "Class data: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+CLASS_DDR_DATA_BASEADDR, CLASS_DDR_DATA_SIZE);
	printk(KERN_INFO "  TMU data: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+TMU_DDR_DATA_BASEADDR, TMU_DDR_DATA_SIZE);
	printk(KERN_INFO " Route tbl: 0x%7x  size=0x%x\n", CONFIG_DDR_PHYS_BASEADDR+ROUTE_TABLE_BASEADDR, ROUTE_TABLE_SIZE);
	printk(KERN_INFO "Total Data Size = 0x%x\n\n", PFE_TOTAL_DATA_SIZE);
#endif
	return 0;
}
device_initcall(c2000_register);
