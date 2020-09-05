
/*
 * diags/test_pcie.c
 *
 * Copyright (C) 2004,2005 Mindspeed Technologies, Inc.
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
#include <types.h>
#include <common.h>
#include <asm/io.h>
#include <mach/comcerto-2000.h>
#include <mach/pcie.h>
#include <mach/serdes.h>
#include <mach/clkcore.h>
#include <mach/gpio.h>
#include <diags.h>
#include "test_pcie.h"
#include "tests.h"
#include "dump.h"
#include "common_func.h"
#include "serdes_common.h"
#include "PCIeMacro.h"
#include "PCIe.c"

#define U8 u8

#define PCI_VENDOR_ID		0x00	/* 16 bits */
#define PCI_DEVICE_ID		0x02	/* 16 bits */
#define PCI_BASE_ADDRESS_0	0x10	/* 32 bits */

/* DWC PCIEe configuration register offsets on APB */
struct pcie_app_reg app_regs[NUM_PCIE_PORTS] = {
	//PCIe0
	{ 0x00000000,
	  0x00000004,
	  0x00000008,
	  0x0000000C,
	  0x00000010,
	  0x00000014,
	  0x00000018,
	  0x00000040,
	  0x00000044,
	  0x00000048,
	  0x00000058,
	  0x00000080,
	  0x00000084,
	  0x000000C0,
	  0x000000C4,
	  0x00000100,
	  0x00000104,
	  0x00000108,
	  0x0000010C
	},
	//PCIe1
	{ 0x00000020,
	  0x00000024,
	  0x00000028,
	  0x0000002C,
	  0x00000030,
	  0x00000034,
	  0x00000038,
	  0x0000004C,
	  0x00000050,
	  0x00000054,
	  0x0000005C,
	  0x00000088,
	  0x0000008C,
	  0x000000C8,
	  0x000000CC,
	  0x00000110,
	  0x00000114,
	  0x00000118,
	  0x0000011C
	}
};

/* Keeping all DDR area of 256MB accesible for inbound transaction */
#define INBOUND_ADDR_MASK	0xFFFFFFF


#define PCIE_SETUP_iATU_IB_ENTRY( _pp, _view_port, _base, _limit, _ctl1, _ctl2, _target ) \
{\
	comcerto_dbi_write_reg(_pp, PCIE_iATU_VIEW_PORT, 4, (u32)(_view_port|iATU_VIEW_PORT_IN_BOUND)); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_CTRL2, 4, 0); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_SRC_LOW, 4, (u32)_base); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_SRC_HIGH, 4, 0); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_LIMIT, 4, (u32)_limit); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_TRGT_LOW, 4, (u32)_target); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_TRGT_HIGH, 4, (u32)0); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_CTRL1, 4, (u32)_ctl1); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_CTRL2, 4, (u32)(_ctl2 |iATU_CTRL2_ID_EN) ); \
}

#define PCIE_SETUP_iATU_OB_ENTRY( _pp, _view_port, _base, _limit, _ctl1, _ctl2, _target ) \
{\
	comcerto_dbi_write_reg(_pp, PCIE_iATU_VIEW_PORT, 4, (u32)_view_port); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_CTRL2, 4, 0); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_SRC_LOW, 4, (u32)_base); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_SRC_HIGH, 4, (u32)0); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_LIMIT, 4, (u32)_limit); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_TRGT_LOW, 4, (u32)_target); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_TRGT_HIGH, 4, (u32)0); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_CTRL1, 4, (u32)_ctl1); \
	comcerto_dbi_write_reg(_pp, PCIE_iATU_CTRL2, 4, (u32)(_ctl2 |iATU_CTRL2_ID_EN) ); \
}

//#define MAX_LINK_UP_WAIT_JIFFIES	100
#define MAX_LINK_UP_WAIT_JIFFIES	0xfff
#define PCI_SLOT(d)         (((d) >> 3) & 0x1f)
#define PCI_FUNC(d)	(((d) >> 8) & 0x7)

static struct pcie_port pcie_port[NUM_PCIE_PORTS];

static int initialized[NUM_PCIE_PORTS] = {0, 0};

/**
 * This function checks whether link is up or not.
 * Returns true if link is up otherwise returns false.
 * @param pp	Pointer to PCIe Port control block.
 */
static int comcerto_pcie_link_up( struct pcie_port *pp  )
{
	unsigned long timeout = MAX_LINK_UP_WAIT_JIFFIES;

	do {
		if (readl( pp->app_base + pp->app_regs->sts0 ) & STS0_RDLH_LINK_UP) {
			printf("PCIe Link up: Ok\n");
			return 1;
		}

		mdelay(1);
	} while (timeout--);

	printf("PCIe Link up: failed\n");

	return 0;
}

/**
 * This function is used to read DBI registers.
 */

static void comcerto_dbi_read_reg(struct pcie_port *pp, int where, int size,
		u32 *val)
{
	u32 address;

	address = (u32)pp->base + (where & ~0x3);

	*val = readl(address);

	if (size == 1)
		*val = (*val >> (8 * (where & 3))) & 0xff;
	else if (size == 2)
		*val = (*val >> (8 * (where & 3))) & 0xffff;
}

/**
 * This function is used to write into DBI registers.
 */
