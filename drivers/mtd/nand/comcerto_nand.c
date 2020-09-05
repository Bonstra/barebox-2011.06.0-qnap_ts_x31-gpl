/*
 *  drivers/mtd/nand/comcerto-nand.c
 *
 *  Copyright (C) Mindspeed Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Overview:
 *   Device driver for the NAND flash device found on the
 *   Comcerto board.
 *
 */
#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <init.h>
#include <types.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <clock.h>

#include <mach/gpio.h>
#include <mach/comcerto-2000.h>
#include <mach/comcerto-common.h>
#include <mach/exp-bus.h>
#include <mach/ecc.h>

#include "nand.h"

#ifndef CONFIG_COMCERTO_NAND_ULOADER

#define BBD_ID_0		0xDEADBEEF
#define BBD_ID_1		0xECC0ECC0
#define MAX_BLOCKS_WITH_BBT	12
#define MAX_NO_SBL_COPIES	6
#define MAX_INHERITED_BAD_BLK	16 /* for boot sector */

#define BBT_STRUCT_SIZE_EXCLUDING_BLK_MARK (9 * 4 + sizeof(struct boot_sector_layout))
#define ECC_BLK_MARK_SET_0_SIZE (1024 - BBT_STRUCT_SIZE_EXCLUDING_BLK_MARK)

#define IRAM_PAYLOAD_SEGMENT_SIZE	0xC000

#endif /* CONFIG_COMCERTO_NAND_ULOADER */


#if defined (CONFIG_NAND_COMCERTO_ECC_24_HW_BCH)
/*
 * spare area layout for BCH ECC bytes calculated over 512-Bytes ECC block size
 */
static struct nand_ecclayout comcerto_ecc_info_512_bch = {
	.eccbytes = 42,
	.eccpos = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
		  11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		  31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41},
	.oobfree = {
		{.offset = 43, .length = 13}
	}
};

/*
 * spare area layout for BCH ECC bytes calculated over 1024-Bytes ECC block size
 */
static struct nand_ecclayout comcerto_ecc_info_1024_bch = {
	.eccbytes = 42,
	.eccpos = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
		  11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		  31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41},
	.oobfree = {
		{.offset = 43, .length = 13}
	}
};

#elif defined (CONFIG_NAND_COMCERTO_ECC_8_HW_BCH)

/*
 * spare area layout for BCH ECC bytes calculated over 512-Bytes ECC block size
 */
static struct nand_ecclayout comcerto_ecc_info_512_bch = {
	.eccbytes = 14,
	.eccpos = {0, 1, 2, 3, 4, 5, 6,
		   7, 8, 9, 10, 11, 12, 13},
	.oobfree = {
		{.offset = 15, .length = 1}
	}
};

/*
 * spare area layout for BCH ECC bytes calculated over 1024-Bytes ECC block size
 */
static struct nand_ecclayout comcerto_ecc_info_1024_bch = {
	.eccbytes = 14,
	.eccpos = {0, 1, 2, 3, 4, 5, 6,
		   7, 8, 9, 10, 11, 12, 13},
	.oobfree = {
		{.offset = 15, .length = 17}
	}
};
#else /* Hamming */
/*
 * spare area layout for Hamming ECC bytes calculated over 512-Bytes ECC block
 * size
 */
static struct nand_ecclayout comcerto_ecc_info_512_hamm = {
	.eccbytes = 4,
	.eccpos = {0, 1, 2, 3},
	.oobfree = {
		{.offset = 4, .length = 12}
	}
};

/*
 * spare area layout for Hamming ECC bytes calculated over 1024-Bytes ECC block
 * size
 */
static struct nand_ecclayout comcerto_ecc_info_1024_hamm = {
	.eccbytes = 4,
	.eccpos = {0, 1, 2, 3},
	.oobfree = {
		{.offset = 4, .length = 28}
	}
};
#endif

#ifndef CONFIG_COMCERTO_NAND_ULOADER
static uint8_t bbt_pattern[] = { 'B', 'b', 't', '0' };
static uint8_t mirror_pattern[] = { '1', 't', 'b', 'B' };

static struct nand_bbt_descr bbt_main_descr = {
	.options = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
		| NAND_BBT_8BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs = 44,
	.len = 4,
	.veroffs = 48,
	.maxblocks = 8,
	.pattern = bbt_pattern,
};

