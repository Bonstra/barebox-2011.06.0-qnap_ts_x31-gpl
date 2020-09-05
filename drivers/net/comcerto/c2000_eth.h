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

#ifndef _C2000_ETH_H_
#define _C2000_ETH_H_


#include "pfe_driver.h"

#ifndef SZ_1K
#define SZ_1K 1024
#endif

#ifndef SZ_1M
#define SZ_1M (1024 * 1024)
#endif

#define BMU2_DDR_BASEADDR	0
#define BMU2_BUF_COUNT		(3 * SZ_1K)
#define BMU2_DDR_SIZE		(DDR_BUF_SIZE * BMU2_BUF_COUNT)

#define TMU_LLM_BASEADDR	(BMU2_DDR_BASEADDR + BMU2_DDR_SIZE)
#define TMU_LLM_QUEUE_LEN	(16 * 256)			/**< Must be power of two and at least 16 * 8 = 128 bytes */
#define TMU_LLM_SIZE		(4 * 16 * TMU_LLM_QUEUE_LEN)	/**< (4 TMU's x 16 queues x queue_len) */

#define HIF_RX_PKT_DDR_BASEADDR (TMU_LLM_BASEADDR + TMU_LLM_SIZE)
#define HIF_RX_PKT_DDR_SIZE     (HIF_RX_DESC_NT * DDR_BUF_SIZE)
#define HIF_TX_PKT_DDR_BASEADDR (HIF_RX_PKT_DDR_BASEADDR + HIF_RX_PKT_DDR_SIZE)
#define HIF_TX_PKT_DDR_SIZE     (HIF_TX_DESC_NT * DDR_BUF_SIZE)

#define HIF_DESC_BASEADDR       (HIF_TX_PKT_DDR_BASEADDR + HIF_TX_PKT_DDR_SIZE)
#define HIF_RX_DESC_SIZE        (16*HIF_RX_DESC_NT)
#define HIF_TX_DESC_SIZE        (16*HIF_TX_DESC_NT)
#define HIF_DESC_SIZE           (HIF_RX_DESC_SIZE + HIF_TX_DESC_SIZE)

//#define FPPDIAG_CTL_BASE_ADDR	(HIF_DESC_BASEADDR + HIF_DESC_SIZE)
#define FPPDIAG_CTL_BASE_ADDR	0x700000
#define FPPDIAG_CTL_SIZE		256	/**< Must be at least 11*8 bytes */
#define FPPDIAG_PAGE_BASE_ADDR	(FPPDIAG_CTL_BASE_ADDR + FPPDIAG_CTL_SIZE)
#define FPPDIAG_PAGE_TOTAL_SIZE	(11 * 256) /**< 256 bytes per PE, 11 PEs */

//#define UTIL_CODE_BASEADDR	(FPPDIAG_PAGE_BASE_ADDR + FPPDIAG_PAGE_TOTAL_SIZE)
#define UTIL_CODE_BASEADDR	0x780000
#define UTIL_CODE_SIZE		(128 * SZ_1K)

#define UTIL_DDR_DATA_BASEADDR	(UTIL_CODE_BASEADDR + UTIL_CODE_SIZE)
#define UTIL_DDR_DATA_SIZE	(64 * SZ_1K)

#define CLASS_DDR_DATA_BASEADDR	(UTIL_DDR_DATA_BASEADDR + UTIL_DDR_DATA_SIZE)
#define CLASS_DDR_DATA_SIZE	(32 * SZ_1K)

#define TMU_DDR_DATA_BASEADDR	(CLASS_DDR_DATA_BASEADDR + CLASS_DDR_DATA_SIZE)
#define TMU_DDR_DATA_SIZE	(32 * SZ_1K)

//#define ROUTE_TABLE_BASEADDR	(TMU_DDR_DATA_BASEADDR + TMU_DDR_DATA_SIZE)
#define ROUTE_TABLE_BASEADDR	0x800000
#define ROUTE_TABLE_HASH_BITS_MAX	15	/**< 32K entries */
#define ROUTE_TABLE_HASH_BITS	8	/**< 256 entries */
#define ROUTE_TABLE_SIZE	((1 << ROUTE_TABLE_HASH_BITS_MAX) * CLASS_ROUTE_SIZE)

#define	PFE_TOTAL_DATA_SIZE	(ROUTE_TABLE_BASEADDR + ROUTE_TABLE_SIZE)

#if PFE_TOTAL_DATA_SIZE > (12 * SZ_1M)
#error DDR mapping above 12MiB
#endif

/* LMEM Mapping */
#define BMU1_LMEM_BASEADDR	0
#define BMU1_BUF_COUNT		256
#define BMU1_LMEM_SIZE		(LMEM_BUF_SIZE * BMU1_BUF_COUNT)


#define CONFIG_DDR_PHYS_BASEADDR 0x03800000
#define CONFIG_DDR_BASEADDR      CONFIG_DDR_PHYS_BASEADDR

#if 0

#define RX_PKT_DDR_BASEADDR  0x300000   //4 pkts of each 2k size
#define TX_PKT_DDR_BASEADDR  0x302000  //4 pkts of each 2K size

#define RX_BD_BASEADDR       0x214000 
#define TX_BD_BASEADDR       0x214200 

#define BMU1_LMEM_BASEADDR   0
#define BMU1_BUF_COUNT       32

#define BMU2_BUF_COUNT       32
#define BMU2_DDR_BASEADDR    0x200000


#define ROUTE_TABLE_BASEADDR	0x215000 
#define ROUTE_TABLE_HASH_BITS	2	/**< 2 entries */

#define TMU_LLM_BASEADDR   0x216000 
#define TMU_LLM_QUEUE_LEN  (16 * 256)

#endif

#define GEMAC_NO_PHY 		1
#define GEMAC_HAVE_SWITCH_PHY     2
#define GEMAC_HAVE_SWITCH       	4


typedef struct gemac_s {

	void *gemac_base; 
	void *egpi_base;

	/* GEMAC config */
	int gemac_mode;
	int gemac_speed;
	int gemac_duplex;
        int flags;
	/* phy iface */
	int phy_reg_index;
	int phy_address;

} gemac_t;

struct pfe {
	unsigned long ddr_phys_baseaddr;
	void *ddr_baseaddr;
	void *cbus_baseaddr;
};


typedef struct c2000_eth_dev {

	int gemac_port;
	void *phyregisters;

	struct gemac_s *gem;
	struct pfe      pfe;

	struct mii_device miidev;

	struct c2000_eth_platform_data *einfo;

} c2000_eth_dev_t;


struct firmware {
	u8 *data;
};


int pfe_probe(struct pfe *pfe);
int pfe_remove(struct pfe *pfe);


//#define dprint(fmt, arg...)	pr_info(fmt, ##arg)
#define dprint(fmt, arg...)
//#define dprint(fmt, arg...)	pr_debug(fmt, ##arg)


#endif //_C2000_ETH_H_

