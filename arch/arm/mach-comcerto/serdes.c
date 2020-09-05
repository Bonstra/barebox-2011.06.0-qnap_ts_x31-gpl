/*
 * Copyright (C) Mindspeed Technologies, Inc. 2011. All rights reserved.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * @file serdes.c
 * @brief this C file will contain all required functions to program
 *        Snowbush SerDes PHY interface.
 * @date 10/02/2011
 */

#include <common.h>
#include <init.h>
#include <asm/types.h>
#include <asm/io.h>
#include <mach/bits.h>
#include <mach/comcerto-2000.h>
#include <mach/serdes.h>
#include <mach/clkcore.h>

#define MAX_LANE_OK_WAIT_JIFFIES	20000

//#define MAX_CMU_OK_WAIT_JIFFIES		200
#define MAX_CMU_OK_WAIT_JIFFIES		0xFFFF

/**
 * This function Wait for the 'Lane OK' to be signaled by the
 * Snowbush Serdes PHY.
 * @param sbphy_num	SerDes PHY intefrace number.
 */
static int wait_lane_ok(u32 sbphy_num)
{
	u32 rd_data = 0, masked_data = 0;
	u32 lane_ok_dtctd_mask = 0x00001000;
	unsigned long deadline = MAX_LANE_OK_WAIT_JIFFIES;


	do // Keep looping until you see the lane_ok_o of Serdes
	{
		rd_data = readl(COMCERTO_SERDES_DWC_CFG_REG( sbphy_num, SD_PHY_STS_REG_OFST));

		masked_data = rd_data & lane_ok_dtctd_mask; // Mask lane_ok Status

		if(masked_data == lane_ok_dtctd_mask) {
			return 1;
		}

	} while (deadline--);


	return 0;
}


/**
 * This function wait for the 'CMU OK' to be signaled by the
 * Snowbush Serdes PHY.
 * @param sbphy_num	SerDes PHY intefrace number.
 */
static int wait_cmu_ok(u32 sbphy_num)
{
	u32 rd_data = 0, masked_data = 0;
	u32 cmu_ok_dtctd_mask = 0x00004000;
	int CMU_Offset;
	unsigned long deadline = MAX_CMU_OK_WAIT_JIFFIES;

	CMU_Offset = COMCERTO_SERDES_DWC_CFG_REG( sbphy_num, SD_PHY_STS_REG_OFST );

	do // Keep looping until you see the cmu_ok_o of Serdes
	{
		rd_data = readl(CMU_Offset);

		masked_data = rd_data & cmu_ok_dtctd_mask; // Mask cmu_ok Status

		if(masked_data == cmu_ok_dtctd_mask) {
			return 1;
		}
	} while (deadline--);


	return 0;
}


/**
 * This function wait for the specified configured Snowbush PHY
 * (Serdes) to issue it's CMU-OK, and it's Lane to become Ready
 * after releasing the CMU & Lane resets.
 * @param sbphy_num	SerDes PHY intefrace number.
 */
static int wait_sb_cmu_lane_rdy(u32 sbphy_num, u32 type)
{
	u32 sd_ctl2_reg_offset;
	u32 cmu_rst_mask = 0x00010000;
	u32 lane_rst_mask = 0x00000040;
	u32 tmp = 0;

	sd_ctl2_reg_offset = COMCERTO_SERDES_DWC_CFG_REG( sbphy_num, SD_PHY_CTRL2_REG_OFST );

	/* Releasing the CMU Reset */
	tmp = readl(sd_ctl2_reg_offset);
	tmp = tmp & (~cmu_rst_mask);
	tmp = tmp | cmu_rst_mask;

	writel(tmp, sd_ctl2_reg_offset );

	/* Waiting for CMU OK */
	if( !wait_cmu_ok(sbphy_num) ) {
		printf("Serdes %d : CMU failed !!\n", sbphy_num);
		return -1;
	}
	printf("Serdes %d : CMU OK\n", sbphy_num);

	if ( type == SD_DEV_TYPE_PCIE )
		writel(0xC3, COMCERTO_SERDES_REG(sbphy_num, (SD_COMMON_LANE << 2)));
	else
		writel(0x03, COMCERTO_SERDES_REG(sbphy_num, (SD_COMMON_LANE << 2)));

	/* Releasing the Lane Reset */
	tmp = readl(sd_ctl2_reg_offset);
	tmp = tmp & (~lane_rst_mask);
	tmp = tmp | lane_rst_mask;

	writel(tmp, sd_ctl2_reg_offset);


	if (type != SD_DEV_TYPE_PCIE) {
		/* Waiting for the Lane Ready */
		if( !wait_lane_ok(sbphy_num) )
		{
			/* Lane OK Detected on Serdes Port */
			printf("Serdes %d : Lane failed !!\n", sbphy_num);
			return -1;
		}
		/* Lane OK Detected on Serdes Port */
		printf("Serdes %d : Lane OK \n", sbphy_num);
	}
	else
		printf ("FIXME: For PCIe, Lane_ok check is not valid\n");

	return 0;
}


/**
 * This function reset the given SerDes.
 * @Param phy_num	SerDes Phy number to be reset.
 */
void serdes_phy_reset( int phy_num )
{
	writel( readl( SERDES_RST_CNTRL ) | ( SERDES0_RST << phy_num ), SERDES_RST_CNTRL );
	writel( readl( SERDES_RST_CNTRL ) & ~(SERDES0_RST << phy_num), SERDES_RST_CNTRL );
}

EXPORT_SYMBOL(serdes_phy_reset);


/**
 * This function initialize the Snowbush PHY (Serdes) for operation
 * with the one of the PCIE,SATA or SGMII IP blocks, and then waiting
 * until it issue it's CMU-OK, and it's  Lane to become Ready after
 * releasing the CMU & Lane Resets.
 * @param phy_num	SerDes PHY intefrace number.
 * @param *regs		Register file (Array of registers and coresponding
 *                      values to be programmed).
 * @param size		Number of registers to be programmed.
 */
int serdes_phy_init(int phy_num, struct serdes_regs_s *regs, int size, int type)
{
	int ii;

	/* Initilize serdes phy registers */
	for( ii = 0; ii < size; ii++ )
		writel(regs[ii].val, COMCERTO_SERDES_REG(phy_num, regs[ii].ofst));

	/* Wait for the initialization of Serdes-1 Port/Lane to become Ready */
	return wait_sb_cmu_lane_rdy(phy_num, type);
}

EXPORT_SYMBOL(serdes_phy_init);

