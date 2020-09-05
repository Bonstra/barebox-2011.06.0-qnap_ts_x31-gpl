deps_config := \
	lib/lzo/Kconfig \
	lib/Kconfig \
	fs/fat/Kconfig \
	fs/Kconfig \
	drivers/otp/Kconfig \
	drivers/led/Kconfig \
	drivers/mfd/Kconfig \
	drivers/clk/Kconfig \
	drivers/mci/Kconfig \
	drivers/video/Kconfig \
	drivers/usb/gadget/Kconfig \
	drivers/usb/otg/Kconfig \
	drivers/usb/host/Kconfig \
	drivers/usb/Kconfig \
	drivers/ata/Kconfig \
	drivers/mtd/ubi/Kconfig \
	drivers/mtd/nand/Kconfig \
	drivers/mtd/Kconfig \
	drivers/nor/Kconfig \
	drivers/i2c/busses/Kconfig \
	drivers/i2c/Kconfig \
	drivers/spi/Kconfig \
	drivers/net/comcerto/Kconfig \
	drivers/net/usb/Kconfig \
	drivers/net/Kconfig \
	drivers/serial/Kconfig \
	drivers/Kconfig \
	net/Kconfig \
	commands/Kconfig \
	common/Kconfig \
	arch/arm/boards/versatile/Kconfig \
	arch/arm/mach-versatile/Kconfig \
	arch/arm/mach-s3c24xx/Kconfig \
	arch/arm/mach-omap/Kconfig \
	arch/arm/mach-nomadik/Kconfig \
	arch/arm/mach-netx/Kconfig \
	arch/arm/mach-mxs/Kconfig \
	arch/arm/mach-imx/Kconfig \
	arch/arm/mach-ep93xx/Kconfig \
	arch/arm/mach-at91/Kconfig \
	arch/arm/cpu/Kconfig \
	diags/Kconfig \
	arch/arm/mach-comcerto/Kconfig \
	arch/arm/Kconfig \
	Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(KERNELVERSION)" "2011.06.0"
include/config/auto.conf: FORCE
endif
ifneq "$(ARCH)" "arm"
include/config/auto.conf: FORCE
endif
ifneq "$(SRCARCH)" "arm"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
