/*
 * (C) Copyright Mindspeed Technologies Inc.
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
 */
#include <init.h>
#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <errno.h>

//#define C2K_DDR_TRAINING_DEBUG
#undef C2K_DDR_TRAINING_DEBUG

#if defined(C2K_DDR_TRAINING_DEBUG)
#define  DPRINT_LVL0(args)   printf args
#define  DPRINT_LVL1(args)   printf args
#define  DPRINT_LVL2(args)   printf args
#define DPRINT_LIST(ds,max,adj2_dsx)   {      \
		for (read_offset=0; read_offset < max; read_offset++) \
			printf("%d,",(adj2_dsx[read_offset]>>(ds*8))&0xFF);\
		printf("\n");\
	}
#else
#define  DPRINT_LVL0(args)   printf args
#define  DPRINT_LVL1(args)   
#define  DPRINT_LVL2(args)    
#define DPRINT_LIST(ds,max,adj2_dsx)   
#endif

#define SZ_1K	        0x400
#define SZ_128K         (SZ_1K * 128)
#define ADDR_JUMP_SIZE  SZ_128K

#define PHYS_SDRAM_SIZE (256*1024*1024)
#define DDR_BASEADDR  0x0

#define CTRL_LOCK_SHIFT 4
#define CTRL_LOCK_MASK  0x3ff
#define PHY_RD_VLD_MASK 0x7fffffff


/* DDR PHY Registers */
#define DDR_PHY_CTL_00_REG  (0x905B0000 + 0x00)
#define DDR_PHY_CTL_01_REG  (0x905B0000 + 0x04)
#define DDR_PHY_CTL_02_REG  (0x905B0000 + 0x08)
#define DDR_PHY_CTL_03_REG  (0x905B0000 + 0x0C)
#define DDR_PHY_CTL_04_REG  (0x905B0000 + 0x10)
#define DDR_PHY_CTL_05_REG  (0x905B0000 + 0x14)
#define DDR_PHY_CTL_06_REG  (0x905B0000 + 0x18)
#define DDR_PHY_CTL_07_REG  (0x905B0000 + 0x1C)
#define DDR_PHY_DLL_STAT_REG    (0x905B0000 + 0x70)
#define DDR_PHY_ZQ_STAT_REG (0x905B0000 + 0x74)

#define CTRL_RESYNC_EN  (1 << 20)
#define CTRL_RESYNC_PLS (1 << 21)

#define ADJ1_MIN_ACCEPTED_WINDOW  50
#define ADJ1_ACCEPTED_WINDOW      55
#define ADJ2_MIN_ACCEPTED_RANGE   5
#define ADJ2_ACCEPTED_RANGE       8  


static u8 do_wr_rd_transaction(u32 ddr_addr_offset, u64*, u32*, u16 mode);

typedef struct adj2_ds_s
{
  u8 win_start;
  u8 win_end;
  u8 win;

  u8 gWin;
  u8 gWin_start;
  u8 gWin_end;
  u8 inx; 
}adj2_ds_t;

typedef struct adj2_val_s
{
  adj2_ds_t ds0;
  adj2_ds_t ds1;
  adj2_ds_t ds2;
  adj2_ds_t ds3;
}adj2_val_t;

/********************************************************************
 * enable_resync:
 *
 *******************************************************************/
void enable_resync(void)
{
	u32 ctrl_val;

	//Enable PHY ctrl_resync
	writel(0x80000000, DDR_PHY_CTL_07_REG);

	ctrl_val = readl(DDR_PHY_CTL_07_REG) & PHY_RD_VLD_MASK;
	ctrl_val |= CTRL_RESYNC_EN;
	writel(ctrl_val, DDR_PHY_CTL_07_REG);
}

/********************************************************************
 * disable_resync:
 *
 *******************************************************************/
void disable_resync(void)
{
	u32 ctrl_val;

	//Clear PHY ctrl_resync
	writel(0x80000000, DDR_PHY_CTL_07_REG);

	ctrl_val = readl(DDR_PHY_CTL_07_REG) & PHY_RD_VLD_MASK;
	ctrl_val &= ~CTRL_RESYNC_EN;
	writel(ctrl_val, DDR_PHY_CTL_07_REG);
}

