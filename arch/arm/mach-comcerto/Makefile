# Object file lists.

obj-y := c2k_start.o clocksource.o lowlevel.o reset.o clkcore.o pad_config.o nand.o gpio.o
#ifndef CONFIG_COMCERTO_NAND_ULOADER
#obj-y += nor.o
#endif
obj-$(CONFIG_COMCERTO_DDR) += ddr.o
obj-$(CONFIG_COMCERTO_DDR_ECC) += mdma.o
obj-$(CONFIG_COMCERTO_SERDES) += serdes.o
obj-$(CONFIG_COMCERTO_NOT_BOOLOADER) += pfe.o
obj-$(CONFIG_COMCERTO_DDR_TRAINING) += training.o


