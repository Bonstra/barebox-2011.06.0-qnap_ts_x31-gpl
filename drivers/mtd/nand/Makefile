
# Generic NAND options
obj-$(CONFIG_NAND)			+= nand.o nand_ecc.o
obj-$(CONFIG_NAND_WRITE)		+= nand_write.o
obj-$(CONFIG_NAND_ECC_SOFT)		+= nand_ecc.o nand_swecc.o
obj-$(CONFIG_MTD_NAND_ECC_BCH)		+= nand_bch.o
obj-$(CONFIG_NAND_ECC_HW)		+= nand_hwecc.o
#ifndef CONFIG_COMCERTO_NAND_ULOADER
obj-$(CONFIG_NAND_ECC_HW_SYNDROME)	+= nand_hwecc_syndrome.o
#endif
obj-$(CONFIG_MTD_NAND_IDS)		+= nand_ids.o
obj-$(CONFIG_NAND)			+= nand_base.o nand-bb.o
obj-$(CONFIG_NAND_BBT)			+= nand_bbt.o

obj-$(CONFIG_MTD_NAND_DISKONCHIP)	+= diskonchip.o
obj-$(CONFIG_MTD_NAND_NOMADIK)		+= nomadik_nand.o
obj-$(CONFIG_NAND_IMX)			+= nand_imx.o
obj-$(CONFIG_NAND_OMAP_GPMC)		+= nand_omap_gpmc.o nand_omap_bch_decoder.o
obj-$(CONFIG_NAND_ATMEL)		+= atmel_nand.o
obj-$(CONFIG_NAND_S3C24X0)		+= nand_s3c2410.o
obj-$(CONFIG_NAND_COMCERTO)		+= comcerto_nand.o