/********************************************************************
 * assert_resync:
 *
 *******************************************************************/
void assert_resync(void)
{
	u32 ctrl_val;

	//Assert ctrl_resync pulse 
	writel(0x80000000, DDR_PHY_CTL_07_REG);

	ctrl_val = readl(DDR_PHY_CTL_07_REG) & PHY_RD_VLD_MASK;
	ctrl_val |= CTRL_RESYNC_PLS;
	writel(ctrl_val, DDR_PHY_CTL_07_REG); 

	//clear it
	ctrl_val &= ~CTRL_RESYNC_PLS;
	writel(ctrl_val, DDR_PHY_CTL_07_REG); 
}


/********************************************************************
 * write_dq_offset_req:
 * Write to the Control Register 4 and 5 to set the delay offset to 
 * write DQ values for each byte
 *******************************************************************/
void write_dq_offset_reg(u8 byte0, u8 byte1, u8 byte2, u8 byte3)
{
	u32 ctrl4_val;
	u32 ctrl5_val;
	u32 ctrl_val;

	//Write the first 3 byte in Control Register 4
	writel(0x80000000, DDR_PHY_CTL_04_REG);
	ctrl4_val = readl(DDR_PHY_CTL_04_REG) & PHY_RD_VLD_MASK;
	ctrl_val = (byte2 << 24) | (byte1 << 16) | 
		   (byte0 << 8) | (ctrl4_val & 0x7f);
	writel(ctrl_val, DDR_PHY_CTL_04_REG);

	//Write the 4th byte in Control Register 5
	writel(0x80000000, DDR_PHY_CTL_05_REG);
	ctrl5_val = readl(DDR_PHY_CTL_05_REG) & PHY_RD_VLD_MASK;
	ctrl_val = (byte3) | (ctrl5_val & ~0x7f);
	writel(ctrl_val, DDR_PHY_CTL_05_REG);

	assert_resync();
}

/********************************************************************
 * read_dqs_offset_req:
 * Write to the Control Register 3 to set the delay offset to 
 * read DQS values for each byte
 ********************************************************************/
void read_dqs_offset_reg(u8 byte0, u8 byte1, u8 byte2, u8 byte3)
{
	u32 ctrl_val;

	ctrl_val = (byte3 << 24) | (byte2 << 16) | 
		   (byte1 << 8) | (byte0);
	writel(ctrl_val, DDR_PHY_CTL_03_REG);

	assert_resync();
}

/**********************************************************************
 * calOffset:
 *
 *********************************************************************/
u8 calOffset(int offset_ctrl, u32 ctrl_offset_max)
{
	u8 offset;

	if (offset_ctrl < ctrl_offset_max) {
		//bit[6] should be 1 for negative delay
		offset = (((ctrl_offset_max - offset_ctrl) & 0x3F) | (1 << 6)); 
	} 
	else {
		//bit[6] should be 0 for positive delay
		offset = (((offset_ctrl - ctrl_offset_max) & 0x3F) | (0 << 6)); 
	}

	return offset;
}

/*********************************************************************
 * adj2_dsx_calculate_window:
 *
 ********************************************************************/