static struct nand_bbt_descr bbt_mirror_descr = {
	.options = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
		| NAND_BBT_8BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs = 44,
	.len = 4,
	.veroffs = 48,
	.maxblocks = 8,
	.pattern = mirror_pattern,
};
#endif /* CONFIG_COMCERTO_NAND_ULOADER */

static uint8_t scan_ff_pattern[] = {0xff};

#if defined (CONFIG_NAND_COMCERTO_ECC_24_HW_BCH)
static struct nand_bbt_descr c2000_badblock_pattern = {
	.offs = 42,
	.len = 1,
	.pattern = scan_ff_pattern
};
#elif defined (CONFIG_NAND_COMCERTO_ECC_8_HW_BCH)
static struct nand_bbt_descr c2000_badblock_pattern = {
	.offs = 14,
	.len = 1,
	.pattern = scan_ff_pattern
};
#else /* Hamming */
static struct nand_bbt_descr c2000_badblock_pattern = {
	.offs = 4,
	.len = 1,
	.pattern = scan_ff_pattern
};
#endif


#ifndef CONFIG_COMCERTO_NAND_ULOADER

struct boot_sector_layout {
        uint32_t nand_block_size;            /* get from nand_init           */
        uint32_t ecc_block_size;             /* default : 512                */
        uint32_t oob_size;                   /* default : 16                 */
        uint32_t pri_boot_block_start_0;     /* default : 0                  */
        uint32_t boot_block_size;            /* Multiple of ECC Block size (512) */
        uint32_t no_of_copies;               /* default : 6                  */
        uint32_t bbt_start_0;                /* default : 2                  */
        uint32_t bbt_start_1;                /* default : 3                  */
        uint32_t complete_bbt_size;          /* should not exceed size of NAND block */
        uint32_t sec_boot_block_start;       /* default : 4                  */
        uint32_t sec_boot_sector_size;       /* Multiple of NAND Block size  */
};

typedef struct boot_sector_layout  boot_sector_layout_t;

/*
 * Comcerto NAND flash bbt decriptors
 */
struct bbt_descriptor {
        uint32_t bbt_id_0;                           /* unique identifier to identify BBT 0xDEADBEEF */
        uint32_t bbt_id_1;                           /* unique identifier to identify BBT 0xECC0ECC0 */
        uint32_t options;                            /* implementation dependent */
        uint32_t bbt_max_blocks;                     /* maximum no of blocks to search for BBT */
        uint32_t bbt_max_resv_blocks;                /* maximum no of reserved blocks to search for BBT */
        uint32_t bbt_trail_location;                 /* */
        uint8_t bbt_set_id;
        uint32_t pri_boot_block_location;            /* */
        uint8_t inherited_bb_flag;                   /* Will be set when inherited bad block is found */
};

typedef struct bbt_descriptor  bbt_descriptor_t;
typedef struct bad_block_table bad_block_table_t;

bad_block_table_t bbt_temp;
bbt_descriptor_t bbt_desc;
boot_sector_layout_t boot_sector;

struct bad_block_table {
        uint32_t bbt_id_0;                   /* unique identifier to identify BBT 0xDEADBEEF */
        uint32_t bbt_id_1;                   /* unique identifier to identify BBT 0xECC0ECC0 */
        uint8_t inherited_blk[MAX_INHERITED_BAD_BLK]; /* Stors upto 8 locations of Inherited Bad Blocks */
        uint8_t boot_blk_split[8];
        boot_sector_layout_t layout;    /* current flash layout structure */
        uint32_t resrved_bbt_sec_start;      /* start location of reserved BBT sector */
        uint8_t ecc_block_mark[ECC_BLK_MARK_SET_0_SIZE];/* size of complete BBT should be
                                           equal to the NAND ECC block size */
};

#endif /* CONFIG_COMCERTO_NAND_ULOADER */
/*
 * MTD structure for Comcerto board
 */
struct comcerto_nand_info {
	struct mtd_info *mtd;
};

#ifndef CONFIG_COMCERTO_NAND_ULOADER
#ifdef CONFIG_COMCERTO_NAND_BBT /* Default: Turned OFF */
/* code under this flag is intended to be used only when
IBR NAND Boot is functional. */

/*Scan the nand flash to find the stored BBT and copy it into RAM.
 *
 */