static void comcerto_dbi_write_reg(struct pcie_port *pp, int where, int size,
		u32 val)
{
	u32 address;
	int pos, val1, mask = 0;

	//printk("%s: start\n", __func__);

	address = (u32)pp->base + (where & ~0x3);

	pos = (where & 0x3) << 3;

	if (size == 4)
		val1 = val;
	else
	{
		if (size == 2)
			mask = 0xffff;
		else if (size == 1)
			mask = 0xff;

		val1 = readl(address);
		val1 = ( val1 & ~( mask  << pos ) ) | ( (val & mask) << pos );
	}

	//printk("%s: address: %08x, val: %08x\n", __func__, address, val1);

	writel(val1, address);

	//printk("%s: done\n", __func__);
}

static int comcerto_pcie_rd_conf(struct pcie_port *pp, int bus_nr,
		u32 devfn, int where, int size, u32 *val)
{
 	u32 address;
	u32 target_address = (u32)(bus_nr << 24) | (PCI_SLOT(devfn) << 19) | (PCI_FUNC(devfn) << 16);

	//Initialize iATU
	if (bus_nr != pp->root_bus_nr) {
		/* Type1 configuration request */
		PCIE_SETUP_iATU_OB_ENTRY( pp, iATU_ENTRY_CNF1, (u32)pp->cfg1_base,
				((u32)(pp->cfg1_base + (iATU_CFG1_SIZE - 1))),
				(AXI_OP_TYPE_CONFIG_RDRW_TYPE1 & iATU_CTRL1_TYPE_MASK),
				0, target_address );

		address = (u32)pp->cfg1_base |(where & 0xFFFC);
	} else {
		/* Type0 configuration request */
		PCIE_SETUP_iATU_OB_ENTRY( pp, iATU_ENTRY_CNF0, (u32)pp->cfg0_base,
				((u32)(pp->cfg0_base + (iATU_CFG0_SIZE - 1))),
				(AXI_OP_TYPE_CONFIG_RDRW_TYPE0 & iATU_CTRL1_TYPE_MASK),
				0, target_address );
		address = (u32)((pp->cfg0_base)|(where & 0xFFFC));
	}

	//printf("%s: %08x\n", __func__, address);

	*val = readl(address);
	if (size == 1)
		*val = (*val >> (8 * (where & 3))) & 0xff;
	else if (size == 2)
		*val = (*val >> (8 * (where & 3))) & 0xffff;

	return 0;
}

static int pcie_read_conf(struct pcie_port *pp, int bus_nr, u32 devfn, int where, int size, u32 *val)
{
	int ret;

 	/* Make sure that link is up.
	 * Filter device numbers, unless it's a type1 access
	 */
	if ( (!pp->link_state)||
			((bus_nr == pp->root_bus_nr) && (PCI_SLOT(devfn) > 0)) ) {
		*val = 0xffffffff;
		return -1;
	}

	BUG_ON (((where & 0x3) + size) > 4);

	ret = comcerto_pcie_rd_conf(pp, bus_nr, devfn, where, size, val);

	return ret;
}

static int comcerto_pcie_wr_conf(struct pcie_port *pp, int bus_nr,
		u32 devfn, int where, int size, u32 val)
{
	int ret = 0;
	u32 address;
	u32 target_address = (u32)(bus_nr << 24) | (PCI_SLOT(devfn) << 19) | (PCI_FUNC(devfn) << 16);

	//Initialize iATU
	if (bus_nr != pp->root_bus_nr) {
		/* Type1 configuration request */
		PCIE_SETUP_iATU_OB_ENTRY( pp, iATU_ENTRY_CNF1, (u32)pp->cfg1_base,
				((u32)(pp->cfg1_base + (iATU_CFG1_SIZE - 1))),
				(AXI_OP_TYPE_CONFIG_RDRW_TYPE1 & iATU_CTRL1_TYPE_MASK),
				0, target_address );

		address = (u32)pp->cfg1_base |(where & 0xFFFC);
	} else {
		/* Type0 configuration request */
		PCIE_SETUP_iATU_OB_ENTRY( pp, iATU_ENTRY_CNF0, (u32)pp->cfg0_base,
				((u32)(pp->cfg0_base + (iATU_CFG0_SIZE - 1))),
				(AXI_OP_TYPE_CONFIG_RDRW_TYPE0 & iATU_CTRL1_TYPE_MASK),
				0, target_address );
		address = (u32)pp->cfg0_base |(where & 0xFFFC);
	}


	if (size == 4)
		writel(val, address);
	else if (size == 2)
		writew(val, address + (where & 2));
	else if (size == 1)
		writeb(val, address + (where & 3));
	else
		ret = -1;

	return ret;
}

static int pcie_write_conf(struct pcie_port *pp, int bus_nr, u32 devfn, int where, int size, u32 val)
{
	int ret;

 	/* Make sure that link is up.
	 * Filter device numbers, unless it's a type1 access
	 */
	if ( (!pp->link_state)||
			((bus_nr == pp->root_bus_nr) && (PCI_SLOT(devfn) > 0)) ) {
		return -1;
	}

	BUG_ON (((where & 0x3) + size) > 4);

	ret = comcerto_pcie_wr_conf(pp, bus_nr, devfn, where, size, val);

	return ret;
}