static void adj2_dsx_calculate_window(u8 max_win_size, u8 result, 
					u8 offsetw, u8 offsetr,
				        adj2_ds_t *adj2)
{
	u8 win;

	if (!(result & 0x0F)) /* success */ 
	{
	  	/* its first time success, start counting from here */
		if (adj2->win_start == 0)
		{
			adj2->win_start = adj2->win_end = offsetw;
		}
		else  {
			adj2->win_end++;
			/* close the window, if we reach 
			the end point of adj1 range */
			if (offsetw == max_win_size)  
			{
				win = (adj2->win_end - adj2->win_start) + 1;

			        if (adj2->win < win)
					adj2->win = win;

				DPRINT_LVL2(("[ds%d:%d..%d=%d]\n",result >> 4, adj2->win_start, adj2->win_end, win));

				/* is this window bigger than 
				the previous one, then take it */
				if (adj2->gWin < win)
				{
					adj2->gWin = win;
					adj2->gWin_start = adj2->win_start;
					adj2->gWin_end = adj2->win_end;
					adj2->inx = offsetr; 
				}
			}
		}
	}
	else /* failure */
	{
		if (adj2->win_start == 0)
		{
			//window not yet started, so nothing to do 
		}
		else
		{
			//this window ends here,calculate window size
			win = (adj2->win_end - adj2->win_start) + 1;

			DPRINT_LVL2(("[ds%d:%d..%d=%d]\n",result >> 4, adj2->win_start, adj2->win_end, win));

			if (adj2->win < win)
			{
				adj2->win = win;
			}

			//is this window bigger than the previous one, 
			if (adj2->gWin < win)
			{
				adj2->gWin = win;
				adj2->gWin_start = adj2->win_start;
				adj2->gWin_end = adj2->win_end;
				adj2->inx = offsetr; 
			}

			/* reset window pointer to measure next window */
			adj2->win_start = adj2->win_end = 0;
		}
	} /* failure */

	return;
}	

/********************************************************************
 * get_adj2_adjusted_value:
 * for basing on adj2 min value logic support 
 *
 *********************************************************************/
static int get_adj2_adjusted_value(u8 max_win_size, u32 *adj2_dsx, 
				adj2_val_t *adj2, u16 ddr16bit_mode)
{
	u8 i, adj2_val;
	u16 ds, shift;
	u8 win_start, win_end, win;
	u8 gwin_start, gwin_end, gwin;

	for(ds=0; ds < 4; ds++) 
	{
		if ((ds == 2) && (ddr16bit_mode)) 
			break;

		shift = ds * 8;

		win_start = win_end = win = 0;
		gwin_start = gwin_end = gwin = 0;

		for(i=0; i < max_win_size; i++) 
		{
			adj2_val = (u8)((adj2_dsx[i] >> shift) & 0xff);

			/* if, adj2 val is accepted */
			if (adj2_val >= ADJ1_MIN_ACCEPTED_WINDOW)  
			{
				if (win_start == 0)   {
	    				win_start = win_end = i;
          			} else {
					win_end++;

					/* close the window, if we reach 
					the end of adj2 range */
					if (i == max_win_size)  
					{
						win = win_end - win_start + 1;
						if (gwin < win) 
						{
							gwin = win;
							gwin_start = win_start;
							gwin_end = win_end;
						}

						DPRINT_LVL2(("DS%d start:%d, end:%d, win:%d\n", ds,win_start,win_end,win));

					}
				}
			}
			else  
			{
				/* if (win_start == 0) do nothing */
				if (win_start != 0)  
				{
					win = win_end - win_start + 1;
					if (gwin < win) 
					{
						gwin = win;
						gwin_start = win_start;
						gwin_end = win_end;
					}

					DPRINT_LVL2(("DS%d start:%d, end:%d, win:%d\n", ds,win_start,win_end,win));

					win_start = win_end = 0;
				}
			}
		} /* end of for (adj2 for a given ds) */

		DPRINT_LVL1(("Read(adj2) DS%d start:%d, end:%d, win:%d\n", ds, gwin_start, gwin_end, gwin));

		/* check for adj2 range size */
		if (gwin < ADJ2_MIN_ACCEPTED_RANGE)  
		{  
			DPRINT_LVL0(("Error - DDR Training results\n"));
			return -1; 
		}
		else if (gwin < ADJ2_ACCEPTED_RANGE) 
		{
			DPRINT_LVL0(("\nWARNING - DDR Training results may be Marginal\n"));
		}

		switch (ds) 
		{
			case 0 :
				adj2->ds0.inx = (gwin_start + gwin_end) >> 1;
				adj2->ds0.gWin = adj2_dsx[adj2->ds0.inx] & 0xff;
				break;
			case 1 : 
				adj2->ds1.inx = (gwin_start + gwin_end) >> 1;
				adj2->ds1.gWin = (adj2_dsx[adj2->ds1.inx] & 0xff00) >> 8;
				break;
			case 2 :
				adj2->ds2.inx = (gwin_start + gwin_end) >> 1;
				adj2->ds2.gWin = (adj2_dsx[adj2->ds2.inx] & 0xff0000) >> 16;
				break;
			case 3:
				adj2->ds3.inx = (gwin_start + gwin_end) >> 1;
				adj2->ds3.gWin = (adj2_dsx[adj2->ds3.inx] & 0xff000000) >> 24;
			default:
				break;
		}
	} /* end of for(ds) */

	if ((adj2->ds0.gWin < ADJ1_ACCEPTED_WINDOW) || 
	    (adj2->ds1.gWin < ADJ1_ACCEPTED_WINDOW)) {
		goto warning;
	}

	if (!ddr16bit_mode)  {
		if ((adj2->ds2.gWin < ADJ1_ACCEPTED_WINDOW) || 
		    (adj2->ds3.gWin < ADJ1_ACCEPTED_WINDOW)) {
			goto warning;
     		}
  	}

	return 0;

warning:
	DPRINT_LVL0(("WARNING - DDR Training results may be Marginal\n"));
	DPRINT_LVL1(("Write win: ds0:%d, ds1:%d, ds2:%d, ds3:%d\n", adj2->ds0.gWin,adj2->ds1.gWin,adj2->ds2.gWin,adj2->ds3.gWin));
	return 0;
}