int comcerto_scan_bbt(struct mtd_info *mtd)
{
	struct nand_chip *nand_device = (struct nand_chip *)(mtd->priv);
        uint8_t bbt_blks_count;
        uint32_t bbt_location;
        uint32_t spare_area_size = 0;
	uint32_t num_ecc_blks = 0;
	size_t retlen;
	uint32_t bbt_start, bbt_start_1;


	bbt_start = IRAM_PAYLOAD_SEGMENT_SIZE * MAX_NO_SBL_COPIES;

        if (bbt_start <= mtd->erasesize) {
		bbt_location = 1;
	} else {
                bbt_start_1 = bbt_start / mtd->erasesize;
                if ((bbt_start_1 * mtd->erasesize) < bbt_start)
                        bbt_start_1++;
		bbt_location = bbt_start_1;
        }

        bbt_desc.bbt_id_0 = BBD_ID_0;
        bbt_desc.bbt_id_1 = BBD_ID_1;
        bbt_desc.bbt_max_blocks = MAX_BLOCKS_WITH_BBT;

        bbt_desc.bbt_set_id = 0;

	if (nand_device->ecc.size < 512)
		spare_area_size = 32;

        num_ecc_blks = mtd->size / nand_device->ecc.size;

	if (num_ecc_blks > mtd->erasesize) {
                printk(KERN_ERR "comcerto_scan_bbt: Number of ECC blocks exceeds \
				 single erase block size \n");
                return 0;
	}

        /* Allocate memory (1 byte per block) and clear the memory bad block table */
        nand_device->bbt = kzalloc(num_ecc_blks, GFP_KERNEL);
        if (!nand_device->bbt) {
                printk(KERN_ERR "comcerto_scan_bbt: Out of memory\n");
                return -ENOMEM;
        }

        for (bbt_blks_count = 0; bbt_blks_count < bbt_desc.bbt_max_blocks; bbt_blks_count++) {
		if (mtd->read(mtd, bbt_location * mtd->erasesize, nand_device->ecc.size, &retlen, (uint8_t *)&bbt_temp) > 0) {
                        if ((bbt_temp.bbt_id_0 == bbt_desc.bbt_id_0) && (bbt_temp.bbt_id_1 == bbt_desc.bbt_id_1)) {
                                /* Got good BBT, now download remaining */
				memcpy(nand_device->bbt, (&bbt_temp + BBT_STRUCT_SIZE_EXCLUDING_BLK_MARK), ECC_BLK_MARK_SET_0_SIZE);
				if (mtd->read(mtd, bbt_location * mtd->erasesize + nand_device->ecc.size + spare_area_size, \
				num_ecc_blks - ECC_BLK_MARK_SET_0_SIZE, &retlen, (uint8_t *) (nand_device->bbt + ECC_BLK_MARK_SET_0_SIZE)) < 0)
					continue;
                                return 0;
                        }else   {
                        /* Got a good block, but BBT IDs don't match.Assuming
			this to be a microloader and next location BBT */
                                boot_sector.bbt_start_0 = ++bbt_location;
                                continue;
                        }
                } else  {
                        /* Got bad block, jump to next block */
                        bbt_location++;
                }
	}

	printk(KERN_WARNING "Bad block table not found\n");
        return 0;
}

/*comcerto_block_isbad
 *
 */
static int comcerto_block_isbad(struct mtd_info *mtd, loff_t offs)
{
	struct nand_chip *nand_device = (struct nand_chip *)(mtd->priv);
	uint32_t block;


	/* Check for invalid offset */
        if (offs > mtd->size)
                return -EINVAL;

	block = (uint32_t) offs / mtd->erasesize;
	if (nand_device->bbt[block] != 0xFF)
		return 1;
	return 0;
}

#endif /* CONFIG_COMCERTO_NAND_BBT */
#endif /* CONFIG_COMCERTO_NAND_ULOADER */

/** Disable/Enable shifting of data to parity module
 *
 * @param[in] en_dis_shift  Enable or disable shift to parity module.
 *
 */
static void comcerto_ecc_shift(uint8_t en_dis_shift)
{
	writel(en_dis_shift, ECC_SHIFT_EN_CFG);
}

/** Initializes h/w ECC with proper configuration values.
 *
 * @param[in] mtd	MTD device structure
 * @param[in] mode	Select between BCH and Hamming
 *
 */