static void __init pcie_port_init_rc(int port)
{
	struct pcie_port *pp = &pcie_port[port];

	//printf("%s: start \n", __func__);

	/* Fixme : Added Venkat */
	writel((readl(pp->base + 0x70C) & ~(0x00ffff00))| 0x00F1F100,  pp->base + 0x70C);
	writel((readl(pp->base + 0x80C) & ~(0xff))|0xF1,  pp->base + 0x80C);

	/*setup iATU for outbound translation */
	PCIE_SETUP_iATU_OB_ENTRY( pp, iATU_ENTRY_MEM, iATU_GET_MEM_BASE(pp->remote_mem_baseaddr),
					iATU_MEM_SIZE - 1, 0, 0, pp->remote_mem_baseaddr );

	/*setup iATU for inbound translation */
	comcerto_dbi_write_reg(pp, PCI_BASE_ADDRESS_0, 4, 0);
	PCIE_SETUP_iATU_IB_ENTRY( pp, 0, 0,
				INBOUND_ADDR_MASK, 0, (iATU_CTRL2_IB_MEM_BAR_MATCH |
					iATU_CTRL2_IB_MATCH_BAR0), COMCERTO_AXI_ACP_BASE );


	//FIXME : Bit:27 definition is not clear from document
	//	  This register setting is copied from simulation code.
	writel(readl(pp->app_base + pp->app_regs->cfg0) | 0x08007FF0,
					pp->app_base + pp->app_regs->cfg0);


	//FIXME : Bit:15 definition is not clear from document
	//	  This register setting is copied from simulation code.
	writel(readl(pp->app_base + pp->app_regs->cfg4) | 0x00008000,
					pp->app_base + pp->app_regs->cfg4);

	writel(readl(pp->app_base + pp->app_regs->cfg5) | (CFG5_APP_INIT_RST | CFG5_LTSSM_ENABLE),
					pp->app_base + pp->app_regs->cfg5);

	//printf("%s: done \n", __func__);
}

static unsigned long pcie_cnf_base_addr[NUM_PCIE_PORTS] =
		{ COMCERTO_AXI_PCIe0_CFG_BASE, COMCERTO_AXI_PCIe1_CFG_BASE };
static unsigned long pcie_remote_base_addr[NUM_PCIE_PORTS] =
		{ COMCERTO_AXI_PCIe0_SLV_BASE, COMCERTO_AXI_PCIe1_SLV_BASE };


static int comcerto_pcie_init( struct pcie_port *pp, int nr)
{
	if (nr >= NUM_PCIE_PORTS) {
		printf("%s : Invalid PCIe port number\n", __func__);
		return 1;
	}

	memset(pp, 0, sizeof(struct pcie_port));

	pp->app_regs = &app_regs[nr];
	pp->root_bus_nr = nr;
	pp->base = pcie_cnf_base_addr[nr];
	pp->app_base = (unsigned long)COMCERTO_PCIE_SATA_USB_CTRL_BASE;
	pp->remote_mem_baseaddr = pcie_remote_base_addr[nr];
	pp->cfg0_base = iATU_GET_CFG0_BASE(pp->remote_mem_baseaddr);
	pp->cfg1_base = iATU_GET_CFG1_BASE(pp->remote_mem_baseaddr);

	if (!nr) {
	     	//writel( readl( AXI_RESET_2 ) | PCIE0_AXI_RST, AXI_RESET_2 );
     		writel( readl( AXI_RESET_2 ) & ~PCIE0_AXI_RST, AXI_RESET_2 );

     		//writel( readl( SERDES_RST_CNTRL ) | (SERDES0_RST), SERDES_RST_CNTRL );
     		writel( readl( SERDES_RST_CNTRL ) & ~(SERDES0_RST), SERDES_RST_CNTRL );


		//step-3
		/* Put PCIE0 and PCIe1 into reset */
		//FIXME : This code is copied from simulation samples,
		//        Making reset bits to 1 might not be required
		//        Need to be cleanup after tested.
     		//writel( readl( PCIe_SATA_RST_CNTRL ) | (PCIE0_PWR_RST|PCIE0_REG_RST), PCIe_SATA_RST_CNTRL );
		writel( readl( PCIe_SATA_RST_CNTRL ) & ~(PCIE0_PWR_RST|PCIE0_REG_RST), PCIe_SATA_RST_CNTRL );


		/* Configure PCIe0 to Root complex mode */
		writel( (readl(pp->app_base + pp->app_regs->cfg0) &
					~DWC_CFG0_DEV_TYPE_MASK) | PCIE_PORT_MODE_RC,
					pp->app_base + pp->app_regs->cfg0);

		/* Enable clocks */
		//FIXME : No clock configurtaion register found for SerDes.
		//        Need to discuss with VLSI team.
		//FIXME : By default PCIe is configured in Gen2 mode. Need to
		//        verify if any setting required to switch to Gen1 mode.
		writel( readl( AXI_CLK_CNTRL_2 )| AXI_PCIE0_CLK_EN,  AXI_CLK_CNTRL_2);

		pp->port_mode = PCIE_PORT_MODE_RC;
	}else {
		if ( !(readl(COMCERTO_GPIO_SYSTEM_CONFIG) & BOOTSTRAP_SERDES1_CNF_SATA0) ) {
			u32 ctrl_reg;

			printf("%s: Boot strap configured for PCIe1\n",__func__);

     			//writel( readl( AXI_RESET_2 ) | PCIE1_AXI_RST, AXI_RESET_2 );
     			writel( readl( AXI_RESET_2 ) & ~PCIE1_AXI_RST, AXI_RESET_2 );

     			//writel( readl( SERDES_RST_CNTRL ) | (SERDES1_RST), SERDES_RST_CNTRL );
     			writel( readl( SERDES_RST_CNTRL ) & ~(SERDES1_RST), SERDES_RST_CNTRL );

			//FIXME : This code is copied from simulation samples,
			//        Making reset bits to 1 might not be required
			//        Need to be cleanup after tested.
			//writel( readl( PCIe_SATA_RST_CNTRL ) | (PCIE1_PWR_RST|PCIE1_REG_RST), PCIe_SATA_RST_CNTRL );
			writel( readl( PCIe_SATA_RST_CNTRL ) & ~(PCIE1_PWR_RST|PCIE1_REG_RST), PCIe_SATA_RST_CNTRL );
			/* Configure PCIe1 to Root complex mode */
			writel( (readl(pp->app_base + pp->app_regs->cfg0) &
					~DWC_CFG0_DEV_TYPE_MASK ) | PCIE_PORT_MODE_RC,
					pp->app_base + pp->app_regs->cfg0);

			pp->port_mode = PCIE_PORT_MODE_RC;

			//FIXME: MOULI
			//As per VLSI code sequence for Serdes-1
			ctrl_reg = COMCERTO_SERDES_DWC_CFG_REG( 1, SD_PHY_CTRL3_REG_OFST );
			writel(0xFF3C, ctrl_reg);

			ctrl_reg = COMCERTO_SERDES_DWC_CFG_REG( 1, SD_PHY_CTRL2_REG_OFST );
			writel(readl(ctrl_reg) & ~0x3, ctrl_reg);
			writel( readl( AXI_CLK_CNTRL_2 )| AXI_PCIE1_CLK_EN,  AXI_CLK_CNTRL_2);
		}
		else {
			pp->port_mode = PCIE_PORT_MODE_NONE;
			return 1;
		}
	}


	//Serdes Initialization.
	if( serdes_phy_init(nr,  pcie_phy_reg_file,
			sizeof(pcie_phy_reg_file) / sizeof(serdes_regs_t),
						SD_DEV_TYPE_PCIE) )
	{
		pp->port_mode = PCIE_PORT_MODE_NONE;
		pr_err("%s: Failed to initialize serdes (%d)\n", __func__, nr );
		return 1;
	}

	//Synopsys PCIe core initialization.
       	pcie_port_init_rc(nr);

	pp->link_state = comcerto_pcie_link_up( &pcie_port[nr] );

	return 0;
}