/*****************************************************************************
 * recalculate_adj1_window:
 * re-calculate the win_start and win_end values for the choosen adj2
 *
 ****************************************************************************/
void recalculate_adj1_window(u8 ctrl_lock_val, u8 ddr16bit_mode, 
				adj2_val_t *adj2, u64 *dword_list, 
				u32 *word_list)
{
	u32 result;
	u32 ddr_addr_offset = 0;

	u8 write_offset, offset;
	u8 ctrl_offset_max;
	u8 max_win_size;
  
	/* Reset window pointers */
	adj2->ds0.win_start = adj2->ds0.win_end = 0;
	adj2->ds1.win_start = adj2->ds1.win_end = 0;
	adj2->ds2.win_start = adj2->ds2.win_end = 0;
	adj2->ds3.win_start = adj2->ds3.win_end = 0;

	adj2->ds0.win = 0;
	adj2->ds1.win = 0;
	adj2->ds2.win = 0;
	adj2->ds3.win = 0;

	adj2->ds0.gWin = 0;
	adj2->ds1.gWin = 0;
	adj2->ds2.gWin = 0;
	adj2->ds3.gWin = 0;

	ctrl_offset_max = (ctrl_lock_val >= 128) ? 0x3F :
				(((ctrl_lock_val/2)-1) & 0x3F); //to make sure it is an odd value
	max_win_size = (2*ctrl_offset_max)+1;

	enable_resync();

	DPRINT_LVL1(("DS0 Read offset: %d\n",adj2->ds0.inx));
	DPRINT_LVL1(("DS1 Read offset: %d\n",adj2->ds1.inx));
	if (!ddr16bit_mode) {
		DPRINT_LVL1(("DS2 Read offset: %d\n",adj2->ds2.inx));
		DPRINT_LVL1(("DS3 Read offset: %d\n",adj2->ds3.inx));
	}

	read_dqs_offset_reg( calOffset(adj2->ds0.inx,ctrl_offset_max),
			     calOffset(adj2->ds1.inx,ctrl_offset_max),
			     calOffset(adj2->ds2.inx,ctrl_offset_max),
			     calOffset(adj2->ds3.inx,ctrl_offset_max));
				
	/* Looping through write range */
	for (write_offset = 0; write_offset < max_win_size; write_offset++)
	{
		offset = calOffset(write_offset,ctrl_offset_max);
		write_dq_offset_reg(offset, offset, offset, offset);

		result = do_wr_rd_transaction(ddr_addr_offset,dword_list,word_list, ddr16bit_mode);
		ddr_addr_offset = (ddr_addr_offset + ADDR_JUMP_SIZE) & (PHYS_SDRAM_SIZE -1);

		adj2_dsx_calculate_window(max_win_size, 
					(result & 0x1)|0x10, write_offset, 
					adj2->ds0.inx, &adj2->ds0);
		adj2_dsx_calculate_window(max_win_size, 
					(result & 0x2)|0x20, write_offset, 
					adj2->ds1.inx, &adj2->ds1);
		if (!ddr16bit_mode) {
  			adj2_dsx_calculate_window(max_win_size, 
					(result & 0x4)|0x40, write_offset, 
					adj2->ds2.inx, &adj2->ds2);
  			adj2_dsx_calculate_window(max_win_size, 
					(result & 0x8)|0x80, write_offset, 
					adj2->ds3.inx, &adj2->ds3);
		}
	}

	disable_resync();

	return;
}