static void comcerto_enable_hw_ecc(struct mtd_info *mtd, int mode)
{
	struct nand_chip *nand_device = (struct nand_chip *)(mtd->priv);
	uint32_t ecc_gen_cfg_val = 0;


	/* CS4 will have the option for ECC calculation */
	writel(ECC_CS4_SEL, ECC_CS_SEL_CFG);

	/* parity calculation for write, syndrome calculation for read.*/
	(mode == NAND_ECC_WRITE) ? (ecc_gen_cfg_val |= PRTY_CALC) : (ecc_gen_cfg_val &= SYNDROME_CALC);

#if defined (CONFIG_NAND_COMCERTO_ECC_8_HW_BCH) || defined (CONFIG_NAND_COMCERTO_ECC_24_HW_BCH)
	ecc_gen_cfg_val &= BCH_MODE;
	ecc_gen_cfg_val = (ecc_gen_cfg_val & ~(ECC_LVL_MASK)) | (ECC_LVL_VAL << ECC_LVL_SHIFT);
#else
	ecc_gen_cfg_val |= HAMM_MODE;
#endif

	ecc_gen_cfg_val = (ecc_gen_cfg_val & ~(BLK_SIZE_MASK)) | nand_device->ecc.size; ;

	writel(ecc_gen_cfg_val, ECC_GEN_CFG);
	/* Reset parity module and latch configured values */
	writel(ECC_INIT, ECC_INIT_CFG);
	comcerto_ecc_shift(ECC_SHIFT_ENABLE);
	return;
}

/** writes ECC bytes generated by the parity module into the flash
 *
 * @param[in] mtd	MTD device structure
 * @param[in] dat	raw data
 * @param[in] ecc_code	buffer for ECC
 *
 */
static int comcerto_calculate_ecc(struct mtd_info *mtd,
				  const uint8_t *dat,
				  uint8_t *ecc_code)
{
	struct nand_chip *nand_device = mtd->priv;
	uint32_t ecc_bytes = nand_device->ecc.bytes;
	uint8_t dummy_var = 0xFF;

	while (!(readl(ECC_IDLE_STAT)))
		;

	writel(ECC_PARITY_OUT_EN, ECC_PRTY_OUT_SEL_CFG);

	/* Even though we do a dummy write to NAND flash, actual ECC bytes are
	 * written to the ECC location in the flash. */
	for ( ; ecc_bytes; ecc_bytes--)
		writeb(dummy_var, nand_device->IO_ADDR_W);

	comcerto_ecc_shift(ECC_SHIFT_DISABLE);
	writel(ECC_PARITY_OUT_DISABLE, ECC_PRTY_OUT_SEL_CFG);

	return 0;
}

/** Checks ECC registers for errors and will correct them, if correctable
 *
 * @param[in] mtd	MTD device structure
 * @param[in] dat	raw data
 * @param[in] read_ecc  ECC read out from flash
 * @param[in] calc_ecc	ECC calculated over the raw data
 *
 */
