/*
;=======================================================================
;   Copyright (C) 2007 Mindspeed Technologies, Inc.
;
;======================================================================
*/
#include <spi/fast_spi.h>
#include <mach/dma.h>
#include <mach/comcerto_spi.h>
#include <mach/gpio.h>
#include <asm/hardware.h>
#include <common.h>
#include <asm/io.h>
#include <mach/dma.h>

#include "c2k_dma.h"

//#define COMCERTO_DMA_DEBUG
#ifdef COMCERTO_DMA_DEBUG
	#define debug_dma_reg(r) do {\
		unsigned int *reg = (unsigned int*)r;\
		printf ("%s:Reg(0x%x) = 0x%08x\n", __func__, (unsigned int)reg, *reg);\
	}while(0)
#else
	#define debug_dma_reg(reg) 
#endif

/*
 ****************************************
 *   dma_configure ()
 *
 *   Single-block Transfer -- without write-back
 *   of control and status information enabled at
 *   the end of the single-block transfer
 ****************************************
 */
void dma_conf(U32 ch_no, U32 dst, U32 data_len, U32 src, struct dma_conf_param *dmaconf)
{
        U32     dma_channel = (1 << ch_no) & 0xFF ;
        U32     ch_reg_multiplier = 0;
        U32 ch_ctrl_l = 0;
        U32 ch_cfg_l = 0;
        U32 ch_cfg_h = 0 ;
	U32 dst_msize, dms, sms, src_per, dest_per;

        /* DMA enable */
        writel(DMA_GLOBAL_ENABLE, DW_DMA_DMAC_DMA_CFG_REG);
	debug_dma_reg(DW_DMA_DMAC_DMA_CFG_REG);

        /* Configure DMA handshaking */

        /*  Last Destination Transaction Request Register */
        writel(((dma_channel << DMA_REG_WE_SHIFT) | (0x0 & 0xFF)), DW_DMA_DMAC_LST_DST_REG);
        /* Single Destination Transaction Request Register */
        writel(((dma_channel << DMA_REG_WE_SHIFT) | (0x0 & 0xFF)), DW_DMA_DMAC_SGL_REQ_DST_REG);

        /* Clear Interrupts on the channal */

        /* Clear for IntTfr Interrupt */
        writel(dma_channel, DW_DMA_DMAC_CLEAR_TFR);
        /* Clear for IntBlock Interrupt */
        writel(dma_channel, DW_DMA_DMAC_CLEAR_BLK);
        /* Clear for IntSrcTran Interrupts */
        writel(dma_channel, DW_DMA_DMAC_CLEAR_SRC_TRAN);
        /* Clear for IntDstTran Interrupt */
        writel(dma_channel, DW_DMA_DMAC_CLEAR_DST_TRAN);
        /* Clear for IntErr Interrupt */
        writel(dma_channel, DW_DMA_DMAC_CLEAR_ERR);


        /* Set up Interrupt Mask registers */

        /* Mask for IntTfr Interrupt */
        writel(((dma_channel << DMA_REG_WE_SHIFT) | (0x0 & 0xFF)), DW_DMA_DMAC_MASK_TFR);
        /* Mask for IntBlock Interrupt */
        writel(((dma_channel << DMA_REG_WE_SHIFT) | dma_channel), DW_DMA_DMAC_MASK_BLOCK);
        /* Mask for IntSrcTran Interrupt */
        writel(((dma_channel << DMA_REG_WE_SHIFT) | dma_channel), DW_DMA_DMAC_MASK_SRC_TRAN);
        /* Mask for IntDstTran Interrupt */
        writel(((dma_channel << DMA_REG_WE_SHIFT) | dma_channel), DW_DMA_DMAC_MASK_DST_TRAN);
        /* Mask for IntErr Interrupt */
        writel(((dma_channel << DMA_REG_WE_SHIFT) | (0x0 & 0xFF)), DW_DMA_DMAC_MASK_ERR);


        /* configure channel specific registers */
        ch_reg_multiplier = (DMA_CHANNEL_REG_COUNT << 3) ;
        //ch_reg_multiplier = ch_no * ch_reg_multiplier;        // ??????????? chek for MUL lib; not req for ch '0'
        //ch_reg_multiplier = 352;        // ch 4
        ch_reg_multiplier = ch_no * 88;        // 88 is the byte size of all dma regs

	if(dmaconf->dir == 0x1) /* DMA direction is Memory --> Peripheral */
	{
		dst_msize = DMA_CTL_DEST_MSIZE_WR;
		dms = DMA_CTL_DMS_WR;
		sms = DMA_CTL_SMS_WR;
		src_per = DMA_CFG_SRC_PER_WR;
		dest_per = DMA_CFG_DEST_PER_WR;
	}
	else
	{
		dst_msize = DMA_CTL_DEST_MSIZE;
		dms = DMA_CTL_DMS;
		sms = DMA_CTL_SMS;
		src_per = DMA_CFG_SRC_PER;
		dest_per = DMA_CFG_DEST_PER;
	}

        /* configure : Source Address Register for Channel */
        /* SAR Address must be alligned to DMA_CTL_SRC_TR_WIDTH boundry */
        writel(src, (DMA_CHANNEL_REG_SAR_BASE+ch_reg_multiplier));
	debug_dma_reg((DMA_CHANNEL_REG_SAR_BASE+ch_reg_multiplier));
        /* configure : Destination Address Register for Channel */
        /* DAR Address must be alligned to DMA_CTL_DST_TR_WIDTH boundry */ /* ???????? */
        writel(dst, (DMA_CHANNEL_REG_DAR_BASE+ch_reg_multiplier));
	debug_dma_reg((DMA_CHANNEL_REG_DAR_BASE+ch_reg_multiplier));


        /* configure BLOCK_TS: Control Register for Channel [32-63] */
        writel(data_len, (DMA_CHANNEL_REG_CTL_BASE+ch_reg_multiplier+4));

        /* configure : Control Register for Channel [0-31] */
        ch_ctrl_l = (((DMA_CTL_INT_EN & DMA_CTL_INT_EN_MASK) << DMA_CTL_INT_EN_SHIFT) |
                                ((DMA_CTL_DST_TR_WIDTH & DMA_CTL_DST_TR_WIDTH_MASK) << DMA_CTL_DST_TR_WIDTH_SHIFT) |
                                ((DMA_CTL_SRC_TR_WIDTH & DMA_CTL_SRC_TR_WIDTH_MASK) << DMA_CTL_SRC_TR_WIDTH_SHIFT) |
                                ((dmaconf->dinc & DMA_CTL_DINC_MASK) << DMA_CTL_DINC_SHIFT) |
                                ((dmaconf->sinc & DMA_CTL_SINC_MASK) << DMA_CTL_SINC_SHIFT) |
                                ((dst_msize & DMA_CTL_DEST_MSIZE_MASK) << DMA_CTL_DEST_MSIZE_SHIFT) |
                                ((DMA_CTL_SRC_MSIZE & DMA_CTL_SRC_MSIZE_MASK) << DMA_CTL_SRC_MSIZE_SHIFT) |
                                ((DMA_CTL_SRC_GATHER_EN & DMA_CTL_SRC_GATHER_EN_MASK) << DMA_CTL_SRC_GATHER_EN_SHIFT) |
                                ((DMA_CTL_DST_SCATTER_EN & DMA_CTL_DST_SCATTER_EN_MASK) << DMA_CTL_DST_SCATTER_EN_SHIFT) |
                                ((dmaconf->dir & DMA_CTL_TT_FC_MASK) << DMA_CTL_TT_FC_SHIFT) |
                                ((dms & DMA_CTL_DMS_MASK) << DMA_CTL_DMS_SHIFT) |
                                ((sms & DMA_CTL_SMS_MASK) << DMA_CTL_SMS_SHIFT) |
                                ((DMA_CTL_LLP_DST_EN & DMA_CTL_LLP_DST_EN_MASK) << DMA_CTL_LLP_DST_EN_SHIFT) |
                                ((DMA_CTL_LLP_SRC_EN & DMA_CTL_LLP_SRC_EN_MASK) << DMA_CTL_LLP_SRC_EN_SHIFT));

        writel(ch_ctrl_l, (DMA_CHANNEL_REG_CTL_BASE+ch_reg_multiplier)); /* 0x220cc05 */
	debug_dma_reg((DMA_CHANNEL_REG_CTL_BASE+ch_reg_multiplier));

        /* configure : Linked List Pointer Register for Channel */
        writel(0x0, (DMA_CHANNEL_REG_LLP_BASE+ch_reg_multiplier));

        /* configure : Configuration Register for Channel [32-63] */
        ch_cfg_h = (((DMA_CFG_FCMODE & DMA_CFG_FCMODE_MASK) << DMA_CFG_FCMODE_SHIFT) |
                           ((DMA_CFG_FIFO_MODE & DMA_CFG_FIFO_MODE_MASK) << DMA_CFG_FIFO_MODE_SHIFT) |
                           ((DMA_CFG_PROTCTL & DMA_CFG_PROTCTL_MASK) << DMA_CFG_PROTCTL_SHIFT) |
                           ((DMA_CFG_DS_UPD_EN & DMA_CFG_DS_UPD_EN_MASK) << DMA_CFG_DS_UPD_EN_SHIFT) |
                           ((DMA_CFG_SS_UPD_EN & DMA_CFG_SS_UPD_EN_MASK) << DMA_CFG_SS_UPD_EN_SHIFT) |
                           ((src_per & DMA_CFG_SRC_PER_MASK) << DMA_CFG_SRC_PER_SHIFT) |
                           ((dest_per & DMA_CFG_DEST_PER_MASK) << DMA_CFG_DEST_PER_SHIFT));

        writel(ch_cfg_h, (DMA_CHANNEL_REG_CFG_BASE+ch_reg_multiplier+4)); /* 0x00000204 */

        /* configure : Configuration Register for Channel [0-31] */
        ch_cfg_l = (((ch_no & DMA_CFG_CH_PRIOR_MASK) << DMA_CFG_CH_PRIOR_SHIFT) |
                           ((DMA_CFG_CH_SUSP & DMA_CFG_CH_SUSP_MASK) << DMA_CFG_CH_SUSP_SHIFT) |
                           ((DMA_CFG_FIFO_EMPTY & DMA_CFG_FIFO_EMPTY_MASK) << DMA_CFG_FIFO_EMPTY_SHIFT) |
                           ((dmaconf->hs_dst & DMA_CFG_HS_SEL_DST_MASK) << DMA_CFG_HS_SEL_DST_SHIFT) |
                           ((dmaconf->hs_src & DMA_CFG_HS_SEL_SRC_MASK) << DMA_CFG_HS_SEL_SRC_SHIFT) |
                           ((DMA_CFG_LOCK_CH_L & DMA_CFG_LOCK_CH_L_MASK) << DMA_CFG_LOCK_CH_L_SHIFT) |
                           ((DMA_CFG_LOCK_B_L & DMA_CFG_LOCK_B_L_MASK) << DMA_CFG_LOCK_B_L_SHIFT) |
                           ((DMA_CFG_LOCK_CH & DMA_CFG_LOCK_CH_MASK) << DMA_CFG_LOCK_CH_SHIFT) |
                           ((DMA_CFG_LOCK_B & DMA_CFG_LOCK_B_MASK) << DMA_CFG_LOCK_B_SHIFT) |
                           ((DMA_CFG_DST_HS_POL & DMA_CFG_DST_HS_POL_MASK) << DMA_CFG_DST_HS_POL_SHIFT) |
                           ((DMA_CFG_SRC_HS_POL & DMA_CFG_SRC_HS_POL_MASK) << DMA_CFG_SRC_HS_POL_SHIFT) |
                           ((DMA_CFG_MAX_ABRST & DMA_CFG_MAX_ABRST_MASK) << DMA_CFG_MAX_ABRST_SHIFT) |
                           ((DMA_CFG_RELOAD_SRC & DMA_CFG_RELOAD_SRC_MASK) << DMA_CFG_RELOAD_SRC_SHIFT) |                     
                           ((DMA_CFG_RELOAD_DST & DMA_CFG_RELOAD_DST_MASK) << DMA_CFG_RELOAD_DST_SHIFT));

        writel(ch_cfg_l, (DMA_CHANNEL_REG_CFG_BASE+ch_reg_multiplier)); /* 0x00000600 */

        return ;
}

/*
 *******************************************
 *   dma_ssi_xfer_cmplete_chk ()
 *
 *   Check for the complete of DMA data xfer
 *   on given channal no.
 *******************************************
 */
retcode dma_ssi_xfer_cmplete_chk(U32 ch_no)
{
        U32 channel_status = 0xF;

        while(channel_status != 0x0)
        {
                channel_status = readl(DW_DMA_DMAC_CH_EN_REG);
                channel_status = channel_status & ((1<<ch_no) & 0xFF);
                //channel_status = channel_status & 0x00000010;
        }

        return RETCODE_OK;
}

/************************************************************
 * dma_start()
 *    - This function enables the dma
 ***********************************************************/
void dma_start(U32 chan)
{
	U32 ch_status = readl(DW_DMA_DMAC_CH_EN_REG);
	u8 ch_en = 0x1;
	U32 val;

	ch_en <<= chan;
        /* Enable the DMA channel */
	val = ((ch_en << DMA_REG_WE_SHIFT) | ch_en) | ch_status;
	debug_dma_reg(DW_DMA_DMAC_CH_EN_REG);
	debug_dma_reg(&val);
        writel(val, DW_DMA_DMAC_CH_EN_REG);
}