/*****************************************************************************
 * ddr_training()
 * 
 ****************************************************************************/
static int ddr_training(void)
{
	u32 word_list[16]  = 
			{  0xffffffff, 0x00000000, 0x12345678, 0x9abcdef0,
				0xf7f70202, 0xdfdf2020, 0x80407fbf, 0x08040204,
				0x8080fdfd, 0x0808dfdf, 0xa5a55a5a, 0x5a5aa5a5,
				0xaaaa5555, 0x5555aaaa, 0x0000ffff, 0x0000ffff};

	u64 dword_list[16]  = 
			{  0xffffffff00000000ULL, 0xffffffff00000000ULL, 
				0x1234567876543210ULL, 0x0123456789abcdefULL,
				0xf7f7f7f702020202ULL, 0xdfdfdfdf20202020ULL,
				0x804020107fbfdfefULL, 0x0804020110204080ULL,
				0x80808080fdfdfdfdULL, 0x08080808dfdfdfdfULL,
				0xa5a5a5a55a5a5a5aULL, 0x5a5a5a5aa5a5a5a5ULL,
				0xaaaaaaaa55555555ULL, 0x55555555aaaaaaaaULL,
				0x00000000ffffffffULL, 0x00000000ffffffffULL
			};

	u8 result;
	u32 ddr_addr_offset = 0;
	adj2_val_t adj2;
	u16 ddr16bit_mode = 0;

	u32 ctrl_lock_val;
	u32 ctrl_offset_max;
	u32 max_win_size;
	u8 read_offset, write_offset, offset;

	u8 rds0,rds1,rds2=0,rds3=0;
	u8 wds0,wds1,wds2=0,wds3=0;

	unsigned int adj2_dsx[128] = { 0 };

	DPRINT_LVL0(("\nDDR Training:\n"));

	/* check for 16bit mode */
	if (readl(0x970000c4) & 0x01000000)
		ddr16bit_mode = 1;

	adj2.ds0.gWin = 0;
	adj2.ds1.gWin = 0;
	adj2.ds2.gWin = 0;
	adj2.ds3.gWin = 0;

	writel(0x80000000, DDR_PHY_DLL_STAT_REG);
	ctrl_lock_val = (readl(DDR_PHY_DLL_STAT_REG) >> CTRL_LOCK_SHIFT ) & CTRL_LOCK_MASK ;

	DPRINT_LVL1(("CTRL_LOCK_VAL: %d\n", ctrl_lock_val));

	
	ctrl_offset_max = (ctrl_lock_val >= 128) ? 0x3F :
				(((ctrl_lock_val/2)-1) & 0x3F); //to make sure it is an odd value
	max_win_size = (2*ctrl_offset_max)+1;

	/* Looping through read */
	for(read_offset = 0; read_offset < max_win_size; read_offset++)
	{
		putchar('.');

		enable_resync();

		offset = calOffset(read_offset,ctrl_offset_max);
		read_dqs_offset_reg(offset, offset, offset, offset);

		DPRINT_LVL1(("\nRead offset: %d, (%c%d)\n", read_offset, (offset & (1 << 6)? '-' : '+'), offset & 0x3f)); 

		/* Reset window pointers */
		adj2.ds0.win_start = adj2.ds0.win_end = 0;
		adj2.ds1.win_start = adj2.ds1.win_end = 0;
		adj2.ds2.win_start = adj2.ds2.win_end = 0;
		adj2.ds3.win_start = adj2.ds3.win_end = 0;
		adj2.ds0.win = 0;
		adj2.ds1.win = 0;
		adj2.ds2.win = 0;
		adj2.ds3.win = 0;

		/* Looping through write range */
		for (write_offset = 0; write_offset <= max_win_size; write_offset++)
		{
			offset = calOffset(write_offset, ctrl_offset_max);
			write_dq_offset_reg(offset, offset, offset, offset);

			result = do_wr_rd_transaction(ddr_addr_offset,dword_list,word_list, ddr16bit_mode);
			ddr_addr_offset = (ddr_addr_offset + ADDR_JUMP_SIZE) & (PHYS_SDRAM_SIZE -1);

			adj2_dsx_calculate_window(max_win_size, (result & 0x1) | 0x10, write_offset, read_offset, &adj2.ds0);
			adj2_dsx_calculate_window(max_win_size, (result & 0x2) | 0x20, write_offset, read_offset, &adj2.ds1);
			if (!ddr16bit_mode)  {
				adj2_dsx_calculate_window(max_win_size, (result & 0x4) | 0x40, write_offset, read_offset, &adj2.ds2);
				adj2_dsx_calculate_window(max_win_size, (result & 0x8) | 0x80, write_offset, read_offset, &adj2.ds3);
			}
		} /* end of write loop  */ 

		disable_resync();

		/* store the adj2 win values */
		if (ddr16bit_mode) {
			adj2_dsx[read_offset] = (adj2.ds1.win << 8) | (adj2.ds0.win); 
		}
		else {
			adj2_dsx[read_offset] = (adj2.ds3.win << 24) | (adj2.ds2.win << 16) | (adj2.ds1.win << 8) | (adj2.ds0.win);
		}

	} //End of read loop

	DPRINT_LVL1(("DS0:%d:%d\n", adj2.ds0.inx , adj2.ds0.gWin));
	DPRINT_LVL1(("DS1:%d:%d\n", adj2.ds1.inx , adj2.ds1.gWin));
	if (!ddr16bit_mode)  {
		DPRINT_LVL1(("DS2:%d:%d\n", adj2.ds2.inx , adj2.ds2.gWin));
		DPRINT_LVL1(("DS3:%d:%d\n", adj2.ds3.inx , adj2.ds3.gWin));
	}
	
	/* ds0 */
	DPRINT_LIST(0,max_win_size,adj2_dsx);
	DPRINT_LIST(1,max_win_size,adj2_dsx);
	if (!ddr16bit_mode)  {
		DPRINT_LIST(2,max_win_size,adj2_dsx);
		DPRINT_LIST(3,max_win_size,adj2_dsx);
	}

	if (get_adj2_adjusted_value(max_win_size, adj2_dsx, &adj2, ddr16bit_mode) < 0)
		goto error;

	recalculate_adj1_window(ctrl_lock_val, ddr16bit_mode, &adj2, dword_list, word_list);

	DPRINT_LVL1(("\nWrite:\nDS0:%d-%d=%d\n", adj2.ds0.gWin_start, adj2.ds0.gWin_end, adj2.ds0.gWin));
	DPRINT_LVL1(("DS1:%d-%d=%d\n", adj2.ds1.gWin_start, adj2.ds1.gWin_end, adj2.ds1.gWin));
	if (!ddr16bit_mode)  {
		DPRINT_LVL1(("DS2:%d-%d=%d\n", adj2.ds2.gWin_start, adj2.ds2.gWin_end, adj2.ds2.gWin));
		DPRINT_LVL1(("DS3:%d-%d=%d\n", adj2.ds3.gWin_start, adj2.ds3.gWin_end, adj2.ds3.gWin));
	}

	/* Configure read offset */
	rds0 = calOffset(adj2.ds0.inx,ctrl_offset_max);
	rds1 = calOffset(adj2.ds1.inx,ctrl_offset_max);
	if (!ddr16bit_mode)  {
		rds2 = calOffset(adj2.ds2.inx,ctrl_offset_max);
		rds3 = calOffset(adj2.ds3.inx,ctrl_offset_max);
	}

	/* configure write offset */
	write_offset = (adj2.ds0.gWin_start + adj2.ds0.gWin_end) >> 1;
	wds0 = calOffset(write_offset,ctrl_offset_max);

   	write_offset = (adj2.ds1.gWin_start + adj2.ds1.gWin_end) >> 1;
	wds1 = calOffset(write_offset,ctrl_offset_max);

	if (!ddr16bit_mode)  {
   		write_offset = (adj2.ds2.gWin_start + adj2.ds2.gWin_end) >> 1;
		wds2 = calOffset(write_offset,ctrl_offset_max);

   		write_offset = (adj2.ds3.gWin_start + adj2.ds3.gWin_end) >> 1;
		wds3 = calOffset(write_offset,ctrl_offset_max);
	}

	enable_resync();
	read_dqs_offset_reg(rds0, rds1, rds2, rds3);
	write_dq_offset_reg(wds0, wds1, wds2, wds3);
	disable_resync();

	DPRINT_LVL0(("Done\n"));
	DPRINT_LVL0(("Read  offset: %02x,%02x,%02x,%02x\n", rds0,rds1,rds2,rds3));
	DPRINT_LVL0(("Write offset: %02x,%02x,%02x,%02x\n", wds0,wds1,wds2,wds3));
	return 0;

error: 
	DPRINT_LVL0(("Error\n"));
	return 0;
}