static int comcerto_correct_ecc(struct mtd_info *mtd, uint8_t *dat,
		uint8_t *read_ecc, uint8_t *calc_ecc)
{
#if !defined (CONFIG_NAND_COMCERTO_ECC_8_HW_BCH) && !defined (CONFIG_NAND_COMCERTO_ECC_24_HW_BCH)
	struct nand_chip *nand_device = mtd->priv;
#else
	uint8_t err_count = 0;
	uint32_t err_corr_data_prev;
#endif
	uint32_t err_corr_data;
	uint16_t mask, index;
	uint64_t start = get_time_ns();

	 /* Wait for syndrome calculation to complete */
	do {
		if ((readl(ECC_IDLE_STAT)) & ECC_IDLE)
			break;

	} while (!is_timeout(start, SECOND * 2));

	 /* If no correction is required */
	if (likely(!(readl(ECC_POLY_STAT) & ECC_CORR_REQ))) {
		return 0;
	}

	/* Error found! Correction required */
#if defined (CONFIG_NAND_COMCERTO_ECC_8_HW_BCH) || defined (CONFIG_NAND_COMCERTO_ECC_24_HW_BCH)
	dev_dbg(mtd->dev, "ECC BCH Errors found!\n");

	/* Initiate correction operation */
	writel(ECC_POLY_START, ECC_POLY_START_CFG);

	err_corr_data_prev = 0;

	/* Read Correction data status register till header is 0x7FD */
	do {
		err_corr_data_prev = readl(ECC_CORR_DATA_STAT);
		if ((err_corr_data_prev >> ECC_BCH_INDEX_SHIFT) == 0x87FD)
			break;
		udelay(1);
		dev_dbg(mtd->dev, "Polling ECC_CORR_DATA_STAT!!!! \n");
	} while (!is_timeout(start, SECOND * 2));

	/* start reading error locations */
	err_corr_data = 0x0;

	while (((err_corr_data >> 16) !=  0x87FE)) {
		err_corr_data = readl(ECC_CORR_DATA_STAT);
		if ((err_corr_data >> 16) ==  0x87FE)
			break;
		if (err_corr_data == err_corr_data_prev)
			continue;
		err_corr_data_prev = err_corr_data;
		index = (err_corr_data >> 16) & 0x7FF;
		mask = err_corr_data & 0xFFFF;
		*((uint16_t *)(dat + (index * 2))) ^= mask;
		while (mask) {
			if (mask & 1)
				err_count++;
			mask >>= 1;
		}
	}

	if (!((readl(ECC_CORR_DONE_STAT)) & ECC_DONE)) {
		dev_dbg(mtd->dev, "ECC Correction Not done!!!! \n");
		dev_dbg(mtd->dev, "No. of errors %d \n", err_count);
		return -1;
	}

	if ((readl(ECC_CORR_STAT)) & ECC_UNCORR) {
		dev_dbg(mtd->dev, "ECC Uncorrectable Errors found!!!! \n");
		return -EIO;
	}

	return 0;

#else	/* Hamming Mode */
	if (readl(ECC_POLY_STAT) == ECC_UNCORR_ERR_HAMM) {
		dev_dbg(mtd->dev, "ECC Uncorrectable Errors found!!!! \n");
		/* 2 or more errors detected and hence cannot
		be corrected */
		return -1; /* uncorrectable */
	} else {  /* 1-bit correctable error */
		dev_dbg(mtd->dev, "ECC Correctable Errors found!!!! \n");
		err_corr_data = readl(ECC_CORR_DATA_STAT);
		index = (err_corr_data >> 16) & 0x1FF;

		if (nand_device->options & NAND_BUSWIDTH_16) {
			mask = 1 << (err_corr_data & 0xF);
			*((uint16_t *)(dat + index)) ^= mask;
		} else {
			mask = 1 << (err_corr_data & 0x7);
			dev_dbg(mtd->dev, "index = x%x , mask = x%x \n", index, mask);
			*(dat + index) ^= mask;
		}

		return 1;

	}
#endif
	return 0;
}

/** writes single page to the NAND device along with the ECC bytes
 *
 * @param[in] mtd	MTD device structure
 * @param[in] chip      nand chip info structure
 * @param[in] buf	data buffer
 *
 */
static void comcerto_nand_write_page_hwecc(struct mtd_info *mtd,
					struct nand_chip *chip,
					const uint8_t *buf)
{
	int i, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	const uint8_t *p = buf;
	uint8_t *oob = chip->oob_poi;

	/* CS4 will have the option for ECC calculation */
	writel(ECC_CS4_SEL, ECC_CS_SEL_CFG);

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {

		chip->ecc.hwctl(mtd, NAND_ECC_WRITE);
		chip->write_buf(mtd, p, eccsize);

		chip->ecc.calculate(mtd, p, oob);
		oob += eccbytes;

		if (chip->ecc.postpad) {
			chip->write_buf(mtd, oob, chip->ecc.postpad);
			oob += chip->ecc.postpad;
		}
	}

	/* Calculate remaining oob bytes */
	i = mtd->oobsize - (oob - chip->oob_poi);
	if (i)
		chip->write_buf(mtd, oob, i);
}

/** reads single page from the NAND device and will read ECC bytes from flash. A
 * function call to comcerto_correct_ecc() will be used to validate the data.
 *
 * @param[in] mtd	MTD device structure
 * @param[in] chip      nand chip info structure
 * @param[in] buf	data buffer
 *
 */
static int comcerto_nand_read_page_hwecc(struct mtd_info *mtd,
		struct nand_chip *chip, uint8_t *buf)
{
	struct nand_chip *nand_device = mtd->priv;
	int i, eccsize = nand_device->ecc.size;
	int eccbytes = nand_device->ecc.bytes;
	int eccsteps = nand_device->ecc.steps;
	uint8_t *p = buf;
	uint8_t *ecc_code = nand_device->buffers->ecccode;
	uint8_t ecc_bytes = nand_device->ecc.bytes;
	uint8_t stat;
	uint8_t *oob = nand_device->oob_poi;

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		chip->ecc.hwctl(mtd, NAND_ECC_READ);
		chip->read_buf(mtd, p, eccsize);
		chip->read_buf(mtd, ecc_code, ecc_bytes);

