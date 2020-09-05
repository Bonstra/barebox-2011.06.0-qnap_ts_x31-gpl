#ifndef _TZ_H
#define _TZ_H

#define TZ_AHB_BASE	0X93000000

/*
 * Register Addresss Map for TrustZone Configuration Block
 */
#define TZ_MST_CFG		(TZ_AHB_BASE + 0x0)
#define TZ_SLV_CFG		(TZ_AHB_BASE + 0x4)
#define TZ_AABL_CFG		(TZ_AHB_BASE + 0x8)
#define TZ_AABH_CFG		(TZ_AHB_BASE + 0xC)
#define TZ_DDR_MASK_CFG		(TZ_AHB_BASE + 0x10)
#define TZ_DDR_THRESH_CFG	(TZ_AHB_BASE + 0x14)
#define TZ_IRAM_THRESH_CFG	(TZ_AHB_BASE + 0x18)
#define TZ_EXP_THRESH_CFG	(TZ_AHB_BASE + 0x1C)
#define	TZ_INT_RAW_STAT		(TZ_AHB_BASE + 0x20)
#define TZ_INT_STAT		(TZ_AHB_BASE + 0x24)
#define TZ_INT_EN		(TZ_AHB_BASE + 0x28)
#define TZ_INT_CLR		(TZ_AHB_BASE + 0x2C)

/*
 * TrustZone DDR Mask Configuration Register
 * Default value is 0x3F => size of external DDR is 2GB
 * Possible values are
 * 0x3F-2G, 0x1F-1G, 0xF-512M, 0x7-256M, 0x3-128M, 0x1-64M, 0x0-32M
 */
#define TZ_CFG_DDR_MASK_2G	0x3F
#define TZ_CFG_DDR_MASK_1G	0x1F
#define TZ_CFG_DDR_MASK_512M	0x0F
#define TZ_CFG_DDR_MASK_256M	0x07
#define TZ_CFG_DDR_MASK_128M	0x03
#define TZ_CFG_DDR_MASK_64M	0x01
#define TZ_CFG_DDR_MASK_32M	0x00

/*
 * TrustZone DDR Threshold Configuration Register
 * Minimum address space is 1MB, therefore, configures the upper 11 bits
 * of the address (bits [30:20]).
 */
#define TZ_CFG_DDR_HIGH_THR_MASK	0x7FF0000
#define TZ_CFG_DDR_LOW_THR_MASK		0x7FF

#define TZ_CFG_DDR_HIGH_THR_SHIFT	16
/*
 * TrustZone IRAM Threshold Configuration Register
 * Minimum address space is 8KB, therefore, configures the upper 3 bits
 * of the address (bits [15:13]).
 */
#define TZ_CFG_IRAM_HIGH_THR_MASK	0x70000
#define TZ_CFG_IRAM_LOW_THR_MASK	0x7

#define TZ_CFG_IRAM_HIGH_THR_SHIFT	16
/*
 * TrustZone EXP Threshold Configuration Register
 * Minimum address space is 1MB, therefore, configures the upper 8 bits
 * of the address (bits [27:20]).
 */
#define TZ_CFG_EXP_HIGH_THR_MASK	0xFF0000
#define TZ_CFG_EXP_LOW_THR_MASK		0xFF

#define TZ_CFG_EXP_HIGH_THR_SHIFT	16
/*
 * Define Max values for Masters and Slaves
 */
#define MAX_TZ_AXI_AHB_MASTERS	17
#define MAX_TZ_FT_SLAVES	(15-2) /* Removed PCIe slave addr space */
#define MAX_TZ_PT_SLAVES	3
#define MAX_TZ_APB_SLAVES	(18)

/*
 * AXI/AHB Masters
 */
typedef enum {
	hfe0_axi_master = 0,
	hfe1_axi_master,
	spacc_axi_master,
	espah_axi_master,
	pcie0_axi_master,
	pcie1_axi_master,
	sata_axi_master,
	dmac_axi_master,
	usb3_axi_master,
	a9_dbg_ahb_master,
	dpi0_ahb_master,
	dpi1_ahb_master,
	dpi_desc_ahb_master,
	dect_ahb_master,
	usb2_ahb_master,
	uart_spi_ahb_master,
	tdma_ahb_master	/* 16 */

} axi_ahb_master;

/*
 * A9 AXI Masters
 */
typedef enum {
	A9_m0_axi_master = 0,
	A9_m1_axi_master
} a9_axi_master;

/*
 * Fully Trusted Slaves
 */
typedef enum {
	dpi0_cfg_ahb_slave = 0,
	dpi1_cfg_ahb_slave,
	dect_cfg_ahb_slave,
	ddr_cfg_ahb_slave,
	tz_cfg_ahb_slave,
	uart_spi_cfg_ahb_slave,
	hfe_cfg_axi_slave,
	spacc_cfg_axi_slave,
	espah_cfg_axi_slave,
	pcie0_cfg_axi_slave,
	pcie1_cfg_axi_slave,
	sata_cfg_axi_slave,
	usb3_cfg_axi_slave,
/*	acp_axi_slave = 13, *//* cannot be tested by A9 master. Will be tested
later with other masters */
	pcie0_axi_slave,
	pcie1_axi_slave
} ft_slave;

/*
 * Partially Trusted Slaves
 */
typedef enum {
	ddr_axi_slave = 0,
	exp_ahb_slave,
	iram_axi_slave
} pt_slave;

/*
 * APB Slaves
 */
typedef enum {
	tdm_apb_slave = 0,
	usb_phy_apb_slave,
	tdma_apb_slave,
	timer_apb_slave,
	psu_apb_slave,
	gpio_apb_slave,
	uart_apb_slave,
	usb3_apb_slave,
	clk_apb_slave,
	rtc_apb_slave,
	otp_apb_slave,
	hfe_wrapper_apb_slave,
	serdes_apb_slave,
	exp_apb_slave,
	ddr_phy_apb_slave,
	tdma2_apb_slave,
	dmac_apb_slave,
	a9_dbg_slave
} apb_slave;

typedef enum {
	tz_state_nt = 0,/* Non-Trusted = 0 */
	tz_state_t	/*     Trusted = 1 */
} tz_state;

/*
 * Every SMC call will switch from the existing security state of the ARM AXI
 * master to the other. Secure <---> Non-Secure.
 */
static inline void tz_switch_trust_state(void)
{
	asm(
	".arch_extension sec\n"
	"smc    #0      @ switch to secure world\n"
	);
}

struct slave {
	u8 tz_cfg_bit_shift;
	u32 address;
};

struct pt_slave {
	u8  tz_cfg_bit_shift;
	u32 address;
	u16 upper_thr;
	u16 lower_thr;
};

extern void tz_mon_init(void);

#endif /* _TZ_H */