/******************************************************************************
 * do_wr_rd_transaction
 *
 *****************************************************************************/
static u8 do_wr_rd_transaction(u32 ddr_address_offset, u64 *dword_list, 
										u32 *word, u16 ddr16bit_mode)
{
	u8 j;
	register int reg_0 __asm__ ("r3");
	register int reg_1 __asm__ ("r4");
	u64 *src, *dst;
	u32 read_val;
	u8 ret_val = 0;

	ddr_address_offset &= ~0x3;

	/* Do 64bit wr+rd */
	dst = (u64 *)(DDR_BASEADDR+ddr_address_offset);
	src = dword_list;
	for(j=0; j < 16; j++)
	{
		__asm__ __volatile__ ("ldmia %0, {%1,%2}" \
			: "+r" (src), "=r" (reg_0), "=r" (reg_1) );

		__asm__ __volatile__ ("stmia %0, {%1,%2}" \
			: "+r" (dst), "=r" (reg_0), "=r" (reg_1) );

		if ((*src & 0x000000ff000000ffLL) != (*dst & 0x000000ff000000ffLL))
		{
			ret_val |= 1;
		}
		if ((*src & 0x0000ff000000ff00LL) != (*dst & 0x0000ff000000ff00LL))
		{
			ret_val |= 1 << 1;
		}

		if (!ddr16bit_mode)  {
			if ((*src & 0x00ff000000ff0000LL) != (*dst & 0x00ff000000ff0000LL))
			{
				ret_val |= 1 << 2;
			}
			if ((*src & 0xff000000ff000000LL) != (*dst & 0xff000000ff000000LL))
			{
				ret_val |= 1 << 3;
			}
		}
		*dst = 0; //clear location

		dst++; 
		src++;
	}

	/* Do 32bit wr+rd */
	for (j=0; j < 16; j++)
	{
		*(((volatile u32 *)(DDR_BASEADDR+ddr_address_offset)) + j) = *word;

		read_val = *(((volatile u32 *)(DDR_BASEADDR+ddr_address_offset)) + j);
		if ((read_val & 0x000000FF) != (*word & 0x000000ff))
		{
			ret_val |= 1;
		}
		if ((read_val & 0x0000ff00) != (*word & 0x0000ff00))
		{
			ret_val |= 1 << 1;
		}
		if (!ddr16bit_mode)  {
			if ((read_val & 0x00ff0000) != (*word & 0x00ff0000))
			{
				ret_val |= 1 << 2;
			}
			if ((read_val & 0xff000000) != (*word & 0xff000000))
			{
				ret_val |= 1 << 3;
			}
		}

		*(((volatile u32 *)(DDR_BASEADDR+ddr_address_offset)) + j) = 0; //clear location

		 word++;
	}

	return ret_val;
}

device_initcall(ddr_training);