		stat = chip->ecc.correct(mtd, p, oob, NULL);
		if (stat < 0)
			mtd->ecc_stats.failed++;
		else
			mtd->ecc_stats.corrected += stat;

		comcerto_ecc_shift(ECC_SHIFT_DISABLE);

		if (chip->ecc.postpad) {
			chip->read_buf(mtd, oob, chip->ecc.postpad);
			oob += chip->ecc.postpad;
		}
	}
	/* Calculate remaining oob bytes */
	i = mtd->oobsize - (oob - chip->oob_poi);
	if (i)
		chip->read_buf(mtd, oob, i);

	return 0;
}

#ifdef CONFIG_COMCERTO_NAND_ERASE_FBB
/* Erase False Bad Blocks created due to the difference in ECC layouts */
int erase_old_nand_fmt(struct mtd_info *mtd, u8 bb_old_layout)
{

	struct nand_chip *nand_device = mtd->priv;

	printf("erase_old_nand_fmt: Erasing old nand format\n");

	if (bb_old_layout == 0) {

	static struct nand_bbt_descr c2000_badblock_pattern_old = {
		.offs = 14,
		.len = 1,
		.pattern = scan_ff_pattern
	};

	nand_device->badblock_pattern = &c2000_badblock_pattern_old;
	} else {

		memset(&nand_device->ecc, 0, sizeof(struct nand_ecc_ctrl));
		nand_device->ecc.mode = NAND_ECC_SOFT_BCH;

		/* Scan to find existence of the device */
		if (nand_scan_ident(mtd, 1)) {
			printf("nand_scan_ident failed \n");
			return -1;
		}
	}

	/* second phase scan */
	if (nand_scan_tail(mtd)) {
		printf("nand_scan_tail failed \n");
	}
	return 0;
}
#endif /* CONFIG_COMCERTO_NAND_ERASE_FBB */


/** Hardware specific access to NAND control-lines
 *
 * @param[in] mtd	MTD device structure
 * @param[in] cmd	NAND command to the controller
 * @param[in] ctrl	control address/command latch
 *
 */
static void comcerto_nand_hwcontrol(struct mtd_info *mtd, int cmd,
						unsigned int ctrl)
{
	struct nand_chip *chip = mtd->priv;


	if (ctrl & NAND_CTRL_CHANGE) {
		if (ctrl & NAND_NCE)
			comcerto_gpio_set_0(COMCERTO_NAND_CE);
		else
			comcerto_gpio_set_1(COMCERTO_NAND_CE);
	}

	if (cmd == NAND_CMD_NONE)
		return;

	if (ctrl & NAND_CLE)
		writeb(cmd, chip->IO_ADDR_W + COMCERTO_NAND_CLE);
	else if (ctrl & NAND_ALE)
		writeb(cmd,  chip->IO_ADDR_W + COMCERTO_NAND_ALE);
	else
		return;
}

/** Checks whether the NAND flash is ready or not
 *
 * @param[in] mtd	MTD device structure
 *
 */
static int comcerto_nand_ready(struct mtd_info *mtd)
{
	return comcerto_gpio_read(COMCERTO_NAND_RDY) ? 1 : 0;
}

/**
 * nand_read_page_raw_syndrome - [INTERN] read raw page data without ecc
 * @mtd: mtd info structure
 * @chip: nand chip info structure
 * @buf: buffer to store read data
 * @page: page number to read
 *
 * We need a special oob layout and handling even when OOB isn't used.
 */
static int nand_read_page_raw_syndrome(struct mtd_info *mtd,
                                        struct nand_chip *chip,
                                        uint8_t *buf, int page)
{
        int eccsize = chip->ecc.size;
        int eccbytes = chip->ecc.bytes;
        uint8_t *oob = chip->oob_poi;
        int steps, size;

        for (steps = chip->ecc.steps; steps > 0; steps--) {
                chip->read_buf(mtd, buf, eccsize);
                buf += eccsize;

                if (chip->ecc.prepad) {
                        chip->read_buf(mtd, oob, chip->ecc.prepad);
                        oob += chip->ecc.prepad;
                }

                chip->read_buf(mtd, oob, eccbytes);
                oob += eccbytes;

                if (chip->ecc.postpad) {
                        chip->read_buf(mtd, oob, chip->ecc.postpad);
                        oob += chip->ecc.postpad;
                }
        }

        size = mtd->oobsize - (oob - chip->oob_poi);
        if (size)
                chip->read_buf(mtd, oob, size);

        return 0;
}