int comcerto_pcie_port_test (unsigned int pcie_host_nr)
{
	u32 value;
        struct pcie_port *pp;
        unsigned int addr = PCI_VENDOR_ID;
        unsigned int devfn = 0;


	if (pcie_host_nr >= NUM_PCIE_PORTS) {
		printf("%s : Invalid port number (%d)\n", __func__, pcie_host_nr);
		return 1;
	}

        value = 0xffffffff;

	printf("\nPCIe%d Root complex test:\n", pcie_host_nr);

	pp = &pcie_port[pcie_host_nr];

	if (!initialized[pcie_host_nr])
	{
		if ( comcerto_pcie_init(pp, pcie_host_nr) ) {
			printf("comcerto_pcie_init failed\n");
			return 1;
		}

		initialized[pcie_host_nr] = 1;
	}

	printf("\n==============PCIE%d TEST RESULT==============\n", pcie_host_nr);


	if ( pp->port_mode == PCIE_PORT_MODE_NONE ) {
		printf("PCIe%d controller is not enabled.........!!!\n", pcie_host_nr);
		return 1;
	}


	if ( ( pp->port_mode == PCIE_PORT_MODE_RC ) && ( pp->link_state ) )
	{
		pcie_read_conf(pp, pcie_host_nr, devfn, addr, 0x4, &value);

		if(value != 0xffffffff)
               	{
                       	printf("PCIe Lane %d: Connected Device Vendor_ID: [0x%04X] Device_ID: [0x%04X] .... TEST PASS\n", pcie_host_nr, (value & 0xFFFF), ((value >> 16) & 0xFFFF));

               	}
		else
		{
                       	printf("PCIe Lane %d: Vendor and Device Id not found for the device.....TEST FAIL\n", pcie_host_nr);
			return 1;
               	}
       	}
	else
	{
		printf("PCIe Lane %d: Link Down Or Controller is not Root Complex......... TEST FAIL\n", pcie_host_nr);
		return 1;
       	}

	return 0;
}

int comcerto_pcie_rc_test (struct diags_test_param *p)
{
	int rc0, rc1;

	rc0 = comcerto_pcie_port_test (0);
	rc1 = comcerto_pcie_port_test (1);

	return rc0 | rc1;
}

int comcerto_pcie_ep_test (struct diags_test_param *p)
{
	printf("\n PCIE endpoint diags is not yet supported ....\n");
	return 0;
}





#define SERDES_RST_CNTRL    0x904B0004
#define PCIE_SATA_RST_CNTRL 0x904B0008

#define GPIO_BASE 0x90470000

// MDMA stuff
#define  MDMA_BASE 0x905E0000
#define  S_frame_1 0x40002000 // Source 1 Frame
#define  S_data_1  0x40006000 // Source Data1 Buffer
#define  D_frame_1 0x20014000 // Destination 1 Frame
// copy to PCIE
#define  D_data_1 0xA0000000 // Destination 1 Frame

//#define LOOPBACK_DEST 0X83000000
#define LOOPBACK_DEST 0X70000000
#define MAX_DATA 0X2000 // make it 32K/4
//#define MAX_DATA 0X0400 // make it
// MAX number of DMA TRANSFER ITERNATIONS
#define MAX_DATA_MASK 0x01FFF // number of bits for max data divided by 4
#define RANDOM_DATA_SIZE 1
#define MAX_ITER 100000

#define DEBUG 0

// macro to generate LSFR data
// linear shift feedback register to generate psaudo random numbers to fill initial data
#define LSFR( X ) (X << 1 | (X >> 10 & (((X >> 10) & 1) ^((X >> 3) & 1) ^ ((X >> 0) & 1))))
// random input dwords
//U32 input_dw[3] = {0xab9304c2, 0xf02cb631,0xa2b3c910};

//U32 random_size_cur = 0xab9304c2; //seed for LSFR random
U32 random_size_cur = 0xab930002; //seed for LSFR random
U32 random_min_size = MAX_DATA_MASK;
U32 random_max_size = 0;

U32 p_mode=1;   // 1 for random, 2 for counter, 3 for fixed pattern (0xA5A5A5A5)
U32 rd_data, data_pattern = 0xA5A5A5A5;
U32 input_data_counter = 0;
U32 output_data_counter = 0;
U32 total_errors = 0;
U8 gen1_only = 0;
U32 tlp_size_v = 0x2000; // divided by 4

int comcerto_pcie_perf_test (struct diags_test_param *p)
{
	int ret = -1;
 // Offset Variable
 U32 offset = 0x60;
 U32 i, j, k,temp, lsfr_data, max_iter_v;
 U32 rd_data;
 U32 check_bit;
 U32 SD0_Ctrl_Reg_offset = 0x34;
 U32 SD1_Ctrl_Reg_offset = 0x44;
 U32 SD1_Ctrl_Reg_Sel_offset = 0x48;
 U32 CMU_Reset_mask = 0x00010000;
 U32 Lane_Reset_mask = 0x00000040;
 U32 ck_soc_div_mask = 0x00000003;
 U8 lanedone = 0;

// ######################################################################################
// Prompt user for Test parameters
// ######################################################################################

printf("########################################\n");
printf("PCIE loopback test, you need to plug external card for this test.\n");
printf("PCIE loopback test, PCIe Gen2/Gen1 support\n");
gen1_only = 0;
//printf("PCIE loopback test, Enter below.\n 1: PCIe Gen1 only support, \n 2: PCIe Gen2/Gen1 support\n");
//scanf("%d", & temp);
//if(temp==1)
//    gen1_only = 1;
//else
//   gen1_only = 0;

/*
 printf("PCIE loopback test, Enter below.\n 1:24Mhz clock, \n2: 48Mhz clock, \n 3: 100MHz clock\n");
scanf("%d", & temp);
if(temp==1)
    clock_24Mhz = 1;
else if (temp==2)
    clock_48Mhz = 1;
 */

printf("Number of Iterations is 10,000; about two minutes run\n");
max_iter_v = 10000;
//printf("Enter number of Iterations, 10,000 is about two minutes run\n");
//scanf("%d", & temp);
//if (temp > 0){
//        max_iter_v = temp;
//        printf("Number of iterations is changed to %d\n", max_iter_v);
//    }
//    else{
//        max_iter_v = 10000;
//        printf("You entered invalid value, using iterations %d\n", max_iter_v);
//    }

printf("###################### Now the test will run ########################################\n");

//#######################################################################################
  // reset pcie card
  printf("reset mpcie 1\n");
  reg_rmw((GPIO_BASE+ 0x0),0x08000000, 0x08000000);
  // output enable
  reg_rmw((GPIO_BASE+ 0x4),0x08000000, 0x08000000);
  nop(400000);
  printf("reset mpcie 0\n");
  reg_rmw((GPIO_BASE+ 0x0),0x08000000, 0x00000000);
  nop(400000);
  printf("reset mpcie 1\n");
  reg_rmw((GPIO_BASE+ 0x0),0x08000000, 0x08000000);
  nop(400000);

 // Making PCIE #0 serdes active
 reg_rmw(SERDES_RST_CNTRL, 0x00000001, 0x00000000 );

 //Making PCIE Controller active

 reg_rmw(PCIE_SATA_RST_CNTRL, 0x00000003, 0x00000000 );

//===========================================================
//            change below to force Gen1 or Gen2
 //  turn on support for Gen1 only

 if (gen1_only){
 printf("Force to Gen1, Writing to link control 2 0x1\n");
 reg_rmw(PCIE0_LCNT2_REG, 0x0000000f, 0x1);
 reg_rmw(PCIE0_LCAP_REG, 0x0000000f, 0x1);
 }
 // instruct pcie to switch to gen2 after init
 // set bit 17=1 (page 824)
 if (gen1_only==0){
 reg_rmw(PCIE0_G2CTRL_REG,0x00020000, 0x00020000);
 }
//===============================================================
 //Configure PCIEO as RC
 reg_wr(DWC_CFG_REG0, 0x08007ff4);
 reg_wr(DWC_CFG_REG4, 0x00008000);
 reg_wr(DWC_CFG_REG5, 0x00000206);

 // ####################### init pcie card is moved to inside sbphy_init* in serdes_common.h
 // // To take the external card out of reset configure GPIO27 to be an output (to drive 0)
 // //Configure  GPIO  output Enable
 // reg_rmw((GPIO_BASE+ 0x0),0x08000000, 0x0);
 // // 0x4 is for output enable
 // reg_rmw((GPIO_BASE+ 0x4),0x08000000, 0x08000000);
 // // Configure  GPIO Output register---GPIO_OUT  to be 1
 // // Making   WifiCard reset is Active High to Active WiFiCard
 // //reg_rmw((GPIO_BASE+ 0x0),0x08000000, 0x08000000); //dror

 //Serdes  initilzation happens here
 sbphy_init_wait_lane_rdy(true, false, false, 0, 0, false, false, true);

    // bhimsens setup hy settings
    //reg_wr((SD0_CFG_BASE+ (0x206 << 2)), 0xC4);
    //reg_wr((SD0_CFG_BASE+ (0x207 << 2)), 0x34);
    //reg_wr((SD0_CFG_BASE+ (0x250 << 2)), 0xF6);
    //reg_wr((SD0_CFG_BASE+ (0x251 << 2)), 0x07);

    // new settings for tx_lev
    //printf("TEST: %D: setting reg 0x207 of SD0 at 0x%X  \n", k, (tx_lev_values_rev[k]<<2));
    //reg_wr((SD0_CFG_BASE+ (0x207 << 2)), (tx_lev_values_rev[k]<<2) );

 // wait for serdes to get ready
 // wait_sb_cmu_lane_rdy(0, true, false, true);

 //Ack Frequency and L0-L1 ASPM Control Register
// rd_data = reg_rd(PCIE0_AFL0L1_REG);
// printf("A=0x%x d=0x%x \n", PCIE0_AFL0L1_REG, rd_data);

reg_rmw( PCIE0_AFL0L1_REG , 0x00FFFF00 , 0x00F1F100 );

 //Gen2 Control Control Register
// rd_data = reg_rd(PCIE0_G2CTRL_REG);
// printf("A=0x%x d=0x%x \n", PCIE0_G2CTRL_REG, rd_data);
 reg_rmw( PCIE0_G2CTRL_REG , 0x000000FF , 0x000000F1 );

 //Symbol Number Register
//rd_data = reg_rd(PCIE0_SYMNUM_REG);
//printf("Addr = 0x%x rd=0x%x \n", PCIE0_SYMNUM_REG, rd_data);

 printf("check link status!\n");
 PCIE0_link_up_chk();
 printf("Link is UP!!!\n");

 //Register Initialization (MEM_SPACE_EN_Enable + BUS_MASTER_EN_Enable + CAP_LIST_Enable)
 reg_wr(PCIE0_CMD_REG, 0x00100006);

 //printf("ATU programming!\n");
 // Outbound Programming  to generate Configuration trnasctions
// reg_wr(PCIE0_iATU_VIEW_REG, 0x00000000);
 /* Set Region Base and Limit */
 //reg_wr(PCIE0_iATU_RLBA_REG, 0xA0000000); //PCIE Base Address
 //reg_wr(PCIE0_iATU_RLIA_REG, 0xA00000FF); //PCIE Limit Address
 /* Set Target Address to 0x00000000 */
 //reg_wr(PCIE0_iATU_RLTA_REG, 0x01000000);
 /* Set Control1 Register : Region to be CFG */
 //reg_wr(PCIE0_iATU_CTL1_REG, 0x00000004);
 /* Enable the Region */
 //reg_wr(PCIE0_iATU_CTL2_REG, 0x80000000);

 //Reading Device ID and Vendor ID of  External WiFI card
 //rd_data = reg_rd(0xA0000000);
 //printf("Read Device and Vendor ID Reg = 0x%x\n", rd_data);

 //Reading Device ID and Vendor ID of  External WiFI card
 //rd_data = reg_rd(0xA0000004);
 //printf("Read Status and Cmd Reg = 0x%x\n", rd_data);

 // type 0 config write to enable Bus master enable and Memory Space enable of External WiFI card
 //rd_data |= 0x6;
 //reg_wr(0xA0000004, rd_data);
 //printf("Enable Bus master and Memory Space in Cmd Reg\n");
 //rd_data = reg_rd(0xA0000004);
 //printf("Read Status and Cmd Reg = 0x%x\n", rd_data);

 //Reading the BAR0  External WiFI Card Configuration Space
 //rd_data = reg_rd(0xA0000010);
 //printf("Read BAR0 = 0x%x\n", rd_data);

 //while (1);
//Register Initialization (MEM_SPACE_EN_Enable + BUS_MASTER_EN_Enable + CAP_LIST_Enable)
  reg_wr(PCIE0_CMD_REG, 0x00100006);

    //Type0 COfig write to write  0xA0000000  Address in BAR0 of  External WiFI Card
    //reg_wr(0xA0000010 ,  0xA0000000);

    //Set iATU Registers (Outbound) page 840
    /* Set Viewport */
    reg_wr(PCIE0_iATU_VIEW_REG, 0x00000001);
    /* Set Region Base and Limit */
    reg_wr(PCIE0_iATU_RLBA_REG, 0xA0000000); //PCIE Base Address
    reg_wr(PCIE0_iATU_RLIA_REG, 0xA000FFFF); //PCIE Limit Address
    /* Set Target Address to 0xA0000000 */
    reg_wr(PCIE0_iATU_RLTA_REG, 0xA0000000);
    /* Set Control1 Register : Region to be MEM */
    reg_wr(PCIE0_iATU_CTL1_REG, 0x00000000);
    /* Enable the Region */
    reg_wr(PCIE0_iATU_CTL2_REG, 0x80000000);

    //Set iATU Registers (Inbound)
    /* Set Viewport */
    reg_wr(PCIE0_iATU_VIEW_REG, 0x80000001);
    /* Set Target Address to Empty space in DDR (0x7FFE_0000) */
    reg_wr(PCIE0_iATU_RLTA_REG, LOOPBACK_DEST);
    /* Set Control1 Register : Region to be MEM */
    reg_wr(PCIE0_iATU_CTL1_REG, 0x00000000);
    /* Enable the Region */
    reg_wr(PCIE0_iATU_CTL2_REG, 0xC0000000);
    //reg_wr(PCIE0_iATU_CTL2_REG, 0x00000000);

    //Register Initialization (MEM_SPACE_EN_Enable + BUS_MASTER_EN_Enable + CAP_LIST_Enable)
  reg_wr(PCIE0_CMD_REG, 0x00100006);
    printf("after ATU programming!\n");

    /* Write to BAR0 */
  reg_wr(PCIE0_BAR0_REG, 0xA0000000);

    printf("\n\nLoopback Enabled\n");
 // set PCIE in loopback mode
 // set bit 2 (page 765)
 reg_rmw( PCIE0_PL_REG + 0x10 , 0x00000004 , 0x00000004);
 nop(4000000);
    // value 0xBABE should be loopbacked and written into DDR LOOPBACK_DEST.
    reg_wr(0xA0000000 ,  0x0000BABE);
    //axi_direct_1w1r();
    nop(40000000);
    rd_data = reg_rd(LOOPBACK_DEST);

    if (rd_data == 0x0000BABE)
        printf("PASSED simple write/read!\n");
    else
        printf("FAILED! rd = 0x%x\n", rd_data);
    //while(1);


    if (max_iter_v==0)
        // not initialized yet
        max_iter_v = MAX_ITER;


    //printf("MAX DATA = %d\n", tlp_size_v);
    printf("Iterations is %d\n", max_iter_v);

 //printf("######## Advanced Error Reporting before TEST ##################\n");
    //rd_data = reg_rd(0x98000104);
    //printf("Uncorrectable Error Status Register (at 0x98000104) = 0x%X\n", rd_data);
    //rd_data = reg_rd(0x98000108);
    //printf("Uncorrectable Error Mask Register (at 0x98000108) = 0x%X\n", rd_data);
    //rd_data = reg_rd(0x9800010C);
    //printf("Uncorrectable Error Severity Register (at 0x9800010C) = 0x%X\n", rd_data);
    //
    //rd_data = reg_rd(0x98000110);
    //printf("Correctable Error Status Register (at 0x98000110) = 0x%X\n", rd_data);
    //rd_data = reg_rd(0x98000114);
    //printf("Correctable Error Mask Register (at 0x98000114) = 0x%X\n", rd_data);
    //reg_wr(0x98000118, 0x1E);
    //rd_data = reg_rd(0x98000118);
    //printf("Enabling ECRC Generation and Check (at 0x98000118) = 0x%X\n", rd_data);
    //
    //rd_data = reg_rd(0x98000130);
    //printf("Root Erro Status Regsiter (at 0x98000130) = 0x%X\n", rd_data);

for (j= 0; j< max_iter_v; j++)
{
    //printf("starting iter = %d\n", j);
    // MDMA data transfer
    // Initialize frame descriptor at address 0x4000_2000
    reg_wr((S_frame_1 + 0x0),0x0);
    reg_wr((S_frame_1 + 0x4),0x1);
    reg_wr((S_frame_1 + 0x8),0x0); //desc_block_size[num_i]<<11 | 1<<9 | block_num[num_i]<<4 | 0<<2 | 1<<1 | 1<<0); // encode mode, read back scratch pad content
    reg_wr((S_frame_1 + 0xC),0x0);
      // Load Base pointer with Data array address
    reg_wr((S_frame_1 + 0x10),S_data_1);
    // Load Base pointer with Data array address
    if (RANDOM_DATA_SIZE){
        tlp_size_v = random_size_cur & MAX_DATA_MASK;
        random_size_cur = LSFR ( random_size_cur );
        if (tlp_size_v < random_min_size)
            random_min_size = tlp_size_v;
        if (tlp_size_v > random_max_size)
            random_max_size = tlp_size_v;

    }else
        tlp_size_v = MAX_DATA;

    reg_wr((S_frame_1 + 0x14),1<<31 | 0<<17 | 1<<16 | (tlp_size_v*4));

    // Initialize Data Buffer location present at 0x0000_4000
    //lsfr_data = 0x12345678 + j;
    lsfr_data = 0xA55A5A5 + j;
    for (i = 0 ; i< tlp_size_v; i++)  // divide by 4, since we write 4bytes
    {
        switch (p_mode){
            case 1:
                lsfr_data = LSFR( lsfr_data );
                break;
            case 2:
                lsfr_data = input_data_counter++;
                break;
            default:
                lsfr_data = data_pattern;
        }
        reg_wr((S_data_1 + i*4),lsfr_data);
        // clear dest data
        reg_wr((LOOPBACK_DEST + i*4),0x12345678);
        //if (i==0)
            //printf("first data 0x%X\n", lsfr_data);
        //reg_wr((S_data_1 + i*4),input_dw[i%3]);
        //printf("input 0x%x at %d\n", input_dw[i%3], i*4);
    }

    // Initialize Destination frame descriptor at address 0x0000_7000
    reg_wr((D_frame_1 + 0x0),0x0);
    reg_wr((D_frame_1 + 0x4),0x1);

    reg_wr((D_frame_1 + 0x8),0x0); //<<11 | 1<<9 | block_num[num_i]<<4 | 0<<2 | 1<<1 | 1<<0);
    reg_wr((D_frame_1 + 0xC),0x0);

  // Load Base pointer with Data array address
    reg_wr((D_frame_1 + 0x10),D_data_1);
    //printf("init D frame\n");
    // Load Base control with length
    reg_wr((D_frame_1 + 0x14),1<<31 | 0<<17 | 1<<16 | (tlp_size_v*4));

// Start programing the DMA at the end enable the DMA
  // Enable the Mem to IO MDMA
    //reg_wr((MDMA_BASE + 0x0),0x2);
    reg_wr((MDMA_BASE + 0x0),0x0);
 // Write in to Mem to FLEN register
    reg_wr((MDMA_BASE + 0x8),0x50F);

 // Write in to Mem to FLEN register
    //reg_wr((MDMA_BASE + 0xC),0x4);

 // Write in to Mem to IRQ
    //reg_wr((MDMA_BASE + 0x10),0x7F);
 //rd_data = reg_rd(MDMA_BASE + 0x0);
    //printf("read data @ 0x0 = 0x%X\n", rd_data);
// Start programing the DMA at the end enable the DMA
   // Enable the Mem to IO MDMA
    //reg_wr((MDMA_BASE + 0x80),0x2);
    reg_wr((MDMA_BASE + 0x80),0x0);
    reg_wr((MDMA_BASE + 0x88),0x50F);
    //reg_wr((MDMA_BASE + 0x8C),0x4);
    //reg_wr((MDMA_BASE + 0x90),0x7F);

// Initialize the Head Pointer
    reg_wr((MDMA_BASE + 0x84),D_frame_1);
    //rd_data = reg_rd(MDMA_BASE + 0x84);
    //printf("read data @ 0x84 = 0x%X\n", rd_data);
// Initialize the Head Pointer
    reg_wr((MDMA_BASE + 0x4),S_frame_1);

    //rd_data = reg_rd(MDMA_BASE + 0x4);
    //printf("read data @ 0x4 = 0x%X\n", rd_data);
    //nop(40000);
    //
    //rd_data = reg_rd(MDMA_BASE + 0x0);
    //printf("read data @ 0x0 = 0x%X\n", rd_data);
    //
    //rd_data = reg_rd(MDMA_BASE + 0x80);
    //printf("read data @ 0x80 = 0x%X\n", rd_data);

//    printf("init pointers\n");

    // Wait till Data gets transfered from Source to Destination Bufffer pointers
    while (! lanedone) {
        // wait for number of cycles equal to total size of job
        for (i = 0; i<(tlp_size_v*4); i++){
        lanedone = 0;
        }

        //rd_data = reg_rd(MDMA_BASE + 0x0);
        //printf("read data @ 0x0 = 0x%X\n", rd_data);
        //rd_data = reg_rd(MDMA_BASE + 0x80);
        //printf("read data @ 0x80 = 0x%X\n", rd_data);

        rd_data = reg_rd(D_frame_1 + 0xC);
        if (rd_data >= 0x80000000) // check if bit 31 is set
        {

            rd_data = reg_rd(D_frame_1 + 0x8);

            lanedone = 1;
        }

    }
    lanedone = 0;
    nop(40000);

    //rd_data = reg_rd(D_data_1 );
    //printf("first destination DWORD 0X%X\n",rd_data );

    //rd_data = reg_rd(LOOPBACK_DEST );
    //printf("first loopback dest DWORD 0X%X\n",rd_data );

    //lsfr_data = 0x12345678 + j;
    lsfr_data = 0xA55A5A5 + j;
    for (i = 0 ; i< tlp_size_v; i++)  // divide by 4, since we write 4bytes
    {
        switch (p_mode){
            case 1:
                lsfr_data = LSFR( lsfr_data );
                break;
            case 2:
                lsfr_data = output_data_counter++;
                break;
            default:
                lsfr_data = data_pattern;
        }
        rd_data = reg_rd(LOOPBACK_DEST +i*4);
        if (lsfr_data!= rd_data){
            if (DEBUG)
                printf("FAILED! @ iter = %d, i=%d, rd_data = 0x%X, expected = 0x%X, ADDR = 0x%X,   ",j, i, rd_data, lsfr_data, (LOOPBACK_DEST +i*4));
            total_errors++;
            rd_data = reg_rd(DWC_STS_REG0);
            if (DEBUG)
                printf("DWC_STS_REG0 = 0x%X\n", rd_data);
        }
        //else
        //    if (i%100==0)
        //        printf("passed @ %d\r", i);
    }
    // print out percentage
    if ( j % (max_iter_v/10) ==0){
        printf("Progress! %d%% iter = %d, total_errors = %d\n", ((100*j)/(max_iter_v)), j, total_errors);
        //if(total_errors)
        //    print("Total Errors! %d", total_errors);
    }
}

//printf("####### Advanced Error Reporting AFTER TEST ###############\n");
//    rd_data = reg_rd(0x98000104);
//    printf("Uncorrectable Error Status Register (at 0x98000104) = 0x%X\n", rd_data);
//    rd_data = reg_rd(0x98000108);
//    printf("Uncorrectable Error Mask Register (at 0x98000108) = 0x%X\n", rd_data);
//    rd_data = reg_rd(0x9800010C);
//    printf("Uncorrectable Error Severity Register (at 0x9800010C) = 0x%X\n", rd_data);
//
//    rd_data = reg_rd(0x98000110);
//    printf("Correctable Error Status Register (at 0x98000110) = 0x%X\n", rd_data);
//    rd_data = reg_rd(0x98000114);
//    printf("Correctable Error Mask Register (at 0x98000114) = 0x%X\n", rd_data);
//    reg_wr(0x98000118, 0x1E);
//    rd_data = reg_rd(0x98000118);
//    printf("Enabling ECRC Generation and Check (at 0x98000118) = 0x%X\n", rd_data);
//
//    rd_data = reg_rd(0x98000130);
//    printf("Root Erro Status Regsiter (at 0x98000130) = 0x%X\n", rd_data);

    if (RANDOM_DATA_SIZE)
        printf("DONE! total iter = %d, random size TLP used, max tlp size used = %d, min tlp size used = %d\n", j, random_max_size*4, random_min_size*4);
    else
        printf("DONE! total iter = %d, each of max size %d\n", j,MAX_DATA);
    printf("Total DWORD Errors = %d, \n", total_errors);

    if(!total_errors)
	    ret = 0;

    return ret;
}