/** Probes for NAND device on comcerto board.
 *
 * @param[in,out] pdev describes the NAND device in the system
 *
 */
static int comcerto_nand_probe(struct device_d *pdev)
{
	struct comcerto_nand_info *info;
	struct mtd_info *mtd;
	struct nand_chip *nand_device;
	int err = 0;

	printf("nand_probe: %s base: 0x%08x size: 0x%s\n", pdev->name, pdev->map_base, size_human_readable(pdev->size));

	/* Allocate memory for info structure */
	info = malloc(sizeof(struct comcerto_nand_info));
	if (!info) {
		err = -ENOMEM;
		goto out;
	}
	memset(info, 0, sizeof(struct comcerto_nand_info));

	/* Allocate memory for MTD device structure */
	mtd = malloc(sizeof(struct mtd_info));
	if (!mtd) {
		err = -ENOMEM;
		goto out_info;
	}
	memset(mtd, 0, sizeof(struct mtd_info));

	/* Link info's mtd data with the initialized MTD structure */
	info->mtd = mtd;

	/* Allocate pointer to nand_device data */
	nand_device = malloc(sizeof(struct nand_chip));
	if (!nand_device) {
		err = -ENOMEM;
		goto out_mtd;
	}
	memset(nand_device, 0, sizeof(struct nand_chip));

	/* Link the private data with the MTD structure */
	mtd->priv = nand_device;

	nand_device->IO_ADDR_R = (void __iomem *) pdev->map_base;
	if (nand_device->IO_ADDR_R == NULL) {
		err = -EIO;
		goto out_nand;
	}

	/* This is the same address to read and write */
	nand_device->IO_ADDR_W = nand_device->IO_ADDR_R;


	/* Set address of hardware control function */
	nand_device->cmd_ctrl = comcerto_nand_hwcontrol;
	nand_device->dev_ready = comcerto_nand_ready;

	/* 20 us command delay time */
	nand_device->chip_delay = 20;

//Patch by QNAP: enable NAND hw ecc
#if 1
	nand_device->ecc.mode = NAND_ECC_HW_SYNDROME;
#else
	nand_device->ecc.mode = NAND_ECC_SOFT_BCH;
#endif

#if defined(CONFIG_MACH_COMCERTO_C2K_ASIC) && defined(CONFIG_NAND_TYPE_SLC)
	nand_device->options = NAND_BUSWIDTH_16;
#else
	nand_device->options = 0;
#endif
	/* Scan to find existence of the device */
	if (nand_scan_ident(mtd, 1)) {
		err = -ENXIO;
		goto out_nand;
	}

	if (nand_device->ecc.mode == NAND_ECC_HW_SYNDROME) {

		nand_device->ecc.hwctl = comcerto_enable_hw_ecc;
		nand_device->ecc.write_page = comcerto_nand_write_page_hwecc;
		nand_device->ecc.read_page = comcerto_nand_read_page_hwecc;
		nand_device->ecc.calculate = comcerto_calculate_ecc;
		nand_device->ecc.correct = comcerto_correct_ecc;
		nand_device->ecc.read_page_raw = nand_read_page_raw_syndrome;

		switch (mtd->writesize) {
		case 512:
			nand_device->ecc.size = mtd->writesize;
#if defined (CONFIG_NAND_COMCERTO_ECC_24_HW_BCH)
			nand_device->ecc.layout = &comcerto_ecc_info_512_bch;
			nand_device->ecc.bytes = 42;
			nand_device->ecc.prepad = 0;
			nand_device->ecc.postpad = 14;
#elif defined (CONFIG_NAND_COMCERTO_ECC_8_HW_BCH)
			nand_device->ecc.layout = &comcerto_ecc_info_512_bch;
			nand_device->ecc.bytes = 14;
			nand_device->ecc.prepad = 0;
			nand_device->ecc.postpad = 2;
#else /* Hamming */
			nand_device->ecc.layout = &comcerto_ecc_info_512_hamm;
			nand_device->ecc.bytes = 4;
			nand_device->ecc.prepad = 0;
			nand_device->ecc.postpad = 2;
#endif
			break;
		case 1024:
			nand_device->ecc.size = mtd->writesize;
#if defined (CONFIG_NAND_COMCERTO_ECC_24_HW_BCH)
			nand_device->ecc.layout = &comcerto_ecc_info_1024_bch;
			nand_device->ecc.bytes = 42;
			nand_device->ecc.prepad = 0;
			nand_device->ecc.postpad = 14;
#elif defined (CONFIG_NAND_COMCERTO_ECC_8_HW_BCH)
			nand_device->ecc.layout = &comcerto_ecc_info_1024_bch;
			nand_device->ecc.bytes = 14;
			nand_device->ecc.prepad = 0;
			nand_device->ecc.postpad = 18;
#else /* Hamming */
			nand_device->ecc.layout = &comcerto_ecc_info_1024_hamm;
			nand_device->ecc.bytes = 4;
			nand_device->ecc.prepad = 0;
			nand_device->ecc.postpad = 18;
#endif
			break;
		default:
			printf("Using default values \n");
			nand_device->ecc.size =  1024;
#if defined (CONFIG_NAND_COMCERTO_ECC_24_HW_BCH)
			nand_device->ecc.layout = &comcerto_ecc_info_1024_bch;
			nand_device->ecc.bytes = 42;
			nand_device->ecc.prepad = 0;
			nand_device->ecc.postpad = 14;
#elif defined (CONFIG_NAND_COMCERTO_ECC_8_HW_BCH)
			nand_device->ecc.layout = &comcerto_ecc_info_1024_bch;
			nand_device->ecc.bytes = 14;
			nand_device->ecc.prepad = 0;
			nand_device->ecc.postpad = 18;
#else /* Hamming */
			nand_device->ecc.layout = &comcerto_ecc_info_1024_hamm;
			nand_device->ecc.bytes = 4;
			nand_device->ecc.prepad = 0;
			nand_device->ecc.postpad = 18;
#endif
			break;
		}

	nand_device->ecc.steps = mtd->writesize / nand_device->ecc.size;

	if(nand_device->ecc.steps * nand_device->ecc.size != mtd->writesize) {
		printk(KERN_WARNING "Invalid ecc parameters\n");
		BUG();
	}

	nand_device->ecc.total = nand_device->ecc.steps * nand_device->ecc.bytes;

	nand_device->badblock_pattern = &c2000_badblock_pattern;

#ifndef CONFIG_COMCERTO_NAND_ULOADER
	nand_device->bbt_td = &bbt_main_descr;
	nand_device->bbt_md = &bbt_mirror_descr;
	/* update flash based bbt */
	nand_device->options |= NAND_USE_FLASH_BBT;
#endif /* CONFIG_COMCERTO_NAND_ULOADER */
	}

#ifdef CONFIG_COMCERTO_NAND_BBT /* Default: Turned OFF */
/* code under this flag is intended to be used only when
IBR NAND Boot is functional. */
	nand_device->scan_bbt = comcerto_scan_bbt;
#endif /* CONFIG_COMCERTO_NAND_BBT */

#ifndef CONFIG_COMCERTO_NAND_ULOADER
	nand_init_ecc_hw_syndrome(nand_device);
#endif

	nand_device->options |= NAND_NO_SUBPAGE_WRITE;

	/* second phase scan */
	if (nand_scan_tail(mtd)) {
		err = -ENXIO;
		goto out_nand;
	}

#ifdef CONFIG_COMCERTO_NAND_BBT /* Default: Turned OFF */
/* code under this flag is intended to be used only when
IBR NAND Boot is functional. */
	mtd->block_isbad = comcerto_block_isbad;
#endif /* CONFIG_COMCERTO_NAND_BBT */

	/*Name of the mtd device */
	mtd->name = pdev->name;
	mtd->dev = pdev;

	add_mtd_device(mtd);

	err = 0;

	goto out;

out_nand:
	kfree(nand_device);
out_mtd:
	kfree(mtd);
out_info:
	kfree(info);
out:
	return err;

}

/*
 * Driver Registration
 */

static struct driver_d comcerto_nand_driver = {
	.name = "comcerto_nand",
	.probe = comcerto_nand_probe,
};

static int comcerto_nand_drv_init(void)
{
	return register_driver(&comcerto_nand_driver);
}

device_initcall(comcerto_nand_drv_init);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Board-specific glue layer for NAND flash on Comcerto \
								board");
